#pragma once

#include <iostream> // [DBG]
#include <list>
#include "Value.h"
#include "GarbageCollector.h"

static const size_t GC_MAX_HEAP_SIZE = 10;

class Heap {
public:
    void setGarbageCollector(const GarbageCollector* gc) {
        this->GC = gc;
    }

    void insertValue(std::shared_ptr<ValueStruct> valuePtr) {
        if (GC && Values.size() >= GC_MAX_HEAP_SIZE)
            GC->collect(std::ref(Values));
        Values.push_back(valuePtr);
    }

    // [DBG]
    void traverse(void) const {
        for (auto& val : Values) {
            std::cout << "Value mark: " << val->getMark() << std::endl;
        }
    }

private:
    std::list<std::shared_ptr<ValueStruct>> Values;
    const GarbageCollector* GC = nullptr;
};
