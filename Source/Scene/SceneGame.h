#pragma once

#include "Graphics/Texture.h"
#include "Graphics/RenderTarget.h"
#include "Graphics/Sprite.h"
#include "Scene.h"
#include "Character/Player.h"
#include "Character/Enemys/EnemyArrangement.h"
#include "Stage.h"
#include "CameraController.h"
#include "ScenePause.h"
#include"Other/MyFunction.h"
#include"LIght/Light.h"
#include "Audio/Audio.h"
#include "Score.h"

#include "Other/Mathf.h"

// ゲームシーン
//class SceneGame
class SceneGame : public Scene
{
	//メンバ関数(public)
public:
	SceneGame() {}
	~SceneGame() override {}

	// 初期化 (まとめ)
	void Initialize() override;
	//パラメーター初期化
	void ParameterInitialize() override;
	//スプライト初期化
	void SpriteInitialize() override;
	//敵をステージに配置
	void EnemySetStage();

	// 終了化
	void Finalize() override;

	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render() override;

	//3D空間の描画
	void Render3DScene();
	//UIの描画
	void UiRender(ID3D11DeviceContext* dc)override;

	//-------------------------------------------------------------------------------------------------------------------

	//メンバ関数(private)
private:
	//背景オブジェクトを動かすための関数
	void MoveBackObject();
	//ライト設定
	void SettingLight(ID3D11DeviceContext* dc, RenderContext& rc)override;
	//プレイヤーの動きで色を替えるライトの設定
	void PlayerSettingLight(RenderContext& rc);

	//デバッグ用フリーカメラ
	DirectX::XMFLOAT3 GetMoveVec() const;
	void FreeCamera(float elapsedTime);

	//マルチスレッドでの衝突回避用
	void ThreadCover();

	//ポーズ処理
	bool PauseProcess(float elapsedTime);

	//スプライトのパラメータを更新する関数
	void SpriteParameterUpdate(float elapsedTime) override;

	//UIをチェンジする関数
	void ChangeUi() override;

	//1フレームだけUpdateを行い、カウントダウンをしている時にプレイヤーが見えるように
	void OneFrameUpdate(float elapsedTime);

	//-------------------------------------------------------------------------------------------------------------------

	//モデル
private:
	Stage*				stage_					= nullptr;
	Player*				player_					= nullptr;
	EnemyArrangement*	enemyArrangement_		= nullptr;
	CameraController*	cameraController_		= nullptr;
	Pause*				pause_					= nullptr;

	//-------------------------------------------------------------------------------------------------------------------
	
	//Sprite, Audio, Effect
private:
	std::unique_ptr<Texture> sprSkyTexture_		= nullptr;			//背景画像
	std::unique_ptr<Sprite> sprite_				= nullptr;			//背景画像

	std::unique_ptr<Sprite> sprTimer_			= nullptr;			//タイマー
	std::unique_ptr<Sprite> sprTimerFrame_		= nullptr;			//タイマーの枠
	std::unique_ptr<Sprite> sprTimerTen_		= nullptr;			//タイマーの少数点
	std::unique_ptr<Sprite> sprGo_				= nullptr;			//GO!!表示
	std::unique_ptr<Sprite> sprFinish_			= nullptr;			//FINISH!!表示
	std::unique_ptr<Sprite> Textsprites_[8];							//フォント
	std::unique_ptr<Sprite> sprSpeedMeter_		= nullptr;			//スピードメーター
	std::unique_ptr<Sprite> sprSpeedMeterFrame_	= nullptr;			//スピードメーターの枠
	std::unique_ptr<Sprite> sprSpeedMax_		= nullptr;			//最大スピード時描画
	std::unique_ptr<Sprite> sprSpeed_			= nullptr;			//スピード(文字)
	std::unique_ptr<Sprite> sprImpuls_			= nullptr;			//衝撃を受けた時のダメージ表現	

	//タイミング
	std::unique_ptr<Sprite> sprPerfect_			= nullptr;			//パーフェクト
	std::unique_ptr<Sprite> sprGreat_			= nullptr;			//グレイト
	std::unique_ptr<Sprite> sprGood_			= nullptr;			//グッド

	//コンボ
	std::unique_ptr<Sprite> sprCombo_			= nullptr;			//コンボ文字
	std::unique_ptr<Sprite> sprComboConut_		= nullptr;			//コンボの数字
	std::unique_ptr<Sprite> sprComboFrame_		= nullptr;			//コンボの枠

