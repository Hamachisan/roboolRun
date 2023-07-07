#include "SceneGame.h"
#include "Graphics/Graphics.h"
#include "Camera.h"
#include "Character/Enemys/EnemyManager.h"
#include "Character/Enemys/EnemySutterObj.h"
#include "Character/Enemys/EnemySquatObj.h"
#include "Character/Enemys/EnemyJumpObj.h"
#include "Character/Enemys/EnemyImmortalObj.h"

#include "Character/BackObj/BackObjManager.h"
#include "Character/BackObj/RockObj.h"
#include "Character/BackObj/TimingObj.h"
#include "Character/BackObj/GoalObj.h"

#include "EffectManager.h"

#include "SceneClear.h"
#include "SceneManager.h"
#include "SceneLoading.h"

// 初期化
void SceneGame::Initialize()
{
	//ステージ初期化
	stage_ = new Stage();

	//プレイヤー初期化
	player_ = new Player();

	//ポーズ初期化
	pause_ = new Pause();

	//スコア初期化
	Score::Instance().Init();

#ifdef _DEBUG
#else
#endif
	//背景オブジェクト生成
	//※BackObjManagerに登録するのは必ずRockObjを一番始めに
	//左列
	for (int i = 0; i < BACK_OBJ_MAX; i++)
	{
		RockObj* rock = new RockObj();
		rock->SetPosition(DirectX::XMFLOAT3(STANDARD_POTITION_X - 20, 0, START_LINE + i * BACK_OBJ_INTERVAL_Z));
		//登録
		BackObjManager::Instance().Register(rock);
	}
	//右列
	for (int i = 0; i < BACK_OBJ_MAX; i++)
	{
		RockObj* rock = new RockObj();
		rock->SetPosition(DirectX::XMFLOAT3(STANDARD_POTITION_X + INTERVAL_X * 3 + 10, 0, START_LINE + i * BACK_OBJ_INTERVAL_Z));
		//登録
		BackObjManager::Instance().Register(rock);
	}

	//ステージに敵オブジェクト配置
	EnemySetStage();

	//ゴールのObj
	GoalObj* goal = new GoalObj();
	goal->SetPosition(DirectX::XMFLOAT3(0, 0, GOAL_LINE));
	BackObjManager::Instance().Register(goal);

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

	//フォント
	Textsprites_[0] = std::make_unique<Sprite>(".\\Data\\Font\\font4.png");

	//スプライトの初期化
	SpriteInitialize();	
	//パラメーター初期化
	ParameterInitialize();

	//光源初期化
	directionalLight_ = std::make_unique<Light>(LightType::Directional);

	//Audio初期化
	bgmGame_		= Audio::Instance().LoadAudioSource("Data/Audio/BGM/Game/saiba.wav");
	seCountDown_	= Audio::Instance().LoadAudioSource("Data/Audio/SE/Scene/Countdown01.wav");
	seFinish_		= Audio::Instance().LoadAudioSource("Data/Audio/SE/Scene/finish.wav");

	//スカイボックステクスチャの読み込み
	skyboxTexture_ = std::make_unique<Texture>("Data/SkyTexture/sky.jpg");
	isSpriteShader_ = true;
	skyboxRenderer_ = std::make_unique<Sprite>(isSpriteShader_);

	//オフスクリーンレンダリング
	sprSkyTexture_ = std::make_unique<Texture>("Data/SkyTexture/skyTexture.png");
	sprite_ = std::make_unique<Sprite>(1);
	//新しい描画ターゲットの生成
	{
		renderTarget_ = std::make_unique<RenderTarget>(static_cast<UINT>(graphics.GetScreenWidth()), 
			static_cast<UINT>(graphics.GetScreenHeight()), 
			DXGI_FORMAT_R8G8B8A8_UNORM);
	}

}

//パラメーター初期化
void SceneGame::ParameterInitialize()
{
	gameStartTime_ = 3.0f;

	//Go!!
	GoPosX_ = 1280 * 0.48 - 256;


	//グラデーション用変数
	rStart_ = 0;		//緑
	gStart_ = 1;
	bStart_ = 0;
	rEnd_ = 1;			//赤
	gEnd_ = 0;
	bEnd_ = 0;

	//光源関連初期化
	ambientLightColor_		= { 0.3, 0.3, 0.3, 1 };	//黒	//ポイントライト
	pointLightColor_		= { 1, 1, 1, 1 };
	pointLightPosition_		= { 0, 0, 0, 0 };
	pointLightRange_		= 100;

}

