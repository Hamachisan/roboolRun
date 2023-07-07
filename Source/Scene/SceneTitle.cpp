#include "Graphics/Graphics.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include "Input/Input.h"
#include "SceneLoading.h"
#include "Camera.h"

#include "Other/Constant.h"

//初期化
void SceneTitle::Initialize()
{
	//ステージ初期化
	stage_ = new Stage();

	//プレイヤー初期化
	player_ = new TitlePlayer();

	//カメラ初期設定
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();	//&つけ忘れない!!
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,	//この位置にカメラを置く
		1000.0f	//見える距離
	);

	//カメラコントローラー初期化
	cameraController_ = new CameraController();
	target_ = player_->GetPosition();

	//スプライト初期化
	SpriteInitialize();
	//パラメーター初期化
	ParameterInitialize();

	//BGM初期化
	bgmTitle_ = Audio::Instance().LoadAudioSource("Data/Audio/BGM/Title/saiba29.wav");
	//SE初期化
	seSelect_ = Audio::Instance().LoadAudioSource("Data/Audio/SE/Scene/select2.wav");
	seStart_ = Audio::Instance().LoadAudioSource("Data/Audio/SE/Scene/start2.wav");
}

//パラメータの初期化
void SceneTitle::ParameterInitialize()
{
	//選択ラインの座標
	posY_ = 450;

	//位置調整用変数設定
	//キーorボタンで切り替わるsprite座標、スケール
	keyOrButtonPos_.x = 500;
	keyOrButtonPos_.y = 460;
	keyOrButtonScale_.x = keyOrButtonScale_.y = 128;

	//プッシュ文字spriteの座標、スケール
	pushPos_.x = 160;
	pushPos_.y = 460;
	pushScale_.x = 512;
	pushScale_.y = 128;

	//Buttonかkeyかの文字spriteの座標、スケール
	StringKeyOrButtonPos_.x = 690;
	StringKeyOrButtonPos_.y = 460;
	StringKeyOrButtonScale_.x = 512;
	StringKeyOrButtonScale_.y = 128;

	//タイトル文字とタイトルの後ろグラデーションの座標、スケール
	titlePos_.x = 490;
	titlePos_.y = 50;
	titleScale_.x = 512 * 1.5;
	titleScale_.y = 128 * 2;
	titleTexPos_.x = 0;
	titleTexPos_.y = 0;
}

void SceneTitle::SpriteInitialize()
{
	//スプライト初期化
	sprGameTitle_				= std::make_unique<Sprite>("Data/Sprite/Scene/Title/ロボールRUN3.png");
	sprGradationBack_			= std::make_unique<Sprite>("Data/Sprite/Scene/Title/gradationBack2.png");
	sprLine_					= std::make_unique<Sprite>("Data/Sprite/Scene/Title/line.png");
	sprBack_					= std::make_unique<Sprite>("Data/Sprite/Scene/Title/TitleBack.png");
	sprAllBack_					= std::make_unique<Sprite>("Data/Sprite/Scene/Reslut/back.png");

	//文字のUI
	sprPushKeyButton_[KEY]		= std::make_unique<Sprite>("Data/Sprite/Scene/Title/EnterKey.png");
	sprPushKeyButton_[BUTTON]	= std::make_unique<Sprite>("Data/Sprite/Scene/Title/Bbutton.png");
	sprPush_					= std::make_unique<Sprite>("Data/Sprite/Scene/Title/Push.png");
	sprkeyOrButton_[KEY]		= std::make_unique<Sprite>("Data/Sprite/Scene/Title/Key.png");
	sprkeyOrButton_[BUTTON]		= std::make_unique<Sprite>("Data/Sprite/Scene/Title/Button.png");

	//レベル選択
	

	//ステージ選択
	

	//スカイボックステクスチャの読み込み
	skyboxTexture_ = std::make_unique<Texture>("Data/SkyTexture/sky.jpg");
	isSpriteShader_ = true;
	skyboxRenderer_ = std::make_unique<Sprite>(isSpriteShader_);

	//フォント
	sprFont_ = std::make_unique<Sprite>("Data/Sprite/number.png");
	sprStage_ = std::make_unique<Sprite>("Data/Sprite/Scene/Title/Selects/stage.png");
	sprLevel_ = std::make_unique<Sprite>("Data/Sprite/Scene/Title/Selects/Level.png");

	sprArrow_ = std::make_unique<Sprite>("Data/Sprite/Scene/Title/Selects/Arrow.png");
	sprFrame_ = std::make_unique<Sprite>("Data/Sprite/Scene/Title/Selects/frame.png");
}

