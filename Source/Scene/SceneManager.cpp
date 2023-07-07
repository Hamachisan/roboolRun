#include "SceneManager.h"

//�X�V����
void SceneManager::Update(float elapsedTime)
{
	if (currntScene_ != nullptr)
	{
		currntScene_->Update(elapsedTime);
	}
}

//�`�揈��
void SceneManager::Render()
{
	if (currntScene_ != nullptr)
	{
		currntScene_->Render();
	}
}

//�V�[���N���A
void SceneManager::Clear()
{
	if (currntScene_ != nullptr)
	{
		currntScene_->Finalize();
		delete currntScene_;
		currntScene_= nullptr;
	}
}

//�V�[���؂�ւ�
void SceneManager::ChangeScene(Scene* scene)
{
	//�Â��V�[�����I������
	Clear();

	//�V�����V�[����ݒ�
	currntScene_ = scene;

	//���������̏ꍇ�͏���������
	if (currntScene_->IsReady() == false)
	{
		//�V�[������������
		currntScene_->Initialize();
	}

}