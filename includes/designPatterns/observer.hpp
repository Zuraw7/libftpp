#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <functional>
#include <map>
#include <vector>

/**
 * @brief Event system allowing objects to subscribe to and be notified of events.
 *
 * Multiple lambdas can be subscribed to the same event and will all be called
 * when that event is notified.
 *
 * @tparam TEvent Type used to identify events (e.g. enum, std::string, int).
 *
 * @example
 * @code
 * Observer<std::string> obs;
 * obs.subscribe("on_click", []() { std::cout << "clicked!\n"; });
 * obs.notify("on_click");
 * @endcode
 */
template<typename TEvent>
class Observer {
public:
    /**
     * @brief Subscribes a lambda to a specific event.
     *
     * Multiple lambdas can be subscribed to the same event.
     *
     * @param event The event to subscribe to.
     * @param lambda The function to call when the event is notified.
     */
    void subscribe(const TEvent& event, const std::function<void()>& lambda) {
        m_subscriptions[event].push_back(lambda);
    }

    /**
     * @brief Notifies all subscribers of a specific event.
     *
     * Calls all lambdas subscribed to the event in the order they were added.
     *
     * @param event The event to notify.
     */
    void notify(const TEvent& event) {
        for (auto& lambda : m_subscriptions[event])
            lambda();
    }

private:
    std::map<TEvent, std::vector<std::function<void()>>> m_subscriptions;
};

#endif
