#pragma once

#include<DirectXMath.h>

//�J����
class Camera
{
private:	//�V���O���g��������׃R���X�g���N�^���v���C�x�[�g�ɐݒ肷��
	Camera(){}
	~Camera(){}

public:

	//�B��̃C���X�^���X�擾
	static Camera& Instance()
	{
		static Camera camera;
		return camera;
	}

	//�w������֌���
	void SetLookAt(
		const DirectX::XMFLOAT3& eye, 
		const DirectX::XMFLOAT3& focus,
		const DirectX::XMFLOAT3& up
	);

	//�p�[�X�y�N�e�B�u�ݒ�
	void SetPerspectiveFov(float fovY, float aspect, float nearZ, float farZ);

	//�r���[�s��擾
	const DirectX::XMFLOAT4X4& GetView() const { return view_; }

	//�v���W�F�N�V�����s��擾	m
	const DirectX::XMFLOAT4X4& GetProjection() const { return projection_; }

	//���_�擾
	const DirectX::XMFLOAT3& GetEye() const { return eye_; }
	//���ӓ_�擾
	const DirectX::XMFLOAT3& GetFocus() const { return focus_; }
	//������擾
	const DirectX::XMFLOAT3& GetUp() const { return up_; }
	//�O�����擾
	const DirectX::XMFLOAT3& GetFront() const { return front_; }
	//�E�����擾
	const DirectX::XMFLOAT3& GetRight() const { return right_; }

	// �߃N���b�v�����擾
	const float& GetNearZ() const { return nearZ_; }
	// ���N���b�v�����擾
	const float& GetFarZ() const { return farZ_; }

private:
	DirectX::XMFLOAT4X4 view_;
	DirectX::XMFLOAT4X4 projection_;

	DirectX::XMFLOAT3 eye_;
	DirectX::XMFLOAT3 focus_;

	DirectX::XMFLOAT3 up_;
	DirectX::XMFLOAT3 front_;
	DirectX::XMFLOAT3 right_;

	//�N���b�v����
	float	nearZ_;
	float	farZ_;
};