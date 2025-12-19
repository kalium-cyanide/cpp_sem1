#pragma once

#include <algorithm>
#include <map>
#include <memory>
#include <string>
#include <vector>

struct DomNode {
    std::string name;
    std::string value;
    std::map<std::string, std::string> attributes;
    std::vector<std::unique_ptr<DomNode>> children;
    DomNode *parent;
};


inline std::unique_ptr<DomNode> detach_dom_node(DomNode *parent, DomNode *child_to_find) {
    auto &children = parent->children;
    auto it = std::find_if(children.begin(), children.end(),
                           [child_to_find](const std::unique_ptr<DomNode> &ptr) {
                               return ptr.get() == child_to_find;
                           });

    if (it != children.end()) {
        std::unique_ptr<DomNode> moved_node = std::move(*it);
        children.erase(it);
        return moved_node;
    }
    return nullptr;
}