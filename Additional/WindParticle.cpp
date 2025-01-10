#include "WindParticle.h"

void WindParticle::Initialize(yunity::Camera* camera)
{
	isActive_ = true;
	camera_ = camera;

	particleDrawer_ = std::make_unique<yunity::ParticleDrawer>();
	particleDrawer_->Initialize("white1x1.png");
	particleDrawer_->SetBlendModeType(yunity::BlendModeType::kAdd);
	particleDrawer_->SetMaterial({ 1.0f, 1.0f, 1.0f, 0.3f });
	particleDrawer_->SetCamera(camera);
	particles_.clear();

}

void WindParticle::Spawn(const Vector3& position)
{
	if (particles_.size() < 100) {
		Particle particle;
		particle.transform.translate = position;
		particle.transform.translate.x += rng.NextFloatRange(-35.0f, 35.0f);
		particle.transform.translate.y += rng.NextFloatRange(-2.0f, 2.0f);
		particle.transform.scale = { 0.2f, 1.0f, 1.0f };
		particle.particleForCPU.color = { 1.0f, 1.0f, 1.0f , 1.0f };
		particle.lifeTime = lifeTime_;
		particle.currentTime = 0.0f;
		particles_.push_back(particle);
	}
}

void WindParticle::Clear()
{
	particles_.clear();
}

void WindParticle::Update()
{

	for (auto& particle : particles_) {
		particle.transform.translate.y += ascent_;
		particle.particleForCPU.color.w = 1.0f - (particle.currentTime / particle.lifeTime);
		particle.BillboardMatrix(*camera_);
	}
	BaseParticle::Update();
}
