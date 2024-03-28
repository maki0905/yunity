#pragma once
#include "IScene.h"

#include "Sprite.h"
class TitleScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void DrawBack() override;
	void Draw3D() override;
	void DrawFront() override;

private:
	std::unique_ptr<Sprite> sprite_ = nullptr;

};