//スプライト初期化
void SceneGame::SpriteInitialize()
{
	//スプライト
	sprTimer_				= std::make_unique<Sprite>("Data/Sprite/number.png");
	sprTimerFrame_			= std::make_unique<Sprite>("Data/Sprite/Game/timeFrame.png");
	sprTimerTen_			= std::make_unique<Sprite>("Data/Sprite/tenn.png");
	sprGo_					= std::make_unique<Sprite>("Data/Sprite/Game/GO.png");
	sprFinish_				= std::make_unique<Sprite>("Data/Sprite/Game/Finish2.png");
	//sprSpeedMeter			= std::make_unique<Sprite>("Data/Sprite/Game/gauge.png");
	sprSpeedMeter_			= std::make_unique<Sprite>();
	sprSpeedMeterFrame_		= std::make_unique<Sprite>("Data/Sprite/Game/frameBlack.png");
	sprSpeedMax_			= std::make_unique<Sprite>("Data/Sprite/Game/maxspeed.png");
	sprSpeed_				= std::make_unique<Sprite>("Data/Sprite/Game/speed.png");
	sprImpuls_				= std::make_unique<Sprite>("Data/Sprite/damage.png");

	//タイミングの精度
	sprPerfect_				= std::make_unique<Sprite>("Data/Sprite/Game/Timing/PERFECT3.png");
	sprGreat_				= std::make_unique<Sprite>("Data/Sprite/Game/Timing/GREAT.png");
	sprGood_				= std::make_unique<Sprite>("Data/Sprite/Game/Timing/GOOD.png");
	//コンボ表示
	sprCombo_				= std::make_unique<Sprite>("Data/Sprite/Game/Timing/Combo.png");
	sprComboConut_			= std::make_unique<Sprite>("Data/Sprite/number.png");
	sprComboFrame_			= std::make_unique<Sprite>("Data/Sprite/Game/Timing/comboBack.png");


	//操作方法のフレーム
	sprUiFrame_				= std::make_unique<Sprite>("Data/Sprite/Game/UI/UiFrame.png");
	//操作方法の後ろに置く
	sprUiBack_				= std::make_unique<Sprite>("Data/Sprite/Game/UI/back.png");
	//障害物の絵
	sprUiObjs_[0]			= std::make_unique<Sprite>("Data/Sprite/Game/UI/immortalObj.png");
	sprUiObjs_[1]			= std::make_unique<Sprite>("Data/Sprite/Game/UI/sutterObj.png");
	sprUiObjs_[2]			= std::make_unique<Sprite>("Data/Sprite/Game/UI/squatObj.png");
	sprUiObjs_[3]			= std::make_unique<Sprite>("Data/Sprite/Game/UI/jumpObj.png");
	//キー、ボタン配置
	sprUiOperations_[KeyBoard::Akey][Operations::keyboard]		= std::make_unique<Sprite>("Data/Sprite/Game/UI/Akey.png");
	sprUiOperations_[KeyBoard::Dkey][Operations::keyboard]		= std::make_unique<Sprite>("Data/Sprite/Game/UI/Dkey.png");
	sprUiOperations_[KeyBoard::Jkey][Operations::keyboard]		= std::make_unique<Sprite>("Data/Sprite/Game/UI/Jkey.png");
	sprUiOperations_[KeyBoard::Kkey][Operations::keyboard]		= std::make_unique<Sprite>("Data/Sprite/Game/UI/Kkey.png");
	sprUiOperations_[KeyBoard::Lkey][Operations::keyboard]		= std::make_unique<Sprite>("Data/Sprite/Game/UI/Lkey.png");
	sprUiOperations_[Pad::Left][Operations::gamePad]			= std::make_unique<Sprite>("Data/Sprite/Game/UI/LeftButton.png");
	sprUiOperations_[Pad::Right][Operations::gamePad]			= std::make_unique<Sprite>("Data/Sprite/Game/UI/RightButton.png");
	sprUiOperations_[Pad::Xbutton][Operations::gamePad]			= std::make_unique<Sprite>("Data/Sprite/Game/UI/Xbutton.png");
	sprUiOperations_[Pad::Abutton][Operations::gamePad]			= std::make_unique<Sprite>("Data/Sprite/Game/UI/Abutton.png");
	sprUiOperations_[Pad::Bbutton][Operations::gamePad]			= std::make_unique<Sprite>("Data/Sprite/Game/UI/Bbutton.png");

}

//敵をステージに配置
void SceneGame::EnemySetStage()
{
	//エネミーの配置配列初期化
	enemyArrangement_ = new EnemyArrangement(SceneManager::Instance().GetStageNum());

	//エネミーマネージャー初期化
	EnemyManager& eManager = EnemyManager::Instance();

	//エネミー配置
	for (int i = 0; i < ENEMY_LINE; i++)
	{
		for (int j = 0; j < ENEMY_COLUMN; j++)
		{
			//ステージ選択
			stageLevel_ = enemyArrangement_->stage_[i][j].num;

			//生成するエネミー
			switch (stageLevel_)
			{
			case 0:	//破壊不能
			{
				EnemyImmortalObj* immortal = new EnemyImmortalObj();
				immortal->SetPosition(DirectX::XMFLOAT3(STANDARD_POTITION_X + INTERVAL_X * j, 0, OBJ_START_LINE + i * INTERVAL_Z));
				eManager.Register(immortal);
			}
			break;
			case 1:	//吹っ飛ばし
			{
				EnemySutterObj* sutter = new EnemySutterObj();
				sutter->SetPosition(DirectX::XMFLOAT3(STANDARD_POTITION_X + INTERVAL_X * j, 0, OBJ_START_LINE + i * INTERVAL_Z));
				eManager.Register(sutter);

				TimingObj* timing = new TimingObj();
				timing->SetPosition(DirectX::XMFLOAT3(STANDARD_POTITION_X + INTERVAL_X * j, 0, OBJ_START_LINE + i * INTERVAL_Z - 11.0f));
				//登録
				BackObjManager::Instance().Register(timing);
			}
			break;
			case 2:	//スライディング
			{
				EnemySquatObj* squat = new EnemySquatObj();
				squat->SetPosition(DirectX::XMFLOAT3(STANDARD_POTITION_X + INTERVAL_X * j, 0, OBJ_START_LINE + i * INTERVAL_Z));
				eManager.Register(squat);

				TimingObj* timing = new TimingObj();
				timing->SetPosition(DirectX::XMFLOAT3(STANDARD_POTITION_X + INTERVAL_X * j, 0, OBJ_START_LINE + i * INTERVAL_Z - 4.0f));
				//登録
				BackObjManager::Instance().Register(timing);
			}
			break;
			case 3:	//ジャンプ
			{
				EnemyJumpObj* jump = new EnemyJumpObj();
				jump->SetPosition(DirectX::XMFLOAT3(STANDARD_POTITION_X + INTERVAL_X * j, 0, OBJ_START_LINE + i * INTERVAL_Z));
				eManager.Register(jump);

				TimingObj* timing = new TimingObj();
				timing->SetPosition(DirectX::XMFLOAT3(STANDARD_POTITION_X + INTERVAL_X * j, 0, OBJ_START_LINE + i * INTERVAL_Z - 9.5f));
				//登録
				BackObjManager::Instance().Register(timing);
			}
			break;
			case 4:	//配置しない
				break;
			}


		}
	}
}

// 終了化
void SceneGame::Finalize()
{
	//ステージ終了化
	SafeDelete(stage_);

	//プレイヤー終了化
	SafeDelete(player_);

	//エネミーの配置配列終了化
	SafeDelete(enemyArrangement_);

	//エネミー終了化
	EnemyManager::Instance().Clear();

	//背景オブジェクト終了化
	BackObjManager::Instance().Clear();
	//カメラコントローラー終了化
	SafeDelete(cameraController_);
	
	//ポーズ終了化
	SafeDelete(pause_);
}

