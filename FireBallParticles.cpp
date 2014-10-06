
#include "FireBallParticles.h"
#include "Vertex.h"
#include <algorithm>

FireBallParticles::FireBallParticles()
	: mIsAllParticlesDead(false), mParticlesShown(0)
{
}


FireBallParticles::~FireBallParticles()
{
	if (mFireBallEffect)
		delete mFireBallEffect;

	if (mFireBallParticleVB)
		ReleaseCOM(mFireBallParticleVB);
}

void FireBallParticles::Init(FXMVECTOR initialPos, float fireballRadius, LPCWSTR texFilename, ID3D11Device* device, FireBallParticlesProperties p)
{
	SetProperties(p);

	mFireBallEffect = new ParticleEffect();
	mFireBallEffect->LoadEffect(L"FX/ParticleEffect.fx", device);

	Vertex::InitParticleVertLayout(device, mFireBallEffect->GetTech());

	BuildFireBallParticleVB(device);

	XMVECTOR vel;
	XMFLOAT3 fVel3;
	for (int i = 0; i < mProperties.numParticles; ++i)
	{
		FireBallParticle newParticle;
		vel = XMVector3Normalize(XMVectorSet(MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f), 0.0f)) * fireballRadius;
		XMStoreFloat3(&newParticle.pos, initialPos + vel);
		fVel3 = GetVelocity();
		vel = XMVector3Normalize(XMVectorSet(fVel3.x, fVel3.y, fVel3.z, 0.0f)) * fireballRadius;
		XMStoreFloat3(&newParticle.vel, vel * GetSpeedMultiplier());
		float pSize = GetSize();
		newParticle.size.x = pSize;
		newParticle.size.y = pSize;
		newParticle.age = 0.0f;
		newParticle.lifetime = GetLifetime();
		newParticle.tweenSegment = 0;
		mFireBallParticles.push_back(newParticle);
	}

	mParticlesShown = mProperties.numParticles;

	D3DX11CreateShaderResourceViewFromFile(device, texFilename, 0, 0, &mFBTexture, 0);

	BuildBlendState(device);
	BuildDSState(device);
}

void FireBallParticles::SetPos(XMFLOAT3 newPos)
{
	for (int i = 0; i < mFireBallParticles.size(); ++i)
	{
		mFireBallParticles[i].pos = newPos;
	}
	mParticlesShown = mProperties.numParticles;
}