//終了化
void SceneTitle::Finalize()
{
	//ステージ終了化
	SafeDelete(stage_);

	//プレイヤー終了化
	SafeDelete(player_);

	//カメラ終了化
	SafeDelete(cameraController_);
}

//更新処理
void SceneTitle::Update(float elapsedTime)
{
	bgmTitle_->Play(true);

	//コントローラーが繋がれていたら表示するUIを変更
	ChangeUi();

	//スカイボックスの設定をマルチスレッドで衝突を起こさない用Initializeでは無く
	//Updateで設定する
	ThreadCover();

	//カメラ処理
	cameraController_->setCameraRange(5);
	target_ = player_->GetPosition();
	target_.x += 5.0f;
	target_.y += 0.5f;
	target_.z -= 8.0f;
	cameraController_->SetTarget(target_);
	cameraController_->Update(elapsedTime);

	//ステージ更新処理
	stage_->Update(elapsedTime);

	//プレイヤー更新処理
	player_->Update(elapsedTime);

	//スプライトのパラメータ更新処理
	SpriteParameterUpdate(elapsedTime);
		
	GamePad& gamePad = Input::Instance().GetGamePad();
	//ボタンを押してステージ選択へ
	switch (titleState_)
	{
		//タイトル画面
	case 0:
		if (gamePad.GetButtonDown() & GamePad::BTN_ENTER  ||
			gamePad.GetButtonDown() & GamePad::BTN_PAD_B)
		{
			titleState_++;
			seStart_->Play(false);
		}
		break;

		//ステージ選択画面
	case 1:
		flashingTime_++;

	
		//ステージ選択内部処理
		StageSelect();
	
		//現在のステージ数保存
		nowStage_ = stageSelect_ + MAX_STAGE * stageLevelSelect_;

		//シーン切り替え
		if (gamePad.GetButtonDown() & GamePad::BTN_ENTER ||
			gamePad.GetButtonDown() & GamePad::BTN_PAD_B)
		{
			isSeFlag_ = true;
			
		}
		//SEを鳴らす
		if (isSeFlag_)
		{
			seStart_->Play(false);
			sePlayTimer_ += 0.1f;
		}
		if (sePlayTimer_ > 2.5f)
		{
			bgmTitle_->Stop();
			//ボタンを押したらローディングシーン切り替え
			SceneManager::Instance().SetStageNum(nowStage_);
			SceneManager::Instance().SetNextScene(NEXT_SCENE_GAME);
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
		}

		break;

	}

}

