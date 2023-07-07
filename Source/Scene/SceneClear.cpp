#include "Graphics/Graphics.h"
#include "SceneClear.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include "SceneLoading.h"
#include "SceneTitle.h"
#include "Input/Input.h"
#include <algorithm>

//������
void SceneClear::Initialize()
{
	//�X�e�[�W������
	stageModel_ = new Stage();

	//�v���C���[������
	player_ = new TitlePlayer();

	//�J���������ݒ�
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();	//&���Y��Ȃ�!!
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,	//���̈ʒu�ɃJ������u��
		1000.0f	//�����鋗��
	);

	//�J�����R���g���[���[������
	cameraController_ = new CameraController();
	target_ = player_->GetPosition();

	//���ʂ���
	clearTime_ = Score::Instance().GetClearTime();

	stage_ = SceneManager::Instance().GetStageNum();
	//�\�[�g
	std::sort(data_.begin(), data_.end());

	score_ = Score::Instance().ResultScore();
	SaveLoadFile();

	SpriteInitialize();

	ParameterInitialize();
}

//�p�����[�^�[������
void SceneClear::ParameterInitialize()
{
	//�������W
	{
		//�X�R�A
		pos_[SpritePram::word_score].x = 360;
		pos_[SpritePram::word_score].y = 10;
		scale_[SpritePram::word_score].x = 512;
		scale_[SpritePram::word_score].y = 128;
		//�N���A�^�C��
		pos_[SpritePram::word_clearTime].x = 260;
		pos_[SpritePram::word_clearTime].y = 320;	//30
		scale_[SpritePram::word_clearTime].x = 256;
		scale_[SpritePram::word_clearTime].y = 64;
		//MAX�R���{
		pos_[SpritePram::word_maxCombo].x = pos_[SpritePram::word_clearTime].x;
		pos_[SpritePram::word_maxCombo].y = pos_[SpritePram::word_clearTime].y + 80;
		scale_[SpritePram::word_maxCombo].x = 256;
		scale_[SpritePram::word_maxCombo].y = 64;
		//�p�[�t�F�N�g
		pos_[SpritePram::word_perfect].x = pos_[SpritePram::word_clearTime].x;
		pos_[SpritePram::word_perfect].y = pos_[SpritePram::word_maxCombo].y + 70;
		scale_[SpritePram::word_perfect].x = 256;
		scale_[SpritePram::word_perfect].y = 64;
		//�O���C�g
		pos_[SpritePram::word_great].x = pos_[SpritePram::word_clearTime].x - 30;
		pos_[SpritePram::word_great].y = pos_[SpritePram::word_perfect].y + 70;
		scale_[SpritePram::word_great].x = 256;
		scale_[SpritePram::word_great].y = 64;
	}
	//�������W
	{
		//�X�R�A
		pos_[SpritePram::num_score].x = 280 + 128;
		pos_[SpritePram::num_score].y = 150;
		scale_[SpritePram::num_score].x = 128;
		scale_[SpritePram::num_score].y = 128;

		//�N���A�^�C��
		pos_[SpritePram::num_clearTime].x = 700;
		pos_[SpritePram::num_clearTime].y = 316;
		//MAX�R���{
		pos_[SpritePram::num_maxCombo].x = pos_[SpritePram::num_clearTime].x - 128;
		pos_[SpritePram::num_maxCombo].y = pos_[SpritePram::num_clearTime].y + 80;
		//�p�[�t�F�N�g
		pos_[SpritePram::num_perfect].x = pos_[SpritePram::num_clearTime].x - 128;
		pos_[SpritePram::num_perfect].y = pos_[SpritePram::num_maxCombo].y + 70;
		//�O���C�g
		pos_[SpritePram::num_great].x = pos_[SpritePram::num_clearTime].x - 128;
		pos_[SpritePram::num_great].y = pos_[SpritePram::num_perfect].y + 70;
	}
	//�����L���O�\�����W
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

//�I����
void SceneClear::Finalize()
{
	//�X�e�[�W�I����
	SafeDelete(stageModel_);

	//�v���C���[�I����
	SafeDelete(player_);

	SafeDelete(cameraController_);
	//data.clear();
}

//�X�V����
void SceneClear::Update(float elapsedTime)
{
	//�J��������
	cameraController_->setCameraRange(5);
	target_ = player_->GetPosition();
	target_.x += 6.0f;
	target_.y += 0.5f;
	target_.z -= 8.0f;
	cameraController_->SetTarget(target_);
	cameraController_->Update(elapsedTime);

	//�X�e�[�W�X�V����
	stageModel_->Update(elapsedTime);

	//�v���C���[�X�V����
	player_->Update(elapsedTime);


	//�R���g���[���[���q����Ă�����\������UI��ύX
	ChangeUi();

	GamePad& gamePad = Input::Instance().GetGamePad();

	//////////////   ���ʔ��\    ////////////////
	////////////////////////////////////////////

	switch (sceneState_)
	{
	case 0:	//����̃��U���g��\������V�[��

		//�X�R�A
		{
			//1�̈�
			scoreTexPosX_[5] = (int)Score::Instance().GetResult() * texSize_;
			//10�̈�
			scoreTexPosX_[4] = ((int)Score::Instance().GetResult() / 10) * texSize_;
			//100�̈�
			scoreTexPosX_[3] = ((int)Score::Instance().GetResult() / 100) * texSize_;
			//1000�̈�
			scoreTexPosX_[2] = ((int)Score::Instance().GetResult() / 1000) * texSize_;
			//10000�̈�
			scoreTexPosX_[1] = ((int)Score::Instance().GetResult() / 10000) * texSize_;
			//100000�̈�
			scoreTexPosX_[0] = ((int)Score::Instance().GetResult() / 100000) * texSize_;

		}
		//�N���A�^�C��
		{
			//1�̈�
			clearTimeTexPosX_[0] = (int)Score::Instance().GetClearTime() * texSize_;
			//10�̈�
			clearTimeTexPosX_[1] = ((int)Score::Instance().GetClearTime() / 10) * texSize_;
			//�����_
			clearTimeTexPosX_[2] = ((int)(Score::Instance().GetClearTime() * 10) % 10) * texSize_;
		}
		//MAX�R���{
		{
			//1�̈�
			maxComboTexPosX_[1] = (int)Score::Instance().GetMaxCombo() * texSize_;
			//10�̈�
			maxComboTexPosX_[0] = ((int)Score::Instance().GetMaxCombo() / 10) * texSize_;
		}
		//�p�[�t�F�N�g��
		{
			//1�̈�
			perfectTexPosX_[1] = (int)Score::Instance().GetPerfectConut() * texSize_;
			//10�̈�
			perfectTexPosX_[0] = ((int)Score::Instance().GetPerfectConut() / 10) * texSize_;
		}
		//�O���C�g��
		{
			//1�̈�
			greatTexPosX_[1] = (int)Score::Instance().GetGreatCount() * texSize_;
			//10�̈�
			greatTexPosX_[0] = ((int)Score::Instance().GetGreatCount() / 10) * texSize_;
		}

		//�����L���O�\����ʂ�
		if (gamePad.GetButtonDown() & gamePad.BTN_ENTER ||
			gamePad.GetButtonDown() & gamePad.BTN_PAD_B)
		{
			sceneState_++;
		}

		break;

	case 1:

		//�����L���O�̕`��p�X�e�[�^�X(3�ʂ܂�)
		for (int i = 0; i < SHOW_RANKING; i++)
		{
			//1�̈�
			RankingTimeTexPos_[i][0] = (int)data_.at(stage_).at(i) * texSize_;
			//10�̈�
			RankingTimeTexPos_[i][1] = ((int)data_.at(stage_).at(i) / 10) * texSize_;
			//100�̈�
			RankingTimeTexPos_[i][2] = ((int)data_.at(stage_).at(i) / 100) * texSize_;
			//1000�̈�
			RankingTimeTexPos_[i][3] = ((int)data_.at(stage_).at(i) / 1000) * texSize_;
			//10000�̈�
			RankingTimeTexPos_[i][4] = ((int)data_.at(stage_).at(i) / 10000) * texSize_;
			//100000�̈�
			RankingTimeTexPos_[i][5] = ((int)data_.at(stage_).at(i) / 100000) * texSize_;
		}



		//�^�C�g���֖߂�
		if (gamePad.GetButtonDown() & gamePad.BTN_ENTER ||
			gamePad.GetButtonDown() & gamePad.BTN_PAD_B)
		{
			SceneManager::Instance().SetNextScene(NEXT_SCENE_OTHER);
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
		}

		break;
	}
}

//�`�揈��
void SceneClear::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();
	
	//��ʃN���A&�����_�[�^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.0f, 1.0f, 1.0f, 0.0f };
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	// 3D���f���`��
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);

		//�X�e�[�W�`��
		stageModel_->Render(dc, shader);
		//�v���C���[�`��
		player_->Render(dc, shader);

		shader->End(dc);
	}
	
	//2D�X�v���C�g�`��
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
	//�w�i
	sprBack_->Render(dc,
		0, 0,						//���W						
		1280, 720,					//�X�P�[��
		0, 0, 						//�e�N�X�`����ʒu
		1280, 720,					//�e�N�X�`���̑傫��
		0,							//�p�x
		1, 1, 1, 1);				//�F(rgb)�s�����x(a)


	//�G���^�[��B�{�^����
	sprPushKeyButton_[keyOrButton_]->Render(dc,
		1150, 590,					//���W
		110, 110,					//�X�P�[��
		0, 0, 						//�e�N�X�`����ʒu
		512, 512,					//�e�N�X�`���̑傫��
		0, 							//�p�x
		1, 1, 1, 1);				//�F(rgb)�s�����x(a)
									
	switch (sceneState_)
	{
	case 0:	//����̃��U���g��\������V�[��
		//����
		sprNext_[sceneState_]->Render(dc,
			700, 580,					//���W
			512, 128,					//�X�P�[��
			0, 0, 						//�e�N�X�`����ʒu
			512, 128,					//�e�N�X�`���̑傫��
			0, 							//�p�x
			1, 1, 1, 1);				//�F(rgb)�s�����x(a)

		//Score����
		sprScore_->Render(dc,
			pos_[SpritePram::word_score].x, pos_[SpritePram::word_score].y,					//���W
			scale_[SpritePram::word_score].x, scale_[SpritePram::word_score].y,				//�X�P�[��
			0, 0, 																			//�e�N�X�`����ʒu
			512, 128,																		//�e�N�X�`���̑傫��
			0, 																				//�p�x
			1, 1, 1, 1);																	//�F(rgb)�s�����x(a)

		//ClearTime����
		sprClearTime_->Render(dc,
			pos_[SpritePram::word_clearTime].x, pos_[SpritePram::word_clearTime].y,			//���W
			scale_[SpritePram::word_clearTime].x, scale_[SpritePram::word_clearTime].y,		//�X�P�[��
			0, 0, 																			//�e�N�X�`����ʒu
			512, 128,																		//�e�N�X�`���̑傫��
			0, 																				//�p�x
			1, 1, 1, 1);																	//�F(rgb)�s�����x(a)

		///MAXCombo����
		sprMaxCombo_->Render(dc,
			pos_[SpritePram::word_maxCombo].x, pos_[SpritePram::word_maxCombo].y,			//���W
			scale_[SpritePram::word_maxCombo].x, scale_[SpritePram::word_maxCombo].y,		//�X�P�[��
			0, 0, 																			//�e�N�X�`����ʒu
			512, 128,																		//�e�N�X�`���̑傫��
			0, 																				//�p�x
			1, 1, 1, 1);																	//�F(rgb)�s�����x(a)

		//�p�[�t�F�N�g����
		sprPerfect_->Render(dc,
			pos_[SpritePram::word_perfect].x, pos_[SpritePram::word_perfect].y,				//���W
			scale_[SpritePram::word_perfect].x, scale_[SpritePram::word_perfect].y,			//�X�P�[��
			0, 0, 																			//�e�N�X�`����ʒu
			512, 128,																		//�e�N�X�`���̑傫��
			0, 																				//�p�x
			1, 1, 1, 1);																	//�F(rgb)�s�����x(a)

		//�O���C�g����
		sprGreat_->Render(dc,
			pos_[SpritePram::word_great].x, pos_[SpritePram::word_great].y,					//���W
			scale_[SpritePram::word_great].x, scale_[SpritePram::word_great].y,				//�X�P�[��
			0, 0, 																			//�e�N�X�`����ʒu
			512, 128,																		//�e�N�X�`���̑傫��
			0, 																				//�p�x
			1, 1, 1, 1);																	//�F(rgb)�s�����x(a)


		//�X�R�A�\��
		RenderNumber(dc, &scoreTexPosX_[3], 3, pos_[SpritePram::num_score], scale_[SpritePram::num_score], 128);

		sprPoint_->Render(dc,
			pos_[SpritePram::word_pt].x, pos_[SpritePram::word_pt].y,						//���W
			scale_[SpritePram::word_pt].x, scale_[SpritePram::word_pt].y,					//�X�P�[��
			0, 0, 																			//�e�N�X�`����ʒu
			512, 128,																		//�e�N�X�`���̑傫��
			0, 																				//�p�x
			1, 1, 1, 1);																	//�F(rgb)�s�����x(a)


		//�N���A�^�C���\��
#if 1
			//���� �^�C���\��
			//10�̈�
		sprNumber_->Render(dc,
			pos_[SpritePram::num_clearTime].x - 128, pos_[SpritePram::num_clearTime].y,		//���W
			64, 64,																			//�X�P�[��
			clearTimeTexPosX_[1], 0, 														//�e�N�X�`����ʒu
			64, 64,																			//�e�N�X�`���̑傫��
			0,																				//�p�x
			1, 1, 1, 1);																	//�F(rgb)�s�����x(a)

		//1�̈�
		sprNumber_->Render(dc,
			pos_[SpritePram::num_clearTime].x - 64, pos_[SpritePram::num_clearTime].y,		//���W
			64, 64,																			//�X�P�[��
			clearTimeTexPosX_[0], 0, 														//�e�N�X�`����ʒu
			64, 64,																			//�e�N�X�`���̑傫��
			0,																				//�p�x
			1, 1, 1, 1);																	//�F(rgb)�s�����x(a)

		//�����_
		sprTimerTen_->Render(dc,
			pos_[SpritePram::num_clearTime].x - 16, pos_[SpritePram::num_clearTime].y,		//���W
			64, 64,																			//�X�P�[��
			0, 0, 																			//�e�N�X�`����ʒu
			64, 64,																			//�e�N�X�`���̑傫��
			0,																				//�p�x
			1, 1, 1, 1);																	//�F(rgb)�s�����x(a)

		//��������
		sprNumber_->Render(dc,
			pos_[SpritePram::num_clearTime].x + 32, pos_[SpritePram::num_clearTime].y,		//���W
			64, 64,																			//�X�P�[��
			clearTimeTexPosX_[2], 0, 														//�e�N�X�`����ʒu
			64, 64,																			//�e�N�X�`���̑傫��
			0,																				//�p�x
			1, 1, 1, 1);																	//�F(rgb)�s�����x(a)
#endif
		//�R���{���\��
		RenderNumber(dc, &maxComboTexPosX_[0], 2, pos_[SpritePram::num_maxCombo]);
		//�p�[�t�F�N�g���\��
		RenderNumber(dc, &perfectTexPosX_[0], 2, pos_[SpritePram::num_perfect]);
		//�O���C�g���\��
		RenderNumber(dc, &greatTexPosX_[0], 2, pos_[SpritePram::num_great]);
		break;

	case 1:
		//Title��
		sprNext_[sceneState_]->Render(dc,
			700, 580,						//���W
			512, 128,						//�X�P�[��
			0, 0, 							//�e�N�X�`����ʒu
			512, 128,						//�e�N�X�`���̑傫��
			0, 								//�p�x
			1, 1, 1, 1);					//�F(rgb)�s�����x(a)

		//�����L���O�\���̕���
		sprRanking_->Render(dc,
			pos_[SpritePram::word_score].x, pos_[SpritePram::word_score].y,				//���W
			scale_[SpritePram::word_score].x, scale_[SpritePram::word_score].y,			//�X�P�[��
			0, 0, 																		//�e�N�X�`����ʒu
			512, 128,																	//�e�N�X�`���̑傫��
			0, 																			//�p�x
			1, 1, 1, 1);																//�F(rgb)�s�����x(a)

		//�X�R�A�\��
		RenderNumber(dc, &scoreTexPosX_[3], 3, pos_[SpritePram::num_score], scale_[SpritePram::num_score], 128);

		sprPoint_->Render(dc,
			pos_[SpritePram::word_pt].x, pos_[SpritePram::word_pt].y,					//���W
			scale_[SpritePram::word_pt].x, scale_[SpritePram::word_pt].y,				//�X�P�[��
			0, 0, 																		//�e�N�X�`����ʒu
			512, 128,																	//�e�N�X�`���̑傫��
			0, 																			//�p�x
			1, 1, 1, 1);																//�F(rgb)�s�����x(a)

		//1st
		spr1st_->Render(dc,
			pos_[SpritePram::word_clearTime].x, pos_[SpritePram::word_clearTime].y,			//���W
			scale_[SpritePram::word_clearTime].x, scale_[SpritePram::word_clearTime].y,		//�X�P�[��
			0, 0, 																			//�e�N�X�`����ʒu
			512, 128,																		//�e�N�X�`���̑傫��
			0, 																				//�p�x
			1, 1, 1, 1);																	//�F(rgb)�s�����x(a)

		//2nd
		spr2nd_->Render(dc,
			pos_[SpritePram::word_maxCombo].x, pos_[SpritePram::word_maxCombo].y,			//���W
			scale_[SpritePram::word_maxCombo].x, scale_[SpritePram::word_maxCombo].y,		//�X�P�[��
			0, 0, 																			//�e�N�X�`����ʒu
			512, 128,																		//�e�N�X�`���̑傫��
			0, 																				//�p�x
			1, 1, 1, 1);																	//�F(rgb)�s�����x(a)
		//3rd
		spr3rd_->Render(dc,
			pos_[SpritePram::word_perfect].x, pos_[SpritePram::word_perfect].y,				//���W
			scale_[SpritePram::word_perfect].x, scale_[SpritePram::word_perfect].y,			//�X�P�[��
			0, 0, 																			//�e�N�X�`����ʒu
			512, 128,																		//�e�N�X�`���̑傫��
			0, 																				//�p�x
			1, 1, 1, 1);																	//�F(rgb)�s�����x(a)

		for (int i = 0; i < SHOW_RANKING; i++)
		{
			//�����l�ɏ������l�����Ă���̂ŏ����l�̏ꍇ�͕`�悵�Ȃ�
			if (data_.at(stage_).at(i) > 5)
			{
				float intarvalY = 80;

				//1�̈�
				sprNumber_->Render(dc,
					pos_[SpritePram::num_ranking].x, pos_[SpritePram::num_ranking].y + (i * intarvalY),			//���W
					64, 64,																						//�X�P�[��
					RankingTimeTexPos_[i][2], 0, 																//�e�N�X�`����ʒu
					64, 64,																						//�e�N�X�`���̑傫��
					0,																							//�p�x
					1, 1, 1, 1);																				//�F(rgb)�s�����x(a)

				//10�̈�
				sprNumber_->Render(dc,
					pos_[SpritePram::num_ranking].x + 64, pos_[SpritePram::num_ranking].y + (i * intarvalY),	//���W
					64, 64,																						//�X�P�[��
					RankingTimeTexPos_[i][1], 0, 																//�e�N�X�`����ʒu
					64, 64,																						//�e�N�X�`���̑傫��
					0,																							//�p�x
					1, 1, 1, 1);																				//�F(rgb)�s�����x(a)
				//100�̈�
				sprNumber_->Render(dc,
					pos_[SpritePram::num_ranking].x + 128, pos_[SpritePram::num_ranking].y + (i * intarvalY),	//���W
					64, 64,																						//�X�P�[��
					RankingTimeTexPos_[i][0], 0, 																//�e�N�X�`����ʒu
					64, 64,																						//�e�N�X�`���̑傫��
					0,																							//�p�x
					1, 1, 1, 1);																				//�F(rgb)�s�����x(a)

				//point
				sprPoint_->Render(dc,
					pos_[SpritePram::num_ranking].x + 192, pos_[SpritePram::num_ranking].y + (i * intarvalY) + 15,	//���W
					256, 64,																						//�X�P�[��
					0, 0, 																							//�e�N�X�`����ʒu
					512, 128,																						//�e�N�X�`���̑傫��
					0, 																								//�p�x
					1, 1, 1, 1);																					//�F(rgb)�s�����x(a)
			}
		}
		break;
	}
}


