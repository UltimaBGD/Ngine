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

#ifndef CIRCLE_H
#define CIRCLE_H

#define TYPE_DECL_HEADER
#include "../ngine.h"
#undef TYPE_DECL_HEADER

#include "CollisionShape.h"
#include "Vector2.h"

namespace NerdThings::Ngine {
    struct NEAPI TCircle : public ICollisionShape {
    private:
        // Private Methods

        bool IsCompatible(ICollisionShape *shape_) override;

        bool RunCollisionCheck(ICollisionShape *shape_) override;
    public:
        // Public Fields

        /*
         * Circle center
         */
        TVector2 Center;

        /*
         * Circle radius
         */
        float Radius;

        // Public Constructor(s)

        /*
         * Create a circle
         */
        TCircle(const TVector2 center_, const float radius_)
            : Center(center_), Radius(radius_) {}
    };
}

#endif //CIRCLE_H