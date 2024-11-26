#include "FireworksParticle.h"

#include "ModelManager.h"

void FireworksParticle::Initialize(yunity::Camera* camera)
{
	isActive_ = false;
	camera_ = camera;

	particleDrawer_ = std::make_unique<yunity::ParticleDrawer>();
	//particleDrawer_->Initialize(yunity::ModelManager::GetInstance()->GetModelData(obj, "Brick"));
	particleDrawer_->Initialize("white1x1.png");
	particleDrawer_->SetBlendModeType(yunity::BlendModeType::kAdd);
	particleDrawer_->SetCamera(camera);

	SetFixedTime(1.0f / 30.0f);
}

void FireworksParticle::Spawn(const Vector3& position)
{
	particles_.clear();
	isActive_ = true;
	for (uint32_t i = 0; i < 10; ++i) {
		/*Particle* particle = new Particle();
		particle->transform.translate = { i * 2.0f, i * 2.0f, 0.0f };
		particle->velocity = { rng.NextFloatRange(-0.1f, 0.1f), rng.NextFloatRange(-0.1f, 0.1f), 0.0f };
		particle->particleForCPU.color = { rng.NextFloatRange(0.0f, 1.0f),rng.NextFloatRange(0.0f, 1.0f),rng.NextFloatRange(0.0f, 1.0f), 1.0f };
		particle->lifeTime = 5.0f;
		particles_.push_back(particle);*/
		Particle particle;
		particle.transform.translate = position;


		particle.velocity = {
			float(power_ * std::cosf(36.0f * i * std::numbers::pi_v<float> / 180.0f) - power_ * std::sinf(36.0f * i * std::numbers::pi_v<float> / 180.0f)),
			float(power_ * std::sinf(36.0f * i * std::numbers::pi_v<float> / 180.0f) + power_ * std::cosf(36.0f * i * std::numbers::pi_v<float> / 180.0f)),
			0.0f
		};

		particle.particleForCPU.color = { rng.NextFloatRange(0.0f, 1.0f), rng.NextFloatRange(0.0f, 1.0f) , rng.NextFloatRange(0.0f, 1.0f) , 1.0 };
		//particle.particleForCPU.color = {1.0f, 1.0f, 1.0f, 1.0f};
		particle.lifeTime = 10.0f;
		particle.currentTime = 0.0f;
		particles_.push_back(particle);
	}
}

void FireworksParticle::Update()
{
	for (auto& particle : particles_) {
		particle.velocity.y -= 0.01f;
		particle.transform.rotate.z += torqe_;
		particle.transform.translate = Add(particle.transform.translate, particle.velocity);
		particle.particleForCPU.color.w = 1.0f - (particle.currentTime / particle.lifeTime);
		particle.BillboardMatrix(*camera_);
	}
	BaseParticle::Update();
	if (particles_.size() == 0) {
		isActive_ = false;
	}
}
