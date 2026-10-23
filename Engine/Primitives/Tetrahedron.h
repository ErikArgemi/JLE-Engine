#pragma once

#include "Primitive.h"

class Tetrahedron : public Primitive {
public:
    Tetrahedron() {
        vertex = {
            0.0f, 0.5f, 0.0f,       1.0f, 1.0f, 1.0f,
            -0.5f, -0.5f, 0.866f,   1.0f, 0.0f, 0.0f,
            1.0f, -0.5f, 0.0f,      0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, -0.866f,  0.0f, 0.0f, 1.0f
        };

        num_index = 12;

        index = {
            0, 1, 2,
            0, 2, 3,
            0, 3, 1,
            1, 2, 3
        };

        // Save the original vertex and index data.
        const auto originalVertex = vertex;
        const auto originalIndex = index;

        // Rebuild the buffers with independent vertices and UVs.
        vertex.clear();
        index.clear();

        for (size_t i = 0; i < originalIndex.size(); i += 3) {
            for (size_t j = 0; j < 3; ++j) {
                unsigned int vertexIndex = originalIndex[i + j];
                size_t offset = static_cast<size_t>(vertexIndex) * 6;

                // Copy position and color (6 floats).
                for (size_t k = 0; k < 6; ++k) {
                    vertex.push_back(originalVertex[offset + k]);
                }

                // UV coordinates: map the complete texture to this face.
                if (j == 0) {
                    vertex.push_back(0.5f);
                    vertex.push_back(0.0f);
                }
                else if (j == 1) {
                    vertex.push_back(0.0f);
                    vertex.push_back(1.0f);
                }
                else {
                    vertex.push_back(1.0f);
                    vertex.push_back(1.0f);
                }

                // Sequential index for each duplicated vertex.
                index.push_back(static_cast<unsigned int>(index.size()));
            }
        }

        num_index = static_cast<int>(index.size());

        texturePath = "Data/PrimitiveTextures/checkerboard.dds";
    }
};