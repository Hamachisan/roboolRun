#pragma once
#include "Character/Character.h"
#include "Graphics/Graphics.h"

//背景オブジェクトクラス(周りに置くだけで描画だけ行う)
class BackObj : public Character
{
public:
	BackObj() {}
	~BackObj() override {}

	//更新処理
	virtual void Update(float elapsedTime) = 0;

	//描画処理
	virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

	//破棄
	void Destroy();

	//デバッグプリミティブ描画
	//virtual void DrawDebugPrimitive();

};
