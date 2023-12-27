#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "Model.hpp"

bool Model::loadedModel = false;

Model::Model(int modelId, glm::vec3 scale, bool createRigidBody,
             bool matrixOffset)
    : modelId(modelId), scale(scale) {
    // std::cout << "Model constructor" << std::endl;

    if (matrixOffset) {
        // x bitmask 48 (2^4 * 3)
        // y bitmask 12 (2^2 * 3)
        // z bitmask 3  (2^0 * 3)
        float x = static_cast<float>((modelId >> 4) & 3);
        float y = static_cast<float>((modelId >> 2) & 3);
        float z = static_cast<float>(modelId & 3);

        // std::cout << "Creating " << modelId << " at x: " << x << ", y: " << y
        //           << ", z: " << z << std::endl;

        // set position
        setModelMatrix(glm::translate(model, glm::vec3(x, y, z)));
    }

    setModelMatrix(glm::scale(model, scale));
}

void Model::loadModel() {
    if (loadedModel) {
        return;
    }
    loadedModel = true;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    std::cout << "Model::loadModel(), MODEL_PATH: " << this->MODEL_PATH
              << std::endl;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
                          MODEL_PATH.c_str())) {
        throw std::runtime_error(warn + err);
    }

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            Vertex vertex{};

            vertex.pos = {attrib.vertices[3 * index.vertex_index + 0],
                          attrib.vertices[3 * index.vertex_index + 1],
                          attrib.vertices[3 * index.vertex_index + 2]};

            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1]};

            vertex.color = {1.0f, 1.0f, 1.0f};

            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }

            indices.push_back(uniqueVertices[vertex]);
        }
    }
}
