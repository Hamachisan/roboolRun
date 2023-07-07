#pragma once

#include "Scene.h"
#include "Other/Constant.h"

//�V�[���}�l�[�W���[
class SceneManager
{
private:
	SceneManager() {}
	~SceneManager() {}

public:
	//�B��̃C���X�^���X�擾
	static SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}

	//�X�V����
	void Update(float elapsedTime);

	//�`�揈��
	void Render();

	//�V�[���N���A
	void Clear();

	//�V�[���؂�ւ�
	void ChangeScene(Scene* scene);

private:
	Scene* currntScene_ = nullptr;

private:
	int stageNum_ = 0;
	int nextScene_ = NEXT_SCENE_OTHER;


public:
	void SetStageNum(int stageNum) { stageNum_ = stageNum; }
	int GetStageNum() { return stageNum_; }

	void SetNextScene(int nextScene) { nextScene_ = nextScene; }
	int GetNextScene() { return nextScene_; }
};
