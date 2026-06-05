#include "../test_utils.hpp"
#include "../../includes/network/client.hpp"
#include "../../includes/network/server.hpp"
#include "../../includes/network/message.hpp"
#include <chrono>
#include <functional>
#include <map>
#include <thread>
#include <vector>

namespace {
	void pump(Server& server, const std::vector<Client*>& clients,
	          const std::function<bool()>& done, int maxIters = 1000) {
		for (int i = 0; i < maxIters && !done(); ++i) {
			server.update();
			for (Client* c : clients)
				c->update();
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	enum { REGISTER = 1, PING = 2 };
}

void server_tests() {
	TEST_SECTION("Server");

	const size_t port = 54322;

	Server server;
	server.start(port);

	// Each client announces a label so we can learn its server-side ID.
	std::map<int, long long> idByLabel;
	server.defineAction(REGISTER, [&](long long& id, const Message& m) {
		Message copy = m;
		int label = 0;
		copy >> label;
		idByLabel[label] = id;
	});

	Client a;
	Client b;
	a.connect("127.0.0.1", port);
	b.connect("127.0.0.1", port);

	bool aGot = false;
	bool bGot = false;
	a.defineAction(PING, [&](const Message&) { aGot = true; });
	b.defineAction(PING, [&](const Message&) { bGot = true; });

	// register both clients, capturing unique IDs
	{
		Message ra(REGISTER);
		ra << int(100);
		a.send(ra);
		Message rb(REGISTER);
		rb << int(200);
		b.send(rb);

		pump(server, {&a, &b}, [&] { return idByLabel.size() == 2; });
		EXPECT(idByLabel.size() == 2);
		EXPECT(idByLabel.count(100) == 1);
		EXPECT(idByLabel.count(200) == 1);
		EXPECT(idByLabel[100] != idByLabel[200]);  // unique IDs
	}

	long long idA = idByLabel[100];
	long long idB = idByLabel[200];

	// sendTo reaches only the targeted client
	{
		aGot = false;
		bGot = false;
		server.sendTo(Message(PING), idA);
		pump(server, {&a, &b}, [&] { return aGot; });
		EXPECT(aGot);
		EXPECT(!bGot);
	}

	// sendToArray reaches only the listed clients
	{
		aGot = false;
		bGot = false;
		server.sendToArray(Message(PING), {idB});
		pump(server, {&a, &b}, [&] { return bGot; });
		EXPECT(bGot);
		EXPECT(!aGot);
	}

	// sendToAll reaches everyone
	{
		aGot = false;
		bGot = false;
		server.sendToAll(Message(PING));
		pump(server, {&a, &b}, [&] { return aGot && bGot; });
		EXPECT(aGot);
		EXPECT(bGot);
	}

	// a disconnected client is reaped: sendTo its old ID then throws
	{
		b.disconnect();
		// let the server notice the closed socket
		pump(server, {&a}, [] { return false; }, 100);
		EXPECT_THROW(server.sendTo(Message(PING), idB));
	}

	a.disconnect();
}
