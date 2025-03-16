#include "Loader.h"

#include <algorithm>

void s21::Loader::LoadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return;
    }
    std::string line;

    std::vector<Vertex> tempVertices;
    std::vector<unsigned int> tempIndices;
    std::string tempMeshname;
    Material tempMaterial;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;
        if (prefix == "v") {
            Vector3 vertex;
            if (iss >> vertex.x >> vertex.y >> vertex.z) {
                m_verticies.push_back(vertex);
            } else {
                std::cerr << "Error reading vertex data: " << line << std::endl;
            }
        } else if (prefix == "vt") {
            Vector2 texCoord;
            if (iss >> texCoord.x >> texCoord.y) {
                m_texCoords.push_back(texCoord);
            } else {
                std::cerr << "Error reading texture coordinate data: " << line << std::endl;
            }
        } else if (prefix == "vn") {
            Vector3 normal;
            if (iss >> normal.x >> normal.y >> normal.z) {
                m_normals.push_back(normal);
            }  else {
                std::cerr << "Error reading normal data: " << line << std::endl;
            }
        } else if (prefix == "o" || prefix == "g") {
            iss >> tempMeshname;
        } else if (prefix == "f") {
            std::vector<Vertex> verts;
            ParseFace(verts, iss);
            for (int i = 0; i < int(verts.size()); i++) {
                tempVertices.push_back(verts[i]);
            }
            std::vector<unsigned int> indices;
            VertexTriangluation(indices, verts);
            for (int i = 0; i < int(indices.size()); i++) {
                unsigned int indnum = (unsigned int)((tempVertices.size()) - verts.size()) + indices[i];
                tempIndices.push_back(indnum);
            }
        } else if (prefix == "usemtl") {
            try {
                SetMeshMaterial(tempIndices, tempVertices, tempMeshname, iss, tempMaterial);
            } catch (const std::bad_alloc& e) {
                std::cerr << "Ошибка выделения памяти: " << e.what() << std::endl;
                return;
            }
        } else if (prefix == "mtllib") {
            ParseMaterial(filename, iss);
        }
    }
    if (!tempIndices.empty() && !tempVertices.empty()) {
        Mesh tempMesh(tempVertices, tempIndices);
        tempMesh.m_material = tempMaterial;
        m_meshes.emplace_back(tempMesh);
        tempIndices.clear();
        tempIndices.clear();
    }  else {
        std::cerr << "No vertices or indices found for the last mesh." << std::endl;
    }
    std::cout << m_meshes.size() << std::endl;

    file.close();
}

void s21::Loader::ParseMaterial(const std::string& filename, std::istringstream& iss) {
    std::string path = "";
    std::string directory = "";
    size_t lastSlashPos = filename.find_last_of("/\\");
    if (lastSlashPos != std::string::npos) {
        directory = filename.substr(0, lastSlashPos);
    }
    std::getline(iss, path);
    if (!path.empty() && path[0] == ' ') {
        path.erase(0, 1);
    }
    path.pop_back();
    std::string wholePath = directory + "/" + path;

    LoadMaterials(wholePath);
}

void s21::Loader::SetMeshMaterial(std::vector<unsigned int>& tempIndices, std::vector<s21::Vertex>& tempVertices,
                                  std::string& tempMeshname, std::istringstream& iss, Material& tempMaterial) {
    std::string materialName;
    iss >> materialName;

    std::cout << materialName << std::endl;
    auto it = m_materials.find(materialName);

    if (it != m_materials.end()) {
        tempMaterial = it->second;
    } else {
        std::cerr << "Material not found: " << materialName << std::endl;
    }
    if (!tempIndices.empty() && !tempVertices.empty()) {
        Mesh tempMesh(tempVertices, tempIndices);
        tempMesh.m_meshName = tempMeshname;
       
        tempMesh.m_material = tempMaterial;
        m_meshes.emplace_back(tempMesh);

        tempIndices.clear();
        tempVertices.clear();
    }
}

