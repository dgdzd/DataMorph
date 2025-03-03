#include <core/Shader.h>

#include <glad/glad.h>

#include <core/FileHandler.h>

Shader::Shader() {
    id = -1;
}

Shader::Shader(const char* pathToVertexShader, const char* pathToFragmentShader, bool autocompile) {
    compiled = false;
    vertexShaderSource = files::get_content(pathToVertexShader);
    fragmentShaderSource = files::get_content(pathToFragmentShader);

    if(autocompile) {
        compile();
    }
}

void Shader::compile() {
    if(!compiled) {
        int success;
        char infoLog[512];

        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            logger.error("Failed to compile vertex shader\n" + std::string(infoLog));
        }

        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            logger.error("Failed to compile fragment shader\n" + std::string(infoLog));
        }

        id = glCreateProgram();
        glAttachShader(id, vertexShader);
        glAttachShader(id, fragmentShader);
        glLinkProgram(id);
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(id, 512, NULL, infoLog);
            logger.error("Failed to link vertex shader and fragment shader\n" + std::string(infoLog));
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        compiled = true;
    } else {
        logger.warn("The shader is already compiled!");
    }
}
    
void Shader::use() {
    glUseProgram(id);
}

void Shader::setInt(const std::string& name, int value) {
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setBool(const std::string& name, bool value) {
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::setFloat(const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setTexture(const std::string& name, const Texture& value) {
    glUniform1i(glGetUniformLocation(id, name.c_str()), value.id);
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) {
    glUniform2f(glGetUniformLocation(id, name.c_str()), value.x, value.y);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) {
    glUniform3f(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z);
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) {
    glUniform4f(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::setMat2(const std::string& name, const glm::mat2& value) {
    glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMat3(const std::string& name, const glm::mat3& value) {
    glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) {
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMat2x3(const std::string& name, const glm::mat2x3& value) {
    glUniformMatrix2x3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMat2x4(const std::string& name, const glm::mat2x4& value) {
    glUniformMatrix2x4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMat3x2(const std::string& name, const glm::mat3x2& value) {
    glUniformMatrix3x2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMat3x4(const std::string& name, const glm::mat3x4& value) {
    glUniformMatrix3x4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMat4x2(const std::string& name, const glm::mat4x2& value) {
    glUniformMatrix4x2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMat4x3(const std::string& name, const glm::mat4x3& value) {
    glUniformMatrix4x3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &value[0][0]);
}
