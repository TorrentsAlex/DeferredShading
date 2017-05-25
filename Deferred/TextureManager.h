#ifndef TURRI_TEXTURE_MANAGER
#define TURRI_TEXTURE_MANAGER

#include <string>
#include <vector>
#include <iostream>
#include <tuple>

//Third-party libraries
#include <GL/glew.h>			//The OpenGL Extension Wrangler
#include <glm/glm.hpp>			//OpenGL Mathematics


const std::pair<std::string, GLuint > cubemapID[6] = {
	{"ft", GL_TEXTURE_CUBE_MAP_POSITIVE_X },
	{ "lf", GL_TEXTURE_CUBE_MAP_NEGATIVE_X },
	{ "up", GL_TEXTURE_CUBE_MAP_POSITIVE_Y },
	{ "dn", GL_TEXTURE_CUBE_MAP_NEGATIVE_Y },
	{ "bk", GL_TEXTURE_CUBE_MAP_POSITIVE_Z },
	{ "tp", GL_TEXTURE_CUBE_MAP_NEGATIVE_Z }
};

struct GLTexture {
	GLuint id;
	int width;
	int height;
};

//This class manages all the data related to the different textures used in the game
class TextureManager {
public:
	GLuint getTextureID(std::string filePath);
	GLuint getTextureCubemapID(std::string type, std::string filePath);
	// Pattern singleton
	static TextureManager& Instance() {
		static TextureManager instance;
		return instance;
	}
	TextureManager(TextureManager const&) = delete;
	void operator=(TextureManager const&) = delete;

	void deleteTextures();

private:
	TextureManager() {};


	GLuint loadTexture(std::string filePath);
	GLuint load3DTexture(GLuint position, std::string filePath);

	std::vector <GLTexture> _textureData;
	std::vector <std::string> _listOfTextures;
};

#endif
