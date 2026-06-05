#ifndef OBSERVABLE_VALUE_HPP
#define OBSERVABLE_VALUE_HPP

#include <functional>
#include <vector>

/**
 * @brief A value that notifies subscribers whenever it changes.
 *
 * Wraps a single value of type TType. Every mutation - through set() or
 * operator=() - invokes all subscribed callbacks, so consumers can react to
 * changes without polling. Reads (get() or the implicit conversion) never
 * notify. Works with any assignable type, primitive or class.
 *
 * @tparam TType The wrapped value type.
 *
 * @code
 * ObservableValue<int> hp = 100;
 * hp.subscribe([&hp] { std::cout << "hp is now " << hp.get() << "\n"; });
 * hp = 80;        // prints: hp is now 80
 * hp.set(60);     // prints: hp is now 60
 * int current = hp;  // implicit read, no notification
 * @endcode
 */
template<typename TType>
class ObservableValue {
public:
	/// @brief Constructs the observable with an initial value.
	ObservableValue(const TType& value) : m_value(value) {}

	/// @brief Assigns a new value and notifies subscribers.
	ObservableValue& operator=(const TType& value) {
		set(value);
		return *this;
	}

	/// @brief Implicit read-only conversion to the wrapped value (no notification).
	operator const TType&() const {
		return get();
	}

	/// @brief Read-only access to the current value.
	const TType& get() const {
		return m_value;
	}

	/// @brief Sets a new value and notifies all subscribers.
	void set(const TType& value) {
		m_value = value;
		notify();
	}

	/// @brief Registers a callback invoked on every change, in subscription order.
	void subscribe(const std::function<void()>& func) {
		m_subscriptions.push_back(func);
	}

private:
	TType m_value;                                       ///< The wrapped value.
	std::vector<std::function<void()>> m_subscriptions;  ///< Callbacks fired on change.

	/// @brief Invokes every subscribed callback.
	void notify() {
		for (auto& func : m_subscriptions)
			func();
	}
};

#endif
