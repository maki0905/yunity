#include "GlobalVariables.h"

#include <cassert>
#include "imgui/imgui.h"
#include "json.hpp"
#include <fstream>
#include <Windows.h>



yunity::GlobalVariables* yunity::GlobalVariables::GetInstance() {
	static GlobalVariables instance;

	return &instance;
}

void yunity::GlobalVariables::CreateGroup(const std::string& groupName) {
	// 指定名のオブジェクトがなければ追加する
	datas_[groupName];

}

void yunity::GlobalVariables::Updata() {
	if (!ImGui::Begin("Global Variables", nullptr, ImGuiWindowFlags_MenuBar)) {
		ImGui::End();
		return;
	}
	if (!ImGui::BeginMenuBar()) {
		return;
	}

	// 各グループについて
	for (std::map<std::string, Group>::iterator itGroup = datas_.begin(); itGroup != datas_.end();
		++itGroup) {
		// グループ名を取得
		const std::string& groupName = itGroup->first;
		// グループの参照を取得
		Group& group = itGroup->second;

		if (!ImGui::BeginMenu(groupName.c_str()))
			continue;

		// 各項目について
		for (std::map<std::string, Item>::iterator itItem = group.begin();
			itItem != group.end(); ++itItem) {
			// 項目名を取得
			const std::string& itemName = itItem->first;
			// 項目の参照を取得
			Item& item = itItem->second;

			// int型の値を保持していれば
			if (std::holds_alternative<int>(item)) {
				int* ptr = std::get_if<int>(&item);
				ImGui::SliderInt(itemName.c_str(), ptr, 0, 100);
			}

			// float型の値を保持していれば
			else if (std::holds_alternative<float>(item)) {
				float* ptr = std::get_if<float>(&item);
				ImGui::SliderFloat(itemName.c_str(), ptr, 0.0f, 1.0f);
			}

			// Vecter3型の値を保持していれば
			else if (std::holds_alternative<Vector3>(item)) {
				Vector3* ptr = std::get_if<Vector3>(&item);
				ImGui::SliderFloat3(itemName.c_str(), reinterpret_cast<float*>(ptr), -10.0f, 10.0f);
			}

			// Vecter2型の値を保持していれば
			else if (std::holds_alternative<Vector2>(item)) {
				Vector2* ptr = std::get_if<Vector2>(&item);
				ImGui::SliderFloat3(itemName.c_str(), reinterpret_cast<float*>(ptr), -10.0f, 10.0f);
			}

			// Vecter4型の値を保持していれば
			else if (std::holds_alternative<Vector4>(item)) {
				Vector4* ptr = std::get_if<Vector4>(&item);
				ImGui::SliderFloat3(itemName.c_str(), reinterpret_cast<float*>(ptr), -10.0f, 10.0f);
			}
		}

		// 改行
		ImGui::Text("\n");

		if (ImGui::Button("Save")) {
			SaveFile(groupName);
			std::string message = std::format("{}.json saved.", groupName);
			MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		}

		ImGui::EndMenu();

	}


	ImGui::EndMenuBar();
	ImGui::End();

}

