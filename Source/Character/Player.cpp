#include "Player.h"
#include "Input/Input.h"
#include <imgui.h>
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "Character/Enemys/EnemyManager.h"
#include "Collision.h"
#include "Scene/SceneGame.h"

//�R���X�g���N�^
Player::Player()
{
	model_ = new Model("Data/Model/Player/robo.mdl");

	//���f�����傫���̂ŃX�P�[�����O
	scale_.x = scale_.y = scale_.z = 0.0065f;
	position_.z = START_LINE;
	position_.x = 0.15f;

	//������ݒ�
	actors_ = Actor::player;

	//�A�j���[�V�����ݒ�
	TransitionCountDownState();
	//TransitionMoveState();

	//�R���g���[���[������
	ZeroMemory(&vibration_, sizeof(XINPUT_VIBRATION));
	//���[�^�[�̃X�s�[�h
	vibration_.wLeftMotorSpeed = 0;
	vibration_.wRightMotorSpeed = 0;

	//SE
	seDamage_		= Audio::Instance().LoadAudioSource("Data/Audio/SE/Player/damage.wav");
	seMove_			= Audio::Instance().LoadAudioSource("Data/Audio/SE/Player/move.wav");
	seJump_			= Audio::Instance().LoadAudioSource("Data/Audio/SE/Player/jump.wav");
	seAttack_		= Audio::Instance().LoadAudioSource("Data/Audio/SE/Player/attack.wav");
	seRoll_			= Audio::Instance().LoadAudioSource("Data/Audio/SE/Player/roll.wav");
	//Effect
	efkJump_		= std::make_unique<Effect>("Data/Effect/HipDrop2.efk");
}

//�f�X�g���N�^
Player::~Player()
{
	delete model_;
}

//�X�V����
void Player::Update(float elapsedTime)
{
	//�Q�[�����J�n���Ă��Ȃ����ɑҋ@�A�j���[�V�������Đ�����
	if (!isGameStart_)
	{

		//���͍X�V����
		UpdateVclocity(elapsedTime);

		//�J�E���g�_�E���A�j���[�V�����Đ�
		UpdateCountDownState(elapsedTime);
		//�A�j���[�V�����Đ�����
		model_->UpdateAnimation(elapsedTime);

		//�I�u�W�F�N�g�s����X�V
		UpdateTransform();

		//���f���s��X�V
		model_->UpdateTransform(transform_);	//�Y�ꂽ��_���I�I

		return;

	}

	//�Q�[���X�^�[�g�������ɃA�j���[�V������Move�ɕύX
	if (transitionMoveStateCount_ == 0)
		TransitionMoveState();
	transitionMoveStateCount_++;


	//��������
	timeAccelerationSpeed_ += elapsedTime * 10 * 2.5f;	//2.5�|���Ă�͕̂␳
	//�ő�
	if (timeAccelerationSpeed_ > maxSpeed_)
	{
		timeAccelerationSpeed_ = maxSpeed_;
		isMaxSpeed_ = true;
	}

	nowSpeed_ = timeAccelerationSpeed_ + moveSpeed_;

	//�f�o�b�N���̂݃S�[���փ��[�v����V���[�g�J�b�g
#ifdef _DEBUG
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_ENTER)
	{
		position_.z = GOAL_LINE - 10;
	}
