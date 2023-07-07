#pragma once

#include<DirectXMath.h>

//カメラコントローラー
class CameraController
{
public:
	CameraController(){}
	~CameraController(){}

	//初期化
	//void Initialize();

	//更新処理
	void Update(float elapsedTime);

	//カメラシェイク
	void ShakeCamera();

	//カメラシェイク初期化
	void CameraShakeInit();
	//パブリック変数
public:
	//カメラとプレイヤーの距離
	float			   range_ = 6.2f;

	//プライベート変数
private:
	DirectX::XMFLOAT3	target_ = { 0, 0, 0 };
	DirectX::XMFLOAT3	angle_ = { 0, 0, 0 };
	DirectX::XMFLOAT3	eye_ = { 0,0,0 };

	float ax_ = 0;
	float ay_ = 0;

	//カメラシェイク用パラメータ
	DirectX::XMFLOAT3 oldTarget_ = { 0, 0, 0 };
	DirectX::XMFLOAT3 oldEye_ = { 0, 0, 0 };

	float			   rollSpeed_ = DirectX::XMConvertToRadians(90);

	float				maxAngleX_ = DirectX::XMConvertToRadians(45);
	float				minAngleX_ = DirectX::XMConvertToRadians(-45);

	//カメラシェイクの変数
	bool isCameraShake_ = false;
	const float  defalutShakeTimer_ = 1.00f;
	float shakeTimer_ = defalutShakeTimer_;
	DirectX::XMFLOAT2 shakePowr_ = {};
	float radian_ = 0;



	//プライベート関数
private:

	//ゲッター、セッター
public:
	//ターゲット位置設定
	void SetTarget(const DirectX::XMFLOAT3& target) { this->target_ = target; }
	//カメラシェイクのフラグ設定
	void SetCameraShake(bool shakeFlag) { isCameraShake_ = shakeFlag; }
	bool GetCameraShake() { return isCameraShake_; }

	//角度セット
	void setCameraYAngle(float angle) { ax_ = DirectX::XMConvertToRadians(angle); }
	void setCameraXAngle(float angle) { ay_ = DirectX::XMConvertToRadians(angle); }
	//距離セット
	void setCameraRange(float distance) { range_ = distance; }
};