void yunity::GlobalVariables::SaveFile(const std::string& groupName) {
	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);

	// 未登録チェック
	assert(itGroup != datas_.end());

	nlohmann::json root;
	root = nlohmann::json::object();

	// jsonオブジェクト登録
	root[groupName] = nlohmann::json::object();

	// 各項目について
	for (std::map<std::string, Item>::iterator itItem = itGroup->second.begin();
		itItem != itGroup->second.end(); ++itItem) {

		// 項目名を取得
		const std::string& itemName = itItem->first;
		// 項目の参照を取得
		Item& item = itItem->second;

		// int型の値を保持していれば
		if (std::holds_alternative<int>(item)) {
			// int型の値を登録
			root[groupName][itemName] = std::get<int>(item);
		}
		// float型の値を保持していれば
		else if (std::holds_alternative<float>(item)) {
			// float型の値を登録
			root[groupName][itemName] = std::get<float>(item);
		}
		// Vector3型の値を保持していれば
		else if (std::holds_alternative<Vector3>(item)) {
			// Vector3型のjson配列登録
			Vector3 value = std::get<Vector3>(item);
			root[groupName][itemName] = nlohmann::json::array({ value.x, value.y, value.z });

		}
		// Vector2型の値を保持していれば
		else if (std::holds_alternative<Vector2>(item)) {
			// Vector2型のjson配列登録
			Vector2 value = std::get<Vector2>(item);
			root[groupName][itemName] = nlohmann::json::array({ value.x, value.y});

		}
		// Vector4型の値を保持していれば
		else if (std::holds_alternative<Vector4>(item)) {
			// Vector4型のjson配列登録
			Vector4 value = std::get<Vector4>(item);
			root[groupName][itemName] = nlohmann::json::array({ value.x, value.y, value.z, value.w });

		}

	}

	// ディレクトリがなければ作成する
	std::filesystem::path dir(kDirectoryPath);
	if (!std::filesystem::exists(dir)) {
		std::filesystem::create_directory(dir);
	}

	// 書き込みJSONファイルのフルパスを合成する
	std::string filePath = kDirectoryPath + groupName + ".json";
	// 書き込み用ファイルストリーム
	std::ofstream ofs;
	// ファイルを書き込み用に聞く
	ofs.open(filePath);

	// ファイルオープン失敗?
	if (ofs.fail()) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
		return;
	}

	// ファイルにjson文字列を書き込む(インデント幅4)
	ofs << std::setw(4) << root << std::endl;
	// ファイルを閉じる
	ofs.close();

}

void yunity::GlobalVariables::LoadFile(const std::string& groupName) {
	// 読み込むJSONファイルのフルパスを合成
	std::string filePath = kDirectoryPath + groupName + ".json";
	// 読み込み用ファイルストリーム
	std::ifstream ifs;
	// ファイルを読み込み用に開く
	ifs.open(filePath);

	// ファイルオープン失敗?
	if (ifs.fail()) {
		std::string message = "Failed open Failure";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
	}

	nlohmann::json root;

	// json文字列からjsonのデータ構造に展開
	ifs >> root;
	// ファイルを閉じる
	ifs.close();

	// グループを検索
	nlohmann::json::iterator itGroup = root.find(groupName);

	// 未登録チェック
	assert(itGroup != root.end());

	// 各アイテムについて
	for (nlohmann::json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {
		// アイテム名を取得
		const std::string& itemName = itItem.key();

		// int型の値を保持していれば
		if (itItem->is_number_integer()) {
			// int型の値を登録
			int value = itItem->get<int>();
			SetValue(groupName, itemName, value);
		}
		// float型の値を保持していれば
		else if (itItem->is_number_float()) {
			// float型の値を登録
			double value = itItem->get<double>();
			SetValue(groupName, itemName, static_cast<float>(value));
		}
		// 要素数3の配列であれば
		else if (itItem->is_array() && itItem->size() == 3) {
			// float型のjson配列登録
			Vector3 value = { itItem->at(0), itItem->at(1), itItem->at(2) };
			SetValue(groupName, itemName, value);
		}
		// 要素数2の配列であれば
		else if (itItem->is_array() && itItem->size() == 2) {
			// float型のjson配列登録
			Vector2 value = { itItem->at(0), itItem->at(1)};
			SetValue(groupName, itemName, value);
		}
		// 要素数4の配列であれば
		else if (itItem->is_array() && itItem->size() == 4) {
			// float型のjson配列登録
			Vector4 value = { itItem->at(0), itItem->at(1), itItem->at(2), itItem->at(3)};
			SetValue(groupName, itemName, value);
		}
	}

}

void yunity::GlobalVariables::LoadFiles() {
	const std::string LocalDirectoryPath = "Resources/GlobalVariables/";
	// ディレクトリがなければスキップする
	if (!std::filesystem::exists(LocalDirectoryPath)) {
		return;
	}
	std::filesystem::directory_iterator dir_it(LocalDirectoryPath);
	for (const std::filesystem::directory_entry& entry : dir_it) {
		// ファイルパスを取得
		const std::filesystem::path& filePath = entry.path();

		// ファイル拡張子を取得
		std::string extension = filePath.extension().string();
		// .jsonファイル以外はスキップ
		if (extension.compare(".json") != 0) {
			continue;
		}

		// ファイル読み込み
		LoadFile(filePath.stem().string());

	}
}

void yunity::GlobalVariables::SetValue(const std::string& groupName, const std::string& key, int value) {
	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem = value;
	// 設定した項目をstd::mapに追加
	group[key] = newItem;
}

void yunity::GlobalVariables::SetValue(const std::string& groupName, const std::string& key, float value) {
	// グループの参照を取得 
	Group& group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem = value;
	// 設定した項目をstd::mapに追加
	group[key] = newItem;
}

void yunity::GlobalVariables::SetValue(const std::string& groupName, const std::string& key, Vector3 value) {
	// グループの参照を取得 
	Group& group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem = value;
	// 設定した項目をstd::mapに追加
	group[key] = newItem;
}

void yunity::GlobalVariables::SetValue(const std::string& groupName, const std::string& key, Vector2 value)
{
	// グループの参照を取得 
	Group& group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem = value;
	// 設定した項目をstd::mapに追加
	group[key] = newItem;
}

void yunity::GlobalVariables::SetValue(const std::string& groupName, const std::string& key, Vector4 value)
{
	// グループの参照を取得 
	Group& group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem = value;
	// 設定した項目をstd::mapに追加
	group[key] = newItem;
}

void yunity::GlobalVariables::AddItem(const std::string& groupName, const std::string& key, int value) {
	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);
	// 項目が未登録なら
	if (itGroup != datas_.end()) {
		SetValue(groupName, key, value);
	}
}

