#include "SmokeParticle.h"

#include "ModelManager.h"

void SmokeParticle::Initialize(yunity::Camera* camera)
{
	isActive_ = true;
	camera_ = camera;

	particleDrawer_ = std::make_unique<yunity::ParticleDrawer>();
	particleDrawer_->Initialize("white1x1.png");
	particleDrawer_->SetBlendModeType(yunity::BlendModeType::kAdd);
	particleDrawer_->SetCamera(camera);
	particles_.clear();

	frequencyTime_ = 0.0f;
	SetFixedTime(fixedTime_);
	spawnTime_ = 0.0f;
}

void SmokeParticle::Spawn(const Vector3& position)
{
	//particles_.clear();
	if (!spawnTime_) {
		Particle particle;
		particle.transform.translate = position;
		particle.transform.scale = { 0.0f, 0.0f, 0.0f };
		particle.particleForCPU.color = { 1.0f, 1.0f, 1.0f , 1.0f };
		particle.lifeTime = 2.0f;
		particle.currentTime = 0.0f;

		particles_.push_back(particle);
		spawnTime_ = 0.5f;
	}
}

void SmokeParticle::Update()
{
	frequencyTime_ += fixedTime_;
	
	if (spawnTime_) {
		spawnTime_ -= fixedTime_;
		spawnTime_ = std::clamp(spawnTime_, 0.0f, 2.0f);
	}

	for (auto& particle : particles_) {
		float scale = particle.currentTime;
		scale = std::clamp(scale, 0.0f, 1.0f);
		particle.transform.scale = { scale, scale, scale };
		//particle.transform.translate = Add(particle.transform.translate, particle.velocity);
		particle.particleForCPU.color.w = 1.0f - (particle.currentTime / particle.lifeTime);
		particle.BillboardMatrix(*camera_);
	}
	BaseParticle::Update();
}
