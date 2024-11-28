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
	circleParticle.lifeTime = lifeTime_[0];
	circleParticle.currentTime = 0.0f;
	circleParticles_.push_back(circleParticle);

	for (uint32_t i = 0; i < 4; ++i) {
		Particle particle;
		particle.transform.translate = position;

		if (i % 2 == 0) {
			particle.transform.rotate.z = degree_ * 1.5f * DegToRad();
		}
		else {
			particle.transform.rotate.z = degree_ * 0.5f * DegToRad();
		}
		particle.velocity = {
			float(power_ * std::cosf(degree_ * i * DegToRad()) - power_ * std::sinf(degree_ * i * DegToRad())),
			float(power_ * std::sinf(degree_ * i * DegToRad()) + power_ * std::cosf(degree_ * i * DegToRad())),
			0.0f
		};

		particle.particleForCPU.color = { 1.0f, 1.0f, 1.0f, 1.0f };
		particle.lifeTime = lifeTime_[1];
		particle.currentTime = 0.0f;
		particles_.push_back(particle);
	}

}

void PointParticle::Update()
{
	for (auto& particle : circleParticles_) {
		Vector3 scale = Lerp({ 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, particle.currentTime / particle.lifeTime);
		particle.transform.scale = scale;
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
