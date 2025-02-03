#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "Camera.h"
#include "World.h"
#include "Body.h"
#include "Tag.h"

namespace yunity {

	/*
	* @brief 3Dオブジェクトクラス
	*/
	class Object3D : public Body {
	public:
		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~Object3D();

		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="model">3Dモデル</param>
		/// <param name="world">物理空間</param>
		/// <param name="shape">形状</param>
		void Initialize(Model* model, World* world, Collider::Shape shape);

		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="world">物理空間</param>
		/// <param name="shape">形状</param>
		void Initialize(World* world, Collider::Shape shape);
		//void Initialize(std::unique_ptr<Model> model, World* world, Collider::Shape shape);

		/// <summary>
		/// 更新
		/// </summary>
		virtual void Update();

		/// <summary>
		/// 描画
		/// </summary>
		virtual void Draw();

		/// <summary>
		/// 衝突時に呼ばれる固有の処理
		/// </summary>
		virtual void OnCollisionEvent();

		/// <summary>
		/// 衝突時に呼ばれる固有の処理
		/// </summary>
		virtual void OnTriggerEvent();

		/// <summary>
		/// setter
		/// </summary>

		void SetPosition(const Vector3& position) { worldTransform_.translation_ = position; }
		void SetRotation(const Vector3& rotation) { worldTransform_.rotation_ = rotation; }
		void SetScale(const Vector3& scale) { worldTransform_.scale_ = scale; }
		void SetCamera(Camera* camera) {
			camera_ = camera;
			if (model_) {
				model_->SetCamera(camera);
			}
		}
		void SetModel(const std::string& modelName, Model* model);
		void SetEnableLighting(bool onOff);
		void SetDirectionalLight(Model::DirectionalLight directionalLight);
		void SetFileName(const std::string fileName) { fileName_ = fileName; }
		void SetHitBody(Body* body) { hitBody_ = body; }
		void SetDirectionLight(DirectionLight* directionLight);
		void SetTag(const Tag& tag) { tag_ = tag; }

		/// <summary>
		/// getter
		/// </summary>

		Vector3 GetTranslation() { return worldTransform_.translation_; }
		Model* GetModel();
		Model* GetModel(const std::string& modelName) { return models_[modelName].get(); }
		Object3D::Body* GetHitBody() { return hitBody_; }
		Tag GetTag() { return tag_; }
		Camera* GetCamera() { return camera_; }

	protected:
		WorldTransform worldTransform_;
		std::unordered_map<std::string, std::unique_ptr<Model>> models_;
		std::unique_ptr<Model> model_;
		Camera* camera_;
	private:
		bool isHit_;
		uint32_t texture_;
		std::string fileName_;
		Object3D::Body* hitBody_;
		Tag tag_ = Tag::kDefalt;
	};
}
