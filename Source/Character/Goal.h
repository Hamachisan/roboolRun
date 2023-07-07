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
	~Goal() override;	//�p����̉��z�֐����I�[�o�[���C�h����Ƃ���override�L�[���[�h�����邱��

	//�X�V����
	void Update(float elapsedTime);

	//�`�揈��
	void Render(ID3D11DeviceContext* dc, Shader* shader);

protected:

private:
	Model* model = nullptr;
};