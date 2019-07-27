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

#include "UIPanel.h"

#include "../Graphics/Renderer.h"
#include "UIWidget.h"

namespace NerdThings::Ngine::UI {
    // Destructor

    UIPanel::~UIPanel() {
    }

    // Public Methods

    void UIPanel::Draw() {
        DrawStyles();

        Graphics::GraphicsManager::PushTarget(_RenderTarget);

        Graphics::Renderer::Clear(Graphics::TColor::Transparent);

        DrawChildren();

        bool popped = false;
        Graphics::GraphicsManager::PopTarget(popped);

        auto rPos = GetRenderPosition();

        // Draw target
        Graphics::Renderer::DrawTexture(_RenderTarget.GetTexture(),
                                       {
                                               rPos.X,
                                               rPos.Y,
                                               GetWidth(),
                                               GetHeight()
                                       },
                                       {
                                               0,
                                               0,
                                               static_cast<float>(_RenderTarget.Width),
                                               static_cast<float>(_RenderTarget.Height) * -1
                                       },
                                       Graphics::TColor::White);
    }

    float UIPanel::GetOffsetAbove(std::string &name_) {
        return GetOffsetAbove(GetChild<UIControl>(name_));
    }

    float UIPanel::GetOffsetBeside(std::string &name_) {
        return GetOffsetBeside(GetChild<UIControl>(name_));
    }

    TVector2 UIPanel::GetLogicPosition() {
        if (_ParentWidget != nullptr)
            return _ParentWidget->GetPosition();
        else
            return UIControl::GetLogicPosition();
    }

    TVector2 UIPanel::GetRenderPosition() {
        if (_ParentWidget != nullptr)
            return _ParentWidget->GetPosition();
        else
            return UIControl::GetRenderPosition();
    }

    void UIPanel::InternalSetParentWidget(UIWidget *widget_) {
        _ParentWidget = widget_;
    }

    void UIPanel::SetHeight(float height_) {
        UIControlSized::SetHeight(height_);

        _RenderTarget = Graphics::TRenderTarget(static_cast<int>(GetWidth()), static_cast<int>(height_));
    }

    void UIPanel::SetWidth(float width_) {
        UIControlSized::SetWidth(width_);

        _RenderTarget = Graphics::TRenderTarget(static_cast<int>(width_), static_cast<int>(GetHeight()));
    }

    void UIPanel::Update() {
        UIControl::Update();
    }

    // Protected Constructor(s)

    UIPanel::UIPanel(float width_, float height_) {
        SetHeight(height_);
        SetWidth(width_);
        _ChildrenConfig = 3; // Allow multiple children

        // Create render target
        _RenderTarget = Graphics::TRenderTarget(static_cast<int>(width_), static_cast<int>(height_));
    }
}