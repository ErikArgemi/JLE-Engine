#pragma once
#include <stdio.h>
#include <unordered_map>

#include "Primitive.h"


class PrimitiveManager {
public:
	PrimitiveManager() {}
	~PrimitiveManager() {}
	
	std::unordered_map<int, Primitive> GetPrimitives() {
		return primitives;
	}

	Primitive& CreatePrimitiveEntity(PrimitiveType type, glm::vec3 Pos = glm::vec3(0.0f));

	void CleanUp() {
		primitives.clear();
	}

private:
	std::unordered_map<int, Primitive> primitives;
	int primitivesSize = 0;
};