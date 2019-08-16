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

#include "UIControlInteractible.h"

#include "../Input/Mouse.h"

namespace NerdThings::Ngine::UI {
    // Public Methods

    void UIControlInteractible::Update() {
        // Get button bounds (from border)
        auto style = GetStyle();
        auto rect = style.GetBorderRect(GetLogicRectangle());

        // Check the mouse is within our parent too, if not we cant be clicked
        auto par = GetParent<UIControl>();
        if (par != nullptr) {
            auto parentRect = par->GetLogicRectangle();

            if (!parentRect.Contains(Input::Mouse::GetMousePosition()))
                return;
        }

        if (rect.Contains(Input::Mouse::GetMousePosition())) {
            // The mouse is over the button
            if (Input::Mouse::IsButtonPressed(MOUSE_BUTTON_LEFT)) {
                // Clicked
                OnClick({this});

                // Deregister click so double events dont happen
                Input::Mouse::CancelButton(MOUSE_BUTTON_LEFT);
            } else if (!_Hovered) {
                // Hovered
                OnHover({this});
                _Hovered = true;
            }
        } else if (_Hovered) {
            OnUnHover({this});
            _Hovered = false;
        }
    }
}