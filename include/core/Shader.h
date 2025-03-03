#ifndef CE_SHADER_H
#define CE_SHADER_H

#include <core/Texture.h>
#include <glm/glm.hpp>
#include <string>

class Shader {
public:
    
    unsigned int id;
    char* vertexShaderSource;
    char* fragmentShaderSource;
    unsigned int vertexShader;
    unsigned int fragmentShader;
    bool compiled;

    Shader();
    Shader(const char* pathToVertexShader, const char* pathToFragmentShader, bool autocompile=true);

    void compile();
    void use();
    void setInt(const std::string& name, int value);
    void setBool(const std::string& name, bool value);
    void setFloat(const std::string& name, float value);
    void setTexture(const std::string& name, const Texture& value);
    void setVec2(const std::string& name, const glm::vec2& value);
    void setVec3(const std::string& name, const glm::vec3& value);
    void setVec4(const std::string& name, const glm::vec4& value);
    void setMat2(const std::string& name, const glm::mat2& value);
    void setMat3(const std::string& name, const glm::mat3& value);
    void setMat4(const std::string& name, const glm::mat4& value);
    void setMat2x3(const std::string& name, const glm::mat2x3& value);
    void setMat2x4(const std::string& name, const glm::mat2x4& value);
    void setMat3x2(const std::string& name, const glm::mat3x2& value);
    void setMat3x4(const std::string& name, const glm::mat3x4& value);
    void setMat4x2(const std::string& name, const glm::mat4x2& value);
    void setMat4x3(const std::string& name, const glm::mat4x3& value);
};

#endif
