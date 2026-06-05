#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "message.hpp"
#include <cstddef>
#include <functional>
#include <map>
#include <netdb.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

/**
 * @brief TCP client for connecting to a server and exchanging typed messages.
 *
 * Connect to a server, register handlers for specific message types via defineAction,
 * send messages with send, and call update regularly to process incoming messages.
 */
class Client {
public:
    /** @brief Establishes a TCP connection to the given address and port.
     *  @throws std::runtime_error if the socket cannot be created, host is unknown, or connection is refused. */
    void connect(const std::string& address, const size_t& port);

    /** @brief Closes the connection to the server and clears the receive buffer. */
    void disconnect();

    /** @brief Registers a handler to be called when a message of the given type is received.
     *  @param messageType The message type to subscribe to.
     *  @param action      Function called with the received message. */
    void defineAction(const Message::Type& messageType, const std::function<void(const Message& msg)>& action);

    /** @brief Serializes and sends a message to the connected server.
     *  @throws std::runtime_error if any part of the send fails. */
    void send(const Message& message);

    /** @brief Drains all bytes available on the socket, dispatches every complete
     *         message to its registered action, and keeps any partial message
     *         buffered for the next call. Disconnects if the server closed the
     *         connection. */
    void update();

private:
    std::string m_address;  ///< Address of the connected server.
    size_t m_port = 0;      ///< Port of the connected server.
    int m_socket = -1;      ///< File descriptor of the TCP socket.
    std::map<Message::Type, std::function<void(const Message&)>> m_actions;  ///< Message type to handler.
    std::vector<std::byte> m_recvBuffer;  ///< Bytes received but not yet forming complete messages; persists across update calls.
};

#endif
