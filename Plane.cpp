#include "Plane.h"
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>

Plane::Plane(const char* diffPath, const char* normalPath, const char* roughPath,
    float repeatX, float repeatY) {
    try {
        // Initialize geometry data
        InitializeGeometry(repeatX, repeatY);

        // Generate and bind VAO
        vao.Bind();

        // Create and initialize VBO
        vbo = std::make_unique<VBO>(vertices);

        // Create and initialize EBO
        ebo = std::make_unique<EBO>(indices);

        // Link VBO attributes to VAO
        vao.LinkAttrib(*vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);                   // Position
        vao.LinkAttrib(*vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float))); // Normal
        vao.LinkAttrib(*vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float))); // Color
        vao.LinkAttrib(*vbo, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float))); // Texture

        // Unbind all
        vao.Unbind();
        vbo->Unbind();
        ebo->Unbind();

        // Create textures
        diffuseMap = std::make_unique<Texture>(diffPath, "diffuse0", 0);
        normalMap = std::make_unique<Texture>(normalPath, "normal0", 1);
        roughnessMap = std::make_unique<Texture>(roughPath, "roughness0", 2);

        if (diffuseMap) {
            diffuseMap->Bind();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }

        if (normalMap) {
            normalMap->Bind();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }

        if (roughnessMap) {
            roughnessMap->Bind();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }

        initialized = true;
    }
    catch (const std::exception& e) {
        // Clean up any resources that were created
        Delete();
        throw std::runtime_error(std::string("Failed to initialize Plane: ") + e.what());
    }
}

void Plane::InitializeGeometry(float repeatX, float repeatY) {
    // Vertices for a 1x1 vertical plane in XY plane (Z forward)
    vertices = {
        Vertex{glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
        Vertex{glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, repeatY)},
        Vertex{glm::vec3(0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(repeatX, repeatY)},
        Vertex{glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(repeatX, 0.0f)}
    };

    // Indices for the plane
    indices = {
        0, 1, 2,
        0, 2, 3
    };
}

Plane::Plane(Plane&& other) noexcept
    : vao(std::move(other.vao))
    , vbo(std::move(other.vbo))
    , ebo(std::move(other.ebo))
    , diffuseMap(std::move(other.diffuseMap))
    , normalMap(std::move(other.normalMap))
    , roughnessMap(std::move(other.roughnessMap))
    , vertices(std::move(other.vertices))
    , indices(std::move(other.indices))
    , initialized(other.initialized)
{
    other.initialized = false;
}

Plane& Plane::operator=(Plane&& other) noexcept {
    if (this != &other) {
        Delete();
        
        vao = std::move(other.vao);
        vbo = std::move(other.vbo);
        ebo = std::move(other.ebo);
        diffuseMap = std::move(other.diffuseMap);
        normalMap = std::move(other.normalMap);
        roughnessMap = std::move(other.roughnessMap);
        vertices = std::move(other.vertices);
        indices = std::move(other.indices);
        initialized = other.initialized;
        
        other.initialized = false;
    }
    return *this;
}

void Plane::Draw(Shader& shader, Camera& camera, glm::mat4 matrix) {
    if (!initialized) {
        throw std::runtime_error("Attempting to draw uninitialized Plane");
    }

    shader.Activate();
    vao.Bind();

    // Bind textures
    if (diffuseMap) diffuseMap->Bind();
    if (normalMap) normalMap->Bind();
    if (roughnessMap) roughnessMap->Bind();

    // Pass the camera position
    glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
    camera.Matrix(shader, "camMatrix");
    
    // Pass the model matrix
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(matrix));

    // Draw the plane
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
}

void Plane::Delete() {
    if (initialized) {
        vao.Delete();
        if (vbo) vbo->Delete();
        if (ebo) ebo->Delete();
        if (diffuseMap) diffuseMap->Delete();
        if (normalMap) normalMap->Delete();
        if (roughnessMap) roughnessMap->Delete();
        initialized = false;
    }
}

Plane::~Plane() {
    Delete();
} 