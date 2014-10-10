#pragma once

#include "d3dUtil.h"
#include "Effect.h"

class SkyBox
{
public:
	SkyBox(ID3D11Device* device, float skySphereRadius,
		const std::wstring& cubmapFilename);
	~SkyBox();

	void Draw(ID3D11DeviceContext* dc, CXMMATRIX vp,
		FXMVECTOR eye);

private:
	ID3D11ShaderResourceView* mCubeMapSRV;
	ID3D11Buffer* mIB, * mVB;
	SkyBoxEffect* mSkyBoxEffect;
	int mIndexCount;
};

