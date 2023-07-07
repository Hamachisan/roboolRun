#pragma once

#include <DirectXMath.h>

//���s�����f�[�^
struct DirectionalLight
{
	DirectX::XMFLOAT4 direction;	//����
	DirectX::XMFLOAT4 color;		//�F
};

//�|�C���g���C�g�f�[�^
struct PointLight
{
	DirectX::XMFLOAT4 position{ 0, 0, 0, 0 };	//���C�g�̈ʒu
	DirectX::XMFLOAT4 color{ 1, 1, 1, 1 };		//���C�g�̐F
	float range{ 0 };							//���C�g�͈̔�
	DirectX::XMFLOAT3 dummy;					//4�̔{���Ɏ��߂邽�߂̃_�~�[
};

// �F���␳���
struct ColorGradingData
{
	float	hueShift = 0;	// �F������
	float	saturation = 1;	// �ʓx����
	float	brightness = 1;	// ���x����
	float	dummy;
};


struct RadialBlurData
{
	float	sampleCount = 2;		// ��
	float	strength = 0.2f;			//�u���[�̋��� 
	DirectX::XMFLOAT2 center{0.5, 0.5};	//���Suv���W
};

// �����_�[�R���e�L�X�g
struct RenderContext
{
	ID3D11DeviceContext*		deviceContext;
	DirectX::XMFLOAT4			screenSize;

	//�J�������
	DirectX::XMFLOAT4X4		view;
	DirectX::XMFLOAT4X4		projection;
	DirectX::XMFLOAT4		viewPosition;
	DirectX::XMFLOAT4		lightDirection;

	//���s�������
	DirectX::XMFLOAT4 ambientLightColor;
	DirectionalLight directionalLight;

	//�|�C���g���C�g���
	PointLight pointLight[8];

	//	�F���␳���
	ColorGradingData	colorGradingData;

	//���˃u���[���
	RadialBlurData	radialBlurData;

};
