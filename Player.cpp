#include "Player.h"

void Player::Initialize(const std::vector<Model*>& models)
{
	models_ = models;
	worldTransform_.Initialize();
	worldTransform_Body_.Initialize();
	worldTransform_Head_.Initialize();
	worldTransform_Larm_.Initialize();
	worldTransform_Rarm_.Initialize();

	worldTransform_.translation_.y = 2.0f;
	worldTransform_Body_.translation_.y = 2.0f;
	worldTransform_Head_.parent_ = &worldTransform_Body_;
	worldTransform_Head_.translation_.y = 4.4f;
	worldTransform_Larm_.parent_ = &worldTransform_Body_;
	worldTransform_Larm_.translation_ = Vector3(-2.2f, 5.4f, 0.0f);
	worldTransform_Rarm_.parent_ = &worldTransform_Body_;
	worldTransform_Rarm_.translation_ = Vector3(2.2f, 5.4f, 0.0f);

}

void Player::Update()
{


	worldTransform_.UpdateMatrix();
	worldTransform_Body_.UpdateMatrix();
	worldTransform_Head_.UpdateMatrix();
	worldTransform_Larm_.UpdateMatrix();
	worldTransform_Rarm_.UpdateMatrix();
}

void Player::Draw(ViewProjection& viewProjection)
{
	models_[0]->Draw(worldTransform_Body_, viewProjection);
	models_[1]->Draw(worldTransform_Head_, viewProjection);
	models_[2]->Draw(worldTransform_Larm_, viewProjection);
	models_[3]->Draw(worldTransform_Rarm_, viewProjection);

}
