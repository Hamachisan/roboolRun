#include "Graphics/Graphics.h"
#include "SceneClear.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include "SceneLoading.h"
#include "SceneTitle.h"
#include "Input/Input.h"
#include <algorithm>

//初期化
void SceneClear::Initialize()
{
	//ステージ初期化
	stageModel_ = new Stage();

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

	//結果を代入
	clearTime_ = Score::Instance().GetClearTime();

	stage_ = SceneManager::Instance().GetStageNum();
	//ソート
	std::sort(data_.begin(), data_.end());

	score_ = Score::Instance().ResultScore();
	SaveLoadFile();

	SpriteInitialize();

	ParameterInitialize();
}

//パラメーター初期化
void SceneClear::ParameterInitialize()
{
	//文字座標
	{
		//スコア
		pos_[SpritePram::word_score].x = 360;
		pos_[SpritePram::word_score].y = 10;
		scale_[SpritePram::word_score].x = 512;
		scale_[SpritePram::word_score].y = 128;
		//クリアタイム
		pos_[SpritePram::word_clearTime].x = 260;
		pos_[SpritePram::word_clearTime].y = 320;	//30
		scale_[SpritePram::word_clearTime].x = 256;
		scale_[SpritePram::word_clearTime].y = 64;
		//MAXコンボ
		pos_[SpritePram::word_maxCombo].x = pos_[SpritePram::word_clearTime].x;
		pos_[SpritePram::word_maxCombo].y = pos_[SpritePram::word_clearTime].y + 80;
		scale_[SpritePram::word_maxCombo].x = 256;
		scale_[SpritePram::word_maxCombo].y = 64;
		//パーフェクト
		pos_[SpritePram::word_perfect].x = pos_[SpritePram::word_clearTime].x;
		pos_[SpritePram::word_perfect].y = pos_[SpritePram::word_maxCombo].y + 70;
		scale_[SpritePram::word_perfect].x = 256;
		scale_[SpritePram::word_perfect].y = 64;
		//グレイト
		pos_[SpritePram::word_great].x = pos_[SpritePram::word_clearTime].x - 30;
		pos_[SpritePram::word_great].y = pos_[SpritePram::word_perfect].y + 70;
		scale_[SpritePram::word_great].x = 256;
		scale_[SpritePram::word_great].y = 64;
	}
	//数字座標
	{
		//スコア
		pos_[SpritePram::num_score].x = 280 + 128;
		pos_[SpritePram::num_score].y = 150;
		scale_[SpritePram::num_score].x = 128;
		scale_[SpritePram::num_score].y = 128;

		//クリアタイム
		pos_[SpritePram::num_clearTime].x = 700;
		pos_[SpritePram::num_clearTime].y = 316;
		//MAXコンボ
		pos_[SpritePram::num_maxCombo].x = pos_[SpritePram::num_clearTime].x - 128;
		pos_[SpritePram::num_maxCombo].y = pos_[SpritePram::num_clearTime].y + 80;
		//パーフェクト
		pos_[SpritePram::num_perfect].x = pos_[SpritePram::num_clearTime].x - 128;
		pos_[SpritePram::num_perfect].y = pos_[SpritePram::num_maxCombo].y + 70;
		//グレイト
		pos_[SpritePram::num_great].x = pos_[SpritePram::num_clearTime].x - 128;
		pos_[SpritePram::num_great].y = pos_[SpritePram::num_perfect].y + 70;
	}
	//ランキング表示座標
	{
		pos_[SpritePram::num_ranking].x = 500;
		pos_[SpritePram::num_ranking].y = 316;
		scale_[SpritePram::num_ranking].x = 256;
		scale_[SpritePram::num_ranking].y = 64;

	}

	//pt
	{
		pos_[SpritePram::word_pt].x = 760;
		pos_[SpritePram::word_pt].y = 176;
		scale_[SpritePram::word_pt].x = 512;
		scale_[SpritePram::word_pt].y = 128;
	}
}

