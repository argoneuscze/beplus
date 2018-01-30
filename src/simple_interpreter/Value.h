#pragma once

#include <map>
#include <memory>

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

class ValueStruct : public Value {
public:
    explicit ValueStruct(std::map<std::string, std::unique_ptr<Value>> & values) : Values(std::move(values)), Mark(0) {
    }

    int getMark(void) const {
        return Mark;
    }

private:
    const std::map<std::string, std::unique_ptr<Value>> Values;
    int Mark;
};
