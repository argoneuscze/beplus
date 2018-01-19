#pragma once

#include <memory>
#include <vector>
#include <map>

#include "Value.h"

class Environment {
public:
    explicit Environment(Environment* prevEnv);

    // adds a new variable to the environment
    void addVariable(std::string& name, std::shared_ptr<Value> value);
    // removes a variable from the environment
    void removeVariable(std::string& name);
    // gets a shared pointer to a variable
    std::shared_ptr<Value> getVariable(std::string& name);

    // creates and returns a new environment, setting its parent to this one
    Environment* fork();
    // destroys the current environment and returns a pointer to the previous one
    Environment* restorePrev();

private:
    Environment* const PrevEnv;
    std::vector<std::unique_ptr<Environment>> Children;

    std::map<std::string, std::shared_ptr<Value>> Variables;
};
