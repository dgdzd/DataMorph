#include <core/Texture.h>

#include <iostream>
#include <stb/stb_image.h>

Texture::Texture() {
    isEmpty = true;
}

Texture::Texture(int width, int height) {
    this->width = width;
    this->height = height;
    isEmpty = true;
}

Texture::Texture(const char* pathToImage, bool autogenerate) {
    stbi_set_flip_vertically_on_load(true);
    glGenTextures(1, &id);
    data = stbi_load(pathToImage, &width, &height, &nrChannels, 0);

    if(data) {
        if(autogenerate) {
            glBindTexture(GL_TEXTURE_2D, id);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            generatedImg = true;
            generatedMipmap = true;
        }
    } else {
        //Logger::CampEngine.error("Failed to load image at: "+std::string(pathToImage));
        isEmpty = true;
    }
    isEmpty = (data == nullptr);
}

void Texture::generateTextureImage() {
    if(!generatedImg) {
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        generatedImg = true;
    } else {
        //Logger::CampEngine.error("Already loaded image!");
    }
}

void Texture::generateMipmap() {
    if(generatedImg) {
        if(!generatedMipmap) {
            glBindTexture(GL_TEXTURE_2D, id);
            glGenerateMipmap(GL_TEXTURE_2D);

            generatedMipmap = true;
        } else {
            std::cout << "ERROR: Already generated texture mipmap!" << std::endl;
        }
    } else {
        std::cout << "ERROR: Please generate the texture image before generating the mipmap!" << std::endl;

    }
}

void Texture::use() {
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::generate(int width, int height, unsigned char* data, int internalFormat) {
    this->width = width;
    this->height = height;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    int format;
    switch(internalFormat) {
        case 1:
        case 2:
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            format = internalFormat;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    generatedImg = true;
    isEmpty = (data == nullptr);
}
