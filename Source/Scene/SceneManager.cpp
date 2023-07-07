#include "SceneManager.h"

//更新処理
void SceneManager::Update(float elapsedTime)
{
	if (currntScene_ != nullptr)
	{
		currntScene_->Update(elapsedTime);
	}
}

//描画処理
void SceneManager::Render()
{
	if (currntScene_ != nullptr)
	{
		currntScene_->Render();
	}
}

//シーンクリア
void SceneManager::Clear()
{
	if (currntScene_ != nullptr)
	{
		currntScene_->Finalize();
		delete currntScene_;
		currntScene_= nullptr;
	}
}

//シーン切り替え
void SceneManager::ChangeScene(Scene* scene)
{
	//古いシーンを終了処理
	Clear();

	//新しいシーンを設定
	currntScene_ = scene;

	//未初期化の場合は初期化処理
	if (currntScene_->IsReady() == false)
	{
		//シーン初期化処理
		currntScene_->Initialize();
	}

}