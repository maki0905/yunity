#pragma once

#include "Object3D.h"

class World;

namespace yunity{

	struct Point {
		Vector3 point;
		Vector3 supportA;
		Vector3 supportB;
	};

	void GJKCalculation(Object3D* colliderA, Object3D* colliderB, World* world);
	void UpdateSimplex3(Point& a, Point& b, Point& c, Point& d, int& simp_dim, Vector3& search_dir);
	bool UpdateSimplex4(Point& a, Point& b, Point& c, Point& d, int& simp_dim, Vector3& search_dir);
	void EPA(Point& a, Point& b, Point& c, Point& d, Object3D* colliderA, Object3D* colliderB, World* world);
	void CalculateSearchPoint(Point& point, Vector3& search_dir, Object3D* colliderA, Object3D* colldierB);
}