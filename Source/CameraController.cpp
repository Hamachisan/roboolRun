#include "CameraController.h"
#include "Camera.h"
#include "Input/Input.h"
#include <time.h>

//更新処理
void CameraController::Update(float elapsedTime)
{
	//srand((unsigned int)time(NULL));

	GamePad& gamepad = Input::Instance().GetGamePad();
	
	//カメラの回転速度
	float speed = rollSpeed_ * elapsedTime;

	//スティックの入力値に合わせてx軸とｙ軸を回転
	//angle.x += ay * speed;
	angle_.x = 0.209439546f;
	//angle.x = ay;
	//angle.y += ax * speed;
	angle_.y = ax_;

	//X軸のカメラ回転を制限
	if (angle_.x > maxAngleX_)
		angle_.x = maxAngleX_ ;
	if (angle_.x < minAngleX_)
		angle_.x = minAngleX_;

	//y軸の回転値を-3.14~3.14に収まるようにする
	if (angle_.y < -DirectX::XM_PI)
	{
		angle_.y += DirectX::XM_2PI;
	}
	if (angle_.y > DirectX::XM_PI)
	{
		angle_.y -= DirectX::XM_2PI;
	}

	//カメラ回転値を回転行列に変換
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle_.x, angle_.y, angle_.z);

	//回転行列から前方向ベクトルを取り出す
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);

	//注意点から後ろベクトル方向にい一定距離離れたカメラ視点を求める
	eye_.x = target_.x - front.x * range_;
	eye_.y = target_.y - front.y * range_;
	eye_.z = target_.z - front.z * range_;

	//保存
	oldEye_ = eye_;
	oldTarget_ = target_;

	//カメラシェイク
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

	//カメラの視点と注意点を設定
	Camera::Instance().SetLookAt(eye_, target_, DirectX::XMFLOAT3(0, 1, 0));
	//戻す
	//eye = oldEye;
	//target = oldTarget;

}

//カメラシェイク初期化
void CameraController::CameraShakeInit()
{
	isCameraShake_ = false;
	shakeTimer_ = defalutShakeTimer_;
	shakePowr_ = {};
}

//カメラシェイク
void CameraController::ShakeCamera()
{
	//constexpr float shakeValueX = 3.0f;
	//constexpr float shakeValueY = 3.0f;
	int max = 50;
	int min = -50;
	
	// 0.0349066	2度
	// 0.0872665    5度
	// 0.174533    10度
	// 0.261799    15度

	//前のラジアン角が0以上の時は 0.0度 ~ -5.0度
	if (radian_ > 0)
	{
		max = 0;
		min = -30;
	}
	// 5.0度 ~ 0.0度 
	else
	{
		max = 30;
		min = 0;
	}



	//5.0 ~ -5.0度
	shakePowr_.x = ((rand() % (max - min + 1)) + min) * 0.1f;	//小数点に直す
	
	

	//ラジアン角に変換
	radian_ = DirectX::XMConvertToRadians(shakePowr_.x);

	//シェイク分の加算
	//target.x += shakePowr.x;
	//target.y += shakePowr.y;
	//eye.x += shakePowr.x;
	//eye.y += shakePowr.y;

	ax_ = radian_;
}