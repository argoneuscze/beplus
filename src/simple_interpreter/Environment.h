#pragma once

class Environment {
public:
    explicit Environment(Environment* prevEnv) : PrevEnv(prevEnv) {
    }

private:
    Environment* const PrevEnv;

    Environment* getPrevEnv() const;
};
