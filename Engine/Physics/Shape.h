#pragma once

#include "MathFunction.h"
#include "Sphere.h"
#include "AABB.h"
#include "OBB.h"
#include "WorldTransform.h"

/// <summary>
/// AABB生成
/// </summary>
/// <param name="traslation"></param>
/// <param name="size"></param>
/// <returns></returns>
AABB CreateAABB(Vector3 traslation, Vector3 size);

/// <summary>
/// OBB生成
/// </summary>
/// <param name="translation"></param>
/// <param name="rotation"></param>
/// <param name="size"></param>
/// <returns></returns>
OBB CreateOBB(Vector3 translation, Vector3 rotation, Vector3 size);

namespace yunity{

	enum class ShapeType {
		kSphere,
		kBox,
		kInvalid
	};

	class CollisionShape {
	public:
		CollisionShape() : shapeType_(ShapeType::kInvalid) {}
		virtual ~CollisionShape();
		virtual void GetAABB(const WorldTransform& worldTransform, Vector3& aabbMin, Vector3& aabbMax) const = 0;
		//virtual void GetBoundingSphere(const Vector3& cneter, const Vector3& radius) const = 0;
		virtual Matrix3x3 CalculateLocalInertia(float mass) const = 0;
		virtual Vector3 Support(const Vector3& direction, WorldTransform& worldTransform) = 0;
		ShapeType GetShapeType() const { return shapeType_; }

	protected:
		ShapeType shapeType_;

	};

	// Box
	class BoxShape : public CollisionShape {
	public:
		BoxShape(const Vector3& halfExtents) : halfExtents_(halfExtents) {
			shapeType_ = ShapeType::kBox;
		}

		// AABBを取得
		void GetAABB(const WorldTransform& worldTransform, Vector3& aabbMin, Vector3& aabbMax) const override {
			aabbMin = Subtract(worldTransform.translation_, halfExtents_);
			aabbMax = Add(worldTransform.translation_, halfExtents_);
		}

		// 慣性テンソルを計算
		Matrix3x3 CalculateLocalInertia(float mass) const override {
			Matrix3x3 result = MakeIdentity3x3();

			float massFactor = mass / 12.0f;

			Vector3 lenght = Multiply(2.0f, halfExtents_);

			result.m[0][0] = massFactor * (lenght.y * lenght.y + lenght.z * lenght.z);
			result.m[1][1] = massFactor * (lenght.x * lenght.x + lenght.z * lenght.z);
			result.m[2][2] = massFactor * (lenght.x * lenght.x + lenght.y * lenght.y);

			return result;
		}

		// サポートベクトルを計算
		Vector3 Support(const Vector3& direction, WorldTransform& worldTransform) {
			// 方向ベクトルをローカル座標系に変換
			Vector3 localDirection = TransformNormal(direction, Inverse(worldTransform.GetRotateMatrix()));

			// サポートベクトルを計算
			Vector3 result;
			result.x = (localDirection.x > 0.0f) ? halfExtents_.x : -halfExtents_.x;
			result.y = (localDirection.y > 0.0f) ? halfExtents_.y : -halfExtents_.y;
			result.z = (localDirection.z > 0.0f) ? halfExtents_.z : -halfExtents_.z;

			// ワールド座標系に変換
			result = Add(TransformVector3(result, worldTransform.GetRotateMatrix()), worldTransform.translation_);
			return result;

		}

	private:
		Vector3 halfExtents_;

	};

	// 球
	class SphereShape : public CollisionShape {
	public:
		SphereShape(float radius) : radius_(radius) {
			shapeType_ = ShapeType::kSphere;
		}

		// AABBを取得
		void GetAABB(const WorldTransform& worldTransform, Vector3& aabbMin, Vector3& aabbMax) const override {
			aabbMin = Subtract(worldTransform.translation_, Vector3(radius_, radius_, radius_));
			aabbMax = Add(worldTransform.translation_, Vector3(radius_, radius_, radius_));
		}

		// 慣性テンソルを計算
		Matrix3x3 CalculateLocalInertia(float mass) const override {
			Matrix3x3 result = MakeIdentity3x3();
			float inertiaValue = (2.0f / 5.0f) * mass * radius_ * radius_;
			result.m[0][0] = inertiaValue;
			result.m[1][1] = inertiaValue;
			result.m[2][2] = inertiaValue;
			return result;
		}

		// サポートベクトルを計算
		Vector3 Support(const Vector3& direction, WorldTransform& worldTransform) {
			Vector3 result = Add(Multiply(radius_, Normalize(direction)), worldTransform.translation_);
			return result;
		}

	private:
		float radius_;
	};


}