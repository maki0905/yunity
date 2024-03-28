#include "CSV.h"

CSV* CSV::GetInstance()
{
	static CSV instance;
	return &instance;
}

void CSV::Initialize()
{
	csvData_.clear();
}

std::stringstream* CSV::LoadCSV(std::string fileName) {
	
	auto itr = csvData_.find(fileName);
	if (itr == csvData_.end()) {
		fileName = "Resources/CSV/" + fileName + ".csv";
		// ファイルを開く
		std::ifstream file;
		file.open(fileName);
		assert(file.is_open());

		csvData_[fileName] << file.rdbuf();

		// ファイルを閉じる
		file.close();
	}

	
	
	return &csvData_[fileName];
}

std::vector<CSV::Data> CSV::UpdateDataCommands() {
	// 1行分の文字列を入れる変数
	std::string line;

	std::vector<CSV::Data> result;
	Data pop;

	// コマンド実行ループ
	while (getline(dateCommands_, line)) {
		// 1行分の文字列をストリームに変更して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		// ,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// Positionコマンド
		if (word.find("Position") == 0) {
			// x座標
			getline(line_stream, word, ',');
			pop.position.x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			pop.position.y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			pop.position.z = (float)std::atof(word.c_str());

		}

		pop.type = 999;

		// Typeコマンド
		if (word.find("Type") == 0) {
			// type(0~2)
			getline(line_stream, word, ',');
			pop.type = (uint32_t)std::atof(word.c_str());
		}

		// ここに発生の処理
		if (pop.type != 999) {
			result.push_back(pop);
		}
	}
	return result;
}

void CSV::WritingData(std::string fileName, std::vector<Data> datas)
{
	std::ofstream outputFile("Resources/CSV/" + fileName + ".csv");
	outputFile << "// width 1000.0f(10/18)";
	outputFile << '\n';
	outputFile << "// height 50.0f(10/18)";
	outputFile << '\n';
	outputFile << "// コマンド,x,y,z";
	outputFile << '\n';
	outputFile << "// コマンド,0:タコ,1:とげ,2:エサ";
	outputFile << '\n';
	for (auto& data : datas) {
		outputFile << "Position,";
		outputFile << data.position.x;
		outputFile << ',';
		outputFile << data.position.y;
		outputFile << ',';
		outputFile << data.position.z;
		outputFile << '\n';
		outputFile << "Type,";
		outputFile << data.type;
		outputFile << '\n';

	}
	outputFile.close();
}