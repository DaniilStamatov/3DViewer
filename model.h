#pragma once

#include <fstream>
#include <vector>

#include "transformations/transformation.h"
namespace s21 {
struct Vertex {
    Vector3 position;
    Vector3 normal;
    Vector2 texture;
};

struct FaceVertex {
    int vertexIndex;
    int textureIndex;
    int normalIndex;
};


struct Material {
    Material() : name("Default Material"), ambient(0.0f, 0.1f, 0.6f), diffuse(0.0f, 0.50980392f, 0.50980392f), 
                specular(0.50196078f, 0.50196078f, 0.50196078f), Ns(10.0f), Ni(1.0f), d(1.0f), illum(2) {
    }
    std::string name;
    Vector3 ambient;
    Vector3 diffuse;
    Vector3 specular;
    float Ns; // Specular Exponent
    float Ni; // Optical Density
    float d;  // Dissolve
    int illum; // Illumination
    std::string map_Ka; // Ambient Texture Map
    std::string map_Kd; // Diffuse Texture Map
    std::string map_Ks; // Specular Texture Map
    std::string map_Ns; // Specular Highlight Map
    std::string map_d;  // Alpha Texture Map
    std::string map_bump; // Bump Map

    void print() const {
        std::cout << "Material Name: " << name << std::endl;
        std::cout << "Ambient: "; ambient.print(); std::cout << std::endl;
        std::cout << "Diffuse: "; diffuse.print(); std::cout << std::endl;
        std::cout << "Specular: "; specular.print(); std::cout << std::endl;
        std::cout << "Specular Exponent (Ns): " << Ns << std::endl;
        std::cout << "Optical Density (Ni): " << Ni << std::endl;
        std::cout << "Dissolve (d): " << d << std::endl;
        std::cout << "Illumination Model (illum): " << illum << std::endl;
        std::cout << "Ambient Texture Map (map_Ka): " << map_Ka << std::endl;
        std::cout << "Diffuse Texture Map (map_Kd): " << map_Kd << std::endl;
        std::cout << "Specular Texture Map (map_Ks): " << map_Ks << std::endl;
        std::cout << "Specular Highlight Map (map_Ns): " << map_Ns << std::endl;
        std::cout << "Alpha Texture Map (map_d): " << map_d << std::endl;
        std::cout << "Bump Map (map_bump): " << map_bump << std::endl;
    }
};

class Model {
   public:
    void SetVerticies(const std::vector<Vector3> &otherVerticies);
    void SetNormals(const std::vector<Vector2> &otherTexCoords);
    void SetTexCoords(const std::vector<Vector3> &otherNormals);

   private:
    std::vector<Vector3> m_verticies;
    std::vector<Vector3> m_normals;
    std::vector<Vector2> m_texCoords;
};
}  // namespace s21
