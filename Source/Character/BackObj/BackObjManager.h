#pragma once

#include<vector>
#include"BackObj.h"

//背景オブジェクトーマネージャー
class BackObjManager
{
private:
	BackObjManager() {}
	~BackObjManager() {}

public:
	//唯一のインスタンス取得
	static BackObjManager& Instance()
	{
		static BackObjManager instance;
		return instance;
	}

	//更新処理
	void Update(float elapsedTime);

	//描画処理
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	//背景オブジェクト登録
	void Register(BackObj* backObj);

	//背景オブジェクト数取得
	int GetBackObjCount() const { return static_cast<int>(backObjs_.size()); }

	//背景オブジェクト取得
	BackObj* GetBackObj(int index) { return backObjs_.at(index); }

	//背景オブジェクト削除
	void Remove(BackObj* backObj);

	//背景オブジェクト全削除
	void Clear();

	//デバッグプリミティブ描画
	//void DrawDebugPrimitive();

private:
	std::vector<BackObj*> backObjs_;

	std::vector<BackObj*> removes_;
};
