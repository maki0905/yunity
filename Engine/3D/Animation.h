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

struct AnimationCommon {
	enum AnimationMode{kStopped, kPlaying, kLooping};
	AnimationMode state;
	float time;
	AnimationCommon() : state(kStopped), time(0.0f){}
};

struct AnimationData {
	Animation animation;
	AnimationCommon animationCommon;

};

/// <summary>
/// Animation解析
/// </summary>
/// <param name="format">フォーマット</param>
/// <param name="folderName">フォルダー名</param>
/// <param name="fileName">ファイル名</param>
/// <returns>アニメーション</returns>
Animation LoadAnimationFile(Format format, const std::string& folderName, const std::string& fileName = "");

/// <summary>
/// 値を計算
/// </summary>
/// <param name="keyframes">キーフレーム</param>
/// <param name="time">タイム</param>
/// <returns>三次元ベクトル</returns>
Vector3 CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time);

/// <summary>
/// クオータニオン計算
/// </summary>
/// <param name="keyframes">キーフレーム</param>
/// <param name="time">タイム</param>
/// <returns>クオータニオン</returns>
Quaternion CalculateQuaternion(const std::vector<KeyframeQuaternion>& keyframes, float time);
