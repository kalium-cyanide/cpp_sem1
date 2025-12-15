#include "layout_engine.hpp"

#include <functional>
#include <stdexcept>
#include <yoga/Yoga.h>

namespace {

    class YogaConfigManager {
    public:
        YGConfigRef config;

        YogaConfigManager() {
            config = YGConfigNew();
            YGConfigSetPointScaleFactor(config, 1.0f);
        }

        ~YogaConfigManager() {
            YGConfigFree(config);
        }
    };

    static YogaConfigManager g_yoga_manager;

    YGNodeRef BuildYogaTree(StyledNode *styled_node);
    std::unique_ptr<LayoutBox> CreateLayoutTree(YGNodeRef yoga_node, const StyledNode *corresponding_styled_node);

    void ApplyYogaValue(YGNodeRef node, const std::string &value, float w, float h,
                        std::function<void(YGNodeRef, float)> setter_px,
                        std::function<void(YGNodeRef, float)> setter_percent,
                        std::function<void(YGNodeRef)> setter_auto) {
        if (value.empty()) return;
        if (value == "auto") {
            setter_auto(node);
        } else if (value.ends_with('%') or value.ends_with("pc")) {
            try {
                setter_percent(node, std::stof(value));
            } catch (...) {}
        } else if (value.ends_with("vw")) {
            try {
                setter_px(node, std::stod(value) / 100.0 * w);
            } catch (...) {}
        } else if (value.ends_with("vh")) {
            try {
                setter_px(node, std::stod(value) / 100.0 * h);
            } catch (...) {}
        } else if (value.ends_with("vmin")) {
            try {
                setter_px(node, std::stod(value) / 100.0 * std::min(w, h));
            } catch (...) {}
        } else if (value.ends_with("vmax")) {
            try {
                setter_px(node, std::stod(value) / 100.0 * std::max(w, h));
            } catch (...) {}
        } else {
            try {
                setter_px(node, std::stod(value));
            } catch (...) {}
        }
    }

    void ApplyYogaEdge(YGNodeRef node, const std::string &value,
                       std::function<void(YGNodeRef, YGEdge, float)> setter,
                       YGEdge edge) {
        if (value.empty()) return;
        try {
            setter(node, edge, std::stof(value));
        } catch (...) {}
    }

    void ApplyYogaStyles(StyledNode *styled_node, YGNodeRef yoga_node, float w, float h) {
        if (styled_node->GetStyle("display") == "none") {
            YGNodeStyleSetDisplay(yoga_node, YGDisplayNone);
            return;
        }

        if (styled_node->GetStyle("flex-direction") == "column") {
            YGNodeStyleSetFlexDirection(yoga_node, YGFlexDirectionColumn);
        } else {
            YGNodeStyleSetFlexDirection(yoga_node, YGFlexDirectionRow);
        }

        if (styled_node->GetStyle("justify-content") == "center") {
            YGNodeStyleSetJustifyContent(yoga_node, YGJustifyCenter);
        } else if (styled_node->GetStyle("justify-content") == "space-between") {
            YGNodeStyleSetJustifyContent(yoga_node, YGJustifySpaceBetween);
        } else if (styled_node->GetStyle("justify-content") == "space-around") {
            YGNodeStyleSetJustifyContent(yoga_node, YGJustifySpaceAround);
        } else if (styled_node->GetStyle("justify-content") == "space-evenly") {
            YGNodeStyleSetJustifyContent(yoga_node, YGJustifySpaceEvenly);
        } else if (styled_node->GetStyle("justify-content") == "flex-end") {
            YGNodeStyleSetJustifyContent(yoga_node, YGJustifyFlexEnd);
        }

        if (styled_node->GetStyle("align-items") == "center") {
            YGNodeStyleSetAlignItems(yoga_node, YGAlignCenter);
        } else if (styled_node->GetStyle("align-items") == "flex-start") {
            YGNodeStyleSetAlignItems(yoga_node, YGAlignFlexStart);
        }

        if (styled_node->GetStyle("flex-wrap") == "wrap") {
            YGNodeStyleSetFlexWrap(yoga_node, YGWrapWrap);
        }

        if (!styled_node->GetStyle("flex-grow").empty()) {
            YGNodeStyleSetFlexGrow(yoga_node, std::stof(styled_node->GetStyle("flex-grow")));
        }
        if (!styled_node->GetStyle("flex-shrink").empty()) {
            YGNodeStyleSetFlexShrink(yoga_node, std::stof(styled_node->GetStyle("flex-shrink")));
        }


        ApplyYogaValue(yoga_node, styled_node->GetStyle("width"), w, h, YGNodeStyleSetWidth, YGNodeStyleSetWidthPercent, YGNodeStyleSetWidthAuto);
        ApplyYogaValue(yoga_node, styled_node->GetStyle("height"), w, h, YGNodeStyleSetHeight, YGNodeStyleSetHeightPercent, YGNodeStyleSetHeightAuto);

        ApplyYogaValue(yoga_node, styled_node->GetStyle("min-width"), w, h, YGNodeStyleSetMinWidth, YGNodeStyleSetWidthPercent, YGNodeStyleSetWidthAuto);
        ApplyYogaValue(yoga_node, styled_node->GetStyle("min-height"), w, h, YGNodeStyleSetMinHeight, YGNodeStyleSetHeightPercent, YGNodeStyleSetHeightAuto);

        ApplyYogaValue(yoga_node, styled_node->GetStyle("max-width"), w, h, YGNodeStyleSetMaxWidth, YGNodeStyleSetWidthPercent, YGNodeStyleSetWidthAuto);
        ApplyYogaValue(yoga_node, styled_node->GetStyle("max-height"), w, h, YGNodeStyleSetMaxHeight, YGNodeStyleSetHeightPercent, YGNodeStyleSetHeightAuto);

        ApplyYogaEdge(yoga_node, styled_node->GetStyle("margin-left"), YGNodeStyleSetMargin, YGEdgeLeft);
        ApplyYogaEdge(yoga_node, styled_node->GetStyle("margin-right"), YGNodeStyleSetMargin, YGEdgeRight);
        ApplyYogaEdge(yoga_node, styled_node->GetStyle("margin-top"), YGNodeStyleSetMargin, YGEdgeTop);
        ApplyYogaEdge(yoga_node, styled_node->GetStyle("margin-bottom"), YGNodeStyleSetMargin, YGEdgeBottom);

        ApplyYogaEdge(yoga_node, styled_node->GetStyle("padding-left"), YGNodeStyleSetPadding, YGEdgeLeft);
        ApplyYogaEdge(yoga_node, styled_node->GetStyle("padding-right"), YGNodeStyleSetPadding, YGEdgeRight);
        ApplyYogaEdge(yoga_node, styled_node->GetStyle("padding-top"), YGNodeStyleSetPadding, YGEdgeTop);
        ApplyYogaEdge(yoga_node, styled_node->GetStyle("padding-bottom"), YGNodeStyleSetPadding, YGEdgeBottom);

        if (styled_node->GetStyle("position") == "absolute") {
            YGNodeStyleSetPositionType(yoga_node, YGPositionTypeAbsolute);
            ApplyYogaEdge(yoga_node, styled_node->GetStyle("left"), YGNodeStyleSetPosition, YGEdgeLeft);
            ApplyYogaEdge(yoga_node, styled_node->GetStyle("top"), YGNodeStyleSetPosition, YGEdgeTop);
            ApplyYogaEdge(yoga_node, styled_node->GetStyle("right"), YGNodeStyleSetPosition, YGEdgeRight);
            ApplyYogaEdge(yoga_node, styled_node->GetStyle("bottom"), YGNodeStyleSetPosition, YGEdgeBottom);
        }
    }