#else
#endif

	//�ړ����͏���
	InputMove(elapsedTime);
	
	//���͍X�V����
	UpdateVclocity(elapsedTime);

	//�X�e�[�g���̃A�j���[�V�����؂�ւ�
	SwitchAnimState(elapsedTime);

	//�A�j���[�V�����Đ�����
	model_->UpdateAnimation(elapsedTime);

	//�v���C���[�ƓG�Ƃ̏Փ˔���
	CollisionRayCastPlayerVsEnemies();

	//�x�N�g���ł̔���
	CollisionVector(elapsedTime);

	//�I�u�W�F�N�g�s����X�V
	UpdateTransform();

	//���f���s��X�V
	model_->UpdateTransform(transform_);	//�Y�ꂽ��_���I�I

	//�R���g���[���[��U��������
	PadVibration(elapsedTime);

	//�J������U�������鎞��
	if (isCameraShakeFlag_)
	{
		shakeTimer_ -= elapsedTime;
	}
	if (shakeTimer_ < 0)
	{
		isCameraShakeFlag_ = false;
	}

	//�^�C�~���O���x�`��
	if (isTimingRender_)
	{
		timingRenderTimer_ -= elapsedTime;
	}
	if (timingRenderTimer_ < 0)
	{
		isTimingRender_ = false;
	}

	//�~�X�Ȃ��S�[��������MAX�R���{���͌��݂̃R���{��
	if (!isMiss_)
	{
		maxCombo_ = combo_;
	}
	if (isMiss_)
	{
		if (combo_ > maxCombo_)
			maxCombo_ = combo_;
	}

}

//�`�揈��
void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model_);
}

//�f�o�b�O�pGUI�`��
void Player::DrawDebugGUI()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("Player", nullptr, ImGuiTreeNodeFlags_None))
	{
		//�g�����X�t�H�[��
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			//�ʒu
			ImGui::InputFloat3("Position", &position_.x);

			//��]
			DirectX::XMFLOAT3 a;
			a.x = DirectX::XMConvertToDegrees(angle_.x);
			a.y = DirectX::XMConvertToDegrees(angle_.y);
			a.z = DirectX::XMConvertToDegrees(angle_.z);

			//ImGui::InputFloat3("Angle", &angle.x);
			ImGui::InputFloat3("Angle", &a.x);
			angle_.x = DirectX::XMConvertToRadians(a.x);
			angle_.y = DirectX::XMConvertToRadians(a.y);
			angle_.z = DirectX::XMConvertToRadians(a.z);

			//�X�P�[��
			ImGui::InputFloat3("Scale", &scale_.x);

		}
		if (ImGui::CollapsingHeader("Velocity", ImGuiTreeNodeFlags_DefaultOpen))
		{
			//���x
			ImGui::InputFloat3("Velocity", &velocity_.x);

		}
		if (ImGui::CollapsingHeader("Vector", ImGuiTreeNodeFlags_DefaultOpen))
		{
			//�x�N�g��
			ImGui::InputFloat("Vector", &nextEnemyLenght_);
		}
		if (ImGui::CollapsingHeader("Speed", ImGuiTreeNodeFlags_DefaultOpen))
		{
			//moveSpeed
			ImGui::InputFloat("Speed", &moveSpeed_);
			//nowSpeed
			ImGui::InputFloat("nowSpeed", &nowSpeed_);

			//���ԉ���
			ImGui::InputFloat("TimeAccelerationSpeed", &timeAccelerationSpeed_);
		}
		if (ImGui::CollapsingHeader("Offset", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::SliderFloat("Offset", &offset_, 0.0f, 10.0f);
		}

		if (ImGui::CollapsingHeader("TimingCount", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::InputInt("maxCombo", &maxCombo_);
			ImGui::InputInt("PERFECT", &perfectCount_);
			ImGui::InputInt("GREAT", &greatCount_);
			ImGui::InputInt("GOOD", &goodCount_);
		}

	}
	ImGui::End();
}

//�f�o�b�O�v���~�e�B�u�`��
void Player::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	//���C�L���X�g�n�_Y���̍��W
	debugRenderer->DrawSphere({ position_.x , position_.y + offset_, position_.z }, 0.5f,
		DirectX::XMFLOAT4(1, 0, 0, 1));

}

//���n�������ɌĂ΂��
void Player::OnLanding()
{
	jumpCount_ = 0;
	seJump_->Stop();
}

//�X�e�[�g���ɃA�j���[�V�����؂�ւ�
void Player::SwitchAnimState(float elapsedTime)
{
	switch (animState_)
	{
	case Player::AnimState::State_Move:
		UpdateMoveState(elapsedTime);
		isAnimMove_ = true;
		break;
	case Player::AnimState::State_Brow:
		UpdateBrowState(elapsedTime);
		isAnimMove_ = false;
		break;
	case Player::AnimState::State_Squat:
		UpdateSquatState(elapsedTime);
		isAnimMove_ = false;
		break;
	case Player::AnimState::State_jump:
		UpdateJumpState(elapsedTime);
		isAnimMove_ = false;
		break;
	default:
		break;
	}
}

