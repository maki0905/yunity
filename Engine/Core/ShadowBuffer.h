#pragma once

#include "DepthBuffer.h"

namespace yunity {
	class ShadowBuffer {
	public:
		static ShadowBuffer* GetInstance();
		void Initalize();

	private:
		ShadowBuffer() = default;
		~ShadowBuffer() = default;
		ShadowBuffer(const ShadowBuffer&) = delete;
		const ShadowBuffer& operator=(const ShadowBuffer) = delete;
	private:
		std::unique_ptr<DepthBuffer> shadowBuffer_;
	};
}