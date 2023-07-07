#include"Collision.h"

//球と球の交差判定
bool Collision::IntersectSphereVsSphere(
	const DirectX::XMFLOAT3& positionA,
	float radiusA,
	const DirectX::XMFLOAT3& positionB,
	float radiusB,
	DirectX::XMFLOAT3& outPositionB	//押された側の修正後のポジション
)
{
	//B->Aの単位ベクトルを算出
	//定義はfloatで作って計算するときにvectorに変換すること！
	DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);	
	DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);	//ロード関数はベクターに変換
	DirectX::XMVECTOR Vec       = 
		DirectX::XMVectorSubtract(PositionA, PositionB);				//引き算 2つのベクトルの差を計算
																		//引数の順番に気を付けて！！
	DirectX::XMVECTOR LengthSq  = DirectX::XMVector3LengthSq(Vec);		//3Dベクトルの長さの2乗を計算

	float lengthSq; //sqは平方　２乗
	DirectX::XMStoreFloat(&lengthSq, LengthSq);							//ストア関数は逆でフロートに変換		

	//距離判定
	float range = radiusA + radiusB;	
	if (lengthSq > range * range ) //当たらない時
	{
		return false;
	}

	//当たった時の処理
	//AがBを押し出す
	Vec = DirectX::XMVector3Normalize(Vec);				//ノーマライズ　→　正規化
	Vec = DirectX::XMVectorScale(Vec, -range);			//大きさの調整
	PositionB = DirectX::XMVectorAdd(PositionA, Vec);	//A+Vecで新しい点を求める
	DirectX::XMStoreFloat3(&outPositionB, PositionB);	//outに代入
	return true;
}

//円柱と円柱の交差判定
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
	//Aの足元がBの頭より上なら当たっていない
	if (positionA.y > heightB + positionB.y)
	{
		return false;
	}
	//Aの頭がBの足元より下なら当たっていない
	if (heightA + positionA.y < positionB.y)
	{
		return false;
	}
	//XZ平面での範囲チェック
	float dx = positionA.x - positionB.x;
	float dz = positionA.z - positionB.z;
	float lengthSq = dx * dx + dz * dz;

	float range = radiusA + radiusB;
	if (lengthSq > range * range)
	{
		return false;
	}

	//AがBを押し出す
	float length = sqrtf(lengthSq);
	dx /= length;
	dz /= length;									//正規化
	outPositionB.x = positionA.x - dx * range;
	outPositionB.z = positionA.z - dz * range;		//out = A - vec * range
	outPositionB.y = positionB.y;					//yはB, yを維持
	
	return true;
}

//球と円柱の交差判定
bool Collision::IntersectSphereVsCylinder(
	const DirectX::XMFLOAT3& spherePosition,
	float sphereRadius,
	const DirectX::XMFLOAT3& cylinderPosition,
	float cylinderRadius,
	float cylinderHeight,
	DirectX::XMFLOAT3& outCylinderPosition
)
{
	//球の足元が円柱の頭より上なら外れ
	if (spherePosition.y - sphereRadius > cylinderPosition.y + cylinderHeight)
		return false;

	//円柱の足元が球の頭より上なら外れ
	if (cylinderPosition.y > spherePosition.y + sphereRadius)
		return false;

	//円柱->球のベクトル作成
	float dx = spherePosition.x - cylinderPosition.x;
	float dz = spherePosition.z - cylinderPosition.z;
	float lengthSq = dx * dx + dz * dz;

	//距離計算
	float range = sphereRadius + cylinderRadius;

	//判定
	if (lengthSq > range * range)
		return false;

	return true;
}


