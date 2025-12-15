#include "xml_parser.hpp"
#include <iostream>
#include <pugixml.hpp>
#include <utility>

std::unique_ptr<DomNode> build_dom_tree(pugi::xml_node pugi_node, DomNode *parent_node) {
    auto node = std::make_unique<DomNode>();

    node->name = pugi_node.name();
    node->parent = parent_node;
    node->value = pugi_node.text().get();

    for (pugi::xml_attribute attr: pugi_node.attributes()) {
        node->attributes[attr.name()] = attr.value();
    }

    for (pugi::xml_node child: pugi_node.children()) {
        if (child.type() == pugi::node_element) {
            node->children.push_back(build_dom_tree(child, node.get()));
        }
    }

    return node;
}

namespace xml_parser {
    std::unique_ptr<DomNode> parse(std::string_view input) {
        pugi::xml_document document;

        pugi::xml_parse_result result = document.load_string(input.data());

        if (!result) {
            std::cerr << "XML parse error: " << result.description()
                      << " at offset " << result.offset << std::endl;
            return nullptr;
        }

        pugi::xml_node pugi_root = document.document_element();
        if (!pugi_root) {
            std::cerr << "Error: No root element found in XML." << std::endl;
            return nullptr;
        }

        return build_dom_tree(pugi_root, nullptr);
    }
}// namespace xml_parser