#pragma once

enum class Object3dRootBindings {
	kWorldTransform, // ワールド変換行列
	kViewProjection, // ビュープロジェクション変換行列
	kRootNode,       // ルートノード
	kTexture,        // テクスチャ
	kMaterial,       // マテリアル
	kLight,          // ライティング
	kCamera,         // カメラ
	kPointLight,
	kCount,          // 最大数
};

enum class  SkinningRootBindings {
	kWorldTransform, // ワールド変換行列
	kViewProjection, // ビュープロジェクション変換行列
	kRootNode,       // ルートノード
	kTexture,        // テクスチャ
	kMaterial,       // マテリアル
	kLight,          // ライティング
	kCamera,         // カメラ
	kPointLight,
	kMatrixPalette,
	kCount,          // 最大数
};

enum class  SpriteRootBindings {
	kWorldViewProjection, // ワールドビュープロジェクション変換行列
	kTexture, // テクスチャ
	kCount // 最大数
};

enum class  ParticleRootBindings {
	kWorldTransform, // ワールド変換行列
	kViewProjection, // ビュープロジェクション変換行列
	kTexture,        // テクスチャ
	kMaterial,       // マテリアル
	kCount,          // 最大数
};

enum class  PrimitiveRootBindings {
	kWorldTransform, // ワールド変換行列
	kViewProjection, // ビュープロジェクション変換行列
	kCount,          // 最大数
};
