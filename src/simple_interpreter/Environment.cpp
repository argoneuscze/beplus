#include "Environment.h"

#include <cassert>

Environment::Environment(Environment* prevEnv): PrevEnv(prevEnv) {
}

void Environment::addVariable(std::string& name, std::shared_ptr<Value> value) {
    Variables.insert(std::make_pair(name, value));
}

void Environment::removeVariable(std::string& name) {
    Variables.erase(name);
}

std::shared_ptr<Value> Environment::getVariable(std::string& name) {
    const auto val = Variables.find(name);
    // check if current environment has variable
    if (val != Variables.end()) {
        return val->second;
    }
    // value not found, check parents, if exist
    // return nullptr if no parent
    if (!PrevEnv)
        return nullptr;
    // otherwise return parent's getVariable
    return PrevEnv->getVariable(name);
}

Environment* Environment::fork() {
    auto newEnv = std::make_unique<Environment>(this);
    const auto ret = newEnv.get();
    Children.push_back(std::move(newEnv));
    return ret;
}

Environment* Environment::restorePrev() {
    // return nullptr if no previous node
    if (!PrevEnv)
        return nullptr;
    // remove this node from previous node and return it
    const auto retPtr = PrevEnv;
    for (auto it = PrevEnv->Children.begin(); it != PrevEnv->Children.end(); ++it) {
        if (it->get() == this) {
            PrevEnv->Children.erase(it);
            return retPtr;
        }
    }
    assert("Previous node doesn't have this one as child" && 0);
    return nullptr;
}
