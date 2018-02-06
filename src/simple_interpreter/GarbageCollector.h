#pragma once

#include "Environment.h"
#include <list>
#include <iostream>

class GarbageCollector {
public:
    explicit GarbageCollector(Environment* rootEnv) : Root(rootEnv) {
    }

    void collect(std::list<std::shared_ptr<ValueStruct>>& memory) const {
        std::cout << "[GC] Mark & Sweep started, allocated: " << memory.size() << std::endl;

        mark();
        sweep(memory);

        std::cout << "[GC] Mark & Sweep finished, allocated: " << memory.size() << std::endl;
    }

private:
    void mark() const;
    void sweep(std::list<std::shared_ptr<ValueStruct>>& memory) const;

    void markEnv(Environment* env) const;

    Environment* Root;
};
