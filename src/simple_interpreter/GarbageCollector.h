#pragma once

#include "Environment.h"
#include <list>

class GarbageCollector {
public:
    explicit GarbageCollector(Environment* rootEnv) : Root(rootEnv) {
    }

    void collect(std::list<std::shared_ptr<ValueStruct>>& memory) const {
        mark();
        sweep(memory);
    }

private:
    void mark() const;
    void sweep(std::list<std::shared_ptr<ValueStruct>>& memory) const;

    void markEnv(Environment* env) const;

    Environment* Root;
};
