#pragma once
#pragma once

#include <iostream>
#include <variant>
#include <string>
#include <map>

#include "json.hpp"
#include "MathFunction.h"
#include "LevelData.h"

//struct ObjectData {
//	Vector3 scaling;
//	Vector3 rotation;
//	Vector3 translation;
//	std::string fileName;
//};
//
//struct LevelData {
//	std::vector<ObjectData> objects;
//};

/*
* @brief レベルエディタークラス
*/
class LevelEditor {
public:
	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static LevelEditor* GetInstance();

	/// <summary>
	/// ファイルから読み込む
	/// </summary>
	/// <param name="GroupName">グループ</param>
	LevelData* LoadFile(const std::string& fileName);

private:
	/// <summary>
	/// オブジェクト読み込み
	/// </summary>
	/// <param name="levelData">レベルデータ</param>
	/// <param name="deserialized">デシリアライズ</param>
	void LoadObjectRecursive(LevelData* levelData, nlohmann::json deserialized);
private:
	LevelEditor() = default;
	~LevelEditor() = default;
	LevelEditor(const LevelEditor& src) = default;
	LevelEditor& operator=(const LevelEditor& src) = default;
private:

	//// 項目
	//using Item = std::variant<int32_t, float, Vector3>;

	//// グループ
	//using Group = std::map<std::string, Item>;

	//// 全データ
	//std::map<std::string, Group> datas_;

	std::map<std::string, LevelData*> levelData_;

	// グローバル変数の保存先ファイルパス
	const std::string kDirectoryPath = "Resources/Level/";

};
