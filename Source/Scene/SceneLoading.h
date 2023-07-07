#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
#include "SceneGame.h"

#include <Xinput.h>

//ローディングシーン
class SceneLoading : public Scene
{
public:
	//コンストラクタの初期化子リストで次のシーンを設定
	SceneLoading(Scene* nextScene) : nextScene_(nextScene) {}
	//SceneLoading(){}
	~SceneLoading() override {};

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

private:
	//ローディングスレッド
	static void LoadingThread(SceneLoading* scene);

	//スプライトのパラメータを更新する関数
	void SpriteParameterUpdate(float elapsedTime) override;

	//UIをチェンジする関数
	void ChangeUi() override;

private:
	std::unique_ptr<Sprite> sprLoadingIcon_		= nullptr;		//ローディング時のぐるぐる
	std::unique_ptr<Sprite> sprLoadingKey_		= nullptr;		//キーボード操作時の説明
	std::unique_ptr<Sprite> sprLoadingPad_		= nullptr;		//パッド操作時の説明
	std::unique_ptr<Sprite> sprNowLoading_		= nullptr;		//ローディング文字
	std::unique_ptr<Sprite> sprStart_			= nullptr;		//スタート文字
	std::unique_ptr<Sprite> sprLine_			= nullptr;		//文字が流れるライン

	std::unique_ptr<Sprite> sprPushKeyButton_[2] = {};			//コントローラー接続しているかどうかでエンターキーかBボタンを表示

	///----------------------------------------------------------------------------

private:
	float	angle_			= 0.0f;				//アイコンの角度
	float	posX_			= 1280 - 512;		//ローディング文字が流れる用X座標変数
	bool	isRender_		= false;			//スタート文字を表示するフラグ
	int		flashingTime_	= 0;				//スタート文字を点滅

	Scene* nextScene_		= nullptr;			//次のシーン

	//コントローラーかpadを繋いでいるかで表示する画像を変更
	bool isController_ = false;

	int keyOrButton = 0;	//0 : キーボード   1 : パッド

	enum
	{
		KEY,
		BUTTON
	};

public:
	//static int nextScenes_;	//次のシーン
};