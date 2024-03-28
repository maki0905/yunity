#pragma once

#include <sstream>
#include <unordered_map>
#include <wrl.h>
#include <cassert>
#include <fstream>
#include <string>

#include "Vector3.h"

// CSVはGameSceneへ移植しました //

class CSV {
public:
	struct Data {
		Vector3 position;
		uint32_t type;
	};

public:
	static CSV* GetInstance();

	void Initialize();

	/// <summary>
	/// データの読み込み
	/// </summary>
	std::stringstream* LoadCSV(std::string fileName);

	/// <summary>
	/// データの更新
	/// </summary>
	std::vector<Data> UpdateDataCommands();

	/// <summary>
	/// データの書き込み
	/// </summary>
	void WritingData(std::string fileName, std::vector<Data> datas);
private:
	CSV() = default;
	~CSV() = default;
	CSV(const CSV&) = delete;
	CSV& operator=(const CSV&) = delete;

private:
	std::unordered_map<std::string, std::stringstream> csvData_;

	// 発生コマンド
	std::stringstream dateCommands_;
};