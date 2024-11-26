#include "PointParticle.h"

void PointParticle::Initialize(yunity::Camera* camera)
{
	isActive_ = true;
	camera_ = camera;

	circleParticleDrawer_ = std::make_unique<yunity::ParticleDrawer>();
	circleParticleDrawer_->Initialize("hole.png");
	circleParticleDrawer_->SetBlendModeType(yunity::BlendModeType::kAdd);
	circleParticleDrawer_->SetCamera(camera);
	
	ellipseParticleDrawer_ = std::make_unique<yunity::ParticleDrawer>();
	ellipseParticleDrawer_->Initialize("ellipse.png");
	ellipseParticleDrawer_->SetBlendModeType(yunity::BlendModeType::kAdd);
	ellipseParticleDrawer_->SetCamera(camera);

	circleParticles_.clear();
	particles_.clear();

	SetFixedTime(1.0f / 30.0f);

}

void PointParticle::Spawn(const Vector3& position)
{
	circleParticles_.clear();
	particles_.clear();

	Particle circleParticle;
	circleParticle.transform.translate = position;
	circleParticle.particleForCPU.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	circleParticle.lifeTime = 2.0f;
	circleParticle.currentTime = 0.0f;
	circleParticles_.push_back(circleParticle);


	const float degree = 90.0f;
	for (uint32_t i = 0; i < 4; ++i) {
		Particle particle;
		particle.transform.translate = position;
		
		if (i % 2 == 0) {
			particle.transform.rotate.z = 135.0f * DegToRad();
		}
		else {
			particle.transform.rotate.z = 45.0f * DegToRad();
		}
		particle.velocity = {
			float(power_ * std::cosf(degree * i * std::numbers::pi_v<float> / 180.0f) - power_ * std::sinf(degree * i * std::numbers::pi_v<float> / 180.0f)),
			float(power_ * std::sinf(degree * i * std::numbers::pi_v<float> / 180.0f) + power_ * std::cosf(degree * i * std::numbers::pi_v<float> / 180.0f)),
			0.0f
		};

		particle.particleForCPU.color = {1.0f, 1.0f, 1.0f, 1.0f};
		particle.lifeTime = 2.0f;
		particle.currentTime = 0.0f;
		particles_.push_back(particle);
	}

}

void PointParticle::Update()
{
	for (auto& particle : circleParticles_) {
		Vector3 scale = Lerp({ 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, particle.currentTime / particle.lifeTime);
		particle.transform.scale = scale;
		//particle.particleForCPU.color.w = 1.0f - (particle.currentTime / particle.lifeTime);
		particle.BillboardMatrix(*camera_);
	}

	for (auto& particle : particles_) {
		particle.transform.translate = Add(particle.transform.translate, particle.velocity);
		particle.particleForCPU.color.w = 1.0f - (particle.currentTime / particle.lifeTime);
		particle.BillboardMatrix(*camera_);
	}
	BaseParticle::Update(&circleParticles_);
	BaseParticle::Update();
}

void PointParticle::Draw()
{
	circleParticleDrawer_->Draw(circleParticles_);
	ellipseParticleDrawer_->Draw(particles_);
}
