#ifndef MEMENTO_HPP
#define MEMENTO_HPP

#include "../dataStructures/data_buffer.hpp"

/**
 * @brief Base class enabling save/load state functionality via the Memento pattern.
 *
 * Inherit from this class to make an object "saveable". The inheriting class must
 * implement _saveToSnapshot() and _loadFromSnapshot() as private methods.
 *
 * @example
 * @code
 * class Player : public Memento {
 * public:
 *     int health, x, y;
 * private:
 *     void _saveToSnapshot(Memento::Snapshot& s) const override { s << health << x << y; }
 *     void _loadFromSnapshot(Memento::Snapshot& s) override { s >> health >> x >> y; }
 * };
 * @endcode
 */
class Memento {
public:
    using Snapshot = DataBuffer;

    /**
     * @brief Saves the current state of the object into a Snapshot.
     * @return A Snapshot containing the serialized state.
     */
    Snapshot save();

    /**
     * @brief Restores the object's state from a Snapshot.
     * @param state The Snapshot to restore from.
     */
    void load(const Memento::Snapshot& state);

private:
    /**
     * @brief Serializes the object's data into the snapshot.
     * @param snapshot The Snapshot to write data into.
     */
    virtual void _saveToSnapshot(Memento::Snapshot& snapshot) const = 0;

    /**
     * @brief Deserializes data from the snapshot into the object.
     * @param snapshot The Snapshot to read data from.
     */
    virtual void _loadFromSnapshot(Memento::Snapshot& snapshot) = 0;
};

#endif
