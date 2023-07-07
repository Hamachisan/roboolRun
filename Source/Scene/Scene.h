#pragma once
#include "Graphics/Graphics.h"

//シーン
class Scene
{
public:
	Scene() {}
	virtual ~Scene() {}

	//初期化
	virtual void Initialize() = 0;				//まとめ
	virtual void SpriteInitialize() = 0;		//スプライト読み込み
	virtual void ParameterInitialize() = 0;		//パラメーターの設定

	//終了化
	virtual void Finalize() = 0;

	//更新処理
	virtual void Update(float elapsedtime) = 0;

	//描画処理
	virtual void Render() = 0;
	virtual void UiRender(ID3D11DeviceContext* dc) = 0;

	//準備完了しているか
	bool IsReady() const { return ready_; }

	//準備完了設定
	void setReady() { ready_ = true; }

	//光のセット
	virtual void SettingLight(ID3D11DeviceContext* dc, RenderContext& rc) {};

	//スプライトのパラメータを更新する関数
	virtual void SpriteParameterUpdate(float elapsedTime) = 0;
	//UIを変更する関数
	virtual void ChangeUi() = 0;

private:
	bool  ready_ = false;

public:
	float gameTimer_ = 0.0f;
};

