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

#ifndef SCENE_H
#define SCENE_H

// Include ngine
#include "../ngine.h"

#include "BaseEntity.h"         // Required for: BaseEntity
#include "EntityContainer.h"    // Required for: EntityContainer

namespace Ngine {
    namespace Core {
        /*
         * A container for entities
         */
        class NEAPI Scene : public EntityContainer {
            // Private Fields

        public:
            // Public Constructor(s)

            /*
             * Create a new Scene
             */
            Scene();

            // Public Destructor

            virtual ~Scene() = default;

            // Public Methods

            /*
             * Draw the scene
             */
            virtual void Draw();

            /*
             * Update the scene
             */
            virtual void Update();
        };
    }
}

#endif //SCENE_H
