#include "../../includes/dataStructures/data_buffer.hpp"

void DataBuffer::reset() {
    m_readOffset = 0;
}

void DataBuffer::clear() {
    m_buffer.clear();
    m_readOffset = 0;
}

const std::byte *DataBuffer::data() const {
    return m_buffer.data();
}

size_t DataBuffer::size() const {
    return  m_buffer.size();
}

void DataBuffer::setBuffer(const std::vector<std::byte>& data) {
    m_buffer = data;
    m_readOffset = 0;
}
