#include "WindParticle.h"

#include "GlobalVariables.h"

void WindParticle::Initialize(yunity::Camera* camera)
{
	isActive_ = true;
	camera_ = camera;

	yunity::GlobalVariables* globalVariables = yunity::GlobalVariables::GetInstance();
	const char* groupName = "WindParticle";
	ascent_ = globalVariables->GetFloatValue(groupName, "Ascent");
	lifeTime_ = globalVariables->GetFloatValue(groupName, "LifeTime");
	particleColor_ = globalVariables->GetVector4Value(groupName, "ParticleColor");
	maxParticle_ = globalVariables->GetIntValue(groupName, "MaxParticle");
	minTranslate_ = globalVariables->GetVector2Value(groupName, "MinTranslate");
	maxTranslate_ = globalVariables->GetVector2Value(groupName, "MaxTranslate");
	particleScale_ = globalVariables->GetVector3Value(groupName, "ParticleScale");

	particleDrawer_ = std::make_unique<yunity::ParticleDrawer>();
	particleDrawer_->Initialize("white1x1.png");
	particleDrawer_->SetBlendModeType(yunity::BlendModeType::kAdd);
	particleDrawer_->SetMaterial(particleColor_);
	particleDrawer_->SetCamera(camera);
	particles_.clear();

}

void WindParticle::Spawn(const Vector3& position)
{
	if (particles_.size() < maxParticle_) {
		Particle particle;
		particle.transform.translate = position;
		particle.transform.translate.x += rng.NextFloatRange(minTranslate_.x, maxTranslate_.x);
		particle.transform.translate.y += rng.NextFloatRange(minTranslate_.y, maxTranslate_.y);
		particle.transform.scale = particleScale_;
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
