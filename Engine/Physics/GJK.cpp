#include "GJK.h"

#include "Body.h"
#include "World.h"

void yunity::GJKCalculation(Object3D* colliderA, Object3D* colliderB, World* world)
{
	// 位置取得
	Vector3 colliderAPosition = colliderA->GetTranslation();
	Vector3 colliderBPosition = colliderB->GetTranslation();

	Point a, b, c, d;

	// 初期の探索方向を設定
	Vector3 search_dir = Subtract(colliderAPosition, colliderBPosition);

	// 2点を求める
	CalculateSearchPoint(c, search_dir, colliderA, colliderB);
	search_dir = Multiply(-1.0f, c.point); // 原点に向かう方向に変更
	CalculateSearchPoint(b, search_dir, colliderA, colliderB);
	if (Dot(b.point, search_dir) < 0.0f) { // 原点方向に進めない場合、衝突していない
		return;
	}

	search_dir = Cross(Cross(Subtract(c.point, b.point), Multiply(-1.0f, b.point)), Subtract(c.point, b.point));
	if (search_dir.x == 0.0f && search_dir.y == 0.0f && search_dir.z == 0.0f) {
		search_dir = Cross(Subtract(c.point, b.point), Vector3(1.0f, 0.0f, 0.0f));
		if (search_dir.x == 0.0f && search_dir.y == 0.0f && search_dir.z == 0.0f) {
			search_dir = Cross(Subtract(c.point, b.point), Vector3(0.0f, 0.0f, -1.0f));
		}
	}


	int simp_dim = 2; // Simplexの次元数
	// GJKメインループ
	for (int i = 0; i < 64; i++) {
		CalculateSearchPoint(a, search_dir, colliderA, colliderB);
		if (Dot(a.point, search_dir) < 0.0f) {
			return;
		}
		simp_dim++;
		if (simp_dim == 3) {
			UpdateSimplex3(a, b, c, d, simp_dim, search_dir);
		}
		else if (UpdateSimplex4(a, b, c, d, simp_dim, search_dir)) {
			EPA(a, b, c, d, colliderA, colliderB, world);
			return;
		}
	}

	return;
}

void yunity::UpdateSimplex3(Point& a, Point& b, Point& c, Point& d, int& simp_dim, Vector3& search_dir)
{

	Vector3 AB = Subtract(b.point, a.point);
	Vector3 AC = Subtract(c.point, a.point);
	Vector3 AO = Multiply(-1.0f, a.point);
	Vector3 n = Cross(AB, AC); // 法線（右手系）

	// AB 辺の外側にあるか
	if (Dot(Cross(n, AC), AO) > 0.0f) {
		// AC 辺の外
		b = a;
		search_dir = Cross(Cross(AC, AO), AC).Normalize();
		simp_dim = 2;
		return;
	}

	// AC 辺の外側にあるか
	if (Dot(Cross(AB, n), AO) > 0.0f) {
		// AB 辺の外
		c = a;
		search_dir = Cross(Cross(AB, AO), AB).Normalize();
		simp_dim = 2;
		return;
	}

	// 原点が三角形の内側にある（平面内）
	simp_dim = 3;
	if (Dot(n, AO) > 0.0f) {
		d = c;
		c = b;
		b = a;
		search_dir = n;
	}
	else {
		d = b;
		b = a;
		search_dir = Multiply(-1.0f, n);
	}


}

bool yunity::UpdateSimplex4(Point& a, Point& b, Point& c, Point& d, int& simp_dim, Vector3& search_dir)
{
	// 法線ベクトルを計算
	Vector3 ABC = Cross(Subtract(b.point, a.point), Subtract(c.point, a.point));
	Vector3 ACD = Cross(Subtract(c.point, a.point), Subtract(d.point, a.point));
	Vector3 ADB = Cross(Subtract(d.point, a.point), Subtract(b.point, a.point));

	// 原点への方向ベクトル
	Vector3 AO = Multiply(-1.0f, a.point);
	simp_dim = 3;

	if (Dot(ABC, AO) > 0.0f) { // ABC面が最も近い
		d = c;
		c = b;
		b = a;
		search_dir = ABC.Normalize();
		return false;
	}

	if (Dot(ACD, AO) > 0.0f) { // ACD面が最も近い
		b = a;
		search_dir = ACD.Normalize();
		return false;
	}

	if (Dot(ADB, AO) > 0.0f) { // ADB面が最も近い
		c = d;
		d = b;
		b = a;
		search_dir = ADB.Normalize();
		return false;
	}
	return true;

}