void FireBallParticles::SetWayPoints(std::vector<PathNode*> wayP)
{ 
	mWayPoints = wayP;

	mTweenPoints.push_back({ wayP[0]->xPos, wayP[0]->zPos, 0, 0, 0, XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f });
	UINT lastAdd = 0;
	XMFLOAT2 lastSame = XMFLOAT2(0.0f, 0.0f);

	for (int i = 1; i < wayP.size(); ++i)
	{
		if (lastSame.x == 0.0f && lastSame.y == 0.0f)
		{
			if (wayP[i]->xPos == mTweenPoints[lastAdd].startPosX)
			{
				lastSame = XMFLOAT2(1.0f, 0.0f);
				continue;
			}
			if (wayP[i]->zPos == mTweenPoints[lastAdd].startPosZ)
			{
				lastSame = XMFLOAT2(0.0f, 1.0f);
				continue;
			}
			//mTweenPoints.push_back({ wayP[i]->xPos, wayP[i]->zPos, 0.0f });
			//++lastAdd;
			//continue;
		}
		if (lastSame.x == 1.0f && lastSame.y == 0.0f)
		{
			if (wayP[i]->xPos == mTweenPoints[lastAdd].startPosX)
			{
				continue;
			}
			lastSame = XMFLOAT2(0.0f, 0.0f);
			mTweenPoints[lastAdd] = { mTweenPoints[lastAdd].startPosX, mTweenPoints[lastAdd].startPosZ, wayP[i - 1]->xPos, wayP[i - 1]->zPos, 
				abs(mTweenPoints[lastAdd].startPosZ - wayP[i - 1]->zPos), XMFLOAT3(0.0f, 0.0f, wayP[i - 1]->zPos > mTweenPoints[lastAdd].startPosZ ? 1.0f : -1.0f), 0.0f };
			mTweenPoints.push_back({ wayP[i - 1]->xPos, wayP[i - 1]->zPos, 0, 0, 0, XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f });
			++lastAdd;
			--i;
			continue;
		}
		if (lastSame.x == 0.0f && lastSame.y == 1.0f)
		{
			if (wayP[i]->zPos == mTweenPoints[lastAdd].startPosZ)
			{
				continue;
			}
			lastSame = XMFLOAT2(0.0f, 0.0f);
			mTweenPoints[lastAdd] = { mTweenPoints[lastAdd].startPosX, mTweenPoints[lastAdd].startPosZ, wayP[i - 1]->xPos, wayP[i - 1]->zPos, 
				abs(mTweenPoints[lastAdd].startPosX - wayP[i - 1]->xPos), XMFLOAT3(wayP[i - 1]->xPos > mTweenPoints[lastAdd].startPosX ? 1.0f : -1.0f, 0.0f, 0.0f), 0.0f };
			mTweenPoints.push_back({ wayP[i - 1]->xPos, wayP[i - 1]->zPos, 0, 0, 0, XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f });
			++lastAdd;
			--i;
			continue;
		}
	}
	if (lastSame.x == 1.0f && lastSame.y == 0.0f)
	{
		mTweenPoints[lastAdd] = { mTweenPoints[lastAdd].startPosX, mTweenPoints[lastAdd].startPosZ, wayP[wayP.size() - 1]->xPos, wayP[wayP.size() - 1]->zPos, 
			abs(mTweenPoints[lastAdd].startPosZ - wayP[wayP.size() - 1]->zPos), XMFLOAT3(0.0f, 0.0f, wayP[wayP.size() - 1]->zPos > mTweenPoints[lastAdd].startPosZ ? 1.0f : -1.0f), 0.0f };
	}
	else
	{
		mTweenPoints[lastAdd] = { mTweenPoints[lastAdd].startPosX, mTweenPoints[lastAdd].startPosZ, wayP[wayP.size() - 1]->xPos, wayP[wayP.size() - 1]->zPos,
			abs(mTweenPoints[lastAdd].startPosX - wayP[wayP.size() - 1]->xPos), XMFLOAT3(wayP[wayP.size() - 1]->xPos > mTweenPoints[lastAdd].startPosX ? 1.0f : -1.0f, 0.0f, 0.0f), 0.0f };
	}

	UINT totalDistance = 0;
	for (int i = 0; i < mTweenPoints.size(); ++i)
	{
		totalDistance += mTweenPoints[i].distance;
	}
	mTweenPoints[0].startTween = 0.0f;
	mTweenPoints[0].tweenTime = (float)mTweenPoints[0].distance / totalDistance;
	mTweenPoints[0].endTween = mTweenPoints[0].tweenTime;
	for (int i = 1; i < mTweenPoints.size(); ++i)
	{
		mTweenPoints[i].startTween = mTweenPoints[i - 1].endTween;
		mTweenPoints[i].tweenTime = (float)mTweenPoints[i].distance / totalDistance;
		mTweenPoints[i].endTween = mTweenPoints[i].startTween + mTweenPoints[i].tweenTime;
	}
}


void FireBallParticles::ResetParticles()
{
	for (int i = 0; i < mFireBallParticles.size(); ++i)
	{
		mFireBallParticles[i].age = 0.0f;
	}
}

void FireBallParticles::FireEffect()
{ 
	mProperties.isFire = true;
	mIsAllParticlesDead = false;
}

