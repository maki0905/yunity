#include "LevelEditor.h"

#include <cassert>
#include "imgui/imgui.h"
#include <fstream>
#include <Windows.h>
#include "MathFunction.h"

yunity::LevelEditor* yunity::LevelEditor::GetInstance()
{
	static LevelEditor instance;

	return &instance;
}

yunity::LevelData* yunity::LevelEditor::LoadFile(const std::string& fileName)
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

	if (levelData_.count(fileName)) {
		return levelData_[fileName].get();
	}
	// レベルデータ格納用インスタンスを生成
	std::unique_ptr<LevelData> levelData = std::make_unique<LevelData>();

	LoadObjectRecursive(levelData.get(), deserialized);

	levelData_[fileName] = std::move(levelData);

	// ファイルを閉じる
	file.close();

	return levelData_[fileName].get();

}

yunity::JointData* yunity::LevelEditor::LoadJointFile(const std::string& fileName)
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

	if (jointData_.count(fileName)) {
		return jointData_[fileName].get();
	}

	// レベルデータ格納用インスタンスを生成
	std::unique_ptr<JointData> jointData = std::make_unique<JointData>();

	LoadJointRecursive(jointData.get(), deserialized);

	jointData_[fileName] = std::move(jointData);

	// ファイルを閉じる
	file.close();

	return jointData_[fileName].get();
}

void yunity::LevelEditor::LoadObjectRecursive(LevelData* levelData, nlohmann::json deserialized)
{
	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {
		assert(object.contains("type"));

		if (object["disabled"].contains("disabled")) {
			bool disabled = object["disabled"].get<bool>();
			if (disabled) {
				continue;
			}
		}

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
			objectData.rotation.x = -(float)transform["rotation"][0] * DegToRad();
			objectData.rotation.y = -(float)transform["rotation"][2] * DegToRad();
			objectData.rotation.z = -(float)transform["rotation"][1] * DegToRad();
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
					objectData.shape = Collider::Shape::kSphere;
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
				// 属性
				objectData.attribute = collider["attribute"];
			}

			if (object.contains("body")) {
				nlohmann::json& body = object["body"];
				objectData.mass = (float)body["mass"];
				objectData.drag = (float)body["drag"];
				objectData.miu = (float)body["miu"];
				objectData.frictionCombine = (uint32_t)body["frictionCombine"];
				objectData.bounciness = (float)body["bounciness"];
				objectData.bounceCombine = (uint32_t)body["bounceCombine"];
			}

			if (object.contains("eventtrigger")) {
				nlohmann::json& eventtrigger = object["eventtrigger"];
				objectData.serialNumber = (uint32_t)eventtrigger["serialnumber"];
			}

			if (object.contains("jointIDs")) {
				nlohmann::json& jointIDs = object["jointIDs"];
				for (uint32_t i = 0; i < jointIDs.size(); i++) {
					uint32_t id = 0;
					if (object.contains("jointIDs")) {
						std::string id_str = object["jointIDs"][i].get<std::string>();
						try {
							id = static_cast<uint32_t>(std::stoul(id_str));
						}
						catch (const std::exception& e) {
							std::cerr << "Failed to convert ID: " << id_str << ", Error: " << e.what() << std::endl;
							continue; // このジョイントをスキップ
						}
					}
					objectData.jointIDs.push_back(id);
				}
			}

			if (object.contains("tag")) {
				nlohmann::json& tag = object["tag"];
				uint32_t num = (uint32_t)tag["tag"];
				objectData.tag_ = static_cast<Tag>(num);
			}

		}

		// EMPTY
		if (type.compare("EMPTY") == 0) {
			// 要素追加
			levelData->objects.emplace_back(LevelData::ObjectData{});
			// 今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData->objects.back();

			objectData.empth = true;

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
				// 属性
				objectData.attribute = (uint32_t)collider["attribute"];
			}

			if (object.contains("body")) {
				nlohmann::json& body = object["body"];
				objectData.mass = (float)body["mass"];
				objectData.drag = (float)body["drag"];
				objectData.miu = (float)body["miu"];
				objectData.frictionCombine = (uint32_t)body["frictionCombine"];
				objectData.bounciness = (float)body["bounciness"];
				objectData.bounceCombine = (uint32_t)body["bounceCombine"];
			}

			if (object.contains("eventtrigger")) {
				nlohmann::json& eventtrigger = object["eventtrigger"];
				objectData.serialNumber = (uint32_t)eventtrigger["serialnumber"];
			}

			if (object.contains("jointIDs")) {
				nlohmann::json& jointIDs = object["jointIDs"];
				for (uint32_t i = 0; i < jointIDs.size(); i++) {
					uint32_t id = 0;
					if (object.contains("jointIDs")) {
						std::string id_str = object["jointIDs"][i].get<std::string>();
						try {
							id = static_cast<uint32_t>(std::stoul(id_str));
						}
						catch (const std::exception& e) {
							std::cerr << "Failed to convert ID: " << id_str << ", Error: " << e.what() << std::endl;
							continue; // このジョイントをスキップ
						}
					}
					objectData.jointIDs.push_back(id);
				}
			}

			if (object.contains("tag")) {
				nlohmann::json& tag = object["tag"];
				uint32_t num = (uint32_t)tag["tag"];
				objectData.tag_ = static_cast<Tag>(num);
			}

		}

		// 再帰
		if (object.contains("children")) {
			LoadObjectRecursive(levelData, deserialized);
		}

	}

}

