#include"Collision.h"

//���Ƌ��̌�������
bool Collision::IntersectSphereVsSphere(
	const DirectX::XMFLOAT3& positionA,
	float radiusA,
	const DirectX::XMFLOAT3& positionB,
	float radiusB,
	DirectX::XMFLOAT3& outPositionB	//�����ꂽ���̏C����̃|�W�V����
)
{
	//B->A�̒P�ʃx�N�g�����Z�o
	//��`��float�ō���Čv�Z����Ƃ���vector�ɕϊ����邱�ƁI
	DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);	
	DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);	//���[�h�֐��̓x�N�^�[�ɕϊ�
	DirectX::XMVECTOR Vec       = 
		DirectX::XMVectorSubtract(PositionA, PositionB);				//�����Z 2�̃x�N�g���̍����v�Z
																		//�����̏��ԂɋC��t���āI�I
	DirectX::XMVECTOR LengthSq  = DirectX::XMVector3LengthSq(Vec);		//3D�x�N�g���̒�����2����v�Z

	float lengthSq; //sq�͕����@�Q��
	DirectX::XMStoreFloat(&lengthSq, LengthSq);							//�X�g�A�֐��͋t�Ńt���[�g�ɕϊ�		

	//��������
	float range = radiusA + radiusB;	
	if (lengthSq > range * range ) //������Ȃ���
	{
		return false;
	}

	//�����������̏���
	//A��B�������o��
	Vec = DirectX::XMVector3Normalize(Vec);				//�m�[�}���C�Y�@���@���K��
	Vec = DirectX::XMVectorScale(Vec, -range);			//�傫���̒���
	PositionB = DirectX::XMVectorAdd(PositionA, Vec);	//A+Vec�ŐV�����_�����߂�
	DirectX::XMStoreFloat3(&outPositionB, PositionB);	//out�ɑ��
	return true;
}

//�~���Ɖ~���̌�������
bool Collision::IntersectCylinderVsCylinder(
	const DirectX::XMFLOAT3& positionA,
	float radiusA,
	float heightA,
	const DirectX::XMFLOAT3& positionB,
	float radiusB,
	float heightB,
	DirectX::XMFLOAT3& outPositionB
)
{
	//A�̑�����B�̓�����Ȃ瓖�����Ă��Ȃ�
	if (positionA.y > heightB + positionB.y)
	{
		return false;
	}
	//A�̓���B�̑�����艺�Ȃ瓖�����Ă��Ȃ�
	if (heightA + positionA.y < positionB.y)
	{
		return false;
	}
	//XZ���ʂł͈̔̓`�F�b�N
	float dx = positionA.x - positionB.x;
	float dz = positionA.z - positionB.z;
	float lengthSq = dx * dx + dz * dz;

	float range = radiusA + radiusB;
	if (lengthSq > range * range)
	{
		return false;
	}

	//A��B�������o��
	float length = sqrtf(lengthSq);
	dx /= length;
	dz /= length;									//���K��
	outPositionB.x = positionA.x - dx * range;
	outPositionB.z = positionA.z - dz * range;		//out = A - vec * range
	outPositionB.y = positionB.y;					//y��B, y���ێ�
	
	return true;
}

//���Ɖ~���̌�������
bool Collision::IntersectSphereVsCylinder(
	const DirectX::XMFLOAT3& spherePosition,
	float sphereRadius,
	const DirectX::XMFLOAT3& cylinderPosition,
	float cylinderRadius,
	float cylinderHeight,
	DirectX::XMFLOAT3& outCylinderPosition
)
{
	//���̑������~���̓�����Ȃ�O��
	if (spherePosition.y - sphereRadius > cylinderPosition.y + cylinderHeight)
		return false;

	//�~���̑��������̓�����Ȃ�O��
	if (cylinderPosition.y > spherePosition.y + sphereRadius)
		return false;

	//�~��->���̃x�N�g���쐬
	float dx = spherePosition.x - cylinderPosition.x;
	float dz = spherePosition.z - cylinderPosition.z;
	float lengthSq = dx * dx + dz * dz;

	//�����v�Z
	float range = sphereRadius + cylinderRadius;

	//����
	if (lengthSq > range * range)
		return false;

	return true;
}


