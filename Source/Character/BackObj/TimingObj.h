#pragma once

#include "BackObj.h"
#include "Graphics/Model.h"

//�w�i�I�u�W�F�N�g��
class TimingObj : public BackObj
{
public:
	TimingObj();
	~TimingObj() override;

	//�X�V����
	void Update(float elapsedTime) override;

	//�`�揈��
	void Render(ID3D11DeviceContext* dc, Shader* shader) override;

protected:
	//���S�������ɌĂ΂��
	void OnDead() override;

private:
	Model* model_ = nullptr;

};