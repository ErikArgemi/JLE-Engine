#pragma once
#include "Primitive.h"

class Pyramid : public Primitive {
public:
    Pyramid() {
        vertex = {
            // Position                 Color                 UV
            // Front face
             0.0f,  1.0f,  0.0f,       1.0f, 0.0f, 0.0f,    0.5f, 0.0f,
            -2.0f, -1.0f,  0.0f,       0.0f, 1.0f, 0.0f,    0.0f, 1.0f,
             0.0f, -1.0f,  2.0f,       0.0f, 0.0f, 1.0f,    1.0f, 1.0f,

             // Right face
              0.0f,  1.0f,  0.0f,       1.0f, 0.0f, 0.0f,    0.5f, 0.0f,
              0.0f, -1.0f,  2.0f,       0.0f, 0.0f, 1.0f,    0.0f, 1.0f,
              2.0f, -1.0f,  0.0f,       1.0f, 0.0f, 0.0f,    1.0f, 1.0f,

              // Back face
               0.0f,  1.0f,  0.0f,       1.0f, 0.0f, 0.0f,    0.5f, 0.0f,
               2.0f, -1.0f,  0.0f,       1.0f, 0.0f, 0.0f,    0.0f, 1.0f,
               0.0f, -1.0f, -2.0f,       0.0f, 1.0f, 1.0f,    1.0f, 1.0f,

               // Left face
                0.0f,  1.0f,  0.0f,       1.0f, 0.0f, 0.0f,    0.5f, 0.0f,
                0.0f, -1.0f, -2.0f,       0.0f, 1.0f, 1.0f,    0.0f, 1.0f,
               -2.0f, -1.0f,  0.0f,       0.0f, 1.0f, 0.0f,    1.0f, 1.0f,

               // Base triangle 1
               -2.0f, -1.0f,  0.0f,       0.0f, 1.0f, 0.0f,    0.0f, 0.0f,
                0.0f, -1.0f,  2.0f,       0.0f, 0.0f, 1.0f,    0.0f, 1.0f,
                2.0f, -1.0f,  0.0f,       1.0f, 0.0f, 0.0f,    1.0f, 1.0f,

                // Base triangle 2
                -2.0f, -1.0f,  0.0f,       0.0f, 1.0f, 0.0f,    0.0f, 0.0f,
                 2.0f, -1.0f,  0.0f,       1.0f, 0.0f, 0.0f,    1.0f, 1.0f,
                 0.0f, -1.0f, -2.0f,       0.0f, 1.0f, 1.0f,    1.0f, 0.0f
        };

        num_index = 18;
        index = {
             0,  1,  2,
             3,  4,  5,
             6,  7,  8,
             9, 10, 11,
            12, 13, 14,
            15, 16, 17
        };

        texturePath = "Data/PrimitiveTextures/checkerboard.dds";
    }
};