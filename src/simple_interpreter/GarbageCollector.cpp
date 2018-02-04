#include "GarbageCollector.h"

void GarbageCollector::mark() const {
    markEnv(this->Root);
}

void GarbageCollector::sweep(std::list<std::shared_ptr<ValueStruct>>& memory) const {
    auto it = memory.begin();
    while (it != memory.end()) {
        auto valStruct = it->get();
        if (!valStruct->getMark()) {
            memory.erase(it++);
        }
        else {
            valStruct->setMark(false);
            ++it;
        }
    }
}

void GarbageCollector::markEnv(Environment* env) const {
    for (auto& mapEntry : env->getVariables()) {
        auto& var = mapEntry.second;

        if (auto valStruct = dynamic_cast<ValueStruct*>(var.get())) {
            valStruct->setMark(true);
        }
    }

    for (auto& child : env->getChildren()) {
        markEnv(child.get());
    }
}