void FireBallParticles::Update(FXMVECTOR newPos, float fireballRadius, float dt, ID3D11DeviceContext* context)
{
	if (!mProperties.isFire || mIsAllParticlesDead)
	{
		return;
	}

	XMFLOAT3 fVel3;
	float pSize;
	bool isParticleStillAlive = false;
	for (int i = 0; i < mFireBallParticles.size(); ++i)
	{
		XMVECTOR pos = XMLoadFloat3(&mFireBallParticles[i].pos);
		XMVECTOR vel = XMLoadFloat3(&mFireBallParticles[i].vel);

		mFireBallParticles[i].age += dt;
		if (mProperties.isOneShot && (mFireBallParticles[i].age >= mFireBallParticles[i].lifetime))
		{
			continue;
		}
		else if (mFireBallParticles[i].age <= mFireBallParticles[i].lifetime)
		{
			isParticleStillAlive = true;
		}
		else if (!mProperties.isOneShot && (mFireBallParticles[i].age >= mFireBallParticles[i].lifetime))
		{
			isParticleStillAlive = true;
			vel = XMVector3Normalize(XMVectorSet(MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f), 0.0f)) * fireballRadius;
			XMStoreFloat3(&mFireBallParticles[i].pos, newPos + vel);
			fVel3 = GetVelocity();
			vel = XMVector3Normalize(XMVectorSet(fVel3.x, fVel3.y, fVel3.z, 0.0f));
			float speedMult = GetSpeedMultiplier();
			if (MathHelper::RandF() > 0.50f)
			{
				if (MathHelper::RandF() > 0.50f)
				{
					vel.m128_f32[0] += MathHelper::RandF(-0.4f, 0.4f);
					//vel.m128_f32[0] *= MathHelper::RandF(-0.06f, 0.06f);
					//vel.m128_f32[2] *= MathHelper::RandF(-0.06f, 0.06f);
				}
				else
				{
					vel.m128_f32[2] += MathHelper::RandF(-0.4f, 0.4f);
					//vel.m128_f32[0] *= MathHelper::RandF(-0.06f, 0.06f);
					//vel.m128_f32[2] *= MathHelper::RandF(-0.06f, 0.06f);
				}
			}
			XMStoreFloat3(&mFireBallParticles[i].vel, vel * speedMult);
			pSize = GetSize();
			mFireBallParticles[i].size.x = pSize;
			mFireBallParticles[i].size.y = pSize;
			mFireBallParticles[i].age = 0.0f;
			mFireBallParticles[i].lifetime = GetLifetime();
			pos = XMLoadFloat3(&mFireBallParticles[i].pos);
			vel = XMLoadFloat3(&mFireBallParticles[i].vel);
		}
		if (pos.m128_f32[1] < (newPos.m128_f32[1] + (fireballRadius * 0.60f)))
		{
			XMVECTOR s1Center = newPos;
			float s1Radius = fireballRadius;
			float currOverLap = 0.0f;
			XMVECTOR correction = XMVectorZero();

			XMVECTOR d = s1Center - pos;

			float distance = sqrt((d.m128_f32[0] * d.m128_f32[0]) /*+ (d.m128_f32[1] * d.m128_f32[1])*/ + (d.m128_f32[2] * d.m128_f32[2])); //Magnitude of the difference

			float overLap = s1Radius - distance;

			if (overLap > currOverLap) // Have Collision
			{
				currOverLap = overLap;

				correction = XMVector3Normalize(d) * currOverLap; //correct collision by moving sphere out of box
			}
			pos += correction;
		}
		pos = pos + vel;
		XMStoreFloat3(&mFireBallParticles[i].pos, pos);
	}

	if (isParticleStillAlive)
	{
		UpdateFireBallParticleVB(context);
	}
	else
	{
		mIsAllParticlesDead = true;
		if (mProperties.isOneShot)
		{
			ResetParticles();
		}
	}
}

