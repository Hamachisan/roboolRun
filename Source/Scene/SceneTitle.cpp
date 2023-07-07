#include "Graphics/Graphics.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include "Input/Input.h"
#include "SceneLoading.h"
#include "Camera.h"

#include "Other/Constant.h"

//������
void SceneTitle::Initialize()
{
	//�X�e�[�W������
	stage_ = new Stage();

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

	//�X�v���C�g������
	SpriteInitialize();
	//�p�����[�^�[������
	ParameterInitialize();

	//BGM������
	bgmTitle_ = Audio::Instance().LoadAudioSource("Data/Audio/BGM/Title/saiba29.wav");
	//SE������
	seSelect_ = Audio::Instance().LoadAudioSource("Data/Audio/SE/Scene/select2.wav");
	seStart_ = Audio::Instance().LoadAudioSource("Data/Audio/SE/Scene/start2.wav");
}

//�p�����[�^�̏�����
void SceneTitle::ParameterInitialize()
{
	//�I�����C���̍��W
	posY_ = 450;

	//�ʒu�����p�ϐ��ݒ�
	//�L�[or�{�^���Ő؂�ւ��sprite���W�A�X�P�[��
	keyOrButtonPos_.x = 500;
	keyOrButtonPos_.y = 460;
	keyOrButtonScale_.x = keyOrButtonScale_.y = 128;

	//�v�b�V������sprite�̍��W�A�X�P�[��
	pushPos_.x = 160;
	pushPos_.y = 460;
	pushScale_.x = 512;
	pushScale_.y = 128;

	//Button��key���̕���sprite�̍��W�A�X�P�[��
	StringKeyOrButtonPos_.x = 690;
	StringKeyOrButtonPos_.y = 460;
	StringKeyOrButtonScale_.x = 512;
	StringKeyOrButtonScale_.y = 128;

	//�^�C�g�������ƃ^�C�g���̌��O���f�[�V�����̍��W�A�X�P�[��
	titlePos_.x = 490;
	titlePos_.y = 50;
	titleScale_.x = 512 * 1.5;
	titleScale_.y = 128 * 2;
	titleTexPos_.x = 0;
	titleTexPos_.y = 0;
}

void SceneTitle::SpriteInitialize()
{
	//�X�v���C�g������
	sprGameTitle_				= std::make_unique<Sprite>("Data/Sprite/Scene/Title/���{�[��RUN3.png");
	sprGradationBack_			= std::make_unique<Sprite>("Data/Sprite/Scene/Title/gradationBack2.png");
	sprLine_					= std::make_unique<Sprite>("Data/Sprite/Scene/Title/line.png");
	sprBack_					= std::make_unique<Sprite>("Data/Sprite/Scene/Title/TitleBack.png");
	sprAllBack_					= std::make_unique<Sprite>("Data/Sprite/Scene/Reslut/back.png");

	//������UI
	sprPushKeyButton_[KEY]		= std::make_unique<Sprite>("Data/Sprite/Scene/Title/EnterKey.png");
	sprPushKeyButton_[BUTTON]	= std::make_unique<Sprite>("Data/Sprite/Scene/Title/Bbutton.png");
	sprPush_					= std::make_unique<Sprite>("Data/Sprite/Scene/Title/Push.png");
	sprkeyOrButton_[KEY]		= std::make_unique<Sprite>("Data/Sprite/Scene/Title/Key.png");
	sprkeyOrButton_[BUTTON]		= std::make_unique<Sprite>("Data/Sprite/Scene/Title/Button.png");

	//���x���I��
	

	//�X�e�[�W�I��
	

	//�X�J�C�{�b�N�X�e�N�X�`���̓ǂݍ���
	skyboxTexture_ = std::make_unique<Texture>("Data/SkyTexture/sky.jpg");
	isSpriteShader_ = true;
	skyboxRenderer_ = std::make_unique<Sprite>(isSpriteShader_);

	//�t�H���g
	sprFont_ = std::make_unique<Sprite>("Data/Sprite/number.png");
	sprStage_ = std::make_unique<Sprite>("Data/Sprite/Scene/Title/Selects/stage.png");
	sprLevel_ = std::make_unique<Sprite>("Data/Sprite/Scene/Title/Selects/Level.png");

	sprArrow_ = std::make_unique<Sprite>("Data/Sprite/Scene/Title/Selects/Arrow.png");
	sprFrame_ = std::make_unique<Sprite>("Data/Sprite/Scene/Title/Selects/frame.png");
}

