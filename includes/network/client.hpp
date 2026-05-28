#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "message.hpp"
#include <functional>
#include <map>
#include <netdb.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

/**
 * @brief TCP client for connecting to a server and exchanging typed messages.
 *
 * Connect to a server, register handlers for specific message types via defineAction,
 * send messages with send, and call update regularly to process incoming messages.
 *
 * @var m_address  Address of the connected server.
 * @var m_port     Port of the connected server.
 * @var m_socket   File descriptor of the TCP socket.
 * @var m_actions  Map of message type to handler function.
 */
class Client {
public:
    /** @brief Establishes a TCP connection to the given address and port.
     *  @throws std::runtime_error if the socket cannot be created, host is unknown, or connection is refused. */
    void connect(const std::string& address, const size_t& port);

    /** @brief Closes the connection to the server. */
    void disconnect();

    /** @brief Registers a handler to be called when a message of the given type is received.
     *  @param messageType The message type to subscribe to.
     *  @param action      Function called with the received message. */
    void defineAction(const Message::Type& messageType, const std::function<void(const Message& msg)>& action);

    /** @brief Serializes and sends a message to the connected server.
     *  @throws std::runtime_error if any part of the send fails. */
    void send(const Message& message);

    /** @brief Processes all messages received since the last call and executes their registered actions. */
    void update();

private:
    std::string m_address;
    size_t m_port = 0;
    int m_socket = -1;
    std::map<Message::Type, std::function<void(const Message&)>> m_actions;
};

#endif
