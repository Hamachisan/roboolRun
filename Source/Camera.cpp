#include "Camera.h"

//�w������֌���
void Camera::SetLookAt(
	const DirectX::XMFLOAT3& eye, 
	const DirectX::XMFLOAT3& focus,
	const DirectX::XMFLOAT3& up)
{
	//���_�A���ӓ_�A���������r���[�s����쐬
	DirectX::XMVECTOR Eye	= DirectX::XMLoadFloat3(&eye);
	DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&focus);
	DirectX::XMVECTOR Up	= DirectX::XMLoadFloat3(&up);
	DirectX::XMMATRIX View	= DirectX::XMMatrixLookAtLH(Eye, Focus, Up);

	DirectX::XMStoreFloat4x4(&view_, View);

	//�r���[���t�s�񉻂��A���[���h�s��ɖ߂�
	DirectX::XMMATRIX World = DirectX::XMMatrixInverse(nullptr, View); //�}�g���b�N�X�͍s��A�C���o�[�X�͋t
	DirectX::XMFLOAT4X4 world;

	DirectX::XMStoreFloat4x4(&world, World);

	//�J�����̕��������o��
	this->right_.x = world._11;
	this->right_.y = world._12;
	this->right_.z = world._13;

	this->up_.x = world._21;
	this->up_.y = world._22;
	this->up_.z = world._23;

	this->front_.x = world._31;
	this->front_.y = world._32;
	this->front_.z = world._33;

	//���_�A���ӓ_��ۑ�
	this->eye_ = eye;
	this->focus_ = focus;
}

//�p�[�X�y�N�e�B�u�ݒ�
void Camera::SetPerspectiveFov(float fovY, float aspect, float nearZ, float farZ)
{
	//��p�A��ʔ䗦�A�N���b�v��������v���W�F�N�V�����s����쐬
	DirectX::XMMATRIX Projection = 
		DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, nearZ, farZ);
	DirectX::XMStoreFloat4x4(&projection_, Projection);

	//�N���b�v�����ݒ�
	this->nearZ_ = nearZ;
	this->farZ_ = farZ;
}