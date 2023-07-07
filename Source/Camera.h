#pragma once

#include<DirectXMath.h>

//カメラ
class Camera
{
private:	//シングルトン化する為コンストラクタをプライベートに設定する
	Camera(){}
	~Camera(){}

public:

	//唯一のインスタンス取得
	static Camera& Instance()
	{
		static Camera camera;
		return camera;
	}

	//指定方向へ向く
	void SetLookAt(
		const DirectX::XMFLOAT3& eye, 
		const DirectX::XMFLOAT3& focus,
		const DirectX::XMFLOAT3& up
	);

	//パースペクティブ設定
	void SetPerspectiveFov(float fovY, float aspect, float nearZ, float farZ);

	//ビュー行列取得
	const DirectX::XMFLOAT4X4& GetView() const { return view_; }

	//プロジェクション行列取得	m
	const DirectX::XMFLOAT4X4& GetProjection() const { return projection_; }

	//視点取得
	const DirectX::XMFLOAT3& GetEye() const { return eye_; }
	//注意点取得
	const DirectX::XMFLOAT3& GetFocus() const { return focus_; }
	//上方向取得
	const DirectX::XMFLOAT3& GetUp() const { return up_; }
	//前方向取得
	const DirectX::XMFLOAT3& GetFront() const { return front_; }
	//右方向取得
	const DirectX::XMFLOAT3& GetRight() const { return right_; }

	// 近クリップ距離取得
	const float& GetNearZ() const { return nearZ_; }
	// 遠クリップ距離取得
	const float& GetFarZ() const { return farZ_; }

private:
	DirectX::XMFLOAT4X4 view_;
	DirectX::XMFLOAT4X4 projection_;

	DirectX::XMFLOAT3 eye_;
	DirectX::XMFLOAT3 focus_;

	DirectX::XMFLOAT3 up_;
	DirectX::XMFLOAT3 front_;
	DirectX::XMFLOAT3 right_;

	//クリップ距離
	float	nearZ_;
	float	farZ_;
};