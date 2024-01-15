#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
// Optional. define TINYOBJLOADER_USE_MAPBOX_EARCUT gives robust trinagulation. Requires C++11
#define TINYOBJLOADER_USE_MAPBOX_EARCUT
#include "tiny_obj_loader.h"

#include <iostream>

#include "mesh.h"

Mesh meshCreate(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices)
{
    GLuint vao = 0, vbo = 0, ebo = 0;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
        glCheckError();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
        glCheckError();

        glEnableVertexAttribArray(eDataIdx::Position);
        glEnableVertexAttribArray(eDataIdx::Color);
        glVertexAttribPointer(eDataIdx::Position,   3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, pos));
        glVertexAttribPointer(eDataIdx::Color,      4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, color));
        glCheckError();
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return Mesh{vao, vbo, ebo, (unsigned int) vertices.size(), (unsigned int) indices.size()};
}

void meshDelete(const Mesh &mesh)
{
    glDeleteBuffers(1, &mesh.vbo);
    glDeleteBuffers(1, &mesh.ebo);
    glDeleteVertexArrays(1, &mesh.vao);
}

std::vector<Mesh> meshLoadFromObj(const std::string &filepath) {
    std::vector<Mesh> meshes;

    tinyobj::ObjReader reader;

    tinyobj::ObjReaderConfig readerConfig;
    readerConfig.triangulate = true;

    if (!reader.ParseFromFile(filepath, readerConfig)) {
        throw std::runtime_error("[Mesh] ERROR loading model with TinyObjReader:\n" + reader.Error());
    }

    if (!reader.Warning().empty()) {
        std::cout << "TinyObjReader: "<< reader.Warning() << std::endl;
    }

    const tinyobj::attrib_t& attrib = reader.GetAttrib();
    const std::vector<tinyobj::shape_t>& shapes = reader.GetShapes();
    const std::vector<tinyobj::material_t>& materials = reader.GetMaterials();

    // Loop over shapes (different meshes in obj files)
    for (unsigned int s=0; s < shapes.size(); s++) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        
        // Loop over all faces of current shape
        size_t indexOffset = 0; 
        for (unsigned int f=0; f<shapes[s].mesh.num_face_vertices.size(); f++) {
            unsigned int materialId = shapes[s].mesh.material_ids[f];
            glm::vec4 faceColor = { materials[materialId].diffuse[0], materials[materialId].diffuse[1], materials[materialId].diffuse[2], 1.0 };

            // Loop over vertices in the face, create vertex data as used in our mesh and add indices to vertices.
            for (size_t v = 0; v < 3; v++) {
                unsigned int idx = shapes[s].mesh.indices[indexOffset + v].vertex_index;

                vertices.push_back({
                        {
                            attrib.vertices[3 * idx],
                            attrib.vertices[3 * idx + 1],
                            attrib.vertices[3 * idx + 2]
                        }, 
                        faceColor
                });

                indices.push_back(indexOffset + v);
            }
            indexOffset += 3;
        }
        meshes.push_back(meshCreate(vertices, indices));
    }

    return meshes;
}
