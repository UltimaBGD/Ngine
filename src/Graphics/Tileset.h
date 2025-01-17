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

#ifndef TILESET_H
#define TILESET_H

#include "../ngine.h"

#include "Vector2.h"
#include "Texture2D.h"

namespace NerdThings::Ngine::Graphics {
    // TODO: Animated tilesets
    /*
     * A tileset
     */
    struct NEAPI TTileset {
        // Public Fields

        /*
         * The tileset texture
         */
        std::shared_ptr<TTexture2D> _Texture;

        /*
         * Tile width
         */
        float _TileHeight;

        /*
         * Tile height
         */
        float _TileWidth;

        // Public Constructor(s)

        TTileset(std::shared_ptr<TTexture2D> texture_, float tileWidth_, float tileHeight_);

        // Public Methods

        void DrawTile(TVector2 position_, int tile_);

        float GetTileHeight() const;

        float GetTileWidth() const;
    };
}

#endif //TILESET_H