//�ړ��x�N�g�����擾
DirectX::XMFLOAT3 Player::GetMoveVec() const
{
	//���͏����擾
	//GamePad& gamePad = Input::Instance().GetGamePad();
	//float ax = gamePad.GetAxisLX();
	//float ay = gamePad.GetAxisLY();
	float ay = 1;	//��ɏ�����ɃX�e�B�b�N���X�Ă�����
	float ax = 0;	//��ɃX�e�B�b�N���X���Ă��Ȃ����

	//�J���������ƃX�e�B�b�N�̓��͒l�ɂ���Đi�s�������v�Z����
	Camera& camera = Camera::Instance();
	const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
	const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

	//�ړ��x�N�g����XZ���ʂɐ����ȃx�N�g���ɂȂ�悤�ɂ���

	//�J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
	float cameraRightX = cameraRight.x;
	float cameraRightZ = cameraRight.z;
	float cameraRightLenght = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
	if (cameraRightLenght > 0.0f)
	{
		//�P�ʃx�N�g����
		cameraRightX /= cameraRightLenght;
		cameraRightZ /= cameraRightLenght;
	}

	//�J�����O�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
	float cameraFrontX =  0;//cameraFront.x;
	float cameraFrontZ =  1;//cameraFront.z;
	float cameraFrontLenght = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
	if (cameraFrontLenght > 0.0f)
	{
		//�P�ʃx�N�g����
		cameraFrontX /= cameraFrontLenght;
		cameraFrontZ /= cameraFrontLenght;
	}

	//�X�e�B�b�N�̐������͒l���J�����E�����ɔ��f���A
	cameraRightX = cameraRightX * ax;
	cameraRightZ = cameraRightZ * ax;
	//�X�e�B�b�N�̐������͒l���J�����O�����ɔ��f���A
	cameraFrontX = cameraFrontX * ay;
	cameraFrontZ = cameraFrontZ * ay;

	//�i�s�x�N�g�����v�Z����
	DirectX::XMFLOAT3 vec;
	vec.x = cameraRightX + cameraFrontX;
	vec.z = cameraRightZ + cameraFrontZ;

	//vec.x =(cameraRightX * ax) + (cameraFrontX * ay);
	//vec.z =(cameraRightZ * ax) + (cameraFrontZ * ay);

	//Y�������ɂ͈ړ����Ȃ�
	vec.y = 0.0f;

	return vec;
}

//�ړ����͏���
void Player::InputMove(float elapsedTime)
{
	//�i�s�x�N�g���擾
	DirectX::XMFLOAT3 moveVec = GetMoveVec();
	//DirectX::XMFLOAT3 moveVec = PlayerMoveVec;

	//�����ŉ���
	Move(moveVec.x, moveVec.z, moveSpeed_ + timeAccelerationSpeed_);
}

//�W�����v���͏���
bool Player::InputJump()
{
	//�{�^�����͂ŃW�����v(�W�����v�񐔐����t��)
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_L ||
		gamePad.GetButtonDown() & GamePad::BTN_PAD_B)
	{
		if (jumpCount_ < jumpLimit_)
		{
			++jumpCount_;
			this->Jump(jumpSpeed_);
			seJump_->Play(false);
			return true;
		}
	}
	return false;
}