//�I����
void SceneTitle::Finalize()
{
	//�X�e�[�W�I����
	SafeDelete(stage_);

	//�v���C���[�I����
	SafeDelete(player_);

	//�J�����I����
	SafeDelete(cameraController_);
}

//�X�V����
void SceneTitle::Update(float elapsedTime)
{
	bgmTitle_->Play(true);

	//�R���g���[���[���q����Ă�����\������UI��ύX
	ChangeUi();

	//�X�J�C�{�b�N�X�̐ݒ���}���`�X���b�h�ŏՓ˂��N�����Ȃ��pInitialize�ł͖���
	//Update�Őݒ肷��
	ThreadCover();

	//�J��������
	cameraController_->setCameraRange(5);
	target_ = player_->GetPosition();
	target_.x += 5.0f;
	target_.y += 0.5f;
	target_.z -= 8.0f;
	cameraController_->SetTarget(target_);
	cameraController_->Update(elapsedTime);

	//�X�e�[�W�X�V����
	stage_->Update(elapsedTime);

	//�v���C���[�X�V����
	player_->Update(elapsedTime);

	//�X�v���C�g�̃p�����[�^�X�V����
	SpriteParameterUpdate(elapsedTime);
		
	GamePad& gamePad = Input::Instance().GetGamePad();
	//�{�^���������ăX�e�[�W�I����
	switch (titleState_)
	{
		//�^�C�g�����
	case 0:
		if (gamePad.GetButtonDown() & GamePad::BTN_ENTER  ||
			gamePad.GetButtonDown() & GamePad::BTN_PAD_B)
		{
			titleState_++;
			seStart_->Play(false);
		}
		break;

		//�X�e�[�W�I�����
	case 1:
		flashingTime_++;

	
		//�X�e�[�W�I���������
		StageSelect();
	
		//���݂̃X�e�[�W���ۑ�
		nowStage_ = stageSelect_ + MAX_STAGE * stageLevelSelect_;

		//�V�[���؂�ւ�
		if (gamePad.GetButtonDown() & GamePad::BTN_ENTER ||
			gamePad.GetButtonDown() & GamePad::BTN_PAD_B)
		{
			isSeFlag_ = true;
			
		}
		//SE��炷
		if (isSeFlag_)
		{
			seStart_->Play(false);
			sePlayTimer_ += 0.1f;
		}
		if (sePlayTimer_ > 2.5f)
		{
			bgmTitle_->Stop();
			//�{�^�����������烍�[�f�B���O�V�[���؂�ւ�
			SceneManager::Instance().SetStageNum(nowStage_);
			SceneManager::Instance().SetNextScene(NEXT_SCENE_GAME);
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
		}

		break;

	}

}

