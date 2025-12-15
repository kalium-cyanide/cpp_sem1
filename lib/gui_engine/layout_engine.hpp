#pragma once

#include "layout_types.hpp"
#include <memory>

namespace layout_engine {
    std::unique_ptr<LayoutBox> calculate_layout(StyledNode *root, float width, float height);
}