    YGNodeRef BuildYogaTree(StyledNode *styled_node, float w, float h) {
        YGNodeRef yoga_node = YGNodeNewWithConfig(g_yoga_manager.config);

        ApplyYogaStyles(styled_node, yoga_node, w, h);

        for (size_t i = 0; i < styled_node->children.size(); ++i) {
            auto child_styled_node = styled_node->children[i].get();
            YGNodeRef child_yoga_node = BuildYogaTree(child_styled_node, w, h);
            YGNodeInsertChild(yoga_node, child_yoga_node, i);
        }

        return yoga_node;
    }

    std::unique_ptr<LayoutBox> CreateLayoutTree(YGNodeRef yoga_node, StyledNode *corresponding_styled_node) {
        auto box = std::make_unique<LayoutBox>();

        box->x = YGNodeLayoutGetLeft(yoga_node);
        box->y = YGNodeLayoutGetTop(yoga_node);
        box->width = YGNodeLayoutGetWidth(yoga_node);
        box->height = YGNodeLayoutGetHeight(yoga_node);
        box->styled_node_ref = corresponding_styled_node;

        for (size_t i = 0; i < YGNodeGetChildCount(yoga_node); ++i) {
            YGNodeRef child_yoga_node = YGNodeGetChild(yoga_node, i);

            if (YGNodeStyleGetDisplay(child_yoga_node) == YGDisplayNone) {
                continue;
            }

            StyledNode *child_styled_node = corresponding_styled_node->children[i].get();
            box->children.push_back(CreateLayoutTree(child_yoga_node, child_styled_node));
        }

        return box;
    }

}// namespace

namespace layout_engine {

    std::unique_ptr<LayoutBox> calculate_layout(StyledNode *root_node, float viewport_width, float viewport_height) {
        YGNodeRef root_yoga_node = BuildYogaTree(root_node, viewport_width, viewport_height);

        YGNodeStyleSetWidth(root_yoga_node, viewport_width);
        YGNodeStyleSetHeight(root_yoga_node, viewport_height);

        YGNodeCalculateLayout(root_yoga_node, viewport_width, viewport_height, YGDirectionLTR);

        std::unique_ptr<LayoutBox> root_layout_box = CreateLayoutTree(root_yoga_node, root_node);

        YGNodeFreeRecursive(root_yoga_node);

        return root_layout_box;
    }

}// namespace layout_engine