//���(���[���ύX)
void Player::Avoidance()
{
	constexpr float METERS = 10.0;			//�ړ��ł��鋗��
	constexpr float ACCELERATION = 50.0;	//�ꎞ������
	constexpr float STAGE_EDGE = 10.0;		//�X�e�[�W�[

	//�{�^�����͂ŉ��
	GamePad& gamePad = Input::Instance().GetGamePad();

	//�����
	//�E�ɉ�𒆂͍�����o���Ȃ�
	if (gamePad.GetButtonDown() & GamePad::BTN_A ||
		gamePad.GetButtonDown() & GamePad::BTN_LEFT
		&& !isMoving_)
	{
		//���x���ق�0�ňʒu���قڍ��[���傫���Ƃ��̂�
		constexpr float LARGELY_STAGE_LEFT_EDGE = -9.9f;
		if (velocity_.x < 0.1 && position_.x > LARGELY_STAGE_LEFT_EDGE)
		{
			isAvoidanceFlag_ = true;
			isMoving_ = true;
			oldPositionX_ = position_.x;	//�������O�̈ʒu�ۑ�
			seMove_->Play(false);
		}
	}
	if (isAvoidanceFlag_)//���
	{
		//PlayerMoveVec.x = -1;

		//���ɉ���
		DirectX::XMFLOAT3 directionFront = GetFront();
		directionFront.x -= ACCELERATION;
		velocity_.x = directionFront.x;

		float difference = oldPositionX_ - position_.x;	//�������O�̈ʒu�ƌ��݂̈ʒu�̍�
		//10m�ړ����I������
		if (difference > METERS)
		{
			isMoving_ = false;
			velocity_.x = 0;
			isAvoidanceFlag_ = false;
			seMove_->Stop();
			//PlayerMoveVec.x = 0;
			if (position_.x < -10.0f)
			{
				position_.x = -STAGE_EDGE;	//�[�ɓ��B
			}
		}
	}


	//���ɉ�𒆂͉E�ɉ���o���Ȃ�
	//�E���
	if (gamePad.GetButtonDown() & GamePad::BTN_D ||
		gamePad.GetButtonDown() & GamePad::BTN_RIGHT
		&& !isMoving_)
	{
		//���x���ق�0�ňʒu���قډE�[��菬�����Ƃ��̂�
		constexpr float LARGELY_STAGE_RIGHT_EDGE = 9.9;
		if (velocity_.x < 0.1 && position_.x < LARGELY_STAGE_RIGHT_EDGE)
		{
			isMoving_ = true;
			isAvoidanceFlag2_ = true;
			oldPositionX2_ = position_.x;
			seMove_->Play(false);
		}
	}
	if (isAvoidanceFlag2_)	//���
	{
		//�E�ɉ���
		//PlayerMoveVec.x = 1;
		DirectX::XMFLOAT3 directionFront = GetFront();
		directionFront.x += ACCELERATION;
		velocity_.x = directionFront.x;

		float difference = oldPositionX2_ - position_.x;	//�������O�̈ʒu�ƌ��݂̈ʒu�̍�
		if (difference < -METERS)
		{
			isMoving_ = false;
			velocity_.x = 0;
			isAvoidanceFlag2_ = false;
			seMove_->Stop();
			//PlayerMoveVec.x = 0;
			if (position_.x > 10.0f)
			{
				position_.x = STAGE_EDGE;	//�[�ɓ��B
			}
		}
	}
}

