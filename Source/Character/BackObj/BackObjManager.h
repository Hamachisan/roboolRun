#pragma once

#include<vector>
#include"BackObj.h"

//�w�i�I�u�W�F�N�g�[�}�l�[�W���[
class BackObjManager
{
private:
	BackObjManager() {}
	~BackObjManager() {}

public:
	//�B��̃C���X�^���X�擾
	static BackObjManager& Instance()
	{
		static BackObjManager instance;
		return instance;
	}

	//�X�V����
	void Update(float elapsedTime);

	//�`�揈��
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	//�w�i�I�u�W�F�N�g�o�^
	void Register(BackObj* backObj);

	//�w�i�I�u�W�F�N�g���擾
	int GetBackObjCount() const { return static_cast<int>(backObjs_.size()); }

	//�w�i�I�u�W�F�N�g�擾
	BackObj* GetBackObj(int index) { return backObjs_.at(index); }

	//�w�i�I�u�W�F�N�g�폜
	void Remove(BackObj* backObj);

	//�w�i�I�u�W�F�N�g�S�폜
	void Clear();

	//�f�o�b�O�v���~�e�B�u�`��
	//void DrawDebugPrimitive();

private:
	std::vector<BackObj*> backObjs_;

	std::vector<BackObj*> removes_;
};
