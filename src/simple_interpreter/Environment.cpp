#include "Environment.h"

#include <cassert>
#include <string>
#include <iostream> // [DBG]

Environment::Environment(Environment* prevEnv): PrevEnv(prevEnv) {
}

bool Environment::setVariable(const std::string & name, const std::shared_ptr<Value> value) {
    const auto val = Variables.find(name);

    if (val != Variables.end()) {
        std::cout << "[Env] found var " << name << " and setting it." << std::endl;
        Variables[name] = value;
        return true;
    }

    if (!PrevEnv) {
        std::cout << "[Env] could not find var to be set in Env" << std::endl;
        return false;
    }

    return PrevEnv->setVariable(name, value);
}

bool Environment::initVariable(const std::string & name, const std::shared_ptr<Value> value) {
    std::cout << "[Env] Initializing variable " << name << std::endl;

    const auto var = Variables.find(name);

    if (var == Variables.end()) {
        Variables[name] = value;
        std::cout << "[Env] Successfully initialized variable." << std::endl;
        return true;
    }
    else {
        std::cout << "[Env] Unable to init variable " << name << " as it already exists in environment." << std::endl;
        return false;
    }
}

std::shared_ptr<Value> Environment::getVariable(const std::string & objName, const std::string & attrName) {
    std::cout << "[Env] Variables.size: " << Variables.size() << std::endl;
    std::cout << "[Env] Trying to find: " << objName << "." << attrName << std::endl;

    const auto val = Variables.find(objName);
    // check if current environment has variable
    if (val != Variables.end()) {
        std::cout << "[Env] Found var" << std::endl;
        if (attrName != "")
            return dynamic_cast<ValueStruct*>(val->second.get())->getValue(attrName);
        return val->second;
    }
    // value not found, check parents, if exist
    // return nullptr if no parent
    if (!PrevEnv) {
        std::cout << "[Env] Could not find var in Env" << std::endl;
        return nullptr;
    }
    // otherwise return parent's getVariable
    return PrevEnv->getVariable(objName, attrName);
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

std::vector<std::unique_ptr<Environment>>& Environment::getChildren() {
    return this->Children;
}

std::map<std::string, std::shared_ptr<Value>>& Environment::getVariables() {
    return this->Variables;
}
