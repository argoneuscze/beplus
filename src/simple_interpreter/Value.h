#pragma once

class Value {
public:
    Value() = default;
    virtual ~Value() = 0;
    Value(const Value& other) = default;
    Value(Value&& other) noexcept = default;
    Value& operator=(const Value& other) = default;
    Value& operator=(Value&& other) noexcept = default;
};

class ValueNumber : public Value {
public:
    explicit ValueNumber(long val);

    long getValue() const;

private:
    const long Value;
};

class ValueBool : public Value {
public:
    explicit ValueBool(const bool val);

    bool getValue(void) const;

private:
    const long Value;
};