// 更新処理
void SceneGame::Update(float elapsedTime)
{
	//コントローラーが繋がれていたら表示するUIを変更
	ChangeUi();

	//スカイボックスの設定をマルチスレッドで衝突を起こさない用Initializeでは無く
	//Updateで設定する
	ThreadCover();


	//ポーズ処理
	if (!PauseProcess(elapsedTime))
		return;
	
	//ゲームをスタートする為のタイマー
	gameStartTime_ -= elapsedTime;

	///ゲーム開始
	if (gameStartTime_ < 0)
	{
		//プレイヤーの待機アニメーションを終了
		player_->SetGameStatrtPlayer(true);

		//BGM再生
		bgmGame_->Play(true, 0.5f);
		
		//ゲームクリアしていないとき
		if (!player_->StageClear())
		{
			gameTimer_ += elapsedTime;

			//フリーカメラ起動
			if (isFreeCamera_)
			{
				FreeCamera(elapsedTime);
				target_ = freeCameraPosition_;
			}
			//起動していない
			else 
			{
				//カメラコントローラー更新処理
				target_ = player_->GetPosition();
				target_.y += 3.5f;
				freeCameraPosition_ = player_->GetPosition();

				//プレイヤー更新処理
				animState_ = player_->GetPlayerAnimations();
				player_->Update(elapsedTime);
				//ポイントライトの移動
				pointLightPosition_.x = 0;
				pointLightPosition_.y = 50.0f;
				pointLightPosition_.z = player_->GetPosition().z - 4;
				pointLightPosition_.w = 0;
			}

			cameraController_->SetTarget(target_);
			cameraController_->Update(elapsedTime);

			//ステージ更新処理
			stage_->Update(elapsedTime);

			//エネミー更新処理
			EnemyManager::Instance().Update(elapsedTime);

			//背景オブジェクト更新処理
			BackObjManager::Instance().Update(elapsedTime);

			//エフェクト更新処理
			EffectManager::Instane().Update(elapsedTime);

			//スプライトのパラメータ更新処理
			SpriteParameterUpdate(elapsedTime);

			//衝突と同時にカメラシェイクを行う
			if (player_->GetCameraShakeFlag())
			{
				cameraController_->SetCameraShake(true);
			}
			else
			{
				cameraController_->SetCameraShake(false);
			}

			//背景オブジェクトの移動
			MoveBackObject();
		}
		//クリアした時
		else
		{
			//カメラの更新を辞めてクリア処理をする

			gameClearTime_ += elapsedTime;

			//スコア設定
			Score::Instance().SetClearTime(gameTimer_);
			Score::Instance().SetMaxCombo(player_->GetMaxCombo());
			Score::Instance().SetPerfectConut(player_->perfectCount_);
			Score::Instance().SetGreatCount(player_->greatCount_);
			Score::Instance().SetGoodCount(player_->goodCount_);

			//ステージ更新処理
			stage_->Update(elapsedTime);

			//プレイヤー更新処理
			animState_ = player_->GetPlayerAnimations();
			player_->Update(elapsedTime);

			//エネミー更新処理
			EnemyManager::Instance().Update(elapsedTime);

			//背景オブジェクト更新処理
			BackObjManager::Instance().Update(elapsedTime);

			//エフェクト更新処理
			EffectManager::Instane().Update(elapsedTime);

			//効果音再生
			if(seCount_ == 0)
				seFinish_->Play(false);
			seCount_++;

			//2秒後結果画面へ遷移させる
			if (gameClearTime_ > 2.0f)
			{
				constexpr float CLEAR_DAMAGE = 10.0f;
				constexpr float INVINCIBLE = 0.2f;

				bgmGame_->Stop();
				
				//背景オブジェクトを消す処理
				int objCount = BackObjManager::Instance().GetBackObjCount();
				for (int i = 0; i < objCount; ++i)
				{
					BackObj* back = BackObjManager::Instance().GetBackObj(i);
					back->ApplyDmaege(CLEAR_DAMAGE, INVINCIBLE);
				}
				//一回でもerase処理を通すためにUpdateを入れる
				BackObjManager::Instance().Update(elapsedTime);

				//シーン遷移
				SceneManager::Instance().SetNextScene(NEXT_SCENE_OTHER);
				SceneManager::Instance().ChangeScene(new SceneLoading(new SceneClear));
			}
		}



	}
	//ゲーム開始前の処理
	else
	{
		//タイマー(カウントダウン)描画処理
		//1の位
		timerTexPosX_[0] = (int)gameStartTime_ * texSize_ + 64;

		//待機アニメーションを更新
		player_->Update(elapsedTime);
		
		//カウントダウン音
		seCountDown_->Play(false);

		//1フレームだけUpdateを通しステージがカウントダウン時でも見えるように
		if(oneFrameCount == 0)
			OneFrameUpdate(elapsedTime);
		oneFrameCount++;


	}

	//カウントダウンのSEが2回なってしまうのを阻止
	if (gameStartTime_ < -1.0f && gameStartTime_ > -2.0f)
	{
		seCountDown_->Stop();
	}

}

// 描画処理 (まとめ)
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	//ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	//ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// 画面クリア＆レンダーターゲット設定
	//FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
	//dc->ClearRenderTargetView(rtv, color);
	//dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//dc->OMSetRenderTargets(1, &rtv, dsv);


	// 描画処理
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ライト方向（下方向）

	//カメラパラメータ設定	
	//こっち使う
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


	// 3D空間の描画を別のバッファに対して行う
	Render3DScene();

	// 書き込み先をバックバッファに変えてオフスクリーンレンダリングの結果を描画する
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();
	// 画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	//色調補正
	SpriteShader* shader = graphics.GetSpriteShader(SpriteShaderId::ColorGrading);

	//色調補正もしくは放射ブラー
	if (!isSpriteShaderSet_)
	{

		rc.colorGradingData = colorGradingData_;
		shader->Begin(rc);
		shader->Draw(rc, sprite_.get());	//ここのDrowは3Dモデルを描画している
		shader->End(rc);
	}
	else
	{
		//放射ブラー
		shader = graphics.GetSpriteShader(SpriteShaderId::RadialBlur);
		rc.radialBlurData = radialBlurData_;
		shader->Begin(rc);
		shader->Draw(rc, sprite_.get());	//ここのDrowは3Dモデルを描画している
		shader->End(rc);
	}


	// 2Dスプライト描画
	{
		UiRender(dc);

		//ポーズ画面描画
		if (isPause_)
			pause_->Render(dc);

	}