//UI描画処理
void SceneTitle::UiRender(ID3D11DeviceContext* dc)
{
	//Titleの後ろ
	sprGradationBack_->Render(dc,
		titlePos_.x, titlePos_.y,			//座標
		titleScale_.x, titleScale_.y,		//スケール
		titleTexPos_.x, titleTexPos_.y,		//テクスチャ基準位置
		PIXEL_512, PIXEL_128,				//テクスチャの大きさ
		0,									//角度
		1, 1, 1, 1);						//色(rgb)不透明度(a)

	//タイトル
	sprGameTitle_->Render(dc,
		titlePos_.x, titlePos_.y,			//座標
		titleScale_.x, titleScale_.y,		//スケール
		0, 0, 								//テクスチャ基準位置
		PIXEL_512, PIXEL_128,				//テクスチャの大きさ
		0,									//角度
		1, 1, 1, 1);						//色(rgb)不透明度(a)

	//半分のライン
	sprBack_->Render(dc,
		0, SCREEN_HEIGHT * 0.5,				//座標
		SCREEN_WIDTH, SCREEN_HEIGHT,		//スケール
		0, 0, 								//テクスチャ基準位置
		SCREEN_WIDTH, SCREEN_HEIGHT,		//テクスチャの大きさ
		0, 									//角度
		1, 1, 1, 1);						//色(rgb)不透明度(a)

	//タイトルスプライト描画
	switch (titleState_)
	{
	case 0: //タイトル描画

		//エンターかBボタンか
		sprPushKeyButton_[keyOrButton_]->Render(dc,
			keyOrButtonPos_.x, keyOrButtonPos_.y,					//座標
			keyOrButtonScale_.x, keyOrButtonScale_.y,				//スケール
			0, 0, 													//テクスチャ基準位置
			PIXEL_512, PIXEL_512,									//テクスチャの大きさ
			0,														//角度
			1, 1, 1, 1);											//色(rgb)不透明度(a)

		//Push
		sprPush_->Render(dc,
			pushPos_.x, pushPos_.y,									//座標
			pushScale_.x, pushScale_.y,								//スケール
			0, 0, 													//テクスチャ基準位置
			PIXEL_512, PIXEL_128,									//テクスチャの大きさ
			0,														//角度
			1, 1, 1, 1);											//色(rgb)不透明度(a)

		//KeyOrButton
		sprkeyOrButton_[keyOrButton_]->Render(dc,
			StringKeyOrButtonPos_.x, StringKeyOrButtonPos_.y,		//座標
			StringKeyOrButtonScale_.x, StringKeyOrButtonScale_.y,	//スケール
			0, 0, 													//テクスチャ基準位置
			PIXEL_512, PIXEL_128,									//テクスチャの大きさ
			0,														//角度
			1, 1, 1, 1);											//色(rgb)不透明度(a)


		break;

	case 1:	//ステージ選択描画
		//ライン
		sprLine_->Render(dc,
			0, posY_,				//座標
			SCREEN_WIDTH, 220,		//スケール
			0, 0, 					//テクスチャ基準位置
			SCREEN_WIDTH, 190,		//テクスチャの大きさ
			0,						//角度
			1, 1, 1, 1);			//色(rgb)不透明度(a)
		
		//レベル選択
		{

			//レベル文字
			sprLevel_->Render(dc,
				Text1PosX_, Text1PosY_,				//座標
				PIXEL_256, PIXEL_128,				//スケール
				0, 0,								//テクスチャ基準位置
				PIXEL_256, PIXEL_128,				//テクスチャの大きさ
				0,									//角度
				1, 1, 1, 1);						//色(rgb)不透明度(a)

			//レベル数字
			sprFont_->Render(dc,
				font1PosX_, font1PosY_,				//座標
				64, 64,								//スケール
				fontTexPosX2_, 0,					//テクスチャ基準位置
				64, 64,								//テクスチャの大きさ
				0,									//角度
				1, 1, 1, 1);						//色(rgb)不透明度(a)
			//レベル数字フレーム
			sprFrame_->Render(dc,
				font1PosX_ - 32, font1PosY_ - 32,	//座標
				128, 128,							//スケール
				0, 0,								//テクスチャ基準位置
				128, 128,							//テクスチャの大きさ
				0,									//角度
				1, 1, 1, 1);						//色(rgb)不透明度(a)

			//矢印下
			if(isArrowDown_ && flashingTime_ / 24 % 2)
				sprArrow_->Render(dc,					
					arrowLevelPosX_, arrowLevelPosY1_,		//座標
					64, 64,									//スケール
					0, 0,									//テクスチャ基準位置
					64, 64,									//テクスチャの大きさ
					180,									//角度
					1, 1, 1, 1);							//色(rgb)不透明度(a)
			//矢印上
			if (isArrowUp_ && flashingTime_ / 24 % 2)
				sprArrow_->Render(dc,
					arrowLevelPosX_, arrowLevelPosY2_,		//座標
					64, 64,									//スケール
					0, 0,									//テクスチャ基準位置
					64, 64,									//テクスチャの大きさ
					0,										//角度
					1, 1, 1, 1);							//色(rgb)不透明度(a)
		}
		//ステージ選択
		{
			//ステージ文字
			sprStage_->Render(dc,
				Text2PosX_, Text2PosY_,					//座標
				PIXEL_256, PIXEL_128,					//スケール
				0, 0,									//テクスチャ基準位置
				PIXEL_256, PIXEL_128,					//テクスチャの大きさ
				0,										//角度
				1, 1, 1, 1);							//色(rgb)不透明度(a)

			//ステージ数字
			sprFont_->Render(dc,
				font2PosX_, font2PosY_,					//座標
				64, 64,									//スケール
				fontTexPosX_, 0,						//テクスチャ基準位置
				64, 64,									//テクスチャの大きさ
				0,										//角度
				1, 1, 1, 1);							//色(rgb)不透明度(a)
			//ステージ数字フレーム
			sprFrame_->Render(dc,
				font2PosX_ - 32, font2PosY_ - 32,		//座標
				128, 128,								//スケール
				0, 0,									//テクスチャ基準位置
				128, 128,								//テクスチャの大きさ
				0,										//角度
				1, 1, 1, 1);							//色(rgb)不透明度(a)


			//矢印左
			if(isArrowLeft_ && flashingTime_ / 24 % 2)
				sprArrow_->Render(dc,
					arrowStagePosX1_ - 64, arrowStagePosY_,		//座標
					64, 64,										//スケール
					0, 0,										//テクスチャ基準位置
					64, 64,										//テクスチャの大きさ
					270,										//角度
					1, 1, 1, 1);								//色(rgb)不透明度(a)
			//矢印右
			if(isArrowRight_ && flashingTime_ / 24 % 2)
				sprArrow_->Render(dc,
					arrowStagePosX2_, arrowStagePosY_,			//座標
					64, 64,										//スケール
					0, 0,										//テクスチャ基準位置
					64, 64,										//テクスチャの大きさ
					90,											//角度
					1, 1, 1, 1);								//色(rgb)不透明度(a)
		}

		break;
	}
}

