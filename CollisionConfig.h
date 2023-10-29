#pragma once
#include <cstdint>

// プレイヤー陣営
const uint32_t kCollisionAttributePlayer = 0b1;
// 敵陣営
const uint32_t kCollisionAttributeEnemy = 0b1 << 1;
// 動かない床
const uint32_t kCollisionAttributeFloor = 0b1 << 2;
// 動く床
const uint32_t kCollisionAttributeMovingFloor = 0b1 << 3;
// ゴール
const uint32_t kCollisionAttributeGoal = 0b1 << 4;