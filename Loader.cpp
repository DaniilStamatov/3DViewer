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
        } else if (prefix == "vt") {
            Vector2 texCoord;
            if (iss >> texCoord.x >> texCoord.y) {
                m_texCoords.push_back(texCoord);
            }
        } else if (prefix == "vn") {
            Vector3 normal;
            if (iss >> normal.x >> normal.y >> normal.z) {
                m_normals.push_back(normal);
            }
        } else if (prefix == "f") {
            std::vector<Vertex> verts;
            ParseFace(verts, iss);
            for (int i = 0; i < int(verts.size()); i++)
			{
				m_vertices.push_back(verts[i]);
			}
            std::vector<unsigned int> indices;
            VertexTriangluation(indices, verts);
            for(int i = 0; i < int(indices.size()); i++) {
                unsigned int indnum = (unsigned int)((m_vertices.size()) - verts.size()) + indices[i];
				m_indices.push_back(indnum);
            }
        }
    }

}

void s21::Loader::ParseFace(std::vector<Vertex>& verts, std::istringstream &stream) {
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
                vertex.texture = tex.empty() ? Vector2(0,0) : m_texCoords[std::stoi(tex) - 1];
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
    if(noNormal) {
        Vector3 normal;
        for(int i = 0; i < verts.size(); i++) {
            if(!(verts[i].normal == Vector3(0, 0, 0))) {
                normal = verts[i].normal;
                break;
            }
        }
        Vector3 A = verts[0].position - verts[1].position;
        Vector3 B = verts[2].position - verts[1].position;
        normal = normalize(A.cross(B));
        RoundNormal(normal);
        for(auto& vert:verts) {
            vert.normal = (normal * -1);
    }
}
}

void s21::Loader::VertexTriangluation(std::vector<unsigned int> &indices, const std::vector<Vertex>& verts) {
    if(verts.size() == 3) {
         indices = {0, 1, 2};
         return;
    }
    std::vector<Vertex> tempVerts = verts;

    while (true)
	{
        for (int i = 0; i < int(tempVerts.size()); i++)
		{
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
                    if (vert.position == pCur.position || vert.position == pPrev.position || vert.position == pNext.position) {
                        indices.push_back(&vert - &verts[0]); 
                    }
                }

                Vector3 tempVec;
                for (const auto& vert : tempVerts) {
                    if (!(vert.position == pCur.position) && !(vert.position == pPrev.position) && !(vert.position == pNext.position)) {
                        tempVec = vert.position;
                        break;
                    }
                }

                for (const auto& vert : verts) {
                    if (vert.position == pPrev.position || vert.position == pNext.position || vert.position == tempVec) {
                        indices.push_back(&vert - &verts[0]);
                    }
                }
                tempVerts.clear();
                break;
}
        }
        if (indices.size() == 0)
			break;

	    if (tempVerts.size() == 0)
			break;
    }
}

std::vector<s21::Vertex> s21::Loader::GetVerticies() const { return m_vertices; }

std::vector<s21::FaceVertex> s21::Loader::GetFaces() const { return m_faceVerticies; }
std::vector<unsigned int> s21::Loader::GetIndices() const { return m_indices; }
void s21::Loader::PrintLoadedInfo() {
    std::cout << "Loaded Vertices: " << m_vertices.size() << std::endl;
    for (const auto &vertex : m_vertices) {
        std::cout << "" << vertex.position.x << ", " << vertex.position.y << ", " << vertex.position.z << " " << vertex.normal.x << ", " << vertex.normal.y << ", " << vertex.normal.z << std::endl;
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
    for (size_t i = 0; i < m_faceVerticies.size(); i += 3) {
        std::cout << "Face " << (i / 3) + 1 << ":" << std::endl;
        std::cout << "  Vertex Index 1: " << m_faceVerticies[i].vertexIndex << ", Texture Index: " << m_faceVerticies[i].textureIndex
                  << ", Normal Index: " << m_faceVerticies[i].normalIndex << std::endl;
        std::cout << "  Vertex Index 2: " << m_faceVerticies[i + 1].vertexIndex << ", Texture Index: " << m_faceVerticies[i + 1].textureIndex
                  << ", Normal Index: " << m_faceVerticies[i + 1].normalIndex << std::endl;
        std::cout << "  Vertex Index 3: " << m_faceVerticies[i + 2].vertexIndex << ", Texture Index: " << m_faceVerticies[i + 2].textureIndex
                  << ", Normal Index: " << m_faceVerticies[i + 2].normalIndex << std::endl;
    }
}

void s21::Loader::Clear() {
    m_vertices.clear();
    m_verticies.clear();
    m_normals.clear();
    m_texCoords.clear();
    m_faceVerticies.clear();
}

void s21::Loader::RoundNormal(Vector3& normal) {
    if(std::abs(normal.x) <= 1e-5) normal.x = 0.0;
    if(std::abs(normal.y) <= 1e-5) normal.y = 0.0;
    if(std::abs(normal.z) <= 1e-5) normal.z = 0.0;
}
