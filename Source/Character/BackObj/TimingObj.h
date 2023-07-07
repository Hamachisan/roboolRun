#pragma once

#include "BackObj.h"
#include "Graphics/Model.h"

//背景オブジェクト岩
class TimingObj : public BackObj
{
public:
	TimingObj();
	~TimingObj() override;

	//更新処理
	void Update(float elapsedTime) override;

	//描画処理
	void Render(ID3D11DeviceContext* dc, Shader* shader) override;

protected:
	//死亡した時に呼ばれる
	void OnDead() override;

private:
	Model* model_ = nullptr;

};