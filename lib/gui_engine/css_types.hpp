#pragma once

#include <map>
#include <string>
#include <vector>

struct Specificity {
    unsigned id_count;
    unsigned class_n_attr_count;
    unsigned tag_count;

    bool operator>(const Specificity &other) const {
        if (id_count != other.id_count)
            return id_count > other.id_count;

        if (class_n_attr_count != other.class_n_attr_count)
            return class_n_attr_count > other.class_n_attr_count;

        return tag_count > other.tag_count;
    }
};


struct CssRule {
    std::string selector;
    Specificity specificity;

    std::map<std::string, std::string> declarations;

    bool operator>(const CssRule &other) {
        return specificity > other.specificity;
    }
};

struct StyleSheet {
    std::vector<CssRule> rules;
};
