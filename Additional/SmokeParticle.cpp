#include "SmokeParticle.h"

#include "ModelManager.h"
#include "GlobalVariables.h"
#include "EngineTimeStep.h"


void SmokeParticle::Initialize(yunity::Camera* camera)
{
	isActive_ = true;
	camera_ = camera;

	particleDrawer_ = std::make_unique<yunity::ParticleDrawer>();
	particleDrawer_->Initialize("white1x1.png");
	particleDrawer_->SetBlendModeType(yunity::BlendModeType::kAdd);
	particleDrawer_->SetCamera(camera);
	particles_.clear();

	spawnTime_ = 0.0f;

	yunity::GlobalVariables* globalVariables = yunity::GlobalVariables::GetInstance();
	const char* groupName = "SmokeParticle";
	spawnInterval_ = globalVariables->GetFloatValue(groupName, "SpawnInterval");
	ascent_ = globalVariables->GetFloatValue(groupName, "Ascent");
	lifeTime_ = globalVariables->GetFloatValue(groupName, "LifeTime");
	minScale_ = globalVariables->GetFloatValue(groupName, "MinScale");
	maxScale_ = globalVariables->GetFloatValue(groupName, "MaxScale");
	minTranslate_ = globalVariables->GetFloatValue(groupName, "MinTranslate");
	maxTranslate_ = globalVariables->GetFloatValue(groupName, "MaxTranslate");

}

void SmokeParticle::Spawn(const Vector3& position)
{
	if (!spawnTime_) {
		Particle particle;
		particle.transform.translate = position;
		particle.transform.translate.y += rng.NextFloatRange(0.0f, 0.5f);
		float scale = rng.NextFloatRange(0.0f, 0.5f);
		particle.transform.scale = { scale, scale, 0.0f };
		particle.particleForCPU.color = { 1.0f, 1.0f, 1.0f , 1.0f };
		particle.lifeTime = lifeTime_;
		particle.currentTime = 0.0f;
		particles_.push_back(particle);
		spawnTime_ = spawnInterval_;
	}
}

void SmokeParticle::Update()
{
	if (spawnTime_) {
		spawnTime_ -= yunity::fixedTimeStep_;
		spawnTime_ = std::clamp(spawnTime_, 0.0f, 1.0f);
	}

	for (auto& particle : particles_) {
		particle.transform.scale = Add(particle.transform.scale, { yunity::fixedTimeStep_, yunity::fixedTimeStep_, 0.0f });
		particle.transform.translate.y += std::powf(particle.currentTime / particle.lifeTime, 5.0f) * ascent_;
		particle.particleForCPU.color.w = 1.0f - (particle.currentTime / particle.lifeTime);
		particle.BillboardMatrix(*camera_);
	}
	BaseParticle::Update();

#ifdef _DEBUG
	yunity::GlobalVariables* globalVariables = yunity::GlobalVariables::GetInstance();
	const char* groupName = "SmokeParticle";
	spawnInterval_ = globalVariables->GetFloatValue(groupName, "SpawnInterval");
	ascent_ = globalVariables->GetFloatValue(groupName, "Ascent");
	lifeTime_ = globalVariables->GetFloatValue(groupName, "LifeTime");
	minScale_ = globalVariables->GetFloatValue(groupName, "MinScale");
	maxScale_ = globalVariables->GetFloatValue(groupName, "MaxScale");
	minTranslate_ = globalVariables->GetFloatValue(groupName, "MinTranslate");
	maxTranslate_ = globalVariables->GetFloatValue(groupName, "MaxTranslate");
#endif // _DEBUG

}
