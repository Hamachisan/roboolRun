#pragma once
#include "Character/Character.h"
#include "Graphics/Graphics.h"
#include "Graphics/Model.h"

//プレイヤークラス(キャラクターの派生クラス)
class TitlePlayer : public Character
{
public:
	TitlePlayer();
	~TitlePlayer() override;

	//更新処理
	void Update(float elapsedTime);

	//描画処理
	void Render(ID3D11DeviceContext* dc, Shader* shader);


private:
	//各種パラメータ
	Model* model_ = nullptr;
	int animIndex_ = 0;
	enum Animations
	{
		Anim_Open,
		Anim_Rotation,
		Anim_Idel,
		Anim_RotationEnd,
		Anim_Walk,
		Anim_RotationContine,
		Anim_Close
	};
	int loopCount_ = 0;
	float rotationTime_ = 3.0f;
};
