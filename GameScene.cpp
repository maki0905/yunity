#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>


GameScene::GameScene(){}

GameScene::~GameScene(){}

void GameScene::Initialize(DirectXCommon* dxCommon)
{
	dxCommon_ = dxCommon;
}

void GameScene::Update()
{

}

void GameScene::Draw()
{
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	dxCommon_->ClearDepthBuffer();
	Model::PreDraw(commandList);

	Model::PostDraw();

}
