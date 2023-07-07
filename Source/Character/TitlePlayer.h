#pragma once
#include "Character/Character.h"
#include "Graphics/Graphics.h"
#include "Graphics/Model.h"

//�v���C���[�N���X(�L�����N�^�[�̔h���N���X)
class TitlePlayer : public Character
{
public:
	TitlePlayer();
	~TitlePlayer() override;

	//�X�V����
	void Update(float elapsedTime);

	//�`�揈��
	void Render(ID3D11DeviceContext* dc, Shader* shader);


private:
	//�e��p�����[�^
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
