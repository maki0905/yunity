#include "Cloth.h"

#include "CameraManager.h"
#include "Input.h"

void Cloth::Initialize(World* world)
{
	for (uint32_t x = 0; x < 4; x++) {
		for (uint32_t y = 0; y < 4; y++) {
			points_[y + x * 4] = std::make_unique<Object3D>();
			points_[y + x * 4]->Initialize(nullptr, world, Collider::Shape::kSphere);
			points_[y + x * 4]->SetHitBoxSize({ 1.0f, 1.0f, 1.0f });
			points_[y + x * 4]->SetScale({ 0.1f, 0.1f, 0.1f });
			points_[y + x * 4]->SetCamera(CameraManager::GetInstance()->GetCamera());
			points_[y + x * 4]->SetCollisionAttribute(kCollisionAttributePlayer);
			points_[y + x * 4]->SetPosition(Vector3{ 5.0f * (float)x, 5.0f * (float)y, 0.0f });
			if (y < 3) {
				points_[y + x * 4]->SetMass(1.0f);
			}
			world->Add(points_[y + x * 4].get());
		}
	}
	for (uint32_t index = 0; index < 24; index++) {
		lines_[index] = std::make_unique<PrimitiveDrawer>();
		lines_[index].reset(PrimitiveDrawer::Create());
		lines_[index]->SetCamera(CameraManager::GetInstance()->GetCamera());
	}

	for (uint32_t x = 0; x < 4; x++) {
		for (uint32_t y = 0; y < 3; y++) {
			joints_[y + x * 3] = std::make_unique<SpringJoint>();
			joints_[y + x * 3]->CreateSpringJoint(points_[y + x * 4].get(), points_[y + x * 4 + 1].get());
			joints_[y + x * 3]->EnableSpring(0, true);
			joints_[y + x * 3]->EnableSpring(1, true);
			joints_[y + x * 3]->SetEquilibriumPoint(0, 5.0f);
			joints_[y + x * 3]->SetEquilibriumPoint(1, 5.0f);
			joints_[y + x * 3]->SetStiffness(0, stiffness_);
			joints_[y + x * 3]->SetStiffness(1, stiffness_);
			joints_[y + x * 3]->SetDamping(0, dampar_);
			joints_[y + x * 3]->SetDamping(1, dampar_);
			world->AddJoint(joints_[y + x * 3].get());

			joints_[y + 12 + x * 3] = std::make_unique<SpringJoint>();
			joints_[y + 12 + x * 3]->CreateSpringJoint(points_[y + x * 3].get(), points_[y + x * 3 + 4].get());
			joints_[y + 12 + x * 3]->EnableSpring(0, true);
			joints_[y + 12 + x * 3]->EnableSpring(1, true);
			joints_[y + 12 + x * 3]->SetEquilibriumPoint(0, 5.0f);
			joints_[y + 12 + x * 3]->SetEquilibriumPoint(1, 5.0f);
			joints_[y + 12 + x * 3]->SetStiffness(0, stiffness_);
			joints_[y + 12 + x * 3]->SetStiffness(1, stiffness_);
			joints_[y + 12 + x * 3]->SetDamping(0, dampar_);
			joints_[y + 12 + x * 3]->SetDamping(1, dampar_);
			world->AddJoint(joints_[y + 12 + x * 3].get());
		}
	}
}

void Cloth::Update()
{
	float force = 10.0f;
	if (Input::GetInstance()->TriggerKey(DIK_3)) {
		for (uint32_t index = 0; index < 4; index++) {
			points_[index * 4]->AddForce({ force, 0.0f, 0.0f }, Body::ForceMode::kImpulse);
			points_[index * 4 + 1]->AddForce({ force * 0.8f, 0.0f, 0.0f }, Body::ForceMode::kImpulse);
			points_[index * 4 + 2]->AddForce({ force * 0.5f, 0.0f, 0.0f }, Body::ForceMode::kImpulse);
		}
	}
	if (Input::GetInstance()->TriggerKey(DIK_4)) {
		for (uint32_t index = 0; index < 4; index++) {
			points_[index * 4]->AddForce({ -force, 0.0f, 0.0f }, Body::ForceMode::kImpulse);
			points_[index * 4 + 1]->AddForce({ -force * 0.8f, 0.0f, 0.0f }, Body::ForceMode::kImpulse);
			points_[index * 4 + 2]->AddForce({ -force * 0.5f, 0.0f, 0.0f }, Body::ForceMode::kImpulse);
		}
	}
	if (Input::GetInstance()->PushKey(DIK_5)) {
		for (uint32_t index = 0; index < 4; index++) {
			points_[index * 4]->AddForce({ 5.0f, 0.0f, 0.0f }, Body::ForceMode::kForce);
		}
	}
	if (Input::GetInstance()->PushKey(DIK_6)) {
		for (uint32_t index = 0; index < 4; index++) {
			points_[index * 4]->AddForce({ -5.0f, 0.0f, 0.0f }, Body::ForceMode::kForce);
		}
	}
}

void Cloth::Draw()
{
	for (uint32_t x = 0; x < 4; x++) {
		for (uint32_t y = 0; y < 3; y++) {
			lines_[y + x * 3]->Draw(points_[y + x * 4]->GetMatWorldTranslation(), points_[y + x * 4 + 1]->GetMatWorldTranslation());
			lines_[y + 12 + x * 3]->Draw(points_[y + x * 3]->GetMatWorldTranslation(), points_[y + x * 3 + 4]->GetMatWorldTranslation());
		}
	}

}
