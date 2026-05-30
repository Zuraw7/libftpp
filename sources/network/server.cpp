#include "../../includes/network/server.hpp"

Server::~Server() {
	close(m_socket);
}

void Server::start(const size_t& p_port) {
	struct sockaddr_in serv_addr;

	m_socket =  socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket < 0)
		throw std::runtime_error("Server: error opening socket");

	int opt = 1;
	setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;  
	serv_addr.sin_addr.s_addr = INADDR_ANY;  
	serv_addr.sin_port = htons(p_port);

	if (bind(m_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
		throw std::runtime_error("Server: error on binding");

	if (listen(m_socket,5) < 0)
		throw std::runtime_error("Server: error listening");

	m_port = p_port;
}

void Server::defineAction(const Message::Type& messageType, const std::function<void(long long& clientID, const Message& msg)>& action) {
	m_actions[messageType] = action;
}
