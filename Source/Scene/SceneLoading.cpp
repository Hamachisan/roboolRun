#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "SceneLoading.h"
#include "SceneManager.h"
#include <thread>

//������
void SceneLoading::Initialize()
{
	//�X�v���C�g������
	SpriteInitialize();

	//�X���b�h�J�n
	std::thread thread(LoadingThread, this);

	//�X���b�h�̊Ǘ���j��
	thread.detach();
}

//�p�����[�^�[������
void SceneLoading::ParameterInitialize()
{

}

//�X�v���C�g������
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


//�I����
void SceneLoading::Finalize()
{
}

//�X�V����
void SceneLoading::Update(float elapsedTime)
{
	//�X�v���C�g�̃p�����[�^�X�V����
	SpriteParameterUpdate(elapsedTime);

	//���̃V�[�����Q�[���̎�
	int nextScene = SceneManager::Instance().GetNextScene();
	if (nextScene == NEXT_SCENE_GAME)
	{
		GamePad& gamePad = Input::Instance().GetGamePad();
		//�R���g���[���[���q����Ă�����\������UI��ύX
		ChangeUi();

		//�_�ł�����p
		flashingTime_++;
		if (nextScene_->IsReady())
		{
			isRender_ = true;
		}

		//�{�^������������Q�[���V�[���֐؂�ւ�
		if (isRender_)
			if (gamePad.GetButtonDown() & GamePad::BTN_ENTER ||
				gamePad.GetButtonDown() & GamePad::BTN_PAD_B)
			{
				SceneManager::Instance().ChangeScene(nextScene_);
				nextScene_ = nullptr;
			}
	}
	//���̃V�[�����Q�[���ȊO�̎�
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
	//��ʉE���Ƀ��[�f�B���O�A�C�R����`��
	float textureWidth = static_cast<float>(sprLoadingIcon_->GetTextureWidth());
	float textureHeight = static_cast<float>(sprLoadingIcon_->GetTextureHeight());
	float positionX = SCREEN_WIDTH - textureWidth;
	float positionY = SCREEN_HEIGHT - textureHeight;

	//���̃V�[�����Q�[���̎�
	int nextScene = SceneManager::Instance().GetNextScene();
	if (nextScene == NEXT_SCENE_GAME)
	{
		//���[���\��
		if (isController_ == false)
			sprLoadingKey_->Render(dc,			
				0, 0,											//���W
				SCREEN_WIDTH, SCREEN_HEIGHT,					//�X�P�[��
				0, 0, 											//�e�N�X�`����ʒu
				SCREEN_WIDTH, SCREEN_HEIGHT,					//�e�N�X�`���̑傫��
				0,												//�p�x
				1, 1, 1, 1);									//�F(rgb)�s�����x(a)
		else
			sprLoadingPad_->Render(
				dc,
				0, 0,											//���W
				SCREEN_WIDTH, SCREEN_HEIGHT,					//�X�P�[��
				0, 0, 											//�e�N�X�`����ʒu
				SCREEN_WIDTH, SCREEN_HEIGHT,					//�e�N�X�`���̑傫��
				0,												//�p�x
				1, 1, 1, 1);									//�F(rgb)�s�����x(a)


		//�o�[
		sprLine_->Render(dc,
			0, 540,									//���W
			SCREEN_WIDTH, 150,						//�X�P�[��
			0, 0, 									//�e�N�X�`����ʒu
			SCREEN_WIDTH, 150,						//�e�N�X�`���̑傫��
			0,										//�p�x
			1, 1, 1, 1);							//�F(rgb)�s�����x(a)

		//NowLoading
		if (!isRender_)
			sprNowLoading_->Render(dc,
				posX_, 550,								//���W
				PIXEL_512, PIXEL_128,					//�X�P�[��
				0, 0, 									//�e�N�X�`����ʒu
				PIXEL_512, PIXEL_128,					//�e�N�X�`���̑傫��
				0,										//�p�x
				1, 1, 1, 1);							//�F(rgb)�s�����x(a)

		//���ɐi��
		if (isRender_)
		{
			if (flashingTime_ / 24 % 2)
			{
				sprStart_->Render(dc,
					700, 550,									//���W
					PIXEL_512, PIXEL_128,						//�X�P�[��
					0, 0, 										//�e�N�X�`����ʒu
					PIXEL_512, PIXEL_128,						//�e�N�X�`���̑傫��
					0,											//�p�x
					1, 1, 1, 1);								//�F(rgb)�s�����x(a)

				//�G���^�[��B�{�^����
				sprPushKeyButton_[keyOrButton]->Render(dc,
					1150, 560,									//���W
					110, 110,									//�X�P�[��
					0, 0, 										//�e�N�X�`����ʒu
					PIXEL_512, PIXEL_512,						//�e�N�X�`���̑傫��
					0, 											//�p�x
					1, 1, 1, 1);								//�F(rgb)�s�����x(a)
			}
		}
	}
	else
	{
		sprLoadingIcon_->Render(dc,
			positionX, positionY,				//���W
			textureWidth, textureHeight,		//�X�P�[��
			0, 0,								//�e�N�X�`����ʒu
			textureWidth, textureHeight,		//�e�N�X�`���̑傫��
			angle_,								//�p�x
			1, 1, 1, 1);						//�F(rgb)�s�����x(a)

	}
}

//�`�揈��
void SceneLoading::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	//��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	//rgb a (0.0~1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	//2D�X�v���C�g�`��
	{
		UiRender(dc);
	}
}

//���[�f�B���O�X���b�h
void SceneLoading::LoadingThread(SceneLoading* scene)
{
	//COM�֘A�̏������ŃX���b�h���ɌĂԕK�v������
	CoInitialize(nullptr);

	//���̃V�[���̏������������s��
	scene->nextScene_->Initialize();

	//�X���b�h���I���O��COM�֘A�̏I����
	CoUninitialize();

	//���̃V�[�����������ݒ�
	scene->nextScene_->setReady();

}

//�X�v���C�g�̃p�����[�^�X�V����
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

//UI�̕ύX����
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