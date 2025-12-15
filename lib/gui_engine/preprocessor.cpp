//
// Created by denis on 14.11.2025.
//
#include "preprocessor.hpp"
#include <regex>
#include <sol/sol.hpp>

void preprocessor::preprocess(std::string &file) {
    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::string);

    std::regex pattern(R"(\{([\s\S]*)\})");

    std::string output;

    lua.set_function("echo", [&](sol::variadic_args args) {
        for (auto i: args) {
            output += lua["tostring"](i).get<std::string>();
        }
    });

    auto iterator = std::sregex_iterator(file.cbegin(), file.cend(), pattern);

    for (; iterator != std::sregex_iterator(); iterator++) {
        auto script = (*iterator)[1].str();

        lua.script(script);

        file.replace(iterator->begin()->first, iterator->begin()->second, output.begin(), output.end());

        output = "";
    }
}
