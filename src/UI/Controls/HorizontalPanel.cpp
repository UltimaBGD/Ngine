/**********************************************************************************************
*
*   Ngine - A (mainly) 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#include "HorizontalPanel.h"

namespace NerdThings::Ngine::UI::Controls {
    // Public Methods

    void HorizontalPanel::FocusNext() {
        // TODO: Add this functionality in feature/advanced-ui for controller/tabbing support.
    }

    void HorizontalPanel::FocusPrev() {
        // TODO: Add this functionality in feature/advanced-ui for controller/tabbing support.
    }

    float HorizontalPanel::GetOffsetAbove(UIControl *control_) {
        if (control_ == nullptr || control_ == this)
            return 0;

        auto style = control_->GetStyle();
        auto dims = style.GetBorderDimensions({control_->GetWidth(), control_->GetHeight()});
        switch (VerticalAlignment) {
            case ALIGN_TOP:
                return 0;
            case ALIGN_MIDDLE:
                return (GetHeight() / 2.0f)  - (dims.Y / 2.0f);
            case ALIGN_BOTTOM:
                return GetHeight() - dims.Y;
        }

        return 0; // Shut the compilers up
    }

    float HorizontalPanel::GetOffsetBeside(UIControl *control_) {
        // This returns the widths of everything before this control

        auto children = GetChildren();
        auto iterator = std::find(children.begin(), children.end(), control_);

        if (iterator == children.end())
            return 0;

        auto index = std::distance(children.begin(), iterator);

        auto style = GetStyle();

        auto offset = style.Padding[0];

        for (auto i = index - 1; i >= 0; i--) {
            auto child = children[i];
            auto childStyle = child->GetStyle();

            // Margins TODO: Is this right
            offset += childStyle.Margin[1];
            offset += childStyle.Margin[3];

            // Width
            offset += childStyle.GetBorderRect(child->GetRenderRectangle()).Width;
        }

        return offset;
    }
}