//���C�ƃ��f���̌�������
bool Collision::IntersectRayVsModel(
	const DirectX::XMFLOAT3& start,
	const DirectX::XMFLOAT3& end,
	const Model* model,
	HitResult& result
)
{
#if 1
	using namespace DirectX; //XMStoreFloat�݂̂����܂��ɂȂ鎞������B���̂Ƃ��͏ȗ����Ȃ�

	XMVECTOR WorldStart = XMLoadFloat3(&start);
	XMVECTOR WorldEnd = XMLoadFloat3(&end);
	XMVECTOR WorldRayVec = XMVectorSubtract(WorldEnd, WorldStart);
	XMVECTOR WorldRayLength = XMVector3Length(WorldRayVec);

	//���[���h��Ԃ̃��C�̒���
	XMStoreFloat(&result.distance, WorldRayLength);

	bool hit = false;
	const ModelResource* resource = model->GetResource();
	for (const ModelResource::Mesh& mesh : resource->GetMeshes())
	{
		//���b�V���m�[�h�擾
		const Model::Node& node = model->GetNodes().at(mesh.nodeIndex);
		

		//���C�����[���h��Ԃ��烍�[�J����Ԃ֕ϊ�
		XMMATRIX WorldTransform = XMLoadFloat4x4(&node.worldTransform);
		XMMATRIX InverseWorldTransform = XMMatrixInverse(nullptr, WorldTransform);	//�t�s��
		
		XMVECTOR S = XMVector3TransformCoord(WorldStart, InverseWorldTransform);
		XMVECTOR E = XMVector3TransformCoord(WorldEnd, InverseWorldTransform);
		XMVECTOR SE = XMVectorSubtract(E, S);
		XMVECTOR V = XMVector3Normalize(SE);
		XMVECTOR Length = XMVector3Length(SE);

		//���C�̒���
		float neart;
		XMStoreFloat(&neart, Length);

		//�O�p�`(��)�Ƃ̌�������
		const std::vector<ModelResource::Vertex>& vertices = mesh.vertices;
		const std::vector<UINT> indices = mesh.indices;	
		// �����_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@����O�p�`�𒊏o����

		int materialIndex = -1;
		XMVECTOR HitPosition;
		XMVECTOR HitNormal;
		for (const ModelResource::Subset& subset : mesh.subsets)
		{
			for (UINT i = 0; i < subset.indexCount; i += 3)
			{
				UINT index = subset.startIndex + i;

				//�O�p�`�̒��_�𒊏o
				const ModelResource::Vertex& a = vertices.at(indices.at(index));
				const ModelResource::Vertex& b = vertices.at(indices.at(index + 1));
				const ModelResource::Vertex& c = vertices.at(indices.at(index + 2));

				XMVECTOR A = XMLoadFloat3(&a.position);
				XMVECTOR B = XMLoadFloat3(&b.position);
				XMVECTOR C = XMLoadFloat3(&c.position);
 
				//�O�p�`�̎O�Ӄx�N�g�����Z�o
				XMVECTOR AB = B - A;
				XMVECTOR BC = C - B;
				XMVECTOR CA = A - C;

				//�O�p�`�̖@���x�N�g�����Z�o
				XMVECTOR N = XMVector3Cross(AB, BC);

				//���ς̌��ʂ��v���X�Ȃ�Ε\����
				XMVECTOR Dot = XMVector3Dot(N, V);
				float dot;
				XMStoreFloat(&dot, Dot);
				if (dot >= 0.0f) continue;	//���Ȃ�R���e�j���[

				//���C�ƕ��ʂ̌�_���Z�o
				XMVECTOR SA = XMVectorSubtract(A, S);	//���炭�ˉe
				XMVECTOR X = XMVectorDivide(XMVector3Dot(SA, N), Dot); 
				float x;
				XMStoreFloat(&x, X);
				if (x < .0f || x > neart) continue;		//��_�܂ł̋��������܂łɌv�Z�����ŋߋ������
														//�傫���Ƃ��̓X�L�b�v����
				XMVECTOR P = XMVectorAdd(XMVectorMultiply(V, X), S);

				//�܂������K�v�ۂ�
				//float sa;
				//XMStoreFloat(&sa, SA);
				//
				//XMVECTOR D2 = XMVector3Dot(T, Normal);
				//float d2;
				//XMStoreFloat(&d2, D2);
				//
				//float t;
				//float x = t / d2;


				//��_���O�p�`�̓����ɂ��邩����
				//1��
				XMVECTOR PA = XMVectorSubtract(A, P);
				XMVECTOR Cross1 = XMVector3Cross(PA, AB); //AB ,AP�x�N�g��
				XMVECTOR Dot1 = XMVector3Dot(Cross1, N);
				float dot1;
				XMStoreFloat(&dot1, Dot1);
				if (dot1 < 0.0f) continue;	//90�x�𒴂�����R���e�j���[

				//2��
				XMVECTOR PB = XMVectorSubtract(B, P);
				XMVECTOR Cross2 = XMVector3Cross(PB, BC); //BC , BP�x�N�g��
				XMVECTOR Dot2 = XMVector3Dot(Cross2, N);
				float dot2;
				XMStoreFloat(&dot2, Dot2);
				if (dot2 < 0.0f) continue;

				//3��
				XMVECTOR PC = XMVectorSubtract(C, P);
				XMVECTOR Cross3 = XMVector3Cross(PC, CA); 
				XMVECTOR Dot3 = XMVector3Dot(Cross3, N);
				float dot3;
				XMStoreFloat(&dot3, Dot3);
				if (dot3 < 0.0f) continue;


				//�ŋߋ������X�V
				neart = x;

				//��_�Ɩ@�����X�V
				HitPosition = P;
				HitNormal = N;
				materialIndex = subset.materialIndex;
			}
		}
		if (materialIndex >= 0)
		{
			//���[�J����Ԃ��烏�[���h��Ԃ֕ϊ�
			XMVECTOR WorldPosition = XMVector3TransformCoord(HitPosition, WorldTransform);

			XMVECTOR WorldCrossVec = XMVectorSubtract(WorldPosition, WorldStart);
			XMVECTOR WorldCrossLength = XMVector3Length(WorldCrossVec);
			float distance;
			XMStoreFloat(&distance, WorldCrossLength);

			//�q�b�g���ۑ�
			if (result.distance > distance) 
			{
				XMVECTOR WorldNormal = XMVector3TransformNormal(HitNormal, WorldTransform);

				result.distance = distance;
				result.materialIndex = materialIndex;
				DirectX::XMStoreFloat3(&result.position, WorldPosition);
				DirectX::XMStoreFloat3(&result.normal, XMVector3Normalize(WorldNormal));
				hit = true;
			}
		}
	}

	return hit;

#endif
}

