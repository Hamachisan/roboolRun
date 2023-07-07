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

// ������
void SceneGame::Initialize()
{
	//�X�e�[�W������
	stage_ = new Stage();

	//�v���C���[������
	player_ = new Player();

	//�|�[�Y������
	pause_ = new Pause();

	//�X�R�A������
	Score::Instance().Init();

#ifdef _DEBUG
#else
#endif
	//�w�i�I�u�W�F�N�g����
	//��BackObjManager�ɓo�^����͕̂K��RockObj����Ԏn�߂�
	//����
	for (int i = 0; i < BACK_OBJ_MAX; i++)
	{
		RockObj* rock = new RockObj();
		rock->SetPosition(DirectX::XMFLOAT3(STANDARD_POTITION_X - 20, 0, START_LINE + i * BACK_OBJ_INTERVAL_Z));
		//�o�^
		BackObjManager::Instance().Register(rock);
	}
	//�E��
	for (int i = 0; i < BACK_OBJ_MAX; i++)
	{
		RockObj* rock = new RockObj();
		rock->SetPosition(DirectX::XMFLOAT3(STANDARD_POTITION_X + INTERVAL_X * 3 + 10, 0, START_LINE + i * BACK_OBJ_INTERVAL_Z));
		//�o�^
		BackObjManager::Instance().Register(rock);
	}

	//�X�e�[�W�ɓG�I�u�W�F�N�g�z�u
	EnemySetStage();

	//�S�[����Obj
	GoalObj* goal = new GoalObj();
	goal->SetPosition(DirectX::XMFLOAT3(0, 0, GOAL_LINE));
	BackObjManager::Instance().Register(goal);

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

	//�t�H���g
	Textsprites_[0] = std::make_unique<Sprite>(".\\Data\\Font\\font4.png");

	//�X�v���C�g�̏�����
	SpriteInitialize();	
	//�p�����[�^�[������
	ParameterInitialize();

	//����������
	directionalLight_ = std::make_unique<Light>(LightType::Directional);

	//Audio������
	bgmGame_		= Audio::Instance().LoadAudioSource("Data/Audio/BGM/Game/saiba.wav");
	seCountDown_	= Audio::Instance().LoadAudioSource("Data/Audio/SE/Scene/Countdown01.wav");
	seFinish_		= Audio::Instance().LoadAudioSource("Data/Audio/SE/Scene/finish.wav");

	//�X�J�C�{�b�N�X�e�N�X�`���̓ǂݍ���
	skyboxTexture_ = std::make_unique<Texture>("Data/SkyTexture/sky.jpg");
	isSpriteShader_ = true;
	skyboxRenderer_ = std::make_unique<Sprite>(isSpriteShader_);

	//�I�t�X�N���[�������_�����O
	sprSkyTexture_ = std::make_unique<Texture>("Data/SkyTexture/skyTexture.png");
	sprite_ = std::make_unique<Sprite>(1);
	//�V�����`��^�[�Q�b�g�̐���
	{
		renderTarget_ = std::make_unique<RenderTarget>(static_cast<UINT>(graphics.GetScreenWidth()), 
			static_cast<UINT>(graphics.GetScreenHeight()), 
			DXGI_FORMAT_R8G8B8A8_UNORM);
	}

}

//�p�����[�^�[������
void SceneGame::ParameterInitialize()
{
	gameStartTime_ = 3.0f;

	//Go!!
	GoPosX_ = 1280 * 0.48 - 256;


	//�O���f�[�V�����p�ϐ�
	rStart_ = 0;		//��
	gStart_ = 1;
	bStart_ = 0;
	rEnd_ = 1;			//��
	gEnd_ = 0;
	bEnd_ = 0;

	//�����֘A������
	ambientLightColor_		= { 0.3, 0.3, 0.3, 1 };	//��	//�|�C���g���C�g
	pointLightColor_		= { 1, 1, 1, 1 };
	pointLightPosition_		= { 0, 0, 0, 0 };
	pointLightRange_		= 100;

}

