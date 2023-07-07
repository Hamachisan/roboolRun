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

//���`�⊮
//float Lerp(float a, float b, float t)
//{
//	return a * (1.0f - t) + (b * t);
//}

//�|�W�V����
struct Pos2D
{
	float x, y;
};

//��`�̃{�b�N�X
struct Hitbox2D
{
	float top, left, right, bottom;
	DirectX::XMFLOAT2 size;	
};

//��`����̃T�C�Y�ݒ�
void OffsetBox(float top, float left, float& bottom, float& right, float sizeX, float sizeY);
void OffsetBox(Hitbox2D& box, float sizeX, float sizeY);

//��`����
bool Hitcheck(float top1, float left1, float bottom1, float right1,
	float top2, float left2, float bottom2, float right2);
bool HitCheck(Hitbox2D box1, Hitbox2D box2);

//�~�`����
bool CricleCheck(Pos2D Pos1, Pos2D Pos2, float range1, float range2);

//����L��̍��E,�㉺�ړ� (�X�N���[���̍��オ0,0�̎��͏㏸������ɂ�MoveLeftOrDown���g���B���~�ɂ�MoveRightOrUp)
//�E�������͏��
float MoveRightOrUp(float pos, float max, float speed);
//���������͉���
float MoveLeftOrDown(float pos, float max, float speed);