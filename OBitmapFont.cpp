#include "OBitmapFont.h"

OBitmapFont::OBitmapFont()
{
	//Initalize variables
	mBitmap = NULL;
	mNewLine = 0;
	mSpace = 0;
}

bool OBitmapFont::buildFont(OTexture *bitmap)
{
	bool success = true;

	//Lock pixels for access
	if (!bitmap->lockTexture())
	{
		printf("Unable to lock bitmap font texture!\n");
		success = false;
	}
	else
	{
		//Set the backgound color
		Uint32 bgColor = bitmap->getPixel32(0, 0);

		//set the cell dimensions
		int cellW = bitmap->getWidth() / 16;
		int cellH = bitmap->getHeight() / 16;

		//New line variables
		int top = cellH;
		int baseA = cellH;

		//The current caracter we're setting
		int currentChar = 0;

		//Got though the cell rows
		for (int rows = 0; rows < 16; ++rows)
		{
			for (int cols = 0; cols < 16; ++cols)
			{
				//Set the character offset
				mChars[currentChar].x = cellW * cols;
				mChars[currentChar].y = cellH * rows;

				//Set the dimensions of the character
				mChars[currentChar].w = cellW;
				mChars[currentChar].h = cellH;

				//Find Left Side
				//Go through pixel columns
				for (int pCol = 0; pCol < cellW; ++pCol)
				{
					//Go through pixel rows
					for (int pRow = 0; pRow < cellH; ++pRow)
					{
						//Get the pixel offsets
						int pX = (cellW * cols) + pCol;
						int pY = (cellH * rows) + pRow;

						//If a non colorkey pixel is found
						if (bitmap->getPixel32(pX, pY) != bgColor)
						{
							//Set the x offset
							mChars[currentChar].x = pX;

							//Break the loops
							pCol = cellW;
							pRow = cellH;
						}//end if pixel is not background color
					}//end for pRow loop
				}//end for pCol loop
				 //Find Right Side
				 //Go through pixel columns
				for (int pColW = cellW - 1; pColW >= 0; --pColW)
				{
					//Go through pixel rows
					for (int pRowW = 0; pRowW < cellH; ++pRowW)
					{
						//Get the pixel offsets
						int pX = (cellW * cols) + pColW;
						int pY = (cellH * rows) + pRowW;

						//If a non colorkey pixel is found
						if (bitmap->getPixel32(pX, pY) != bgColor)
						{
							//Set the width
							mChars[currentChar].w = (pX - mChars[currentChar].x) + 1;

							//Break the loops
							pColW = -1;
							pRowW = cellH;
						}//end if pixel is not background color
					}//end for pRow loop
				}//end for pCol loop
				 //Find Top
				 //Go through pixel rows
				for (int pRow = 0; pRow < cellH; ++pRow)
				{
					//Go through pixel columns
					for (int pCol = 0; pCol < cellW; ++pCol)
					{
						//Get the pixel offsets
						int pX = (cellW * cols) + pCol;
						int pY = (cellH * rows) + pRow;

						//If a non colorkey pixel is found
						if (bitmap->getPixel32(pX, pY) != bgColor)
						{
							//If new top is found
							if (pRow < top)
							{
								top = pRow;
							}

							//Break the loops
							pCol = cellW;
							pRow = cellH;
						}//end if pixel is not background color
					}//end for pRow loop
				}//end for pCol loop
				 //Find Bottom of A
				if (currentChar == 'A')
				{
					//Go through pixel rows
					for (int pRow = cellH - 1; pRow >= 0; --pRow)
					{
						//Go through pixel columns
						for (int pCol = 0; pCol < cellW; ++pCol)
						{
							//Get the pixel offsets
							int pX = (cellW * cols) + pCol;
							int pY = (cellH * rows) + pRow;

							//If a non colorkey pixel is found
							if (bitmap->getPixel32(pX, pY) != bgColor)
							{
								//Bottom of a is found
								baseA = pRow+1;

								//Break the loops
								pCol = cellW;
								pRow = -1;
							}//end if pixel is not background color
						}//end for pRow loop
					}//end for pCol loop
				}//If current char is "A"

				//Go to the next character
				++currentChar;
			}//end for cols loop
		}//end for rows loop

		//Calcualte space
		mSpace = cellW / 2;

		//Calculate new line
		mNewLine = baseA - top;

		//Lop off excess top pixels
		for (int i = 0; i < 256; ++i)
		{
			mChars[i].y += top;
			mChars[i].h -= top;
		}

		bitmap->unlockTexture();
		mBitmap = bitmap;
	}//end else texter locked

	return success;
}//end method buildfont

void OBitmapFont::renderText(int x, int y, std::string text)
{
	//If the font has been built
	if (mBitmap != NULL)
	{
		//Temp offsets
		int curX = x, curY = y;
		
		//Go though the text
		for (int i = 0; i < text.length(); ++i)
		{
			//If the current character is a space
			if (text[i] == ' ')
			{
				//Move over
				curX += mSpace;
			}
			//If the current character is a newline
			else if (text[i] == '\n')
			{
				//Move down
				curY += mNewLine;

				//Move back
				curX = x;
			}
			else
			{
				//Get the ASCII value of the character
				int ascii = (unsigned char)text[i];

				//Show the character
				mBitmap->render(curX, curY, &mChars[ascii]);

				//Move over the width of the character with one pixel of padding
				curX += mChars[ascii].w + 1;
			}//End if/else if/else carater detection
		}//end for text length
	}//end if bitmat isn't null
}//end method renderText