void SceneClear::SpriteInitialize()
{
	sprNext_[0]						= std::make_unique<Sprite>("Data/Sprite/Scene/Reslut/next.png");
	sprNext_[1]						= std::make_unique<Sprite>("Data/Sprite/Scene/Reslut/GoTitle.png");
	sprPoint_						= std::make_unique<Sprite>("Data/Sprite/Scene/Reslut/pt.png");
	sprRanking_						= std::make_unique<Sprite>("Data/Sprite/Scene/Reslut/Ranking.png");
	Textsprites_[0]					= std::make_unique<Sprite>(".\\Data\\Font\\font4.png");
	sprNumber_						= std::make_unique<Sprite>("Data/Sprite/number.png");
	sprTimerTen_					= std::make_unique<Sprite>("Data/Sprite/tenn.png");
	sprBack_						= std::make_unique<Sprite>("Data/Sprite/Scene/Reslut/back.png");

	sprPerfect_						= std::make_unique<Sprite>("Data/Sprite/Scene/Reslut/PERFECT3.png");
	sprGreat_						= std::make_unique<Sprite>("Data/Sprite/Scene/Reslut/GREAT.png");
	sprClearTime_					= std::make_unique<Sprite>("Data/Sprite/Scene/Reslut/clearTime.png");
	sprMaxCombo_					= std::make_unique<Sprite>("Data/Sprite/Scene/Reslut/maxCombo.png");
	sprScore_						= std::make_unique<Sprite>("Data/Sprite/Scene/Reslut/Score.png");

	spr1st_							= std::make_unique<Sprite>("Data/Sprite/Scene/Reslut/1st.png");
	spr2nd_							= std::make_unique<Sprite>("Data/Sprite/Scene/Reslut/2nd.png");
	spr3rd_							= std::make_unique<Sprite>("Data/Sprite/Scene/Reslut/3rd.png");

	sprPushKeyButton_[KEY]			= std::make_unique<Sprite>("Data/Sprite/Scene/Title/EnterKey.png");
	sprPushKeyButton_[BUTTON]		= std::make_unique<Sprite>("Data/Sprite/Scene/Title/Bbutton.png");

}

//終了化
void SceneClear::Finalize()
{
	//ステージ終了化
	SafeDelete(stageModel_);

	//プレイヤー終了化
	SafeDelete(player_);

	SafeDelete(cameraController_);
	//data.clear();
}

