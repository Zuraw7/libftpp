#ifndef SERVER_HPP
#define SERVER_HPP

#include "message.hpp"
#include <cstddef>
#include <functional>
#include <map>
#include <vector>

/**
 * @brief Per-connection state held by the server for one client.
 */
struct ClientState {
	int sock = -1;                  ///< File descriptor of the accepted client socket.
	std::vector<std::byte> buffer;  ///< Bytes received but not yet forming complete messages; persists across update calls.
};

/**
 * @brief TCP server accepting multiple clients and routing typed messages.
 *
 * Start the server with start, register handlers per message type via
 * defineAction, and call update regularly to accept new connections, read
 * incoming messages, and dispatch them. Messages can be sent to one client
 * (sendTo), a subset (sendToArray), or everyone (sendToAll). Each connected
 * client is identified by a unique, monotonically increasing ID.
 */
class Server {
public:
	/** @brief Closes all client sockets and the listening socket. */
	~Server();

	/** @brief Creates, binds and listens on a non-blocking socket at the given port.
	 *  @throws std::runtime_error if the socket cannot be created, bound, or set to listen. */
	void start(const size_t& p_port);

	/** @brief Registers a handler called when a message of the given type is received.
	 *  @param messageType The message type to subscribe to.
	 *  @param action      Function called with the sending client's ID and the message. */
	void defineAction(const Message::Type& messageType, const std::function<void(long long& clientID, const Message& msg)>& action);

	/** @brief Serializes and sends a message to a single client.
	 *  @throws std::runtime_error if no client with the given ID exists.
	 *  @note A send failure on a dropped client is ignored; update() reaps it. */
	void sendTo(const Message& message, long long clientID);

	/** @brief Sends a message to each client ID in the list (see sendTo). */
	void sendToArray(const Message& message, const std::vector<long long> &clientIDs);

	/** @brief Sends a message to every currently connected client. */
	void sendToAll(const Message& message);

	/** @brief Accepts pending connections, drains each ready client's socket,
	 *         dispatches every complete message to its action, keeps partial
	 *         messages buffered, and removes clients that have disconnected. */
	void update();

private:
	long long m_nextClientID = 0;  ///< ID assigned to the next accepted client (never reused).
	size_t m_port = 0;             ///< Port the server is listening on.
	int m_socket = -1;             ///< File descriptor of the listening socket (non-blocking).
	std::map<long long, ClientState> m_clients;  ///< Connected clients keyed by client ID.
	std::map<const Message::Type, std::function<void(long long& clientID, const Message& msg)>> m_actions;  ///< Message type to handler.

};

#endif
