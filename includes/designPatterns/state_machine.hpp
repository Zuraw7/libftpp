#ifndef STATE_MACHINE_HPP
#define STATE_MACHINE_HPP

#include <functional>
#include <map>
#include <set>
#include <stdexcept>
#include <utility>

template<typename TState>
class StateMachine {
public:
    void addState(const TState& state) {
        m_states.emplace(state);
        if (!m_initialized) {
            m_currentState = state;
            m_initialized = true;
        }
    }

    void addTransition(const TState& startState, const TState& finalState, const std::function<void()>& lambda) {
        std::pair<TState, TState> statePair(startState, finalState);
        m_transitions[statePair] = lambda;
    }

    void addAction(const TState& state, const std::function<void()>& lambda) {
        m_actions[state] = lambda;
    }

    void transitionTo(const TState& state) {
        if (m_states.find(state) == m_states.end())
            throw std::runtime_error("StateMachine: No such state");

        auto it = m_transitions.find({m_currentState, state});
        if (it == m_transitions.end())
            throw std::runtime_error("StateMachine: No transition defined");

        it->second();
        m_currentState = state;
    }

    void update() {
        auto it = m_actions.find(m_currentState);
        if (it == m_actions.end())
            throw std::runtime_error("StateMachine: No action defined");

        it->second();
    }

private:
    bool m_initialized = false;
    TState m_currentState;
    std::set<TState> m_states;
    std::map<TState, std::function<void()>> m_actions;
    std::map<std::pair<TState, TState>, std::function<void()>> m_transitions;

};

#endif
