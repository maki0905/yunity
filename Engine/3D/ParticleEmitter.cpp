#include "ParticleEmitter.h"

#include "ImGuiManager.h"

void ParticleEmitter::Initialize(Transform transform, float frequency, uint32_t count)
{
	worldTransform_.Initialize();
	worldTransform_.scale_ = transform.scale;
	worldTransform_.rotation_ = transform.rotate;
	worldTransform_.translation_ = transform.translate;

	frequency_ = frequency;
	frequencyTime_ = 0.0f;
	count_ = count;

	primitiveDrawer_.reset(PrimitiveDrawer::Create(PrimitiveDrawer::Type::kBox));
	primitiveDrawer_->SetCamera(camera_);
}

bool ParticleEmitter::Update()
{
	ImGui::Begin("Emitter");
	ImGui::SliderFloat3("translation", &worldTransform_.translation_.x, -10.0f, 10.0f);
	ImGui::End();

	worldTransform_.UpdateMatrix(RotationType::Euler);

	frequencyTime_ += 1.0f / 60.0f; // 時間を進める
	if (frequency_ <= frequencyTime_) { // 頻度より大きいなら発生
		frequencyTime_ -= frequency_; // 余計に過ぎた時間も加味して頻度計算する
		return true;
	}
	return false;
}

void ParticleEmitter::Draw()
{

	primitiveDrawer_->Draw(worldTransform_);

}
