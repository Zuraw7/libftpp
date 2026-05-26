#ifndef POOL_HPP
#define POOL_HPP

#include <stdexcept>
#include <vector>

/**
 * @brief Raw aligned storage for a single object of type TType.
 * @tparam TType Type whose size and alignment requirements are mirrored.
 */
template<typename TType>
struct RawSlot {
	alignas(TType) std::byte data[sizeof(TType)];
};

/**
 * @brief Memory pool managing a fixed collection of pre-allocated TType objects.
 *
 * Objects are acquired from the pool via acquire() and returned automatically
 * when the returned Pool::Object goes out of scope.
 *
 * @tparam TType Type of objects stored in the pool.
 */
template<typename TType>
class Pool {
public:

	/**
	 * @brief RAII handle to a pre-allocated object inside a Pool.
	 *
	 * Releases the slot back to the pool and calls TType's destructor
	 * when it goes out of scope.
	 */
	class Object {
		public:
			friend class Pool;

			/** @brief Destroys the managed TType object and returns the slot to the pool. */
			~Object() {
				reinterpret_cast<TType *>(m_pool->m_storage[m_index].data)->~TType();
				m_pool->m_freeSlots.push_back(m_index);
			}

			/**
			 * @brief Returns a pointer to the managed TType object.
			 * @return Pointer to the object stored at this slot.
			 */
			TType* operator->() {
				return reinterpret_cast<TType *>(m_pool->m_storage[m_index].data);
			}

		private:
			Object(Pool* pool, size_t index): m_pool(pool), m_index(index) {}
			Pool *m_pool;
			size_t m_index;
	};

	/**
	 * @brief Grows the pool to hold at least numberOfObjectStored objects.
	 *
	 * Newly added slots are immediately available for acquire().
	 * Shrinking is not supported and throws an exception.
	 *
	 * @param numberOfObjectStored Desired total capacity of the pool.
	 * @throws std::runtime_error if numberOfObjectStored is smaller than the current size.
	 */
	void resize(const size_t& numberOfObjectStored) {
		if (numberOfObjectStored < m_storage.size())
			throw std::runtime_error("Can't shrink pool");
		else if (numberOfObjectStored == m_storage.size())
			return;

		size_t prevSize = m_storage.size();
		m_storage.resize(numberOfObjectStored);

		for (size_t i = prevSize; i < numberOfObjectStored; i++)
			m_freeSlots.push_back(i);
	}

	/**
	 * @brief Acquires a free slot and constructs a TType object in place.
	 *
	 * @tparam TArgs Constructor argument types.
	 * @param p_args Arguments forwarded to TType's constructor.
	 * @return Pool::Object owning the newly constructed object.
	 * @throws std::runtime_error if no free slots are available.
	 */
	template<typename ... TArgs>
	Object acquire(TArgs&& ... p_args) {
		if (m_freeSlots.empty())
			throw std::runtime_error("Pool is full");
		size_t index = m_freeSlots.at(m_freeSlots.size() - 1);
		m_freeSlots.pop_back();

		// new (&m_storage.at(index).data) TType(std::forward<TArgs>(p_args)...);
		new (&m_storage.at(index).data) TType(p_args...);
		return Object(this, index);
	}

private:
	std::vector<RawSlot<TType>> m_storage;
	std::vector<size_t> m_freeSlots;

};

#endif
