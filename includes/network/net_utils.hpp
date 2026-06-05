#ifndef NET_UTILS_HPP
#define NET_UTILS_HPP

#include "message.hpp"
#include <cstddef>
#include <optional>
#include <vector>

namespace net {

    static constexpr size_t headerSize = sizeof(Message::Type) + sizeof(size_t);

    std::vector<std::byte> frame(const Message &message);
    std::optional<Message> unframe(std::vector<std::byte> &buffer);

    bool sendAll(int sock, const std::vector<std::byte> &data);
    bool recvAvailable(int sock, std::vector<std::byte> &buffer);

}

#endif
