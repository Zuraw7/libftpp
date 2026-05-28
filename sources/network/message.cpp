#include "../../includes/network/message.hpp"
#include <cstddef>

Message::Message(int type) : m_type(type) {

}

Message &Message::operator<<(const std::string &value) {
    size_t len = value.size();
    m_msgBuffer << len;
    for (char c: value)
        m_msgBuffer << c;

    return *this;
}

Message &Message::operator>>(std::string &value) {
    size_t len = 0;
    m_msgBuffer >> len;
    value.resize(len);
    for (size_t i = 0; i < len; i++) {
        char c;
        m_msgBuffer >> c;
        value[i] = c;
    }

    return *this;
}

int Message::type() {
    return m_type;
}

void Message::clear() {
    m_msgBuffer.clear();
}
