#pragma once

#include <memory>
#include <vector>

class Environment {
public:
    explicit Environment(Environment* prevEnv);

    // creates and returns a new environment, setting its parent to this one
    Environment* fork();
    // destroys the current environment and returns a pointer to the previous one
    Environment* restorePrev();

private:
    Environment* const PrevEnv;
    std::vector<std::unique_ptr<Environment>> Children;

    Environment* getPrevEnv() const;
};