void yunity::GlobalVariables::AddItem(const std::string& groupName, const std::string& key, float value) {
	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);
	if (itGroup != datas_.end()) {
		SetValue(groupName, key, value);
	}
}

void yunity::GlobalVariables::AddItem(const std::string& groupName, const std::string& key, Vector3 value) {
	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);
	if (itGroup != datas_.end()) {
		SetValue(groupName, key, value);
	}

}

void yunity::GlobalVariables::AddItem(const std::string& groupName, const std::string& key, Vector2 value)
{
	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);
	if (itGroup != datas_.end()) {
		SetValue(groupName, key, value);
	}
}

void yunity::GlobalVariables::AddItem(const std::string& groupName, const std::string& key, Vector4 value)
{
	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);
	if (itGroup != datas_.end()) {
		SetValue(groupName, key, value);
	}
}

int yunity::GlobalVariables::GetIntValue(const std::string& groupName, const std::string& key) {
	assert(datas_.find(groupName) != datas_.end());

	// グループの参照を取得
	Group& group = datas_[groupName];

	assert(group.contains(key));
	return std::get<int>(group[key]);
}

float yunity::GlobalVariables::GetFloatValue(const std::string& groupName, const std::string& key) {
	assert(datas_.find(groupName) != datas_.end());

	// グループの参照を取得
	Group& group = datas_[groupName];

	assert(group.contains(key));

	return std::get<float>(group[key]);
}

Vector3
yunity::GlobalVariables::GetVector3Value(const std::string& groupName, const std::string& key) {
	assert(datas_.find(groupName) != datas_.end());

	// グループの参照を取得
	Group& group = datas_[groupName];
	// assert(itGroup.key() == key);
	assert(group.contains(key));

	return std::get<Vector3>(group[key]);
}

Vector2 yunity::GlobalVariables::GetVector2Value(const std::string& groupName, const std::string& key)
{
	assert(datas_.find(groupName) != datas_.end());

	// グループの参照を取得
	Group& group = datas_[groupName];
	// assert(itGroup.key() == key);
	assert(group.contains(key));

	return std::get<Vector2>(group[key]);
}

Vector4 yunity::GlobalVariables::GetVector4Value(const std::string& groupName, const std::string& key)
{
	assert(datas_.find(groupName) != datas_.end());

	// グループの参照を取得
	Group& group = datas_[groupName];
	// assert(itGroup.key() == key);
	assert(group.contains(key));

	return std::get<Vector4>(group[key]);

}
