#pragma once

#include "d3dUtil.h"
#include "Effect.h"
#include <vector>
#include "PathNode.h"

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
		UINT tweenSegment;
	};
	struct FireBallRangeElement
	{
		bool isRandomRange;
		XMFLOAT2 range;
	};
	struct FireBallParticlesProperties
	{
		UINT numParticles;
		FireBallRangeElement velX;
		FireBallRangeElement velY;
		FireBallRangeElement velZ;
		XMFLOAT3 velocityAddition;
		FireBallRangeElement speedMult;
		FireBallRangeElement size;
		FireBallRangeElement lifetime;
		bool isOneShot;
		bool isFire;
	};

	struct TweenPoint
	{
		int startPosX;
		int startPosZ;
		int endPosX;
		int endPosZ;
		UINT distance;
		XMFLOAT3 vector;
		float startTween;
		float endTween;
		float tweenTime;
	};


	const int MAX_PARTICLES = 100000;

	FireBallParticles();
	~FireBallParticles();

	void Init(FXMVECTOR initialPos, float fireballRadius, LPCWSTR texFilename, ID3D11Device* device, FireBallParticlesProperties p);
	void Update(FXMVECTOR newPos, float fireballRadius, float dt, ID3D11DeviceContext* context);
	void Update(FXMVECTOR newPos, float fireballRadius, float dt, float ratio, ID3D11DeviceContext* context);
	void DrawFireBall(FXMVECTOR eyePos, CXMMATRIX viewProj, ID3D11DeviceContext* context);
	void SetPos(XMFLOAT3 newPos);
	void FireEffect();

	void SetFireBallTexture(ID3D11ShaderResourceView* fbTex){ mFBTexture = fbTex; }
	void SetProperties(FireBallParticlesProperties p){ mProperties = p; }

	void SetWayPoints(std::vector<PathNode*> wayP);

	void SetOriginalPos(FXMVECTOR oPos){ XMStoreFloat3(&mOriginalPos, oPos); }

private:
	void BuildBlendState(ID3D11Device* device);
	void BuildDSState(ID3D11Device* device);
	void BuildFireBallParticleVB(ID3D11Device* device);
	void UpdateFireBallParticleVB(ID3D11DeviceContext* context);
	void ResetParticles();
	XMFLOAT3 GetVelocity();
	float GetSpeedMultiplier();
	float GetSize();
	float GetLifetime();

	ParticleEffect* mFireBallEffect;
	std::vector<FireBallParticle> mFireBallParticles;
	ID3D11Buffer* mFireBallParticleVB;
	ID3D11ShaderResourceView* mFBTexture;
	ID3D11BlendState* mAdditiveBS;
	ID3D11DepthStencilState* mNoDepthDS;

	FireBallParticlesProperties mProperties;
	bool mIsAllParticlesDead;
	UINT mParticlesShown;

	XMFLOAT3 mOriginalPos;
	std::vector<PathNode*> mWayPoints;
	std::vector<TweenPoint> mTweenPoints;
};

