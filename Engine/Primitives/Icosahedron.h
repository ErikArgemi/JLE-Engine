#pragma once

#include "Primitive.h"

class Icosahedron : public Primitive {
public:
	Icosahedron() {
		vertex = {
			//  Position                        Color
			0.0f, 1.0f, 0.0f,               1.0f, 0.0f, 0.0f,
			-0.276f, 0.447f, 0.851f,        0.0f, 1.0f, 0.0f,
			0.724f, 0.447f, 0.526f,         0.0f, 0.0f, 1.0f,
			0.724f, 0.447f, -0.526f,       1.0f, 0.0f, 0.0f,
			-0.276f, 0.447f, -0.851f,       0.0f, 1.0f, 1.0f,
			-0.894f, 0.447f, 0.0f,          1.0f, 1.0f, 0.0f,
			0.276f, -0.447f, 0.851f,        1.0f, 0.0f, 1.0f,
			0.894f, -0.447f, 0.0f,          1.0f, 1.0f, 1.0f,
			0.276f, -0.447f, -0.851f,        1.0f, 0.0f, 0.0f,
			-0.724f, -0.447f, -0.526f,       0.0f, 1.0f, 0.0f,
			-0.724f, -0.447f, 0.526f,        0.0f, 0.0f, 1.0f,
			0.0f, -1.0f, 0.0f,              1.0f, 0.0f, 0.0f
		};
		num_index = 60;
		index = {
			//top 5 faces
			0,1,2,
			0,2,3,
			0,3,4,
			0,4,5,
			0,5,1,
			//middle 10 faces
			1,6,2,
			2,6,7,
			3,2,7,
			3,7,8,
			4,3,8,
			4,8,9,
			5,4,9,
			5,9,10,
			1,5,10,
			1,10,6,
			//bottom 5 faces
			11,6,7,
			11,7,8,
			11,8,9,
			11,9,10,
			11,10,6
		};
		texturePath = "Data/PrimitiveTextures/checkerboard.dds";
	}
};