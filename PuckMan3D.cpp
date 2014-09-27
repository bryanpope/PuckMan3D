//***************************************************************************************
// BoxDemo.cpp by Frank Luna (C) 2011 All Rights Reserved.
//
// Demonstrates rendering a colored box.
//
// Controls:
//		Hold the left mouse button down and move the mouse to rotate.
//      Hold the right mouse butt on down to zoom in and out.
//
//***************************************************************************************

#include "d3dApp.h"
#include "d3dx11Effect.h"
#include "MathHelper.h"
#include "LightHelper.h"
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "randgen.h"
#include "Vertex.h"
#include "Quad.h"
#include "Effects.h"
#include "ThirdPersonCam.h"
#include "Cube.h"
#include "GraphicalObject.h"
#include "Projectile.h"
#include "Effect.h"
#include "fmod.hpp"
#include "fmod_errors.h"
#include "FontRasterizer.h"
#include "Terrain.h"
#include "Player.h"
#include "SkyBox.h"
#include "GeometryGenerator.h"
#include "Blinky.h"
#include "Inky.h"
#include "Pinky.h"
#include "Clyde.h"
#include "Materials.h"

//#include "BasicMeshGeometry.h"
#include "MazeLoader.h"

#include "xnacollision.h"


struct TestParticle
{
	XMFLOAT3 pos;
	XMFLOAT3 vel;
	XMFLOAT2 size;
};

const int MAX_PARTICLES = 100000;

class PuckMan3D : public D3DApp
{
public:
	PuckMan3D(HINSTANCE hInstance);
	~PuckMan3D();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene(); 

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

private:
	const float PUCKMAN_SPEED = 1000.0f;
	void BuildTestPyramid();
	void BuildVertexLayout();
	void BuildSceneLights();
	void BuildParticleVB();
	void BuildBlendStates();
	void BuildDSStates();
	void resetGame();
	void loadSystem();
	void loadGhostDeathSFX();
	void playGhostDeathSFX();
	void loadScaredGhostSFX();
	void playScaredGhostSFX();
	void loadDeathSFX();
	void playDeathSFX();
	void loadFruitSFX();
	void playFruitSFX();
	void loadBeginningSFX();
	void playBeginningSFX();
	void loadExtraLifeSFX();
	void playExtraLifeSFX();
	void loadSirenSFX();
	void playSirenSFX();
	void loadWaSFX();
	void playWaSFX();
	void loadKaSFX();
	void playKaSFX();
	void updateGhosts(float dt);
	void UpdateParticleVB();
	bool UpdateGroundCollision();
	void UpdateKeyboardInput(float dt);
	void UpdateCollision();
	void updateStringStream();
	XMVECTOR PacManAABoxOverLap(XMVECTOR s1Center);

	bool PuckMan3D::PacManGhostOverlapTest(XMVECTOR s1Center, XMVECTOR s2Center);
	bool PuckMan3D::PacManPelletOverlapTest(XMVECTOR s1Center, XMVECTOR s2Center);
	bool PuckMan3D::PacManPowerUpOverlapTest(XMVECTOR s1Center, XMVECTOR s2Center);

	void PuckManSpeed();

	XMVECTOR CylToCyl(FXMVECTOR c1Pos, float c1Rad, float c1Height,
		FXMVECTOR c2Pos, float c2Rad, float c2Height);

	void DrawParticles();

	XNA::OrientedBox* GetOrientedBox(FXMVECTOR extents, const GraphicalObject* obj);
	XNA::Sphere* GetBoundingSphere(const GraphicalObject* obj, float radius);

	void BuildPuckMan();
	void BuildGhosts();
	void SetMaterials();
	void BuildShapeGeometryBuffers();
	void ResetPuckMan();
	void ResetGhosts();

private:
	struct Fruit
	{
		XMFLOAT3 pos;
		XMFLOAT3 vel;

		Fruit(FXMVECTOR pos, FXMVECTOR vel)
		{
			XMStoreFloat3(&this->pos, pos);
			XMStoreFloat3(&this->vel, vel);
		}

	};

	ID3D11Buffer* mShapesVB;
	ID3D11Buffer* mShapesIB;

	//LitMatEffect* mLitMatEffect;
	LitMatEffect* mLitMatInstanceEffect;
	LitTexEffect* mLitTexEffect;
	ParticleEffect* mParticleEffect;

	ThirdPersonCam* mCam;
	BaseCamera* m2DCam;

	FontRasterizer* mFont;

	XMFLOAT4 mAmbientColour;
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;
	XMFLOAT4X4 m2DProj;

	PointLightOptimized mPointLights[3];
	SpotLightOptimized mSpotLight;

	Player* mTestPlayer;
	Terrain* mTestTerrain;
	SkyBox* mSkyBox;
	BasicModel* mBarnProjectile;
	BasicModel* mFarmModel;
	BasicModel* mMonsterModel;

	BasicModel* mMazeModel;
	Character* mMazeCharacter;
	BasicModel* mMazeModelInstanced;
	Character* mMazeCharacterInstanced;

	std::vector<Character*> mTestChars;

	std::vector<Projectile*> mProjectiles;

	std::vector<TestParticle> mParticles;

	ID3D11Buffer* mParticleVB;
	ID3D11ShaderResourceView* mParticleTexture;

	ID3D11BlendState* mAdditiveBS;
	ID3D11BlendState* mTransparentBS;
	ID3D11DepthStencilState* mNoDepthDS;
	ID3D11DepthStencilState* mFontDS;

	bool mMouseReleased;
	POINT mLastMousePos;
	float mSlope;

	XMFLOAT3 mEyePosW;
	bool mIsKeyPressed;
	bool mForward;
	bool mBackward;
	bool mLeft;
	bool mRight;
	bool powerUpActivated = false;
	bool mIsBlue = false;
	bool mIsMoving = false;
	bool mIsPlayerDead = false;
	bool mIsBeginningPlaying = false;
	bool mCanMove = true;
	bool mIsPaused = false;
	float mSpeed;
	float fruitR = 0.60;
	float mNextTime = 0.0f;
	float mCurrentTime = 0.0f;
	float mTotalTime = 0.0f;
	float mPauseTime = 0.0f;
	float mTotalDeathTime = 0.0f;
	int mLevelCounter;
	int mPelletCounter = 0;

	int mBoxVertexOffset;
	int mGridVertexOffset;
	int mPelletVertexOffset;
	int mPacManVertexOffset;
	int mPowerUpVertexOffset;
	int mGhostVertexOffset;
	//int mFruitVertexOffset;

	UINT mBoxIndexOffset;
	UINT mGridIndexOffset;
	UINT mPelletIndexOffset;
	UINT mPacManIndexOffset;
	UINT mPowerUpIndexOffset;
	UINT mGhostIndexOffset;
	//UINT mFruitIndexOffset;

	UINT mBoxIndexCount;
	UINT mGridIndexCount;
	UINT mPelletIndexCount;
	UINT mPacManIndexCount;
	UINT mPowerUpIndexCount;
	UINT mGhostIndexCount;


	// Define transformations from local spaces to world space.
	XMFLOAT4X4 mPelletWorld[240];
	XMFLOAT4X4 mPowerUpWorld[4];
	XMFLOAT4X4 mPacManWorld[3];
	XMFLOAT4X4 mGhostWorld[4];
	XMFLOAT4X4 mBoxWorld[55];
	XMFLOAT4X4 mGridWorld;
	XMFLOAT4X4 mFruitWorld[2];

	Blinky* mBlinky;
	Pinky* mPinky;
	Inky* mInky;
	Clyde* mClyde;

	std::vector<Vertex::NormalTexVertex> mMazeVerts;
	std::vector<UINT> mMazeInd;
	UINT mCountWalls;
	UINT mCountPellets;
	UINT mCountPowerUps;
	UINT mCountPacMans;
	UINT mCountGhosts;

	enum GhostState
	{
		GS_NORMAL = 0,
		GS_BLUE,
		GS_FLASHING
	};
	enum SoundsState
	{
		SS_DEFAULT = 0,
		SS_WA,
		SS_KA
	};
	enum FruitState
	{
		FS_DEFAULT = 0,
		FS_FRUIT
	};

	enum FacingState
	{
		FCS_FORWARD,
		FCS_BACKWARD,
		FCS_RIGHT,
		FCS_LEFT,
		FCS_DEFAULT
	};

	enum GameState
	{
		GS_ATTRACT = 0,
		GS_PLAY,
		GS_GAMEOVER
	};
	GameState mGameState = GameState::GS_ATTRACT;
	FacingState mFacingState = FCS_DEFAULT;
	GhostState ghostState = GhostState::GS_NORMAL;
	SoundsState soundStates = SoundsState::SS_DEFAULT;
	FruitState fruitState = FruitState::FS_DEFAULT;

	FMOD::System     *sys;
	FMOD::Sound      *sound[9];
	FMOD::Channel    *channel[8];
	FMOD::ChannelGroup *soundGroup, *masterGroup;
	FMOD_RESULT result;
	unsigned int      version;
	void             *extradriverdata = 0;

	RandGen rg;
	int randNumber;
	int mScore = 0;
	std::stringstream currScore;

	float mTimeGhostCurrent;
	float mTimeGhostNext;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	PuckMan3D theApp(hInstance);
	


	if( !theApp.Init() )
		return 0;

	
	return theApp.Run();
}


