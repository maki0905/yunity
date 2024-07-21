﻿#pragma once
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
const uint32_t kCollisionAttributeMoveFloor = 0b1 << 4;
const uint32_t kCollisionAttributeCoin = 0b1 << 5;
