#include"Stage.h"

static Stage* instance = nullptr;

//コンストラクタ
Stage::Stage()
{
	//ステージモデルを読み込み
	model_ = new Model("Data/Model/Stage/stage.mdl");
	
	//インスタンス
	instance = this;
}

//デストラクタ
Stage::~Stage()
{
	//ステージモデルを破棄
	delete model_;
}

//更新処理
void Stage::Update(float elapsedTime)
{
	//今はまだだよ!!
}

//描画処理
void Stage::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	//シェーダーにモデルを描画してもらう
	shader->Draw(dc, model_);
}

//レイキャスト
bool Stage::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	return Collision::IntersectRayVsModel(start, end, model_, hit);
}

//インスタンス取得
Stage& Stage::Instance()
{
	return *instance;
}