//UI�`�揈��
void SceneTitle::UiRender(ID3D11DeviceContext* dc)
{
	//Title�̌��
	sprGradationBack_->Render(dc,
		titlePos_.x, titlePos_.y,			//���W
		titleScale_.x, titleScale_.y,		//�X�P�[��
		titleTexPos_.x, titleTexPos_.y,		//�e�N�X�`����ʒu
		PIXEL_512, PIXEL_128,				//�e�N�X�`���̑傫��
		0,									//�p�x
		1, 1, 1, 1);						//�F(rgb)�s�����x(a)

	//�^�C�g��
	sprGameTitle_->Render(dc,
		titlePos_.x, titlePos_.y,			//���W
		titleScale_.x, titleScale_.y,		//�X�P�[��
		0, 0, 								//�e�N�X�`����ʒu
		PIXEL_512, PIXEL_128,				//�e�N�X�`���̑傫��
		0,									//�p�x
		1, 1, 1, 1);						//�F(rgb)�s�����x(a)

	//�����̃��C��
	sprBack_->Render(dc,
		0, SCREEN_HEIGHT * 0.5,				//���W
		SCREEN_WIDTH, SCREEN_HEIGHT,		//�X�P�[��
		0, 0, 								//�e�N�X�`����ʒu
		SCREEN_WIDTH, SCREEN_HEIGHT,		//�e�N�X�`���̑傫��
		0, 									//�p�x
		1, 1, 1, 1);						//�F(rgb)�s�����x(a)

	//�^�C�g���X�v���C�g�`��
	switch (titleState_)
	{
	case 0: //�^�C�g���`��

		//�G���^�[��B�{�^����
		sprPushKeyButton_[keyOrButton_]->Render(dc,
			keyOrButtonPos_.x, keyOrButtonPos_.y,					//���W
			keyOrButtonScale_.x, keyOrButtonScale_.y,				//�X�P�[��
			0, 0, 													//�e�N�X�`����ʒu
			PIXEL_512, PIXEL_512,									//�e�N�X�`���̑傫��
			0,														//�p�x
			1, 1, 1, 1);											//�F(rgb)�s�����x(a)

		//Push
		sprPush_->Render(dc,
			pushPos_.x, pushPos_.y,									//���W
			pushScale_.x, pushScale_.y,								//�X�P�[��
			0, 0, 													//�e�N�X�`����ʒu
			PIXEL_512, PIXEL_128,									//�e�N�X�`���̑傫��
			0,														//�p�x
			1, 1, 1, 1);											//�F(rgb)�s�����x(a)

		//KeyOrButton
		sprkeyOrButton_[keyOrButton_]->Render(dc,
			StringKeyOrButtonPos_.x, StringKeyOrButtonPos_.y,		//���W
			StringKeyOrButtonScale_.x, StringKeyOrButtonScale_.y,	//�X�P�[��
			0, 0, 													//�e�N�X�`����ʒu
			PIXEL_512, PIXEL_128,									//�e�N�X�`���̑傫��
			0,														//�p�x
			1, 1, 1, 1);											//�F(rgb)�s�����x(a)


		break;

	case 1:	//�X�e�[�W�I��`��
		//���C��
		sprLine_->Render(dc,
			0, posY_,				//���W
			SCREEN_WIDTH, 220,		//�X�P�[��
			0, 0, 					//�e�N�X�`����ʒu
			SCREEN_WIDTH, 190,		//�e�N�X�`���̑傫��
			0,						//�p�x
			1, 1, 1, 1);			//�F(rgb)�s�����x(a)
		
		//���x���I��
		{

			//���x������
			sprLevel_->Render(dc,
				Text1PosX_, Text1PosY_,				//���W
				PIXEL_256, PIXEL_128,				//�X�P�[��
				0, 0,								//�e�N�X�`����ʒu
				PIXEL_256, PIXEL_128,				//�e�N�X�`���̑傫��
				0,									//�p�x
				1, 1, 1, 1);						//�F(rgb)�s�����x(a)

			//���x������
			sprFont_->Render(dc,
				font1PosX_, font1PosY_,				//���W
				64, 64,								//�X�P�[��
				fontTexPosX2_, 0,					//�e�N�X�`����ʒu
				64, 64,								//�e�N�X�`���̑傫��
				0,									//�p�x
				1, 1, 1, 1);						//�F(rgb)�s�����x(a)
			//���x�������t���[��
			sprFrame_->Render(dc,
				font1PosX_ - 32, font1PosY_ - 32,	//���W
				128, 128,							//�X�P�[��
				0, 0,								//�e�N�X�`����ʒu
				128, 128,							//�e�N�X�`���̑傫��
				0,									//�p�x
				1, 1, 1, 1);						//�F(rgb)�s�����x(a)

			//���
			if(isArrowDown_ && flashingTime_ / 24 % 2)
				sprArrow_->Render(dc,					
					arrowLevelPosX_, arrowLevelPosY1_,		//���W
					64, 64,									//�X�P�[��
					0, 0,									//�e�N�X�`����ʒu
					64, 64,									//�e�N�X�`���̑傫��
					180,									//�p�x
					1, 1, 1, 1);							//�F(rgb)�s�����x(a)
			//����
			if (isArrowUp_ && flashingTime_ / 24 % 2)
				sprArrow_->Render(dc,
					arrowLevelPosX_, arrowLevelPosY2_,		//���W
					64, 64,									//�X�P�[��
					0, 0,									//�e�N�X�`����ʒu
					64, 64,									//�e�N�X�`���̑傫��
					0,										//�p�x
					1, 1, 1, 1);							//�F(rgb)�s�����x(a)
		}
		//�X�e�[�W�I��
		{
			//�X�e�[�W����
			sprStage_->Render(dc,
				Text2PosX_, Text2PosY_,					//���W
				PIXEL_256, PIXEL_128,					//�X�P�[��
				0, 0,									//�e�N�X�`����ʒu
				PIXEL_256, PIXEL_128,					//�e�N�X�`���̑傫��
				0,										//�p�x
				1, 1, 1, 1);							//�F(rgb)�s�����x(a)

			//�X�e�[�W����
			sprFont_->Render(dc,
				font2PosX_, font2PosY_,					//���W
				64, 64,									//�X�P�[��
				fontTexPosX_, 0,						//�e�N�X�`����ʒu
				64, 64,									//�e�N�X�`���̑傫��
				0,										//�p�x
				1, 1, 1, 1);							//�F(rgb)�s�����x(a)
			//�X�e�[�W�����t���[��
			sprFrame_->Render(dc,
				font2PosX_ - 32, font2PosY_ - 32,		//���W
				128, 128,								//�X�P�[��
				0, 0,									//�e�N�X�`����ʒu
				128, 128,								//�e�N�X�`���̑傫��
				0,										//�p�x
				1, 1, 1, 1);							//�F(rgb)�s�����x(a)


			//���
			if(isArrowLeft_ && flashingTime_ / 24 % 2)
				sprArrow_->Render(dc,
					arrowStagePosX1_ - 64, arrowStagePosY_,		//���W
					64, 64,										//�X�P�[��
					0, 0,										//�e�N�X�`����ʒu
					64, 64,										//�e�N�X�`���̑傫��
					270,										//�p�x
					1, 1, 1, 1);								//�F(rgb)�s�����x(a)
			//���E
			if(isArrowRight_ && flashingTime_ / 24 % 2)
				sprArrow_->Render(dc,
					arrowStagePosX2_, arrowStagePosY_,			//���W
					64, 64,										//�X�P�[��
					0, 0,										//�e�N�X�`����ʒu
					64, 64,										//�e�N�X�`���̑傫��
					90,											//�p�x
					1, 1, 1, 1);								//�F(rgb)�s�����x(a)
		}

		break;
	}
}

