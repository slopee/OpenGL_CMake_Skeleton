#pragma once

class Heightmap
{
public:
	Heightmap(int width, int height);
	~Heightmap() = default;
	
	void BindTexture() const;
private:
	void DebugPrint(int width, int height, unsigned char* textureData);
	unsigned char* GenerateStairsHeightMap(int width, int height);

private:
	unsigned int m_TextureId;
	
};
