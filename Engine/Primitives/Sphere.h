#pragma once
#include "Primitive.h"

class Sphere : public Primitive {
public:
    Sphere() {
        constexpr int segments = 20;
        constexpr int rings = 5;
        constexpr float PI = 3.14159265358979323846f;

        // Original sphere data: position + color (6 floats per vertex).
        std::vector<float> originalVertex;
        std::vector<unsigned int> originalIndex;

        // Top pole.
        originalVertex.insert(originalVertex.end(), {
            0.0f, 1.0f, 0.0f,
            1.0f, 0.0f, 0.0f
            });

        // Generate the latitude rings.
        // Each ring has one extra vertex to close the UV seam.
        for (int r = 0; r < rings; ++r) {
            float latitude =
                PI * static_cast<float>(r + 1) / (rings + 1);

            float y = std::cos(latitude);
            float radius = std::sin(latitude);

            for (int s = 0; s <= segments; ++s) {
                float longitude =
                    2.0f * PI * static_cast<float>(s) / segments;

                float x = radius * std::sin(longitude);
                float z = radius * std::cos(longitude);

                originalVertex.insert(originalVertex.end(), {
                    x, y, z,
                    1.0f, 1.0f, 1.0f
                    });
            }
        }

        // Bottom pole.
        const unsigned int bottomIndex =
            static_cast<unsigned int>(originalVertex.size() / 6);

        originalVertex.insert(originalVertex.end(), {
            0.0f, -1.0f, 0.0f,
            1.0f, 1.0f, 1.0f
            });

        // Generate the top cap triangles.
        for (int s = 0; s < segments; ++s) {
            unsigned int current =
                1 + s;
            unsigned int next =
                current + 1;

            originalIndex.push_back(0);
            originalIndex.push_back(next);
            originalIndex.push_back(current);
        }

        // Generate triangles between adjacent latitude rings.
        for (int r = 0; r < rings - 1; ++r) {
            unsigned int upperRing =
                1 + r * (segments + 1);
            unsigned int lowerRing =
                upperRing + segments + 1;

            for (int s = 0; s < segments; ++s) {
                unsigned int upperCurrent = upperRing + s;
                unsigned int upperNext = upperCurrent + 1;
                unsigned int lowerCurrent = lowerRing + s;
                unsigned int lowerNext = lowerCurrent + 1;

                originalIndex.insert(originalIndex.end(), {
                    upperCurrent, upperNext, lowerCurrent,
                    upperNext, lowerNext, lowerCurrent
                    });
            }
        }

        // Generate the bottom cap triangles.
        unsigned int lastRing =
            1 + (rings - 1) * (segments + 1);

        for (int s = 0; s < segments; ++s) {
            originalIndex.push_back(bottomIndex);
            originalIndex.push_back(lastRing + s);
            originalIndex.push_back(lastRing + s + 1);
        }

        // Rebuild the mesh with independent vertices for every triangle.
        // Final layout: position (3) + color (3) + UV (2).
        vertex.clear();
        index.clear();

        for (size_t i = 0; i + 2 < originalIndex.size(); i += 3) {
            for (size_t corner = 0; corner < 3; ++corner) {
                unsigned int sourceIndex = originalIndex[i + corner];
                size_t offset =
                    static_cast<size_t>(sourceIndex) * 6;

                // Copy position and color.
                for (size_t k = 0; k < 6; ++k) {
                    vertex.push_back(originalVertex[offset + k]);
                }

                // Give each triangle its own UV triangle.
                if (corner == 0) {
                    vertex.push_back(0.5f);
                    vertex.push_back(0.0f);
                }
                else if (corner == 1) {
                    vertex.push_back(0.0f);
                    vertex.push_back(1.0f);
                }
                else {
                    vertex.push_back(1.0f);
                    vertex.push_back(1.0f);
                }

                // Every vertex is unique, so indices are sequential.
                index.push_back(
                    static_cast<unsigned int>(index.size())
                );
            }
        }

        num_index = static_cast<int>(index.size());
        texturePath = "Data/PrimitiveTextures/checkerboard.dds";
    }
};