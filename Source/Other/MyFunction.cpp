#include"MyFunction.h"

//��`����̃T�C�Y�ݒ�
void OffsetBox(float top, float left, float& bottom, float& right, float sizeX, float sizeY)
{
	top = top;
	left = left;
	bottom = top + sizeY;
	right = left + sizeX;
}
//��`����̃T�C�Y�ݒ�(�I�[�o�[���[�h)
void OffsetBox(Hitbox2D& box, float sizeX, float sizeY)
{
	box.size.x = sizeX;
	box.size.y = sizeY;

	box.bottom = box.top + box.size.y;
	box.right = box.left + box.size.x;
}

//��`����
bool Hitcheck(float top1, float left1, float bottom1, float right1,
	float top2, float left2, float bottom2, float right2)
{
	//�������Ă��鎞
	if ((right1 > left2) &&
		(left1 < right2) &&
		(top1 < bottom2) &&
		(bottom1 > top2))
	{
		return true;
	}
	//����ȊO
	else
		return false;
}
//��`����(�I�[�o�[���[�h)
bool HitCheck(Hitbox2D box1, Hitbox2D box2)
{
	//����������
	if ((box1.right > box2.left) &&
		(box1.left < box2.right) &&
		(box1.top < box2.bottom) &&
		(box1.bottom > box2.top))
	{
		return true;
	}
	//�������Ă��Ȃ�
	return false;
}

//�~�`����
bool CricleCheck(Pos2D Pos1, Pos2D Pos2, float range1, float range2)
{
	Pos2D dist;	// = Pos1 - Pos2;
	dist.x = Pos1.x - Pos2.x;
	dist.y = Pos1.y - Pos2.y;
	float distance = sqrtf(dist.x * dist.x + dist.y * dist.y);
	if (distance < range1 + range2)
	{
		return true;
	}

	return false;
}

//����L��̍��E,�㉺�ړ�
//�E�������͏��
float MoveRightOrUp(float pos, float max, float speed)
{
	pos += speed;
	if (pos > max)
		pos = max;
	return pos;
}
//���������͉���
float MoveLeftOrDown(float pos, float max, float speed)
{
	pos -= speed;
	if (pos < max)
		pos = max;
	return pos;
}

