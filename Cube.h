#ifndef CUBE_H
#define CUBE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <memory>

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"
#include "Texture.h"
#include "shaderClass.h"

class Cube {
private:
    std::unique_ptr<VAO> VAO1;
    std::unique_ptr<VBO> VBO1;
    std::unique_ptr<EBO> EBO1;
    std::vector<Texture> textures;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    void InitializeGeometry();

public:
    Cube(const std::vector<Texture>& textures);
    
    // Delete copy constructor and assignment operator
    Cube(const Cube&) = delete;
    Cube& operator=(const Cube&) = delete;
    
    // Allow move constructor and assignment operator
    Cube(Cube&&) noexcept = default;
    Cube& operator=(Cube&&) noexcept = default;

    void Draw(Shader& shader, Camera& camera, const glm::mat4& transform);
    void Delete();
    ~Cube();
};

#endif 