#ifdef _DEBUG
	// 3Dデバッグ描画
	{
		//プレイヤーデバッグプリミティブ描画
		player_->DrawDebugPrimitive();

		//エネミーデバッグプリミティブ描画
		EnemyManager::Instance().DrawDebugPrimitive();

		// ラインレンダラ描画実行
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// デバッグレンダラ描画実行
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);

		//こっちがちゃんとしたライトのImGui
		directionalLight_->DrawDebugGUI();
	}
#else
#endif


#ifdef _DEBUG
	// 2DデバッグGUI描画
	{
		//プレイヤーデバッグ描画
		player_->DrawDebugGUI();

		ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
		if (ImGui::Begin("Game", nullptr, ImGuiTreeNodeFlags_None))
		{
			//
			if (ImGui::CollapsingHeader("target", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::SliderFloat("targetX", &target_.x, 0.0f, 10.0f);
				ImGui::SliderFloat("targetY", &target_.y, 0.0f, 10.0f);
				ImGui::SliderFloat("targetZ", &target_.z, -10.0f, 10.0f);

			}
			ImGui::SliderFloat("cameraRange", &cameraController_->range_, 0.0f, 50.0f);

			//平行光源の方向と色
			if (ImGui::CollapsingHeader("directional", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::SliderFloat("directionalX", &directional_.x, -1.0f, 1.0f);
				ImGui::SliderFloat("directionalY", &directional_.y, -1.0f, 1.0f);
				ImGui::SliderFloat("directionalZ", &directional_.z, -1.0f, 1.0f);
				ImGui::ColorEdit3("color", &ambientLightColor_.x);
			}

			//ポイントライト
			if (ImGui::CollapsingHeader("pointLight", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::SliderFloat("pointLightPosX", &pointLightPosition_.x, -500, 1000);
				ImGui::SliderFloat("pointLightPosY", &pointLightPosition_.y, -10, 50);
				ImGui::SliderFloat("pointLightPosZ", &pointLightPosition_.z, -500, 1000);
				ImGui::ColorEdit3("pointLightColor", &pointLightColor_.x);
				ImGui::SliderFloat("PointLightRange", &pointLightRange_, 0.0f, 100.0f);
			}

			//フリーカメラ
			if (ImGui::CollapsingHeader("freeCamera", ImGuiTreeNodeFlags_DefaultOpen))
			{
				DirectX::XMFLOAT3 pos = freeCameraPosition_;
				ImGui::Text("freeCameraPos %.1f, %.1f, %.1f", pos.x, pos.y, pos.z);

				ImGui::SliderFloat("range : ", &cameraController_->range_, +0.01f, +20.0f);

				ImGui::Checkbox("free_camera", &isFreeCamera_);
			}

			ImGui::Checkbox("setBlur", &isSpriteShaderSet_);

			if (ImGui::TreeNode("ColorGrading"))
			{
				ImGui::SliderFloat("hueShift", &colorGradingData_.hueShift, 0.0f, +360.0f);
				ImGui::SliderFloat("saturation", &colorGradingData_.saturation, 0.0f, +2.0f);
				ImGui::SliderFloat("brightness", &colorGradingData_.brightness, 0.0f, +2.0f);

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Blur"))
			{
				ImGui::SliderFloat("count", &radialBlurData_.sampleCount, 0.0f, +16.0f);
				ImGui::SliderFloat("_Strength ", &radialBlurData_.strength, 0.0f, +1.0f);

				ImGui::TreePop();
			}


		}
	}
	ImGui::End();
#else
#endif
}

//3D空間を描画
void SceneGame::Render3DScene()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = renderTarget_->GetRenderTargetView().Get();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// 画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// ビューポートの設定
	D3D11_VIEWPORT	vp = {};
	vp.Width = graphics.GetScreenWidth();
	vp.Height = graphics.GetScreenHeight();
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	dc->RSSetViewports(1, &vp);

	// 描画処理
	RenderContext rc;
	rc.deviceContext = dc;

	//カメラパラメータ設定	
	//こっち使う
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

	//ライト設定
	SettingLight(dc, rc);
	PlayerSettingLight(rc);

	// 3Dモデル描画
	{
		Shader* shader = graphics.GetShader();
		//shader->Begin(dc, rc);
		//shader->End(dc);


		//シェーダーの切り替え
		shader = graphics.GetPhongShader();
		shader->Begin(dc, rc);

		//エネミー描画
		EnemyManager::Instance().Render(dc, shader);

		//ステージ描画
		stage_->Render(dc, shader);

		//背景オブジェクト描画
		BackObjManager::Instance().Render(dc, shader);

		//goal->Render(dc, shader);

		//プレイヤー描画
		player_->Render(dc, shader);

		shader->End(dc);

	}

	// 3Dエフェクト描画
	{
		EffectManager::Instane().Render(rc.view, rc.projection);
	}

}

//UI描画
void SceneGame::UiRender(ID3D11DeviceContext* dc)
{
	//カウントダウンのタイマー(ゲーム未スタート)
	if (gameStartTime_ > 0)
	{
		sprTimer_->Render(dc,
			SCREEN_WIDTH * ABOUT_HALF - PIXEL_64,		//X座標
			SCREEN_HEIGHT * ABOUT_HALF - PIXEL_64, 		//Y座標
			PIXEL_128, PIXEL_128,						//画像サイズ
			timerTexPosX_[0], 0, PIXEL_64, PIXEL_64,	//画像読み込み位置と読み込みサイズ
			0,											//角度
			1, 1, 1, 1);								//色
	}
	//ゲーム中
	else
	{
		
		//GO
		sprGo_->Render(dc,
			GoPosX_,									//X座標
			SCREEN_HEIGHT * ABOUT_HALF - PIXEL_64,		//Y座標
			PIXEL_512, PIXEL_64 * 3,					//画像サイズ
			0, 0, PIXEL_512, PIXEL_256,					//画像読み込み位置と読み込みサイズ
			0,											//角度
			1, 1, 1, 1);								//色

		//タイマーの枠
		sprTimerFrame_->Render(dc,
			SCREEN_WIDTH * 0.5 - 105,					//X座標
			0.0f,										//Y座標
			PIXEL_256, PIXEL_128,						//画像サイズ
			0, 0, PIXEL_128, PIXEL_64,					//画像読み込み位置と読み込みサイズ
			0,											//角度
			1, 1, 1, 0.5);								//色

		//画面上配置する現在の時間
		{
			//10の位
			sprTimer_->Render(dc,
				SCREEN_WIDTH * ABOUT_HALF - texSize_,		//X座標
				30.0f, 										//Y座標
				PIXEL_64, PIXEL_64,							//画像サイズ
				timerTexPosX_[1], 0, PIXEL_64, PIXEL_64,	//画像読み込み位置と読み込みサイズ
				0,											//角度
				1, 1, 1, 1);								//色
			//1の位
			sprTimer_->Render(dc,
				SCREEN_WIDTH * ABOUT_HALF,					//X座標
				30.0f,										//Y座標
				PIXEL_64, PIXEL_64,							//画像サイズ
				timerTexPosX_[0], 0, PIXEL_64, PIXEL_64,	//画像読み込み位置と読み込みサイズ
				0,											//角度
				1, 1, 1, 1);								//色
			//小数点
			sprTimerTen_->Render(dc,
				SCREEN_WIDTH * ABOUT_HALF + texSize_ - 25,	//X座標
				35.0f,										//Y座標
				PIXEL_64, PIXEL_64,							//画像サイズ
				0, 0, PIXEL_64, PIXEL_64,					//画像読み込み位置と読み込みサイズ
				0,											//角度
				1, 1, 1, 1);								//色
			//少数第一位
			sprTimer_->Render(dc,
				SCREEN_WIDTH * 0.5 + PIXEL_64,				//X座標
				30.0f, 										//Y座標
				64, 64,										//画像サイズ
				timerTexPosX_[2], 0, PIXEL_64, PIXEL_64,	//画像読み込み位置と読み込みサイズ
				0,											//角度
				1, 1, 1, 1);								//色
		}
	}

	//スピードメーター
	{
		//スピード(文字)
		sprSpeed_->Render(dc,								
			20,												//X座標
			580, 											//Y座標
			768 * 0.5f, PIXEL_256 * 0.2f,					//画像サイズ
			0, 0, 768, PIXEL_256,							//画像読み込み位置と読み込みサイズ
			0,												//角度
			1, 1, 1, 1);									//色

		//スピードメーター
		sprSpeedMeter_->Render(dc,
			20,												//X座標
			630, 											//Y座標
			meterTexX_, PIXEL_256 * 0.2,					//画像サイズ
			0, 0, 768, PIXEL_256,							//画像読み込み位置と読み込みサイズ
			0,												//角度
			r_, g_, b_, 1);									//色
		
		//スピードメーターの枠
		sprSpeedMeterFrame_->Render(dc,
			20,												//X座標
			630, 											//Y座標
			768 * 0.5, PIXEL_256 * 0.2,						//画像サイズ
			0, 0, 768, PIXEL_256,							//画像読み込み位置と読み込みサイズ
			0,												//角度
			1, 1, 1, 1);									//色

		//スピードが最大に達したら表示する
		if (speed_ >= maxSpeed_)
		{
			if (flashingTime_ / 24 % 2)					
				sprSpeedMax_->Render(dc,
					20,										//X座標
					630, 									//Y座標
					768 * 0.5, PIXEL_256 * 0.2,				//画像サイズ
					0, 0, 768, PIXEL_256,					//画像読み込み位置と読み込みサイズ
					0,										//角度
					1, 1, 1, 1);							//色
		}
	}

	//操作方法のUIを表示
	{
		//プレイヤーの操作入力状況に応じて操作説明を強調する
		float posx = 0;
		switch (animState_)
		{
		case 0:	//move
			posx = 0;
			break;
		case 1: //panch
			posx = 128;
			break;
		case 2: //squat
			posx = 256;
			break;
		case 3: // jump
			posx = 384;
			break;
		}
		//プレイヤーが走行以外のアニメーションを行っている時に
		if (!player_->isAnimMove_)
		{
			//少し明るくする為のスプライト
			sprUiBack_->Render(dc,	
				posx,										//X座標
				-20, 										//Y座標
				128, 120,									//画像サイズ
				0, 0, PIXEL_512, PIXEL_256,					//画像読み込み位置と読み込みサイズ
				0, 											//角度
				1, 1, 1, 1);								//色
		}

		//左端から

		//左右移動オブジェクト
		{
			//フレーム
			sprUiFrame_->Render(dc,
				0,											//X座標		
				-20, 										//Y座標
				128, 120,									//画像サイズ
				0, 0, 128, 64,								//画像読み込み位置と読み込みサイズ
				180, 										//角度
				1, 1, 1, 1);								//色
			//障害物
			sprUiObjs_[0]->Render(dc,
				10,											//X座標
				16, 										//Y座標
				48, 48,										//画像サイズ
				0, 0, 512, 512,								//画像読み込み位置と読み込みサイズ
				0, 											//角度
				1, 1, 1, 1);								//色
			//キー、もしくはボタン
			sprUiOperations_[anyKeyOrButton_[0]][whichOperation_]->Render(dc,
				64,											//X座標
				0, 											//Y座標
				48, 48,										//画像サイズ
				0, 0, 512, 512,								//画像読み込み位置と読み込みサイズ
				0, 											//角度
				1, 1, 1, 1									//色
			);
			//キー、もしくはボタン
			sprUiOperations_[anyKeyOrButton_[1]][whichOperation_]->Render(dc,
				64,											//X座標
				48, 										//Y座標
				48, 48,										//画像サイズ
				0, 0, 512, 512,								//画像読み込み位置と読み込みサイズ
				0,											//角度
				1, 1, 1, 1									//色
			);
		}
		//吹っ飛ばしオブジェクト
		{
			//フレーム
			sprUiFrame_->Render(dc,
				128,										//X座標
				-20,										//Y座標
				128, 120,									//画像サイズ
				0, 0, 128, 64,								//画像読み込み位置と読み込みサイズ
				180,										//角度
				1, 1, 1, 1);								//色
			//障害物
			sprUiObjs_[1]->Render(dc,
				128 + 10,									//X座標
				16, 										//Y座標
				48, 48,										//画像サイズ
				0, 0, 512, 512,								//画像読み込み位置と読み込みサイズ
				0, 											//角度
				1, 1, 1, 1);								//色
			//キー、もしくはボタン
			sprUiOperations_[anyKeyOrButton_[2]][whichOperation_]->Render(dc,
				128 + 64,									//X座標
				16, 										//Y座標
				48, 48,										//画像サイズ
				0, 0, 512, 512,								//画像読み込み位置と読み込みサイズ
				0, 											//角度
				1, 1, 1, 1									//色
			);
		}
		//潜るオブジェクト
		{
			//フレーム
			sprUiFrame_->Render(dc,
				256,										//X座標
				-20, 										//Y座標
				128, 120,									//画像サイズ
				0, 0, 128, 64,								//画像読み込み位置と読み込みサイズ
				180,										//角度
				1, 1, 1, 1);								//色

			//障害物
			sprUiObjs_[2]->Render(dc,
				256 + 10,									//X座標
				16, 										//Y座標
				48, 48,										//画像サイズ
				0, 0, 512, 512,								//画像読み込み位置と読み込みサイズ
				0, 											//角度
				1, 1, 1, 1);								//色

			//キー、もしくはボタン
			sprUiOperations_[anyKeyOrButton_[3]][whichOperation_]->Render(dc,
				256 + 64,									//X座標
				16, 										//Y座標
				48, 48,										//画像サイズ
				0, 0, 512, 512,								//画像読み込み位置と読み込みサイズ
				0, 											//角度
				1, 1, 1, 1);								//色
				
		}
		//ジャンプオブジェクト
		{
			//フレーム
			sprUiFrame_->Render(dc,
				384,										//X座標
				-20, 										//Y座標
				128, 120,									//画像サイズ
				0, 0, 128, 64,								//画像読み込み位置と読み込みサイズ
				180,										//角度
				1, 1, 1, 1);								//色

			//障害物
			sprUiObjs_[3]->Render(dc,
				384 + 10,									//X座標
				16, 										//Y座標
				48, 48,										//画像サイズ
				0, 0, 512, 256,								//画像読み込み位置と読み込みサイズ
				0,											//角度
				1, 1, 1, 1);								//色
				
			//キー、もしくはボタン
			sprUiOperations_[anyKeyOrButton_[4]][whichOperation_]->Render(dc,
				384 + 64,									//X座標
				16, 										//Y座標
				48, 48,										//画像サイズ
				0, 0, 512, 512,								//画像読み込み位置と読み込みサイズ
				0, 											//角度
				1, 1, 1, 1);								//色
		}

	}

	//タイミングの精度を表示
	if (player_->isTimingRender_)
	{
		if (player_->timing_ == player_->PERFECT)
		{
			sprPerfect_->Render(dc,
				470,								//X座標
				370, 								//Y座標
				384, 96,							//画像サイズ
				0, 0, 512, 128,						//画像読み込み位置と読み込みサイズ
				0, 									//角度
				1, 1, 1, 1);						//色
		}
		if (player_->timing_ == player_->GREAT)
		{
			sprGreat_->Render(dc,
				470,								//X座標
				370, 								//Y座標
				384, 96,							//画像サイズ
				0, 0, 512, 128,						//画像読み込み位置と読み込みサイズ
				0, 									//角度
				1, 1, 1, 1);						//色
		}
		if (player_->timing_ == player_->GOOD)
		{
			sprGood_->Render(dc,
				470,								//X座標
				370, 								//Y座標
				384, 96,							//画像サイズ
				0, 0, 512, 128,						//画像読み込み位置と読み込みサイズ
				0,									//角度
				1, 1, 1, 1);						//色
		}
	}

	//コンボの枠
	sprComboFrame_->Render(dc,
		820,							//X座標
		-5, 							//Y座標
		400, 100,						//画像サイズ
		0, 0, 512, 256,					//画像読み込み位置と読み込みサイズ
		0, 								//角度
		1, 1, 1, 1);					//色

	//コンボ表示
	sprCombo_->Render(dc,
		950,							//X座標
		20, 							//Y座標
		256, 68,						//画像サイズ
		0, 0, 256, 64,					//画像読み込み位置と読み込みサイズ
		0, 								//角度
		1, 1, 1, 1);					//色
	//コンボ1の位
	sprComboConut_->Render(dc,		
		855,							//X座標
		20, 							//Y座標
		60, 60,							//画像サイズ
		comboTexPosX_[1], 0, 64, 64,	//画像読み込み位置と読み込みサイズ
		0, 								//角度
		1, 1, 1, 1);					//色
	//コンボ10の位
	sprComboConut_->Render(dc,
		914,							//X座標
		20, 							//Y座標
		60, 60,							//画像サイズ
		comboTexPosX_[0], 0, 64, 64,	//画像読み込み位置と読み込みサイズ
		0, 								//角度
		1, 1, 1, 1);					//色

	//衝突した時に赤いエフェクト表示
	if (player_->isImpulse_)
	{
		//if (ImpulsTimer < 0.5f)
		sprImpuls_->Render(dc,	
			0,								//X座標
			0,								//Y座標
			1280, 720,						//画像サイズ
			0, 0, 1920, 1080,				//画像読み込み位置と読み込みサイズ
			0,								//角度
			1, 1, 1, 1);					//色

	}

	//ゲームが終了したら
	if (player_->StageClear())
	{
		//FINISH
		sprFinish_->Render(dc,
			0,								//X座標
			0,								//Y座標
			1280, 720,						//画像サイズ
			0, 0, 1280, 720,				//画像読み込み位置と読み込みサイズ
			0,								//角度
			1, 1, 1, 1);					//色
	}
}

//背景オブジェクトを移動させる関数
void SceneGame::MoveBackObject()
{
	//オブジェクト生成数を減らしたいのでプレイヤーが通りすぎた背景オブジェクトは一番後ろに移動させる
	//背景オブジェクト生成
	//カウント
	for (int i = 0; i < BACK_OBJ_MAX * 2; ++i)
	{
		//プレイヤーと比較するオブジェクト
		BackObj* rock1 = BackObjManager::Instance().GetBackObj(i);
		//比較
		float rockPosZ = rock1->GetPosition().z;
		float playerPosZ = player_->GetPosition().z;

		//		岩2　障害物  岩5		↑
		//		岩2　障害物  岩5		↑
		//		岩2　障害物  岩5		↑
		//		岩1　障害物  岩4		↑
		//		岩1　障害物  岩4		↑
		//		岩1　障害物  岩4		↑
		//		岩0　障害物  岩3		↑	進行方向
		//		岩0　障害物　岩3		↑
		//		岩0　障害物　岩3		↑

		//左列...3個まで)
		if (BACK_OBJ_MAX > i)
		{
			if (playerPosZ > rockPosZ + BACK_OBJ_INTERVAL_Z)
			{
				//比較したオブジェクトを配置する基準にするオブジェクト
				BackObj* rock2;
				//左列一番前のオブジェクトの時は左列の一番最後のオブジェクトを取得
				if (i == LEFT_FRONT_LINE)
					rock2 = BackObjManager::Instance().GetBackObj(LEFT_FRONT_LINE + 2);
				//それ以外は一つ前のオブジェクトを取得
				else
					rock2 = BackObjManager::Instance().GetBackObj(i - 1);

				//列の最後に移動させる
				rock1->SetPosition({ rock2->GetPosition().x, rock2->GetPosition().y, rock2->GetPosition().z + BACK_OBJ_INTERVAL_Z });


			}
		}
		//右列...3個以上
		else
		{
			if (playerPosZ > rockPosZ + BACK_OBJ_INTERVAL_Z)
			{
				//比較したオブジェクトを配置する基準にするオブジェクト
				BackObj* rock2;
				//右列一番前のオブジェクトの時は右列の最後のオブジェクトを取得
				if (i == RIGHT_FRONT_LINE)
					rock2 = BackObjManager::Instance().GetBackObj(RIGHT_FRONT_LINE + 2);
				//それ以外は一つ前のオブジェクトを取得
				else
					rock2 = BackObjManager::Instance().GetBackObj(i - 1);

				//列の最後に移動させる
				rock1->SetPosition({ rock2->GetPosition().x, rock2->GetPosition().y, rock2->GetPosition().z + BACK_OBJ_INTERVAL_Z });

			}
		}

	}
}

//ライト関係の設定
void SceneGame::SettingLight(ID3D11DeviceContext* dc, RenderContext& rc)
{
	Camera& camera = Camera::Instance();

	//パラメータ設定
	//環境光(今は未使用)
	rc.ambientLightColor = ambientLightColor_;
	//カメラの位置
	rc.viewPosition.x = camera.GetEye().x;
	rc.viewPosition.y = camera.GetEye().y;
	rc.viewPosition.z = camera.GetEye().z;
	rc.viewPosition.w = 1;
	//ライト方向
	rc.directionalLight.direction.x = directionalLight_->GetDirection().x;
	rc.directionalLight.direction.y = directionalLight_->GetDirection().y;
	rc.directionalLight.direction.z = directionalLight_->GetDirection().z;
	rc.directionalLight.direction.w = 0;
	//ライトの色
	rc.directionalLight.color = directionalLight_->GetColor();

	//ポイントライト
	//取り敢えず1つだけ使う
	rc.pointLight[0].color = pointLightColor_;
	rc.pointLight[0].position = pointLightPosition_;
	rc.pointLight[0].range = pointLightRange_;
	rc.pointLight[0].dummy = { 0, 0, 0 };
	for (int i = 3; i < 8; i++)
	{
		rc.pointLight[i].color = { 1, 1, 1, 1 };
		rc.pointLight[i].position = pointLightPosition_;
		rc.pointLight[i].range = 0;
		rc.pointLight[i].dummy = {0, 0, 0};

	}
	memcpy_s(rc.pointLight, sizeof(rc.pointLight), rc.pointLight, sizeof(rc.pointLight));

}

//プレイヤーアニメーション時にプレイヤーにライトを当てる
void SceneGame::PlayerSettingLight(RenderContext& rc) 
{
	DirectX::XMFLOAT4 pos;
	pos.x = player_->GetPosition().x;
	pos.y = player_->GetPosition().y + 5;
	pos.z = player_->GetPosition().z;
	pos.w = 0;
	rc.pointLight[1].position = pos;
	rc.pointLight[1].range = 8;
	rc.pointLight[1].dummy = { 0, 0, 0 };
	rc.pointLight[2].position = pos;
	rc.pointLight[2].range = 8;
	rc.pointLight[2].dummy = { 0, 0, 0 };

	switch (animState_)
	{
	case 0:	//move
		rc.pointLight[1].color =  { 1, 1, 1, 1 };//白
		rc.pointLight[2].color =  { 1, 1, 1, 1 };//白
		break;
	case 1: //blow
		rc.pointLight[1].color = { 1, 0, 0, 1 };//赤
		rc.pointLight[2].color = { 1, 0, 0, 1 };//赤
		break;
	case 2: //squat
		rc.pointLight[1].color = { 0, 1, 0, 1 };//緑
		rc.pointLight[2].color = { 0, 1, 0, 1 };//緑
		break;
	case 3: // jump
		rc.pointLight[1].color = { 0, 0, 1, 1 };//青
		rc.pointLight[2].color = { 0, 0, 1, 1 };//青
		break;
	}
	if (animState_ == 0)
	{
		rc.pointLight[1].range = 0;
		rc.pointLight[2].range = 0;
	}
}

//フリーカメラを動かすための処理
DirectX::XMFLOAT3 SceneGame::GetMoveVec() const
{
	// 入力情報を取得
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisLX();
	float ay = gamePad.GetAxisLY();

	// カメラ方向とスティックの入力値によって進行方向を計算
	Camera& camera = Camera::Instance();
	const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
	const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

	// 移動ベクトルはXZ平面に水平なベクトルになるようにする

	// カメラ右方向ベクトルをXZ単位ベクトルに変換
	float cameraRightX = cameraRight.x;
	float cameraRightZ = cameraRight.z;
	float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
	if (cameraRightLength > 0.0f)
	{
		// 単位ベクトル化
		cameraRightX /= cameraRightLength;
		cameraRightZ /= cameraRightLength;
	}

	// カメラ前方向ベクトルをXZ単位ベクトルに変換
	float cameraFrontX = cameraFront.x;
	float cameraFrontZ = cameraFront.z;
	float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
	if (cameraFrontLength > 0.0f)
	{
		// 単位ベクトル化
		cameraFrontX /= cameraFrontLength;
		cameraFrontZ /= cameraFrontLength;
	}

	// スティックの水平入力値をカメラ右方向に反映し、
	// スティックの垂直入力値をカメラ前方向に反映し、
	// 進行ベクトルを計算する
	DirectX::XMFLOAT3 vec;
	vec.x = cameraRightX * ax + cameraFrontX * ay;
	vec.z = cameraRightZ * ax + cameraFrontZ * ay;

	// Y軸方向には移動しない
	vec.y = 0.0f;

	return vec;
}

//デバック用フリーカメラ
void SceneGame::FreeCamera(float elapsedTime)
{
	freeCameraVelocity_ = GetMoveVec();

	float mx = freeCameraVelocity_.x * freeCameraSpeed_;
	float mz = freeCameraVelocity_.z * freeCameraSpeed_;
	freeCameraPosition_.x += mx;
	freeCameraPosition_.z += mz;
	

	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButton() & GamePad::BTN_Z)	freeCameraPosition_.y += freeCameraSpeed_ * 0.5f;
	if (gamePad.GetButton() & GamePad::BTN_X)	freeCameraPosition_.y -= freeCameraSpeed_ * 0.5f;
	if (gamePad.GetButton() & GamePad::BTN_L)	ax_ += 10;
	if (gamePad.GetButton() & GamePad::BTN_J)	ax_ -= 10;

	cameraController_->setCameraYAngle(ax_);

}

//マルチスレッド衝突回避用関数
void SceneGame::ThreadCover()
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


		//色調補正
		//3D空間をspriteとして設定
		sprite_->SetShaderResourceView(renderTarget_->GetShaderResourceView(),
			renderTarget_->GetWidth(), renderTarget_->GetHeight());
		//バッファ更新
		sprite_->Update(0, 0, graphics.GetScreenWidth(), graphics.GetScreenHeight(),
			0, 0, static_cast<float>(renderTarget_->GetWidth()), static_cast<float>(renderTarget_->GetHeight()),
			0,
			1, 1, 1, 1);

		threadCover_++;
	}
}

