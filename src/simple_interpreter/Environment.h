#pragma once

#include <memory>
#include <vector>
#include <map>

#include "Value.h"

class Environment {
public:
    explicit Environment(Environment* prevEnv);

    // inits a new variable in the environment
    bool initVariable(const std::string& name, std::shared_ptr<Value> value);
    // sets a variable in the environment
    bool setVariable(const std::string& name, std::shared_ptr<Value> value);
    // gets a shared pointer to a variable
    std::shared_ptr<Value> getVariable(const std::string& objName, const std::string& attrName);
    // gets a raw pointer to a struct attribute
    //std::shared_ptr<Value> getStructAttr(const std::string & structName, const std::string & attrName);

    // creates and returns a new environment, setting its parent to this one
    Environment* fork();
    // destroys the current environment and returns a pointer to the previous one
    Environment* restorePrev();

    std::vector<std::unique_ptr<Environment>>& getChildren();
    std::map<std::string, std::shared_ptr<Value>>& getVariables();

private:
    Environment* const PrevEnv;
    std::vector<std::unique_ptr<Environment>> Children;

    std::map<std::string, std::shared_ptr<Value>> Variables;
};
