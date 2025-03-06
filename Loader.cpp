#include "Loader.h"

void s21::Loader::LoadFromFile(const std::string &filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;
        if (prefix == "v") {
            Vector3 vertex;
            if (iss >> vertex.x >> vertex.y >> vertex.z) {
                m_verticies.push_back(vertex);
            }
        } else if (prefix == "f") {
            ParseFace(iss);
        } else if (prefix == "vt") {
            Vector2 texCoord;
            if (iss >> texCoord.x >> texCoord.y) {
                m_texCoords.push_back(texCoord);
            }
        } else if (prefix == "vn") {
            Vector3 normal;
            if (iss >> normal.x >> normal.y) {
                m_normals.push_back(normal);
            }
        }
    }
}

void s21::Loader::ParseFace(std::istringstream &stream) {
    std::vector<FaceVertex> faceVertices;
    std::string token;
    while (stream >> token) {
        size_t firstSlash = token.find("/");
        size_t secondSlash = token.find("/", firstSlash + 1);
        FaceVertex faceVertex;
        faceVertex.vertexIndex = std::stoi(token.substr(0, firstSlash)) - 1;
        if (firstSlash != std::string::npos) {
            if (secondSlash != std::string::npos) {
                std::string tex = token.substr(firstSlash + 1, secondSlash - firstSlash - 1);
                faceVertex.textureIndex = tex.empty() ? -1 : std::stoi(tex) - 1;
                faceVertex.normalIndex = std::stoi(token.substr(secondSlash + 1)) - 1;
            } else {
                faceVertex.textureIndex = std::stoi(token.substr(firstSlash + 1)) - 1;
                faceVertex.normalIndex = -1;
            }
        } else {
            faceVertex.textureIndex = -1;
            faceVertex.normalIndex = -1;
        }
        faceVertices.push_back(faceVertex);
    }

    size_t n = faceVertices.size();
    m_faceVerticies.reserve(n - 2);
    for (size_t i = 1; i < n - 1; ++i) {
        FaceVertex v1 = faceVertices[0];
        FaceVertex v2 = faceVertices[i];
        FaceVertex v3 = faceVertices[i + 1];

        m_faceVerticies.push_back(v1);
        m_faceVerticies.push_back(v2);
        m_faceVerticies.push_back(v3);
    }
}

std::vector<s21::Vector3> s21::Loader::GetVerticies() const { return m_verticies; }

std::vector<s21::Vector2> s21::Loader::GetTexturesCoords() const { return m_texCoords; }

std::vector<s21::Vector3> s21::Loader::GetNormals() const { return m_normals; }

std::vector<s21::FaceVertex> s21::Loader::GetFaces() const { return m_faceVerticies; }

void s21::Loader::PrintLoadedInfo() {
    std::cout << "Loaded Vertices: " << m_verticies.size() << std::endl;
    for (const auto &vertex : m_verticies) {
        std::cout << "Vertex: (" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")" << std::endl;
    }

    std::cout << "Loaded Texture Coordinates: " << m_texCoords.size() << std::endl;
    for (const auto &texCoord : m_texCoords) {
        std::cout << "Texture Coordinate: (" << texCoord.x << ", " << texCoord.y << ")" << std::endl;
    }

    std::cout << "Loaded Normals: " << m_normals.size() << std::endl;
    for (const auto &normal : m_normals) {
        std::cout << "Normal: (" << normal.x << ", " << normal.y << ", " << normal.z << ")" << std::endl;
    }

    std::cout << "Loaded Faces: " << m_faceVerticies.size() << std::endl;
    for (const auto &face : m_faceVerticies) {
        std::cout << "Face: Vertex Index: " << face.vertexIndex << ", Texture Index: " << face.textureIndex
                  << ", Normal Index: " << face.normalIndex << std::endl;
    }
}

void s21::Loader::Clear() {
    m_verticies.clear();
    m_normals.clear();
    m_texCoords.clear();
    m_faceVerticies.clear();
}
