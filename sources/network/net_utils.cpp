#include "../../includes/network/net_utils.hpp"
#include <cerrno>
#include <cstddef>
#include <cstring>
#include <optional>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <vector>

namespace net {

    std::vector<std::byte> frame(const Message &message) {

        Message::Type type = message.type();
        DataBuffer buffer = message.getMessageBuffer();
        const std::byte *payload = buffer.data();
        size_t size = buffer.size();

        std::vector<std::byte> msg(sizeof(Message::Type) + sizeof(size_t) + size);
        std::memcpy(msg.data(), &type, sizeof(Message::Type));
        std::memcpy(msg.data() + sizeof(Message::Type), &size, sizeof(size_t));
        if (size > 0)
            std::memcpy(msg.data() + headerSize, payload, size);

        return msg;
    }

    std::optional<Message> unframe(std::vector<std::byte> &buffer) {
        if (buffer.size() < headerSize)
            return std::nullopt;

        Message::Type type;
        size_t size;

        std::memcpy(&type, buffer.data(), sizeof(Message::Type));
        std::memcpy(&size, buffer.data() + sizeof(Message::Type), sizeof(size_t));

        if (buffer.size() < headerSize + size)
            return std::nullopt;

        const std::vector<std::byte> payload(buffer.begin() + headerSize, buffer.begin() + headerSize + size);

        Message message(type);
        message.uploadBuffer(payload);
        buffer.erase(buffer.begin(), buffer.begin() + headerSize + size);
        return message;
    }

    bool sendAll(int sock, const std::vector<std::byte> &data) {
        size_t sent = 0;

        while (sent < data.size()) {
            ssize_t bytesSent = ::send(sock, data.data() + sent, data.size() - sent, MSG_NOSIGNAL);
            if (bytesSent < 0)
                return false;
            sent += bytesSent;
        }
        return true;
    }

    bool recvAvailable(int sock, std::vector<std::byte> &buffer) {
        std::byte tmp[4096];

        while (true) {
            ssize_t r = recv(sock, tmp, sizeof(tmp), MSG_DONTWAIT);
            if (r > 0) {
                buffer.insert(buffer.end(), tmp, tmp + r);
            } else if (r == 0) {
                return false;
            } else {
                if (errno == EWOULDBLOCK || errno == EAGAIN)
                    return true;
                return false;
            }
        }
    }

}
