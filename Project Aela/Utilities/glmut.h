/*
* Name: GLM Utilities
* Author: Robert Ciborowski
* Date: 16/12/2017
* Description: A file used for utilities regarding the glm library.
*/

#pragma once
#include <glm/glm.hpp>
#include <unordered_map>

// The following structs are for when you want to create a HashMap with glm::vecs as the keys.
struct Vec2HashMapFunctions {
	size_t operator()(const glm::vec2& k) const {
		return std::hash<float>()(k.x) ^ std::hash<float>()(k.y);
	}

	bool operator()(const glm::vec2& a, const glm::vec2& b) const {
		return a.x == b.x && a.y == b.y;
	}
};

struct IVec2HashMapFunctions {
	size_t operator()(const glm::ivec2& k) const {
		return std::hash<int>()(k.x) ^ std::hash<int>()(k.y);
	}

	bool operator()(const glm::ivec2& a, const glm::ivec2& b) const {
		return a.x == b.x && a.y == b.y;
	}
};

struct Vec3HashMapFunctions {
	size_t operator()(const glm::vec3& k) const {
		return std::hash<float>()(k.x) ^ std::hash<float>()(k.y) ^ std::hash<float>()(k.z);
	}

	bool operator()(const glm::vec3& a, const glm::vec3& b) const {
		return a.x == b.x && a.y == b.y && a.z == b.z;
	}
};

struct IVec3HashMapFunctions {
	size_t operator()(const glm::ivec3& k) const {
		return std::hash<int>()(k.x) ^ std::hash<int>()(k.y) ^ std::hash<int>()(k.z);
	}

	bool operator()(const glm::ivec3& a, const glm::ivec3& b) const {
		return a.x == b.x && a.y == b.y && a.z == b.z;
	}
};

struct Vec4HashMapFunctions {
	size_t operator()(const glm::vec4& k) const {
		return std::hash<float>()(k.x) ^ std::hash<float>()(k.y) ^ std::hash<float>()(k.z) ^ std::hash<float>()(k.a);
	}

	bool operator()(const glm::vec4& a, const glm::vec4& b) const {
		return a.x == b.x && a.y == b.y && a.z == b.z && a.a == b.a;
	}
};