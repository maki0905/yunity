#pragma once

#include "Vector3.h"

class Player;

class PlayerProduction {
public:
	virtual ~PlayerProduction() = default;
	virtual void Initialize() = 0;
	virtual void Update(Player* player) = 0;
protected:
	// イージング用
	float time_;
};

class InGameProduction : public PlayerProduction {
public:
	void Initialize();
	void Update(Player* player);
private:
	// カメラの終点
	Vector3 endCameraPosition_;
};

class ResetProduction : public PlayerProduction {
public:
	void Initialize();
	void Update(Player* player);
private:
	float resetTime_;
};

class DeathProduction : public PlayerProduction {
public:
	void Initialize();
	void Update(Player* player);
private:
	float deathUpTime_;
	float deathDownTime_;
	Vector3 deathUp_;
	Vector3 deathDown_;
	Vector3 topPosition_;

};

class GoalProduction : public PlayerProduction {
public:
	void Initialize();
	void Update(Player* player);
private:
	float goalTime_;
	float clearTime_;
	float zeemOutPositionZ_;
	Vector3 clearCameraPosition_;
};