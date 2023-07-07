#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "SceneLoading.h"
#include "SceneManager.h"
#include <thread>

//初期化
void SceneLoading::Initialize()
{
	//スプライト初期化
	SpriteInitialize();

	//スレッド開始
	std::thread thread(LoadingThread, this);

	//スレッドの管理を破棄
	thread.detach();
}

//パラメーター初期化
void SceneLoading::ParameterInitialize()
{

}

//スプライト初期化
void SceneLoading::SpriteInitialize()
{
	sprLoadingIcon_						= std::make_unique<Sprite>("Data/Sprite/Scene/Loading/LoadingIcon.png");
	sprLoadingKey_				= std::make_unique<Sprite>("Data/Sprite/Scene/Loading/LoadingAndKey.png");
	sprLoadingPad_				= std::make_unique<Sprite>("Data/Sprite/Scene/Loading/LoadingAndPad.png");
	sprNowLoading_				= std::make_unique<Sprite>("Data/Sprite/Scene/Loading/nowLoading.png");
	sprStart_					= std::make_unique<Sprite>("Data/Sprite/Scene/Loading/Start.png");
	sprLine_						= std::make_unique<Sprite>("Data/Sprite/Scene/Loading/line.png");

	sprPushKeyButton_[KEY]		= std::make_unique<Sprite>("Data/Sprite/Scene/Title/EnterKey.png");
	sprPushKeyButton_[BUTTON]	= std::make_unique<Sprite>("Data/Sprite/Scene/Title/Bbutton.png");
}


//終了化
void SceneLoading::Finalize()
{
}

//更新処理
void SceneLoading::Update(float elapsedTime)
{
	//スプライトのパラメータ更新処理
	SpriteParameterUpdate(elapsedTime);

	//次のシーンがゲームの時
	int nextScene = SceneManager::Instance().GetNextScene();
	if (nextScene == NEXT_SCENE_GAME)
	{
		GamePad& gamePad = Input::Instance().GetGamePad();
		//コントローラーが繋がれていたら表示するUIを変更
		ChangeUi();

		//点滅させる用
		flashingTime_++;
		if (nextScene_->IsReady())
		{
			isRender_ = true;
		}

		//ボタンを押したらゲームシーンへ切り替え
		if (isRender_)
			if (gamePad.GetButtonDown() & GamePad::BTN_ENTER ||
				gamePad.GetButtonDown() & GamePad::BTN_PAD_B)
			{
				SceneManager::Instance().ChangeScene(nextScene_);
				nextScene_ = nullptr;
			}
	}
	//次のシーンがゲーム以外の時
	else
	{
		if (nextScene_->IsReady())
		{
			SceneManager::Instance().ChangeScene(nextScene_);
			nextScene_ = nullptr;
		}
	}
}

void SceneLoading::UiRender(ID3D11DeviceContext* dc)
{
	//画面右下にローディングアイコンを描画
	float textureWidth = static_cast<float>(sprLoadingIcon_->GetTextureWidth());
	float textureHeight = static_cast<float>(sprLoadingIcon_->GetTextureHeight());
	float positionX = SCREEN_WIDTH - textureWidth;
	float positionY = SCREEN_HEIGHT - textureHeight;

	//次のシーンがゲームの時
	int nextScene = SceneManager::Instance().GetNextScene();
	if (nextScene == NEXT_SCENE_GAME)
	{
		//ルール表示
		if (isController_ == false)
			sprLoadingKey_->Render(dc,			
				0, 0,											//座標
				SCREEN_WIDTH, SCREEN_HEIGHT,					//スケール
				0, 0, 											//テクスチャ基準位置
				SCREEN_WIDTH, SCREEN_HEIGHT,					//テクスチャの大きさ
				0,												//角度
				1, 1, 1, 1);									//色(rgb)不透明度(a)
		else
			sprLoadingPad_->Render(
				dc,
				0, 0,											//座標
				SCREEN_WIDTH, SCREEN_HEIGHT,					//スケール
				0, 0, 											//テクスチャ基準位置
				SCREEN_WIDTH, SCREEN_HEIGHT,					//テクスチャの大きさ
				0,												//角度
				1, 1, 1, 1);									//色(rgb)不透明度(a)


		//バー
		sprLine_->Render(dc,
			0, 540,									//座標
			SCREEN_WIDTH, 150,						//スケール
			0, 0, 									//テクスチャ基準位置
			SCREEN_WIDTH, 150,						//テクスチャの大きさ
			0,										//角度
			1, 1, 1, 1);							//色(rgb)不透明度(a)

		//NowLoading
		if (!isRender_)
			sprNowLoading_->Render(dc,
				posX_, 550,								//座標
				PIXEL_512, PIXEL_128,					//スケール
				0, 0, 									//テクスチャ基準位置
				PIXEL_512, PIXEL_128,					//テクスチャの大きさ
				0,										//角度
				1, 1, 1, 1);							//色(rgb)不透明度(a)

		//次に進む
		if (isRender_)
		{
			if (flashingTime_ / 24 % 2)
			{
				sprStart_->Render(dc,
					700, 550,									//座標
					PIXEL_512, PIXEL_128,						//スケール
					0, 0, 										//テクスチャ基準位置
					PIXEL_512, PIXEL_128,						//テクスチャの大きさ
					0,											//角度
					1, 1, 1, 1);								//色(rgb)不透明度(a)

				//エンターはBボタンか
				sprPushKeyButton_[keyOrButton]->Render(dc,
					1150, 560,									//座標
					110, 110,									//スケール
					0, 0, 										//テクスチャ基準位置
					PIXEL_512, PIXEL_512,						//テクスチャの大きさ
					0, 											//角度
					1, 1, 1, 1);								//色(rgb)不透明度(a)
			}
		}
	}
	else
	{
		sprLoadingIcon_->Render(dc,
			positionX, positionY,				//座標
			textureWidth, textureHeight,		//スケール
			0, 0,								//テクスチャ基準位置
			textureWidth, textureHeight,		//テクスチャの大きさ
			angle_,								//角度
			1, 1, 1, 1);						//色(rgb)不透明度(a)

	}
}

//描画処理
void SceneLoading::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	//画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	//rgb a (0.0~1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	//2Dスプライト描画
	{
		UiRender(dc);
	}
}

//ローディングスレッド
void SceneLoading::LoadingThread(SceneLoading* scene)
{
	//COM関連の初期化でスレッド毎に呼ぶ必要がある
	CoInitialize(nullptr);

	//次のシーンの初期化処理を行う
	scene->nextScene_->Initialize();

	//スレッドが終わる前にCOM関連の終了化
	CoUninitialize();

	//次のシーン準備完了設定
	scene->nextScene_->setReady();

}

//スプライトのパラメータ更新処理
void SceneLoading::SpriteParameterUpdate(float elapsedTime)
{
	constexpr float speed = 180;
	angle_ += speed * elapsedTime;

	posX_ -= 10;
	if (posX_ + 512 < 0)
	{
		posX_ = 1280;
	}

}

//UIの変更処理
void SceneLoading::ChangeUi()
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetControllerState())
	{
		keyOrButton = BUTTON;
		isController_ = true;
	}
	else
	{
		keyOrButton = KEY;
		isController_ = false;
	}
}