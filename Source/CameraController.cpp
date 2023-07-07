#include "CameraController.h"
#include "Camera.h"
#include "Input/Input.h"
#include <time.h>

//�X�V����
void CameraController::Update(float elapsedTime)
{
	//srand((unsigned int)time(NULL));

	GamePad& gamepad = Input::Instance().GetGamePad();
	
	//�J�����̉�]���x
	float speed = rollSpeed_ * elapsedTime;

	//�X�e�B�b�N�̓��͒l�ɍ��킹��x���Ƃ�������]
	//angle.x += ay * speed;
	angle_.x = 0.209439546f;
	//angle.x = ay;
	//angle.y += ax * speed;
	angle_.y = ax_;

	//X���̃J������]�𐧌�
	if (angle_.x > maxAngleX_)
		angle_.x = maxAngleX_ ;
	if (angle_.x < minAngleX_)
		angle_.x = minAngleX_;

	//y���̉�]�l��-3.14~3.14�Ɏ��܂�悤�ɂ���
	if (angle_.y < -DirectX::XM_PI)
	{
		angle_.y += DirectX::XM_2PI;
	}
	if (angle_.y > DirectX::XM_PI)
	{
		angle_.y -= DirectX::XM_2PI;
	}

	//�J������]�l����]�s��ɕϊ�
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle_.x, angle_.y, angle_.z);

	//��]�s�񂩂�O�����x�N�g�������o��
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);

	//���ӓ_������x�N�g�������ɂ���苗�����ꂽ�J�������_�����߂�
	eye_.x = target_.x - front.x * range_;
	eye_.y = target_.y - front.y * range_;
	eye_.z = target_.z - front.z * range_;

	//�ۑ�
	oldEye_ = eye_;
	oldTarget_ = target_;

	//�J�����V�F�C�N
	if (isCameraShake_)
	{
		ShakeCamera();	
		//range = 6.5f;
	}
	else
	{
		CameraShakeInit();
		//range = 5.5f;
		ax_ = 0;
	}

	//�J�����̎��_�ƒ��ӓ_��ݒ�
	Camera::Instance().SetLookAt(eye_, target_, DirectX::XMFLOAT3(0, 1, 0));
	//�߂�
	//eye = oldEye;
	//target = oldTarget;

}

//�J�����V�F�C�N������
void CameraController::CameraShakeInit()
{
	isCameraShake_ = false;
	shakeTimer_ = defalutShakeTimer_;
	shakePowr_ = {};
}

//�J�����V�F�C�N
void CameraController::ShakeCamera()
{
	//constexpr float shakeValueX = 3.0f;
	//constexpr float shakeValueY = 3.0f;
	int max = 50;
	int min = -50;
	
	// 0.0349066	2�x
	// 0.0872665    5�x
	// 0.174533    10�x
	// 0.261799    15�x

	//�O�̃��W�A���p��0�ȏ�̎��� 0.0�x ~ -5.0�x
	if (radian_ > 0)
	{
		max = 0;
		min = -30;
	}
	// 5.0�x ~ 0.0�x 
	else
	{
		max = 30;
		min = 0;
	}



	//5.0 ~ -5.0�x
	shakePowr_.x = ((rand() % (max - min + 1)) + min) * 0.1f;	//�����_�ɒ���
	
	

	//���W�A���p�ɕϊ�
	radian_ = DirectX::XMConvertToRadians(shakePowr_.x);

	//�V�F�C�N���̉��Z
	//target.x += shakePowr.x;
	//target.y += shakePowr.y;
	//eye.x += shakePowr.x;
	//eye.y += shakePowr.y;

	ax_ = radian_;
}