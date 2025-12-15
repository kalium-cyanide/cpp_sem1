#pragma once

#include "css_types.hpp"
#include "dom_types.hpp"
#include "layout_types.hpp"
#include <vector>


namespace style_engine {

    std::unique_ptr<StyledNode> apply_styles(DomNode *root_node, const StyleSheet &stylesheet);

}// namespace style_engine