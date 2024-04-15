#pragma once

#include <memory>

#include "Input.h"
#include "Collider.h"
#include "Model.h"
#include "WorldTransform.h"
#include "Camera.h"
#include "Sprite.h"

#include "PlayerBullet.h"

class Enemy;

class Player : public Collider
{
public:
	void Initialize(Camera* camera);

	void Update();

	void Draw();

	void DrawBullet();

	Vector3 GetTranslation() { return worldTransfrom_.translation_; }

	void SetTranslation(const Vector3 translation) { worldTransfrom_.translation_ = translation; }

	/// <summary>
	/// 旋回
	/// </summary>
	void Rotate();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	void Reticle(const Matrix4x4& matViewport);

	void DrawUI();

	/// <summary>
	/// 親となるワールドトランスフォームをセット
	/// </summary>
	/// <param name="parent">親となるワールドトランスフォーム</param>
	void SetParent(WorldTransform* parent);

	// 弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() { return bullets_; }

	Vector2 GetReticle() { return sprite2DReticle_->GetPosition(); }

	void SetEnemy(Enemy* enemy) { enemys_.push_back(enemy); }

private:
	XINPUT_STATE pad_;
	XINPUT_STATE prePad_;

	std::unique_ptr<Model> model_;

	WorldTransform worldTransfrom_;

	Camera* camera_;
	
	uint32_t attackTime_ = 0;

	WorldTransform worldTransform3DReticle_;

	std::unique_ptr<Sprite> sprite2DReticle_;

	// 弾
	std::list<PlayerBullet*> bullets_;

	std::vector<Enemy*> enemys_;


};

