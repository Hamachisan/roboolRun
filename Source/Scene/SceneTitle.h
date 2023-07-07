#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
#include "Stage.h"
#include "Character/Player.h"
#include "Character/TitlePlayer.h"
#include "CameraController.h"
//#include <Xinput.h>
#include "Graphics/Texture.h"
#include "Other/Constant.h"
#include "Audio/Audio.h"

//タイトルシーン
class SceneTitle : public Scene
{
public:
	SceneTitle() {};
	~SceneTitle() override {}

	//初期化
	void Initialize() override;
	void SpriteInitialize() override;
	void ParameterInitialize() override;

	//終了化
	void Finalize() override;

	//更新処理
	void Update(float elapsedTime) override;

	//描画処理
	void Render() override;
	//UIの描画
	void UiRender(ID3D11DeviceContext* dc)override;

	//-------------------------------------------------------------------------------------------------------------------

private:
	//ステージ選択
	void StageSelect();

	//マルチスレッドでの衝突回避用
	void ThreadCover();

	//スプライトのパラメータを更新する関数
	void SpriteParameterUpdate(float elapsedTime) override;

	//UIをチェンジする関数
	void ChangeUi() override;

	//-------------------------------------------------------------------------------------------------------------------

	//モデル
private:
	Stage* stage_			= nullptr;
	TitlePlayer* player_	= nullptr;

	//-------------------------------------------------------------------------------------------------------------------

	//Sprite
private:
	std::unique_ptr<Sprite> sprGameTitle_			= nullptr;		//タイトル
	std::unique_ptr<Sprite> sprGradationBack_		= nullptr;		//タイトルの後ろに
	
	std::unique_ptr<Sprite> sprBack_				= nullptr;		//下半分を透過させる
	std::unique_ptr<Sprite> sprAllBack_				= nullptr;
	std::unique_ptr<Sprite> sprPushKeyButton_[2]	= {};			//エンターキーもしくはBボタンの表示
	std::unique_ptr<Sprite> sprPush_				= nullptr;		//Push文字の表示
	std::unique_ptr<Sprite> sprkeyOrButton_[2]		= {};			//KeyかButtonか文字の表示
	
	std::unique_ptr<Sprite> sprLine_				= nullptr;		//枠

	std::unique_ptr<Sprite> skyboxRenderer_			= nullptr;		//空描画用スプライト
	std::unique_ptr<Texture> skyboxTexture_;						//パノラマスカイボックス

	std::unique_ptr<Sprite> sprFont_				= nullptr;		//フォント
	std::unique_ptr<Sprite> sprStage_				= nullptr;		//ステージ文字
	std::unique_ptr<Sprite> sprLevel_				= nullptr;		//レベル文字

	std::unique_ptr<Sprite> sprArrow_				= nullptr;	//矢印
	std::unique_ptr<Sprite> sprFrame_				= nullptr;	//数字の周りの囲い


	//Audio
	std::unique_ptr<AudioSource> bgmTitle_			= nullptr;	//タイトル画面BGM
	std::unique_ptr<AudioSource> seSelect_			= nullptr;	//ステージ選択時のピコピコ
	std::unique_ptr<AudioSource> seStart_			= nullptr;	//ステージ選択時の決定音



	//-------------------------------------------------------------------------------------------------------------------

private:
	int titleState_ = 0;					//場面
	int stageSelect_ = 0;					//ステージ選択
	int stageLevelSelect_ = 0;				//ステージレベル選択

	//カメラ
	CameraController* cameraController_ = nullptr;
	DirectX::XMFLOAT3 target_;

	//SpriteShaderを使うSpriteかどうかの判断
	bool isSpriteShader_ = false;
	//マルチスレッドでinitializeで競合しない用(ローディング時)
	int threadCover_ = 0;

	//コントローラーかpadを繋いでいるかで表示する画像を変更
	//DWORD dwResult;
	int keyOrButton_ = 0;
	enum
	{
		KEY,
		BUTTON
	};

	///////////////////////////////////////////////////////////////
	//					Spriteのパラメータ
	///////////////////////////////////////////////////////////////

	
	//キーorボタンで切り替わるsprite座標、スケール
	DirectX::XMFLOAT2 keyOrButtonPos_;
	DirectX::XMFLOAT2 keyOrButtonScale_;

	//Pushの文字sprite座標。スケール
	DirectX::XMFLOAT2 pushPos_;
	DirectX::XMFLOAT2 pushScale_;

	//Buttonかkeyかの文字spriteの座標、スケール
	DirectX::XMFLOAT2 StringKeyOrButtonPos_;
	DirectX::XMFLOAT2 StringKeyOrButtonScale_;

	//タイトルとタイトルの後ろグラデーションの座標、スケール、テクスチャ座標
	DirectX::XMFLOAT2 titlePos_{};	
	DirectX::XMFLOAT2 titleScale_{};	
	DirectX::XMFLOAT2 titleTexPos_{};
	float posY_ = 0;

	float fontTexPosX_ = 64;
	float fontTexPosX2_ = 64;

	//ステージ選択矢印表示
	bool isArrowUp_		= true;
	bool isArrowDown_	= true;
	bool isArrowRight_	= true;
	bool isArrowLeft_	= true;
	//点滅用タイマー
	int flashingTime_ = 0;
	//se再生フラグ
	bool isSeFlag_ = false;
	float sePlayTimer_ = 0.0f;

	///////////////////////////////////////////////////////////////////////

	enum LEVEL
	{
		level1,
		level2
	};
	enum Arrow
	{
		Dwon,
		Up
	};

	int nowStage_ = 0;
	int nowLevel_ = 1;

	float offsetPosY1_ = 480;
	float offsetPosY2_ = 880;


	//調性用パラメータ
	float font1PosX_ = 346;
	float font1PosY_ = 530;
	float font2PosX_ = 820;
	float font2PosY_ = 530;
	float Text1PosX_ = 20;
	float Text1PosY_ = 500;
	float Text2PosX_ = 490;
	float Text2PosY_ = 500;

	float arrowLevelPosX_ = 340;
	float arrowLevelPosY1_ = 600;
	float arrowLevelPosY2_ = 460; 

	float arrowStagePosX1_ = 810;
	float arrowStagePosX2_ = 900;
	float arrowStagePosY_ = 530;

	//-------------------------------------------------------------------------------------------------------------------
};