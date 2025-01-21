#pragma once

#include <iostream>
#include <variant>
#include <string>
#include <map>

#include "MathFunction.h"


namespace yunity {
	/// <summary>
/// グローバル変数
/// </summary>
	class GlobalVariables {
	public:
		static GlobalVariables* GetInstance();

		/// <summary>
		/// グループの作成
		/// </summary>
		/// <param name="groupName">グループ名</param>
		void CreateGroup(const std::string& groupName);

		/// <summary>
		/// 毎フレーム処理
		/// </summary>
		void Updata();

		/// <summary>
		/// ファイルに書き出し
		/// </summary>
		/// <param name="GroupName">グループ</param>
		void SaveFile(const std::string& groupName);

		/// <summary>
		/// ファイルから読み込む
		/// </summary>
		/// <param name="GroupName">グループ</param>
		void LoadFile(const std::string& groupName);

		/// <summary>
		/// ディレクトリの全ファイル読み込み
		/// </summary>
		void LoadFiles();

		// 値のセット(int)
		void SetValue(const std::string& groupName, const std::string& key, int32_t value);
		// 値のセット(float)
		void SetValue(const std::string& groupName, const std::string& key, float value);
		// 値のセット(Vector3)
		void SetValue(const std::string& groupName, const std::string& key, Vector3 value);
		// 値のセット(Vector2)
		void SetValue(const std::string& groupName, const std::string& key, Vector2 value);
		// 値のセット(Vector4)
		void SetValue(const std::string& groupName, const std::string& key, Vector4 value);

		// 項目の追加(int)
		void AddItem(const std::string& groupName, const std::string& key, int32_t value);
		// 項目の追加(float)
		void AddItem(const std::string& groupName, const std::string& key, float value);
		// 項目の追加(Vector3)
		void AddItem(const std::string& groupName, const std::string& key, Vector3 value);
		// 項目の追加(Vector2)
		void AddItem(const std::string& groupName, const std::string& key, Vector2 value);
		// 項目の追加(Vector4)
		void AddItem(const std::string& groupName, const std::string& key, Vector4 value);

		// 値の取得(int32_t)
		int32_t GetIntValue(const std::string& groupName, const std::string& key);
		// 値の取得(float)
		float GetFloatValue(const std::string& groupName, const std::string& key);
		// 値の取得(Vector3)
		Vector3 GetVector3Value(const std::string& groupName, const std::string& key);
		// 値の取得(Vector2)
		Vector2 GetVector2Value(const std::string& groupName, const std::string& key);
		// 値の取得(Vector4)
		Vector4 GetVector4Value(const std::string& groupName, const std::string& key);

	private:
		GlobalVariables() = default;
		~GlobalVariables() = default;
		GlobalVariables(const GlobalVariables& src) = default;
		GlobalVariables& operator=(const GlobalVariables& src) = default;
	private:

		// 項目
		using Item = std::variant<int32_t, float, Vector3, Vector2, Vector4>;

		// グループ
		using Group = std::map<std::string, Item>;

		// 全データ
		std::map<std::string, Group> datas_;

		// グローバル変数の保存先ファイルパス
		const std::string kDirectoryPath = "Resources/GlobalVariables/";

	};
}