void FireBallParticles::Update(FXMVECTOR newPos, float fireballRadius, float dt, float ratio, ID3D11DeviceContext* context)
{
	if (!mProperties.isFire || mIsAllParticlesDead)
	{
		return;
	}

	XMFLOAT3 fVel3;
	float pSize;
	bool isParticleStillAlive = false;

	/*XMFLOAT3 nPos;
	XMVECTOR oPos = XMLoadFloat3(&mOriginalPos);
	XMVECTOR dPos = newPos - oPos;
	float distance = sqrt((dPos.m128_f32[0] * dPos.m128_f32[0]) + (dPos.m128_f32[2] * dPos.m128_f32[2]));
	float radius = distance * 0.5f;
	XMVECTOR ndPos = XMVector3Normalize(dPos);
	XMVECTOR centrePoint = ndPos * radius;*/

	struct find_tween : std::unary_function<TweenPoint, bool>
	{
		float tweenVal;
		find_tween(float tween) :tweenVal(tween) { }
		bool operator()(TweenPoint const& t) const
		{
			return ((tweenVal >= t.startTween) && (tweenVal <= t.endTween));
		}
	};
	/*struct find_pos : std::unary_function<TweenPoint, bool>
	{
		XMFLOAT3 posVal;
		find_pos(XMFLOAT3 pos) :posVal(pos) { }
		bool operator()(TweenPoint const& t) const
		{
			return ((tweenVal >= t.startTween) && (tweenVal <= t.endTween));
		}
	};*/

	/*std::vector<TweenPoint>::iterator it = std::find_if(mTweenPoints.begin(), mTweenPoints.end(), find_tween(ratio));
	float distRatio = ratio / it->endTween;
	float distance = it->distance * distRatio;
	XMVECTOR newPos = XMLoadFloat3(&(it->vector)) * distance;*/
	//UINT index = ratio * (mWayPoints.size() - 1);

	for (int i = 0; i < mFireBallParticles.size(); ++i)
	{
		//XMVECTOR pos = XMLoadFloat3(&mFireBallParticles[i].pos);
		//XMVECTOR vel = XMLoadFloat3(&mFireBallParticles[i].vel);
		//pos = pos + vel;
		//XMStoreFloat3(&mFireBallParticles[i].pos, pos);

		//mFireBallParticles[i].pos.x = centrePoint.m128_f32[0] + (radius * cos(ratio * XM_PI));
		//mFireBallParticles[i].pos.y = centrePoint.m128_f32[1] + (radius * sin(ratio * XM_PI)) + 4.0f;

		//mFireBallParticles[i].pos.x = mWayPoints[index]->xPos;
		//mFireBallParticles[i].pos.z = mWayPoints[index]->zPos;

		XMVECTOR pos = XMLoadFloat3(&mFireBallParticles[i].pos);
		UINT tIndex = mFireBallParticles[i].tweenSegment;
		XMFLOAT3 vector = mTweenPoints[tIndex].vector;
		XMVECTOR vel = XMLoadFloat3(&vector);
		pos = pos + (vel * dt * 5);
		XMStoreFloat3(&mFireBallParticles[i].pos, pos);
		if (abs(vector.x) == 1.0f && vector.z == 0.0f)
		{
			if (mTweenPoints[tIndex].endPosX > mTweenPoints[tIndex].startPosX)
			{
				if (mFireBallParticles[i].pos.x >= mTweenPoints[tIndex].endPosX)
				{
					mFireBallParticles[i].tweenSegment++;
				}
			}
			else
			{
				if (mFireBallParticles[i].pos.x <= mTweenPoints[tIndex].endPosX)
				{
					mFireBallParticles[i].tweenSegment++;
				}
			}
		}
		else  // vector.x == 0.0f && vector.z == 1.0f;
		{
			if (mTweenPoints[tIndex].endPosZ > mTweenPoints[tIndex].startPosZ)
			{
				if (mFireBallParticles[i].pos.z >= mTweenPoints[tIndex].endPosZ)
				{
					mFireBallParticles[i].tweenSegment++;
				}
			}
			else
			{
				if (mFireBallParticles[i].pos.z <= mTweenPoints[tIndex].endPosZ)
				{
					mFireBallParticles[i].tweenSegment++;
				}
			}
		}
		if (mFireBallParticles[i].tweenSegment >= mTweenPoints.size())
		{
			mFireBallParticles[i].tweenSegment = 0;
		}
	}

	UpdateFireBallParticleVB(context);
}

