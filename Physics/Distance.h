#pragma once

#include <cassert>

#include "MathFunction.h"

class Shape;

struct DistanceProxy {
	DistanceProxy() : m_vertices(nullptr), m_count(0), m_radius(0.0f){}

	void Set(const Shape* shape, uint32_t index);

	void Set(const Vector2* vertices, uint32_t count, float radius);

	uint32_t GetSupport(const Vector2& d)const;

	const Vector2& GetSupportVertex(const Vector2& d)const;

	uint32_t GetVertexCount()const;

	const Vector2& GetVertex(uint32_t index) const;

	Vector2 m_buffer[2];
	const Vector2* m_vertices;
	uint32_t m_count;
	float m_radius;
};

struct SimplexCache {
	float metric;
	uint16_t count;
	uint8_t indexA[3];
	uint8_t indexB[3];
};

struct DistanceInput {
	DistanceProxy proxyA;
	DistanceProxy proxyB;
	myTransform transformA;
	myTransform transformB;
	bool useRadii;
};

struct DistanceOutput {
	Vector2 pointA;
	Vector2 pointB;
	float distance;
	uint32_t iterations;
};

void Distance(DistanceOutput* output, SimplexCache* cache, const DistanceInput* input);

struct ShapeCastInput {
	DistanceProxy proxyA;
	DistanceProxy proxyB;
	myTransform transformA;
	myTransform transformB;
	Vector2 translationB;
};

struct ShapeCastOutput {
	Vector2 point;
	Vector2 normal;
	float lambda;
	uint32_t iterations;
};

bool ShapeCast(ShapeCastOutput* output, const ShapeCastInput* input);

inline uint32_t DistanceProxy::GetVertexCount() const {
	return m_count;
}

inline const Vector2& DistanceProxy::GetVertex(uint32_t index)const {
	assert(0 <= index && index < m_count);
	return m_vertices[index];
}

inline uint32_t DistanceProxy::GetSupport(const Vector2& v) const {
	uint32_t bestIndex = 0;
	float bestValue = Dot(m_vertices[0], v);
	for (uint32_t i = 1; i < m_count; ++i) {
		float value = Dot(m_vertices[i], v);
		if (value > bestValue) {
			bestIndex = i;
			bestValue = value;
		}
	}

	return bestIndex;
}

inline const Vector2& DistanceProxy::GetSupportVertex(const Vector2& v) const {
	uint32_t bestIndex = 0;
	float bestValue = Dot(m_vertices[0], v);
	for (uint32_t i = 1; i < m_count; ++i) {
		float value = Dot(m_vertices[i], v);
		if (value > bestValue) {
			bestIndex = i;
			bestValue = value;
		}
	}

	return m_vertices[bestIndex];
}