#include "../../includes/designPatterns/memento.hpp"

Memento::Snapshot Memento::save() {
    Memento::Snapshot snapshot;
    _saveToSnapshot(snapshot);
    return snapshot;
}

void Memento::load(const Memento::Snapshot& snapshot) {
    Memento::Snapshot snap = snapshot;
    _loadFromSnapshot(snap);
}
