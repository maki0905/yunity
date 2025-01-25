#include "FloatingParticle.h"

#include "EngineTimeStep.h"

#include "GlobalVariables.h"
void FloatingParticle::Initialize(yunity::Camera* camera)
{
	isActive_ = true;
	camera_ = camera;
	centerPosition_ = { 0.0f, 0.0f, 0.0f };
	targetPosition_ = { 0.0f, 0.0f, 0.0f };
	particleDrawer_ = std::make_unique<yunity::ParticleDrawer>();
	particleDrawer_->Initialize("circle.png");
	particleDrawer_->SetBlendModeType(yunity::BlendModeType::kAdd);
	particleDrawer_->SetCamera(camera);
	particles_.clear();
	angle_ = 0.0f;
	isLerp_ = false;
	yunity::GlobalVariables* globalVariables = yunity::GlobalVariables::GetInstance();
	const char* groupName = "FloatingParticle";
	lifeTime_ = globalVariables->GetFloatValue(groupName, "LifeTime");
	power_ = globalVariables->GetFloatValue(groupName, "Power");
	speedDegree_ = globalVariables->GetFloatValue(groupName, "SpeedDegree");
	minPosition_ = globalVariables->GetVector3Value(groupName, "MinPosition");
	maxPosition_ = globalVariables->GetVector3Value(groupName, "MaxPosition");
}

void FloatingParticle::Spawn(const Vector3& position)
{
	particles_.clear();
	centerPosition_ = position;
	for (uint32_t i = 0; i < 5; ++i) {
		Particle particle;
		particle.transform.translate = position;
		particle.transform.translate = Add(position, { rng.NextFloatRange(minPosition_.x, maxPosition_.x), rng.NextFloatRange(minPosition_.y, maxPosition_.y), rng.NextFloatRange(minPosition_.z, maxPosition_.z) });
		particle.velocity = { rng.NextFloatRange(0.0f, 1.0f), rng.NextFloatRange(0.0f, 1.0f), rng.NextFloatRange(0.0f, 1.0f) };

		particle.particleForCPU.color = { rng.NextFloatRange(0.0f, 1.0f), rng.NextFloatRange(0.0f, 1.0f), rng.NextFloatRange(0.0f, 1.0f), 1.0f };
		particle.lifeTime = lifeTime_;
		particle.currentTime = 0.0f;
		particles_.push_back(particle);
	}

}

void FloatingParticle::Update()
{
	angle_ += speedDegree_ * DegToRad() * yunity::fixedTimeStep_;
	for (auto& particle : particles_) {
		if (Length(targetPosition_) > 0.0f) {
			float t = 1.0f - std::powf(1.0f - particle.currentTime, 2);
			particle.transform.translate = Lerp(particle.transform.translate, targetPosition_, t);
			particle.transform.translate = Add(particle.transform.translate, { 0.0f, particle.velocity.y * std::cosf(particle.currentTime * particle.velocity.y), 0.0f });
			particle.particleForCPU.color.w = 1.0f - (particle.currentTime / particle.lifeTime);
		}
		else {
			Vector3 rotate = { angle_, angle_, angle_ };
			Matrix4x4 rotateMatrix = MakeRotateXYZMatrix(rotate);
			Vector3 move = TransformNormal(particle.velocity, rotateMatrix);
			particle.transform.translate = Add(centerPosition_, move);
		}
		particle.BillboardMatrix(*camera_);
	}

	if (isLerp_) {
		BaseParticle::Update();
	}

	if (particles_.size() == 0) {
		isActive_ = false;
	}

#ifdef _DEBUG
	yunity::GlobalVariables* globalVariables = yunity::GlobalVariables::GetInstance();
	const char* groupName = "FloatingParticle";
	lifeTime_ = globalVariables->GetFloatValue(groupName, "LifeTime");
	power_ = globalVariables->GetFloatValue(groupName, "Power");
	speedDegree_ = globalVariables->GetFloatValue(groupName, "SpeedDegree");
	minPosition_ = globalVariables->GetVector3Value(groupName, "MinPosition");
	maxPosition_ = globalVariables->GetVector3Value(groupName, "MaxPosition");
#endif // _DEBUG


}
