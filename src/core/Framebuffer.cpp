#include <core/Framebuffer.h>

Framebuffer::Framebuffer(unsigned int width, unsigned int height) {
	this->width = width;
	this->height = height;

    texture.generate(width, height, nullptr, GL_RGB);

    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.id, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        //Logger::CampEngine.error("Failed to initialize FBO");
    }
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        //Logger::CampEngine.error("Failed to initialize MSFBO");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    init();
}

void Framebuffer::init() {
    // Ici on initialise toutes les données concernant les vertex

    unsigned int VBO;
    float vertices[] = {
        //  Position      Texture
            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, 1.0f,   1.0f, 1.0f,
            -1.0f, 1.0f,  0.0f, 1.0f,

            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, -1.0f,  1.0f, 0.0f,
            1.0f, 1.0f,   1.0f, 1.0f

    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);

    // Pour chaque vertex qui fait 4x la taille d'un float, les deux premiers correspondent à la position.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    // Pour chaque vertex qui fait 4x la taille d'un float, les deux derniers correspondent aux coordonnées de la texture.
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Framebuffer::use() {
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void Framebuffer::unuse() {
    // 0 = le framebuffer par défaut (celui qui va s'afficher à l'écran au final)
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::render() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Framebuffer::rescale_buffer(float width, float height) {
    texture.generate(width, height, nullptr, GL_RGB);

    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.id, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        //Logger::CampEngine.error("Failed to initialize FBO");
    }
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        //Logger::CampEngine.error("Failed to initialize MSFBO");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}