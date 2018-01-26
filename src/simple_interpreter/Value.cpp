#include "Value.h"
#include <utility>

Value::~Value() = default;

ValueNumber::ValueNumber(const long val) : Value(val) {
}

long ValueNumber::getValue() const {
    return Value;
}

ValueBool::ValueBool(const bool val) : Value(val) {
}

bool ValueBool::getValue(void) const {
    return Value;
}
