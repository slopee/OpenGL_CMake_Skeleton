#include "Heightmap.h"
#include <cstddef>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <GL/glew.h>

//---------------------------------------------------------------------------------------------------------------------
Heightmap::Heightmap(int width, int height)
{	
	unsigned char* textureData = GenerateStairsHeightMap(width, height);
	DebugPrint(width, height, textureData);

	glGenTextures(1, &m_TextureId);
	glBindTexture(GL_TEXTURE_2D, m_TextureId);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
	delete[] textureData;
}

//---------------------------------------------------------------------------------------------------------------------
void Heightmap::BindTexture() const
{
	glBindTexture(GL_TEXTURE_2D, m_TextureId);
}

//---------------------------------------------------------------------------------------------------------------------
void Heightmap::DebugPrint(int width, int height, unsigned char* textureData)
{
	int n = 0;
	std::cout.fill('0');
	for (int i = 0; i < width; ++i)
	{
		std::cout << i << ": ";
		for (int j = 0; j < height; ++j)
		{
			if (j != 0)
			{
				std::cout << " - ";
			}

			std::cout << "(" << std::setw(3) << (int)textureData[n] << ",";
			std::cout << std::setw(3) << (int)textureData[n + 1] << ",";
			std::cout << std::setw(3) << (int)textureData[n + 2] << ")";
			n += 3;
		}
		std::cout << std::endl;
	}
}

//---------------------------------------------------------------------------------------------------------------------
unsigned char* Heightmap::GenerateStairsHeightMap(int width, int height)
{
	const int halfIteration = (width * height * 3) / 2;
	unsigned char* textureData = new unsigned char[width * height * 3];

	int currentHeight = 0;
	int heightIncrement = static_cast<int>(255/(height/2)) + 1;
	int lastHeightIteration = 0;

	for (int i = 0, iteration = 0; i < width * height * 3; i += 3, ++iteration)
	{
		if (iteration > 0)
		{
			int hIteration = iteration / height;
			int wIteration = iteration % height;

			currentHeight = 0;

			if (lastHeightIteration != hIteration)
			{
				if (lastHeightIteration != 0)
				{
					if (heightIncrement > 0)
					{
						heightIncrement = -(heightIncrement / 2);
					}
					else
					{
						heightIncrement *= -2;
					}					
				}
				lastHeightIteration = hIteration;
			}

			if (hIteration >= 1)
			{
				currentHeight = (int)textureData[(iteration - height) * 3];
				if (hIteration != height / 2)
				{
					if (hIteration <= wIteration && hIteration <= (width - wIteration - 1))
					{
						currentHeight += heightIncrement;
					}
					else if (hIteration > height / 2 && (height - hIteration <= wIteration && height - hIteration <= (width - wIteration - 1)))
					{
						currentHeight -= heightIncrement;
					}
				}
			}
		}

		currentHeight = std::min(255, std::max(0, currentHeight));
		//currentHeight = 255;
		textureData[i] = currentHeight; // R
		textureData[i + 1] = currentHeight; // G
		textureData[i + 2] = currentHeight; // B
	}

	return textureData;
}
