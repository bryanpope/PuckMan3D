
#include "FireBallParticles.h"
#include "Vertex.h"

FireBallParticles::FireBallParticles()
{
}


FireBallParticles::~FireBallParticles()
{
	if (mFireBallEffect)
		delete mFireBallEffect;

	if (mFireBallParticleVB)
		ReleaseCOM(mFireBallParticleVB);
}

void FireBallParticles::Init(FXMVECTOR initialPos, LPCWSTR texFilename, ID3D11Device* device)
{
	mFireBallEffect = new ParticleEffect();
	mFireBallEffect->LoadEffect(L"FX/ParticleEffect.fx", device);

	Vertex::InitParticleVertLayout(device, mFireBallEffect->GetTech());

	BuildFireBallParticleVB(device);

	XMVECTOR vel;
	for (int i = 0; i < 10000; ++i)
	{
		FireBallParticle newParticle;
		XMStoreFloat3(&newParticle.pos, initialPos);
		vel = XMVector3Normalize(XMVectorSet(MathHelper::RandF(-0.5f, 0.5f), 0.5 + MathHelper::RandF(0.0f, 0.2f), MathHelper::RandF(-0.5f, 0.5f), 0.0f));
		XMStoreFloat3(&newParticle.vel, vel * MathHelper::RandF(0.05f, 0.2f));
		newParticle.size.x = 0.1f;
		newParticle.size.y = 0.1f;
		newParticle.age = 0.0f;
		newParticle.lifetime = MathHelper::RandF(0.5f, 1.0f);
		mFireBallParticles.push_back(newParticle);
	}

	D3DX11CreateShaderResourceViewFromFile(device, texFilename, 0, 0, &mFBTexture, 0);

	BuildBlendState(device);
	BuildDSState(device);
}

void FireBallParticles::Update(FXMVECTOR newPos, float fireballRadius, float dt, ID3D11DeviceContext* context)
{
	for (int i = 0; i < mFireBallParticles.size(); ++i)
	{
		XMVECTOR pos = XMLoadFloat3(&mFireBallParticles[i].pos);
		XMVECTOR vel = XMLoadFloat3(&mFireBallParticles[i].vel);

		mFireBallParticles[i].age += dt;
		if (mFireBallParticles[i].age >= mFireBallParticles[i].lifetime)
		{
			vel = XMVector3Normalize(XMVectorSet(MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f), 0.0f)) * fireballRadius;
			XMStoreFloat3(&mFireBallParticles[i].pos, newPos + vel);
			//mFireBallParticles[i].pos.y -= 1.25f;
			vel = XMVector3Normalize(XMVectorSet(MathHelper::RandF(-0.03f, 0.03f), 0.5 + MathHelper::RandF(0.0f, 0.2f), MathHelper::RandF(-0.03f, 0.03f), 0.0f));
			float speedMult = MathHelper::RandF(0.01f, 0.06f);
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
			mFireBallParticles[i].size.x = MathHelper::RandF(0.05f, 0.1f);
			mFireBallParticles[i].size.y = MathHelper::RandF(0.05f, 0.1f);
			mFireBallParticles[i].age = 0.0f;
			mFireBallParticles[i].lifetime = MathHelper::RandF(0.25f, 2.25f);
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

	UpdateFireBallParticleVB(context);
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

	for (UINT i = 0; i < mFireBallParticles.size(); ++i)
	{
		v[i].pos = mFireBallParticles[i].pos;
		v[i].size = mFireBallParticles[i].size;
	}

	context->Unmap(mFireBallParticleVB, 0);
}

void FireBallParticles::DrawFireBall(FXMVECTOR eyePos, CXMMATRIX viewProj, ID3D11DeviceContext* context)
{
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
		context->Draw(mFireBallParticles.size(), 0);
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

