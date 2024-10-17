#pragma once

#include <Windows.h>
#include <d3d12.h>
#include <string>
#include <wrl.h>

#include "MathFunction.h"
#include "PipelineState.h"
#include "RootSignature.h"

/*
* @スプライト
*/
class Sprite
{
public:
	/// <summary>
	/// 頂点データ構造体
	/// </summary>
	struct VertexPosUv {
		Vector3 pos;
		Vector2 uv;
	};	

	/// <summary>
	/// 定数バッファ用データ構造体
	/// </summary>
	struct ConstBufferData {
		Vector4 color;
		Matrix4x4 mat;
	};

public:
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	static void StaticInitialize();

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="commandList"></param>
	static void PreDraw(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// スプライト生成
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="position">座標</param>
	/// <param name="color">色</param>
	/// <param name="anchorpoint">アンカーポイント</param>
	/// <param name="isFlipx">左右反転</param>
	/// <param name="isFlipY">上下反転</param>
	/// <returns></returns>
	static Sprite* Create(uint32_t textureHandle, Vector2 position, Vector4 color = { 1,1,1,1 }, Vector2 anchorpoint = { 0.0f, 0.0f }, bool isFlipx = false, bool isFlipY = false);

private:
	// 頂点数
	static const int kVertNum = 4;
	// ディスクリプタサイズ
	static UINT descriptorHandleIncrementSize_;
	// コマンドリスト
	static ID3D12GraphicsCommandList* commandList_;
	//// ルートシグネチャー
	static RootSignature* rootSignature_;
	//// パイプラインステートオブジェクト
	static PipelineState* pipelineState_;
	// 射影行列
	static Matrix4x4 matProjection_;
	// ルートシグネチャー
	//static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
	// パイプラインステートオブジェクト
	//static Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState_;


public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Sprite();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Sprite(uint32_t textureHandle, Vector2 position, Vector2 size, Vector4 color, Vector2 anchorpoint, bool isFlipx, bool isFlipY);

	

	void Finalize();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize();

	/// <summary>
	/// テクスチャハンドルの設定
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void SetTextureHandle(uint32_t textureHandle);

	uint32_t GetTextureHandle() { return textureHandle_; }

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const Vector2 position);

	const Vector2& GetPosition() { return position_; }

	/// <summary>
	/// 角度の設定
	/// </summary>
	/// <param name="rotation">角度</param>
	void SetRotation(float rotation);
	float GetRotation() { return rotation_; }

	/// <summary>
	/// サイズの設定
	/// </summary>
	/// <param name="size">サイズ</param>
	void SetSize(const Vector2& size);
	const Vector2& GetSize() { return size_; }

	/// <summary>
	/// アンカーポイントの設定
	/// </summary>
	/// <param name="anchorPoint">アンカーポイント</param>
	void SetAnchorPoint(const Vector2& anchorPoint);
	const Vector2 GetAnchorPoint() { return anchorPoint_; }

	/// <summary>
	/// 色の設定
	/// </summary>
	/// <param name="color">色</param>
	void SetColor(const Vector4& color) { color_ = color; }
	const Vector4& GetColor() { return color_; }

	/// <summary>
	/// 左右反転の設定
	/// </summary>
	/// <param name="isFlipX">左右反転</param>
	void SetIsFlipX(bool isFlipX);
	bool GetIsFlipX(){ return isFlipX_; }

	/// <summary>
	/// 上下反転の設定
	/// </summary>
	/// <param name="isFlipY">上下反転</param>
	void SetIsFlipY(bool isFlipY);
	bool GetIsFlipY() { return isFlipY_; }

	/// <summary>
	/// テクスチャ範囲設定
	/// </summary>
	/// <param name="texBase">テクスチャ左上座標</param>
	/// <param name="texSize">テクスチャサイズ</param>
	void SetTextureRect(const Vector2 texBase, const Vector2& texSize);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// 頂点バッファマップ
	VertexPosUv* vertMap = nullptr;
	// 定数バッファマップ
	ConstBufferData* constMap = nullptr;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	// テクスチャ番号
	UINT textureHandle_ = 0;
	// Z軸回りの回転角
	float rotation_ = 0.0f;
	// 座標
	Vector2 position_{};
	// スプライト幅、高さ
	Vector2 size_ = { 100.0f, 100.0f };
	// アンカーポイント
	Vector2 anchorPoint_ = { 0.0f, 0.0f };
	// ワールド行列
	Matrix4x4 matWorld_{};
	// 色
	Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	// 左右反転
	bool isFlipX_ = false;
	// 上下反転
	bool isFlipY_ = false;
	// テクスチャ始点
	Vector2 texBase_ = { 0.0f, 0.0f };
	// テクスチャ幅、高さ
	Vector2 texSize_ = { 100.0f, 100.0f };
	// リソース設定
	D3D12_RESOURCE_DESC resourceDesc_;

private:
	/// <summary>
	/// 頂点データ転送
	/// </summary>
	void TransferVertices();

};

