#ifndef POOL_HPP
#define POOL_HPP

#include <vector>

// TODO: Create doxygen documentation
// Struct containing byte equivalent of TType object
// 	For example if int is passed it would contain 4 byte of memory
template<typename TType>
struct RawSlot {
	alignas(TType) std::byte data[sizeof(TType)];
};

template<typename TType>
class Pool {
public:
	// TODO: constructor and destructor of Pool class?

	class Object {
		public:
			// Should I create constructor?
			~Object();
			// TODO: make it return object's raw pointer 
			TType* operator->();
			// TODO: Should I create release function? Not specified in subject
		private:
			TType *m_ptr;
			Pool *m_pool;
			size_t m_index;
	};

	// TODO: make it safe -> what it resizing cuts existing objects? -> resize removes objects
	// how to manage freeSlots to keep free and add new free's to the list
	void resize(const size_t& numberOfObjectStored) {
		m_storage.resize(numberOfObjectStored);
		m_freeSlots.resize(numberOfObjectStored);
	}

	template<typename ... TArgs>
	Object acquire(TArgs&& ... p_args);

private:
	std::vector<RawSlot<TType>> m_storage;
	std::vector<size_t> m_freeSlots;

};

#endif