PuckMan3D::PuckMan3D(HINSTANCE hInstance)
	: D3DApp(hInstance), mLitTexEffect(0), mMouseReleased(true), mCam(0), mPelletCounter(0), mLevelCounter(1), mTestPlayer(0), mTestTerrain(0),
	mSkyBox(NULL), mParticleEffect(NULL), mIsKeyPressed(false), mSpeed(710.0f),
	mCountPellets(0), mLitMatInstanceEffect(0), mTimeGhostCurrent(0.0f), mTimeGhostNext(0.0f)
{
	soundStates = SoundsState::SS_KA;
	for (int i = 0; i < 8; ++i)
	{
		channel[i] = 0;
	}

	XMVECTOR pos = XMVectorSet(1.0f, 1.0f, 5.0f, 0.0f);
	XMVECTOR look = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	mMainWndCaption = L"Box Demo";
	
	mLastMousePos.x = 0;
	mLastMousePos.y = 0;

	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mView, I);
	XMStoreFloat4x4(&mProj, I);
	XMStoreFloat4x4(&m2DProj, I);
	XMStoreFloat4x4(&mGridWorld, I);

	srand((UINT)time(NULL));

	AllocConsole();
	freopen("CON", "w", stdout);
}

PuckMan3D::~PuckMan3D()
{
	Vertex::CleanLayouts();

	for (int i = 0; i < mTestChars.size(); ++i)
	{
		delete mTestChars[i];
	}

	if(mTestTerrain)
		delete mTestTerrain;

	if (mLitMatInstanceEffect)
		delete mLitMatInstanceEffect;
	
	if (mLitTexEffect)
		delete mLitTexEffect;

	if (mTestPlayer)
		delete mTestPlayer;
	
	if(mCam)
		delete mCam;

	if (m2DCam)
		delete m2DCam;

	if (mSkyBox)
		delete mSkyBox;

	if (mParticleEffect)
		delete mParticleEffect;

	if (mParticleVB)
		ReleaseCOM(mParticleVB);

	if (mParticleTexture)
		ReleaseCOM(mParticleTexture);

	if (mAdditiveBS)
		ReleaseCOM(mAdditiveBS);

	if (mTransparentBS)
		ReleaseCOM(mTransparentBS);

	if (mNoDepthDS)
		ReleaseCOM(mNoDepthDS);
}

void PuckMan3D::BuildSceneLights()
{
	mPointLights[0].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mPointLights[0].lightColour = XMFLOAT4(1.00f, 0.001f, 0.001f, 1.0f);
	mPointLights[0].range = 1000.0f;
	mPointLights[0].att = XMFLOAT3(0.0f, 0.02f, 0.0f);
	mPointLights[0].pad = 0.0f;

	mPointLights[1].pos = XMFLOAT3(-10.0f, 0.0f, -15.0f);
	mPointLights[1].lightColour = XMFLOAT4(0.001f, 1.00f, 0.001f, 1.0f);
	mPointLights[1].range = 1000.0f;
	mPointLights[1].att = XMFLOAT3(0.0f, 0.02f, 0.0f);
	mPointLights[1].pad = 0.0f;

	mPointLights[2].pos = XMFLOAT3(10.0f, 0.0f, -15.0f);
	mPointLights[2].lightColour = XMFLOAT4(0.001f, 0.001f, 1.00f, 1.0f);
	mPointLights[2].range = 1000.0f;
	mPointLights[2].att = XMFLOAT3(0.0f, 0.02f, 0.0f);
	mPointLights[2].pad = 0.0f;

	mSpotLight.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mSpotLight.lightColour = XMFLOAT4(0.75f, 0.75f, 0.75f, 1.0f);
	mSpotLight.range = 100.0f;
	mSpotLight.att = XMFLOAT3(0.0f, 0.25f, 0.0f);
	XMVECTOR temp = XMVectorSet(-mSpotLight.pos.x, -mSpotLight.pos.y,
							    -mSpotLight.pos.z, 0.0f);
	temp = XMVector3Normalize(temp);
	XMStoreFloat3(&mSpotLight.direction, temp);
	mSpotLight.spot = 128.0f;

	mAmbientColour = XMFLOAT4(0.9f, 0.9f, 0.9f, 1.0f);
}

bool PuckMan3D::Init()
{
	loadSystem();

	//load the sound effects
	loadScaredGhostSFX();
	loadGhostDeathSFX();
	loadDeathSFX();
	loadFruitSFX();
	loadBeginningSFX();
	loadExtraLifeSFX();
	loadSirenSFX();
	loadWaSFX();
	loadKaSFX();
	
	playBeginningSFX();
	//pass the score into stringstream
	currScore << mScore;

	if(!D3DApp::Init())
		return false;

	mLitMatInstanceEffect = new LitMatEffect();
	mLitMatInstanceEffect->LoadEffect(L"FX/lightingInstanced.fx", md3dDevice);
	Vertex::InitLitMatInstanceLayout(md3dDevice, mLitMatInstanceEffect->GetTech());

	//mMazeModel = new BasicModel(md3dDevice, mLitMatEffect, "Mazes/mainLevel.txt");
	mMazeModelInstanced = new BasicModel(md3dDevice, mLitMatInstanceEffect, "Mazes/mainLevel.txt");

	BuildPuckMan();
	BuildGhosts();
	SetMaterials();
	BuildShapeGeometryBuffers();

	mMazeModelInstanced->GetMesh()->SetMaterial(Materials::BOX);

	mLitTexEffect = new LitTexEffect();
	mLitTexEffect->LoadEffect(L"FX/lighting.fx", md3dDevice);

	mParticleEffect = new ParticleEffect();
	mParticleEffect->LoadEffect(L"FX/ParticleEffect.fx", md3dDevice);

	Vertex::InitLitTexLayout(md3dDevice, mLitTexEffect->GetTech());

	XMVECTOR pos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR look = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	mMazeCharacterInstanced = new Character(pos, look, up, *mMazeModelInstanced);

	m2DCam = new BaseCamera(XMVectorSet(0.0f, 0.0f, -0.5f, 0.0f), XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	
	m2DCam->Update();

	BuildSceneLights();

	Terrain::InitInfo terrainInfo;
	terrainInfo.cellHeight = 1.0f;
	terrainInfo.cellWidth = 1.0f;
	terrainInfo.height = 513;
	terrainInfo.width = 513;
	terrainInfo.heightMapFilename = L"Textures/terrain8.raw";
	terrainInfo.layerMapFilename0 = L"Textures/frozen6d.png";
	terrainInfo.layerMapFilename1 = L"Textures/rock1a.png";
	terrainInfo.layerMapFilename2 = L"Textures/rock6b.png";
	terrainInfo.layerMapFilename3 = L"Textures/frozen2c.png";
	terrainInfo.blendMapFilename = L"Textures/splat1b.png";
	terrainInfo.heightScale = 100.0f;
	terrainInfo.texTilesWide = 20.0f;
	terrainInfo.texTilesHigh = 20.0f;

	mTestTerrain = new Terrain();
	mTestTerrain->Init(md3dDevice, terrainInfo);

	Vertex::InitTerrainVertLayout(md3dDevice, mTestTerrain->GetEffect()->GetTech());
	Vertex::InitParticleVertLayout(md3dDevice, mParticleEffect->GetTech());

	BuildParticleVB();

	D3DX11CreateShaderResourceViewFromFile(md3dDevice, L"Textures/TestAdditive.png", 0, 0, &mParticleTexture, 0);

	BuildBlendStates();
	BuildDSStates();

	ID3D11ShaderResourceView* font;
	D3DX11CreateShaderResourceViewFromFile(md3dDevice, L"Textures/font2.png", 0, 0, &font, 0);

	mFont = new FontRasterizer(m2DCam, XMLoadFloat4x4(&m2DProj), mLitTexEffect, 10, 10, font, md3dDevice);

	mTimeGhostCurrent = 0.0f;
	mTimeGhostNext = mTimeGhostCurrent + (1/30);

	return true;
}

void PuckMan3D::BuildBlendStates()
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

	HR(md3dDevice->CreateBlendState(&bsDesc, &mAdditiveBS));

	bsDesc.RenderTarget[0].BlendEnable = true;
	bsDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bsDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	HR(md3dDevice->CreateBlendState(&bsDesc, &mTransparentBS));
}

void PuckMan3D::BuildDSStates()
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

	HR(md3dDevice->CreateDepthStencilState(&dsDesc, &mNoDepthDS));

	dsDesc.DepthEnable = false;
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

	HR(md3dDevice->CreateDepthStencilState(&dsDesc, &mFontDS));
}

void PuckMan3D::BuildParticleVB()
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
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mParticleVB));

}

void PuckMan3D::UpdateParticleVB()
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(md3dImmediateContext->Map(mParticleVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	Vertex::ParticleVertex* v = reinterpret_cast<Vertex::ParticleVertex*> (mappedData.pData);

	for (UINT i = 0; i < mParticles.size(); ++i)
	{
		v[i].pos = mParticles[i].pos;
		v[i].size = mParticles[i].size;
	}

	md3dImmediateContext->Unmap(mParticleVB, 0);
}

void PuckMan3D::OnResize()
{
	D3DApp::OnResize();

	// The window resized, so update the aspect ratio and recompute the projection matrix.
	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&mProj, P);

	P = XMMatrixOrthographicOffCenterLH(0.0f, mClientWidth, 0.0f, mClientHeight, 0.0001f, 1000.0f);
	XMStoreFloat4x4(&m2DProj, P);
}

void PuckMan3D::UpdateCollision()
{
	return;
	for (int i = 0; i < mTestChars.size(); ++i)
	{
		XMVECTOR playerPos = mTestPlayer->GetPos();
		XMVECTOR correct = CylToCyl(playerPos, 1, 5, mTestChars[i]->GetPos(),
			1, 5);
		playerPos -= correct;
		mTestPlayer->SetPos(playerPos);
	}
}

