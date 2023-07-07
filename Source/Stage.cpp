#include"Stage.h"

static Stage* instance = nullptr;

//�R���X�g���N�^
Stage::Stage()
{
	//�X�e�[�W���f����ǂݍ���
	model_ = new Model("Data/Model/Stage/stage.mdl");
	
	//�C���X�^���X
	instance = this;
}

//�f�X�g���N�^
Stage::~Stage()
{
	//�X�e�[�W���f����j��
	delete model_;
}

//�X�V����
void Stage::Update(float elapsedTime)
{
	//���͂܂�����!!
}

//�`�揈��
void Stage::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	//�V�F�[�_�[�Ƀ��f����`�悵�Ă��炤
	shader->Draw(dc, model_);
}

//���C�L���X�g
bool Stage::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	return Collision::IntersectRayVsModel(start, end, model_, hit);
}

//�C���X�^���X�擾
Stage& Stage::Instance()
{
	return *instance;
}