#pragma once

#include "d3dUtil.h"


class Effect
{
protected:
	ID3DX11Effect* mEffect;
	ID3DX11EffectTechnique* mTech;
	ID3DX11EffectMatrixVariable* mfxWVP;

public:
	Effect(void);
	virtual ~Effect(void);

	void LoadEffect(std::wstring filename, ID3D11Device* device);
	virtual void Draw(ID3D11DeviceContext* context, ID3D11Buffer* vb, ID3D11Buffer* ib, int indexCount);

	virtual void Draw(ID3D11DeviceContext* context, ID3D11Buffer* vb, ID3D11Buffer* ib, int startIndex, int indexCount);

	virtual void Draw(ID3D11DeviceContext* context, ID3D11Buffer* vb, ID3D11Buffer* ib, int startIndex, int indexCount, int baseVertexLoc);

	virtual void DrawInstanced(ID3D11DeviceContext* context, ID3D11Buffer* vb, ID3D11Buffer* ib, ID3D11Buffer* instanceb, int instanceCount, int startIndex, int indexCount);

	ID3DX11EffectMatrixVariable* GetWVP() const
	{
		return mfxWVP;
	}


	ID3DX11EffectTechnique* GetTech() const
	{
		return mTech;
	}
	

protected:

	virtual void LoadEffectParams();
	virtual void SetPerObjectParams(CXMMATRIX wvp);
};

class SkyBoxEffect : public Effect
{
protected:
	ID3DX11EffectShaderResourceVariable* mfxCubeMap;

public:
	virtual void Draw(ID3D11DeviceContext* context, ID3D11Buffer* vb, ID3D11Buffer* ib, int indexCount);

	virtual void SetPerObjectParams(CXMMATRIX wvp, ID3D11ShaderResourceView* skybox);
	virtual void LoadEffectParams();
};


class LitTexEffect : public Effect
{
protected:
	ID3D11InputLayout* mInputLayout;

	ID3DX11EffectMatrixVariable* mfxWorld;
	ID3DX11EffectMatrixVariable* mfxInvTranspose;
	ID3DX11EffectVectorVariable* mfxAmbientLight;
	ID3DX11EffectVectorVariable* mfxEyePos;
	ID3DX11EffectVariable* mfxPointLight;
	ID3DX11EffectVariable* mfxSpotLight;
	ID3DX11EffectShaderResourceVariable* mfxDiffuseMap;

public:
	virtual ~LitTexEffect();

	virtual void SetPerFrameParams(FXMVECTOR ambient, FXMVECTOR eyePos,
								   const PointLightOptimized& point,
							       const SpotLightOptimized& spot);

	virtual void SetPerObjectParams(CXMMATRIX world, CXMMATRIX invTranspose, CXMMATRIX wvp, ID3D11ShaderResourceView* texture);

	virtual void SetDiffuseMap(ID3D11ShaderResourceView* texture);

protected:
	
	virtual void LoadEffectParams();

};

class LitMatEffect : public Effect
{
protected:
	ID3D11InputLayout* mInputLayout;

	ID3DX11EffectMatrixVariable* mfxWorld;
	ID3DX11EffectMatrixVariable* mfxViewProj;
	ID3DX11EffectMatrixVariable* mfxInvTranspose;
	ID3DX11EffectVectorVariable* mfxAmbientLight;
	ID3DX11EffectVectorVariable* mfxEyePos;
	ID3DX11EffectVariable* mfxPointLight;
	ID3DX11EffectVariable* mfxSpotLight;
	//ID3DX11EffectShaderResourceVariable* mfxDiffuseMap;
	ID3DX11EffectVariable* mfxMaterial;

public:
	virtual ~LitMatEffect();

	virtual void SetPerFrameParams(FXMVECTOR ambient, FXMVECTOR eyePos, const PointLightOptimized* point, const SpotLightOptimized* spot);

	virtual void SetPerObjectParams(CXMMATRIX world, CXMMATRIX invTranspose, CXMMATRIX wvp, CXMMATRIX viewProj, Material &mat);

	virtual void SetEffectTech(LPCSTR effectName);

	//virtual void SetDiffuseMap(ID3D11ShaderResourceView* texture);

protected:

	virtual void LoadEffectParams();

};


class TerrainEffect : public LitTexEffect
{
protected:
	ID3DX11EffectShaderResourceVariable** mfxDiffuseMaps;
	ID3DX11EffectShaderResourceVariable* mfxBlendMap;


	virtual void LoadEffectParams();

public:
	virtual ~TerrainEffect();

	virtual void SetPerObjectParams(CXMMATRIX world, CXMMATRIX invTranspose, 
									CXMMATRIX wvp, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* texture2,
									ID3D11ShaderResourceView* texture3, ID3D11ShaderResourceView* texture4,
									ID3D11ShaderResourceView* blendMap);

	virtual void Draw(ID3D11DeviceContext* context, ID3D11Buffer* vb,
					  ID3D11Buffer* ib, int indexCount);

};


class ParticleEffect : public Effect
{
protected:
	ID3DX11EffectVectorVariable* mfxEyePos;
	ID3DX11EffectShaderResourceVariable* mfxDiffuseMap;

public:
	virtual ~ParticleEffect(){}

	virtual void SetPerFrameParams(FXMVECTOR eyePos);

	virtual void SetPerObjectParams(CXMMATRIX wvp, ID3D11ShaderResourceView* texture);

	virtual void SetDiffuseMap(ID3D11ShaderResourceView* texture);

protected:

	virtual void LoadEffectParams();

};

class BlurEffect : public Effect
{
protected:
	ID3DX11EffectVectorVariable* mfxEyePos;
	ID3DX11EffectShaderResourceVariable* mfxDiffuseMap;

	ID3DX11EffectScalarVariable* mfxWeights;
	ID3DX11EffectShaderResourceVariable* mfxInputMap;
	ID3DX11EffectUnorderedAccessViewVariable* mfxOutputMap;

public:
	virtual ~BlurEffect(){}

	//virtual void SetPerFrameParams(FXMVECTOR eyePos);

	//virtual void SetPerObjectParams(CXMMATRIX wvp, ID3D11ShaderResourceView* texture);

	//virtual void SetDiffuseMap(ID3D11ShaderResourceView* texture);

	virtual void SetWeights(const float weights[9])           { mfxWeights->SetFloatArray(weights, 0, 9); }
	virtual void SetInputMap(ID3D11ShaderResourceView* tex)   { mfxInputMap->SetResource(tex); }
	virtual void SetOutputMap(ID3D11UnorderedAccessView* tex) { mfxOutputMap->SetUnorderedAccessView(tex); }

	ID3DX11EffectTechnique* HorzBlurTech;
	ID3DX11EffectTechnique* VertBlurTech;

protected:

	virtual void LoadEffectParams();

};

