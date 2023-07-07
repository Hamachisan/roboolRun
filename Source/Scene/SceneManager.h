#pragma once

#include "Scene.h"
#include "Other/Constant.h"

//シーンマネージャー
class SceneManager
{
private:
	SceneManager() {}
	~SceneManager() {}

public:
	//唯一のインスタンス取得
	static SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}

	//更新処理
	void Update(float elapsedTime);

	//描画処理
	void Render();

	//シーンクリア
	void Clear();

	//シーン切り替え
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
