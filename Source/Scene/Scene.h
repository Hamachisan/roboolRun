#pragma once
#include "Graphics/Graphics.h"

//�V�[��
class Scene
{
public:
	Scene() {}
	virtual ~Scene() {}

	//������
	virtual void Initialize() = 0;				//�܂Ƃ�
	virtual void SpriteInitialize() = 0;		//�X�v���C�g�ǂݍ���
	virtual void ParameterInitialize() = 0;		//�p�����[�^�[�̐ݒ�

	//�I����
	virtual void Finalize() = 0;

	//�X�V����
	virtual void Update(float elapsedtime) = 0;

	//�`�揈��
	virtual void Render() = 0;
	virtual void UiRender(ID3D11DeviceContext* dc) = 0;

	//�����������Ă��邩
	bool IsReady() const { return ready_; }

	//���������ݒ�
	void setReady() { ready_ = true; }

	//���̃Z�b�g
	virtual void SettingLight(ID3D11DeviceContext* dc, RenderContext& rc) {};

	//�X�v���C�g�̃p�����[�^���X�V����֐�
	virtual void SpriteParameterUpdate(float elapsedTime) = 0;
	//UI��ύX����֐�
	virtual void ChangeUi() = 0;

private:
	bool  ready_ = false;

public:
	float gameTimer_ = 0.0f;
};

