#include "Environment.h"

Environment::Environment(std::unique_ptr<Environment>&& prevEnv): PrevEnv(std::move(prevEnv)) {
}

std::unique_ptr<Environment>&& Environment::getPrevEnvRvalRef() {
    return std::move(PrevEnv);
}

Environment* Environment::getPrevEnv() const {
    return PrevEnv.get();
}
