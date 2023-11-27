﻿#include "Skydome.h"
#include <cassert>

void Skydome::Initialize(Model* model)
{
	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.scale_ = { 400.0f, 400.0f, 400.0f };
	worldTransform_.UpdateMatrix(RotationType::Euler);
}

void Skydome::Update()
{
}

void Skydome::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}
