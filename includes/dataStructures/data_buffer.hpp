#ifndef DATA_BUFFER_HPP
#define DATA_BUFFER_HPP

#include <cstddef>
#include <cstring>
#include <stdexcept>
#include <vector>

/**
 * @brief Polymorphic container for storing and retrieving objects in byte format.
 *
 * Supports serialization via operator<< and deserialization via operator>>.
 * Internally stores data as raw bytes, allowing any trivially copyable type to be pushed and popped.
 */
class DataBuffer {
public:
    /**
     * @brief Serializes an object into the buffer.
     *
     * Appends the raw bytes of value to the end of the buffer.
     *
     * @tparam TArg Type of the object to serialize.
     * @param value Object to serialize.
     * @return Reference to this DataBuffer for chaining.
     */
    template<typename TArg>
    DataBuffer& operator<<(const TArg &value) {
        const std::byte* begin = reinterpret_cast<const std::byte*>(&value);
        m_buffer.insert(m_buffer.end(), begin, begin + sizeof(TArg));
        return *this;
    }

    /**
     * @brief Deserializes an object from the buffer.
     *
     * Reads sizeof(TArg) bytes from the current read position and copies them into value,
     * then advances the read offset.
     *
     * @tparam TArg Type of the object to deserialize.
     * @param value Reference to the object to write the deserialized data into.
     * @return Reference to this DataBuffer for chaining.
     * @throws std::runtime_error if the read offset is beyond the buffer's size.
     */
    template<typename TArg>
    DataBuffer& operator>>(TArg &value) {
        if (m_readOffset >= m_buffer.size())
            throw std::runtime_error("DataBuffer: read out of bounds");
        
        TArg* arg = reinterpret_cast<TArg*>(m_buffer.data() + m_readOffset);
        std::memcpy(&value, arg, sizeof(TArg));
        m_readOffset += sizeof(TArg);
        return *this;
    }

    /** @brief Resets the read offset to the beginning of the buffer. */
    void reset();

    /** @brief Clears all data from the buffer and resets the read offset. */
    void clear();

private:
    std::vector<std::byte> m_buffer;
    size_t m_readOffset = 0;

};

#endif
