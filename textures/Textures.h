#pragma once
#include "../src/Renderer.h"
#include <vector>

class Texture
{
public:
	Texture(const std::string &path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;
	void AddNewTexture(const std::string& path);
	void ChangeImage();

private:

	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char *m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
	
	std::vector <std::string> buffList;
	int buffListPos = 0;

	bool reverse = false;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};