//ポーズ
bool SceneGame::PauseProcess(float elapsedTime)
{
	isPause_ = pause_->GetIsPause();
	if (pause_->Update(elapsedTime))
	{
		//デバックのみのワープキー(フリーカメラ)
#ifdef _DEBUG
		GamePad& gamePad = Input::Instance().GetGamePad();
		if (gamePad.GetButtonDown() & GamePad::BTN_C)
		{
			if (isFreeCamera_ == true) isFreeCamera_ = false;
			else if (isFreeCamera_ == false) isFreeCamera_ = true;
		}
#endif

		//update
		return false;

	}

	return true;
}

//スプライト用パラメータ更新処理
void SceneGame::SpriteParameterUpdate(float elapsedTime)
{
	//タイマー(ゲーム時間)描画処理
	//1の位
	timerTexPosX_[0] = (int)gameTimer_ * texSize_;
	//10の位
	timerTexPosX_[1] = ((int)gameTimer_ / 10) * texSize_;
	//小数点
	timerTexPosX_[2] = ((int)(gameTimer_ * 10) % 10) * texSize_;

	//GO!! 移動させる
	if (gameStartTime_ < -1.0f)
	{
		GoPosX_ -= 50;
	}

	//コンボ
	//1の位
	comboTexPosX_[0] = player_->GetCombo() * texSize_;
	//10の位
	comboTexPosX_[1] = player_->GetCombo() / 10 * texSize_;

	//スピードメーター描画計算
	speed_ = player_->GetNowSpeed();
	maxSpeed_ = player_->GetMaxSpeed();	//最大スピード
	ratio_ = speed_ / maxSpeed_;
	meterTexX_ = (ratio_ * (768 * 0.5f));
	//グラデーション計算
	r_ = (rEnd_ - rStart_) * ratio_ + rStart_;
	g_ = (gEnd_ - gStart_) * ratio_ + gStart_;
	b_ = (bEnd_ - bStart_) * ratio_ + bStart_;

	//放射ブラーのパラメータ設定
	float sp = speed_ / maxSpeed_;
	radialBlurData_.strength = mathf_.Lerp(0.0001, 0.25, sp);
	//ブラーの計算
	//if(player->GetIsMaxSpeed())
	//{
	//	radialBlurData.sampleCount = 2;
	//	radialBlurData.strength = 0.25f;
	//}
	//else
	//{
	//	radialBlurData.sampleCount = 2;
	//	radialBlurData.strength = 0.01f;
	//}
	//radialBlurData.strength = mathf.Lerp(0, maxSpeed, speed);
	//radialBlurData.strength *= 0.004f;
	//if (radialBlurData.strength > 0.25f) radialBlurData.strength = 0.25f;

	//衝突(ぶつかってしまった時に周りに赤いSpriteを表示する)
	if (player_->isImpulse_)
	{
		ImpulsTimer_ += elapsedTime;
	}
	if (ImpulsTimer_ > 0.5f)
	{
		ImpulsTimer_ = 0;
		player_->isImpulse_ = false;
	}
	//点滅させる用
	flashingTime_++;

	//吹っ飛ばしオブジェクトのタイミング精度表示
	timing_ = player_->timing_;

}

