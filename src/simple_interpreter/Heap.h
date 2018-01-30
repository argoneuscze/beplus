#pragma once

#include <iostream> // [DBG]
#include <list>
#include "Value.h"

class Heap {
public:
    explicit Heap(void) { }

    void insertValue(std::unique_ptr<ValueStruct> valuePtr) {
        Values.push_back(std::move(valuePtr)); 
    }

    // [DBG]
    void traverse(void) const {
        for (auto & val : Values) {
            std::cout << "Value mark: " << val->getMark() << std::endl;
        }
    }

private:
    std::list<std::unique_ptr<ValueStruct>> Values;
};
