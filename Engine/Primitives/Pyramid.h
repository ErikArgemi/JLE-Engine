#pragma once
#include "Primitive.h"

class Pyramid : public Primitive {
	Pyramid() {
		vertex = {
			//  Position                        Color
			0.0f, 1.0f, 0.0f,               1.0f, 0.0f, 0.0f,
			-2.0f, -1.0f, 0.0f,				0.0f, 1.0f, 0.0f,
			0.0f, -1.0f, 2.0f,				0.0f, 0.0f, 1.0f,
			2.0f, -1.0f, 0.0f,				1.0f, 0.0f, 0.0f,
			0.0f, -1.0f, -2.0f,				0.0f, 1.0f, 1.0f
		};
		num_index = 18;
		index = {
			//top faces
			0,1,2,
			0,2,3,
			0,3,4,
			0,4,1,
			//base
			1,2,3,
			1,3,4
		};
		texturePath = "Data/PrimitiveTextures/checkerboard.dds";
	}
};