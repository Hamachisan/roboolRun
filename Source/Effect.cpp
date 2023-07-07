#include "Graphics/Graphics.h"
#include "Effect.h"
#include "EffectManager.h"

//�R���X�g���N�^
Effect::Effect(const char* filename)
{
	//�G�t�F�N�g��ǂݍ��ޑO�Ƀ��b�N����
	//���}���`�X���b�h��Effect���쐬�����DeviceContext�𓯎��A�N�Z�X����
	//�t���[�Y���\��������̂Ŕr�����䂷��
	std::lock_guard<std::mutex> lock(Graphics::Instance().GetMutex());


	//Effekseer�̃��\�[�X��ǂݍ���
	//Effekseer��UEF-16�̃t�@�C���p�X�ȊO�͑Ή����Ă��Ȃ����ߕ����R�[�h�ϊ����K�v
	char16_t utf16Filename[256];
	Effekseer::ConvertUtf8ToUtf16(utf16Filename, 256, filename);

	//Effekseer::manager���擾
	Effekseer::Manager* effekseerManager = EffectManager::Instane().GetEffeckseerManager();

	//effekseer�G�t�F�N�g��ǂݍ���
	effekseerEffect = Effekseer::Effect::Create(effekseerManager, (EFK_CHAR*)utf16Filename);

}

//�f�X�g���N�^
Effect::~Effect()
{
	effekseerEffect->Release();
	effekseerEffect = nullptr;
}

//�Đ�
Effekseer::Handle Effect::Play(const DirectX::XMFLOAT3& position, float scale)
{
	Effekseer::Manager* effekseerManager = EffectManager::Instane().GetEffeckseerManager();

	Effekseer::Handle handle = effekseerManager->Play(effekseerEffect,
		position.x, position.y, position.z);
	effekseerManager->SetScale(handle, scale, scale, scale);
	return handle;
}

//��~
void Effect::Stop(Effekseer::Handle handle)
{
	Effekseer::Manager* effekseerManager = EffectManager::Instane().GetEffeckseerManager();
	effekseerManager->StopEffect(handle);
}

//���W�ݒ�
void Effect::setPosition(Effekseer::Handle handle, const DirectX::XMFLOAT3& position)
{
	Effekseer::Manager* effekseerManager = EffectManager::Instane().GetEffeckseerManager();
	effekseerManager->SetLocation(handle, position.x, position.y, position.z);
}

//�X�P�[���ݒ�
void Effect::SetScale(Effekseer::Handle handle, const DirectX::XMFLOAT3& scale)
{
	Effekseer::Manager* effekseerManager = EffectManager::Instane().GetEffeckseerManager();
	effekseerManager->SetScale(handle, scale.x, scale.y, scale.z);
}