XMFLOAT3 FireBallParticles::GetVelocity()
{
	return XMFLOAT3(mProperties.velocityAddition.x + (mProperties.velX.isRandomRange ? MathHelper::RandF(mProperties.velX.range.x, mProperties.velX.range.y) : mProperties.velX.range.x),
		mProperties.velocityAddition.y + (mProperties.velY.isRandomRange ? MathHelper::RandF(mProperties.velY.range.x, mProperties.velY.range.y) : mProperties.velY.range.x),
		mProperties.velocityAddition.z + (mProperties.velZ.isRandomRange ? MathHelper::RandF(mProperties.velZ.range.x, mProperties.velZ.range.y) : mProperties.velZ.range.x));
}

float FireBallParticles::GetSpeedMultiplier()
{
	return (mProperties.speedMult.isRandomRange ? MathHelper::RandF(mProperties.speedMult.range.x, mProperties.speedMult.range.y) : mProperties.speedMult.range.x);
}

float FireBallParticles::GetSize()
{
	return (mProperties.size.isRandomRange ? MathHelper::RandF(mProperties.size.range.x, mProperties.size.range.y) : mProperties.size.range.x);
}

float FireBallParticles::GetLifetime()
{
	return (mProperties.lifetime.isRandomRange ? MathHelper::RandF(mProperties.lifetime.range.x, mProperties.lifetime.range.y) : mProperties.lifetime.range.x);
}

void FireBallParticles::BuildFireBallParticleVB(ID3D11Device* device)
{
	std::vector<Vertex::ParticleVertex> vertices(MAX_PARTICLES);

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(Vertex::ParticleVertex) * MAX_PARTICLES;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(device->CreateBuffer(&vbd, &vinitData, &mFireBallParticleVB));

}

void FireBallParticles::UpdateFireBallParticleVB(ID3D11DeviceContext* context)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(context->Map(mFireBallParticleVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	Vertex::ParticleVertex* v = reinterpret_cast<Vertex::ParticleVertex*> (mappedData.pData);

	mParticlesShown = 0;
	for (UINT i = 0; i < mFireBallParticles.size(); ++i)
	{
		if (mFireBallParticles[i].age <= mFireBallParticles[i].lifetime)
		{
			v[mParticlesShown].pos = mFireBallParticles[i].pos;
			v[mParticlesShown++].size = mFireBallParticles[i].size;
		}
	}

	context->Unmap(mFireBallParticleVB, 0);
}

void FireBallParticles::DrawFireBall(FXMVECTOR eyePos, CXMMATRIX viewProj, ID3D11DeviceContext* context)
{
	if (mIsAllParticlesDead)
	{
		return;
	}

	mFireBallEffect->SetPerFrameParams(eyePos);
	mFireBallEffect->SetPerObjectParams(viewProj, mFBTexture);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	context->IASetInputLayout(Vertex::GetParticleVertLayout());

	UINT stride = sizeof(Vertex::ParticleVertex);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, &mFireBallParticleVB, &stride, &offset);

	context->OMSetDepthStencilState(mNoDepthDS, 0);
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	context->OMSetBlendState(mAdditiveBS, blendFactor, 0xffffffff);

	D3DX11_TECHNIQUE_DESC techDesc;
	mFireBallEffect->GetTech()->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		mFireBallEffect->GetTech()->GetPassByIndex(p)->Apply(0, context);
		context->Draw(mParticlesShown, 0);
	}

	context->OMSetBlendState(0, blendFactor, 0xffffffff);
	context->OMSetDepthStencilState(NULL, 0);
}

void FireBallParticles::BuildBlendState(ID3D11Device* device)
{
	D3D11_BLEND_DESC bsDesc = { 0 };
	bsDesc.AlphaToCoverageEnable = false;
	bsDesc.IndependentBlendEnable = false;

	bsDesc.RenderTarget[0].BlendEnable = true;
	bsDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	bsDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	bsDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bsDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bsDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bsDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bsDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HR(device->CreateBlendState(&bsDesc, &mAdditiveBS));
}

void FireBallParticles::BuildDSState(ID3D11Device* device)
{
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	dsDesc.StencilEnable = false;
	dsDesc.StencilReadMask = 0xff;
	dsDesc.StencilWriteMask = 0xff;

	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// We are not rendering backfacing polygons, so these settings do not matter.
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	HR(device->CreateDepthStencilState(&dsDesc, &mNoDepthDS));
}

