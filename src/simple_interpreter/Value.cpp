#include "Value.h"
#include <utility>

Value::~Value() = default;

ValueNumber::ValueNumber(const long val) : Value(val) {
}

long ValueNumber::getValue() const {
    return Value;
}