void yunity::LevelEditor::LoadJointRecursive(JointData* jointData, nlohmann::json deserialized)
{
	// "joints"の全ジョイントを走査
	for (nlohmann::json& joint : deserialized["Joints"]) {

		// IDを取得 (文字列を数値に変換)
		uint32_t id = 0;
		if (joint.contains("ID")) {
			std::string id_str = joint["ID"].get<std::string>();
			try {
				id = static_cast<uint32_t>(std::stoul(id_str));
			}
			catch (const std::exception& e) {
				std::cerr << "Failed to convert ID: " << id_str << ", Error: " << e.what() << std::endl;
				continue; // このジョイントをスキップ
			}
		}
		// 種別ごとに
		std::string type = joint["type"].get<std::string>();

		if (type == "springJoint") {
			JointData::SpringJointParmameter springJointParmeter;
			nlohmann::json& springEnabled = joint["springEnabled"];
			nlohmann::json& equilibriumPoint = joint["equilibriumPoint"];
			nlohmann::json& stiffness = joint["stiffness"];
			nlohmann::json& dampingCoefficient = joint["dampingCoefficient"];

			for (uint32_t i = 0; i < 3; i++) {
				springJointParmeter.springEnabled[i] = (uint32_t)springEnabled[i];
				springJointParmeter.equilibriumPoint[i] = (float)equilibriumPoint[i];
				springJointParmeter.stiffness[i] = (float)stiffness[i];
				springJointParmeter.dampingCoefficient[i] = (float)dampingCoefficient[i];
			}

			springJointParmeter.id = id;
			springJointParmeter.type = JointType::kSpring;

			jointData->springJoints.emplace_back(springJointParmeter);

		}
		else if (type == "pulleyJoint") {
			JointData::PulleyJointParmameter pulleyJointParmeter;
			nlohmann::json& groundAnchor = joint["groundAnchor"];
			nlohmann::json& anchor = joint["anchor"];
			nlohmann::json& ratio = joint["ratio"];
			pulleyJointParmeter.groundAnchorA.x = (float)groundAnchor["A"][0];
			pulleyJointParmeter.groundAnchorA.y = (float)groundAnchor["A"][1];
			pulleyJointParmeter.groundAnchorA.z = (float)groundAnchor["A"][2];
			pulleyJointParmeter.anchorA.x = (float)anchor["A"][0];
			pulleyJointParmeter.anchorA.y = (float)anchor["A"][1];
			pulleyJointParmeter.anchorA.z = (float)anchor["A"][2];
			pulleyJointParmeter.groundAnchorB.x = (float)groundAnchor["B"][0];
			pulleyJointParmeter.groundAnchorB.y = (float)groundAnchor["B"][1];
			pulleyJointParmeter.groundAnchorB.z = (float)groundAnchor["B"][2];
			pulleyJointParmeter.anchorB.x = (float)anchor["B"][0];
			pulleyJointParmeter.anchorB.y = (float)anchor["B"][1];
			pulleyJointParmeter.anchorB.z = (float)anchor["B"][2];
			pulleyJointParmeter.ratio = (float)ratio;

			pulleyJointParmeter.id = id;
			pulleyJointParmeter.type = JointType::kPulley;

			jointData->pulleyJoints.emplace_back(pulleyJointParmeter);

		}

	}
}