float timer = 0.0f;
void PuckMan3D::UpdateScene(float dt)
{
	UpdateKeyboardInput(dt);
	updateStringStream();

	if (mCanMove == false)
	{
		mTotalDeathTime += dt;
	}

	mTimeGhostCurrent += dt;
	if (mTimeGhostCurrent >= mTimeGhostNext)
	{
		if (mGameState == GameState::GS_PLAY && mCanMove && !mIsPaused)
		{
			mBlinky->Update(dt);
		}
		MazeLoader::SetGhostPos(XMVectorSet(mBlinky->getPos().x, mBlinky->getPos().y, mBlinky->getPos().z + 0.5f, 0.0f), 0);
		MazeLoader::SetGhostPos(XMVectorSet(mInky->getPos().x, mInky->getPos().y, mInky->getPos().z, 0.0f), 1);
		MazeLoader::SetGhostPos(XMVectorSet(mPinky->getPos().x, mPinky->getPos().y, mPinky->getPos().z, 0.0f), 2);
		MazeLoader::SetGhostPos(XMVectorSet(mClyde->getPos().x, mClyde->getPos().y, mClyde->getPos().z, 0.0f), 3);
		mTimeGhostNext += (1.0f / 10.0f);
	}

	std::vector<MazeLoader::MazeElementSpecs> pacMans = MazeLoader::GetPacManData();
	XMVECTOR pos = XMLoadFloat3(&pacMans[0].pos);
	XMVECTOR vel = XMLoadFloat3(&pacMans[0].vel);

	PuckManSpeed();

	pos = pos + (vel * mSpeed * dt);

	MazeLoader::SetPacManPos(pos, 0);
	MazeLoader::SetPacManVel(vel, 0);

	//// Checking PacMan collision with maze
	MazeLoader::SetPacManPos(PacManAABoxOverLap(pos), 0);

	std::vector<MazeLoader::MazeElementSpecs> ghosts = MazeLoader::GetGhostData();
	////Checking PacMan Collision with Ghost
	for (int i = 0; i < ghosts.size(); ++i)
	{
		XMVECTOR ghostPos = XMLoadFloat3(&ghosts[i].pos);

		if (PacManGhostOverlapTest(pos, ghostPos) == true)
		{
			if (!powerUpActivated)
			{
				result = channel[6]->setPaused(true);
				MazeLoader::RemoveLastPacMan();
				MazeLoader::InitialPosition pacPos = MazeLoader::GetInitialPos();
				MazeLoader::SetPacManPos(XMVectorSet(pacPos.pacMan.x, pacPos.pacMan.y, pacPos.pacMan.z, 0.0f), 0);
				std::vector<MazeLoader::MazeElementSpecs> pacMans = MazeLoader::GetPacManData();
				playDeathSFX();
				mIsPlayerDead = true;
				mIsMoving = false;
				mCanMove = false;
				mIsPaused = true;
				ResetGhosts();
				break;
			}
		}

	}

	std::vector<MazeLoader::MazeElementSpecs> pellets = MazeLoader::GetPelletData();
	////checking PacMan collision with Pellets
	//// If collision is true remove the pellet.
	for (int i = 0; i < pellets.size(); ++i)
	{
		XMVECTOR pelPos = XMLoadFloat3(&pellets[i].pos);

		if (pellets[i].isCollider && PacManPelletOverlapTest(pos, pelPos) == true)
		{
			if (soundStates == SoundsState::SS_KA)
			{
				playWaSFX();
			}
			else if (soundStates == SoundsState::SS_WA)
			{
				playKaSFX();
			}
			mPelletCounter++;
			MazeLoader::RemovePellet(i);
			mScore += 10;
			break;
		}

	}
	std::vector<MazeLoader::MazeElementSpecs> powerUps = MazeLoader::GetPowerUpData();
	////checking PacMan collision with PowerUps
	//// If collision is true remove the powerup.
	for (int i = 0; i < powerUps.size(); ++i)
	{
		XMVECTOR pUpPos = XMLoadFloat3(&powerUps[i].pos);

		if (powerUps[i].isCollider && PacManPowerUpOverlapTest(pos, pUpPos) == true)
		{
			powerUpActivated = true;
			ghostState = GhostState::GS_BLUE;
			mCurrentTime = 0.0f;
			mTotalTime = 3.0f;
			mPelletCounter++;
			MazeLoader::RemovePowerUp(i);
			mScore += 50;
			break;
		}
	}

	////PacMan Tunnel Check
	if (pacMans[0].pos.x < -14.0f)
	{
		pos.m128_f32[0] = 14.0f;
		MazeLoader::SetPacManPos(pos, 0);
	}
	if (pacMans[0].pos.x > 14.0f)
	{
		pos.m128_f32[0] = -14.0f;
		MazeLoader::SetPacManPos(pos, 0);
	}

	if (mTotalDeathTime >= 2.0f)
	{//allow movement
		mCanMove = true;
		mTotalDeathTime = 0.0;
	}

	if (mIsMoving)
	{//check if the player is moving to determine when to play the background sound
		playSirenSFX();
		result = channel[6]->setPaused(false);
	}
	else if (mIsPlayerDead || mGameState != GameState::GS_PLAY)
	{
		powerUpActivated = false;
		mIsBlue = false;
		mIsMoving = false;
		result = channel[6]->setPaused(true);
	}

	if (!pacMans[0].isShown)
	{
		mGameState = GameState::GS_GAMEOVER;
	}

	//reset board if all pellets are gone
	if (mPelletCounter == MazeLoader::GetEatableCount())
	{
		resetGame();
		mLevelCounter++;
		mIsPaused = true;
	}

	if (mIsPaused)
	{
		mPauseTime += dt;
	}

	if (mPauseTime >= 2.0f)
	{
		mIsPaused = false;
		mCanMove = true;
		mPauseTime = 0.0f;
	}

	if (powerUpActivated)
	{//check if the power up is activated to detrmine if playScaredGhostSFX is active or not
		playScaredGhostSFX();
		result = channel[1]->setPaused(false);
	}
	else
	{
		result = channel[1]->setPaused(true);
	}
	updateGhosts(dt);

	float eyeOffset = 25.0f;

	// Camera X, Y, Z Positioning.
	float x = pacMans[0].pos.x;
	float y = pacMans[0].pos.y + eyeOffset;
	float z = pacMans[0].pos.z - eyeOffset;

	mEyePosW = XMFLOAT3(x, y, z);

	// Build the view matrix.
	XMVECTOR eyePos = XMVectorSet(x, y, z, 1.0f);
	XMVECTOR target = XMVectorSet(pacMans[0].pos.x, pacMans[0].pos.y + 5, pacMans[0].pos.z, 0.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX V = XMMatrixLookAtLH(eyePos, target, up);
	XMStoreFloat4x4(&mView, V);

	mMazeCharacterInstanced->Update(dt);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	Vertex::InstancedData* dataView;

	std::vector<MazeLoader::MazeElementSpecs> walls = MazeLoader::GetWallData();
	md3dImmediateContext->Map(mMazeModelInstanced->GetMesh()->GetInstanceBWalls(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	dataView = reinterpret_cast<Vertex::InstancedData*>(mappedData.pData);
	mCountWalls = 0;
	for (UINT i = 0; i < walls.size(); ++i)
	{
		dataView[mCountWalls++] = { walls[i].world, walls[i].colour };
	}
	md3dImmediateContext->Unmap(mMazeModelInstanced->GetMesh()->GetInstanceBWalls(), 0);

	md3dImmediateContext->Map(mMazeModelInstanced->GetMesh()->GetInstanceBPellets(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	dataView = reinterpret_cast<Vertex::InstancedData*>(mappedData.pData);
	mCountPellets = 0;
	for (UINT i = 0; i < pellets.size(); ++i)
	{
		if (pellets[i].isShown)
		{
			dataView[mCountPellets++] = { pellets[i].world, pellets[i].colour };
		}
	}
	md3dImmediateContext->Unmap(mMazeModelInstanced->GetMesh()->GetInstanceBPellets(), 0);

	md3dImmediateContext->Map(mMazeModelInstanced->GetMesh()->GetInstanceBPowerUps(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	dataView = reinterpret_cast<Vertex::InstancedData*>(mappedData.pData);
	mCountPowerUps = 0;
	for (UINT i = 0; i < powerUps.size(); ++i)
	{
		if (powerUps[i].isShown)
		{
			dataView[mCountPowerUps++] = { powerUps[i].world, powerUps[i].colour };
		}
	}
	md3dImmediateContext->Unmap(mMazeModelInstanced->GetMesh()->GetInstanceBPowerUps(), 0);

	md3dImmediateContext->Map(mMazeModelInstanced->GetMesh()->GetInstanceBPacMans(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	dataView = reinterpret_cast<Vertex::InstancedData*>(mappedData.pData);
	mCountPacMans = 0;
	for (UINT i = 0; i < pacMans.size(); ++i)
	{
		if (pacMans[i].isShown)
		{
			dataView[mCountPacMans++] = { pacMans[i].world, pacMans[i].colour };
		}
	}
	md3dImmediateContext->Unmap(mMazeModelInstanced->GetMesh()->GetInstanceBPacMans(), 0);

	md3dImmediateContext->Map(mMazeModelInstanced->GetMesh()->GetInstanceBGhosts(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	dataView = reinterpret_cast<Vertex::InstancedData*>(mappedData.pData);
	mCountGhosts = 0;
	for (UINT i = 0; i < ghosts.size(); ++i)
	{
		dataView[mCountGhosts++] = { ghosts[i].world, ghosts[i].colour };
	}
	md3dImmediateContext->Unmap(mMazeModelInstanced->GetMesh()->GetInstanceBGhosts(), 0);

	return;
	mTestPlayer->AddForce(XMVectorSet(0.0f, -20.0f * dt, 0.0f, 0.0f));

	mTestPlayer->Update(dt);

	if (!UpdateGroundCollision())
	{
		mTestPlayer->RemoveVelocityApplied();
	}
	float terrainHeight = mTestTerrain->GetHeight(mTestPlayer->GetPos().m128_f32[0], mTestPlayer->GetPos().m128_f32[2]);

	XMVECTOR playerPos = mTestPlayer->GetPos();
	XMVECTOR toPlayer = playerPos - mCam->GetPos();
	toPlayer = XMVector3Normalize(toPlayer);
	mCam->SetFacing(toPlayer, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	mCam->SetPos(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	mCam->Update();

	m2DCam->Update();

	XMStoreFloat3(&mSpotLight.pos, mCam->GetPos());
	XMStoreFloat3(&mSpotLight.direction, mCam->GetLook());

	for (int i = 0; i < mTestChars.size(); ++i)
	{
		mTestChars[i]->Update(dt);
	}
	
	for (int i = 0; i < mProjectiles.size(); ++i)
	{
		
		mProjectiles[i]->Update(dt);

		if (mProjectiles[i]->GetDistanceTravelled() >
			mProjectiles[i]->MAX_DISTANCE)
		{
			delete mProjectiles[i];
			mProjectiles.erase(mProjectiles.begin() + i);
			i--;
		}
	}

	for (int i = 0; i < mParticles.size(); ++i)
	{
		XMVECTOR vel = XMLoadFloat3(&mParticles[i].vel) * 2.5f;
		XMVECTOR pos = XMLoadFloat3(&mParticles[i].pos);
		pos = pos + vel;
		XMStoreFloat3(&mParticles[i].pos, pos);
	}
	
	UpdateParticleVB();
	UpdateCollision();
}

void PuckMan3D::DrawScene()
{
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, 
		reinterpret_cast<const float*>(&Colors::Black));
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, 
			D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

	md3dImmediateContext->IASetInputLayout(Vertex::GetNormalTexVertLayout());
    md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	XMVECTOR ambient = XMLoadFloat4(&mAmbientColour);
	XMMATRIX proj = XMLoadFloat4x4(&mProj);

	XMVECTOR eyePos = XMVectorSet(mEyePosW.x, mEyePosW.y, mEyePosW.z, 0.0f);
	XMMATRIX view = XMLoadFloat4x4(&mView);

	mLitMatInstanceEffect->SetPerFrameParams(ambient, eyePos, mPointLights);

	XMMATRIX vp = view * proj;

	//// Draw the grid.
	XMMATRIX world = XMLoadFloat4x4(&mGridWorld);
	XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
	XMMATRIX worldViewProj = world*view*proj;
	XMMATRIX viewProj = view*proj;
	
	world = XMLoadFloat4x4(&mGridWorld);
	worldInvTranspose = MathHelper::InverseTranspose(world);
	worldViewProj = world*view*proj;

	MazeLoader::OffsetsCountsMazeElements oc = MazeLoader::GetOffsetsCounts();

	md3dImmediateContext->IASetInputLayout(Vertex::GetNormalMatVertInstanceLayout());
	mLitMatInstanceEffect->SetEffectTech("LitMatTechInstanced");
	Material boxColour = Materials::BOX;
	mLitMatInstanceEffect->SetPerObjectParams(world, worldInvTranspose, worldViewProj, viewProj, boxColour);
	mLitMatInstanceEffect->DrawInstanced(md3dImmediateContext, mMazeModelInstanced->GetMesh()->GetVB(), mMazeModelInstanced->GetMesh()->GetIB(), mMazeModelInstanced->GetMesh()->GetInstanceBWalls(),
		mCountWalls, oc.walls.indexOffset, oc.walls.indexCount);
	if (mGameState == GameState::GS_PLAY)
	{
		md3dImmediateContext->IASetInputLayout(Vertex::GetNormalMatVertInstanceLayout());
		mLitMatInstanceEffect->SetEffectTech("LitMatTechInstanced");
		Material pelletColour = Materials::PELLET;
		mLitMatInstanceEffect->SetPerObjectParams(world, worldInvTranspose, worldViewProj, viewProj, pelletColour);
		mLitMatInstanceEffect->DrawInstanced(md3dImmediateContext, mMazeModelInstanced->GetMesh()->GetVB(), mMazeModelInstanced->GetMesh()->GetIB(), mMazeModelInstanced->GetMesh()->GetInstanceBPellets(),
			mCountPellets, oc.pellets.indexOffset, oc.pellets.indexCount);

		md3dImmediateContext->IASetInputLayout(Vertex::GetNormalMatVertInstanceLayout());
		mLitMatInstanceEffect->SetEffectTech("LitMatTechInstanced");
		Material powerUpColour = Materials::POWERUP;
		mLitMatInstanceEffect->SetPerObjectParams(world, worldInvTranspose, worldViewProj, viewProj, powerUpColour);
		mLitMatInstanceEffect->DrawInstanced(md3dImmediateContext, mMazeModelInstanced->GetMesh()->GetVB(), mMazeModelInstanced->GetMesh()->GetIB(), mMazeModelInstanced->GetMesh()->GetInstanceBPowerUps(),
			mCountPowerUps, oc.powerUps.indexOffset, oc.powerUps.indexCount);

		std::vector<MazeLoader::MazeElementSpecs> pacMans = MazeLoader::GetPacManData();
		world = XMLoadFloat4x4(&mGridWorld);
		worldInvTranspose = MathHelper::InverseTranspose(world);
		worldViewProj = world*view*proj;
		Material pacManColour = Materials::PACMAN;
		mLitMatInstanceEffect->SetPerObjectParams(world, worldInvTranspose, worldViewProj, viewProj, pacManColour);
		mLitMatInstanceEffect->DrawInstanced(md3dImmediateContext, mMazeModelInstanced->GetMesh()->GetVB(), mMazeModelInstanced->GetMesh()->GetIB(), mMazeModelInstanced->GetMesh()->GetInstanceBPacMans(),
			mCountPacMans, oc.pacMan.indexOffset, oc.pacMan.indexCount);

		world = XMMatrixTranslation(mBlinky->getPos().x, mBlinky->getPos().y, mBlinky->getPos().z);
		world = XMLoadFloat4x4(&mGridWorld);
		worldInvTranspose = MathHelper::InverseTranspose(world);
		worldViewProj = world*view*proj;
		Material blinkyColour = Materials::BLINKY;
		mLitMatInstanceEffect->SetPerObjectParams(world, worldInvTranspose, worldViewProj, viewProj, blinkyColour);
		mLitMatInstanceEffect->DrawInstanced(md3dImmediateContext, mMazeModelInstanced->GetMesh()->GetVB(), mMazeModelInstanced->GetMesh()->GetIB(), mMazeModelInstanced->GetMesh()->GetInstanceBGhosts(),
			mCountGhosts, oc.ghosts.indexOffset, oc.ghosts.indexCount);

		world = XMMatrixTranslation(mInky->getPos().x, mInky->getPos().y, mInky->getPos().z);
		world = XMLoadFloat4x4(&mGridWorld);
		worldInvTranspose = MathHelper::InverseTranspose(world);
		worldViewProj = world*view*proj;
		Material inkyColour = Materials::INKY;
		mLitMatInstanceEffect->SetPerObjectParams(world, worldInvTranspose, worldViewProj, viewProj, inkyColour);
		mLitMatInstanceEffect->DrawInstanced(md3dImmediateContext, mMazeModelInstanced->GetMesh()->GetVB(), mMazeModelInstanced->GetMesh()->GetIB(), mMazeModelInstanced->GetMesh()->GetInstanceBGhosts(),
			1, oc.ghosts.indexOffset, oc.ghosts.indexCount);

		world = XMMatrixTranslation(mPinky->getPos().x, mPinky->getPos().y, mPinky->getPos().z);
		world = XMLoadFloat4x4(&mGridWorld);
		worldInvTranspose = MathHelper::InverseTranspose(world);
		worldViewProj = world*view*proj;
		Material pinkyColour = Materials::PINKY;
		mLitMatInstanceEffect->SetPerObjectParams(world, worldInvTranspose, worldViewProj, viewProj, pinkyColour);
		mLitMatInstanceEffect->DrawInstanced(md3dImmediateContext, mMazeModelInstanced->GetMesh()->GetVB(), mMazeModelInstanced->GetMesh()->GetIB(), mMazeModelInstanced->GetMesh()->GetInstanceBGhosts(),
			1, oc.ghosts.indexOffset, oc.ghosts.indexCount);

		world = XMMatrixTranslation(mClyde->getPos().x, mClyde->getPos().y, mClyde->getPos().z);
		world = XMLoadFloat4x4(&mGridWorld);
		worldInvTranspose = MathHelper::InverseTranspose(world);
		worldViewProj = world*view*proj;
		Material clydeColour = Materials::CLYDE;
		mLitMatInstanceEffect->SetPerObjectParams(world, worldInvTranspose, worldViewProj, viewProj, clydeColour);
		mLitMatInstanceEffect->DrawInstanced(md3dImmediateContext, mMazeModelInstanced->GetMesh()->GetVB(), mMazeModelInstanced->GetMesh()->GetIB(), mMazeModelInstanced->GetMesh()->GetInstanceBGhosts(),
			1, oc.ghosts.indexOffset, oc.ghosts.indexCount);

		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		md3dImmediateContext->OMSetBlendState(mTransparentBS, blendFactor, 0xffffffff);
		md3dImmediateContext->OMSetDepthStencilState(mFontDS, 0);

		std::stringstream os;
		//os << "(" << pacMans[0].pos.x << ", " << pacMans[0].pos.z << ")" << "    " << mPelletCounter;
		mLitTexEffect->SetPerFrameParams(ambient, eyePos, mPointLights[0], mSpotLight);
		mFont->DrawFont(md3dImmediateContext, XMVectorSet(10.0f, 500.0f, 0.0f, 0.0f), 50, 75, 10, os.str());
		mFont->DrawFont(md3dImmediateContext, XMVectorSet(10.0f, 620.0f, 0.0f, 0.0f), 50, 75, 25, "Score: " + currScore.str());
		md3dImmediateContext->OMSetDepthStencilState(0, 0);
		md3dImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff);
	}
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	md3dImmediateContext->OMSetBlendState(mTransparentBS, blendFactor, 0xffffffff);
	md3dImmediateContext->OMSetDepthStencilState(mFontDS, 0);

	mLitTexEffect->SetPerFrameParams(ambient, eyePos, mPointLights[0], mSpotLight);
	
	if (mGameState == GameState::GS_ATTRACT)
	{
		mFont->DrawFont(md3dImmediateContext, XMVectorSet(10.0f, 620.0f, 0.0f, 0.0f), 50, 75, 15, "Press space to begin");
	}
	if (mGameState == GameState::GS_GAMEOVER)
	{
		mFont->DrawFont(md3dImmediateContext, XMVectorSet(10.0f, 620.0f, 0.0f, 0.0f), 50, 75, 15, "Press space to go back.");
	}
	md3dImmediateContext->OMSetDepthStencilState(0, 0);
	md3dImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff);

	HR(mSwapChain->Present(1, 0));
}

void PuckMan3D::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(mhMainWnd);	 
}

void PuckMan3D::OnMouseUp(WPARAM btnState, int x, int y)
{
	mMouseReleased = false;
	ReleaseCapture();
}

void PuckMan3D::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_RBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));
		mLastMousePos.x = x;
		mLastMousePos.y = y;
	}
}

void PuckMan3D::UpdateKeyboardInput(float dt)
{
	std::vector<MazeLoader::MazeElementSpecs> pacMans = MazeLoader::GetPacManData();
	XMVECTOR vel;

	mIsKeyPressed = false;
	vel.m128_f32[0] = 0.0f;
	vel.m128_f32[1] = 0.0f;
	vel.m128_f32[2] = 0.0f;
	mFacingState = FCS_DEFAULT;
	if (mGameState == GameState::GS_PLAY && mCanMove && !mIsPaused)
	{
		// Move Forward
		if (GetAsyncKeyState('W') || GetAsyncKeyState(VK_UP) & 0x8000)
		{
			mIsKeyPressed = true;
			mFacingState = FCS_FORWARD;
			mIsMoving = true;
			vel.m128_f32[0] = 0.0f * dt;
			vel.m128_f32[1] = 0.0f * dt;
			vel.m128_f32[2] = 1.0f * dt;
			if (vel.m128_f32[2] < 0.00826695096f)
			{
				vel.m128_f32[2] = 0.00826695096f;
			}
		}

		// Move Backwards 
		if (GetAsyncKeyState('S') || GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			mIsKeyPressed = true;
			mFacingState = FCS_BACKWARD;
			mIsMoving = true;
			vel.m128_f32[0] = 0.0f * dt;
			vel.m128_f32[1] = 0.0f * dt;
			vel.m128_f32[2] = -1.0f * dt;
			if (vel.m128_f32[2] > -0.00826695096f)
			{
				vel.m128_f32[2] = -0.00826695096f;
			}
		}

		// Move Left
		if (GetAsyncKeyState('A') || GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			mIsKeyPressed = true;
			mFacingState = FCS_LEFT;
			mIsMoving = true;
			vel.m128_f32[0] = -1.0f * dt;
			if (vel.m128_f32[0] > -0.00826695096f)
			{
				vel.m128_f32[0] = -0.00826695096f;
			}
			vel.m128_f32[1] = 0.0f * dt;
			vel.m128_f32[2] = 0.0f * dt;
		}

		// Move Right
		if (GetAsyncKeyState('D') || GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			mIsKeyPressed = true;
			mFacingState = FCS_RIGHT;
			mIsMoving = true;
			vel.m128_f32[0] = 1.0f * dt;
			if (vel.m128_f32[0] < 0.00826695096f)
			{
				vel.m128_f32[0] = 0.00826695096f;
			}

			vel.m128_f32[1] = 0.0f * dt;
			vel.m128_f32[2] = 0.0f * dt;
		}
	}
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
	{
		std::exit(1);
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if (mGameState == GameState::GS_ATTRACT)
		{
			mGameState = GameState::GS_PLAY;
		}
		else if (mGameState == GameState::GS_PLAY)
		{

		}
		else if (mGameState == GameState::GS_GAMEOVER)
		{
			resetGame();
			mGameState = GameState::GS_ATTRACT;
		}
	}
	MazeLoader::SetPacManVel(vel, 0);
}
//PuckMan’s Speed while eating dots :
//In First level PuckMan’s speed is 0.71 meters per second.
//In Second to Fourth level speed is 0.79 meters per second.
//In Fifth to Twentieth level speed is 0.87 meters per second.
//Levels Twenty - one plus speed is 0.79 meters per second.
//
//PuckMan’s Speed while NOT eating dots :
//In First level PuckMan’s speed is 0.8 meters per second.
//In Second to Fourth level speed is 0.9 meters per second.
//In Fifth to Twentieth level speed is 1.0 meters per second.
//Levels Twenty - one plus speed is 0.9 meters per second.
//
//PuckMan’s Speed while eating dots and ghost frightened :
//In First level PuckMan’s speed is 0.79 meters per second.
//In Second to Fourth level speed is 0.83 meters per second.
//In Fifth to Twentieth level speed is 0.87 meters per second.
//Levels Twenty - one plus speed is 0.87 meters per second.
//
//PuckMan’s Speed while NOT eating dots and ghost frightened :
//In First level PuckMan’s speed is 0.9 meters per second.
//In Second to Fourth level speed is 0.95 meters per second.
//In Fifth to Twentieth level speed is 1.0 meters per second.
//Levels Twenty - one plus speed is 1.0 meters per second.
//
void PuckMan3D::PuckManSpeed()
{
	std::vector<MazeLoader::MazeElementSpecs> pacMans = MazeLoader::GetPacManData();
	XMVECTOR pos = XMLoadFloat3(&pacMans[0].pos);
	float posX = pacMans[0].pos.x;
	float posZ = pacMans[0].pos.z;

	//translate Puckmans Position to Pellet space
	int transX = (int)floor(posX + (MazeLoader::GetMazeWidth() * 0.5));
	int transZ = MazeLoader::GetMazeHeight() - (int)round(posZ + (MazeLoader::GetMazeHeight() * 0.5)); //invert the z
	int row = transZ;
	int col = transX;

	switch (mFacingState)
	{
	case FCS_FORWARD:
		if (mLevelCounter == 1)
		{

			if (MazeLoader::IsPellet(row - 1, col))
			{
				mSpeed = PUCKMAN_SPEED * 0.71f; //eating pellet speed
			}
			if (!(MazeLoader::IsPellet(row - 1, col)))
			{
				mSpeed = PUCKMAN_SPEED * 0.8f; // no pellet to eat speed
			}
			if (MazeLoader::IsPellet(row - 1, col) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.79f; // eating pellet and ghost frightened speed
			}
			if (!MazeLoader::IsPellet(row - 1, col) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.9f; // no pellet to eat and ghost frightened speed
			}
		}
		if (mLevelCounter > 1 && mLevelCounter < 5) // levels 2 through 4
		{
			if (MazeLoader::IsPellet(row - 1, col))
			{
				mSpeed = PUCKMAN_SPEED * 0.79f; //eating pellet speed
			}
			if (!(MazeLoader::IsPellet(row - 1, col)))
			{
				mSpeed = PUCKMAN_SPEED * 0.9f; // no pellet to eat speed
			}
			if (MazeLoader::IsPellet(row - 1, col) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.83f; // eating pellet and ghost frightened speed
			}
			if (!MazeLoader::IsPellet(row - 1, col) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.95f; // no pellet to eat and ghost frightened speed
			}
		}
		if (mLevelCounter > 4 && mLevelCounter < 21) // levels 5 through 20
		{
			if (MazeLoader::IsPellet(row - 1, col))
			{
				mSpeed = PUCKMAN_SPEED * 0.87f; //eating pellet speed
			}
			if (!(MazeLoader::IsPellet(row - 1, col)))
			{
				mSpeed = PUCKMAN_SPEED * 1.0f; // no pellet to eat speed
			}
			if (MazeLoader::IsPellet(row - 1, col) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.87f; // eating pellet and ghost frightened speed
			}
			if (!MazeLoader::IsPellet(row - 1, col) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 1.0f; // no pellet to eat and ghost frightened speed
			}
		}
		if (mLevelCounter >= 21) // Level 21 and up
		{
			if (MazeLoader::IsPellet(row - 1, col))
			{
				mSpeed = PUCKMAN_SPEED * 0.79f; //eating pellet speed
			}
			if (!(MazeLoader::IsPellet(row - 1, col)))
			{
				mSpeed = PUCKMAN_SPEED * 0.9f; // no pellet to eat speed
			}
			if (MazeLoader::IsPellet(row - 1, col) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.87f; // eating pellet and ghost frightened speed
			}
			if (!MazeLoader::IsPellet(row - 1, col) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 1.0f; // no pellet to eat and ghost frightened speed
			}
		}

		break;

	case FCS_BACKWARD:
		if (mLevelCounter == 1)
		{
			if (MazeLoader::IsPellet(row + 1, col))
			{
				mSpeed = PUCKMAN_SPEED * 0.71f; //eating pellet speed
			}
			if (!(MazeLoader::IsPellet(row + 1, col)))
			{
				mSpeed = PUCKMAN_SPEED * 0.8f; // no pellet to eat speed
			}
			if (MazeLoader::IsPellet(row + 1, col) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.79f; // eating pellet and ghost frightened speed
			}
			if (!MazeLoader::IsPellet(row + 1, col) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.9f; // no pellet to eat and ghost frightened speed
			}
		}
		if (mLevelCounter > 1 && mLevelCounter < 5) // levels 2 through 4
		{
			if (MazeLoader::IsPellet(row + 1, col))
			{
				mSpeed = PUCKMAN_SPEED * 0.79f; //eating pellet speed
			}
			if (!(MazeLoader::IsPellet(row + 1, col)))
			{
				mSpeed = PUCKMAN_SPEED * 0.9f; // no pellet to eat speed
			}
			if (MazeLoader::IsPellet(row + 1, col) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.83f; // eating pellet and ghost frightened speed
			}
			if (!MazeLoader::IsPellet(row + 1, col) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.95f; // no pellet to eat and ghost frightened speed
			}
		}
		if (mLevelCounter > 4 && mLevelCounter < 21) // levels 5 through 20
		{
			if (MazeLoader::IsPellet(row + 1, col))
			{
				mSpeed = PUCKMAN_SPEED * 0.87f; //eating pellet speed
			}
			if (!(MazeLoader::IsPellet(row + 1, col)))
			{
				mSpeed = PUCKMAN_SPEED * 1.0f; // no pellet to eat speed
			}
			if (MazeLoader::IsPellet(row + 1, col) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.87f; // eating pellet and ghost frightened speed
			}
			if (!MazeLoader::IsPellet(row + 1, col) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 1.0f; // no pellet to eat and ghost frightened speed
			}
		}
		if (mLevelCounter >= 21) // Level 21 and up
		{
			if (MazeLoader::IsPellet(row + 1, col))
			{
				mSpeed = PUCKMAN_SPEED * 0.79f; //eating pellet speed
			}
			if (!(MazeLoader::IsPellet(row + 1, col)))
			{
				mSpeed = PUCKMAN_SPEED * 0.9f; // no pellet to eat speed
			}
			if (MazeLoader::IsPellet(row + 1, col) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.87f; // eating pellet and ghost frightened speed
			}
			if (!MazeLoader::IsPellet(row + 1, col) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 1.0f; // no pellet to eat and ghost frightened speed
			}
		}

		break;

	case FCS_RIGHT:
		if (mLevelCounter == 1)
		{
			if (MazeLoader::IsPellet(row, col + 1))
			{
				mSpeed = PUCKMAN_SPEED * 0.71f; //eating pellet speed
			}
			if (!(MazeLoader::IsPellet(row, col + 1)))
			{
				mSpeed = PUCKMAN_SPEED * 0.8f; // no pellet to eat speed
			}
			if (MazeLoader::IsPellet(row, col + 1) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.79f; // eating pellet and ghost frightened speed
			}
			if (!MazeLoader::IsPellet(row, col + 1) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.9f; // no pellet to eat and ghost frightened speed
			}
		}
		if (mLevelCounter > 1 && mLevelCounter < 5) // levels 2 through 4
		{
			if (MazeLoader::IsPellet(row, col + 1))
			{
				mSpeed = PUCKMAN_SPEED * 0.79f; //eating pellet speed
			}
			if (!(MazeLoader::IsPellet(row, col + 1)))
			{
				mSpeed = PUCKMAN_SPEED * 0.9f; // no pellet to eat speed
			}
			if (MazeLoader::IsPellet(row, col + 1) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.83f; // eating pellet and ghost frightened speed
			}
			if (!MazeLoader::IsPellet(row, col + 1) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.95f; // no pellet to eat and ghost frightened speed
			}
		}
		if (mLevelCounter > 4 && mLevelCounter < 21) // levels 5 through 20
		{
			if (MazeLoader::IsPellet(row, col + 1))
			{
				mSpeed = PUCKMAN_SPEED * 0.87f; //eating pellet speed
			}
			if (!(MazeLoader::IsPellet(row, col + 1)))
			{
				mSpeed = PUCKMAN_SPEED * 1.0f; // no pellet to eat speed
			}
			if (MazeLoader::IsPellet(row, col + 1) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.87f; // eating pellet and ghost frightened speed
			}
			if (!MazeLoader::IsPellet(row, col + 1) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 1.0f; // no pellet to eat and ghost frightened speed
			}
		}
		if (mLevelCounter >= 21) // Level 21 and up
		{
			if (MazeLoader::IsPellet(row, col + 1))
			{
				mSpeed = PUCKMAN_SPEED * 0.79f; //eating pellet speed
			}
			if (!(MazeLoader::IsPellet(row, col + 1)))
			{
				mSpeed = PUCKMAN_SPEED * 0.9f; // no pellet to eat speed
			}
			if (MazeLoader::IsPellet(row, col + 1) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.87f; // eating pellet and ghost frightened speed
			}
			if (!MazeLoader::IsPellet(row, col + 1) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 1.0f; // no pellet to eat and ghost frightened speed
			}
		}

		break;

	case FCS_LEFT:
		if (mLevelCounter == 1)
		{
			if (MazeLoader::IsPellet(row, col - 1))
			{
				mSpeed = PUCKMAN_SPEED * 0.71f; //eating pellet speed
			}
			if (!(MazeLoader::IsPellet(row, col - 1)))
			{
				mSpeed = PUCKMAN_SPEED * 0.8f; // no pellet to eat speed
			}
			if (MazeLoader::IsPellet(row, col - 1) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.79f; // eating pellet and ghost frightened speed
			}
			if (!MazeLoader::IsPellet(row, col - 1) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.9f; // no pellet to eat and ghost frightened speed
			}
		}
		if (mLevelCounter > 1 && mLevelCounter < 5) // levels 2 through 4
		{
			if (MazeLoader::IsPellet(row, col - 1))
			{
				mSpeed = PUCKMAN_SPEED * 0.79f; //eating pellet speed
			}
			if (!(MazeLoader::IsPellet(row, col - 1)))
			{
				mSpeed = PUCKMAN_SPEED * 0.9f; // no pellet to eat speed
			}
			if (MazeLoader::IsPellet(row, col - 1) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.83f; // eating pellet and ghost frightened speed
			}
			if (!MazeLoader::IsPellet(row, col - 1) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.95f; // no pellet to eat and ghost frightened speed
			}
		}
		if (mLevelCounter > 4 && mLevelCounter < 21) // levels 5 through 20
		{
			if (MazeLoader::IsPellet(row, col - 1))
			{
				mSpeed = PUCKMAN_SPEED * 0.87f; //eating pellet speed
			}
			if (!(MazeLoader::IsPellet(row, col - 1)))
			{
				mSpeed = PUCKMAN_SPEED * 1.0f; // no pellet to eat speed
			}
			if (MazeLoader::IsPellet(row, col - 1) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.87f; // eating pellet and ghost frightened speed
			}
			if (!MazeLoader::IsPellet(row, col - 1) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 1.0f; // no pellet to eat and ghost frightened speed
			}
		}
		if (mLevelCounter >= 21) // Level 21 and up
		{
			if (MazeLoader::IsPellet(row, col - 1))
			{
				mSpeed = PUCKMAN_SPEED * 0.79f; //eating pellet speed
			}
			if (!(MazeLoader::IsPellet(row, col - 1)))
			{
				mSpeed = PUCKMAN_SPEED * 0.9f; // no pellet to eat speed
			}
			if (MazeLoader::IsPellet(row, col - 1) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.87f; // eating pellet and ghost frightened speed
			}
			if (!MazeLoader::IsPellet(row, col - 1) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 1.0f; // no pellet to eat and ghost frightened speed
			}
		}

		break;

	case FCS_DEFAULT:
		mSpeed = PUCKMAN_SPEED * 0.0f;
	}
}


XMVECTOR PuckMan3D::PacManAABoxOverLap(XMVECTOR s1Center)
{
	float s1Radius = MazeLoader::RADIUS_PAC_MAN;
	std::vector<MazeLoader::AABox> boxData = MazeLoader::GetWallCollisionData();
	float currOverLap = 0.0f;
	XMVECTOR correction = XMVectorZero();

	for (int i = 0; i < boxData.size(); ++i)
	{
		XMVECTOR min = XMLoadFloat3(&boxData[i].min);
		XMVECTOR max = XMLoadFloat3(&boxData[i].max);

		XMVECTOR A = XMVectorClamp(s1Center, min, max);

		XMVECTOR d = s1Center - A; //difference between the closest point on the box  and sphere center

		float distance = sqrt((d.m128_f32[0] * d.m128_f32[0]) /*+ (d.m128_f32[1] * d.m128_f32[1])*/ + (d.m128_f32[2] * d.m128_f32[2])); //Magnitude of the difference

		float overLap = s1Radius - distance;

		if (overLap > currOverLap) // Have Collision
		{
			currOverLap = overLap;

			correction = XMVector3Normalize(d) * currOverLap; //correct collision by moving sphere out of box
		}
	}
	return s1Center + correction;
}

bool PuckMan3D::PacManGhostOverlapTest(XMVECTOR s1Center, XMVECTOR s2Center)
{
	float s1Radius = MazeLoader::RADIUS_PAC_MAN;

	float s2Radius = MazeLoader::RADIUS_GHOST;

	XMVECTOR d = s1Center - s2Center; // difference between the two spheres centers
	float distance = sqrt((d.m128_f32[0] * d.m128_f32[0]) + (d.m128_f32[1] * d.m128_f32[1]) + (d.m128_f32[2] * d.m128_f32[2])); //magnitude of the difference
	float sumRadius = s1Radius + s2Radius; //sum of the 2 spheres radii

	if (distance < sumRadius) // have collision
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool PuckMan3D::PacManPelletOverlapTest(XMVECTOR s1Center, XMVECTOR s2Center)
{
	float s1Radius = MazeLoader::RADIUS_PAC_MAN;

	float s2Radius = MazeLoader::RADIUS_PELLET;

	XMVECTOR d = s1Center - s2Center; // difference between the two spheres centers
	float distance = sqrt((d.m128_f32[0] * d.m128_f32[0]) + (d.m128_f32[1] * d.m128_f32[1]) + (d.m128_f32[2] * d.m128_f32[2])); //magnitude of the difference
	float sumRadius = s1Radius + s2Radius; //sum of the 2 spheres radii

	if (distance < sumRadius) // have collision
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool PuckMan3D::PacManPowerUpOverlapTest(XMVECTOR s1Center, XMVECTOR s2Center)
{
	float s1Radius = MazeLoader::RADIUS_PAC_MAN;

	float s2Radius = MazeLoader::RADIUS_POWERUP;

	XMVECTOR d = s1Center - s2Center; // difference between the two spheres centers
	float distance = sqrt((d.m128_f32[0] * d.m128_f32[0]) + (d.m128_f32[1] * d.m128_f32[1]) + (d.m128_f32[2] * d.m128_f32[2])); //magnitude of the difference
	float sumRadius = s1Radius + s2Radius; //sum of the 2 spheres radii

	if (distance < sumRadius) // have collision
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool PuckMan3D::UpdateGroundCollision()
{
	XMVECTOR playerPos = mTestPlayer->GetPos();
	float terrainHeight = mTestTerrain->GetHeight(playerPos.m128_f32[0],
							playerPos.m128_f32[2]);
	//player is collided with the ground if the player's y is less than the terrain's y
	mSlope = terrainHeight - playerPos.m128_f32[1];
	if (mSlope > 0.05f)
	{
		return false;
	}
	if(playerPos.m128_f32[1] <= terrainHeight)
	{
		mTestPlayer->SetPos(XMVectorSet(playerPos.m128_f32[0], terrainHeight,
			playerPos.m128_f32[2], 1.0f));

		mTestPlayer->HitGround();
	}
	else
	{
		mTestPlayer->LeaveGround();
	}
}

void PuckMan3D::DrawParticles()
{
	XMVECTOR eyePos = mCam->GetPos();
	XMMATRIX proj = XMLoadFloat4x4(&mProj);
	XMMATRIX view = mCam->GetView();

	XMMATRIX vp = view * proj;

	mParticleEffect->SetPerFrameParams(eyePos);
	mParticleEffect->SetPerObjectParams(vp, mParticleTexture);

	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	md3dImmediateContext->IASetInputLayout(Vertex::GetParticleVertLayout());
	
	UINT stride = sizeof(Vertex::ParticleVertex);
	UINT offset = 0;

	md3dImmediateContext->IASetVertexBuffers(0, 1, &mParticleVB, &stride, &offset);
	
	md3dImmediateContext->OMSetDepthStencilState(mNoDepthDS, 0);
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	md3dImmediateContext->OMSetBlendState(mAdditiveBS, blendFactor, 0xffffffff);

	D3DX11_TECHNIQUE_DESC techDesc; 
	mParticleEffect->GetTech()->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		mParticleEffect->GetTech()->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->Draw(mParticles.size(), 0);
	}

	md3dImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff);
	md3dImmediateContext->OMSetDepthStencilState(NULL, 0);
}

XNA::OrientedBox* PuckMan3D::GetOrientedBox(FXMVECTOR extents, const GraphicalObject* obj)
{
	XNA::OrientedBox* ret = new XNA::OrientedBox();
	XMStoreFloat3(&ret->Extents, extents);
	XMStoreFloat3(&ret->Center, obj->GetPos());

	XMMATRIX rot = //XMMatrixIdentity();
		XMMatrixSet(
		obj->GetRight().m128_f32[0],
		obj->GetRight().m128_f32[1],
		obj->GetRight().m128_f32[2], 0.0f,
		obj->GetUp().m128_f32[0],
		obj->GetUp().m128_f32[1],
		obj->GetUp().m128_f32[2], 0.0f,
		obj->GetLook().m128_f32[0],
		obj->GetLook().m128_f32[1],
		obj->GetLook().m128_f32[2], 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	XMStoreFloat4(&ret->Orientation, XMQuaternionRotationMatrix(rot));

	return ret;
}

XNA::Sphere* PuckMan3D::GetBoundingSphere(const GraphicalObject* obj,
	float radius)
{
	XNA::Sphere* ret = new XNA::Sphere();
	XMStoreFloat3(&ret->Center, obj->GetPos());
	ret->Radius = radius;
	return ret;
}

XMVECTOR PuckMan3D::CylToCyl(FXMVECTOR c1Pos, float c1Rad, float c1Height,
	FXMVECTOR c2Pos, float c2Rad, float c2Height)
{
	XMVECTOR ret = XMVectorZero();
	//first lets do circle circle on xz plane
	XMVECTOR d = c2Pos - c1Pos;
	d.m128_f32[1] = 0.0f;
	d.m128_f32[3] = 0.0f;
	float distance = XMVector3Length(d).m128_f32[0];
	if (distance < c1Rad + c2Rad)
	{
		float diffY = c2Pos.m128_f32[1] - c1Pos.m128_f32[1];
		float halfHeights = c1Height / 2.0f + c2Height / 2.0f;
		if (diffY < halfHeights)
		{
			ret = d * ((c1Rad + c2Rad) - distance);
		}
	}

	return ret;
}

void PuckMan3D::BuildPuckMan()
{
	MazeLoader::InitialPosition pacPos = MazeLoader::GetInitialPos();
	MazeLoader::SetPacManPos(XMVectorSet(-12.0f, 0.75f, -17.0f, 0.0f), 1);
	MazeLoader::SetPacManPos(XMVectorSet(-9.5f, 0.75f, -17.0f, 0.0f), 2);
}

void PuckMan3D::ResetPuckMan()
{
	MazeLoader::InitialPosition pacPos = MazeLoader::GetInitialPos();
	MazeLoader::SetPacManPos(XMVectorSet(-12.0f, 0.75f, -17.0f, 0.0f), 1);
	MazeLoader::SetPacManPos(XMVectorSet(-9.5f, 0.75f, -17.0f, 0.0f), 2);
}

void PuckMan3D::BuildGhosts()
{
	MazeLoader::InitialPosition gPos = MazeLoader::GetInitialPos();
	mBlinky = new Blinky(XMVectorSet(gPos.blinky.x, gPos.blinky.y, gPos.blinky.z, 0.0f), XMVectorSet(3.75f, 0.0f, 3.75f, 0.0f), 0.75f);
	mInky = new Inky(XMVectorSet(gPos.inky.x, gPos.inky.y, gPos.inky.z, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), 0.75f);
	mPinky = new Pinky(XMVectorSet(gPos.pinky.x, gPos.pinky.y, gPos.pinky.z, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), 0.75f);
	mClyde = new Clyde(XMVectorSet(gPos.clyde.x, gPos.clyde.y, gPos.clyde.z, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), 0.75f);

	ResetGhosts();
}

void PuckMan3D::ResetGhosts()
{
	////Positioning the Ghost
	MazeLoader::InitialPosition gPos = MazeLoader::GetInitialPos();
	std::vector<MazeLoader::MazeElementSpecs> ghosts = MazeLoader::GetGhostData();
	
	mBlinky->setPos(XMVectorSet(gPos.blinky.x, gPos.blinky.y, gPos.blinky.z, 0.0f));
	mInky->setPos(XMVectorSet(gPos.inky.x, gPos.inky.y, gPos.inky.z, 0.0f));
	mPinky->setPos(XMVectorSet(gPos.pinky.x, gPos.pinky.y, gPos.pinky.z, 0.0f));
	mClyde->setPos(XMVectorSet(gPos.clyde.x, gPos.clyde.y, gPos.clyde.z, 0.0f));

	MazeLoader::SetGhostPos(XMVectorSet(mBlinky->getPos().x, mBlinky->getPos().y, mBlinky->getPos().z, 0.0f), 0);
	MazeLoader::SetGhostPos(XMVectorSet(mInky->getPos().x, mInky->getPos().y, mInky->getPos().z, 0.0f), 1);
	MazeLoader::SetGhostPos(XMVectorSet(mPinky->getPos().x, mPinky->getPos().y, mPinky->getPos().z, 0.0f), 2);
	MazeLoader::SetGhostPos(XMVectorSet(mClyde->getPos().x, mClyde->getPos().y, mClyde->getPos().z, 0.0f), 3);
}

void PuckMan3D::SetMaterials()
{

}

void PuckMan3D::BuildShapeGeometryBuffers()
{
	GeometryGenerator::MeshData fruit;
	GeometryGenerator::MeshData peach;
	GeometryGenerator::MeshData apple;
	GeometryGenerator::MeshData grapes;
}

void PuckMan3D::updateGhosts(float dt)
{
	mCurrentTime += dt;

	switch (ghostState)
	{
		//set the ghost to their default colours
	case GS_NORMAL:
		MazeLoader::SetGhostColour(Materials::BLINKY.Diffuse, 0);
		MazeLoader::SetGhostColour(Materials::INKY.Diffuse, 1);
		MazeLoader::SetGhostColour(Materials::PINKY.Diffuse, 2);
		MazeLoader::SetGhostColour(Materials::CLYDE.Diffuse, 3);
		break;

		//set the Ghost blue
	case GS_BLUE:
		if (mCurrentTime < mTotalTime)
		{
			MazeLoader::SetGhostColour(Materials::GHOSTFRIGHTENED.Diffuse, 0);
			MazeLoader::SetGhostColour(Materials::GHOSTFRIGHTENED.Diffuse, 1);
			MazeLoader::SetGhostColour(Materials::GHOSTFRIGHTENED.Diffuse, 2);
			MazeLoader::SetGhostColour(Materials::GHOSTFRIGHTENED.Diffuse, 3);
		
		}

		if (mCurrentTime >= mTotalTime)
		{
			ghostState = GhostState::GS_FLASHING;
			mCurrentTime = 0.0f;
			mNextTime = 0.3f;
			mTotalTime = 3.0f;
		}
		break;

		//Set the Ghost to be flashing once they are close to being in normalMode
	case GS_FLASHING:
		if (mCurrentTime >= mNextTime)
		{
			mNextTime += 0.3f;
			mIsBlue = !mIsBlue;
		}

		if (mIsBlue)
		{
			MazeLoader::SetGhostColour(Materials::GHOSTFRIGHTENED.Diffuse, 0);
			MazeLoader::SetGhostColour(Materials::GHOSTFRIGHTENED.Diffuse, 1);
			MazeLoader::SetGhostColour(Materials::GHOSTFRIGHTENED.Diffuse, 2);
			MazeLoader::SetGhostColour(Materials::GHOSTFRIGHTENED.Diffuse, 3);
		}
		else
		{
			MazeLoader::SetGhostColour(Materials::GHOSTFLASHING.Diffuse, 0);
			MazeLoader::SetGhostColour(Materials::GHOSTFLASHING.Diffuse, 1);
			MazeLoader::SetGhostColour(Materials::GHOSTFLASHING.Diffuse, 2);
			MazeLoader::SetGhostColour(Materials::GHOSTFLASHING.Diffuse, 3);
		}

		if (mCurrentTime >= mTotalTime)
		{
			mNextTime = 0.0f;
			mCurrentTime = 0.0f;
			mTotalTime = 0.0f;
			ghostState = GhostState::GS_NORMAL;
			powerUpActivated = false;
		}
		break;
	}
}

void PuckMan3D::resetGame()
{
	result = channel[6]->setPaused(true);
	ghostState = GhostState::GS_NORMAL;
	soundStates = SoundsState::SS_KA;
	fruitState = FruitState::FS_DEFAULT;
	mFacingState = FCS_DEFAULT;
	if (mGameState == GS_GAMEOVER)
	{
		mScore = 0;
		MazeLoader::ResetPacMan();
	}
	mPelletCounter = 0;
	powerUpActivated = false;
	mIsBlue = false;
	mIsMoving = false;
	mIsPlayerDead = false;
	mIsBeginningPlaying = false;
	ResetGhosts();
	MazeLoader::ResetPellets();
	MazeLoader::ResetPowerUps();
	MazeLoader::ResetPacManPosition();
}

void PuckMan3D::loadGhostDeathSFX()
{
	result = sys->createSound("Sounds/eatghost.wav", FMOD_DEFAULT, 0, &sound[0]);
	result = sound[0]->setMode(FMOD_LOOP_OFF);
}

void PuckMan3D::playGhostDeathSFX()
{
	bool isPlaying = false;

	if (channel[0] != NULL)
	{
		channel[0]->isPlaying(&isPlaying);
	}

	if (!isPlaying)
	{
		result = sys->playSound(sound[0], 0, false, &channel[0]);
		result = channel[0]->setChannelGroup(soundGroup);
		result = channel[0]->setPaused(false);
	}
}

void PuckMan3D::loadScaredGhostSFX()
{
	result = sys->createSound("Sounds/Frightened.wav", FMOD_DEFAULT, 0, &sound[1]);

	result = sound[1]->setMode(FMOD_LOOP_OFF);
}

void PuckMan3D::playScaredGhostSFX()
{
	bool isPlaying = false;

	if (channel[1] != NULL)
	{
		channel[1]->isPlaying(&isPlaying);
	}

	if (!isPlaying && powerUpActivated)
	{
		result = sys->playSound(sound[1], 0, false, &channel[1]);
		result = channel[1]->setChannelGroup(soundGroup);
		result = channel[1]->setPaused(false);
	}
}

void PuckMan3D::loadDeathSFX()
{
	result = sys->createSound("Sounds/death.wav", FMOD_DEFAULT, 0, &sound[2]);
	result = sound[2]->setMode(FMOD_LOOP_OFF);
}

void PuckMan3D::playDeathSFX()
{
	bool isPlaying = false;

	if (channel[2] != NULL)
	{
		channel[2]->isPlaying(&isPlaying);
	}

	//if (!isPlaying)
	//{
		result = sys->playSound(sound[2], 0, false, &channel[2]);
		result = channel[2]->setChannelGroup(soundGroup);
		result = channel[2]->setPaused(false);
	//}

}

void PuckMan3D::loadFruitSFX()
{
	result = sys->createSound("Sounds/eatfruit.wav", FMOD_DEFAULT, 0, &sound[3]);
	result = sound[3]->setMode(FMOD_LOOP_OFF);
}

void PuckMan3D::playFruitSFX()
{
	bool isPlaying = false;

	if (channel[4] != NULL)
	{
		channel[4]->isPlaying(&isPlaying);
	}

	if (!isPlaying)
	{
		result = sys->playSound(sound[3], 0, false, &channel[4]);
		result = channel[4]->setChannelGroup(soundGroup);
		result = channel[4]->setPaused(false);
	}
}

void PuckMan3D::loadExtraLifeSFX()
{
	result = sys->createSound("Sounds/extrapac.wav", FMOD_DEFAULT, 0, &sound[4]);
	result = sound[4]->setMode(FMOD_LOOP_OFF);
}

void PuckMan3D::playExtraLifeSFX()
{
	bool isPlaying = false;

	if (channel[4] != NULL)
	{
		channel[4]->isPlaying(&isPlaying);
	}

	if (!isPlaying)
	{
		result = sys->playSound(sound[4], 0, false, &channel[4]);
		result = channel[4]->setChannelGroup(soundGroup);
		result = channel[4]->setPaused(false);
	}
}

void PuckMan3D::loadBeginningSFX()
{
	result = sys->createSound("Sounds/beginning.wav", FMOD_DEFAULT, 0, &sound[5]);
	result = sound[5]->setMode(FMOD_LOOP_OFF);
}

void PuckMan3D::playBeginningSFX()
{
	bool isPlaying = false;

	if (channel[5] != NULL)
	{
		channel[5]->isPlaying(&isPlaying);
	}

	if (!isPlaying)
	{
		result = sys->playSound(sound[5], 0, false, &channel[5]);
		result = channel[5]->setChannelGroup(soundGroup);
		result = channel[5]->setPaused(false);		
	}
}

void PuckMan3D::loadSirenSFX()
{
	result = sys->createSound("Sounds/MainSiren.wav", FMOD_DEFAULT, 0, &sound[6]);
	result = sound[6]->setMode(FMOD_LOOP_NORMAL);
}

void PuckMan3D::playSirenSFX()
{
	bool isPlaying = false;

	if (channel[6] != NULL)
	{
		channel[6]->isPlaying(&isPlaying);
	}

	if (!isPlaying && !mIsPlayerDead)
	{
		result = sys->playSound(sound[6], 0, false, &channel[6]);
		result = channel[6]->setChannelGroup(soundGroup);
		result = channel[6]->setPaused(false);
	}
}

void PuckMan3D::loadWaSFX()
{
	result = sys->createSound("Sounds/Wa2.wav", FMOD_DEFAULT, 0, &sound[7]);
	result = sound[7]->setMode(FMOD_LOOP_OFF);
}

void PuckMan3D::playWaSFX()
{
	bool isPlaying = false;

	if (channel[3] != NULL)
	{
		channel[3]->isPlaying(&isPlaying);
	}

	//if (!isPlaying)
	//{
		result = sys->playSound(sound[7], 0, false, &channel[3]);
		result = channel[3]->setChannelGroup(soundGroup);
		result = channel[3]->setPaused(false);
		soundStates = SoundsState::SS_WA;
	//}
}

void PuckMan3D::loadKaSFX()
{
	result = sys->createSound("Sounds/Ka.wav", FMOD_DEFAULT, 0, &sound[8]);
	result = sound[8]->setMode(FMOD_LOOP_OFF);
}

void PuckMan3D::playKaSFX()
{
	bool isPlaying = false;

	if (channel[3] != NULL)
	{
		channel[3]->isPlaying(&isPlaying);
	}

	//if (!isPlaying)
	//{
		result = sys->playSound(sound[8], 0, false, &channel[3]);
		result = channel[3]->setChannelGroup(soundGroup);
		result = channel[3]->setPaused(false);
		soundStates = SoundsState::SS_KA;
	//}
}

void PuckMan3D::loadSystem()
{
	//Create a System object and initialize and set's up all audio based code

	result = FMOD::System_Create(&sys);

	result = sys->getVersion(&version);

	if (version < FMOD_VERSION)
	{
		OutputDebugString(L"FMOD lib version doesn't match header version");
	}

	result = sys->init(32, FMOD_INIT_NORMAL, extradriverdata);

	result = sys->createChannelGroup("SoundGroup", &soundGroup);

	result = sys->getMasterChannelGroup(&masterGroup);

	result = masterGroup->addGroup(soundGroup);
}

void PuckMan3D::updateStringStream()
{
	currScore.str("");
	currScore << mScore;
}



