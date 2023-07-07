#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
#include <memory>
#include <stdlib.h>
#include <vector>
#include "Character/Enemys/EnemyArrangement.h"
#include "Character/TitlePlayer.h"
#include "Stage.h"
#include "Camera.h"
#include "CameraController.h"

#define COLUMN (4)	//行列の列
#define FILENAME "saveData.dat"
#define DATA_FORMAT (0)		//0...バイナリ 1...テキスト		※現在テキストモードでの生成は不可
#define SHOW_RANKING (3)

struct Ranking
{
	float top_ = 0;
	float second_ = 0;
	float third_ = 0;
	float save_ = 0;
};

class SceneClear : public Scene
{
	//public関数
public:
	SceneClear(){}
	~SceneClear(){}

	void Initialize() override;
	void SpriteInitialize() override;
	void ParameterInitialize() override;

	void Finalize() override;
	void Update(float elapsedTime) override;
	void Render() override;
	//UIの描画
	void UiRender(ID3D11DeviceContext* dc)override;

	//private関数
private:
	void SaveLoadFile();

	void RenderNumber(ID3D11DeviceContext* dc, float* size, int digit, DirectX::XMFLOAT2 pos,
		DirectX::XMFLOAT2 scale = { 64, 64 }, float intervalX = 64, float intervalY = 0);

	//スプライトのパラメータを更新する関数
	void SpriteParameterUpdate(float elapsedTime) override;

	//UIをチェンジする関数
	void ChangeUi() override;

	//モデル等
private:
	Stage*	stageModel_		= nullptr;
	TitlePlayer* player_	= nullptr;

	CameraController* cameraController_ = nullptr;
	DirectX::XMFLOAT3 target_;

	//Sprite,Audio, Effect
private:
	std::unique_ptr<Sprite> sprNext_[2]				 = {};			//次のシーンへ誘導する文字
	std::unique_ptr<Sprite> sprPoint_				= nullptr;		//pt文字
	std::unique_ptr<Sprite> sprNumber_				= nullptr;		//タイマー
	std::unique_ptr<Sprite> sprTimerTen_			= nullptr;		//タイマーの少数点
	std::unique_ptr<Sprite> Textsprites_[8]			= {};			//文字font

	std::unique_ptr<Sprite> sprBack_				= nullptr;		//背景

	std::unique_ptr<Sprite> sprScore_				= nullptr; 		//スコア文字
	std::unique_ptr<Sprite> sprPerfect_				= nullptr;		//パーフェクト文字
	std::unique_ptr<Sprite> sprGreat_				= nullptr;		//グレイト文字
	std::unique_ptr<Sprite> sprClearTime_			= nullptr;		//クリアタイム文字
	std::unique_ptr<Sprite> sprMaxCombo_			= nullptr;		//MAXコンボ文字

	//ランキング表示
	std::unique_ptr<Sprite> sprRanking_				= nullptr;		//ランキング文字
	std::unique_ptr<Sprite> spr1st_					= nullptr; 		//1st
	std::unique_ptr<Sprite> spr2nd_					= nullptr; 		//2nd
	std::unique_ptr<Sprite> spr3rd_					= nullptr; 		//3rd

	std::unique_ptr<Sprite> sprPushKeyButton_[2] = {};	//コントローラー接続しているかどうかでエンターキーかBボタンを表示


	//メンバ変数
private:
	int sceneState_ = 0;

	//タイマー関連
	float clearTime_ = 0.0f;
	float onesPlace_ = 0;				//1の位
	float tensPlace_ = 0;				//10の位
	float firstDecimalPlace_ = 0;		//小数点第1位
	float texSize_ = 64;					//テクスチャずらす値
	float RankingTimeTexPos_[SHOW_RANKING][6];

	float scoreTexPosX_[6];				//テクスチャX座標
	float clearTimeTexPosX_[3];				//テクスチャX座標
	float maxComboTexPosX_[2];				//テクスチャX座標
	float perfectTexPosX_[2];				//テクスチャX座標
	float greatTexPosX_[2];				//テクスチャX座標

	//Sprite座標
	DirectX::XMFLOAT2 pos_[12] = {};
	DirectX::XMFLOAT2 scale_[12] = {};


	//結果発表
	/*Ranking ranking;
	Ranking comparison;*/
	FILE* fp_ = nullptr;

	//今回のスコア
	float score_ = 0;

	//ソートする用の配列
	std::vector<vector<float>> data_ = { MAX_STAGE * MAX_STAGE_LEVEL, vector<float>(COLUMN) };	//一先ずめっちゃ多くしている
	//書き込み用の配列
	Ranking ranking_[MAX_STAGE * MAX_STAGE_LEVEL];
	int textCount_ = 0;	//テキスト読み書きの時のみ使う

	//今のステージのindex
	int stage_ = 0;

	//コントローラーかpadを繋いでいるかで表示する画像を変更
	int keyOrButton_ = 0;


	//enum
private:
	enum
	{
		top = 0,
		second,
		third,
		save
	};

	enum SpritePram
	{
		//文字
		word_score,			//0
		word_clearTime,		//1
		word_maxCombo,		//2
		word_perfect,		//3
		word_great,			//4

		//数字
		num_score,			//5
		num_clearTime,		//6
		num_maxCombo,		//7
		num_perfect,		//8
		num_great,			//9

		//ランキング
		num_ranking,		//10

		//py
		word_pt,			//11
	};
	enum
	{
		KEY,
		BUTTON
	};
};

