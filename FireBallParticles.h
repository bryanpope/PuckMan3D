#pragma once

#include "d3dUtil.h"
#include "Effect.h"
#include <vector>

class FireBallParticles
{
public:
	struct FireBallParticle
	{
		XMFLOAT3 pos;
		XMFLOAT3 vel;
		XMFLOAT2 size;
		float age;
		float lifetime;
	};

	const int MAX_PARTICLES = 100000;

	FireBallParticles();
	~FireBallParticles();

	void Init(FXMVECTOR initialPos, LPCWSTR texFilename, ID3D11Device* device);
	void Update(FXMVECTOR newPos, float fireballRadius, float dt, ID3D11DeviceContext* context);
	void SetFireBallTexture(ID3D11ShaderResourceView* fbTex){ mFBTexture = fbTex; }
	void DrawFireBall(FXMVECTOR eyePos, CXMMATRIX viewProj, ID3D11DeviceContext* context);

private:
	void BuildBlendState(ID3D11Device* device);
	void BuildDSState(ID3D11Device* device);
	void BuildFireBallParticleVB(ID3D11Device* device);
	void UpdateFireBallParticleVB(ID3D11DeviceContext* context);

	ParticleEffect* mFireBallEffect;
	std::vector<FireBallParticle> mFireBallParticles;
	ID3D11Buffer* mFireBallParticleVB;
	ID3D11ShaderResourceView* mFBTexture;
	ID3D11BlendState* mAdditiveBS;
	ID3D11DepthStencilState* mNoDepthDS;
};

