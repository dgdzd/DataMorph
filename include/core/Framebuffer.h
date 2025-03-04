#include <core/Texture.h>

class Framebuffer {
	unsigned int FBO;
	unsigned int RBO;
	unsigned int VAO;

	void init();

public:
	Texture texture;
	unsigned int width;
	unsigned int height;

	Framebuffer(unsigned int width, unsigned int height);

	void use();
	void unuse();
	void render();
};