//�U������
void Player::Attack(float elapsedTime)
{
	//�v���C���[�̃x�N�g��
	DirectX::XMVECTOR playerVec = DirectX::XMLoadFloat3(&this->position_);
	int enemyCount = EnemyManager::Instance().GetEnemyCount();
	for (int i = 0; i < enemyCount; i++)
	{
		Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
		DirectX::XMVECTOR enemyVec = DirectX::XMLoadFloat3(&enemy->GetPosition());

		//�v���C���[ -> �G�l�~�[�ւ̃x�N�g������
		DirectX::XMVECTOR playerEnemyVec = DirectX::XMVectorSubtract(enemyVec, playerVec);

		//���������x�N�g���̋������Z�o���ۑ�
		playerEnemyVecLenght_ = DirectX::XMVectorGetX(DirectX::XMVector3Length(playerEnemyVec));

		//�������߂����̂�������(�G�t�F�N�g���o�O���Ă��܂��̂�)
		//float range = 10;
		//if (playerEnemyVecLenght < range)
		//{
			//���̋������ōU��������q�b�g
		float hitDistanece = 6.0f;
		Actor enemyActor = enemy->GetCaracterActor();
		//�󂹂�I�u�W�F�N�g������̎��̂݃_���[�W������
		if (enemyActor == Actor::enemySutter)
		{
			//�G�l�~�[���������Ă��鎞�̂�
			if (enemy->isAlive_)
			{

				if (playerEnemyVecLenght_ < hitDistanece)
				{
					float impulsX = (ImpulsDirection()) * 3;		//���E�ɐ�����΂��ʂ��擾
					float magnification = 3.5f;//�{��
					float impulsZ = ImpulsPower(magnification);		//������΂��p���[�擾
					enemy->AddImpulse({ impulsX, 70.5f, impulsZ });	//�Ռ���^����

					enemy->SetImpulseFlag(true);					//�������ł���Œ��Ƃ����t���O
					enemy->impuluseTimer_ = 0.0f;					//�������ł��鎞�Ԃ̃^�C�}�[
					enemy->isAlive_ = false;						//��������ł��邯�Ǖ\��������ׂɂ܂�����������
					enemy->destroyTime_ = 1.0f;						//�������Ԃ�ݒ肷��
					//SutterObj.cpp -> BlownAway�Ő�����яI���������������Ă���

					enemy->isEfkHit_ = true;						//������΂����I�u�W�F�N�g�ɃG�t�F�N�g���Đ�����

					//�R���g���[���[��U��������t���O�ƃ^�C�}�[
					isVibrationFlag_ = true;
					vibrationTimer_ = 0.2f;

					//SE�Đ�
					seAttack_->Play(false);

					//�^�C�~���O�̐��x����
					float accuracy = 4.5f;
					AttackTimingAccuracy(playerEnemyVecLenght_, accuracy);



				}
			}
		}
		//}
	}
}

