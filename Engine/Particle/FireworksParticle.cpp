#include "FireworksParticle.h"

#include "ModelManager.h"
#include "GlobalVariables.h"

void FireworksParticle::Initialize(yunity::Camera* camera)
{
	isActive_ = false;
	camera_ = camera;

	particleDrawer_ = std::make_unique<yunity::ParticleDrawer>();
	particleDrawer_->Initialize("white1x1.png");
	particleDrawer_->SetBlendModeType(yunity::BlendModeType::kAdd);
	particleDrawer_->SetCamera(camera);

	yunity::GlobalVariables* globalVariables = yunity::GlobalVariables::GetInstance();
	const char* groupName = "FireworksParticle";
	power_ = globalVariables->GetFloatValue(groupName, "Power");
	torqe_ = globalVariables->GetFloatValue(groupName, "Torqe");
	ascent_ = globalVariables->GetFloatValue(groupName, "Ascent");
	lifeTime_ = globalVariables->GetFloatValue(groupName, "LifeTime");
	spawnPosition_ = globalVariables->GetVector3Value(groupName, "SpawnPosition");
	minSpawnPosition_ = globalVariables->GetVector3Value(groupName, "MinSpawnPosition");
	maxSpawnPosition_ = globalVariables->GetVector3Value(groupName, "MaxSpawnPosition");
	minVelocity_ = globalVariables->GetVector3Value(groupName, "MinVelocity");
	maxVelocity_ = globalVariables->GetVector3Value(groupName, "MaxVelocity");
}

void FireworksParticle::Spawn(const Vector3& position)
{
	particles_.clear();
	isActive_ = true;
	for (uint32_t i = 0; i < particleQuantity_; ++i) {
		Particle particle;
		particle.transform.translate = Add(position, spawnPosition_);
		particle.transform.translate = Add(particle.transform.translate, { rng.NextFloatRange(minSpawnPosition_.x, maxSpawnPosition_.x), rng.NextFloatRange(minSpawnPosition_.y, maxSpawnPosition_.y), 0.0f });


		particle.velocity = {
			rng.NextFloatRange(minVelocity_.x, maxVelocity_.x),
			rng.NextFloatRange(minVelocity_.y, maxVelocity_.y),
			rng.NextFloatRange(minVelocity_.z, maxVelocity_.z)
		};

		particle.particleForCPU.color = { rng.NextFloatRange(0.0f, 1.0f), rng.NextFloatRange(0.0f, 1.0f) , rng.NextFloatRange(0.0f, 1.0f) , 1.0 };
		particle.lifeTime = lifeTime_;
		particle.currentTime = 0.0f;
		particles_.push_back(particle);
	}
}

void FireworksParticle::Update()
{
	for (auto& particle : particles_) {
		particle.velocity.y -= ascent_;
		particle.transform.rotate = Add(particle.transform.rotate, particle.velocity);
		particle.transform.translate = Add(particle.transform.translate, particle.velocity);
		particle.particleForCPU.color.w = 1.0f - (particle.currentTime / particle.lifeTime);
		particle.BillboardMatrix(*camera_);
	}
	BaseParticle::Update();
	if (particles_.size() == 0) {
		isActive_ = false;
	}

#ifdef _DEBUG
	yunity::GlobalVariables* globalVariables = yunity::GlobalVariables::GetInstance();
	const char* groupName = "FireworksParticle";
	power_ = globalVariables->GetFloatValue(groupName, "Power");
	torqe_ = globalVariables->GetFloatValue(groupName, "Torqe");
	ascent_ = globalVariables->GetFloatValue(groupName, "Ascent");
	lifeTime_ = globalVariables->GetFloatValue(groupName, "LifeTime");
	spawnPosition_ = globalVariables->GetVector3Value(groupName, "SpawnPosition");
	minSpawnPosition_ = globalVariables->GetVector3Value(groupName, "MinSpawnPosition");
	maxSpawnPosition_ = globalVariables->GetVector3Value(groupName, "MaxSpawnPosition");
	minVelocity_ = globalVariables->GetVector3Value(groupName, "MinVelocity");
	maxVelocity_ = globalVariables->GetVector3Value(groupName, "MaxVelocity");
#endif // _DEBUG

}
