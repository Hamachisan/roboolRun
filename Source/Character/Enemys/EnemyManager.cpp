#include"EnemyManager.h"
#include"Collision.h"

//�X�V����
void EnemyManager::Update(float elapsedTime)
{
	for (Enemy* enemy : enemies_)
	{
		enemy->Update(elapsedTime);

	}

	//�j������
	for (Enemy* enemy : removes_)
	{
		std::vector<Enemy*>::iterator it = std::find(enemies_.begin(), enemies_.end(), enemy);

		if (it != enemies_.end())
		{
			enemies_.erase(it);
		}

		//�j��
		delete enemy;
	}
	//�j�����X�g�̃N���A
	removes_.clear();

	//�G���m�̏Փ˔���
	CollisionEnemyVsEnemy();
}

//�`�揈��
void EnemyManager::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	for (Enemy* enemy : enemies_)
	{
		enemy->Render(dc, shader);
	}
}

//�G�l�~�[�o�^
void EnemyManager::Register(Enemy* enemy)
{
	enemies_.emplace_back(enemy);
}

//�G�l�~�[�폜
void EnemyManager::Remove(Enemy* enemy)
{
	//�j�����X�g�ɒǉ�
	removes_.emplace_back(enemy);
}

//�G�l�~�[�S�폜
void EnemyManager::Clear()
{
	for (Enemy* enemy : enemies_)
	{
		if (enemy != nullptr)
		{
			delete enemy;
			enemy = nullptr;
		 }
	}
}

//�G�l�~�[�̃f�o�b�O�v���~�e�B�u�`��
void EnemyManager::DrawDebugPrimitive()
{
	for (Enemy* enemy : enemies_)
	{
		enemy->DrawDebugPrimitive();
	}

}

//�G�l�~�[�ƃG�l�~�[�Ƃ̏Փ˔���
void EnemyManager::CollisionEnemyVsEnemy()
{
	int enemyCount = static_cast<int>(enemies_.size());
	for (int a = 0; a < enemyCount; a++)
	{
		//��ƂȂ���̓G
		Enemy* enemyA = enemies_.at(a);

		for (int b = a + 1; b < enemyCount; b++)
		{
			if (a == b) continue; //���L�����͔��肵�Ȃ��ėǂ�
			//�ʒu�C������鑤�̓G
			Enemy* enemyB = enemies_.at(b);
			if (!enemyA->isAlive_) continue;
			DirectX::XMFLOAT3 outPosition;
			if (Collision::IntersectCylinderVsCylinder(
				enemyA->GetPosition(), enemyA->GetRadius(),	enemyA->GetHeight(),
				enemyB->GetPosition(), enemyB->GetRadius(),  enemyB->GetHeight(),
				outPosition)
				)
			{
				enemyB->SetPosition(outPosition);
			}

		}
	}
}