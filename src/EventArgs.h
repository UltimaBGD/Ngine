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

#ifndef EVENTARGS_H
#define EVENTARGS_H

#include "ngine.h"
#include "Core/EventHandler.h"
#include "Math/Vector2.h"

namespace NerdThings::Ngine {
    // Forward declarations

    namespace Core {
        class BaseEntity;

        class Scene;

        class Game;
    }

    namespace UI {
        class UIControl;
    }

    // Other event args

    /*
     * Mouse button down event args
     */
    struct MouseButtonEventArgs : Core::EventArgs {
        // Public Fields

        /*
         * The mouse button
         */
        EMouseButton Button;

        /*
         * Whether or not the button is pressed
         */
        bool Pressed;

        // Public Constructor(s)

        MouseButtonEventArgs(const EMouseButton button_, const bool pressed_)
                : Button(button_), Pressed(pressed_) {}
    };

    /*
     * Mouse moved event args
     */
    struct MouseMovedEventArgs : Core::EventArgs {
        // Public Fields

        /*
         * The current mouse position
         */
        Math::TVector2 MousePosition;

        /*
         * The change in mouse position
         */
        Math::TVector2 DeltaMousePosition;

        // Public Constructor(s)

        MouseMovedEventArgs(Math::TVector2 mousePosition_, Math::TVector2 deltaMousePosition_)
                : MousePosition(mousePosition_), DeltaMousePosition(deltaMousePosition_) {}
    };

    struct MouseScrollChangedEventArgs : Core::EventArgs {
        // Public Fields

        /*
         * Scroll wheel value
         */
        int Value;

        // Public Constructor(s)

        MouseScrollChangedEventArgs(int value_)
                : Value(value_) {}
    };

    struct EntityTransformChangedEventArgs : Core::EventArgs {
        // Public Fields

        /*
         * Entity origin
         */
        Math::TVector2 EntityOrigin;

        /*
         * The new entity position
         */
        Math::TVector2 EntityPosition;

        /*
         * The new rotation
         */
        float EntityRotation;

        /*
         * The new scale
         */
        float EntityScale;

        // Public Constructor(s)

        EntityTransformChangedEventArgs(Math::TVector2 origin_, Math::TVector2 pos_, float rot_, float scale_)
                : EntityOrigin(origin_), EntityPosition(pos_), EntityRotation(rot_), EntityScale(scale_) {}
    };

    struct SceneLoadEventArgs : Core::EventArgs {
        // Public Fields

        /*
         * The game that the scene has been loaded into
         */
        Core::Game *Game;

        // Public Constructor(s)

        SceneLoadEventArgs(Core::Game *game_)
                : Game(game_) {}
    };

    struct UIControlEventArgs : Core::EventArgs {
        // Public Fields

        /*
         * The control this belongs to
         */
        UI::UIControl *Control;

        // Public Constructor(s)

        UIControlEventArgs(UI::UIControl *control_)
                : Control(control_) {}
    };
}

#endif //EVENTARGS_H