//描画処理
void SceneTitle::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	//画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f }; //RGBA(0.0~1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };

	// カメラパラメータ設定
	Camera& camera = Camera::Instance();
	rc.deviceContext = dc;
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();
	rc.screenSize.x = graphics.GetScreenWidth();
	rc.screenSize.y = graphics.GetScreenHeight();
	rc.screenSize.z = camera.GetNearZ();
	rc.screenSize.w = camera.GetFarZ();
	rc.viewPosition.x = camera.GetEye().x;
	rc.viewPosition.y = camera.GetEye().y;
	rc.viewPosition.z = camera.GetEye().z;
	rc.viewPosition.w = 1;

	//スカイボックス描画
	{

		SpriteShader* shader = graphics.GetSpriteShader(SpriteShaderId::Skybox);
		shader->Begin(rc);

		shader->Draw(rc, skyboxRenderer_.get());

		shader->End(rc);

	}

	// 3Dモデル描画
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);

		//ステージ描画
		stage_->Render(dc, shader);
		//プレイヤー描画
		player_->Render(dc, shader);

		shader->End(dc);
	}

	//2Dスプライト描画
	{
		UiRender(dc);
	}

	//ImGui描画
#ifdef _DEBUG
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Title", nullptr, ImGuiTreeNodeFlags_None))
	{
		//UI配置調整
#if 1
		if (ImGui::CollapsingHeader("pos", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::SliderFloat("font1PosX", &font1PosX_, 0, 1280);
			//ImGui::SliderFloat("font1PosY", &font1PosY_, 0, 1280);
			ImGui::SliderFloat("font2PosX", &font2PosX_, 0, 1280);
			//ImGui::SliderFloat("font2PosY", &font2PosY_, 0, 720);
			ImGui::SliderFloat("Text1PosX", &Text1PosX_, 0, 1280);
			//ImGui::SliderFloat("Text1PosY", &Text1PosY_, 0, 720);
			ImGui::SliderFloat("Text2PosX", &Text2PosX_, 0, 1280);
			//ImGui::SliderFloat("Text2PosY", &Text2PosY_, 0, 720);

			ImGui::SliderFloat("arrowLevelPosX ", &arrowLevelPosX_, 0, 1280);
			ImGui::SliderFloat("arrowLevelPosY1", &arrowLevelPosY1_, 0, 720);
			ImGui::SliderFloat("arrowLevelPosY2", &arrowLevelPosY2_, 0, 720);
			ImGui::SliderFloat("arrowStagePosX1", &arrowStagePosX1_, 0, 1280);
			ImGui::SliderFloat("arrowStagePosX2", &arrowStagePosX2_, 0, 1280);
			ImGui::SliderFloat("arrowStagePosY ", &arrowStagePosY_, 0, 720);

		}

		

#endif

		if (ImGui::CollapsingHeader("stageSelect", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::InputInt("stage", &nowStage_);
		}
		if (ImGui::CollapsingHeader("levelSelect", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::InputInt("level", &stageLevelSelect_);
		}

	}
	ImGui::End();
#else
#endif
}


//ステージ選択
void SceneTitle::StageSelect()
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	//右に移動した(ステージ数は上がる)
	if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT ||
		gamePad.GetButtonDown() & GamePad::BTN_D)
	{
		//上限値以内
		if (stageSelect_ < MAX_STAGE - 1)
		{
			fontTexPosX_ = 64 * (stageSelect_ + 2);
			seSelect_->Stop();
			seSelect_->Play(false);
		}
		stageSelect_++;
		//上限
		if (stageSelect_ > MAX_STAGE - 1)
		{
			stageSelect_ = MAX_STAGE - 1;	//これ以上動かない
			fontTexPosX_ = 64 * (MAX_STAGE);
		}
		
		
	}

	//左に移動(ステージ数は下がる)
	if (gamePad.GetButtonDown() & GamePad::BTN_LEFT ||
		gamePad.GetButtonDown() & GamePad::BTN_A)
	{
		//下限値以内
		if (stageSelect_ > 0)
		{
			fontTexPosX_ = 64 * stageSelect_ ;
			seSelect_->Stop();
			seSelect_->Play(false);
		}
		stageSelect_--;
		//下限
		if (stageSelect_ < 0)
		{
			stageSelect_ = 0;
			fontTexPosX_ = 64;
		}
	
	}

	//下に移動(ステージのレベルが上がる)
	if (gamePad.GetButtonDown() & GamePad::BTN_DOWN ||
		gamePad.GetButtonDown() & GamePad::BTN_S)
	{
		//上限値以内
		if (stageLevelSelect_ < MAX_STAGE_LEVEL - 1)
		{
			fontTexPosX2_ = 64 * (stageLevelSelect_ + 2);
			seSelect_->Stop();
			seSelect_->Play(false);
		}
		stageLevelSelect_++;
		//上限
		if (stageLevelSelect_ > MAX_STAGE_LEVEL - 1)
		{
			stageLevelSelect_ = MAX_STAGE_LEVEL - 1;	//これ以上動かない

			fontTexPosX2_ = 64 * MAX_STAGE_LEVEL;
		}
	
	}
	//上に移動(ステージのレベルが下がる)
	if (gamePad.GetButtonDown() & GamePad::BTN_UP ||
		gamePad.GetButtonDown() & GamePad::BTN_W)
	{
		//下限値以内
		if (stageLevelSelect_ > 0)
		{
			fontTexPosX2_ = 64 ;
			seSelect_->Stop();
			seSelect_->Play(false);
		}
		stageLevelSelect_--;
		//下限
		if (stageLevelSelect_ < 0)
		{
			stageLevelSelect_ = 0;
			fontTexPosX2_ = 64;
		}
	}



	//矢印表示
	if (stageSelect_ == MAX_STAGE - 1) isArrowRight_ = false;
	else isArrowRight_ = true;

	//矢印表示
	if (stageSelect_ == 0) isArrowLeft_ = false;
	else isArrowLeft_ = true;

	if (stageLevelSelect_ == MAX_STAGE_LEVEL - 1) isArrowDown_ = false;
	else isArrowDown_ = true;

	if (stageLevelSelect_ == 0) isArrowUp_ = false;
	else isArrowUp_ = true;

}

