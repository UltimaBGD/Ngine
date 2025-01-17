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

#include "Sprite.h"

#include <cmath>

#include "../Graphics/Color.h"
#include "../Graphics/Texture2D.h"
#include "Rectangle.h"
#include "Vector2.h"
#include "Drawing.h"

namespace NerdThings::Ngine::Graphics {
    // Public Constructor(s)

    TSprite::TSprite(std::shared_ptr<TTexture2D> texture_) {
        _Textures.push_back(texture_);

        DrawHeight = texture_->Height;
        DrawWidth = texture_->Width;
    }

    TSprite::TSprite(std::shared_ptr<TTexture2D> texture_, int frameWidth_, int frameHeight_, int drawWidth_, int drawHeight_,
                   float imageSpeed_, int startingFrame)
        : DrawHeight(drawHeight_), DrawWidth(drawWidth_), FrameWidth(frameWidth_), FrameHeight(frameHeight_),
          ImageSpeed(imageSpeed_) {
        _Textures.push_back(texture_);
        CurrentFrame = startingFrame;
    }

    TSprite::TSprite(std::vector<std::shared_ptr<TTexture2D>> textures_, float imageSpeed_, int startingFrame_) { }

    // Public Methods

    void TSprite::Draw(TVector2 position_, float rotation_, TVector2 origin_) {
        Drawing::DrawTexture(GetCurrentTexture(),
                             TRectangle(
                                 position_,
                                 static_cast<float>(DrawWidth),
                                 static_cast<float>(DrawHeight)),
                             GetSourceRectangle(),
                             TColor::White,
                             origin_,
                             rotation_);
    }

    int TSprite::FrameX() {
        if (!_SpriteSheet)
            return 0;

        auto x = 0;
        for (auto i = 0; i < CurrentFrame; i++) {
            x += FrameWidth;
            if (x >= GetCurrentTexture()->Width)
                x = 0;
        }

        return x;
    }

    int TSprite::FrameY() {
        if (!_SpriteSheet)
            return 0;

        auto x = 0;
        auto y = 0;
        for (auto i = 0; i < CurrentFrame; i++) {
            x += FrameWidth;
            if (x >= GetCurrentTexture()->Width) {
                x = 0;
                y += FrameHeight;
            }
        }

        return y;
    }

    std::shared_ptr<TTexture2D> TSprite::GetCurrentTexture() {
        if (_Textures.empty())
            return nullptr;

        if (_SpriteSheet) {
            return _Textures[0];
        }

        return _Textures[CurrentFrame];
    }

    TRectangle TSprite::GetSourceRectangle() {
        if (_SpriteSheet)
            return {
                static_cast<float>(FrameX()),
                static_cast<float>(FrameY()),
                static_cast<float>(FrameWidth),
                static_cast<float>(FrameHeight)
            };
        else
            return {
                0,
                0,
                static_cast<float>(GetCurrentTexture()->Width),
                static_cast<float>(GetCurrentTexture()->Height)
            };
    }

    bool TSprite::IsAnimated() {
        if (_SpriteSheet) {
            if (GetCurrentTexture() != nullptr)
                return FrameHeight != GetCurrentTexture()->Height || FrameWidth != GetCurrentTexture()->Width;
            else
                return false;
        } else {
            return _Textures.size() > 1;
        }
    }

    void TSprite::SetTexture(std::shared_ptr<TTexture2D> texture_) {
        _Textures.clear();
        _Textures.push_back(texture_);
    }

    void TSprite::SetTextures(std::vector<std::shared_ptr<TTexture2D> > textures_) {
        _Textures = textures_;
    }

    void TSprite::Update() {
        if (IsAnimated()) {
            // Increment timer
            _AnimationTimer++;

            while (_AnimationTimer > 0 && fmod(_AnimationTimer, ImageSpeed) == 0) {
                // Reset timer
                _AnimationTimer = 0;

                // Increase frame
                CurrentFrame++;

                // Reset if out of range
                if (_SpriteSheet) {
                    auto count = (GetCurrentTexture()->Width / FrameWidth) * (GetCurrentTexture()->Height / FrameHeight);

                    if (CurrentFrame > count - 1)
                        CurrentFrame = 0;
                } else {
                    if (CurrentFrame > _Textures.size() - 1)
                        CurrentFrame = 0;
                }
            }
        }
    }

    // Operators

    bool TSprite::operator==(const TSprite &b) {
        return _Textures == b._Textures && DrawHeight == b.DrawHeight && DrawWidth == b.DrawWidth && FrameHeight == b.FrameHeight && FrameWidth == b.FrameWidth && ImageSpeed == b.ImageSpeed;
    }

    bool TSprite::operator!=(const TSprite &b) {
        return _Textures != b._Textures || DrawHeight != b.DrawHeight || DrawWidth != b.DrawWidth || FrameHeight != b.FrameHeight || FrameWidth != b.FrameWidth || ImageSpeed != b.ImageSpeed;
    }
}