void s21::Loader::ParseFace(std::vector<Vertex>& verts, std::istringstream& stream) {
    std::string token;
    Vertex vertex;
    bool noNormal = false;
    while (stream >> token) {
        size_t firstSlash = token.find("/");
        size_t secondSlash = token.find("/", firstSlash + 1);
        vertex.position = m_verticies[std::stoi(token.substr(0, firstSlash)) - 1];
        if (firstSlash != std::string::npos) {
            if (secondSlash != std::string::npos) {
                std::string tex = token.substr(firstSlash + 1, secondSlash - firstSlash - 1);
                vertex.texture = tex.empty() ? Vector2(0, 0) : m_texCoords[std::stoi(tex) - 1];
                vertex.normal = m_normals[std::stoi(token.substr(secondSlash + 1)) - 1];
                RoundNormal(vertex.normal);
            } else {
                vertex.texture = m_texCoords[std::stoi(token.substr(firstSlash + 1)) - 1];
                noNormal = true;
            }
        } else {
            vertex.texture = Vector2(0, 0);
            noNormal = true;
        }
        verts.push_back(vertex);
    }
    if (noNormal) {
        Vector3 normal;
        for (int i = 0; i < verts.size(); i++) {
            if (!(verts[i].normal == Vector3(0, 0, 0))) {
                normal = verts[i].normal;
                break;
            }
        }
        Vector3 A = verts[0].position - verts[1].position;
        Vector3 B = verts[2].position - verts[1].position;
        normal = normalize(A.cross(B));
        RoundNormal(normal);
        for (auto& vert : verts) {
            vert.normal = (normal * -1);
        }
    }
}

void s21::Loader::VertexTriangluation(std::vector<unsigned int>& indices, const std::vector<Vertex>& verts) {
    if (verts.size() == 3) {
        indices = {0, 1, 2};
        return;
    }
    std::vector<Vertex> tempVerts = verts;

    while (true) {
        for (int i = 0; i < int(tempVerts.size()); i++) {
            Vertex pPrev = tempVerts[(i - 1 + tempVerts.size()) % tempVerts.size()];
            Vertex pCur = tempVerts[i];
            Vertex pNext = tempVerts[(i + 1) % tempVerts.size()];
            if (tempVerts.size() == 3) {
                for (int j = 0; j < 3; j++) {
                    indices.push_back(j);
                }

                tempVerts.clear();
                break;
            }
            if (tempVerts.size() == 4) {
                for (const auto& vert : verts) {
                    if (vert.position == pCur.position || vert.position == pPrev.position ||
                        vert.position == pNext.position) {
                        indices.push_back(&vert - &verts[0]);
                    }
                }

                Vector3 tempVec;
                for (const auto& vert : tempVerts) {
                    if (!(vert.position == pCur.position) && !(vert.position == pPrev.position) &&
                        !(vert.position == pNext.position)) {
                        tempVec = vert.position;
                        break;
                    }
                }

                for (const auto& vert : verts) {
                    if (vert.position == pPrev.position || vert.position == pNext.position ||
                        vert.position == tempVec) {
                        indices.push_back(&vert - &verts[0]);
                    }
                }
                tempVerts.clear();
                break;
            }
        }
        if (indices.size() == 0) break;

        if (tempVerts.size() == 0) break;
    }
}

std::vector<s21::Mesh> s21::Loader::GetMeshes() const { return m_meshes; }

