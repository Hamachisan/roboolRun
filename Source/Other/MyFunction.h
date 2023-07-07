#pragma once
#include <DirectXMath.h>

//safe_delete
template <typename T>
inline void SafeDelete(T*& p)
{
    if (p != nullptr)
    {
        delete (p);
        (p) = nullptr;
    }
}

//線形補完
//float Lerp(float a, float b, float t)
//{
//	return a * (1.0f - t) + (b * t);
//}

//ポジション
struct Pos2D
{
	float x, y;
};

//矩形のボックス
struct Hitbox2D
{
	float top, left, right, bottom;
	DirectX::XMFLOAT2 size;	
};

//矩形判定のサイズ設定
void OffsetBox(float top, float left, float& bottom, float& right, float sizeX, float sizeY);
void OffsetBox(Hitbox2D& box, float sizeX, float sizeY);

//矩形判定
bool Hitcheck(float top1, float left1, float bottom1, float right1,
	float top2, float left2, float bottom2, float right2);
bool HitCheck(Hitbox2D box1, Hitbox2D box2);

//円形判定
bool CricleCheck(Pos2D Pos1, Pos2D Pos2, float range1, float range2);

//上限有りの左右,上下移動 (スクリーンの左上が0,0の時は上昇させるにはMoveLeftOrDownを使う。下降にはMoveRightOrUp)
//右もしくは上へ
float MoveRightOrUp(float pos, float max, float speed);
//左もしくは下へ
float MoveLeftOrDown(float pos, float max, float speed);