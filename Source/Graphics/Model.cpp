#include "Graphics/Graphics.h"
#include "Graphics/Model.h"
#include "Graphics/ResourceManager.h"

// �R���X�g���N�^
Model::Model(const char* filename)
{
	// ���\�[�X�ǂݍ���
	//resource = std::make_shared<ModelResource>();
	//resource->Load(Graphics::Instance().GetDevice(), filename);
	resource = ResourceManager::Instance()->LoadModelResource(filename);


	// �m�[�h
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

	// �s��v�Z
	const DirectX::XMFLOAT4X4 transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	UpdateTransform(transform);
}

// �ϊ��s��v�Z
void Model::UpdateTransform(const DirectX::XMFLOAT4X4& transform)
{
	DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);

	for (Node& node : nodes)
	{
		// ���[�J���s��Z�o
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(node.scale.x, node.scale.y, node.scale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&node.rotate));
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(node.translate.x, node.translate.y, node.translate.z);
		DirectX::XMMATRIX LocalTransform = S * R * T;

		// ���[���h�s��Z�o
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

		// �v�Z���ʂ��i�[
		DirectX::XMStoreFloat4x4(&node.localTransform, LocalTransform);
		DirectX::XMStoreFloat4x4(&node.worldTransform, WorldTransform);
	}
}

// �A�j���[�V�����X�V����
void Model::UpdateAnimation(float elapsedTime)
{
	using namespace DirectX;

	// �Đ����Ȃ珈�����Ȃ�
	if (!IsPlayAnimation()) return;

	// �u�����h���̌v�Z
	float blendRate = 1.0f;
	blendRate = currentAnimationSeconds / animationBlendSeconds;

	// �w��̃A�j���[�V�����擾
	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(currentAnimationIndex);

	// �A�j���[�V�����f�[�^����L�[�t���[���f�[�^���X�g���擾
	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
	int KeyCount = static_cast<int>(keyframes.size());
	for (int keyIndex = 0; keyIndex < KeyCount -1; ++keyIndex)
	{
		// ���݂̎��Ԃ��ǂ̃L�[�t���[���̊Ԃɂ��邩���肷��
		const ModelResource::Keyframe& keyframe0 = keyframes.at(keyIndex);
		const ModelResource::Keyframe& keyframe1 = keyframes.at(keyIndex + 1);
		if (currentAnimationSeconds >= keyframe0.seconds &&
			currentAnimationSeconds < keyframe1.seconds)
		{
			// �Đ����ԂƃL�[�t���[���̎��Ԃ���⊮�����v�Z����
			float rate = (currentAnimationSeconds - keyframe0.seconds) / 
				(keyframe1.seconds - keyframe0.seconds);

			int nodeCount = static_cast<int>(nodes.size());
			for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
			{
				// 2�̃L�[�t���[���̊Ԃ̕⊮�v�Z
				const ModelResource::NodeKeyData& key0 = keyframe0.nodeKeys.at(nodeIndex);
				const ModelResource::NodeKeyData& key1 = keyframe1.nodeKeys.at(nodeIndex);

				Node& node = nodes[nodeIndex];

				//�u�����h�⊮����
				if (blendRate < 1.0f)
				{
					//���݂̎p���Ǝ��̃L�[�t���[���Ƃ̎p���̕⊮
					XMStoreFloat3(&node.translate, XMVectorLerp(XMLoadFloat3(&node.translate), XMLoadFloat3(&key1.translate), blendRate));
					XMStoreFloat4(&node.rotate, XMQuaternionSlerp(XMLoadFloat4(&node.rotate), XMLoadFloat4(&key1.rotate), blendRate));
				}
				else
				{
					//�O�̃L�[�t���[���Ǝ��̃L�[�t���[���̎p����⊮
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

	// �ŏI�t���[�������@
	if (animationEndFrag)
	{
		animationEndFrag = false;
		currentAnimationIndex = -1;
		return;
	}

	// ���Ԍo��
	currentAnimationSeconds +=  elapsedTime * animationPlaySpeed;

	// �Đ����Ԃ��I�[���Ԃ𒴂�����
	if (currentAnimationSeconds >= animation.secondsLength)
	{
		if (animationLoopFrag)
		{
			// �Đ����Ԃ������߂�
			currentAnimationSeconds -= animation.secondsLength;
		}
		else
		{
			// �Đ��I��
			animationEndFrag = true;
		}
	}
}

// �A�j���[�V�����Đ�
void Model::PlayAnimation(int index, bool loop, float blendSeconds, float playSpeed)
{
	currentAnimationIndex = index;
	currentAnimationSeconds = 0.0f;
	animationBlendTime = 0.0f;
	animationBlendSeconds = blendSeconds;
	animationLoopFrag = loop;
	animationPlaySpeed = playSpeed;

}

// �A�j���[�V�����Đ�����
bool Model::IsPlayAnimation() const
{
	if (currentAnimationIndex < 0) return false;
	if (currentAnimationIndex >= resource->GetAnimations().size()) return false;
	return true;
}