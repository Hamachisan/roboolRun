#include"Enemy.h"
#include"Graphics/Graphics.h"
#include"EnemyManager.h"

//�f�o�b�O�v���~�e�B�u�`��
void Enemy::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	//�Փ˔���p�̃f�o�b�O����`��
	//debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

	//�Փ˔���p�̃f�o�b�O�~����`��
	debugRenderer->DrawCylinder(position_, radius_, height_, DirectX::XMFLOAT4(0, 0, 0, 1));
}

//�j��
void Enemy::Destroy()
{
	EnemyManager::Instance().Remove(this);
}