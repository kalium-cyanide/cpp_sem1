#pragma once

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