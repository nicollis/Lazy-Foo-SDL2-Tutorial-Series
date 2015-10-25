#pragma once
#ifndef _OGE_TILE_H_
#define _OGE_TILE_H_

#include <SDL.h>
#include <stdio.h>
#include <string>
#include <fstream>

#include "OTexture.h"
#include "Util.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int LEVEL_WIDTH;
extern const int LEVEL_HEIGHT;

//The tile
class Tile
{
public:

	//Tile constants
	static const int TILE_WIDTH = 80;
	static const int TILE_HEIGHT = 80;
	static const int TOTAL_TILES = 192;
	static const int TOTAL_TILE_SPRITES = 12;

	//Collection of tiles to be used by map
	static Tile* TileSet[TOTAL_TILES];
	static SDL_Rect TileClips[TOTAL_TILE_SPRITES];

	//The different tile sprites
	static const int TILE_RED = 0;
	static const int TILE_GREEN = 1;
	static const int TILE_BLUE = 2;
	static const int TILE_CENTER = 3;
	static const int TILE_TOP = 4;
	static const int TILE_TOPRIGHT = 5;
	static const int TILE_RIGHT = 6;
	static const int TILE_BOTTOMRIGHT = 7;
	static const int TILE_BOTTOM = 8;
	static const int TILE_BOTTOMLEFT = 9;
	static const int TILE_LEFT = 10;
	static const int TILE_TOPLEFT = 11;

	static bool init(SDL_Renderer *renderer);

	static void free();

	//Checks collision box against set of tiles
	static bool checkWallCollision(SDL_Rect box, Tile* tiles[]);

	//Initalize position and type
	Tile(int x, int y, int tileType);

	//Show the tile
	void render(SDL_Rect &camera);

	//Get the tile type
	int getType();

	//Get the collision box
	SDL_Rect getBox();

private:
	static bool setTiles();

	//The attriubutes of the tile
	SDL_Rect mBox;

	//The tile type
	int mType;

	//Tile Texture
	static OTexture sTileTexture;
};
#endif