//レイとモデルの交差判定
bool Collision::IntersectRayVsModel(
	const DirectX::XMFLOAT3& start,
	const DirectX::XMFLOAT3& end,
	const Model* model,
	HitResult& result
)
{
#if 1
	using namespace DirectX; //XMStoreFloatのみあいまいになる時がある。そのときは省略しない

	XMVECTOR WorldStart = XMLoadFloat3(&start);
	XMVECTOR WorldEnd = XMLoadFloat3(&end);
	XMVECTOR WorldRayVec = XMVectorSubtract(WorldEnd, WorldStart);
	XMVECTOR WorldRayLength = XMVector3Length(WorldRayVec);

	//ワールド空間のレイの長さ
	XMStoreFloat(&result.distance, WorldRayLength);

	bool hit = false;
	const ModelResource* resource = model->GetResource();
	for (const ModelResource::Mesh& mesh : resource->GetMeshes())
	{
		//メッシュノード取得
		const Model::Node& node = model->GetNodes().at(mesh.nodeIndex);
		

		//レイをワールド空間からローカル空間へ変換
		XMMATRIX WorldTransform = XMLoadFloat4x4(&node.worldTransform);
		XMMATRIX InverseWorldTransform = XMMatrixInverse(nullptr, WorldTransform);	//逆行列
		
		XMVECTOR S = XMVector3TransformCoord(WorldStart, InverseWorldTransform);
		XMVECTOR E = XMVector3TransformCoord(WorldEnd, InverseWorldTransform);
		XMVECTOR SE = XMVectorSubtract(E, S);
		XMVECTOR V = XMVector3Normalize(SE);
		XMVECTOR Length = XMVector3Length(SE);

		//レイの長さ
		float neart;
		XMStoreFloat(&neart, Length);

		//三角形(面)との交差判定
		const std::vector<ModelResource::Vertex>& vertices = mesh.vertices;
		const std::vector<UINT> indices = mesh.indices;	
		// ↑頂点バッファとインデックスバッファから三角形を抽出する

		int materialIndex = -1;
		XMVECTOR HitPosition;
		XMVECTOR HitNormal;
		for (const ModelResource::Subset& subset : mesh.subsets)
		{
			for (UINT i = 0; i < subset.indexCount; i += 3)
			{
				UINT index = subset.startIndex + i;

				//三角形の頂点を抽出
				const ModelResource::Vertex& a = vertices.at(indices.at(index));
				const ModelResource::Vertex& b = vertices.at(indices.at(index + 1));
				const ModelResource::Vertex& c = vertices.at(indices.at(index + 2));

				XMVECTOR A = XMLoadFloat3(&a.position);
				XMVECTOR B = XMLoadFloat3(&b.position);
				XMVECTOR C = XMLoadFloat3(&c.position);
 
				//三角形の三辺ベクトルを算出
				XMVECTOR AB = B - A;
				XMVECTOR BC = C - B;
				XMVECTOR CA = A - C;

				//三角形の法線ベクトルを算出
				XMVECTOR N = XMVector3Cross(AB, BC);

				//内積の結果がプラスならば表向き
				XMVECTOR Dot = XMVector3Dot(N, V);
				float dot;
				XMStoreFloat(&dot, Dot);
				if (dot >= 0.0f) continue;	//裏ならコンテニュー

				//レイと平面の交点を算出
				XMVECTOR SA = XMVectorSubtract(A, S);	//恐らく射影
				XMVECTOR X = XMVectorDivide(XMVector3Dot(SA, N), Dot); 
				float x;
				XMStoreFloat(&x, X);
				if (x < .0f || x > neart) continue;		//交点までの距離が今までに計算した最近距離より
														//大きいときはスキップする
				XMVECTOR P = XMVectorAdd(XMVectorMultiply(V, X), S);

				//まだ何か必要ぽい
				//float sa;
				//XMStoreFloat(&sa, SA);
				//
				//XMVECTOR D2 = XMVector3Dot(T, Normal);
				//float d2;
				//XMStoreFloat(&d2, D2);
				//
				//float t;
				//float x = t / d2;


				//交点が三角形の内側にあるか判定
				//1つめ
				XMVECTOR PA = XMVectorSubtract(A, P);
				XMVECTOR Cross1 = XMVector3Cross(PA, AB); //AB ,APベクトル
				XMVECTOR Dot1 = XMVector3Dot(Cross1, N);
				float dot1;
				XMStoreFloat(&dot1, Dot1);
				if (dot1 < 0.0f) continue;	//90度を超えたらコンテニュー

				//2つめ
				XMVECTOR PB = XMVectorSubtract(B, P);
				XMVECTOR Cross2 = XMVector3Cross(PB, BC); //BC , BPベクトル
				XMVECTOR Dot2 = XMVector3Dot(Cross2, N);
				float dot2;
				XMStoreFloat(&dot2, Dot2);
				if (dot2 < 0.0f) continue;

				//3つめ
				XMVECTOR PC = XMVectorSubtract(C, P);
				XMVECTOR Cross3 = XMVector3Cross(PC, CA); 
				XMVECTOR Dot3 = XMVector3Dot(Cross3, N);
				float dot3;
				XMStoreFloat(&dot3, Dot3);
				if (dot3 < 0.0f) continue;


				//最近距離を更新
				neart = x;

				//交点と法線を更新
				HitPosition = P;
				HitNormal = N;
				materialIndex = subset.materialIndex;
			}
		}
		if (materialIndex >= 0)
		{
			//ローカル空間からワールド空間へ変換
			XMVECTOR WorldPosition = XMVector3TransformCoord(HitPosition, WorldTransform);

			XMVECTOR WorldCrossVec = XMVectorSubtract(WorldPosition, WorldStart);
			XMVECTOR WorldCrossLength = XMVector3Length(WorldCrossVec);
			float distance;
			XMStoreFloat(&distance, WorldCrossLength);

			//ヒット情報保存
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

