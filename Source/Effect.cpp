#include "Graphics/Graphics.h"
#include "Effect.h"
#include "EffectManager.h"

//コンストラクタ
Effect::Effect(const char* filename)
{
	//エフェクトを読み込む前にロックする
	//※マルチスレッドでEffectを作成するとDeviceContextを同時アクセスして
	//フリーズすつ可能性があるので排他制御する
	std::lock_guard<std::mutex> lock(Graphics::Instance().GetMutex());


	//Effekseerのリソースを読み込む
	//EffekseerはUEF-16のファイルパス以外は対応していないため文字コード変換が必要
	char16_t utf16Filename[256];
	Effekseer::ConvertUtf8ToUtf16(utf16Filename, 256, filename);

	//Effekseer::managerを取得
	Effekseer::Manager* effekseerManager = EffectManager::Instane().GetEffeckseerManager();

	//effekseerエフェクトを読み込み
	effekseerEffect = Effekseer::Effect::Create(effekseerManager, (EFK_CHAR*)utf16Filename);

}

//デストラクタ
Effect::~Effect()
{
	effekseerEffect->Release();
	effekseerEffect = nullptr;
}

//再生
Effekseer::Handle Effect::Play(const DirectX::XMFLOAT3& position, float scale)
{
	Effekseer::Manager* effekseerManager = EffectManager::Instane().GetEffeckseerManager();

	Effekseer::Handle handle = effekseerManager->Play(effekseerEffect,
		position.x, position.y, position.z);
	effekseerManager->SetScale(handle, scale, scale, scale);
	return handle;
}

//停止
void Effect::Stop(Effekseer::Handle handle)
{
	Effekseer::Manager* effekseerManager = EffectManager::Instane().GetEffeckseerManager();
	effekseerManager->StopEffect(handle);
}

//座標設定
void Effect::setPosition(Effekseer::Handle handle, const DirectX::XMFLOAT3& position)
{
	Effekseer::Manager* effekseerManager = EffectManager::Instane().GetEffeckseerManager();
	effekseerManager->SetLocation(handle, position.x, position.y, position.z);
}

//スケール設定
void Effect::SetScale(Effekseer::Handle handle, const DirectX::XMFLOAT3& scale)
{
	Effekseer::Manager* effekseerManager = EffectManager::Instane().GetEffeckseerManager();
	effekseerManager->SetScale(handle, scale.x, scale.y, scale.z);
}