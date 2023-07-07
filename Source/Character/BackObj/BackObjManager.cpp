#include"BackObjManager.h"

//更新処理
void BackObjManager::Update(float elapsedTime)
{
	for (BackObj* backObj : backObjs_)
	{
		backObj->Update(elapsedTime);

	}

	//破棄処理(ゲーム中に消すことが無いから必要無いかもしれない)
	for (BackObj* backObj : removes_)
	{
		std::vector<BackObj*>::iterator it = std::find(backObjs_.begin(), backObjs_.end(), backObj);
	
		if (it != backObjs_.end())
		{
			backObjs_.erase(it);
		}
	
		//破棄
		delete backObj;
	}
	//破棄リストのクリア
	removes_.clear();
}

//描画処理
void BackObjManager::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	for (BackObj* backObj : backObjs_)
	{
		backObj->Render(dc, shader);
	}
}

//背景オブジェクト登録
void BackObjManager::Register(BackObj* backObj)
{
	backObjs_.emplace_back(backObj);
}

//背景オブジェクト削除
void BackObjManager::Remove(BackObj* backObj)
{
	//破棄リストに追加
	removes_.emplace_back(backObj);
}

//背景オブジェクト全削除
void BackObjManager::Clear()
{
	for (BackObj* backObj : backObjs_)
	{
		if (backObj != nullptr)
		{
			//Remove(backObj);
			delete backObj;
			backObj = nullptr;
		}
	}
}

//背景オブジェクトのデバッグプリミティブ描画
//void BackObjManager::DrawDebugPrimitive()
//{
//	for (BackObj* backObj : backObjs)
//	{
//		backObj->DrawDebugPrimitive();
//	}
//
//}