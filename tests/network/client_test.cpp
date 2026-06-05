#include "../test_utils.hpp"
#include "../../includes/network/client.hpp"
#include "../../includes/network/server.hpp"
#include "../../includes/network/message.hpp"
#include <chrono>
#include <functional>
#include <string>
#include <thread>

namespace {
	// Pumps both sides until `done` is true or the iteration budget runs out.
	void pump(Server& server, Client& client,
	          const std::function<bool()>& done, int maxIters = 1000) {
		for (int i = 0; i < maxIters && !done(); ++i) {
			server.update();
			client.update();
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
}

void client_tests() {
	TEST_SECTION("Client");

	const size_t port = 54321;

	Server server;
	server.start(port);

	Client client;
	client.connect("127.0.0.1", port);

	// client -> server: type and payload arrive intact
	{
		int gotType = -1;
		int gotPayload = 0;
		server.defineAction(1, [&](long long&, const Message& m) {
			gotType = m.type();
			Message copy = m;
			copy >> gotPayload;
		});

		Message msg(1);
		msg << int(777);
		client.send(msg);

		pump(server, client, [&] { return gotType != -1; });
		EXPECT(gotType == 1);
		EXPECT(gotPayload == 777);
	}

	// server -> client: string payload round-trips
	{
		int cliType = -1;
		std::string cliText;
		client.defineAction(2, [&](const Message& m) {
			cliType = m.type();
			Message copy = m;
			copy >> cliText;
		});

		Message reply(2);
		reply << std::string("hello");
		server.sendToAll(reply);

		pump(server, client, [&] { return cliType != -1; });
		EXPECT(cliType == 2);
		EXPECT(cliText == "hello");
	}

	// only the action for the received type fires
	{
		bool typeAFired = false;
		bool typeBFired = false;
		client.defineAction(10, [&](const Message&) { typeAFired = true; });
		client.defineAction(11, [&](const Message&) { typeBFired = true; });

		Message msg(10);
		server.sendToAll(msg);

		pump(server, client, [&] { return typeAFired; });
		EXPECT(typeAFired);
		EXPECT(!typeBFired);
	}

	client.disconnect();
}
