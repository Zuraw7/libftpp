#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "../dataStructures/data_buffer.hpp"
#include <string>

/**
 * @brief Network message container used for client-server communication.
 *
 * A Message carries a type identifier and a binary payload. The type tells
 * the receiver how to interpret the payload. Data is serialized into an
 * internal DataBuffer via operator<< and deserialized via operator>>.
 *
 * Supports trivially copyable types and std::string natively.
 * operator<< and operator>> can be chained.
 *
 * @var m_type      Integer identifying the message type.
 * @var m_msgBuffer Binary payload buffer.
 */
class Message {
public:
    /// Integer alias used as message type - pass your own enum values.
    using Type = int;

    /** @brief Constructs a message with the given type. Payload is empty until data is pushed via <<. */
    Message(int type);

    /** @brief Serializes a trivially copyable value into the payload. */
    template<typename TArg>
    Message& operator<<(const TArg& value) {
        m_msgBuffer << value;
        return *this;
    }

    /** @brief Deserializes a trivially copyable value from the payload. */
    template<typename TArg>
    Message& operator>>(TArg& value) {
        m_msgBuffer >> value;
        return *this;
    }

    /** @brief Serializes a std::string - stores length then bytes. */
    Message& operator<<(const std::string& value);

    /** @brief Deserializes a std::string - reads length then bytes. */
    Message& operator>>(std::string& value);

    /** @brief Returns the message type set in the constructor. */
    int type() const;
    
    /** @brief Clears the payload buffer and resets the read offset. The message type is preserved. */
    void clear();

    /** @brief Returns a copy of the internal payload buffer.*/
    DataBuffer getMessageBuffer() const;

    /** @brief Replaces the payload buffer with raw bytes. */
    void uploadBuffer(const std::vector<std::byte>& data);

private:
    Type m_type;
    DataBuffer m_msgBuffer;
};

#endif
