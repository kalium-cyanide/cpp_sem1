#pragma once

#include "dom_types.hpp"
#include <algorithm>
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


inline std::unique_ptr<StyledNode> detach_styled_node(StyledNode *parent, StyledNode *child_to_find) {
    auto &children = parent->children;
    auto it = std::find_if(children.begin(), children.end(),
                           [child_to_find](const std::unique_ptr<StyledNode> &ptr) {
                               return ptr.get() == child_to_find;
                           });

    if (it != children.end()) {
        std::unique_ptr<StyledNode> moved_node = std::move(*it);
        children.erase(it);
        return moved_node;
    }
    return nullptr;
}

struct LayoutBox {
    float x = 0.0f;
    float y = 0.0f;
    float width = 0.0f;
    float height = 0.0f;

    StyledNode *styled_node_ref;
    std::vector<std::unique_ptr<LayoutBox>> children;
};