#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include <stdexcept>

/**
 * @brief Ensures that a TType class has only one instance and provides access to it.
 *
 * Inherit from this class to make TType a singleton. The inheriting class should:
 * - Declare a private constructor to prevent direct instantiation.
 * - Declare `friend class Singleton<TType>` to allow Singleton to call the private constructor.
 *
 * @tparam TType Type to manage as a singleton.
 *
 * @example
 * @code
 * class GameManager : public Singleton<GameManager> {
 *     friend class Singleton<GameManager>;
 * private:
 *     GameManager(int score) : score(score) {}
 * public:
 *     int score;
 * };
 *
 * GameManager gm;
 * gm.instantiate(0);
 * gm.instance()->score = 100;
 * @endcode
 */
template <typename TType>
class Singleton {
public:
    /**
     * @brief Returns the managed instance of TType.
     * @return Pointer to the singleton instance.
     * @throws std::runtime_error if instantiate() has not been called yet.
     */
    TType *instance() {
        if (!m_instance)
            throw std::runtime_error("Singleton: Instance does not exists");
        return m_instance;
    }

    /**
     * @brief Creates the managed instance of TType using the provided arguments.
     *
     * @tparam TArgs Constructor argument types.
     * @param p_args Arguments forwarded to TType's constructor.
     * @throws std::runtime_error if the instance has already been created.
     */
    template<typename ... TArgs>
    void instantiate(TArgs&& ... p_args) {
        if (m_instance)
            throw std::runtime_error("Singleton: Instance already exists");
        m_instance = new TType(std::forward<TArgs>(p_args)...);
    }

    /** @brief Destroys the managed instance. */
    ~Singleton() { delete m_instance; }

private:
    TType *m_instance = nullptr;
};

#endif
