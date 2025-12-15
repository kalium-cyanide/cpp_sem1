#pragma once

#include <functional>

#include "../utility.hpp"
#include "ValidatorIO.hpp"

using Middleware = std::function<void(const ValidatorInput &, ValidatorOutput &,
                                      std::function<void()>)>;

class Validator {
    array_t<Middleware> middlewares;

public:
    void use(Middleware function) { middlewares.push_back(function); }

    void validate(const ValidatorInput &input, ValidatorOutput &output) {
        size_t currentIndex = 0;

        std::function<void()> next = [&, this]() {
            if (currentIndex < middlewares.size()) {
                auto middleware = middlewares[currentIndex++];
                middleware(input, output, next);
            }
        };

        next();
    }
};