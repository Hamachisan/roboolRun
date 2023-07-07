#pragma once

#include"Graphics/Model.h"
#include "Character.h"
#include "Collision.h"
#include "Graphics/Shader.h"
//
class Goal : public Character
{
public:
	Goal();
	~Goal() override;	//継承先の仮想関数をオーバーライドするときはoverrideキーワードをつけること

	//更新処理
	void Update(float elapsedTime);

	//描画処理
	void Render(ID3D11DeviceContext* dc, Shader* shader);

protected:

private:
	Model* model = nullptr;
};