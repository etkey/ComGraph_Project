#ifndef PLANE_CLASS_H
#define PLANE_CLASS_H

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"
#include "Texture.h"
#include "shaderClass.h"
#include <memory>

class Plane {
public:
    // Constructor that generates a plane with textures
    Plane(const char* diffPath, const char* normalPath, const char* roughPath,
          float repeatX = 1.0f, float repeatY = 1.0f);
    
    // Prevent copying
    Plane(const Plane&) = delete;
    Plane& operator=(const Plane&) = delete;
    
    // Allow moving
    Plane(Plane&& other) noexcept;
    Plane& operator=(Plane&& other) noexcept;
    
    // Draws the plane with the specified shader, camera and transformation
    void Draw(Shader& shader, Camera& camera, glm::mat4 matrix = glm::mat4(1.0f));
    
    // Deletes all the objects
    void Delete();
    
    // Destructor
    ~Plane();

private:
    // Initialize the plane geometry
    void InitializeGeometry(float repeatX, float repeatY);

private:
    // OpenGL objects
    VAO vao;
    std::unique_ptr<VBO> vbo;
    std::unique_ptr<EBO> ebo;
    
    // Textures
    std::unique_ptr<Texture> diffuseMap;
    std::unique_ptr<Texture> normalMap;
    std::unique_ptr<Texture> roughnessMap;
    
    // Geometry data
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    
    // Track if resources have been initialized
    bool initialized = false;
}; 

#endif 