//更新処理
void SceneClear::Update(float elapsedTime)
{
	//カメラ処理
	cameraController_->setCameraRange(5);
	target_ = player_->GetPosition();
	target_.x += 6.0f;
	target_.y += 0.5f;
	target_.z -= 8.0f;
	cameraController_->SetTarget(target_);
	cameraController_->Update(elapsedTime);

	//ステージ更新処理
	stageModel_->Update(elapsedTime);

	//プレイヤー更新処理
	player_->Update(elapsedTime);


	//コントローラーが繋がれていたら表示するUIを変更
	ChangeUi();

	GamePad& gamePad = Input::Instance().GetGamePad();

	//////////////   結果発表    ////////////////
	////////////////////////////////////////////

	switch (sceneState_)
	{
	case 0:	//今回のリザルトを表示するシーン

		//スコア
		{
			//1の位
			scoreTexPosX_[5] = (int)Score::Instance().GetResult() * texSize_;
			//10の位
			scoreTexPosX_[4] = ((int)Score::Instance().GetResult() / 10) * texSize_;
			//100の位
			scoreTexPosX_[3] = ((int)Score::Instance().GetResult() / 100) * texSize_;
			//1000の位
			scoreTexPosX_[2] = ((int)Score::Instance().GetResult() / 1000) * texSize_;
			//10000の位
			scoreTexPosX_[1] = ((int)Score::Instance().GetResult() / 10000) * texSize_;
			//100000の位
			scoreTexPosX_[0] = ((int)Score::Instance().GetResult() / 100000) * texSize_;

		}
		//クリアタイム
		{
			//1の位
			clearTimeTexPosX_[0] = (int)Score::Instance().GetClearTime() * texSize_;
			//10の位
			clearTimeTexPosX_[1] = ((int)Score::Instance().GetClearTime() / 10) * texSize_;
			//小数点
			clearTimeTexPosX_[2] = ((int)(Score::Instance().GetClearTime() * 10) % 10) * texSize_;
		}
		//MAXコンボ
		{
			//1の位
			maxComboTexPosX_[1] = (int)Score::Instance().GetMaxCombo() * texSize_;
			//10の位
			maxComboTexPosX_[0] = ((int)Score::Instance().GetMaxCombo() / 10) * texSize_;
		}
		//パーフェクト数
		{
			//1の位
			perfectTexPosX_[1] = (int)Score::Instance().GetPerfectConut() * texSize_;
			//10の位
			perfectTexPosX_[0] = ((int)Score::Instance().GetPerfectConut() / 10) * texSize_;
		}
		//グレイト数
		{
			//1の位
			greatTexPosX_[1] = (int)Score::Instance().GetGreatCount() * texSize_;
			//10の位
			greatTexPosX_[0] = ((int)Score::Instance().GetGreatCount() / 10) * texSize_;
		}

		//ランキング表示画面へ
		if (gamePad.GetButtonDown() & gamePad.BTN_ENTER ||
			gamePad.GetButtonDown() & gamePad.BTN_PAD_B)
		{
			sceneState_++;
		}

		break;

	case 1:

		//ランキングの描画用ステータス(3位まで)
		for (int i = 0; i < SHOW_RANKING; i++)
		{
			//1の位
			RankingTimeTexPos_[i][0] = (int)data_.at(stage_).at(i) * texSize_;
			//10の位
			RankingTimeTexPos_[i][1] = ((int)data_.at(stage_).at(i) / 10) * texSize_;
			//100の位
			RankingTimeTexPos_[i][2] = ((int)data_.at(stage_).at(i) / 100) * texSize_;
			//1000の位
			RankingTimeTexPos_[i][3] = ((int)data_.at(stage_).at(i) / 1000) * texSize_;
			//10000の位
			RankingTimeTexPos_[i][4] = ((int)data_.at(stage_).at(i) / 10000) * texSize_;
			//100000の位
			RankingTimeTexPos_[i][5] = ((int)data_.at(stage_).at(i) / 100000) * texSize_;
		}



		//タイトルへ戻る
		if (gamePad.GetButtonDown() & gamePad.BTN_ENTER ||
			gamePad.GetButtonDown() & gamePad.BTN_PAD_B)
		{
			SceneManager::Instance().SetNextScene(NEXT_SCENE_OTHER);
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
		}

		break;
	}
}

//描画処理
void SceneClear::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();
	
	//画面クリア&レンダーターゲット設定
	FLOAT color[] = { 0.0f, 1.0f, 1.0f, 0.0f };
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	// 3Dモデル描画
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);

		//ステージ描画
		stageModel_->Render(dc, shader);
		//プレイヤー描画
		player_->Render(dc, shader);

		shader->End(dc);
	}
	
	//2Dスプライト描画
	UiRender(dc);

	//ImGui
