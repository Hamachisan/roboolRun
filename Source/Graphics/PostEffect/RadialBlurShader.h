#pragma once

#include <memory>
#include <wrl.h>
#include "Graphics/Shader.h"

class RadialBlurShader : public SpriteShader
{
public:
	RadialBlurShader(ID3D11Device* device);
	~RadialBlurShader() override {}

	void Begin(const RenderContext& rc) override;
	void Draw(const RenderContext& rc, const Sprite* sprite) override;
	void End(const RenderContext& rc) override;

private:
	struct CbRadiialBlur
	{
		float	sampleCount;		// ��
		float	strength;			//�u���[�̋��� 
		DirectX::XMFLOAT2 center;	//���S���W
		
	};


	Microsoft::WRL::ComPtr<ID3D11Buffer>				radialBlurShaderConstantBuffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>			vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			inputLayout;

	Microsoft::WRL::ComPtr<ID3D11BlendState>			blendState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		depthStencilState;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>			samplerState;
};