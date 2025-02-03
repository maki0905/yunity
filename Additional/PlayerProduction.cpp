#include "PlayerProduction.h"

#include "GlobalVariables.h"
#include "CommonData.h"
#include "Tradition.h"
#include "Player.h"
#include "EngineTimeStep.h"
#include "RenderTexture.h"
#include "SceneManager.h"

void InGameProduction::Initialize()
{
	time_ = 0.0f;

	yunity::GlobalVariables* globalVariables = yunity::GlobalVariables::GetInstance();
	const char* groupName = "InGameProduction";

	endCameraPosition_ = globalVariables->GetVector3Value(groupName, "EndCameraPosition");
}

void InGameProduction::Update(Player* player)
{
	/*if (!Tradition::GetInstance()->GetOut()) {
		CommonData::GetInstance()->flagState_ = FlagState::kPlay;
	}*/
	Tradition::GetInstance()->Update();
	player->GetCamera()->SetOffset(Lerp({ 0.0f, 0.0f, 0.0f }, endCameraPosition_, std::clamp(1.0f - Tradition::GetInstance()->GetTime(), 0.0f, 1.0f)));
	player->SetScale(Lerp({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, std::clamp(1.0f - Tradition::GetInstance()->GetTime(), 0.0f, 1.0f)));
	if (player->GetWorldTransform()->scale_.x == 1.0f) {
		player->SetIsScore(true);
		player->SetIsReticle(true);
		player->ChangeProductionState(nullptr);
		CommonData::GetInstance()->flagState_ = FlagState::kPlay;
	}

}

void ResetProduction::Initialize()
{
	yunity::GlobalVariables* globalVariables = yunity::GlobalVariables::GetInstance();
	const char* groupName = "ResetProduction";

	time_ = 0.0;
	resetTime_ = globalVariables->GetFloatValue(groupName, "ResetTime");
}

void ResetProduction::Update(Player* player)
{
	if (time_ / resetTime_ >= 1.0f) {
		time_ += yunity::fixedTimeStep_;
		float t = std::clamp((time_ / resetTime_) - 1.0f, 0.0f, 1.0f);
		player->SetScale(Lerp({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, t));
		if (t == 1.0f) {
			CommonData::GetInstance()->flagState_ = FlagState::kPlay;
			time_ = 0.0f;
			player->ChangeProductionState(nullptr);
		}
	}
	else {
		time_ += yunity::fixedTimeStep_;
		time_ = std::clamp(time_, 0.0f, resetTime_);
		player->GetCamera()->SetTranslate(Lerp(player->GetDeathCameraPosition(), {player->GetSpawnPoint().x, player->GetSpawnPoint().y + player->GetCamera()->GetOffset().y, player->GetDeathCameraPosition().z}, time_ / resetTime_));
		player->SetTranslation(Lerp(player->GetDeathPosition(), player->GetSpawnPoint(), time_));
		if (time_ / resetTime_ == 1.0f) {
			player->Reset();
			player->SetIsTrigger(false);
			player->GetCamera()->SetTarget(player->GetWorldTransform());
		}
	}
}

void DeathProduction::Initialize()
{
	yunity::GlobalVariables* globalVariables = yunity::GlobalVariables::GetInstance();
	const char* groupName = "DeathProduction";

	time_ = 0.0;
	topPosition_ = { 0.0f, 0.0f, 0.0f };
	deathUpTime_ = globalVariables->GetFloatValue(groupName, "DeathUpTime");
	deathDownTime_ = globalVariables->GetFloatValue(groupName, "DeathDownTime");
	deathUp_ = globalVariables->GetVector3Value(groupName, "DeathUp");
	deathDown_ = globalVariables->GetVector3Value(groupName, "DeathDown");
}

void DeathProduction::Update(Player* player)
{
	if (time_ / deathUpTime_ < 1.0f) {
		time_ += yunity::fixedTimeStep_;
		time_ = std::clamp(time_, 0.0f, deathUpTime_);
		float t = 1.0f - std::powf(1.0f - (time_ / deathUpTime_), 5);
		Vector3 pos = Slerp(player->GetDeathPosition(), Add(player->GetDeathPosition(), deathUp_), t);
		pos.x = player->GetDeathPosition().x;
		player->SetTranslation(pos);
		if (time_ / deathUpTime_ == 1.0f) {
			topPosition_ = player->GetTranslation();
		}
	}
	else if ((time_ / deathUpTime_) - 1.0f < 1.0f) {
		time_ += yunity::fixedTimeStep_;
		float t = std::clamp((time_ / deathUpTime_) - 1.0f, 0.0f, deathDownTime_);
		Vector3 pos = Slerp(topPosition_, Add(topPosition_, deathDown_), t);
		pos.x = topPosition_.x;
		player->SetTranslation(pos);
	}
	else {
		CommonData::GetInstance()->flagState_ = FlagState::kReset;
		player->SetDeathPosition(player->GetTranslation());
		player->SetIsTrigger(false);
		player->SetScale({ 0.0f, 0.0f, 0.0f });
		time_ = 0.0f;
		player->ChangeProductionState(std::make_unique<ResetProduction>());
	}
}

void GoalProduction::Initialize()
{
	yunity::GlobalVariables* globalVariables = yunity::GlobalVariables::GetInstance();
	const char* groupName = "GoalProduction";

	time_ = 0.0;
	goalTime_ = globalVariables->GetFloatValue(groupName, "GoalTime");
	clearTime_ = globalVariables->GetFloatValue(groupName, "ClearTime");
	zeemOutPositionZ_ = globalVariables->GetFloatValue(groupName, "ZeemOutPositionZ_");
}

void GoalProduction::Update(Player* player)
{
	if (time_ / goalTime_ < 1.0f) {
		time_ += yunity::fixedTimeStep_;
		time_ = std::clamp(time_, 0.0f, goalTime_);
		player->SetPosition(Lerp(player->GetOldPlayerPosition(), {player->GetGoalPoint().x, player->GetMatWorldTranslation().y, 0.0f}, time_ / goalTime_));

		if (time_ / goalTime_ == 1.0f) {
			CommonData::GetInstance()->flagState_ = FlagState::kClear;
			clearCameraPosition_ = player->GetCamera()->GetTranslate();
			player->GetCamera()->SetTarget(nullptr);
			Tradition::GetInstance()->Initialize();
			Tradition::GetInstance()->Start();
		}
	}
	else if (((time_ / goalTime_) - 1.0f) / clearTime_ < 1.0f) {
		if (player->Result()) {
			time_ += yunity::fixedTimeStep_;
			float t = std::clamp(((time_ / goalTime_) - 1.0f), 0.0f, clearTime_);
			player->GetCamera()->SetTranslate(Lerp(clearCameraPosition_, { clearCameraPosition_.x, player->GetGoalPoint().y, zeemOutPositionZ_ }, t / clearTime_));
			if (t / clearTime_ == 1.0f) {
				clearCameraPosition_ = player->GetCamera()->GetTranslate();
				yunity::RenderTexture::GetInstance()->SelectPostEffect(yunity::PostEffects::kRadialBlur, true);
			}
		}
	}
	else {
		time_ += yunity::fixedTimeStep_;
		float t = std::clamp(((time_ / goalTime_) - 1.0f) / clearTime_ - 1.0f, 0.0f, 1.0f);
		player->GetCamera()->SetTranslate(Lerp(clearCameraPosition_, player->GetGoalPoint(), t));
		Tradition::GetInstance()->Update();
		if (!Tradition::GetInstance()->GetIn()) {
			CommonData::GetInstance()->isGoal_ = false;
			yunity::SceneManager::GetInstance()->ChangeScene("TITLE");
			Tradition::GetInstance()->Initialize();
		}
	}

}

