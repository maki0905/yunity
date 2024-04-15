#pragma once

#include <functional>
///< summary>
/// 時限発動
///  </summary>

class TimedCall {
public:
	// コンストラクタ
	TimedCall(std::function<void()> call, uint32_t time);
	// 更新
	void Updata();
	// 完了ならtrueを返す
	bool IsFinished() { return flag_; }

private:
	// コールバック
	std::function<void(void)> callback_;
	// 残り時間
	uint32_t time_;
	// 完了フラグ
	bool flag_ = false;
};