	//操作方法UI
	std::unique_ptr<Sprite> sprUiFrame_				= nullptr;		//UIを置くための枠
	std::unique_ptr<Sprite> sprUiBack_				= nullptr;		//UIの背後
	std::unique_ptr<Sprite> sprUiObjs_[4]			= {};			//障害物オブジェクトUI
	std::unique_ptr<Sprite> sprUiOperations_[5][2]	= {};			//ボタン、キーボード

	//Audio関連
	//BGM
	std::unique_ptr<AudioSource> bgmGame_			= nullptr;		//ゲームBGM
	//SE
	std::unique_ptr<AudioSource> seCountDown_		= nullptr;		//カウントダウンの音
	std::unique_ptr<AudioSource> seFinish_			= nullptr;		//ゴールした音


	std::unique_ptr<Sprite> skyboxRenderer_ = nullptr;		//	空描画用スプライト
	std::unique_ptr<Texture> skyboxTexture_;					//パノラマスカイボックス

	//-------------------------------------------------------------------------------------------------------------------

	//enum
private:
	enum Operations
	{
		keyboard = 0,
		gamePad
	};
	enum KeyBoard
	{
		Akey = 0,
		Dkey,
		Jkey,
		Kkey,
		Lkey
	};
	enum Pad
	{
		Left = 0,
		Right,
		Xbutton,
		Abutton,
		Bbutton
	};

	//-------------------------------------------------------------------------------------------------------------------
	
	//メンバ変数(private)
private:
	//
	int stageLevel_ = 0;			//ステージで障害物の描画を変更
	DirectX::XMFLOAT3 target_;	//カメラのターゲット
	//ポーズ
	bool isPause_ = false;

	float gameStartTime_ = 0.0f;	//ゲームをスタートするまでの時間
	float gameClearTime_ = 0.0f;	//ゲームをクリアした後の時間

	//スプライトのパラメータ
	//タイマー関連
	float onesPlace_					= 0;	//1の位
	float tensPlace_					= 0;	//10の位
	float firstDecimalPlace_			= 0;	//小数点第1位
	float texSize_						= 64;	//テクスチャずらす値
	float timerTexPosX_[3]				= {};	//テクスチャX座標
	float ImpulsTimer_					= 0;
	//最大スピード時文字の点滅
	int flashingTime_					= 0;

	//Go!! ポジション
	float GoPosX_ = 0;

	//メーター
	float meterTexX_ = 0;
	float speed_, maxSpeed_, ratio_ = 0;

	//RGBグラデーション
	float r_, g_, b_, rStart_, gStart_, bStart_, rEnd_, gEnd_, bEnd_ = 0;

	//光源関連のパラメータ
	DirectX::XMFLOAT4			ambientLightColor_ = {};
	std::unique_ptr<Light>		directionalLight_;
	DirectX::XMFLOAT3			directional_ = { 0, -1, -1 };
	//ポイントライト
	DirectX::XMFLOAT4			pointLightPosition_{0, 0, 0, 0};
	DirectX::XMFLOAT4			pointLightColor_{1, 1, 1, 1};//白
	float						pointLightRange_ = 0;
	int							animState_ = 0;

	//デバック用フリーカメラ
	DirectX::XMFLOAT3			freeCameraPosition_{0, 0, 0};
	DirectX::XMFLOAT3			freeCameraVelocity_{0, 0, 0};
	const float					freeCameraSpeed_ = 1.0f;
	bool						isFreeCamera_	= false;
	float						ax_ = 0;
	float						ay_ = 0;

	//タイミング表示
	int timing_ = 0;

	//コンボ表示
	float comboTexPosX_[2] = {};

	//コントローラーかpadを繋いでいるかで表示する画像を変更
	int	whichOperation_ = Operations::keyboard;
	int anyKeyOrButton_[5] = {};

	//SpriteShaderを使うSpriteかどうかの判断
	bool isSpriteShader_ = false;
	//マルチスレッドでinitializeで競合しない用(ローディング時)
	int threadCover_ = 0;

	//オフスクリーン用レンダリング描画ターゲット
	std::unique_ptr<RenderTarget> renderTarget_;
	//	色調補正情報
	ColorGradingData	colorGradingData_;
	// 放射ブラー情報
	RadialBlurData radialBlurData_;
	//
	bool isSpriteShaderSet_ = true;

	//線形補完を扱うクラス
	Mathf mathf_;

	//効果音を一度だけ鳴らす為の変数
	int seCount_ = 0;

	//Updateを1フレームだけ行う用変数
	int oneFrameCount = 0;
};
