#pragma once

#include<DirectXMath.h>

//�J�����R���g���[���[
class CameraController
{
public:
	CameraController(){}
	~CameraController(){}

	//������
	//void Initialize();

	//�X�V����
	void Update(float elapsedTime);

	//�J�����V�F�C�N
	void ShakeCamera();

	//�J�����V�F�C�N������
	void CameraShakeInit();
	//�p�u���b�N�ϐ�
public:
	//�J�����ƃv���C���[�̋���
	float			   range_ = 6.2f;

	//�v���C�x�[�g�ϐ�
private:
	DirectX::XMFLOAT3	target_ = { 0, 0, 0 };
	DirectX::XMFLOAT3	angle_ = { 0, 0, 0 };
	DirectX::XMFLOAT3	eye_ = { 0,0,0 };

	float ax_ = 0;
	float ay_ = 0;

	//�J�����V�F�C�N�p�p�����[�^
	DirectX::XMFLOAT3 oldTarget_ = { 0, 0, 0 };
	DirectX::XMFLOAT3 oldEye_ = { 0, 0, 0 };

	float			   rollSpeed_ = DirectX::XMConvertToRadians(90);

	float				maxAngleX_ = DirectX::XMConvertToRadians(45);
	float				minAngleX_ = DirectX::XMConvertToRadians(-45);

	//�J�����V�F�C�N�̕ϐ�
	bool isCameraShake_ = false;
	const float  defalutShakeTimer_ = 1.00f;
	float shakeTimer_ = defalutShakeTimer_;
	DirectX::XMFLOAT2 shakePowr_ = {};
	float radian_ = 0;



	//�v���C�x�[�g�֐�
private:

	//�Q�b�^�[�A�Z�b�^�[
public:
	//�^�[�Q�b�g�ʒu�ݒ�
	void SetTarget(const DirectX::XMFLOAT3& target) { this->target_ = target; }
	//�J�����V�F�C�N�̃t���O�ݒ�
	void SetCameraShake(bool shakeFlag) { isCameraShake_ = shakeFlag; }
	bool GetCameraShake() { return isCameraShake_; }

	//�p�x�Z�b�g
	void setCameraYAngle(float angle) { ax_ = DirectX::XMConvertToRadians(angle); }
	void setCameraXAngle(float angle) { ay_ = DirectX::XMConvertToRadians(angle); }
	//�����Z�b�g
	void setCameraRange(float distance) { range_ = distance; }
};