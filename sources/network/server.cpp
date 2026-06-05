#include "../../includes/network/net_utils.hpp"
#include "../../includes/network/server.hpp"
#include <bits/types/struct_timeval.h>
#include <cstddef>
#include <fcntl.h>
#include <netinet/in.h>
#include <optional>
#include <stdexcept>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

Server::~Server() {
	for (const auto &client: m_clients)
		close(client.second.sock);
	if (m_socket >= 0)
		close(m_socket);
}

void Server::start(const size_t& p_port) {
	struct sockaddr_in serv_addr;

	m_socket = socket(AF_INET, SOCK_STREAM, 0);
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

	int flags = fcntl(m_socket, F_GETFL, 0);
	fcntl(m_socket, F_SETFL, flags | O_NONBLOCK);

	m_port = p_port;
}

void Server::defineAction(const Message::Type& messageType, const std::function<void(long long& clientID, const Message& msg)>& action) {
	m_actions[messageType] = action;
}

void Server::sendTo(const Message& message, long long clientID) {
	auto it = m_clients.find(clientID);
	if (it == m_clients.end())
		throw std::runtime_error("Server: cannot send message, no such client");

	int clientSock = it->second.sock;
	std::vector<std::byte> msg = net::frame(message);
	net::sendAll(clientSock, msg);
}

void Server::sendToArray(const Message& message, const std::vector<long long> &clientIDs) {
	for (long long id: clientIDs)
		sendTo(message, id);
}

void Server::sendToAll(const Message& message) {
	for (const auto &[id, clientState]: m_clients) {
		sendTo(message, id);
	}
}

void Server::update() {
	int maxFD = m_socket;
	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(m_socket, &readfds);

	for (const auto &[id, clientState]: m_clients) {
		int sock = clientState.sock;
		FD_SET(sock, &readfds);
		if (sock > maxFD)
			maxFD = sock;
	}

	struct timeval tv = {0, 0};
	if (::select(maxFD + 1, &readfds, nullptr, nullptr, &tv) < 0) {
		throw std::runtime_error("Server: failed to run select");
	}

	if (FD_ISSET(m_socket, &readfds)) {
		int clientSock;
		while ((clientSock = ::accept(m_socket, nullptr, nullptr)) >= 0)
			m_clients[m_nextClientID++] = ClientState{clientSock, {} };
	}

	for (auto it = m_clients.begin(); it != m_clients.end(); ) {
		long long id = it->first;
		int sock = it->second.sock;
		std::vector<std::byte> &buffer = it->second.buffer;
		if (FD_ISSET(sock, &readfds)) {
			if (!net::recvAvailable(sock, buffer)){
				close(sock);
				it = m_clients.erase(it);
				continue;
			}
			while (auto message = net::unframe(buffer)) {
				auto act = m_actions.find(message->type());
				if (act != m_actions.end())
					act->second(id, message.value());
			}
		}
		it++;
	}
}
