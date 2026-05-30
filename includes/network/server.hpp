#ifndef SERVER_HPP
#define SERVER_HPP

#include "message.hpp"
#include <functional>
#include <map>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

class Server {
public:
	~Server();

	void start(const size_t& p_port);
	void defineAction(const Message::Type& messageType, const std::function<void(long long& clientID, const Message& msg)>& action);
	void sendTo(const Message& message, long long clientID);
	void sendToArray(const Message& message, std::vector<long long> clientIDs);
	void sendToAll(const Message& message);
	void update();
// Jedyna różnica vs Client: Server musi też w update() sprawdzać nowe połączenia (accept z MSG_DONTWAIT lub O_NONBLOCK na głównym sockecie).


private:
	size_t m_port = 0;
	int m_socket = -1;
	// long long - id, int socket	
	std::map<long long, int> m_clients;
	std::map<const Message::Type, std::function<void(long long& clientID, const Message& msg)>> m_actions;

};

#endif