void SceneClear::SaveLoadFile()
{
	//���ʂ̃Z�[�u�ƃ��[�h
	//�ǂݍ���
	errno_t err = fopen_s(&fp_, FILENAME, "rb");
	//����N�����͕K�����s(�t�@�C�����Ȃ��̂Ő���)
	if (err != 0)
	{
		//�V�����t�@�C���𐶐�
		errno_t err = fopen_s(&fp_, FILENAME, "wb");
		if (err != 0)
			return;

		for (int i = 0; i < MAX_STAGE * MAX_STAGE_LEVEL; i++)
		{
			//�o�C�i���Ő������邩�e�L�X�g�Ő������邩
#if DATA_FORMAT			

			//�e�L�X�g�t�@�C���Ő���
			data.at(i).at(top) = 1000000 * (i + 1);
			data.at(i).at(second) = 2000000 * (i + 1);
			data.at(i).at(third) = 3000000 * (i + 1);
			data.at(i).at(save) = 4000000 * (i + 1);

			fprintf(fp, "%f %f %f %f \n",
				data.at(i).at(top), data.at(i).at(second),
				data.at(i).at(third), data.at(i).at(save));
		}
#else
			//�o�C�i���t�@�C���Ő���
			ranking_[i].top_ = 0.1f * (i );
			ranking_[i].second_ = 0.2f * (i );
			ranking_[i].third_ = 0.3f * (i );
			ranking_[i].save_ = 0.4f * (i);
		}

		std::fwrite(&ranking_, sizeof(float), (MAX_STAGE* MAX_STAGE_LEVEL) * COLUMN, fp_);	//ranking�\���̂̒l���t�@�C���ɏ�������
#endif
		std::fclose(fp_);
	}
		////�t�@�C�������݂��Ă��鎞(2��ڈȍ~))
		//�o�C�i������ǂݍ��ނ��e�L�X�g����ǂݍ��ނ�
