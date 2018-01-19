#include "Environment.h"

#include <cassert>
#include <iostream> // [DBG]

Environment::Environment(Environment* prevEnv): PrevEnv(prevEnv) {
}

void Environment::setVariable(const std::string & name, const std::shared_ptr<Value> value) {
    std::cout << "[Env] Setting " << name << std::endl;
    Variables[name] = value;
    std::cout << "[Env] Variables.size: " << Variables.size() << std::endl;
}

std::shared_ptr<Value> Environment::getVariable(const std::string & name) {
    std::cout << "[Env] Variables.size: " << Variables.size() << std::endl;
    std::cout << "[Env] Trying to find: " << name << std::endl;

    const auto val = Variables.find(name);
    // check if current environment has variable
    if (val != Variables.end()) {
        std::cout << "[Env] Found var" << std::endl;
        return val->second;
    }
    // value not found, check parents, if exist
    // return nullptr if no parent
    if (!PrevEnv) {
        std::cout << "[Env] Could not find var in Env" << std::endl;
        return nullptr;
    }
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
