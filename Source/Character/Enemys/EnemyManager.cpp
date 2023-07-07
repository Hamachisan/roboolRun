#include"EnemyManager.h"
#include"Collision.h"

//更新処理
void EnemyManager::Update(float elapsedTime)
{
	for (Enemy* enemy : enemies_)
	{
		enemy->Update(elapsedTime);

	}

	//破棄処理
	for (Enemy* enemy : removes_)
	{
		std::vector<Enemy*>::iterator it = std::find(enemies_.begin(), enemies_.end(), enemy);

		if (it != enemies_.end())
		{
			enemies_.erase(it);
		}

		//破棄
		delete enemy;
	}
	//破棄リストのクリア
	removes_.clear();

	//敵同士の衝突判定
	CollisionEnemyVsEnemy();
}

//描画処理
void EnemyManager::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	for (Enemy* enemy : enemies_)
	{
		enemy->Render(dc, shader);
	}
}

//エネミー登録
void EnemyManager::Register(Enemy* enemy)
{
	enemies_.emplace_back(enemy);
}

//エネミー削除
void EnemyManager::Remove(Enemy* enemy)
{
	//破棄リストに追加
	removes_.emplace_back(enemy);
}

//エネミー全削除
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

//エネミーのデバッグプリミティブ描画
void EnemyManager::DrawDebugPrimitive()
{
	for (Enemy* enemy : enemies_)
	{
		enemy->DrawDebugPrimitive();
	}

}

//エネミーとエネミーとの衝突判定
void EnemyManager::CollisionEnemyVsEnemy()
{
	int enemyCount = static_cast<int>(enemies_.size());
	for (int a = 0; a < enemyCount; a++)
	{
		//基準となる方の敵
		Enemy* enemyA = enemies_.at(a);

		for (int b = a + 1; b < enemyCount; b++)
		{
			if (a == b) continue; //同キャラは判定しなくて良い
			//位置修正される側の敵
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