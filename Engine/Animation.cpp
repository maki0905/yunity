#include "Animation.h"

#include <cassert>

Animation LoadAnimationFile(Format format, const std::string& folderName, const std::string& fileName)
{
    Animation animation;
    Assimp::Importer importer;
    std::string filePath = "Resources/Models/" + folderName + "/" + folderName;
    if (fileName.size()) {
        filePath = "Resources/Models/" + folderName + "/" + fileName;
    }
    filePath = AddToFormat(filePath, format);

    const aiScene* scene = importer.ReadFile(filePath.c_str(), 0);
    assert(scene->mNumAnimations != 0); // アニメーションがない
    aiAnimation* animationAssimp = scene->mAnimations[0]; // 最初のアニメーションだけ採用。もちろん複数対応するにこしたことはない
    animation.duration = float(animationAssimp->mDuration / animationAssimp->mTicksPerSecond); // 時間の単位を秒に変換
    // assimpでは個々のNodeのAnimationをchannelと呼んでいるのでchannelを回してNodeAnimationの情報を取ってくる
    for (uint32_t channelIndex = 0; channelIndex < animationAssimp->mNumChannels; ++channelIndex) {
        aiNodeAnim* nodeAnimationAssimp = animationAssimp->mChannels[channelIndex];
        NodeAnimation& nodeAnimation = animation.nodeAnimations[nodeAnimationAssimp->mNodeName.C_Str()];

        // Translate
        for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumPositionKeys; ++keyIndex) {
            aiVectorKey& keyAssimp = nodeAnimationAssimp->mPositionKeys[keyIndex];
            KeyframeVector3 keyframe;
            keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond); // ここも秒に変換
            keyframe.value = { -keyAssimp.mValue.x, keyAssimp.mValue.y, keyAssimp.mValue.z }; // 右手->左手
            nodeAnimation.translate.push_back(keyframe);
        }

        // Rotate
        for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumRotationKeys; ++keyIndex) {
            aiQuatKey& keyAssimp = nodeAnimationAssimp->mRotationKeys[keyIndex];
            KeyframeQuaternion keyframe;
            keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond); // ここも秒に変換
            keyframe.value = { keyAssimp.mValue.x, -keyAssimp.mValue.y, -keyAssimp.mValue.z, keyAssimp.mValue.w }; // 右手->左手
            nodeAnimation.rotate.push_back(keyframe);
        }

        // Scale
        for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumScalingKeys; ++keyIndex) {
            aiVectorKey& keyAssimp = nodeAnimationAssimp->mScalingKeys[keyIndex];
            KeyframeVector3 keyframe;
            keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond); // ここも秒に変換
            keyframe.value = { keyAssimp.mValue.x, keyAssimp.mValue.y, keyAssimp.mValue.z }; // 右手->左手
            nodeAnimation.translate.push_back(keyframe);
        }

    }

    return animation;
}

Vector3 CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time)
{
    assert(!keyframes.empty()); // キーがないものは返す値がわからないのでダメ
    if (keyframes.size() == 1 || time <= keyframes[0].time) { // キーが1つか、時刻がキーフレーム前なら最初の値とする
        return keyframes[0].value;
    }

    for (size_t index = 0; index < keyframes.size() - 1; ++index) {
        size_t nextIndex = index + 1;
        // 
        if (keyframes[index].time <= time && time <= keyframes[nextIndex].time) {
            float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
            return Lerp(keyframes[index].value, keyframes[nextIndex].value, t);
        }
    }

    // ここまできた場合は一番後の時刻よりも後ろなので最後の値を返すことにする
    return (*keyframes.rbegin()).value;
}

Quaternion CalculateQuaternion(const std::vector<KeyframeQuaternion>& keyframes, float time)
{
    assert(!keyframes.empty()); // キーがないものは返す値がわからないのでダメ
    if (keyframes.size() == 1 || time <= keyframes[0].time) { // キーが1つか、時刻がキーフレーム前なら最初の値とする
        return keyframes[0].value;
    }

    for (size_t index = 0; index < keyframes.size() - 1; ++index) {
        size_t nextIndex = index + 1;
        // 
        if (keyframes[index].time <= time && time <= keyframes[nextIndex].time) {
            float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
            return Slerp(keyframes[index].value, keyframes[nextIndex].value, t);
        }
    }

    // ここまできた場合は一番後の時刻よりも後ろなので最後の値を返すことにする
    return (*keyframes.rbegin()).value;
}