//�v���C���[�ƃG�l�~�[�̏Փ˔��� (���C�L���X�g)
void Player::CollisionRayCastPlayerVsEnemies()
{
	//�v���C���[�̃x�N�g��
	DirectX::XMVECTOR playerVec = DirectX::XMLoadFloat3(&this->position_);
	EnemyManager& enemyManager = EnemyManager::Instance();
	//�G�l�~�[�̐��擾
	int enemyCount = EnemyManager::Instance().GetEnemyCount();
	
	//�����p
	srand((unsigned int)time(NULL));

	for (int i = 0; i < enemyCount; i++)
	{
		//�x�N�g������
		Enemy* enemy = enemyManager.GetEnemy(i);
		DirectX::XMVECTOR enemyVec = DirectX::XMLoadFloat3(&enemy->GetPosition());
		DirectX::XMVECTOR playerEnemyVec = DirectX::XMVectorSubtract(enemyVec, playerVec);
		//���������x�N�g���̋������Z�o
		playerEnemyVecLenght_ = DirectX::XMVectorGetX(DirectX::XMVector3Length(playerEnemyVec));

		//�^�C�~���O���x����Ɏg���x�N�g��
		DirectX::XMVECTOR pushButtonPosVec = DirectX::XMLoadFloat3(&pushButtonPosition_);
		DirectX::XMVECTOR pushButtonVecEnemyVec = DirectX::XMVectorSubtract(enemyVec, pushButtonPosVec);
		//�����Z�o
		float pushButtonEnemyVecLenght = DirectX::XMVectorGetX(DirectX::XMVector3Length(pushButtonVecEnemyVec));

		//�v���C���[ -> �G�l�~�[�ւ̃x�N�g���̒�����10�ȉ��̃G�l�~�[�̂ݔ��肷��
		//�����C���΂����G�l�~�[���������Ă��鎞�̂�
		float range = 10;
		if (playerEnemyVecLenght_ < range && enemy->isAlive_)
		{
			//���C�L���X�g
			Actor enemyActor = enemy->GetCaracterActor();
			//��Q�������蔲����A�W�����v����A�̎��Ƀ��C�L���X�g�̔��ˈʒu��ύX���Ԃ���悤��
			if (isOffsetChange_ == false)
			{
				switch (enemyActor)
				{
				/*case Actor::enemySutter:
					offset_ = 4.0f;
					break;*/
				case Actor::enemySquat:
					offset_ = 3.0f;	//����
					break;
				case Actor::enemyJump:
					offset_ = 1.0f;	//����
					break;
				}
			}
			//���C�̊J�n�ʒu...����
			DirectX::XMFLOAT3 start = { position_.x, position_.y + offset_, position_.z };
			//���C�̏I�_�ʒu...�ړ��̈ʒu 
			DirectX::XMFLOAT3 end = { position_.x, position_.y + offset_, position_.z + 1.0f};

			//���C�L���X�g�ł̓G�Ƃ̔���
			HitResult hit;
			//�Փ˂������̏���
			if (enemy->RayCast(start, end, hit))
			{
				//�O����Փ˂�����
				if (velocity_.z > 0.0f)
				{
					//�G�l�~�[���������Ă��鎞�̂�
					if (enemy->isAlive_)
					{
						enemy->isAlive_ = false;				//�I�u�W�F�N�g�j��
						enemy->SetImpulseFlag(true);				
						this->SetImpulseFlag(true);				//�v���C���[�ɂ��Ռ���^������������

						float impulsX = ImpulsDirection();		//���E�ɐ�����΂��ʂ��擾
						float impulsZ = nowSpeed_ * 1.2f;		//�����̃X�s�[�h�ɂ���Đ�����΂������𒲐�
						if (impulsZ < 40)
							impulsZ = 40.0f;					//�Œᐁ����уX�s�[�h

						enemy->destroyTime_ = 0.5f;						//�������Ԑݒ�
						enemy->impuluseTimer_ = 0.0f;					//�^�C�}�[�������l�ɖ߂�
						enemy->AddImpulse({ impulsX, 10.0, impulsZ });	//�Ռ���^����

						//���Ԃɂ�������0�ɂ���
						timeAccelerationSpeed_ = 0;
						isMaxSpeed_ = false;

						//�R���g���[���[��U��������t���O�ƃ^�C�}�[
						isVibrationFlag_ = true;
						vibrationTimer_ = 0.6f;

						//�J�����V�F�C�N�ׂ̈̃t���O
						isCameraShakeFlag_ = true;
						shakeTimer_ = 0.2f;
						oldPositionX_ = position_.x;

						//SE�Đ�
						seDamage_->Play(false);

						//�R���{�����Z�b�g
						combo_ = 0;
						//�~�X�����̂Ō��݂̃R���{���͍ő�R���{���ł͖����Ȃ���
						isMiss_ = true;
					}
				}

			}
			//�Ԃ���Ȃ�������
			else
			{

				//�ʂ肷������
				if (position_.z > enemy->GetPosition().z)
				{
					range = 50;
					//�{�^�������������̃x�N�g����
					if (pushButtonEnemyVecLenght < range)
					{
						//�^�C�~���O�\��
						Actor enemyActor = enemy->GetCaracterActor();
						switch (enemyActor)
						{
							//���蔲�����Q�������g�̃A�j���\�V����������ʂ���������
						case Character::Actor::enemySquat:	//���蔲�����Q��
							if (animState_ == AnimState::State_Squat)
							{
								float accuracy = 13.0f;	//���苗��
								TimingAccuracy(pushButtonEnemyVecLenght, accuracy);
							}

							break;
							//�W�����v�����Q�������g�̃A�j���[�V�������W�����v��������
						case Character::Actor::enemyJump:	//�W�����v�����Q��
							if (animState_ == AnimState::State_jump)
							{
								float accuracy = 18.0f;	//���苗��
								TimingAccuracy(pushButtonEnemyVecLenght, accuracy);
							}

							break;
						}
					}
				}
			}

			
		}
	}
}