//UIをチェンジする
void SceneGame::ChangeUi()
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetControllerState())
	{
		// Controller is connected
		whichOperation_ = Operations::gamePad;
		anyKeyOrButton_[0] = Pad::Left;
		anyKeyOrButton_[1] = Pad::Right;
		anyKeyOrButton_[2] = Pad::Xbutton;
		anyKeyOrButton_[3] = Pad::Abutton;
		anyKeyOrButton_[4] = Pad::Bbutton;
	}
	else
	{
		whichOperation_ = Operations::keyboard;
		anyKeyOrButton_[0] = KeyBoard::Akey;
		anyKeyOrButton_[1] = KeyBoard::Dkey;
		anyKeyOrButton_[2] = KeyBoard::Jkey;
		anyKeyOrButton_[3] = KeyBoard::Kkey;
		anyKeyOrButton_[4] = KeyBoard::Lkey;
	}
}

void SceneGame::OneFrameUpdate(float elapsedTime)
{
	//カメラコントローラー更新処理
	target_ = player_->GetPosition();
	target_.y += 3.5f;
	freeCameraPosition_ = player_->GetPosition();

	cameraController_->SetTarget(target_);
	cameraController_->Update(elapsedTime);

	//ステージ更新処理
	stage_->Update(elapsedTime);

	//エネミー更新処理
	EnemyManager::Instance().Update(elapsedTime);

	//背景オブジェクト更新処理
	BackObjManager::Instance().Update(elapsedTime);

	//エフェクト更新処理
	EffectManager::Instane().Update(elapsedTime);

	//スプライトのパラメータ更新処理
	SpriteParameterUpdate(elapsedTime);

}