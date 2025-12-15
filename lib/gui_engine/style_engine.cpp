#include "style_engine.hpp"
#include <sstream>

namespace style_engine {

    std::vector<std::string_view> split(std::string string, std::string_view delimeters) {
        std::vector<std::string_view> result;
        auto begin = string.begin();

        while (true) {
            auto end = std::find_first_of(begin, string.end(), delimeters.begin(), delimeters.end());

            if (end == string.end()) { break; }

            result.push_back(std::string_view(begin, end));
            begin = end + 1;
        }

        result.push_back(std::string_view(begin, string.end()));

        return result;
    }

    bool is_match_selector(DomNode *node, std::string_view selector) {
        if (selector[0] == '#') {
            auto id = node->attributes["id"];

            if (id == selector.substr(1)) {
                return true;
            }
        }

        if (selector[0] == '.') {
            auto classes = split(node->attributes["class"], " ");

            if (std::find(classes.begin(), classes.end(), selector.substr(1)) != classes.end()) {
                return true;
            }
        }

        return false;
    }

    std::vector<const CssRule *> collect_matching_rules(DomNode *node, const StyleSheet &stylesheet) {
        std::vector<const CssRule *> matches;

        for (auto &rule: stylesheet.rules) {

            if (is_match_selector(node, rule.selector)) {
                matches.push_back(&rule);
            }
        }

        std::sort(matches.begin(), matches.end(), std::greater());

        return matches;
    }

    std::map<std::string, std::string> apply_cascade(const std::vector<const CssRule *> &matching_rules) {
        std::map<std::string, std::string> final_styles;

        for (auto it = matching_rules.rbegin(); it != matching_rules.rend(); ++it) {
            const CssRule *rule = *it;
            for (const auto &decl: rule->declarations) {
                final_styles[decl.first] = decl.second;
            }
        }
        return final_styles;
    }

    std::unique_ptr<StyledNode> build_styled_tree(DomNode *dom_node, const StyleSheet &stylesheet) {
        std::unique_ptr<StyledNode> styled_node = std::make_unique<StyledNode>(dom_node->name);

        std::vector<const CssRule *> matchingRules = collect_matching_rules(dom_node, stylesheet);
        styled_node->styles = apply_cascade(matchingRules);
        styled_node->ref = dom_node;

        for (auto &child_dom_node: dom_node->children) {
            std::unique_ptr<StyledNode> child_styled_node = build_styled_tree(child_dom_node.get(), stylesheet);

            if (child_styled_node) {
                styled_node->children.push_back(std::move(child_styled_node));
            }
        }

        return styled_node;
    }

    std::unique_ptr<StyledNode> apply_styles(DomNode *root_node, const StyleSheet &stylesheet) {
        return std::move(build_styled_tree(root_node, stylesheet));
    }

}// namespace style_engine