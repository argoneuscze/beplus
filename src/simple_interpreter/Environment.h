#pragma once

#include <memory>

class Environment {
public:
    explicit Environment(std::unique_ptr<Environment>&& prevEnv);

    std::unique_ptr<Environment>&& getPrevEnvRvalRef();

private:
    std::unique_ptr<Environment> PrevEnv;

    Environment* getPrevEnv() const;
};
