#include "Graphics/Graphics.h"
#include "Graphics/Model.h"
#include "Graphics/ResourceManager.h"

// コンストラクタ
Model::Model(const char* filename)
{
	// リソース読み込み
	//resource = std::make_shared<ModelResource>();
	//resource->Load(Graphics::Instance().GetDevice(), filename);
	resource = ResourceManager::Instance()->LoadModelResource(filename);


	// ノード
	const std::vector<ModelResource::Node>& resNodes = resource->GetNodes();

	nodes.resize(resNodes.size());
	for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
	{
		auto&& src = resNodes.at(nodeIndex);
		auto&& dst = nodes.at(nodeIndex);

		dst.name = src.name.c_str();
		dst.parent = src.parentIndex >= 0 ? &nodes.at(src.parentIndex) : nullptr;
		dst.scale = src.scale;
		dst.rotate = src.rotate;
		dst.translate = src.translate;

		if (dst.parent != nullptr)
		{
			dst.parent->children.emplace_back(&dst);
		}
	}

	// 行列計算
	const DirectX::XMFLOAT4X4 transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	UpdateTransform(transform);
}

// 変換行列計算
void Model::UpdateTransform(const DirectX::XMFLOAT4X4& transform)
{
	DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);

	for (Node& node : nodes)
	{
		// ローカル行列算出
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(node.scale.x, node.scale.y, node.scale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&node.rotate));
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(node.translate.x, node.translate.y, node.translate.z);
		DirectX::XMMATRIX LocalTransform = S * R * T;

		// ワールド行列算出
		DirectX::XMMATRIX ParentTransform;
		if (node.parent != nullptr)
		{
			ParentTransform = DirectX::XMLoadFloat4x4(&node.parent->worldTransform);
		}
		else
		{
			ParentTransform = Transform;
		}
		DirectX::XMMATRIX WorldTransform = LocalTransform * ParentTransform;

		// 計算結果を格納
		DirectX::XMStoreFloat4x4(&node.localTransform, LocalTransform);
		DirectX::XMStoreFloat4x4(&node.worldTransform, WorldTransform);
	}
}

// アニメーション更新処理
void Model::UpdateAnimation(float elapsedTime)
{
	using namespace DirectX;

	// 再生中なら処理しない
	if (!IsPlayAnimation()) return;

	// ブレンド率の計算
	float blendRate = 1.0f;
	blendRate = currentAnimationSeconds / animationBlendSeconds;

	// 指定のアニメーション取得
	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(currentAnimationIndex);

	// アニメーションデータからキーフレームデータリストを取得
	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
	int KeyCount = static_cast<int>(keyframes.size());
	for (int keyIndex = 0; keyIndex < KeyCount -1; ++keyIndex)
	{
		// 現在の時間がどのキーフレームの間にいるか判定する
		const ModelResource::Keyframe& keyframe0 = keyframes.at(keyIndex);
		const ModelResource::Keyframe& keyframe1 = keyframes.at(keyIndex + 1);
		if (currentAnimationSeconds >= keyframe0.seconds &&
			currentAnimationSeconds < keyframe1.seconds)
		{
			// 再生時間とキーフレームの時間から補完率を計算する
			float rate = (currentAnimationSeconds - keyframe0.seconds) / 
				(keyframe1.seconds - keyframe0.seconds);

			int nodeCount = static_cast<int>(nodes.size());
			for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
			{
				// 2つのキーフレームの間の補完計算
				const ModelResource::NodeKeyData& key0 = keyframe0.nodeKeys.at(nodeIndex);
				const ModelResource::NodeKeyData& key1 = keyframe1.nodeKeys.at(nodeIndex);

				Node& node = nodes[nodeIndex];

				//ブレンド補完処理
				if (blendRate < 1.0f)
				{
					//現在の姿勢と次のキーフレームとの姿勢の補完
					XMStoreFloat3(&node.translate, XMVectorLerp(XMLoadFloat3(&node.translate), XMLoadFloat3(&key1.translate), blendRate));
					XMStoreFloat4(&node.rotate, XMQuaternionSlerp(XMLoadFloat4(&node.rotate), XMLoadFloat4(&key1.rotate), blendRate));
				}
				else
				{
					//前のキーフレームと次のキーフレームの姿勢を補完
					XMStoreFloat3(&node.translate,
						XMVectorLerp(
							XMLoadFloat3(&key0.translate), XMLoadFloat3(&key1.translate), rate));
					XMStoreFloat4(&node.rotate,
						XMQuaternionSlerp(XMLoadFloat4(&key0.rotate), XMLoadFloat4(&key1.rotate), rate));
				}
			}

			break;
		}
	}

	// 最終フレーム処理　
	if (animationEndFrag)
	{
		animationEndFrag = false;
		currentAnimationIndex = -1;
		return;
	}

	// 時間経過
	currentAnimationSeconds +=  elapsedTime * animationPlaySpeed;

	// 再生時間が終端時間を超えたら
	if (currentAnimationSeconds >= animation.secondsLength)
	{
		if (animationLoopFrag)
		{
			// 再生時間を巻き戻す
			currentAnimationSeconds -= animation.secondsLength;
		}
		else
		{
			// 再生終了
			animationEndFrag = true;
		}
	}
}

// アニメーション再生
void Model::PlayAnimation(int index, bool loop, float blendSeconds, float playSpeed)
{
	currentAnimationIndex = index;
	currentAnimationSeconds = 0.0f;
	animationBlendTime = 0.0f;
	animationBlendSeconds = blendSeconds;
	animationLoopFrag = loop;
	animationPlaySpeed = playSpeed;

}

// アニメーション再生中か
bool Model::IsPlayAnimation() const
{
	if (currentAnimationIndex < 0) return false;
	if (currentAnimationIndex >= resource->GetAnimations().size()) return false;
	return true;
}