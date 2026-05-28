#include "../../includes/network/client.hpp"
#include <sys/socket.h>

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
}

void Client::defineAction(const Message::Type& messageType, const std::function<void(const Message& msg)>& action) {
    m_actions[messageType] = action;
}

void Client::send(const Message& message) {
    DataBuffer buffer = message.getMessageBuffer();
    Message::Type type = message.type();
    size_t size = buffer.size();
    const std::byte *payload = buffer.data();

    if (::send(m_socket, &type, sizeof(Message::Type), 0) < 0)
        throw std::runtime_error("Client: send failed");
    if (::send(m_socket, &size, sizeof(size_t), 0) < 0)
        throw std::runtime_error("Client: send failed");
    if (::send(m_socket, payload, size, 0) < 0)
        throw std::runtime_error("Client: send failed");
}

void Client::update() {
    Message::Type type;
    size_t payloadSize;

    while (::recv(m_socket, &type, sizeof(type), MSG_DONTWAIT) > 0) {
        if (::recv(m_socket, &payloadSize, sizeof(payloadSize), 0) <= 0)
            return;

        std::vector<std::byte> payload(payloadSize);
        if (::recv(m_socket, payload.data(), payloadSize, 0) <= 0)
            return;

        Message msg(type);
        msg.uploadBuffer(payload);

        auto it = m_actions.find(type);
        if (it != m_actions.end())
            it->second(msg);
    }
}
