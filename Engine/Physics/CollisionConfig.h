#pragma once
#include <cstdint>

// プレイヤー陣営
const uint32_t kCollisionAttributePlayer = 0b1;
// 床
const uint32_t kCollisionAttributeFloor = 0b1 << 1;
// 動かせるもの
const uint32_t kCollisionAttributeTrampoline = 0b1 << 2;
// ゴール
const uint32_t kCollisionAttributeGoal = 0b1 << 3;
// 動く床
const uint32_t kCollisionAttributeMoveFloor = 0b1 << 4;
// コイン
const uint32_t kCollisionAttributeCoin = 0b1 << 5;
// セレクト
const uint32_t kCollisionAttributeSelect = 0b1 << 6;
// トゲ
const uint32_t kCollisionAttributeSpike = 0b1 << 7;
// TV
const uint32_t kCollisionAttributeTV1= 0b1 << 8;
const uint32_t kCollisionAttributeTV2 = 0b1 << 9;
const uint32_t kCollisionAttributeTV3 = 0b1 << 10;
// 力場
const uint32_t kCollisionAttributeForceField = 0b1 << 11;
// ポール
const uint32_t kCollisionAttributePillar = 0b1 << 12;

