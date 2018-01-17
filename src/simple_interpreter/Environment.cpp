#include "Environment.h"

#include <cassert>

Environment::Environment(Environment* prevEnv): PrevEnv(prevEnv) {
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

Environment* Environment::getPrevEnv() const {
    return PrevEnv;
}
