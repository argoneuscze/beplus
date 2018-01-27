#pragma once

#include "Environment.h"
#include "Heap.h"

class GarbageCollector {
public:
    explicit GarbageCollector(void) { }

    void mark(Heap & heap, Environment & env);

    void sweep(Heap & heap);
};
