#pragma once

#include <iostream> // [DBG]
#include <list>
#include "Value.h"

class Heap {
public:
    explicit Heap(void) { }

    void insertValue(std::shared_ptr<ValueStruct> valuePtr) {
        Values.push_back(valuePtr);
    }

    // [DBG]
    void traverse(void) const {
        for (auto & val : Values) {
            std::cout << "Value mark: " << val->getMark() << std::endl;
        }
    }

private:
    std::list<std::shared_ptr<ValueStruct>> Values;
};