//��Q����ʂ������̔�����x�N�g���ōs��
void Player::CollisionVector(float elapsedTime)
{
	//�v���C���[�̃x�N�g��
	DirectX::XMVECTOR playerVec = DirectX::XMLoadFloat3(&this->position_);
	//�G�̐��擾
	int enemyCount = EnemyManager::Instance().GetEnemyCount();
	EnemyManager& enemyManager = EnemyManager::Instance();

	float maxLenght = FLT_MAX;
	for (int i = 0; i < enemyCount; ++i)
	{
		//�G�l�~�[�̂׃N�g��
		Enemy* enemy = enemyManager.GetEnemy(i);
		DirectX::XMVECTOR enemyVec = DirectX::XMLoadFloat3(&enemy->GetPosition());

		//�v���C���[ -> �G�l�~�[�ւ̃x�N�g������
		DirectX::XMVECTOR playerEnemyVec = DirectX::XMVectorSubtract(enemyVec, playerVec);
		
		//���������x�N�g���̋������Z�o���ۑ�
		playerEnemyVecLenght_ = DirectX::XMVectorGetX(DirectX::XMVector3Length(playerEnemyVec));
		//��ԋ߂��G��ۑ�(ImGui�p)
		if (maxLenght > playerEnemyVecLenght_)
		{
			maxLenght = playerEnemyVecLenght_;
			nextEnemyLenght_ = maxLenght;
		}
		
		
		//�ʂ肷���������
		if (enemy->GetPosition().z < position_.z - 2.0f)
		{
			enemy->ApplyDmaege(DAMAGE, INVINCIBLE_TIME);

		}
	}

}

//�U���������̃^�C�~���O�̐��x����
void Player::AttackTimingAccuracy(float Lenght, float distance)
{
	//�^�C�~���O�\��
	isTimingRender_ = true;		//�`��OK
	timingRenderTimer_ = 0.6;	//�`�掞�Ԑݒ�
	timing_ = Timing::GREAT;
	if (Lenght < distance)
		timing_ = Timing::PERFECT;

	//�R���{
	combo_++;

	//����
	switch (timing_)
	{
	case Timing::GREAT:
		greatCount_++;
		break;
	case Timing::PERFECT:
		perfectCount_++;
		break;
	}
}

//���̑��A�N�V�����̃^�C�~���O���x����
void Player::TimingAccuracy(float Lenght, float distance)
{
	isTimingRender_ = true;			//�`��OK
	timingRenderTimer_ = 0.6f;		//�`�掞�Ԑݒ�

	timing_ = Timing::GREAT;
	if (Lenght < distance)
		timing_ = Timing::PERFECT;

	//���t���[���R���{�����Z�����̂�h��
	if (isCombo_)
	{
		combo_++;	//�R���{���Z
		isCombo_ = false;	//��񂾂�������ʂ�

		switch (timing_)
		{
		case Timing::GREAT:
			greatCount_++;
			break;
		case Timing::PERFECT:
			perfectCount_++;
			break;
		}
	};
}

//���E�����ƈЗ͐ݒ�
float Player::ImpulsDirection()
{
	int rad = rand() % 2;					//���E�ǂ���ɔ�΂��������őI��
	float impulsX;							//�錾	
	if (rad == 0)							//rad��0�Ȃ�E�A1�Ȃ獶�����ɐ�����΂�
		impulsX = 10.0f;					//	
	else
		impulsX = -10.0f;					//

	return impulsX;
}

//������΂��p���[��ݒ�
float Player::ImpulsPower(float magnification)
{
	float impuls = nowSpeed_ * magnification;		//�����̃X�s�[�h�ɂ���Đ�����΂������𒲐�
	
	return impuls;
}

//�R���g���[����U��������
void Player::PadVibration(float elapsedTime)
{
	if (isVibrationFlag_)
		vibrationTimer_ -= elapsedTime;
	if (vibrationTimer_ > 0)
	{
		vibration_.wLeftMotorSpeed = 21000; // use any value between 0-65535 here
		vibration_.wRightMotorSpeed = 21000; // use any value between 0-65535 here
		XInputSetState(word_, &vibration_);
	}
	else
	{
		vibration_.wLeftMotorSpeed = 0; // use any value between 0-65535 here
		vibration_.wRightMotorSpeed = 0; // use any value between 0-65535 here
		XInputSetState(word_, &vibration_);
	}
}

////////////////////////////////////////////////////

//�N���A�t���O
bool Player::StageClear()
{
	if (position_.z > GOAL_LINE)
		isClear_ = true;
	else
		isClear_ = false;

	return isClear_;
}