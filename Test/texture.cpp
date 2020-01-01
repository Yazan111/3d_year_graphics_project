#include <stdio.h>
#include <windows.h>
#include <gl.h>			// Header File For The OpenGL32 Library
#include <glu.h>			// Header File For The GLu32 Library
#include <glaux.h>		// Header File For The Glaux Library
#include "texture.h"
#include <glut.h>
#include <iostream>

#include "stb_image.h"


/**********************************************************
*
* VARIABLES DECLARATION
*
*********************************************************/

int num_texture = -1; //Counter to keep track of the last loaded texture



					  /**********************************************************
					  *
					  * FUNCTION LoadBitmap(char *)
					  *
					  * This function loads a bitmap file and return the OpenGL reference ID to use that texture
					  *
					  *********************************************************/

// added by Yazan Aamer
int LoadTexture(char *filename, int alpha)
{
	unsigned int texture;


	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data;

	if(strstr(filename, ".png")) {

		data = stbi_load(filename, &width, &height, &nrChannels, STBI_rgb_alpha);
		if (data) {
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}

		else {
			std::cout << "failed to load PNG";
		}

	}else {
		data = stbi_load(filename, &width, &height, &nrChannels, STBI_rgb);
		if (data) {
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else {
			std::cout << "failed to load JPG";
		}

		stbi_image_free(data);

	}

	return texture;
}


Texture::Texture()
{
}


Texture::~Texture()
{
}
