#include "../../includes/dataStructures/data_buffer.hpp"

void DataBuffer::reset() {
    m_readOffset = 0;
}

void DataBuffer::clear() {
    m_buffer.clear();
    m_readOffset = 0;
}
