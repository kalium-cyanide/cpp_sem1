#include "css_parser.hpp"

#include <algorithm>
#include <istream>
#include <string>

namespace CssParser {

    std::string_view trim(const std::string_view &string) {
        auto begin = string.find_first_not_of(" \n\0\t\r");
        auto end = string.find_last_not_of(" \n\0\t\r");

        return string.substr(begin, end - begin + 1);
    }

    Specificity calculate_specificity(std::string_view selector) {
        auto specificity = Specificity{};
        bool in_token = false;

        for (char c: selector) {
            if (std::isspace(c)) {
                in_token = false;
            } else if (c == '#') {
                specificity.id_count++;
                in_token = true;
            } else if (c == '.') {
                specificity.class_n_attr_count++;
                in_token = true;
            } else if (std::isalpha(c)) {
                if (!in_token) {
                    specificity.tag_count++;
                }
                in_token = true;
            } else {
                in_token = true;
            }
        }

        return specificity;
    }


    std::map<std::string, std::string> parse_declarations(std::string_view block) {
        std::map<std::string, std::string> declarations;
        size_t position = 0;

        while (position < block.length()) {
            auto semicolon_pos = block.find(';', position);
            if (semicolon_pos == std::string_view::npos) { semicolon_pos = block.length(); }

            std::string_view declaration = block.substr(position, semicolon_pos - position);
            position = semicolon_pos + 1;

            auto colon = declaration.find(':');
            if (colon == std::string_view::npos) { continue; }

            std::string_view property = trim(declaration.substr(0, colon));
            std::string_view value = trim(declaration.substr(colon + 1));

            if (property.empty() or value.empty()) { continue; }

            declarations[std::string(property)] = std::string(value);
        }

        return declarations;
    }

    StyleSheet parse(std::istream &in) {
        StyleSheet stylesheet;
        size_t pos = 0;

        auto css_content = std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
        auto css_view = std::string_view(css_content);

        while (pos < css_view.length()) {
            auto open_brace = css_view.find('{', pos);
            if (open_brace == std::string_view::npos) break;

            auto selector = trim(css_view.substr(pos, open_brace - pos));


            auto close_brace = css_view.find('}', open_brace);
            if (close_brace == std::string_view::npos) break;

            auto declarations_block = css_view.substr(open_brace + 1, close_brace - (open_brace + 1));

            pos = close_brace + 1;

            if (selector.empty()) { continue; }

            CssRule rule;
            rule.selector = std::string(selector);
            rule.specificity = calculate_specificity(selector);
            rule.declarations = parse_declarations(declarations_block);
            stylesheet.rules.push_back(std::move(rule));
        }

        return stylesheet;
    }
}// namespace CssParser