//�X�v���C�g������
void SceneGame::SpriteInitialize()
{
	//�X�v���C�g
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

	//�^�C�~���O�̐��x
	sprPerfect_				= std::make_unique<Sprite>("Data/Sprite/Game/Timing/PERFECT3.png");
	sprGreat_				= std::make_unique<Sprite>("Data/Sprite/Game/Timing/GREAT.png");
	sprGood_				= std::make_unique<Sprite>("Data/Sprite/Game/Timing/GOOD.png");
	//�R���{�\��
	sprCombo_				= std::make_unique<Sprite>("Data/Sprite/Game/Timing/Combo.png");
	sprComboConut_			= std::make_unique<Sprite>("Data/Sprite/number.png");
	sprComboFrame_			= std::make_unique<Sprite>("Data/Sprite/Game/Timing/comboBack.png");


	//������@�̃t���[��
	sprUiFrame_				= std::make_unique<Sprite>("Data/Sprite/Game/UI/UiFrame.png");
	//������@�̌��ɒu��
	sprUiBack_				= std::make_unique<Sprite>("Data/Sprite/Game/UI/back.png");
	//��Q���̊G
	sprUiObjs_[0]			= std::make_unique<Sprite>("Data/Sprite/Game/UI/immortalObj.png");
	sprUiObjs_[1]			= std::make_unique<Sprite>("Data/Sprite/Game/UI/sutterObj.png");
	sprUiObjs_[2]			= std::make_unique<Sprite>("Data/Sprite/Game/UI/squatObj.png");
	sprUiObjs_[3]			= std::make_unique<Sprite>("Data/Sprite/Game/UI/jumpObj.png");
	//�L�[�A�{�^���z�u
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

//�G���X�e�[�W�ɔz�u
void SceneGame::EnemySetStage()
{
	//�G�l�~�[�̔z�u�z�񏉊���
	enemyArrangement_ = new EnemyArrangement(SceneManager::Instance().GetStageNum());

	//�G�l�~�[�}�l�[�W���[������
	EnemyManager& eManager = EnemyManager::Instance();

	//�G�l�~�[�z�u
	for (int i = 0; i < ENEMY_LINE; i++)
	{
		for (int j = 0; j < ENEMY_COLUMN; j++)
		{
			//�X�e�[�W�I��
			stageLevel_ = enemyArrangement_->stage_[i][j].num;

			//��������G�l�~�[
			switch (stageLevel_)
			{
			case 0:	//�j��s�\
			{
				EnemyImmortalObj* immortal = new EnemyImmortalObj();
				immortal->SetPosition(DirectX::XMFLOAT3(STANDARD_POTITION_X + INTERVAL_X * j, 0, OBJ_START_LINE + i * INTERVAL_Z));
				eManager.Register(immortal);
			}
			break;
			case 1:	//������΂�
			{
				EnemySutterObj* sutter = new EnemySutterObj();
				sutter->SetPosition(DirectX::XMFLOAT3(STANDARD_POTITION_X + INTERVAL_X * j, 0, OBJ_START_LINE + i * INTERVAL_Z));
				eManager.Register(sutter);

				TimingObj* timing = new TimingObj();
				timing->SetPosition(DirectX::XMFLOAT3(STANDARD_POTITION_X + INTERVAL_X * j, 0, OBJ_START_LINE + i * INTERVAL_Z - 11.0f));
				//�o�^
				BackObjManager::Instance().Register(timing);
			}
			break;
			case 2:	//�X���C�f�B���O
			{
				EnemySquatObj* squat = new EnemySquatObj();
				squat->SetPosition(DirectX::XMFLOAT3(STANDARD_POTITION_X + INTERVAL_X * j, 0, OBJ_START_LINE + i * INTERVAL_Z));
				eManager.Register(squat);

				TimingObj* timing = new TimingObj();
				timing->SetPosition(DirectX::XMFLOAT3(STANDARD_POTITION_X + INTERVAL_X * j, 0, OBJ_START_LINE + i * INTERVAL_Z - 4.0f));
				//�o�^
				BackObjManager::Instance().Register(timing);
			}
			break;
			case 3:	//�W�����v
			{
				EnemyJumpObj* jump = new EnemyJumpObj();
				jump->SetPosition(DirectX::XMFLOAT3(STANDARD_POTITION_X + INTERVAL_X * j, 0, OBJ_START_LINE + i * INTERVAL_Z));
				eManager.Register(jump);

				TimingObj* timing = new TimingObj();
				timing->SetPosition(DirectX::XMFLOAT3(STANDARD_POTITION_X + INTERVAL_X * j, 0, OBJ_START_LINE + i * INTERVAL_Z - 9.5f));
				//�o�^
				BackObjManager::Instance().Register(timing);
			}
			break;
			case 4:	//�z�u���Ȃ�
				break;
			}


		}
	}
}

// �I����
void SceneGame::Finalize()
{
	//�X�e�[�W�I����
	SafeDelete(stage_);

	//�v���C���[�I����
	SafeDelete(player_);

	//�G�l�~�[�̔z�u�z��I����
	SafeDelete(enemyArrangement_);

	//�G�l�~�[�I����
	EnemyManager::Instance().Clear();

	//�w�i�I�u�W�F�N�g�I����
	BackObjManager::Instance().Clear();
	//�J�����R���g���[���[�I����
	SafeDelete(cameraController_);
	
	//�|�[�Y�I����
	SafeDelete(pause_);
}

// �X�V����
void SceneGame::Update(float elapsedTime)
{
	//�R���g���[���[���q����Ă�����\������UI��ύX
	ChangeUi();

	//�X�J�C�{�b�N�X�̐ݒ���}���`�X���b�h�ŏՓ˂��N�����Ȃ��pInitialize�ł͖���
	//Update�Őݒ肷��
	ThreadCover();


	//�|�[�Y����
	if (!PauseProcess(elapsedTime))
		return;
	
	//�Q�[�����X�^�[�g����ׂ̃^�C�}�[
	gameStartTime_ -= elapsedTime;

	///�Q�[���J�n
	if (gameStartTime_ < 0)
	{
		//�v���C���[�̑ҋ@�A�j���[�V�������I��
		player_->SetGameStatrtPlayer(true);

		//BGM�Đ�
		bgmGame_->Play(true, 0.5f);
		
		//�Q�[���N���A���Ă��Ȃ��Ƃ�
		if (!player_->StageClear())
		{
			gameTimer_ += elapsedTime;

			//�t���[�J�����N��
			if (isFreeCamera_)
			{
				FreeCamera(elapsedTime);
				target_ = freeCameraPosition_;
			}
			//�N�����Ă��Ȃ�
			else 
			{
				//�J�����R���g���[���[�X�V����
				target_ = player_->GetPosition();
				target_.y += 3.5f;
				freeCameraPosition_ = player_->GetPosition();

				//�v���C���[�X�V����
				animState_ = player_->GetPlayerAnimations();
				player_->Update(elapsedTime);
				//�|�C���g���C�g�̈ړ�
				pointLightPosition_.x = 0;
				pointLightPosition_.y = 50.0f;
				pointLightPosition_.z = player_->GetPosition().z - 4;
				pointLightPosition_.w = 0;
			}

			cameraController_->SetTarget(target_);
			cameraController_->Update(elapsedTime);

			//�X�e�[�W�X�V����
			stage_->Update(elapsedTime);

			//�G�l�~�[�X�V����
			EnemyManager::Instance().Update(elapsedTime);

			//�w�i�I�u�W�F�N�g�X�V����
			BackObjManager::Instance().Update(elapsedTime);

			//�G�t�F�N�g�X�V����
			EffectManager::Instane().Update(elapsedTime);

			//�X�v���C�g�̃p�����[�^�X�V����
			SpriteParameterUpdate(elapsedTime);

			//�Փ˂Ɠ����ɃJ�����V�F�C�N���s��
			if (player_->GetCameraShakeFlag())
			{
				cameraController_->SetCameraShake(true);
			}
			else
			{
				cameraController_->SetCameraShake(false);
			}

			//�w�i�I�u�W�F�N�g�̈ړ�
			MoveBackObject();
		}
		//�N���A������
		else
		{
			//�J�����̍X�V�����߂ăN���A����������

			gameClearTime_ += elapsedTime;

			//�X�R�A�ݒ�
			Score::Instance().SetClearTime(gameTimer_);
			Score::Instance().SetMaxCombo(player_->GetMaxCombo());
			Score::Instance().SetPerfectConut(player_->perfectCount_);
			Score::Instance().SetGreatCount(player_->greatCount_);
			Score::Instance().SetGoodCount(player_->goodCount_);

			//�X�e�[�W�X�V����
			stage_->Update(elapsedTime);

			//�v���C���[�X�V����
			animState_ = player_->GetPlayerAnimations();
			player_->Update(elapsedTime);

			//�G�l�~�[�X�V����
			EnemyManager::Instance().Update(elapsedTime);

			//�w�i�I�u�W�F�N�g�X�V����
			BackObjManager::Instance().Update(elapsedTime);

			//�G�t�F�N�g�X�V����
			EffectManager::Instane().Update(elapsedTime);

			//���ʉ��Đ�
			if(seCount_ == 0)
				seFinish_->Play(false);
			seCount_++;

			//2�b�㌋�ʉ�ʂ֑J�ڂ�����
			if (gameClearTime_ > 2.0f)
			{
				constexpr float CLEAR_DAMAGE = 10.0f;
				constexpr float INVINCIBLE = 0.2f;

				bgmGame_->Stop();
				
				//�w�i�I�u�W�F�N�g����������
				int objCount = BackObjManager::Instance().GetBackObjCount();
				for (int i = 0; i < objCount; ++i)
				{
					BackObj* back = BackObjManager::Instance().GetBackObj(i);
					back->ApplyDmaege(CLEAR_DAMAGE, INVINCIBLE);
				}
				//���ł�erase������ʂ����߂�Update������
				BackObjManager::Instance().Update(elapsedTime);

				//�V�[���J��
				SceneManager::Instance().SetNextScene(NEXT_SCENE_OTHER);
				SceneManager::Instance().ChangeScene(new SceneLoading(new SceneClear));
			}
		}



	}
	//�Q�[���J�n�O�̏���
	else
	{
		//�^�C�}�[(�J�E���g�_�E��)�`�揈��
		//1�̈�
		timerTexPosX_[0] = (int)gameStartTime_ * texSize_ + 64;

		//�ҋ@�A�j���[�V�������X�V
		player_->Update(elapsedTime);
		
		//�J�E���g�_�E����
		seCountDown_->Play(false);

		//1�t���[������Update��ʂ��X�e�[�W���J�E���g�_�E�����ł�������悤��
		if(oneFrameCount == 0)
			OneFrameUpdate(elapsedTime);
		oneFrameCount++;


	}

	//�J�E���g�_�E����SE��2��Ȃ��Ă��܂��̂�j�~
	if (gameStartTime_ < -1.0f && gameStartTime_ > -2.0f)
	{
		seCountDown_->Stop();
	}

}

// �`�揈�� (�܂Ƃ�)
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	//ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	//ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	//FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
	//dc->ClearRenderTargetView(rtv, color);
	//dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//dc->OMSetRenderTargets(1, &rtv, dsv);


	// �`�揈��
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ���C�g�����i�������j

	//�J�����p�����[�^�ݒ�	
	//�������g��
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


	// 3D��Ԃ̕`���ʂ̃o�b�t�@�ɑ΂��čs��
	Render3DScene();

	// �������ݐ���o�b�N�o�b�t�@�ɕς��ăI�t�X�N���[�������_�����O�̌��ʂ�`�悷��
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();
	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	//�F���␳
	SpriteShader* shader = graphics.GetSpriteShader(SpriteShaderId::ColorGrading);

	//�F���␳�������͕��˃u���[
	if (!isSpriteShaderSet_)
	{

		rc.colorGradingData = colorGradingData_;
		shader->Begin(rc);
		shader->Draw(rc, sprite_.get());	//������Drow��3D���f����`�悵�Ă���
		shader->End(rc);
	}
	else
	{
		//���˃u���[
		shader = graphics.GetSpriteShader(SpriteShaderId::RadialBlur);
		rc.radialBlurData = radialBlurData_;
		shader->Begin(rc);
		shader->Draw(rc, sprite_.get());	//������Drow��3D���f����`�悵�Ă���
		shader->End(rc);
	}


	// 2D�X�v���C�g�`��
	{
		UiRender(dc);

		//�|�[�Y��ʕ`��
		if (isPause_)
			pause_->Render(dc);

	}



#ifdef _DEBUG
	// 3D�f�o�b�O�`��
	{
		//�v���C���[�f�o�b�O�v���~�e�B�u�`��
		player_->DrawDebugPrimitive();

		//�G�l�~�[�f�o�b�O�v���~�e�B�u�`��
		EnemyManager::Instance().DrawDebugPrimitive();

		// ���C�������_���`����s
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// �f�o�b�O�����_���`����s
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);

		//�������������Ƃ������C�g��ImGui
		directionalLight_->DrawDebugGUI();
	}
