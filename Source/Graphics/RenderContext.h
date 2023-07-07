#pragma once

#include <DirectXMath.h>

//平行光源データ
struct DirectionalLight
{
	DirectX::XMFLOAT4 direction;	//方向
	DirectX::XMFLOAT4 color;		//色
};

//ポイントライトデータ
struct PointLight
{
	DirectX::XMFLOAT4 position{ 0, 0, 0, 0 };	//ライトの位置
	DirectX::XMFLOAT4 color{ 1, 1, 1, 1 };		//ライトの色
	float range{ 0 };							//ライトの範囲
	DirectX::XMFLOAT3 dummy;					//4の倍数に収めるためのダミー
};

// 色調補正情報
struct ColorGradingData
{
	float	hueShift = 0;	// 色相調整
	float	saturation = 1;	// 彩度調整
	float	brightness = 1;	// 明度調整
	float	dummy;
};


struct RadialBlurData
{
	float	sampleCount = 2;		// 回数
	float	strength = 0.2f;			//ブラーの強さ 
	DirectX::XMFLOAT2 center{0.5, 0.5};	//中心uv座標
};

// レンダーコンテキスト
struct RenderContext
{
	ID3D11DeviceContext*		deviceContext;
	DirectX::XMFLOAT4			screenSize;

	//カメラ情報
	DirectX::XMFLOAT4X4		view;
	DirectX::XMFLOAT4X4		projection;
	DirectX::XMFLOAT4		viewPosition;
	DirectX::XMFLOAT4		lightDirection;

	//平行光源情報
	DirectX::XMFLOAT4 ambientLightColor;
	DirectionalLight directionalLight;

	//ポイントライト情報
	PointLight pointLight[8];

	//	色調補正情報
	ColorGradingData	colorGradingData;

	//放射ブラー情報
	RadialBlurData	radialBlurData;

};