#ifdef _DEBUG
	{
		ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
		if (ImGui::Begin("Result", nullptr, ImGuiTreeNodeFlags_None))
		{
			if (ImGui::CollapsingHeader("Score", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::InputFloat("score", &score_);
				ImGui::InputFloat("top", &ranking_[0].top_);
				ImGui::InputFloat("second", &ranking_[0].second_);
				ImGui::InputFloat("third", &ranking_[0].third_);
				ImGui::InputFloat("save", &ranking_[0].save_);
			}
			if (ImGui::CollapsingHeader("pos", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::SliderFloat("ScorePosX",			&pos_[SpritePram::word_score].x,		0, 1280);
				ImGui::SliderFloat("ScorePosY",			&pos_[SpritePram::word_score].y,		0, 720);
				ImGui::SliderFloat("clearTimePosX",		&pos_[SpritePram::word_clearTime].x,	0, 1280);
				ImGui::SliderFloat("clearTimePosY",		&pos_[SpritePram::word_clearTime].y,	0, 720);
				ImGui::SliderFloat("maxComboPosX",		&pos_[SpritePram::word_maxCombo].x,		0, 1280);
				ImGui::SliderFloat("maxComboPosY",		&pos_[SpritePram::word_maxCombo].y,		0, 720);
				ImGui::SliderFloat("perfectPosX",		&pos_[SpritePram::word_perfect].x,		0, 1280);
				ImGui::SliderFloat("perfectPosY",		&pos_[SpritePram::word_perfect].y,		0, 720);
				ImGui::SliderFloat("greatPosX",			&pos_[SpritePram::word_great].x,		0, 1280);
				ImGui::SliderFloat("greatPosY",			&pos_[SpritePram::word_great].y,		0, 720);

				ImGui::SliderFloat("PosX", &pos_[SpritePram::num_clearTime].x, 0, 1280);
				ImGui::SliderFloat("PosY", &pos_[SpritePram::num_clearTime].y, 0, 720);

				ImGui::SliderFloat("PtPosX", &pos_[SpritePram::word_pt].x, 0, 1280);
				ImGui::SliderFloat("PtPosY", &pos_[SpritePram::word_pt].y, 0, 720);
			}

			if (ImGui::CollapsingHeader("scale", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::SliderFloat("ScoreScaleX",		&scale_[SpritePram::word_score].x,		512, 0);
				ImGui::SliderFloat("ScoreScaleY",		&scale_[SpritePram::word_score].y,		512, 0);
				ImGui::SliderFloat("clearTimeScaleX",	&scale_[SpritePram::word_clearTime].x,	512, 0);
				ImGui::SliderFloat("clearTimeScaleY",	&scale_[SpritePram::word_clearTime].y,	512, 0);
				ImGui::SliderFloat("maxComboScaleX",	&scale_[SpritePram::word_maxCombo].x,	512, 0);
				ImGui::SliderFloat("maxComboScaleY",	&scale_[SpritePram::word_maxCombo].y,	512, 0);
				ImGui::SliderFloat("perfectScaleX",		&scale_[SpritePram::word_perfect].x,	512, 0);
				ImGui::SliderFloat("perfectScaleY",		&scale_[SpritePram::word_perfect].y,	512, 0);
				ImGui::SliderFloat("greatScaleX",		&scale_[SpritePram::word_great].x,		512, 0);
				ImGui::SliderFloat("greatScaleY",		&scale_[SpritePram::word_great].y,		512, 0);

				ImGui::SliderFloat("PtScaleX", &scale_[SpritePram::word_pt].x, 512, 0);
				ImGui::SliderFloat("PtScaleY", &scale_[SpritePram::word_pt].y, 512, 0);
			}



		}
		ImGui::End();
	}
#else
#endif

}

void SceneClear::UiRender(ID3D11DeviceContext* dc)
{
	//背景
	sprBack_->Render(dc,
		0, 0,						//座標						
		1280, 720,					//スケール
		0, 0, 						//テクスチャ基準位置
		1280, 720,					//テクスチャの大きさ
		0,							//角度
		1, 1, 1, 1);				//色(rgb)不透明度(a)


	//エンターはBボタンか
	sprPushKeyButton_[keyOrButton_]->Render(dc,
		1150, 590,					//座標
		110, 110,					//スケール
		0, 0, 						//テクスチャ基準位置
		512, 512,					//テクスチャの大きさ
		0, 							//角度
		1, 1, 1, 1);				//色(rgb)不透明度(a)
									
	switch (sceneState_)
	{
	case 0:	//今回のリザルトを表示するシーン
		//次へ
		sprNext_[sceneState_]->Render(dc,
			700, 580,					//座標
			512, 128,					//スケール
			0, 0, 						//テクスチャ基準位置
			512, 128,					//テクスチャの大きさ
			0, 							//角度
			1, 1, 1, 1);				//色(rgb)不透明度(a)

		//Score文字
		sprScore_->Render(dc,
			pos_[SpritePram::word_score].x, pos_[SpritePram::word_score].y,					//座標
			scale_[SpritePram::word_score].x, scale_[SpritePram::word_score].y,				//スケール
			0, 0, 																			//テクスチャ基準位置
			512, 128,																		//テクスチャの大きさ
			0, 																				//角度
			1, 1, 1, 1);																	//色(rgb)不透明度(a)

		//ClearTime文字
		sprClearTime_->Render(dc,
			pos_[SpritePram::word_clearTime].x, pos_[SpritePram::word_clearTime].y,			//座標
			scale_[SpritePram::word_clearTime].x, scale_[SpritePram::word_clearTime].y,		//スケール
			0, 0, 																			//テクスチャ基準位置
			512, 128,																		//テクスチャの大きさ
			0, 																				//角度
			1, 1, 1, 1);																	//色(rgb)不透明度(a)

		///MAXCombo文字
		sprMaxCombo_->Render(dc,
			pos_[SpritePram::word_maxCombo].x, pos_[SpritePram::word_maxCombo].y,			//座標
			scale_[SpritePram::word_maxCombo].x, scale_[SpritePram::word_maxCombo].y,		//スケール
			0, 0, 																			//テクスチャ基準位置
			512, 128,																		//テクスチャの大きさ
			0, 																				//角度
			1, 1, 1, 1);																	//色(rgb)不透明度(a)

		//パーフェクト文字
		sprPerfect_->Render(dc,
			pos_[SpritePram::word_perfect].x, pos_[SpritePram::word_perfect].y,				//座標
			scale_[SpritePram::word_perfect].x, scale_[SpritePram::word_perfect].y,			//スケール
			0, 0, 																			//テクスチャ基準位置
			512, 128,																		//テクスチャの大きさ
			0, 																				//角度
			1, 1, 1, 1);																	//色(rgb)不透明度(a)

		//グレイト文字
		sprGreat_->Render(dc,
			pos_[SpritePram::word_great].x, pos_[SpritePram::word_great].y,					//座標
			scale_[SpritePram::word_great].x, scale_[SpritePram::word_great].y,				//スケール
			0, 0, 																			//テクスチャ基準位置
			512, 128,																		//テクスチャの大きさ
			0, 																				//角度
			1, 1, 1, 1);																	//色(rgb)不透明度(a)


		//スコア表示
		RenderNumber(dc, &scoreTexPosX_[3], 3, pos_[SpritePram::num_score], scale_[SpritePram::num_score], 128);

		sprPoint_->Render(dc,
			pos_[SpritePram::word_pt].x, pos_[SpritePram::word_pt].y,						//座標
			scale_[SpritePram::word_pt].x, scale_[SpritePram::word_pt].y,					//スケール
			0, 0, 																			//テクスチャ基準位置
			512, 128,																		//テクスチャの大きさ
			0, 																				//角度
			1, 1, 1, 1);																	//色(rgb)不透明度(a)


		//クリアタイム表示
#if 1
			//結果 タイム表示
			//10の位
		sprNumber_->Render(dc,
			pos_[SpritePram::num_clearTime].x - 128, pos_[SpritePram::num_clearTime].y,		//座標
			64, 64,																			//スケール
			clearTimeTexPosX_[1], 0, 														//テクスチャ基準位置
			64, 64,																			//テクスチャの大きさ
			0,																				//角度
			1, 1, 1, 1);																	//色(rgb)不透明度(a)

		//1の位
		sprNumber_->Render(dc,
			pos_[SpritePram::num_clearTime].x - 64, pos_[SpritePram::num_clearTime].y,		//座標
			64, 64,																			//スケール
			clearTimeTexPosX_[0], 0, 														//テクスチャ基準位置
			64, 64,																			//テクスチャの大きさ
			0,																				//角度
			1, 1, 1, 1);																	//色(rgb)不透明度(a)

		//小数点
		sprTimerTen_->Render(dc,
			pos_[SpritePram::num_clearTime].x - 16, pos_[SpritePram::num_clearTime].y,		//座標
			64, 64,																			//スケール
			0, 0, 																			//テクスチャ基準位置
			64, 64,																			//テクスチャの大きさ
			0,																				//角度
			1, 1, 1, 1);																	//色(rgb)不透明度(a)

		//少数第一位
		sprNumber_->Render(dc,
			pos_[SpritePram::num_clearTime].x + 32, pos_[SpritePram::num_clearTime].y,		//座標
			64, 64,																			//スケール
			clearTimeTexPosX_[2], 0, 														//テクスチャ基準位置
			64, 64,																			//テクスチャの大きさ
			0,																				//角度
			1, 1, 1, 1);																	//色(rgb)不透明度(a)
#endif
		//コンボ数表示
		RenderNumber(dc, &maxComboTexPosX_[0], 2, pos_[SpritePram::num_maxCombo]);
		//パーフェクト数表示
		RenderNumber(dc, &perfectTexPosX_[0], 2, pos_[SpritePram::num_perfect]);
		//グレイト数表示
		RenderNumber(dc, &greatTexPosX_[0], 2, pos_[SpritePram::num_great]);
		break;

	case 1:
		//Titleへ
		sprNext_[sceneState_]->Render(dc,
			700, 580,						//座標
			512, 128,						//スケール
			0, 0, 							//テクスチャ基準位置
			512, 128,						//テクスチャの大きさ
			0, 								//角度
			1, 1, 1, 1);					//色(rgb)不透明度(a)

		//ランキング表示の文字
		sprRanking_->Render(dc,
			pos_[SpritePram::word_score].x, pos_[SpritePram::word_score].y,				//座標
			scale_[SpritePram::word_score].x, scale_[SpritePram::word_score].y,			//スケール
			0, 0, 																		//テクスチャ基準位置
			512, 128,																	//テクスチャの大きさ
			0, 																			//角度
			1, 1, 1, 1);																//色(rgb)不透明度(a)

		//スコア表示
		RenderNumber(dc, &scoreTexPosX_[3], 3, pos_[SpritePram::num_score], scale_[SpritePram::num_score], 128);

		sprPoint_->Render(dc,
			pos_[SpritePram::word_pt].x, pos_[SpritePram::word_pt].y,					//座標
			scale_[SpritePram::word_pt].x, scale_[SpritePram::word_pt].y,				//スケール
			0, 0, 																		//テクスチャ基準位置
			512, 128,																	//テクスチャの大きさ
			0, 																			//角度
			1, 1, 1, 1);																//色(rgb)不透明度(a)

		//1st
		spr1st_->Render(dc,
			pos_[SpritePram::word_clearTime].x, pos_[SpritePram::word_clearTime].y,			//座標
			scale_[SpritePram::word_clearTime].x, scale_[SpritePram::word_clearTime].y,		//スケール
			0, 0, 																			//テクスチャ基準位置
			512, 128,																		//テクスチャの大きさ
			0, 																				//角度
			1, 1, 1, 1);																	//色(rgb)不透明度(a)

		//2nd
		spr2nd_->Render(dc,
			pos_[SpritePram::word_maxCombo].x, pos_[SpritePram::word_maxCombo].y,			//座標
			scale_[SpritePram::word_maxCombo].x, scale_[SpritePram::word_maxCombo].y,		//スケール
			0, 0, 																			//テクスチャ基準位置
			512, 128,																		//テクスチャの大きさ
			0, 																				//角度
			1, 1, 1, 1);																	//色(rgb)不透明度(a)
		//3rd
		spr3rd_->Render(dc,
			pos_[SpritePram::word_perfect].x, pos_[SpritePram::word_perfect].y,				//座標
			scale_[SpritePram::word_perfect].x, scale_[SpritePram::word_perfect].y,			//スケール
			0, 0, 																			//テクスチャ基準位置
			512, 128,																		//テクスチャの大きさ
			0, 																				//角度
			1, 1, 1, 1);																	//色(rgb)不透明度(a)

		for (int i = 0; i < SHOW_RANKING; i++)
		{
			//初期値に小さい値を入れてあるので初期値の場合は描画しない
			if (data_.at(stage_).at(i) > 5)
			{
				float intarvalY = 80;

				//1の位
				sprNumber_->Render(dc,
					pos_[SpritePram::num_ranking].x, pos_[SpritePram::num_ranking].y + (i * intarvalY),			//座標
					64, 64,																						//スケール
					RankingTimeTexPos_[i][2], 0, 																//テクスチャ基準位置
					64, 64,																						//テクスチャの大きさ
					0,																							//角度
					1, 1, 1, 1);																				//色(rgb)不透明度(a)

				//10の位
				sprNumber_->Render(dc,
					pos_[SpritePram::num_ranking].x + 64, pos_[SpritePram::num_ranking].y + (i * intarvalY),	//座標
					64, 64,																						//スケール
					RankingTimeTexPos_[i][1], 0, 																//テクスチャ基準位置
					64, 64,																						//テクスチャの大きさ
					0,																							//角度
					1, 1, 1, 1);																				//色(rgb)不透明度(a)
				//100の位
				sprNumber_->Render(dc,
					pos_[SpritePram::num_ranking].x + 128, pos_[SpritePram::num_ranking].y + (i * intarvalY),	//座標
					64, 64,																						//スケール
					RankingTimeTexPos_[i][0], 0, 																//テクスチャ基準位置
					64, 64,																						//テクスチャの大きさ
					0,																							//角度
					1, 1, 1, 1);																				//色(rgb)不透明度(a)

				//point
				sprPoint_->Render(dc,
					pos_[SpritePram::num_ranking].x + 192, pos_[SpritePram::num_ranking].y + (i * intarvalY) + 15,	//座標
					256, 64,																						//スケール
					0, 0, 																							//テクスチャ基準位置
					512, 128,																						//テクスチャの大きさ
					0, 																								//角度
					1, 1, 1, 1);																					//色(rgb)不透明度(a)
			}
		}
		break;
	}
}


void SceneClear::SaveLoadFile()
{
	//結果のセーブとロード
	//読み込み
	errno_t err = fopen_s(&fp_, FILENAME, "rb");
	//初回起動時は必ず失敗(ファイルがないので生成)
	if (err != 0)
	{
		//新しいファイルを生成
		errno_t err = fopen_s(&fp_, FILENAME, "wb");
		if (err != 0)
			return;

		for (int i = 0; i < MAX_STAGE * MAX_STAGE_LEVEL; i++)
		{
			//バイナリで生成するかテキストで生成するか
#if DATA_FORMAT			

			//テキストファイルで生成
			data.at(i).at(top) = 1000000 * (i + 1);
			data.at(i).at(second) = 2000000 * (i + 1);
			data.at(i).at(third) = 3000000 * (i + 1);
			data.at(i).at(save) = 4000000 * (i + 1);

			fprintf(fp, "%f %f %f %f \n",
				data.at(i).at(top), data.at(i).at(second),
				data.at(i).at(third), data.at(i).at(save));
		}
#else
			//バイナリファイルで生成
			ranking_[i].top_ = 0.1f * (i );
			ranking_[i].second_ = 0.2f * (i );
			ranking_[i].third_ = 0.3f * (i );
			ranking_[i].save_ = 0.4f * (i);
		}

		std::fwrite(&ranking_, sizeof(float), (MAX_STAGE* MAX_STAGE_LEVEL) * COLUMN, fp_);	//ranking構造体の値をファイルに書き込み
#endif
		std::fclose(fp_);
	}
		////ファイルが存在している時(2回目以降))
		//バイナリから読み込むかテキストから読み込むか
#if DATA_FORMAT			
		//テキスト
		char line[256];	//1行の最大文字数(バイト)
		char str[256];	//
		while (fgets(str, 256, fp) != NULL) {
			//sscanf_s(str, "%f %f %f %f", &rank[top], &rank[second], &rank[third], &rank[save]);
			sscanf_s(str, "%f %f %f %f", &data.at(textCount).at(top), &data.at(textCount).at(second),
				&data.at(textCount).at(third), &data.at(textCount).at(save));
			textCount++;
		}
#else
		//バイナリ
		std::fread(&ranking_, sizeof(float), (MAX_STAGE * MAX_STAGE_LEVEL) * COLUMN, fp_);
#endif
		std::fclose(fp_);

		//ソートする為にベクター配列に代入
		for (int i = 0; i < MAX_STAGE * MAX_STAGE_LEVEL; i++)
		{
			data_.at(i).at(top) = ranking_[i].top_;
			data_.at(i).at(second) = ranking_[i].second_;
			data_.at(i).at(third) = ranking_[i].third_;
			data_.at(i).at(save) = ranking_[i].save_;
		}

		//今回のステージの4番目に今回の結果を上書き
		data_.at(stage_).at(save) = score_;

		//今回のステージだけソート
		std::sort(data_.at(stage_).begin(), data_.at(stage_).end(), std::greater<float>());

		//fwrite用にrankingにソートした値を代入
		for (int i = 0; i < MAX_STAGE * MAX_STAGE_LEVEL; i++)
		{
			ranking_[i].top_ = data_.at(i).at(top);
			ranking_[i].second_ = data_.at(i).at(second);
			ranking_[i].third_ = data_.at(i).at(third);
			ranking_[i].save_ = data_.at(i).at(save);
		}

		//順位の更新適応後の書き込み
		err = fopen_s(&fp_, FILENAME, "wb");
		if (err != 0)
			return;
		//バイナリで書き込むかテキストで書き込むか
#if  DATA_FORMAT
		for (int i = 0; i < MAX_STAGE * MAX_STAGE_LEVEL; i++)
		{
			fprintf(fp, "%f %f %f %f \n",
				data.at(i).at(top), data.at(i).at(second),
				data.at(i).at(third), data.at(i).at(save));
		}
		fprintf(fp, "top[%f],second[%f],third[%f]、save[%f]",
#else
		std::fwrite(&ranking_, sizeof(float), (MAX_STAGE * MAX_STAGE_LEVEL) * COLUMN, fp_);	//ranking構造体の値をファイルに書き込み
#endif
		std::fclose(fp_);

}

//数字をスプライトで表示する
void SceneClear::RenderNumber(ID3D11DeviceContext* dc, float* size ,int digit, 
	DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 scale, float intervalX, float intervalY)
{
	int num = 0;
	for (int i = 0; i < digit; i++)
	{
		sprNumber_->Render(dc,
			pos.x + (i * intervalX), 
			pos.y + (i * intervalY),
			scale.x, scale.y,
			*size, 0,
			64, 64,
			0, 1, 1, 1, 1);
		size++;
		num++;
	}
}

//スプライトのパラメータ更新処理
void SceneClear::SpriteParameterUpdate(float elapsedTime)
{
	//特に移動変更するパラメータは無いので処理はしない&呼ばない
}

//Ui変更処理
void SceneClear::ChangeUi()
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