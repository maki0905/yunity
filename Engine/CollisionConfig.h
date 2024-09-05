#pragma once
#include <cstdint>

// プレイヤー陣営
const uint32_t kCollisionAttributePlayer = 0b1;
// 床
const uint32_t kCollisionAttributeFloor = 0b1 << 1;
// トランポリン
const uint32_t kCollisionAttributeTrampoline = 0b1 << 2;
// ゴール
const uint32_t kCollisionAttributeGoal = 0b1 << 3;
// 動く床
const uint32_t kCollisionAttributeMoveFloorLeft = 0b1 << 4;
const uint32_t kCollisionAttributeMoveFloorRight = 0b1 << 11;
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
const uint32_t kCollisionAttributeVentilator = 0b1 << 11;

