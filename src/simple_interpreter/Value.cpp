#include "Value.h"

Value::~Value() = default;

ValueNumber::ValueNumber(const long val) : Value(val) {
}

long ValueNumber::getValue() const {
    return Value;
}
