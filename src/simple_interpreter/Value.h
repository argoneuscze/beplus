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
    explicit ValueBool(bool val);

    bool getValue(void) const;

private:
    const long Value;
};

class ValueStruct : public Value {
public:
    explicit
    ValueStruct(std::map<std::string, std::shared_ptr<Value>>& values) : Values(std::move(values)), Mark(false) {
    }

    void setMark(const bool val) {
        Mark = val;
    }

    bool getMark(void) const {
        return Mark;
    }

    std::shared_ptr<Value> getValue(const std::string& name) const {
        return Values.at(name);
    }

    void setValue(const std::string& name, std::shared_ptr<Value> val) {
        Values[name] = val;
    }

private:
    std::map<std::string, std::shared_ptr<Value>> Values;
    bool Mark;
};