//マルチスレッド衝突回避用関数
void SceneTitle::ThreadCover()
{
	if (threadCover_ == 0)
	{
		Graphics& graphics = Graphics::Instance();
		skyboxRenderer_->SetShaderResourceView(skyboxTexture_->GetShaderResourceView(),
			skyboxTexture_->GetWidth(), skyboxTexture_->GetHeight());
		//render関数で呼ぶと3Dモデルより前に描画されてしまう
		skyboxRenderer_->Update(0, 0, graphics.GetScreenWidth(), graphics.GetScreenHeight(),
			0, 0, static_cast<float>(skyboxTexture_->GetWidth()), static_cast<float>(skyboxTexture_->GetHeight()),
			0,
			1, 1, 1, 1);

		threadCover_++;
	}
}


//スプライトのパラメータ更新処理
void SceneTitle::SpriteParameterUpdate(float elapsedTime)
{
	//タイトル文字の後ろのグラデーション用画像のスクロール
	titleTexPos_.x += 3;
	//画像横サイズより
	if (titleTexPos_.x > 1526)titleTexPos_.x = 0;
}

//UI変更処理
void SceneTitle::ChangeUi()
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetControllerState())
	{
		keyOrButton_ = BUTTON;
	}
	else
	{
		keyOrButton_ = KEY;
	}
}