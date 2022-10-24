#pragma once
#include <cstdint>

///味方陣営
const uint32_t kCollisionAttributeMine = 0b1;

//その他
const uint32_t kCollisionAttributeThier = 0b1 << 1;

//NOダメージ
//const uint32_t kCollisionAttributeNoDamage = 0b1 << 2;

////プレイヤー
//const uint32_t kCollisionAttributePlayer = 0b1;
////アイテム
//const uint32_t kCollisionAttributeItem = 0b1 << 3;
//
/////ダメージ
////地面
//const uint32_t kCollisionAttributeGround = 0b1 << 2;
////壁
//const uint32_t kCollisionAttributeWall = 0b1 << 4;