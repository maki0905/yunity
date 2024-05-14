#pragma once
#include <vector>
#include <map>
#include <string>


#include "Keyframe.h"
#include "Format.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct NodeAnimation {
	std::vector<KeyframeVector3> translate;
	std::vector<KeyframeQuaternion> rotate;
	std::vector<KeyframeVector3> scale;
};

template<typename tValue>
struct AnimationCurve {
	std::vector<Keyframe<tValue>> keyframes;
};

//struct NodeAnimation {
//	AnimationCurve<Vector3> translate;
//	AnimationCurve<Quaternion> rotate;
//	AnimationCurve<Vector3> scale;
//};

struct Animation {
	float duration; // アニメーション全体の尺(単位は秒)
	// NodeAnimationの集合。Node名でひけるようにしておく
	std::map<std::string, NodeAnimation> nodeAnimations;
};

// Animation解析
Animation LoadAnimationFile(Format format, const std::string& folderName, const std::string& fileName = "");

Vector3 CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time);
Quaternion CalculateQuaternion(const std::vector<KeyframeQuaternion>& keyframes, float time);
