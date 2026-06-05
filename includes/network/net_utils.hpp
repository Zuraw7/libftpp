#ifndef NET_UTILS_HPP
#define NET_UTILS_HPP

#include "message.hpp"
#include <cstddef>
#include <optional>
#include <vector>

/**
 * @brief Wire format and the helpers to send and receive messages over it:
 * framing (frame/unframe) converts between a Message and raw bytes, while
 * sendAll/recvAvailable move those bytes over a socket.
 *
 * The wire layout of one message is:
 *
 *     [Message::Type type][size_t payloadSize][payload bytes]
 *
 * The header (type + payloadSize) has a fixed size; the payload length is
 * carried in payloadSize so a byte stream can be split back into messages.
 */
namespace net {

    /// Size in bytes of the fixed message header (type + payloadSize).
    static constexpr size_t headerSize = sizeof(Message::Type) + sizeof(size_t);

    /** @brief Serializes a message into its wire representation (header + payload).
     *  @return A byte vector ready to be sent. */
    std::vector<std::byte> frame(const Message &message);

    /** @brief Extracts one complete message from the front of a receive buffer.
     *
     *  If a full message is present, builds it, erases its bytes from buffer,
     *  and returns it. If the buffer does not yet hold a complete message,
     *  leaves buffer untouched and returns std::nullopt. Call in a loop to
     *  drain all complete messages.
     *
     *  @param buffer Byte stream to consume from (modified in place). */
    std::optional<Message> unframe(std::vector<std::byte> &buffer);

    /** @brief Sends the whole buffer, looping over partial writes.
     *  @return true if all bytes were sent, false on send failure. */
    bool sendAll(int sock, const std::vector<std::byte> &data);

    /** @brief Non-blocking drain of a socket: appends all currently available
     *         bytes to buffer.
     *  @return true if the socket is still alive (including when no data was
     *          available), false if the peer closed the connection or an error
     *          occurred. */
    bool recvAvailable(int sock, std::vector<std::byte> &buffer);

}

#endif