#else
#endif


#ifdef _DEBUG
	// 2D�f�o�b�OGUI�`��
	{
		//�v���C���[�f�o�b�O�`��
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

			//���s�����̕����ƐF
			if (ImGui::CollapsingHeader("directional", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::SliderFloat("directionalX", &directional_.x, -1.0f, 1.0f);
				ImGui::SliderFloat("directionalY", &directional_.y, -1.0f, 1.0f);
				ImGui::SliderFloat("directionalZ", &directional_.z, -1.0f, 1.0f);
				ImGui::ColorEdit3("color", &ambientLightColor_.x);
			}

			//�|�C���g���C�g
			if (ImGui::CollapsingHeader("pointLight", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::SliderFloat("pointLightPosX", &pointLightPosition_.x, -500, 1000);
				ImGui::SliderFloat("pointLightPosY", &pointLightPosition_.y, -10, 50);
				ImGui::SliderFloat("pointLightPosZ", &pointLightPosition_.z, -500, 1000);
				ImGui::ColorEdit3("pointLightColor", &pointLightColor_.x);
				ImGui::SliderFloat("PointLightRange", &pointLightRange_, 0.0f, 100.0f);
			}

			//�t���[�J����
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

//3D��Ԃ�`��
void SceneGame::Render3DScene()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = renderTarget_->GetRenderTargetView().Get();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// �r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT	vp = {};
	vp.Width = graphics.GetScreenWidth();
	vp.Height = graphics.GetScreenHeight();
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	dc->RSSetViewports(1, &vp);

	// �`�揈��
	RenderContext rc;
	rc.deviceContext = dc;

	//�J�����p�����[�^�ݒ�	
	//�������g��
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

	//�X�J�C�{�b�N�X�`��
	{

		SpriteShader* shader = graphics.GetSpriteShader(SpriteShaderId::Skybox);
		shader->Begin(rc);

		shader->Draw(rc, skyboxRenderer_.get());

		shader->End(rc);

	}

	//���C�g�ݒ�
	SettingLight(dc, rc);
	PlayerSettingLight(rc);

	// 3D���f���`��
	{
		Shader* shader = graphics.GetShader();
		//shader->Begin(dc, rc);
		//shader->End(dc);


		//�V�F�[�_�[�̐؂�ւ�
		shader = graphics.GetPhongShader();
		shader->Begin(dc, rc);

		//�G�l�~�[�`��
		EnemyManager::Instance().Render(dc, shader);

		//�X�e�[�W�`��
		stage_->Render(dc, shader);

		//�w�i�I�u�W�F�N�g�`��
		BackObjManager::Instance().Render(dc, shader);

		//goal->Render(dc, shader);

		//�v���C���[�`��
		player_->Render(dc, shader);

		shader->End(dc);

	}

	// 3D�G�t�F�N�g�`��
	{
		EffectManager::Instane().Render(rc.view, rc.projection);
	}

}

//UI�`��
void SceneGame::UiRender(ID3D11DeviceContext* dc)
{
	//�J�E���g�_�E���̃^�C�}�[(�Q�[�����X�^�[�g)
	if (gameStartTime_ > 0)
	{
		sprTimer_->Render(dc,
			SCREEN_WIDTH * ABOUT_HALF - PIXEL_64,		//X���W
			SCREEN_HEIGHT * ABOUT_HALF - PIXEL_64, 		//Y���W
			PIXEL_128, PIXEL_128,						//�摜�T�C�Y
			timerTexPosX_[0], 0, PIXEL_64, PIXEL_64,	//�摜�ǂݍ��݈ʒu�Ɠǂݍ��݃T�C�Y
			0,											//�p�x
			1, 1, 1, 1);								//�F
	}
	//�Q�[����
	else
	{
		
		//GO
		sprGo_->Render(dc,
			GoPosX_,									//X���W
			SCREEN_HEIGHT * ABOUT_HALF - PIXEL_64,		//Y���W
			PIXEL_512, PIXEL_64 * 3,					//�摜�T�C�Y
			0, 0, PIXEL_512, PIXEL_256,					//�摜�ǂݍ��݈ʒu�Ɠǂݍ��݃T�C�Y
			0,											//�p�x
			1, 1, 1, 1);								//�F

		//�^�C�}�[�̘g
		sprTimerFrame_->Render(dc,
			SCREEN_WIDTH * 0.5 - 105,					//X���W
			0.0f,										//Y���W
			PIXEL_256, PIXEL_128,						//�摜�T�C�Y
			0, 0, PIXEL_128, PIXEL_64,					//�摜�ǂݍ��݈ʒu�Ɠǂݍ��݃T�C�Y
			0,											//�p�x
			1, 1, 1, 0.5);								//�F

		//��ʏ�z�u���錻�݂̎���
		{
			//10�̈�
			sprTimer_->Render(dc,
				SCREEN_WIDTH * ABOUT_HALF - texSize_,		//X���W
				30.0f, 										//Y���W
				PIXEL_64, PIXEL_64,							//�摜�T�C�Y
				timerTexPosX_[1], 0, PIXEL_64, PIXEL_64,	//�摜�ǂݍ��݈ʒu�Ɠǂݍ��݃T�C�Y
				0,											//�p�x
				1, 1, 1, 1);								//�F
			//1�̈�
			sprTimer_->Render(dc,
				SCREEN_WIDTH * ABOUT_HALF,					//X���W
				30.0f,										//Y���W
				PIXEL_64, PIXEL_64,							//�摜�T�C�Y
				timerTexPosX_[0], 0, PIXEL_64, PIXEL_64,	//�摜�ǂݍ��݈ʒu�Ɠǂݍ��݃T�C�Y
				0,											//�p�x
				1, 1, 1, 1);								//�F
			//�����_
			sprTimerTen_->Render(dc,
				SCREEN_WIDTH * ABOUT_HALF + texSize_ - 25,	//X���W
				35.0f,										//Y���W
				PIXEL_64, PIXEL_64,							//�摜�T�C�Y
				0, 0, PIXEL_64, PIXEL_64,					//�摜�ǂݍ��݈ʒu�Ɠǂݍ��݃T�C�Y
				0,											//�p�x
				1, 1, 1, 1);								//�F
			//��������
			sprTimer_->Render(dc,
				SCREEN_WIDTH * 0.5 + PIXEL_64,				//X���W
				30.0f, 										//Y���W
				64, 64,										//�摜�T�C�Y
				timerTexPosX_[2], 0, PIXEL_64, PIXEL_64,	//�摜�ǂݍ��݈ʒu�Ɠǂݍ��݃T�C�Y
				0,											//�p�x
				1, 1, 1, 1);								//�F
		}
	}

	//�X�s�[�h���[�^�[
	{
		//�X�s�[�h(����)
		sprSpeed_->Render(dc,								
			20,												//X���W
			580, 											//Y���W
			768 * 0.5f, PIXEL_256 * 0.2f,					//�摜�T�C�Y
			0, 0, 768, PIXEL_256,							//�摜�ǂݍ��݈ʒu�Ɠǂݍ��݃T�C�Y
			0,												//�p�x
			1, 1, 1, 1);									//�F

		//�X�s�[�h���[�^�[
		sprSpeedMeter_->Render(dc,
			20,												//X���W
			630, 											//Y���W
			meterTexX_, PIXEL_256 * 0.2,					//�摜�T�C�Y
			0, 0, 768, PIXEL_256,							//�摜�ǂݍ��݈ʒu�Ɠǂݍ��݃T�C�Y
			0,												//�p�x
			r_, g_, b_, 1);									//�F
		
		//�X�s�[�h���[�^�[�̘g
		sprSpeedMeterFrame_->Render(dc,
			20,												//X���W
			630, 											//Y���W
			768 * 0.5, PIXEL_256 * 0.2,						//�摜�T�C�Y
			0, 0, 768, PIXEL_256,							//�摜�ǂݍ��݈ʒu�Ɠǂݍ��݃T�C�Y
			0,												//�p�x
			1, 1, 1, 1);									//�F

		//�X�s�[�h���ő�ɒB������\������
		if (speed_ >= maxSpeed_)
		{
			if (flashingTime_ / 24 % 2)					
				sprSpeedMax_->Render(dc,
					20,										//X���W
					630, 									//Y���W
					768 * 0.5, PIXEL_256 * 0.2,				//�摜�T�C�Y
					0, 0, 768, PIXEL_256,					//�摜�ǂݍ��݈ʒu�Ɠǂݍ��݃T�C�Y
					0,										//�p�x
					1, 1, 1, 1);							//�F
		}
	}

	//������@��UI��\��
	{
		//�v���C���[�̑�����͏󋵂ɉ����đ����������������
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
		//�v���C���[�����s�ȊO�̃A�j���[�V�������s���Ă��鎞��
		if (!player_->isAnimMove_)
		{
			//�������邭����ׂ̃X�v���C�g
			sprUiBack_->Render(dc,	
				posx,										//X���W
				-20, 										//Y���W
				128, 120,									//�摜�T�C�Y
				0, 0, PIXEL_512, PIXEL_256,					//�摜�ǂݍ��݈ʒu�Ɠǂݍ��݃T�C�Y
				0, 											//�p�x
				1, 1, 1, 1);								//�F
		}

		//���[����

		//���E�ړ��I�u�W�F�N�g
		{
			//�t���[��
			sprUiFrame_->Render(dc,
				0,											//X���W		
				-20, 										//Y���W
				128, 120,									//�摜�T�C�Y
				0, 0, 128, 64,								//�摜�ǂݍ��݈ʒu�Ɠǂݍ��݃T�C�Y
				180, 										//�p�x
				1, 1, 1, 1);								//�F
			//��Q��
			sprUiObjs_[0]->Render(dc,
				10,											//X���W
				16, 										//Y���W
				48, 48,										//�摜�T�C�Y
				0, 0, 512, 512,								//�摜�ǂݍ��݈ʒu�Ɠǂݍ��݃T�C�Y
				0, 											//�p�x
				1, 1, 1, 1);								//�F
			//�L�[�A�������̓{�^��
			sprUiOperations_[anyKeyOrButton_[0]][whichOperation_]->Render(dc,
				64,											//X���W
				0, 											//Y���W
				48, 48,										//�摜�T�C�Y
				0, 0, 512, 512,								//�摜�ǂݍ��݈ʒu�Ɠǂݍ��݃T�C�Y
				0, 											//�p�x
				1, 1, 1, 1									//�F
			);
			//�L�[�A�������̓{�^��
			sprUiOperations_[anyKeyOrButton_[1]][whichOperation_]->Render(dc,
				64,											//X���W
				48, 										//Y���W
				48, 48,										//�摜�T�C�Y
				0, 0, 512, 512,								//�摜�ǂݍ��݈ʒu�Ɠǂݍ��݃T�C�Y
				0,											//�p�x
				1, 1, 1, 1									//�F
			);
		}
		//������΂��I�u�W�F�N�g
		{
			//�t���[��
			sprUiFrame_->Render(dc,
				128,										//X���W
				-20,										//Y���W
				128, 120,									//�摜�T�C�Y
				0, 0, 128, 64,								//�摜�ǂݍ��݈ʒu�Ɠǂݍ��݃T�C�Y
				180,										//�p�x
				1, 1, 1, 1);								//�F
			//��Q��
			sprUiObjs_[1]->Render(dc,
				128 + 10,									//X���W
				16, 										//Y���W
				48, 48,										//�摜�T�C�Y
				0, 0, 512, 512,								//�摜�ǂݍ��݈ʒu�Ɠǂݍ��݃T�C�Y
				0, 											//�p�x
				1, 1, 1, 1);								//�F
			//�L�[�A�������̓{�^��
			sprUiOperations_[anyKeyOrButton_[2]][whichOperation_]->Render(dc,
				128 + 64,									//X���W
				16, 										//Y���W
				48, 48,										//�摜�T�C�Y
				0, 0, 512, 512,								//�摜�ǂݍ��݈ʒu�Ɠǂݍ��݃T�C�Y
				0, 											//�p�x
				1, 1, 1, 1									//�F
			);
		}
		//����I�u�W�F�N�g
		{
			//�t���[��
			sprUiFrame_->Render(dc,
				256,										//X���W
				-20, 										//Y���W
				128, 120,									//�摜�T�C�Y
				0, 0, 128, 64,								//�摜�ǂݍ��݈ʒu�Ɠǂݍ��݃T�C�Y
				180,										//�p�x
				1, 1, 1, 1);								//�F

			//��Q��
			sprUiObjs_[2]->Render(dc,
				256 + 10,									//X���W
				16, 										//Y���W
				48, 48,										//�摜�T�C�Y
				0, 0, 512, 512,								//�摜�ǂݍ��݈ʒu�Ɠǂݍ��݃T�C�Y
				0, 											//�p�x
				1, 1, 1, 1);								//�F

			//�L�[�A�������̓{�^��
			sprUiOperations_[anyKeyOrButton_[3]][whichOperation_]->Render(dc,
				256 + 64,									//X���W
				16, 										//Y���W
				48, 48,										//�摜�T�C�Y
				0, 0, 512, 512,								//�摜�ǂݍ��݈ʒu�Ɠǂݍ��݃T�C�Y
				0, 											//�p�x
				1, 1, 1, 1);								//�F
				
		}
		//�W�����v�I�u�W�F�N�g
		{
			//�t���[��
			sprUiFrame_->Render(dc,
				384,										//X���W
				-20, 										//Y���W
				128, 120,									//�摜�T�C�Y
				0, 0, 128, 64,								//�摜�ǂݍ��݈ʒu�Ɠǂݍ��݃T�C�Y
				180,										//�p�x
				1, 1, 1, 1);								//�F

			//��Q��
			sprUiObjs_[3]->Render(dc,
				384 + 10,									//X���W
				16, 										//Y���W
				48, 48,										//�摜�T�C�Y
				0, 0, 512, 256,								//�摜�ǂݍ��݈ʒu�Ɠǂݍ��݃T�C�Y
				0,											//�p�x
				1, 1, 1, 1);								//�F
				
			//�L�[�A�������̓{�^��
			sprUiOperations_[anyKeyOrButton_[4]][whichOperation_]->Render(dc,
				384 + 64,									//X���W
				16, 										//Y���W
				48, 48,										//�摜�T�C�Y
				0, 0, 512, 512,								//�摜�ǂݍ��݈ʒu�Ɠǂݍ��݃T�C�Y
				0, 											//�p�x
				1, 1, 1, 1);								//�F
		}

	}

	//�^�C�~���O�̐��x��\��
	if (player_->isTimingRender_)
	{
		if (player_->timing_ == player_->PERFECT)
		{
			sprPerfect_->Render(dc,
				470,								//X���W
				370, 								//Y���W
				384, 96,							//�摜�T�C�Y
				0, 0, 512, 128,						//�摜�ǂݍ��݈ʒu�Ɠǂݍ��݃T�C�Y
				0, 									//�p�x
				1, 1, 1, 1);						//�F
		}
		if (player_->timing_ == player_->GREAT)
		{
			sprGreat_->Render(dc,
				470,								//X���W
				370, 								//Y���W
				384, 96,							//�摜�T�C�Y
				0, 0, 512, 128,						//�摜�ǂݍ��݈ʒu�Ɠǂݍ��݃T�C�Y
				0, 									//�p�x
				1, 1, 1, 1);						//�F
		}
		if (player_->timing_ == player_->GOOD)
		{
			sprGood_->Render(dc,
				470,								//X���W
				370, 								//Y���W
				384, 96,							//�摜�T�C�Y
				0, 0, 512, 128,						//�摜�ǂݍ��݈ʒu�Ɠǂݍ��݃T�C�Y
				0,									//�p�x
				1, 1, 1, 1);						//�F
		}
	}

	//�R���{�̘g
	sprComboFrame_->Render(dc,
		820,							//X���W
		-5, 							//Y���W
		400, 100,						//�摜�T�C�Y
		0, 0, 512, 256,					//�摜�ǂݍ��݈ʒu�Ɠǂݍ��݃T�C�Y
		0, 								//�p�x
		1, 1, 1, 1);					//�F

	//�R���{�\��
	sprCombo_->Render(dc,
		950,							//X���W
		20, 							//Y���W
		256, 68,						//�摜�T�C�Y
		0, 0, 256, 64,					//�摜�ǂݍ��݈ʒu�Ɠǂݍ��݃T�C�Y
		0, 								//�p�x
		1, 1, 1, 1);					//�F
	//�R���{1�̈�
	sprComboConut_->Render(dc,		
		855,							//X���W
		20, 							//Y���W
		60, 60,							//�摜�T�C�Y
		comboTexPosX_[1], 0, 64, 64,	//�摜�ǂݍ��݈ʒu�Ɠǂݍ��݃T�C�Y
		0, 								//�p�x
		1, 1, 1, 1);					//�F
	//�R���{10�̈�
	sprComboConut_->Render(dc,
		914,							//X���W
		20, 							//Y���W
		60, 60,							//�摜�T�C�Y
		comboTexPosX_[0], 0, 64, 64,	//�摜�ǂݍ��݈ʒu�Ɠǂݍ��݃T�C�Y
		0, 								//�p�x
		1, 1, 1, 1);					//�F

	//�Փ˂������ɐԂ��G�t�F�N�g�\��
	if (player_->isImpulse_)
	{
		//if (ImpulsTimer < 0.5f)
		sprImpuls_->Render(dc,	
			0,								//X���W
			0,								//Y���W
			1280, 720,						//�摜�T�C�Y
			0, 0, 1920, 1080,				//�摜�ǂݍ��݈ʒu�Ɠǂݍ��݃T�C�Y
			0,								//�p�x
			1, 1, 1, 1);					//�F

	}

	//�Q�[�����I��������
	if (player_->StageClear())
	{
		//FINISH
		sprFinish_->Render(dc,
			0,								//X���W
			0,								//Y���W
			1280, 720,						//�摜�T�C�Y
			0, 0, 1280, 720,				//�摜�ǂݍ��݈ʒu�Ɠǂݍ��݃T�C�Y
			0,								//�p�x
			1, 1, 1, 1);					//�F
	}
}

//�w�i�I�u�W�F�N�g���ړ�������֐�
void SceneGame::MoveBackObject()
{
	//�I�u�W�F�N�g�����������炵�����̂Ńv���C���[���ʂ肷�����w�i�I�u�W�F�N�g�͈�Ԍ��Ɉړ�������
	//�w�i�I�u�W�F�N�g����
	//�J�E���g
	for (int i = 0; i < BACK_OBJ_MAX * 2; ++i)
	{
		//�v���C���[�Ɣ�r����I�u�W�F�N�g
		BackObj* rock1 = BackObjManager::Instance().GetBackObj(i);
		//��r
		float rockPosZ = rock1->GetPosition().z;
		float playerPosZ = player_->GetPosition().z;

		//		��2�@��Q��  ��5		��
		//		��2�@��Q��  ��5		��
		//		��2�@��Q��  ��5		��
		//		��1�@��Q��  ��4		��
		//		��1�@��Q��  ��4		��
		//		��1�@��Q��  ��4		��
		//		��0�@��Q��  ��3		��	�i�s����
		//		��0�@��Q���@��3		��
		//		��0�@��Q���@��3		��

		//����...3�܂�)
		if (BACK_OBJ_MAX > i)
		{
			if (playerPosZ > rockPosZ + BACK_OBJ_INTERVAL_Z)
			{
				//��r�����I�u�W�F�N�g��z�u�����ɂ���I�u�W�F�N�g
				BackObj* rock2;
				//�����ԑO�̃I�u�W�F�N�g�̎��͍���̈�ԍŌ�̃I�u�W�F�N�g���擾
				if (i == LEFT_FRONT_LINE)
					rock2 = BackObjManager::Instance().GetBackObj(LEFT_FRONT_LINE + 2);
				//����ȊO�͈�O�̃I�u�W�F�N�g���擾
				else
					rock2 = BackObjManager::Instance().GetBackObj(i - 1);

				//��̍Ō�Ɉړ�������
				rock1->SetPosition({ rock2->GetPosition().x, rock2->GetPosition().y, rock2->GetPosition().z + BACK_OBJ_INTERVAL_Z });


			}
		}
		//�E��...3�ȏ�
		else
		{
			if (playerPosZ > rockPosZ + BACK_OBJ_INTERVAL_Z)
			{
				//��r�����I�u�W�F�N�g��z�u�����ɂ���I�u�W�F�N�g
				BackObj* rock2;
				//�E���ԑO�̃I�u�W�F�N�g�̎��͉E��̍Ō�̃I�u�W�F�N�g���擾
				if (i == RIGHT_FRONT_LINE)
					rock2 = BackObjManager::Instance().GetBackObj(RIGHT_FRONT_LINE + 2);
				//����ȊO�͈�O�̃I�u�W�F�N�g���擾
				else
					rock2 = BackObjManager::Instance().GetBackObj(i - 1);

				//��̍Ō�Ɉړ�������
				rock1->SetPosition({ rock2->GetPosition().x, rock2->GetPosition().y, rock2->GetPosition().z + BACK_OBJ_INTERVAL_Z });

			}
		}

	}
}

//���C�g�֌W�̐ݒ�
void SceneGame::SettingLight(ID3D11DeviceContext* dc, RenderContext& rc)
{
	Camera& camera = Camera::Instance();

	//�p�����[�^�ݒ�
	//����(���͖��g�p)
	rc.ambientLightColor = ambientLightColor_;
	//�J�����̈ʒu
	rc.viewPosition.x = camera.GetEye().x;
	rc.viewPosition.y = camera.GetEye().y;
	rc.viewPosition.z = camera.GetEye().z;
	rc.viewPosition.w = 1;
	//���C�g����
	rc.directionalLight.direction.x = directionalLight_->GetDirection().x;
	rc.directionalLight.direction.y = directionalLight_->GetDirection().y;
	rc.directionalLight.direction.z = directionalLight_->GetDirection().z;
	rc.directionalLight.direction.w = 0;
	//���C�g�̐F
	rc.directionalLight.color = directionalLight_->GetColor();

	//�|�C���g���C�g
	//��芸����1�����g��
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

//�v���C���[�A�j���[�V�������Ƀv���C���[�Ƀ��C�g�𓖂Ă�
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
		rc.pointLight[1].color =  { 1, 1, 1, 1 };//��
		rc.pointLight[2].color =  { 1, 1, 1, 1 };//��
		break;
	case 1: //blow
		rc.pointLight[1].color = { 1, 0, 0, 1 };//��
		rc.pointLight[2].color = { 1, 0, 0, 1 };//��
		break;
	case 2: //squat
		rc.pointLight[1].color = { 0, 1, 0, 1 };//��
		rc.pointLight[2].color = { 0, 1, 0, 1 };//��
		break;
	case 3: // jump
		rc.pointLight[1].color = { 0, 0, 1, 1 };//��
		rc.pointLight[2].color = { 0, 0, 1, 1 };//��
		break;
	}
	if (animState_ == 0)
	{
		rc.pointLight[1].range = 0;
		rc.pointLight[2].range = 0;
	}
}

//�t���[�J�����𓮂������߂̏���
DirectX::XMFLOAT3 SceneGame::GetMoveVec() const
{
	// ���͏����擾
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisLX();
	float ay = gamePad.GetAxisLY();

	// �J���������ƃX�e�B�b�N�̓��͒l�ɂ���Đi�s�������v�Z
	Camera& camera = Camera::Instance();
	const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
	const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

	// �ړ��x�N�g����XZ���ʂɐ����ȃx�N�g���ɂȂ�悤�ɂ���

	// �J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
	float cameraRightX = cameraRight.x;
	float cameraRightZ = cameraRight.z;
	float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
	if (cameraRightLength > 0.0f)
	{
		// �P�ʃx�N�g����
		cameraRightX /= cameraRightLength;
		cameraRightZ /= cameraRightLength;
	}

	// �J�����O�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
	float cameraFrontX = cameraFront.x;
	float cameraFrontZ = cameraFront.z;
	float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
	if (cameraFrontLength > 0.0f)
	{
		// �P�ʃx�N�g����
		cameraFrontX /= cameraFrontLength;
		cameraFrontZ /= cameraFrontLength;
	}

	// �X�e�B�b�N�̐������͒l���J�����E�����ɔ��f���A
	// �X�e�B�b�N�̐������͒l���J�����O�����ɔ��f���A
	// �i�s�x�N�g�����v�Z����
	DirectX::XMFLOAT3 vec;
	vec.x = cameraRightX * ax + cameraFrontX * ay;
	vec.z = cameraRightZ * ax + cameraFrontZ * ay;

	// Y�������ɂ͈ړ����Ȃ�
	vec.y = 0.0f;

	return vec;
}

//�f�o�b�N�p�t���[�J����
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

//�}���`�X���b�h�Փˉ��p�֐�
void SceneGame::ThreadCover()
{
	if (threadCover_ == 0)
	{
		Graphics& graphics = Graphics::Instance();
		skyboxRenderer_->SetShaderResourceView(skyboxTexture_->GetShaderResourceView(),
			skyboxTexture_->GetWidth(), skyboxTexture_->GetHeight());
		//render�֐��ŌĂԂ�3D���f�����O�ɕ`�悳��Ă��܂�
		skyboxRenderer_->Update(0, 0, graphics.GetScreenWidth(), graphics.GetScreenHeight(),
			0, 0, static_cast<float>(skyboxTexture_->GetWidth()), static_cast<float>(skyboxTexture_->GetHeight()),
			0,
			1, 1, 1, 1);


		//�F���␳
		//3D��Ԃ�sprite�Ƃ��Đݒ�
		sprite_->SetShaderResourceView(renderTarget_->GetShaderResourceView(),
			renderTarget_->GetWidth(), renderTarget_->GetHeight());
		//�o�b�t�@�X�V
		sprite_->Update(0, 0, graphics.GetScreenWidth(), graphics.GetScreenHeight(),
			0, 0, static_cast<float>(renderTarget_->GetWidth()), static_cast<float>(renderTarget_->GetHeight()),
			0,
			1, 1, 1, 1);

		threadCover_++;
	}
}

//�|�[�Y
bool SceneGame::PauseProcess(float elapsedTime)
{
	isPause_ = pause_->GetIsPause();
	if (pause_->Update(elapsedTime))
	{
		//�f�o�b�N�݂̂̃��[�v�L�[(�t���[�J����)
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

//�X�v���C�g�p�p�����[�^�X�V����
void SceneGame::SpriteParameterUpdate(float elapsedTime)
{
	//�^�C�}�[(�Q�[������)�`�揈��
	//1�̈�
	timerTexPosX_[0] = (int)gameTimer_ * texSize_;
	//10�̈�
	timerTexPosX_[1] = ((int)gameTimer_ / 10) * texSize_;
	//�����_
	timerTexPosX_[2] = ((int)(gameTimer_ * 10) % 10) * texSize_;

	//GO!! �ړ�������
	if (gameStartTime_ < -1.0f)
	{
		GoPosX_ -= 50;
	}

	//�R���{
	//1�̈�
	comboTexPosX_[0] = player_->GetCombo() * texSize_;
	//10�̈�
	comboTexPosX_[1] = player_->GetCombo() / 10 * texSize_;

	//�X�s�[�h���[�^�[�`��v�Z
	speed_ = player_->GetNowSpeed();
	maxSpeed_ = player_->GetMaxSpeed();	//�ő�X�s�[�h
	ratio_ = speed_ / maxSpeed_;
	meterTexX_ = (ratio_ * (768 * 0.5f));
	//�O���f�[�V�����v�Z
	r_ = (rEnd_ - rStart_) * ratio_ + rStart_;
	g_ = (gEnd_ - gStart_) * ratio_ + gStart_;
	b_ = (bEnd_ - bStart_) * ratio_ + bStart_;

	//���˃u���[�̃p�����[�^�ݒ�
	float sp = speed_ / maxSpeed_;
	radialBlurData_.strength = mathf_.Lerp(0.0001, 0.25, sp);
	//�u���[�̌v�Z
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

	//�Փ�(�Ԃ����Ă��܂������Ɏ���ɐԂ�Sprite��\������)
	if (player_->isImpulse_)
	{
		ImpulsTimer_ += elapsedTime;
	}
	if (ImpulsTimer_ > 0.5f)
	{
		ImpulsTimer_ = 0;
		player_->isImpulse_ = false;
	}
	//�_�ł�����p
	flashingTime_++;

	//������΂��I�u�W�F�N�g�̃^�C�~���O���x�\��
	timing_ = player_->timing_;

}

//UI���`�F���W����
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
	//�J�����R���g���[���[�X�V����
	target_ = player_->GetPosition();
	target_.y += 3.5f;
	freeCameraPosition_ = player_->GetPosition();

	cameraController_->SetTarget(target_);
	cameraController_->Update(elapsedTime);

	//�X�e�[�W�X�V����
	stage_->Update(elapsedTime);

	//�G�l�~�[�X�V����
	EnemyManager::Instance().Update(elapsedTime);

	//�w�i�I�u�W�F�N�g�X�V����
	BackObjManager::Instance().Update(elapsedTime);

	//�G�t�F�N�g�X�V����
	EffectManager::Instane().Update(elapsedTime);

	//�X�v���C�g�̃p�����[�^�X�V����
	SpriteParameterUpdate(elapsedTime);

}