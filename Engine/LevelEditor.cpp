#include "LevelEditor.h"

#include <cassert>
#include "imgui/imgui.h"
#include <fstream>
#include <Windows.h>


LevelEditor* LevelEditor::GetInstance()
{
	static LevelEditor instance;

	return &instance;
}

LevelData* LevelEditor::LoadFile(const std::string& fileName)
{
	// 読み込むJSONファイルのフルパスを合成
	std::string filePath = kDirectoryPath + fileName + ".json";

	// 読み込み用ファイルストリーム
	std::ifstream file;
	// ファイルを読み込み用に開く
	file.open(filePath);

	// ファイルオープン失敗をチェック
	if (file.fail()) {
		std::string message = "Failed open Failure";
		MessageBoxA(nullptr, message.c_str(), "LevelEditor", 0);
		assert(0);
	}

	nlohmann::json deserialized;

	// 解凍
	file >> deserialized;

	// 正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();
	// 正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	// レベルデータ格納用インスタンスを生成
	LevelData* levelData = new LevelData();

	LoadObjectRecursive(levelData, deserialized);

	levelData_[fileName] = levelData;

	// ファイルを閉じる
	file.close();

	return levelData_[fileName];

}

void LevelEditor::LoadObjectRecursive(LevelData* levelData, nlohmann::json deserialized)
{
	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {
		assert(object.contains("type"));

		// 種別を取得
		std::string type = object["type"].get<std::string>();

		// MESH
		if (type.compare("MESH") == 0) {
			// 要素追加
			levelData->objects.emplace_back(LevelData::ObjectData{});
			// 今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData->objects.back();
			if (object.contains("file_name")) {
				// ファイル名
				objectData.fileName = object["file_name"];
			}

			// トランスフォームのパラメーター読み込み
			nlohmann::json& transform = object["transform"];
			// 平行移動
			objectData.translation.x = (float)transform["translation"][0];
			objectData.translation.y = (float)transform["translation"][2];
			objectData.translation.z = (float)transform["translation"][1];
			// 回転角
			objectData.rotation.x = -(float)transform["rotation"][0];
			objectData.rotation.y = -(float)transform["rotation"][2];
			objectData.rotation.z = -(float)transform["rotation"][1];
			// スケーリング
			objectData.scaling.x = (float)transform["scaling"][0];
			objectData.scaling.y = (float)transform["scaling"][2];
			objectData.scaling.z = (float)transform["scaling"][1];

			// 当たり判定の読み込み
			if (object.contains("collider")) {
				nlohmann::json& collider = object["collider"];
				if (collider["type"] == "BOX") {
					if (objectData.rotation.x == 0.0f && objectData.rotation.y == 0.0f && objectData.rotation.z == 0.0f) {
						objectData.shape = Collider::Shape::kAABB;
					}
					else {
						objectData.shape = Collider::Shape::kOBB;
					}
				}
				else if (collider["type"] == "SPHERE") {

				}

				// サイズ
				objectData.size.x = (float)collider["size"][0];
				objectData.size.y = (float)collider["size"][2];
				objectData.size.z = (float)collider["size"][1];
				// 位置
				objectData.center.x = (float)collider["center"][0];
				objectData.center.y = (float)collider["center"][2];
				objectData.center.z = (float)collider["center"][1];
				// 衝突するかどうか
				objectData.isTrigger = collider["isTrigger"];
			}

		}

		// 再帰
		if (object.contains("children")) {
			LoadObjectRecursive(levelData, deserialized);
		}

	}

}



