#pragma once
#pragma once

#include <iostream>
#include <variant>
#include <string>
#include <map>

#include "json.hpp"
#include "MathFunction.h"
#include "LevelData.h"
#include "JointData.h"

namespace yunity {
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

		/// <summary>
		/// ファイルから読み込み
		/// </summary>
		/// <param name="fileName"></param>
		/// <returns></returns>
		JointData* LoadJointFile(const std::string& fileName);

	private:
		/// <summary>
		/// オブジェクト読み込み
		/// </summary>
		/// <param name="levelData">レベルデータ</param>
		/// <param name="deserialized">デシリアライズ</param>
		void LoadObjectRecursive(LevelData* levelData, nlohmann::json deserialized);

		/// <summary>
		/// ジョイント読み込み
		/// </summary>
		/// <param name="jointData"></param>
		/// <param name="deserialized"></param>
		void LoadJointRecursive(JointData* jointData, nlohmann::json deserialized);
	private:
		LevelEditor() = default;
		~LevelEditor() = default;
		LevelEditor(const LevelEditor& src) = default;
		LevelEditor& operator=(const LevelEditor& src) = default;
	private:

		std::map<std::string, std::unique_ptr<LevelData>> levelData_;
		std::map<std::string, std::unique_ptr<JointData>> jointData_;

		// グローバル変数の保存先ファイルパス
		const std::string kDirectoryPath = "Resources/Level/";

	};

}
