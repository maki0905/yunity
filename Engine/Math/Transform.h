#pragma once
#include "Vector3.h"
#include "Quaternion.h"

struct EulerTransform {
	Vector3 scale = {1.0f, 1.0f, 1.0f};
	Vector3 rotate = {0.0f, 0.0f, 0.0f};
	Vector3 translate = { 0.0f, 0.0f, 0.0f };
};

struct QuaternionTransform {
	Vector3 scale = { 1.0f, 1.0f, 1.0f };
	Quaternion rotate;
	Vector3 translate = { 0.0f, 0.0f, 0.0f };
};