std::vector<s21::FaceVertex> s21::Loader::GetFaces() const { return m_faceVerticies; }
std::vector<unsigned int> s21::Loader::GetIndices() const { return m_indices; }
void s21::Loader::PrintLoadedInfo() {
    std::cout << "Loaded Vertices: " << m_vertices.size() << std::endl;
    for (const auto& vertex : m_vertices) {
        std::cout << "" << vertex.position.x << ", " << vertex.position.y << ", " << vertex.position.z << " "
                  << vertex.normal.x << ", " << vertex.normal.y << ", " << vertex.normal.z << std::endl;
    }

    std::cout << "Loaded Texture Coordinates: " << m_texCoords.size() << std::endl;
    for (const auto& texCoord : m_texCoords) {
        std::cout << "Texture Coordinate: (" << texCoord.x << ", " << texCoord.y << ")" << std::endl;
    }

    std::cout << "Loaded Normals: " << m_normals.size() << std::endl;
    for (const auto& normal : m_normals) {
        std::cout << "Normal: (" << normal.x << ", " << normal.y << ", " << normal.z << ")" << std::endl;
    }

    std::cout << "Loaded Faces: " << m_faceVerticies.size() << std::endl;
    for (size_t i = 0; i < m_faceVerticies.size(); i += 3) {
        std::cout << "Face " << (i / 3) + 1 << ":" << std::endl;
        std::cout << "  Vertex Index 1: " << m_faceVerticies[i].vertexIndex
                  << ", Texture Index: " << m_faceVerticies[i].textureIndex
                  << ", Normal Index: " << m_faceVerticies[i].normalIndex << std::endl;
        std::cout << "  Vertex Index 2: " << m_faceVerticies[i + 1].vertexIndex
                  << ", Texture Index: " << m_faceVerticies[i + 1].textureIndex
                  << ", Normal Index: " << m_faceVerticies[i + 1].normalIndex << std::endl;
        std::cout << "  Vertex Index 3: " << m_faceVerticies[i + 2].vertexIndex
                  << ", Texture Index: " << m_faceVerticies[i + 2].textureIndex
                  << ", Normal Index: " << m_faceVerticies[i + 2].normalIndex << std::endl;
    }

    std::cout << m_meshes.size() << std::endl;
}

void s21::Loader::Clear() {
    m_vertices.clear();
    m_verticies.clear();
    m_normals.clear();
    m_texCoords.clear();
    m_faceVerticies.clear();
    m_meshes.clear();
}

void s21::Loader::RoundNormal(Vector3& normal) {
    if (std::abs(normal.x) <= 1e-5) normal.x = 0.0;
    if (std::abs(normal.y) <= 1e-5) normal.y = 0.0;
    if (std::abs(normal.z) <= 1e-5) normal.z = 0.0;
}

bool s21::Loader::LoadMaterials(const std::string& path) {
    Material tempMaterial;
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Could not open file:" << path << std::endl;
        return false;
    }
    std::string line;
    Material newMat;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;
        if (prefix == "newmtl") {
            if (!newMat.name.empty()) {
                m_materials[newMat.name] = newMat;
                newMat = Material();
            }
            iss >> newMat.name;
        } else if (prefix == "Ns") {
            if (!(iss >> newMat.Ns)) {
                std::cerr << "Error reading Ns for material: " << newMat.name << std::endl;
            }
        } else if (prefix == "d") {
            if (!(iss >> newMat.d)) {
                std::cerr << "Error reading d for material: " << newMat.name << std::endl;
            }
        } else if (prefix == "illum") {
            if (!(iss >> newMat.illum)) {
                std::cerr << "Error reading illum for material: " << newMat.name << std::endl;
            }
        } else if (prefix == "Ni") {
            if (!(iss >> newMat.Ni)) {
                std::cerr << "Error reading Ni for material: " << newMat.name << std::endl;
            }
        } else if (prefix == "Ka") {
            Vector3 ambient;
            if (iss >> ambient.x >> ambient.y >> ambient.z) {
                newMat.ambient = ambient;
            } else {
                std::cerr << "Error reading Ka for material: " << newMat.name << std::endl;
            }
        } else if (prefix == "Kd") {
            Vector3 diffuse;
            if (iss >> diffuse.x >> diffuse.y >> diffuse.z) {
                newMat.diffuse = diffuse;
            } else {
                std::cerr << "Error reading Kd for material: " << newMat.name << std::endl;
            }
        } else if (prefix == "Ks") {
            Vector3 spec;
            if (iss >> spec.x >> spec.y >> spec.z) {
                newMat.specular = spec;
            } else {
                std::cerr << "Error reading Ks for material: " << newMat.name << std::endl;
            }
        }
    }

    if (!newMat.name.empty()) m_materials[newMat.name] = newMat;
    return !m_materials.empty();
}
