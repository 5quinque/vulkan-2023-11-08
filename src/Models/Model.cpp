#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "Model.hpp"

bool Model::loadedModel = false;

Model::Model(int modelId, glm::vec3 scale, bool createRigidBody,
             bool matrixOffset, glm::vec3 position)
    : modelId(modelId), scale(scale) {
    if (matrixOffset) {
        // If we use bit packing, the following explanation and function
        // can be used to pack the coordinates into a single int
        //
        // Starting with 6 bits of information, we can store 2^6 = 64
        // This isn't very large when each axis only gets 4 positive values,
        // but it makes it easier to understand and scaling up follows the
        // same logic
        // x coord. 48 dec - 110000 bin : bitmask 2 bits (2^4 * 3)
        // y coord. 12 dec - 001100 bin : bitmask 2 bits (2^2 * 3)
        // z coord. 3  dec - 000011 bin : bitmask 2 bits (2^0 * 3)

        // a standard int has a minimum of 16 bits, so we can store 3
        // coordinates in a single int using 5 bits for each
        // float x = static_cast<float>((modelId >> 10) & 31);
        // float y = static_cast<float>((modelId >> 5) & 31);
        // float z = static_cast<float>(modelId & 31);
        //
        //
        // int packCoordinates(int x, int y, int z) {
        //     // Ensure x, y, z are within the range 0-31
        //     assert(x >= 0 && x < 32);
        //     assert(y >= 0 && y < 32);
        //     assert(z >= 0 && z < 32);

        //     // Shift x by 10 bits to the left, y by 5 bits and leave z as is
        //     // Then combine them using bitwise OR
        //     return (x << 10) | (y << 5) | z;
        // }

        // set position
        setModelMatrix(glm::translate(model, position));
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
