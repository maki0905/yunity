#include "TimedCall.h"

TimedCall::TimedCall(std::function<void(void)> call, uint32_t time) : callback_(call), time_(time) {};

void TimedCall::Updata() {
	if (flag_ == true) {
		return;
	}
	time_--;
	if (time_ <= 0) {
		flag_ = true;
		// コールバック関数呼び出し
		callback_();
	}

}
