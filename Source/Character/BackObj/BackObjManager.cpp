#include"BackObjManager.h"

//�X�V����
void BackObjManager::Update(float elapsedTime)
{
	for (BackObj* backObj : backObjs_)
	{
		backObj->Update(elapsedTime);

	}

	//�j������(�Q�[�����ɏ������Ƃ���������K�v������������Ȃ�)
	for (BackObj* backObj : removes_)
	{
		std::vector<BackObj*>::iterator it = std::find(backObjs_.begin(), backObjs_.end(), backObj);
	
		if (it != backObjs_.end())
		{
			backObjs_.erase(it);
		}
	
		//�j��
		delete backObj;
	}
	//�j�����X�g�̃N���A
	removes_.clear();
}

//�`�揈��
void BackObjManager::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	for (BackObj* backObj : backObjs_)
	{
		backObj->Render(dc, shader);
	}
}

//�w�i�I�u�W�F�N�g�o�^
void BackObjManager::Register(BackObj* backObj)
{
	backObjs_.emplace_back(backObj);
}

//�w�i�I�u�W�F�N�g�폜
void BackObjManager::Remove(BackObj* backObj)
{
	//�j�����X�g�ɒǉ�
	removes_.emplace_back(backObj);
}

//�w�i�I�u�W�F�N�g�S�폜
void BackObjManager::Clear()
{
	for (BackObj* backObj : backObjs_)
	{
		if (backObj != nullptr)
		{
			//Remove(backObj);
			delete backObj;
			backObj = nullptr;
		}
	}
}

//�w�i�I�u�W�F�N�g�̃f�o�b�O�v���~�e�B�u�`��
//void BackObjManager::DrawDebugPrimitive()
//{
//	for (BackObj* backObj : backObjs)
//	{
//		backObj->DrawDebugPrimitive();
//	}
//
//}