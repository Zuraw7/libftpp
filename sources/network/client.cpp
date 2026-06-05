#include "../../includes/network/client.hpp"
#include "../../includes/network/net_utils.hpp"
#include <cstddef>
#include <cstring>
#include <stdexcept>
#include <sys/socket.h>
#include <vector>

void Client::connect(const std::string& address, const size_t& port) {
    struct sockaddr_in serv_addr;
    struct hostent *server;

    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket < 0)
        throw std::runtime_error("Client: failed to create socket");

    server = gethostbyname(address.c_str());
    if (server == NULL)
        throw std::runtime_error("Client: no such host: " + address);

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(port);

    if (::connect(m_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        throw std::runtime_error("Client: connection refused");

    m_address = address;
    m_port = port;
}

void Client::disconnect() {
    close(m_socket);
    m_socket = -1;
    m_recvBuffer.clear();
}

void Client::defineAction(const Message::Type& messageType, const std::function<void(const Message& msg)>& action) {
    m_actions[messageType] = action;
}

void Client::send(const Message& message) {

    std::vector<std::byte> msg = net::frame(message);
    if (!net::sendAll(m_socket, msg))
        throw std::runtime_error("Client: failed to send message");
}

void Client::update() {
    if (!net::recvAvailable(m_socket, m_recvBuffer)) {
        disconnect();
        return;
    }

    while (auto msg = net::unframe(m_recvBuffer)) {
        auto it = m_actions.find(msg->type());
        if (it != m_actions.end())
            it->second(msg.value());
    }
}