//�`�揈��
void SceneTitle::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	//��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f }; //RGBA(0.0~1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };

	// �J�����p�����[�^�ݒ�
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

	// 3D���f���`��
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);

		//�X�e�[�W�`��
		stage_->Render(dc, shader);
		//�v���C���[�`��
		player_->Render(dc, shader);

		shader->End(dc);
	}

	//2D�X�v���C�g�`��
	{
		UiRender(dc);
	}

	//ImGui�`��
#ifdef _DEBUG
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Title", nullptr, ImGuiTreeNodeFlags_None))
	{
		//UI�z�u����
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


//�X�e�[�W�I��
void SceneTitle::StageSelect()
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	//�E�Ɉړ�����(�X�e�[�W���͏オ��)
	if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT ||
		gamePad.GetButtonDown() & GamePad::BTN_D)
	{
		//����l�ȓ�
		if (stageSelect_ < MAX_STAGE - 1)
		{
			fontTexPosX_ = 64 * (stageSelect_ + 2);
			seSelect_->Stop();
			seSelect_->Play(false);
		}
		stageSelect_++;
		//���
		if (stageSelect_ > MAX_STAGE - 1)
		{
			stageSelect_ = MAX_STAGE - 1;	//����ȏ㓮���Ȃ�
			fontTexPosX_ = 64 * (MAX_STAGE);
		}
		
		
	}

	//���Ɉړ�(�X�e�[�W���͉�����)
	if (gamePad.GetButtonDown() & GamePad::BTN_LEFT ||
		gamePad.GetButtonDown() & GamePad::BTN_A)
	{
		//�����l�ȓ�
		if (stageSelect_ > 0)
		{
			fontTexPosX_ = 64 * stageSelect_ ;
			seSelect_->Stop();
			seSelect_->Play(false);
		}
		stageSelect_--;
		//����
		if (stageSelect_ < 0)
		{
			stageSelect_ = 0;
			fontTexPosX_ = 64;
		}
	
	}

	//���Ɉړ�(�X�e�[�W�̃��x�����オ��)
	if (gamePad.GetButtonDown() & GamePad::BTN_DOWN ||
		gamePad.GetButtonDown() & GamePad::BTN_S)
	{
		//����l�ȓ�
		if (stageLevelSelect_ < MAX_STAGE_LEVEL - 1)
		{
			fontTexPosX2_ = 64 * (stageLevelSelect_ + 2);
			seSelect_->Stop();
			seSelect_->Play(false);
		}
		stageLevelSelect_++;
		//���
		if (stageLevelSelect_ > MAX_STAGE_LEVEL - 1)
		{
			stageLevelSelect_ = MAX_STAGE_LEVEL - 1;	//����ȏ㓮���Ȃ�

			fontTexPosX2_ = 64 * MAX_STAGE_LEVEL;
		}
	
	}
	//��Ɉړ�(�X�e�[�W�̃��x����������)
	if (gamePad.GetButtonDown() & GamePad::BTN_UP ||
		gamePad.GetButtonDown() & GamePad::BTN_W)
	{
		//�����l�ȓ�
		if (stageLevelSelect_ > 0)
		{
			fontTexPosX2_ = 64 ;
			seSelect_->Stop();
			seSelect_->Play(false);
		}
		stageLevelSelect_--;
		//����
		if (stageLevelSelect_ < 0)
		{
			stageLevelSelect_ = 0;
			fontTexPosX2_ = 64;
		}
	}



	//���\��
	if (stageSelect_ == MAX_STAGE - 1) isArrowRight_ = false;
	else isArrowRight_ = true;

	//���\��
	if (stageSelect_ == 0) isArrowLeft_ = false;
	else isArrowLeft_ = true;

	if (stageLevelSelect_ == MAX_STAGE_LEVEL - 1) isArrowDown_ = false;
	else isArrowDown_ = true;

	if (stageLevelSelect_ == 0) isArrowUp_ = false;
	else isArrowUp_ = true;

}

//�}���`�X���b�h�Փˉ��p�֐�
void SceneTitle::ThreadCover()
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

		threadCover_++;
	}
}


//�X�v���C�g�̃p�����[�^�X�V����
void SceneTitle::SpriteParameterUpdate(float elapsedTime)
{
	//�^�C�g�������̌��̃O���f�[�V�����p�摜�̃X�N���[��
	titleTexPos_.x += 3;
	//�摜���T�C�Y���
	if (titleTexPos_.x > 1526)titleTexPos_.x = 0;
}

//UI�ύX����
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