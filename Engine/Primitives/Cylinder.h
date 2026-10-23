#pragma once

#include "Primitive.h"

class Cylinder : public Primitive {
public:
	Cylinder()
	{
		// Keep your original vertex data here.
		// Each original vertex contains 6 floats:
		// position (x, y, z) + color (r, g, b).

		vertex = {
			// Position				Color
			0.0f, 1.0f, 0.0f,       1.0f, 0.0f, 0.0f,
			0.000f, 1.0f, 1.000f,   0.0f, 1.0f, 0.0f,
			0.309f, 1.0f, 0.951f,   0.0f, 0.0f, 1.0f,
			0.588f, 1.0f, 0.809f,   1.0f, 0.0f, 0.0f,
			0.809f, 1.0f, 0.588f,   0.0f, 1.0f, 0.0f,
			0.951f, 1.0f, 0.309f,   0.0f, 0.0f, 1.0f,
			1.000f, 1.0f, 0.000f,   1.0f, 0.0f, 0.0f,
			0.951f, 1.0f, -0.309f,  0.0f, 1.0f, 0.0f,
			0.809f, 1.0f, -0.588f,  0.0f, 0.0f, 1.0f,
			0.588f, 1.0f, -0.809f,  1.0f, 0.0f, 0.0f,
			0.309f, 1.0f, -0.951f,  0.0f, 1.0f, 0.0f,
			0.000f, 1.0f, -1.000f,  0.0f, 0.0f, 1.0f,
			-0.309f, 1.0f, -0.951f, 1.0f, 0.0f, 0.0f,
			-0.588f, 1.0f, -0.809f, 0.0f, 1.0f, 0.0f,
			-0.809f, 1.0f, -0.588f, 0.0f, 0.0f, 1.0f,
			-0.951f, 1.0f, -0.309f, 1.0f, 0.0f, 0.0f,
			-1.000f, 1.0f, 0.000f,  0.0f, 1.0f, 0.0f,
			-0.951f, 1.0f, 0.309f,  0.0f, 0.0f, 1.0f,
			-0.809f, 1.0f, 0.588f,  1.0f, 0.0f, 0.0f,
			-0.588f, 1.0f, 0.809f,  0.0f, 1.0f, 0.0f,
			-0.309f, 1.0f, 0.951f,  0.0f, 0.0f, 1.0f,
			//Bottom half//////////
			0.000f, -1.0f, 1.000f,  1.0f, 0.0f, 0.0f,
			0.309f, -1.0f, 0.951f,  0.0f, 1.0f, 0.0f,
			0.588f, -1.0f, 0.809f,  0.0f, 0.0f, 1.0f,
			0.809f, -1.0f, 0.588f,  1.0f, 0.0f, 0.0f,
			0.951f, -1.0f, 0.309f,  0.0f, 1.0f, 0.0f,
			1.000f, -1.0f, 0.000f,  0.0f, 0.0f, 1.0f,
			0.951f, -1.0f, -0.309f, 1.0f, 0.0f, 0.0f,
			0.809f, -1.0f, -0.588f, 0.0f, 1.0f, 0.0f,
			0.588f, -1.0f, -0.809f, 0.0f, 0.0f, 1.0f,
			0.309f, -1.0f, -0.951f, 1.0f, 0.0f, 0.0f,
			0.000f, -1.0f, -1.000f, 0.0f, 1.0f, 0.0f,
			-0.309f, -1.0f, -0.951f,        0.0f, 0.0f, 1.0f,
			-0.588f, -1.0f, -0.809f,        1.0f, 0.0f, 0.0f,
			-0.809f, -1.0f, -0.588f,        0.0f, 1.0f, 0.0f,
			-0.951f, -1.0f, -0.309f,        0.0f, 0.0f, 1.0f,
			-1.000f, -1.0f, 0.000f, 1.0f, 0.0f, 0.0f,
			-0.951f, -1.0f, 0.309f, 0.0f, 1.0f, 0.0f,
			-0.809f, -1.0f, 0.588f, 0.0f, 0.0f, 1.0f,
			-0.588f, -1.0f, 0.809f, 1.0f, 0.0f, 0.0f,
			-0.309f, -1.0f, 0.951f, 0.0f, 1.0f, 0.0f,
			0.0f, -1.0f, 0.0f,      0.0f, 0.0f, 1.0f
		};

		// Keep your original index data here.
		index = {
			//Top Face
			0, 1, 2,
			0, 2, 3,
			0, 3, 4,
			0, 4, 5,
			0, 5, 6,
			0, 6, 7,
			0, 7, 8,
			0, 8, 9,
			0, 9, 10,
			0, 10, 11,
			0, 11, 12,
			0, 12, 13,
			0, 13, 14,
			0, 14, 15,
			0, 15, 16,
			0, 16, 17,
			0, 17, 18,
			0, 18, 19,
			0, 19, 20,
			0, 20, 1,
			//Middle Faces
			1, 21, 2,       2, 21, 22,
			2, 22, 3,       3, 22, 23,
			3, 23, 4,       4, 23, 24,
			4, 24, 5,       5, 24, 25,
			5, 25, 6,       6, 25, 26,
			6, 26, 7,       7, 26, 27,
			7, 27, 8,       8, 27, 28,
			8, 28, 9,       9, 28, 29,
			9, 29, 10,      10, 29, 30,
			10, 30, 11,     11, 30, 31,
			11, 31, 12,     12, 31, 32,
			12, 32, 13,     13, 32, 33,
			13, 33, 14,     14, 33, 34,
			14, 34, 15,     15, 34, 35,
			15, 35, 16,     16, 35, 36,
			16, 36, 17,     17, 36, 37,
			17, 37, 18,     18, 37, 38,
			18, 38, 19,     19, 38, 39,
			19, 39, 20,     20, 39, 40,
			20, 40, 1,      1, 40, 21,
			//Bottom Faces
			41, 21, 22,
			41, 22, 23,
			41, 23, 24,
			41, 24, 25,
			41, 25, 26,
			41, 26, 27,
			41, 27, 28,
			41, 28, 29,
			41, 29, 30,
			41, 30, 31,
			41, 31, 32,
			41, 32, 33,
			41, 33, 34,
			41, 34, 35,
			41, 35, 36,
			41, 36, 37,
			41, 37, 38,
			41, 38, 39,
			41, 39, 40,
			41, 40, 21
		};

		// Save the original data before rebuilding the buffers.
		const auto originalVertex = vertex;
		const auto originalIndex = index;

		// Clear the buffers so we can create independent vertices
		// for every triangle, including UV coordinates.
		vertex.clear();
		index.clear();

		// Each triangle gets the complete checkerboard texture.
		for (size_t i = 0; i + 2 < originalIndex.size(); i += 3)
		{
			for (size_t j = 0; j < 3; ++j)
			{
				const unsigned int originalVertexIndex =
					originalIndex[i + j];

				// Original vertices have 6 floats each:
				// position (3) + color (3).
				const size_t offset =
					static_cast<size_t>(originalVertexIndex) * 6;

				// Copy position and color.
				for (size_t k = 0; k < 6; ++k)
				{
					vertex.push_back(originalVertex[offset + k]);
				}

				// Add UV coordinates for this triangle corner.
				if (j == 0)
				{
					vertex.push_back(0.5f);
					vertex.push_back(0.0f);
				}
				else if (j == 1)
				{
					vertex.push_back(0.0f);
					vertex.push_back(1.0f);
				}
				else
				{
					vertex.push_back(1.0f);
					vertex.push_back(1.0f);
				}

				// Every duplicated vertex gets a new sequential index.
				index.push_back(
					static_cast<unsigned int>(index.size())
				);
			}
		}

		num_index = static_cast<int>(index.size());

		texturePath = "Data/PrimitiveTextures/checkerboard.dds";
	}
};