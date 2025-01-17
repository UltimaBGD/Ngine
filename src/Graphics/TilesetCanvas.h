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

#ifndef TILESETCANVAS_H
#define TILESETCANVAS_H

#include "../ngine.h"

#include "Rectangle.h"
#include "../Physics/Polygon.h"
#include "Canvas.h"
#include "Tileset.h"

namespace NerdThings::Ngine::Graphics {
    /*
     * A tileset canvas
     */
    class NEAPI TilesetCanvas : public Canvas {
        // Private Fields

        /*
         * The tile data
         */
        std::vector<int> _Tiles;

        /*
         * The tileset
         */
        TTileset _Tileset;

    public:
        // Public Constructor(s)

        /*
         * Create a tileset.
         * Width and height is in tile space.
         * Tiles indices start from 1, so tile one is index 1.
         */
        TilesetCanvas(const TTileset& tileset_, float width_, float height_);

        /*
         * Create a tileset with data.
         * Width and height is in tile space.
         * Tiles indices start from 1, so tile one is index 1.
         */
        TilesetCanvas(const TTileset& tileset_, float width_, float height_, std::vector<int> tiles_);

        // Public Methods

        /*
         * Get collision shapes for a tile in a range.
         * All shapes must be deleted afterwards.
         */
        std::vector<Physics::ICollisionShape *> GetCollisionShapesFor(int tile_, TRectangle range_, TVector2 tilesetPosition_ = TVector2::Zero);

        /*
         * Get collision shapes for tiles in a range.
         * All shapes must be deleted afterwards.
         */
        std::vector<Physics::ICollisionShape *> GetCollisionShapesFor(std::vector<int> tiles_, TRectangle range_, TVector2 tilesetPosition_ = TVector2::Zero);

        /*
         * Get collision shapes for tiles in a range.
         * min_ <= tile <= max_.
         * All shapes must be deleted afterwards.
         */
        std::vector<Physics::ICollisionShape *> GetCollisionShapesFor(int min_, int max_, TRectangle range_, TVector2 tilesetPosition_ = TVector2::Zero);

        /*
         * Get the tile value at the position (0,0 is first tile, 1,0 is second tile etc.).
         */
        int GetTileAt(TVector2 pos_);

        /*
         * Get the tileset being used
         */
        TTileset *GetTileset();

        /*
         * Set the tile value at a position.
         */
        void SetTileAt(TVector2 pos_, int tile_);

        /*
         * Set all tile data
         */
        void SetTileData(std::vector<int> data_);
    protected:

        // Protected Methods

        /*
         * Redraw the canvas
         */
        void RenderTargetRedraw() override;
    };
}

#endif //TILESETCANVAS_H
