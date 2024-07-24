#pragma once

class CommonData {
public:
	static CommonData* GetInstance();

	void Initialize();

	bool isGoal_;
	int stageNum_;



private:
	CommonData() = default;
	~CommonData() = default;
	CommonData(const CommonData&) = default;
	const CommonData& operator=(const CommonData&) = delete;
};