#if DATA_FORMAT			
		//�e�L�X�g
		char line[256];	//1�s�̍ő啶����(�o�C�g)
		char str[256];	//
		while (fgets(str, 256, fp) != NULL) {
			//sscanf_s(str, "%f %f %f %f", &rank[top], &rank[second], &rank[third], &rank[save]);
			sscanf_s(str, "%f %f %f %f", &data.at(textCount).at(top), &data.at(textCount).at(second),
				&data.at(textCount).at(third), &data.at(textCount).at(save));
			textCount++;
		}
#else
		//�o�C�i��
		std::fread(&ranking_, sizeof(float), (MAX_STAGE * MAX_STAGE_LEVEL) * COLUMN, fp_);
#endif
		std::fclose(fp_);

		//�\�[�g����ׂɃx�N�^�[�z��ɑ��
		for (int i = 0; i < MAX_STAGE * MAX_STAGE_LEVEL; i++)
		{
			data_.at(i).at(top) = ranking_[i].top_;
			data_.at(i).at(second) = ranking_[i].second_;
			data_.at(i).at(third) = ranking_[i].third_;
			data_.at(i).at(save) = ranking_[i].save_;
		}

		//����̃X�e�[�W��4�Ԗڂɍ���̌��ʂ��㏑��
		data_.at(stage_).at(save) = score_;

		//����̃X�e�[�W�����\�[�g
		std::sort(data_.at(stage_).begin(), data_.at(stage_).end(), std::greater<float>());

		//fwrite�p��ranking�Ƀ\�[�g�����l����
		for (int i = 0; i < MAX_STAGE * MAX_STAGE_LEVEL; i++)
		{
			ranking_[i].top_ = data_.at(i).at(top);
			ranking_[i].second_ = data_.at(i).at(second);
			ranking_[i].third_ = data_.at(i).at(third);
			ranking_[i].save_ = data_.at(i).at(save);
		}

		//���ʂ̍X�V�K����̏�������
		err = fopen_s(&fp_, FILENAME, "wb");
		if (err != 0)
			return;
		//�o�C�i���ŏ������ނ��e�L�X�g�ŏ������ނ�
#if  DATA_FORMAT
		for (int i = 0; i < MAX_STAGE * MAX_STAGE_LEVEL; i++)
		{
			fprintf(fp, "%f %f %f %f \n",
				data.at(i).at(top), data.at(i).at(second),
				data.at(i).at(third), data.at(i).at(save));
		}
		fprintf(fp, "top[%f],second[%f],third[%f]�Asave[%f]",
#else
		std::fwrite(&ranking_, sizeof(float), (MAX_STAGE * MAX_STAGE_LEVEL) * COLUMN, fp_);	//ranking�\���̂̒l���t�@�C���ɏ�������
#endif
		std::fclose(fp_);

}

//�������X�v���C�g�ŕ\������
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

//�X�v���C�g�̃p�����[�^�X�V����
void SceneClear::SpriteParameterUpdate(float elapsedTime)
{
	//���Ɉړ��ύX����p�����[�^�͖����̂ŏ����͂��Ȃ�&�Ă΂Ȃ�
}

//Ui�ύX����
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