#pragma once

#include "Triangle.h"
#include "WorldTransform.h"
class T
{
public:
	// ������
	void Initialize();

	// �X�V
	void Update();

	// �`��
	void Draw();

private:
	WorldTransform worldTransform_;
	Triangle* triangle_;

};

