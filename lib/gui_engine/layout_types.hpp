#pragma once

#include "dom_types.hpp"
#include <map>
#include <memory>
#include <string>
#include <vector>

struct StyledNode {
    std::string name;
    DomNode *ref;
    std::map<std::string, std::string> styles;
    std::vector<std::unique_ptr<StyledNode>> children;
    StyledNode *parent;

    std::string GetStyle(const std::string &name) const {
        auto it = styles.find(name);
        if (it != styles.end()) {
            return it->second;
        }
        return "";
    }
};

struct LayoutBox {
    float x = 0.0f;
    float y = 0.0f;
    float width = 0.0f;
    float height = 0.0f;

    StyledNode *styled_node_ref;
    std::vector<std::unique_ptr<LayoutBox>> children;
};