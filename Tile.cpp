#include "Tile.h"

//Static Methods
bool Tile::init(SDL_Renderer *renderer)
{
	bool success = true;

	sTileTexture.setRenderer(renderer);

	if (!sTileTexture.textureIsLoaded())
	{
		if (!sTileTexture.loadFromFile("39_tiling/tiles.png"))
		{
			printf("Filed to load tile set texture!\n");
			success = false;
		}

		//Load tile map
		if (!setTiles())
		{
			printf("Failed to load tile set!\n");
			success = false;
		}
	}
	else
	{
		printf("Warning: Tile Texture and Set already Loaded!\n");
	}
	return success;
}

void Tile::free()
{
	if (sTileTexture.textureIsLoaded())
	{
		//Deallocate tiles
		for (int i = 0; i < TOTAL_TILES; ++i)
		{
			if (TileSet[i] != NULL)
			{
				delete TileSet[i];
				TileSet[i] = NULL;
			}
		}

		sTileTexture.free();
	}
	else
	{
		printf("Warning: Tile has already been free'd!\n");
	}
}

bool Tile::setTiles()
{
	//Success Flag
	bool tilesLoaded = true;

	//The tile offsets
	int x = 0, y = 0;
	
	//Open the map
	std::ifstream map("39_tiling/lazy.map");

	//If the map couldn't be loaded
	if (!map)
	{
		printf("Unable to load map file!\n");
		tilesLoaded = false;
	}
	else
	{
		//Initialize the tiles
		for (int i = 0; i < TOTAL_TILES; ++i)
		{
			//Determins what kind of tile will be made
			int tileType = -1;

			//Read tile from map file
			map >> tileType;

			//If there was a problem in reading the map
			if (map.fail())
			{
				//Stop loading map
				printf("Error loading map: Unexpexted end of file!\n");
				tilesLoaded = false;
				break;
			}

			//If the number is a valid tile number
			if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES))
			{
				TileSet[i] = new Tile(x, y, tileType);
			}
			//If we don't recognize the number
			else
			{
				//Stop loading map
				printf("Error loading map: Invalid tile type at %d!\n", i);
				tilesLoaded = false;
				break;
			}

			//Move to next tile spot 
			x += TILE_WIDTH;

			//If we've gone to far
			if (x >= LEVEL_WIDTH)
			{
				//Move back
				x = 0;

				//Move to the next row
				y += TILE_HEIGHT;
			}
		}//for tile load loop

		 //Clip the sprite sheet
		if (tilesLoaded)
		{
			TileClips[TILE_RED].x = 0;
			TileClips[TILE_RED].y = 0;
			TileClips[TILE_RED].w = TILE_WIDTH;
			TileClips[TILE_RED].h = TILE_HEIGHT;

			TileClips[TILE_GREEN].x = 0;
			TileClips[TILE_GREEN].y = 80;
			TileClips[TILE_GREEN].w = TILE_WIDTH;
			TileClips[TILE_GREEN].h = TILE_HEIGHT;

			TileClips[TILE_BLUE].x = 0;
			TileClips[TILE_BLUE].y = 160;
			TileClips[TILE_BLUE].w = TILE_WIDTH;
			TileClips[TILE_BLUE].h = TILE_HEIGHT;

			TileClips[TILE_TOPLEFT].x = 80;
			TileClips[TILE_TOPLEFT].y = 0;
			TileClips[TILE_TOPLEFT].w = TILE_WIDTH;
			TileClips[TILE_TOPLEFT].h = TILE_HEIGHT;

			TileClips[TILE_LEFT].x = 80;
			TileClips[TILE_LEFT].y = 80;
			TileClips[TILE_LEFT].w = TILE_WIDTH;
			TileClips[TILE_LEFT].h = TILE_HEIGHT;

			TileClips[TILE_BOTTOMLEFT].x = 80;
			TileClips[TILE_BOTTOMLEFT].y = 160;
			TileClips[TILE_BOTTOMLEFT].w = TILE_WIDTH;
			TileClips[TILE_BOTTOMLEFT].h = TILE_HEIGHT;

			TileClips[TILE_TOP].x = 160;
			TileClips[TILE_TOP].y = 0;
			TileClips[TILE_TOP].w = TILE_WIDTH;
			TileClips[TILE_TOP].h = TILE_HEIGHT;

			TileClips[TILE_CENTER].x = 160;
			TileClips[TILE_CENTER].y = 80;
			TileClips[TILE_CENTER].w = TILE_WIDTH;
			TileClips[TILE_CENTER].h = TILE_HEIGHT;

			TileClips[TILE_BOTTOM].x = 160;
			TileClips[TILE_BOTTOM].y = 160;
			TileClips[TILE_BOTTOM].w = TILE_WIDTH;
			TileClips[TILE_BOTTOM].h = TILE_HEIGHT;

			TileClips[TILE_TOPRIGHT].x = 240;
			TileClips[TILE_TOPRIGHT].y = 0;
			TileClips[TILE_TOPRIGHT].w = TILE_WIDTH;
			TileClips[TILE_TOPRIGHT].h = TILE_HEIGHT;

			TileClips[TILE_RIGHT].x = 240;
			TileClips[TILE_RIGHT].y = 80;
			TileClips[TILE_RIGHT].w = TILE_WIDTH;
			TileClips[TILE_RIGHT].h = TILE_HEIGHT;

			TileClips[TILE_BOTTOMRIGHT].x = 240;
			TileClips[TILE_BOTTOMRIGHT].y = 160;
			TileClips[TILE_BOTTOMRIGHT].w = TILE_WIDTH;
			TileClips[TILE_BOTTOMRIGHT].h = TILE_HEIGHT;
		}//Define tile clips
	}//else map did load

	//close the file
	map.close();

	return tilesLoaded;
}//static setTiles method

bool Tile::checkWallCollision(SDL_Rect box, Tile* tiles[])
{
	//Go through the tiles
	for (int i = 0; i < Tile::TOTAL_TILES; ++i)
	{
		//If the tile is a wall type tile
		if ((tiles[i]->getType() >= Tile::TILE_CENTER) && (tiles[i]->getType() <= Tile::TILE_TOPLEFT))
		{
			//If the collision box touches the wall tile
			if (checkCollision(box, tiles[i]->getBox()))
			{
				return true;
			}
		}
	}

	//If no wall tiles were touched
	return false;
}

//Non-static class methods
Tile::Tile(int x, int y, int tileType)
{
	//Get the offsets
	mBox.x = x;
	mBox.y = y;

	//Set the collision box
	mBox.w = TILE_WIDTH;
	mBox.h = TILE_HEIGHT;

	//Get the tile type
	mType = tileType;
}

void Tile::render(SDL_Rect &camera)
{
	//If the tile is on screen
	if (checkCollision(camera, mBox))
	{
		//Show the tile
		sTileTexture.render(mBox.x - camera.x, mBox.y - camera.y, &TileClips[mType]);
	}
}

int Tile::getType()
{
	return mType;
}

SDL_Rect Tile::getBox()
{
	return mBox;
}

Tile* Tile::TileSet[Tile::TOTAL_TILES];
SDL_Rect Tile::TileClips[Tile::TOTAL_TILE_SPRITES];
OTexture Tile::sTileTexture = OTexture();