void yunity::EPA(Point& a, Point& b, Point& c, Point& d, Object3D* colliderA, Object3D* colliderB, World* world)
{
	Point faces[64][4];

	faces[0][0] = a;
	faces[0][1] = b;
	faces[0][2] = c;
	faces[0][3].point = Normalize(Cross(Subtract(b.point, a.point), Subtract(c.point, a.point))); //ABC
	faces[1][0] = a;
	faces[1][1] = c;
	faces[1][2] = d;
	faces[1][3].point = Normalize(Cross(Subtract(c.point, a.point), Subtract(d.point, a.point))); //ACD
	faces[2][0] = a;
	faces[2][1] = d;
	faces[2][2] = b;
	faces[2][3].point = Normalize(Cross(Subtract(d.point, a.point), Subtract(b.point, a.point))); //ADB
	faces[3][0] = b;
	faces[3][1] = d;
	faces[3][2] = c;
	faces[3][3].point = Normalize(Cross(Subtract(d.point, b.point), Subtract(c.point, b.point))); //BDC

	int numFaces = 4;
	int closestFace;

	for (int index = 0; index < 64; index++) {
		// 最も原点に近い面を探す
		float minDistance = Dot(faces[0][0].point, faces[0][3].point);
		closestFace = 0;
		for (int i = 1; i < numFaces; i++) {
			float distance = Dot(faces[i][0].point, faces[i][3].point);
			if (distance < minDistance) {
				minDistance = distance;
				closestFace = i;
			}

		}

		// 最も原点に近い面の法線方向を取得
		Vector3 searchDistance = faces[closestFace][3].point;
		Point p;
		CalculateSearchPoint(p, searchDistance, colliderA, colliderB);

		if (Dot(p.point, searchDistance) - minDistance < 0.00001f) { // 収束判定
			// 最近傍面の法線と投影点を計算
			Plane closestPlane = PlaneFromTri(
				faces[closestFace][0].point,
				faces[closestFace][1].point,
				faces[closestFace][2].point);
			Vector3 projectionPoint = ProjectPointOntoPlane(closestPlane, Vector3(0, 0, 0));

			// バリツェントリック座標を求める
			float u, v, w;
			Barycentric(
				faces[closestFace][0].point,
				faces[closestFace][1].point,
				faces[closestFace][2].point,
				projectionPoint, u, v, w);

			// A, B の衝突点（ワールド座標）
			Vector3 worldPointA = Add(Add(Multiply(u, faces[closestFace][0].supportA), Multiply(v, faces[closestFace][1].supportA)), Multiply(w, faces[closestFace][2].supportA));
			Vector3 worldPointB = Add(Add(Multiply(u, faces[closestFace][0].supportB), Multiply(v, faces[closestFace][1].supportB)), Multiply(w, faces[closestFace][2].supportB));

			// 衝突法線と深度（ワールド空間）
			Vector3 normal = Subtract(worldPointA, worldPointB);
			float penetration = normal.Length();
			if (penetration < 1e-6f) return; // 無視
			normal = Divide(normal, penetration); // 正規化
			//penetration = std::clamp(penetration, 0.0f, 100.0f); // 安全クランプ

			// 衝突点（ワールド空間）
			Vector3 contactPoint = Multiply(0.5f, Add(worldPointA, worldPointB));

			// ローカル座標へ変換
			Matrix4x4 invMatA = Inverse(colliderA->GetWorldTransform()->matWorld_);
			Matrix4x4 invMatB = Inverse(colliderB->GetWorldTransform()->matWorld_);
			Vector3 localA = TransformPoint(contactPoint, invMatA);
			Vector3 localB = TransformPoint(contactPoint, invMatB);

			// マニフォールド登録
			yunity::World::PersistentManifold persistentManifold;
			persistentManifold.colliderA = colliderA;
			persistentManifold.colliderB = colliderB;
			persistentManifold.localPointA = localA;
			persistentManifold.localPointB = localB;
			persistentManifold.contactNormal = normal; // ワールド空間での法線
			persistentManifold.penetrationDepth = penetration;
			world->AddPersistentManifold(persistentManifold);
			return;
		}

		//if (Dot(p.point, searchDistance) - minDistance < 0.00001f) { // 収束判定
		//	// 投影点を求める
		//	Plane closestPlane = PlaneFromTri(faces[closestFace][0].point, faces[closestFace][1].point, faces[closestFace][2].point);
		//	Vector3 projectionPoint = ProjectPointOntoPlane(closestPlane, Vector3(0, 0, 0));

		//	// バリツェントリック座標を求める
		//	float u, v, w;
		//	Barycentric(faces[closestFace][0].point, faces[closestFace][1].point, faces[closestFace][2].point,
		//		projectionPoint, u, v, w);


		//	// ワールド座標の衝突点を求める（supportA/B はワールド座標）
		//	Vector3 worldPointA = Add(Add(Multiply(u, faces[closestFace][0].supportA), Multiply(v, faces[closestFace][1].supportA)), Multiply(w, faces[closestFace][2].supportA));
		//	Vector3 worldPointB = Add(Add(Multiply(u, faces[closestFace][0].supportB), Multiply(v, faces[closestFace][1].supportB)), Multiply(w, faces[closestFace][2].supportB));

		//	//// 衝突法線と深度（ワールド空間）
		//	//Vector3 normal = Subtract(worldPointA, worldPointB);
		//	//float penetration = normal.Length();
		//	//if (penetration < 1e-6f) return; // ほぼゼロなら無視
		//	//normal = Divide(normal, penetration); // 正規化
		//	//penetration = std::clamp(penetration, 0.0f, 0.05f); // 過剰な深さを抑制

		//	//// 衝突点（ワールド空間）
		//	//Vector3 contactPoint = Multiply(0.5f, Add(worldPointA, worldPointB));

		//	//// ローカル座標へ変換
		//	//Matrix4x4 invMatA = Inverse(colliderA->GetWorldTransform()->matWorld_);
		//	//Matrix4x4 invMatB = Inverse(colliderB->GetWorldTransform()->matWorld_);

		//	//Vector3 localA = TransformPoint(contactPoint, invMatA);
		//	//Vector3 localB = TransformPoint(contactPoint, invMatB);

		//	// 衝突点を求める
		//	Vector3 localA = Add(Add(Multiply(u, faces[closestFace][0].supportA), Multiply(v, faces[closestFace][1].supportA)), Multiply(w, faces[closestFace][2].supportA));
		//	Vector3 localB = Add(Add(Multiply(u, faces[closestFace][0].supportB), Multiply(v, faces[closestFace][1].supportB)), Multiply(w, faces[closestFace][2].supportB));
		//	float penetration = Subtract(localA, localB).Length();
		//	Vector3 normal = Subtract(localA, localB).Normalize();

		//	localA = Subtract(localA, colliderA->GetTranslation());
		//	localB = Subtract(localB, colliderB->GetTranslation());

		//	// 衝突データを格納
		//	//collisionInfo.AddContactPoint(localA, localB, normal, penetration);
		//	yunity::World::PersistentManifold persistentManifold;
		//	persistentManifold.colliderA = colliderA;
		//	persistentManifold.colliderB = colliderB;
		//	persistentManifold.localPointA = localA;
		//	persistentManifold.localPointB = localB;
		//	persistentManifold.contactNormal = normal;
		//	persistentManifold.penetrationDepth = penetration;
		//	world->AddPersistentManifold(persistentManifold);
		//	return;
		//}

		Point loose_edges[32][2];
		int num_loose_edges = 0;

		for (int i = 0; i < numFaces; i++)
		{
			if (Dot(faces[i][3].point, Subtract(p.point, faces[i][0].point)) > 0)
			{
				for (int j = 0; j < 3; j++)
				{
					Point current_edge[2] = { faces[i][j], faces[i][(j + 1) % 3] };
					bool found_edge = false;
					for (int k = 0; k < num_loose_edges; k++)
					{
						if (PointsEqual(loose_edges[k][1].point, current_edge[0].point) && PointsEqual(loose_edges[k][0].point, current_edge[1].point)) {
							loose_edges[k][0] = loose_edges[num_loose_edges - 1][0];
							loose_edges[k][1] = loose_edges[num_loose_edges - 1][1];
							num_loose_edges--;
							found_edge = true;
							k = num_loose_edges;
						}
					}

					if (!found_edge) {
						if (num_loose_edges >= 32) break;
						loose_edges[num_loose_edges][0] = current_edge[0];
						loose_edges[num_loose_edges][1] = current_edge[1];
						num_loose_edges++;
					}
				}

				faces[i][0] = faces[numFaces - 1][0];
				faces[i][1] = faces[numFaces - 1][1];
				faces[i][2] = faces[numFaces - 1][2];
				faces[i][3] = faces[numFaces - 1][3];
				numFaces--;
				i--;
			}
		}

		for (int i = 0; i < num_loose_edges; i++)
		{
			if (numFaces >= 64) break;
			faces[numFaces][0] = loose_edges[i][0];
			faces[numFaces][1] = loose_edges[i][1];
			faces[numFaces][2] = p;
			faces[numFaces][3].point = Cross(Subtract(loose_edges[i][0].point, loose_edges[i][1].point), Subtract(loose_edges[i][0].point, p.point)).Normalize();


			Vector3 center = Divide(Add(Add(faces[numFaces][0].point, faces[numFaces][1].point), p.point), 3.0f);
			if (Dot(center, faces[numFaces][3].point) < 0) {
				Point temp = faces[numFaces][0];
				faces[numFaces][0] = faces[numFaces][1];
				faces[numFaces][1] = temp;
				faces[numFaces][3].point = Multiply(-1.0f, faces[numFaces][3].point);
			}
			numFaces++;
		}
	}
	return;
}

void yunity::CalculateSearchPoint(Point& point, Vector3& search_dir, Object3D* colliderA, Object3D* colldierB)
{
	point.supportB = colldierB->GetCollisionShape()->Support(search_dir, *colldierB->GetWorldTransform());
	point.supportA = colliderA->GetCollisionShape()->Support(Multiply(-1.0f, search_dir), *colliderA->GetWorldTransform());
	point.point = Subtract(point.supportB, point.supportA);
}

bool yunity::PointsEqual(const Vector3& a, const Vector3& b, float epsilon)
{
	return
		(
			fabs(a.x - b.x) < epsilon &&
			fabs(a.y - b.y) < epsilon &&
			fabs(a.z - b.z) < epsilon
			);
}