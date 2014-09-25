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

	//void BuildPuckMaze();
	//void BuildPellets();
	void BuildPuckMan();
	void BuildGhosts();
	//void BuildPowerUps();
	void SetMaterials();
	void BuildShapeGeometryBuffers();

private:
	/*(struct PowerUp
	{
		XMFLOAT3 pos;

		PowerUp(FXMVECTOR pos)
		{
			XMStoreFloat3(&this->pos, pos);
		}
	};
	struct Pellet
	{
		XMFLOAT3 pos;
		XMFLOAT4X4 World;
		XMFLOAT4 Color;

		Pellet(FXMVECTOR pos)
		{
			XMStoreFloat3(&this->pos, pos);
		}
	};
	struct PacMan
	{
		XMFLOAT3 pos;
		XMFLOAT3 vel;

		PacMan(FXMVECTOR pos, FXMVECTOR vel)
		{
			XMStoreFloat3(&this->pos, pos);
			XMStoreFloat3(&this->vel, vel);
		}

	};*/
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

	/*struct AABox
	{
		XMFLOAT3 pos;
		float width; //x size
		float depth; //z size

		XMFLOAT3 min;
		XMFLOAT3 max;

		AABox(FXMVECTOR pos, float width, float depth)
		{
			XMStoreFloat3(&this->pos, pos);
			this->width = width;
			this->depth = depth;
			min.x = this->pos.x - (width / 2.0f);
			max.x = this->pos.x + (width / 2.0f);
			min.y = 0;
			max.y = 0.75f;
			min.z = this->pos.z - (depth / 2.0f);
			max.z = this->pos.z + (depth / 2.0f);
		}
	};*/

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
	float mSpeed;
	float fruitR = 0.60;
	float mNextTime = 0.0f;
	float mCurrentTime = 0.0f;
	float mTotalTime = 0.0f;
	int mLevelCounter;
	int mPelletCounter;

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
	//UINT mFruitIndexCount;

	GameTimer timer;
	GameTimer flashingTimer;

	//float mPacManR;
	//float mGhostR;
	//float mPelletR;
	//float mPowerUpR;

	/*Material mGridMat;
	Material mBoxMat;
	Material mPelletMat;
	Material mPowerUpMat;
	Material mPacManMat;
	Material mGhostMat;
	Material mPinkyMat;
	Material mInkyMat;
	Material mClydeMat;
	Material mCherry;
	Material mPeachMat;
	Material mAppleMat;
	Material mGrapesMat;*/

	// Define transformations from local spaces to world space.
	XMFLOAT4X4 mPelletWorld[240];
	XMFLOAT4X4 mPowerUpWorld[4];
	XMFLOAT4X4 mPacManWorld[3];
	XMFLOAT4X4 mGhostWorld[4];
	XMFLOAT4X4 mBoxWorld[55];
	XMFLOAT4X4 mGridWorld;
	XMFLOAT4X4 mFruitWorld[2];

	//std::vector<AABox> mBoxData;
	//std::vector<PacMan> mPacMan;
	//std::vector<Pellet> mPellet;
	//std::vector<PowerUp> mPowerUp;
	//std::vector<Fruit> mFruit;
	//std::vector<Fruit> mPeach;
	//std::vector<Fruit> mApple;
	//std::vector<Fruit> mGrapes;
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

	FacingState mFacingState = FCS_DEFAULT;

	bool isPlaying = false;
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
	: D3DApp(hInstance), mLitTexEffect(0), mMouseReleased(true), mCam(0), mLevelCounter(1), mTestPlayer(0), mTestTerrain(0),
mSkyBox(NULL), mParticleEffect(NULL), mIsKeyPressed(false), mSpeed(710.0f),
mCountPellets(0), mLitMatInstanceEffect(0)
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

	/*for (int i = 0; i < mFruit.size(); ++i)
	{
		XMStoreFloat4x4(&mFruitWorld[i], XMMatrixTranslation(mFruit[i].pos.x, mFruit[i].pos.y, mFruit[i].pos.z));
	}

	for (int i = 0; i < mPeach.size(); ++i)
	{
		XMStoreFloat4x4(&mFruitWorld[i], XMMatrixTranslation(mPeach[i].pos.x, mPeach[i].pos.y, mPeach[i].pos.z));
	}

	for (int i = 0; i < mApple.size(); ++i)
	{
		XMStoreFloat4x4(&mFruitWorld[i], XMMatrixTranslation(mApple[i].pos.x, mApple[i].pos.y, mApple[i].pos.z));
	}

	for (int i = 0; i < mGrapes.size(); ++i)
	{
		XMStoreFloat4x4(&mFruitWorld[i], XMMatrixTranslation(mGrapes[i].pos.x, mGrapes[i].pos.y, mGrapes[i].pos.z));
	}*/

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

	//if (mBarnProjectile)
	//	delete mBarnProjectile;

	//if (mFarmModel)
	//	delete mFarmModel;

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
	/* test code, test a point light out */
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

	//mSpotLight.pos = XMFLOAT3(mCam->GetPos().m128_f32[0], mCam->GetPos().m128_f32[1], mCam->GetPos().m128_f32[2]);
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

	//mLitMatEffect = new LitMatEffect();
	//mLitMatEffect->LoadEffect(L"FX/lighting.fx", md3dDevice);
	//Vertex::InitLitMatLayout(md3dDevice, mLitMatEffect->GetTech());

	mLitMatInstanceEffect = new LitMatEffect();
	mLitMatInstanceEffect->LoadEffect(L"FX/lightingInstanced.fx", md3dDevice);
	Vertex::InitLitMatInstanceLayout(md3dDevice, mLitMatInstanceEffect->GetTech());

	//mMazeModel = new BasicModel(md3dDevice, mLitMatEffect, "Mazes/mainLevel.txt");
	mMazeModelInstanced = new BasicModel(md3dDevice, mLitMatInstanceEffect, "Mazes/mainLevel.txt");

	//BuildPuckMaze();
	//BuildPellets();
	BuildPuckMan();
	BuildGhosts();
	//BuildPowerUps();
	SetMaterials();
	BuildShapeGeometryBuffers();

	//mMazeModel->GetMesh()->SetMaterial(mBoxMat);
	//mMazeModelInstanced->GetMesh()->SetMaterial(mBoxMat);
	mMazeModelInstanced->GetMesh()->SetMaterial(Materials::BOX);

	mLitTexEffect = new LitTexEffect();
	mLitTexEffect->LoadEffect(L"FX/lighting.fx", md3dDevice);

	mParticleEffect = new ParticleEffect();
	mParticleEffect->LoadEffect(L"FX/ParticleEffect.fx", md3dDevice);

	Vertex::InitLitTexLayout(md3dDevice, mLitTexEffect->GetTech());

	//XMVECTOR pos = XMVectorSet(100.0f, 30.0f, 475.0f, 0.0f);
	//XMVECTOR pos = XMVectorSet(0.0f, 30.0f, 250.0f, 0.0f);
	XMVECTOR pos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR look = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	//mMazeCharacter = new Character(pos, look, up, *mMazeModel);
	mMazeCharacterInstanced = new Character(pos, look, up, *mMazeModelInstanced);

	//mFarmModel = new BasicModel(md3dDevice, mLitTexEffect, "Models/farm.obj", false, true);
	//mMonsterModel = new BasicModel(md3dDevice, mLitTexEffect, "Models/troll2.obj", false, true);

	//mTestPlayer = new Player(pos,look, up, *mFarmModel, 100.0f, 200.0f, 1.0f);

	//mCam = new ThirdPersonCam(mTestPlayer, 8.0f, 3.0f, 3.0f);
	//mCam->Update();

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

	//mSkyBox = new SkyBox(md3dDevice, 1000.0f, L"Textures/TwilightCM.dds");

	//mBarnProjectile = new BasicModel(md3dDevice, mLitTexEffect, "Models/farm.obj", false, true);
	BuildParticleVB();

	D3DX11CreateShaderResourceViewFromFile(md3dDevice, L"Textures/TestAdditive.png", 0, 0, &mParticleTexture, 0);

	BuildBlendStates();
	BuildDSStates();

	// Test Barns
	//Character* newBarn = new Character(XMVectorSet(5.0f, 0.0f, 5.0f, 0.0f), XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), *mFarmModel); 
	//Character* newTroll = new Character(XMVectorSet(25.0f, 25.0f, 250.0f, 0.0f), XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), *mMonsterModel);

	//mTestChars.push_back(newTroll);

	ID3D11ShaderResourceView* font;
	D3DX11CreateShaderResourceViewFromFile(md3dDevice, L"Textures/font2.png", 0, 0, &font, 0);

	mFont = new FontRasterizer(m2DCam, XMLoadFloat4x4(&m2DProj), mLitTexEffect, 10, 10, font, md3dDevice);

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
		/*for (int j = 0; j < mParticles.size(); ++j)
		{
			//get our bounding sphere for the particle
			XNA::Sphere bs;
			bs.Center = mParticles[j].pos;
			bs.Radius = 1.0f;

			//get our oriented box for the char
			XNA::OrientedBox* bb = 
				GetOrientedBox(XMVectorSet(1.5f, 1.5f, 1.5f, 0.0f),
				mTestChars[i]);

			XMFLOAT3 dir;
			float overlap = XNA::OverlapSphereOrientedBox(&bs, bb, dir);
			if (overlap > 0.0f)
			{
				XMVECTOR particlePos = XMLoadFloat3(&bs.Center);
				XMVECTOR move = XMLoadFloat3(&dir) * overlap;
				particlePos = particlePos + move;
				XMStoreFloat3(&mParticles[j].pos, particlePos);
				//delete mTestChars[i];
				//mTestChars.erase(mTestChars.begin() + i);
				//break;
			}

			delete bb;


		}*/
	}
}

float timer = 0.0f;
void PuckMan3D::UpdateScene(float dt)
{
	UpdateKeyboardInput(dt);
	updateStringStream();

	mBlinky->Update();
	
	MazeLoader::SetGhostPos(XMVectorSet(mBlinky->getPos().x, mBlinky->getPos().y, mBlinky->getPos().z, 0.0f), 0);
	MazeLoader::SetGhostPos(XMVectorSet(mInky->getPos().x, mInky->getPos().y, mInky->getPos().z, 0.0f), 1);
	MazeLoader::SetGhostPos(XMVectorSet(mPinky->getPos().x, mPinky->getPos().y, mPinky->getPos().z, 0.0f), 2);
	MazeLoader::SetGhostPos(XMVectorSet(mClyde->getPos().x, mClyde->getPos().y, mClyde->getPos().z, 0.0f), 3);

	std::vector<MazeLoader::MazeElementSpecs> pacMans = MazeLoader::GetPacManData();
	XMVECTOR pos = XMLoadFloat3(&pacMans[0].pos);
	XMVECTOR vel = XMLoadFloat3(&pacMans[0].vel);

	PuckManSpeed();

	pos = pos + (vel * mSpeed * dt);

	MazeLoader::SetPacManPos(pos, 0);
	MazeLoader::SetPacManVel(vel, 0);

	//XMStoreFloat3(&pacMans[0].pos, pos);
	//XMStoreFloat3(&pacMans[0].vel, vel);

	//// Checking PacMan collision with maze
	MazeLoader::SetPacManPos(PacManAABoxOverLap(pos), 0);
	
	//XMStoreFloat3(&pacMans[0].pos, PacManAABoxOverLap(pos));

	std::vector<MazeLoader::MazeElementSpecs> ghosts = MazeLoader::GetGhostData();
	////Checking PacMan Collision with Ghost
	for (int i = 0; i < ghosts.size(); ++i)
	{
		XMVECTOR ghostPos = XMLoadFloat3(&ghosts[i].pos);

		if (PacManGhostOverlapTest(pos, ghostPos) == true)
		{
			if (!powerUpActivated)
			{
				//mPacMan.pop_back();
				MazeLoader::ErasePacMan(pacMans.size() - 1);
				MazeLoader::InitialPosition pacPos = MazeLoader::GetInitialPos();
				MazeLoader::SetPacManPos(XMVectorSet(pacPos.pacMan.x, pacPos.pacMan.y, pacPos.pacMan.z, 0.0f), 0);
				//mPacMan[0].pos.x = 0.0f;
				//mPacMan[0].pos.y = 0.75f;
				//mPacMan[0].pos.z = -8.5f;
				playDeathSFX();
				mIsPlayerDead = true;
				mIsMoving = false;
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

		if (PacManPelletOverlapTest(pos, pelPos) == true)
		{
			if (!isPlaying)
			{

			}

			if (soundStates == SoundsState::SS_KA)
			{
				playWaSFX();
			}
			else if (soundStates == SoundsState::SS_WA)
			{
				playKaSFX();
			}
			mPelletCounter++;
			MazeLoader::ErasePellet(i);
			mScore += 10;
			break;
			//--i;
		}

	}
	std::vector<MazeLoader::MazeElementSpecs> powerUps = MazeLoader::GetPowerUpData();
	////checking PacMan collision with PowerUps
	//// If collision is true remove the powerup.
	for (int i = 0; i < powerUps.size(); ++i)
	{
		XMVECTOR pUpPos = XMLoadFloat3(&powerUps[i].pos);

		if (PacManPowerUpOverlapTest(pos, pUpPos) == true)
		{
			powerUpActivated = true;
			ghostState = GhostState::GS_BLUE;
			mCurrentTime = 0.0f;
			mTotalTime = 3.0f;
			timer.Reset();
			timer.Start();
			MazeLoader::ErasePowerUp(i);
			mScore += 50;
			break;
			//--i;
		}
	}

	//// If collision is true remove the cherry.
	/*for (int i = 0; i < mFruit.size(); ++i)
	{
		XMVECTOR pUpPos = XMLoadFloat3(&mFruit[i].pos);

		if (PacManPowerUpOverlapTest(pos, pUpPos) == true)
		{
			playFruitSFX();
			mFruit.erase(mFruit.begin() + i);
			fruitState = FruitState::FS_DEFAULT;
			--i;
			break;
		}
	}

	for (int i = 0; i < mPeach.size(); ++i)
	{
		XMVECTOR pUpPos = XMLoadFloat3(&mPeach[i].pos);

		if (PacManPowerUpOverlapTest(pos, pUpPos) == true)
		{
			playFruitSFX();
			mPeach.erase(mPeach.begin() + i);
			fruitState = FruitState::FS_DEFAULT;
			--i;
			break;
		}
	}

	for (int i = 0; i < mApple.size(); ++i)
	{
		XMVECTOR pUpPos = XMLoadFloat3(&mApple[i].pos);

		if (PacManPowerUpOverlapTest(pos, pUpPos) == true)
		{
			playFruitSFX();
			mApple.erase(mApple.begin() + i);
			fruitState = FruitState::FS_DEFAULT;
			--i;
			break;
		}
	}

	for (int i = 0; i < mGrapes.size(); ++i)
	{
		XMVECTOR pUpPos = XMLoadFloat3(&mGrapes[i].pos);

		if (PacManPowerUpOverlapTest(pos, pUpPos) == true)
		{
			playFruitSFX();
			mGrapes.erase(mGrapes.begin() + i);
			fruitState = FruitState::FS_DEFAULT;
			--i;
			break;
		}
	}*/

	////PacMan Tunnel Check

	if (pacMans[0].pos.x < -14.0f)
	{
		pos.m128_f32[0] = 14.0f;
		MazeLoader::SetPacManPos(pos, 0);
		//mPacMan[0].pos.x = 14.0f;
	}
	if (pacMans[0].pos.x > 14.0f)
	{
		pos.m128_f32[0] = -14.0f;
		MazeLoader::SetPacManPos(pos, 0);
		//mPacMan[0].pos.x = -14.0f;
	}

	if (mIsMoving)
	{//check if the player is moving to determine when to play the background sound
		playSirenSFX();
		result = channel[6]->setPaused(false);
	}
	else if (mIsPlayerDead)
	{
		powerUpActivated = false;
		mIsBlue = false;
		mIsMoving = false;
		result = channel[6]->setPaused(true);
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

	//reset board if all pellets are gone
	if (pellets.size() == 0 && powerUps.size() == 0)
	{
		resetGame();
		mLevelCounter++;
	}
	updateGhosts(dt);

	/*if (mPelletCounter == 5 && fruitState == FruitState::FS_DEFAULT)
	{//randomly pick a fruit to draw when enough pellets are removed. 
		randNumber = rg(8) + 1;
		//position fruit
		if (randNumber == 1 || randNumber == 2)
		{
			mFruit.push_back(Fruit(XMVectorSet(0.0f, 0.75f, -2.5f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)));
			fruitState = FruitState::FS_FRUIT;
		}
		else if (randNumber == 3 || randNumber == 4)
		{
			mPeach.push_back(Fruit(XMVectorSet(1.0f, 0.75f, -2.5f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)));
			fruitState = FruitState::FS_FRUIT;
		}
		else if (randNumber == 5 || randNumber == 6)
		{
			mApple.push_back(Fruit(XMVectorSet(2.0f, 0.75f, -2.5f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)));
			fruitState = FruitState::FS_FRUIT;
		}
		else if (randNumber == 7 || randNumber == 8)
		{
			mGrapes.push_back(Fruit(XMVectorSet(3.0f, 0.75f, -2.5f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)));
			fruitState = FruitState::FS_FRUIT;
		}
	}
	if (mPelletCounter == 170)
	{
		randNumber = rg(8) + 1;
		//position fruit
		if (randNumber == 1 || randNumber == 2)
		{
			mFruit.push_back(Fruit(XMVectorSet(0.0f, 0.75f, -2.5f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)));
			fruitState = FruitState::FS_FRUIT;
		}
		else if (randNumber == 3 || randNumber == 4)
		{
			mPeach.push_back(Fruit(XMVectorSet(1.0f, 0.75f, -2.5f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)));
			fruitState = FruitState::FS_FRUIT;
		}
		else if (randNumber == 5 || randNumber == 6)
		{
			mApple.push_back(Fruit(XMVectorSet(2.0f, 0.75f, -2.5f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)));
			fruitState = FruitState::FS_FRUIT;
		}
		else if (randNumber == 7 || randNumber == 8)
		{
			mGrapes.push_back(Fruit(XMVectorSet(3.0f, 0.75f, -2.5f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)));
			fruitState = FruitState::FS_FRUIT;
		}
	}*/

	//XMVECTOR camPos = mCam->GetPos();

	/*camPos = XMVectorSet(camPos.m128_f32[0], mTestTerrain->GetHeight(camPos.m128_f32[0],
										   camPos.m128_f32[2]) + 4.0f,
										   camPos.m128_f32[2], 1.0f);

	mCam->SetPos(camPos);*/

	float eyeOffset = 25.0f;
	//float eyeOffset = 0.0f;

	// Camera X, Y, Z Positioning.
	float x = pacMans[0].pos.x;
	float y = pacMans[0].pos.y + eyeOffset;
	float z = pacMans[0].pos.z - eyeOffset;
	//float y = mPacMan[0].pos.y + eyeOffset;
	//float z = mPacMan[0].pos.z - eyeOffset;

	mEyePosW = XMFLOAT3(x, y, z);

	// Build the view matrix.
	XMVECTOR eyePos = XMVectorSet(x, y, z, 1.0f);
	XMVECTOR target = XMVectorSet(pacMans[0].pos.x, pacMans[0].pos.y + 5, pacMans[0].pos.z, 0.0f);//XMVectorZero();
	//XMVECTOR target = XMVectorZero();
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
		dataView[mCountPellets++] = { pellets[i].world, pellets[i].colour };
	}
	md3dImmediateContext->Unmap(mMazeModelInstanced->GetMesh()->GetInstanceBPellets(), 0);

	md3dImmediateContext->Map(mMazeModelInstanced->GetMesh()->GetInstanceBPowerUps(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	dataView = reinterpret_cast<Vertex::InstancedData*>(mappedData.pData);
	mCountPowerUps = 0;
	for (UINT i = 0; i < powerUps.size(); ++i)
	{
		dataView[mCountPowerUps++] = { powerUps[i].world, powerUps[i].colour };
	}
	md3dImmediateContext->Unmap(mMazeModelInstanced->GetMesh()->GetInstanceBPowerUps(), 0);

	md3dImmediateContext->Map(mMazeModelInstanced->GetMesh()->GetInstanceBPacMans(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	dataView = reinterpret_cast<Vertex::InstancedData*>(mappedData.pData);
	mCountPacMans = 0;
	for (UINT i = 0; i < pacMans.size(); ++i)
	{
		dataView[mCountPacMans++] = { pacMans[i].world, pacMans[i].colour };
	}
	md3dImmediateContext->Unmap(mMazeModelInstanced->GetMesh()->GetInstanceBPacMans(), 0);

	md3dImmediateContext->Map(mMazeModelInstanced->GetMesh()->GetInstanceBGhosts(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	dataView = reinterpret_cast<Vertex::InstancedData*>(mappedData.pData);
	mCountGhosts = 0;
	for (UINT i = 0; i < ghosts.size(); ++i)
	{
		switch (i)
		{
		case 0:
		//	ghosts[i].colour = mGhostMat.Diffuse;
			ghosts[i].colour = Materials::BLINKY.Diffuse;
			break;
		case 1:
			//ghosts[i].colour = mInkyMat.Diffuse;
			ghosts[i].colour = Materials::INKY.Diffuse;
			break;
		case 2:
			//ghosts[i].colour = mPinkyMat.Diffuse;
			ghosts[i].colour = Materials::PINKY.Diffuse;
			break;
		case 3:
			//ghosts[i].colour = mClydeMat.Diffuse;
			ghosts[i].colour = Materials::CLYDE.Diffuse;
			break;
		}
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
	//mCam->SetFacing(XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));
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

	/*mLitMatEffect->SetPerObjectParams(world, worldInvTranspose, worldViewProj, mGridMat);
	mLitMatEffect->Draw(md3dImmediateContext, mShapesVB, mShapesIB, mGridIndexOffset, mGridIndexCount, mGridVertexOffset);

	int vertexOffset = 0;
	int indexOffset = 0;

	//// Draw the Pac-Man Maze with boxes
	for (int i = 0; i < mBoxData.size(); ++i)
	{
		world = XMMatrixTranslation(mBoxData[i].pos.x, mBoxData[i].pos.y, mBoxData[i].pos.z);
		worldInvTranspose = MathHelper::InverseTranspose(world);
		worldViewProj = world*view*proj;

		mLitMatEffect->SetPerObjectParams(world, worldInvTranspose, worldViewProj, mBoxMat);

		mLitMatEffect->Draw(md3dImmediateContext, mShapesVB, mShapesIB, indexOffset, 36, vertexOffset);
		vertexOffset += 24;
		indexOffset += 36;
	}
	//}
	//// Draw the Pellets.
	for (int i = 0; i < mPellet.size(); ++i)
	{
		world = XMMatrixTranslation(mPellet[i].pos.x, mPellet[i].pos.y, mPellet[i].pos.z);
		worldInvTranspose = MathHelper::InverseTranspose(world);
		worldViewProj = world*view*proj;

		mLitMatEffect->SetPerObjectParams(world, worldInvTranspose, worldViewProj, mPelletMat);
		mLitMatEffect->Draw(md3dImmediateContext, mShapesVB, mShapesIB, mPelletIndexOffset, mPelletIndexCount, mPelletVertexOffset);
	}

	//// Draw the PowerUps
	for (int i = 0; i < mPowerUp.size(); ++i)
	{
		world = XMMatrixTranslation(mPowerUp[i].pos.x, mPowerUp[i].pos.y, mPowerUp[i].pos.z);
		worldInvTranspose = MathHelper::InverseTranspose(world);
		worldViewProj = world*view*proj;

		mLitMatEffect->SetPerObjectParams(world, worldInvTranspose, worldViewProj, mPowerUpMat);
		mLitMatEffect->Draw(md3dImmediateContext, mShapesVB, mShapesIB, mPowerUpIndexOffset, mPowerUpIndexCount, mPowerUpVertexOffset);
	}

	//// Draw the PacMans.
	for (int i = 0; i < mPacMan.size(); ++i)
	{
		world = XMMatrixTranslation(mPacMan[i].pos.x, mPacMan[i].pos.y, mPacMan[i].pos.z);
		worldInvTranspose = MathHelper::InverseTranspose(world);
		worldViewProj = world*view*proj;

		mLitMatEffect->SetPerObjectParams(world, worldInvTranspose, worldViewProj, mPacManMat);
		mLitMatEffect->Draw(md3dImmediateContext, mShapesVB, mShapesIB, mPacManIndexOffset, mPacManIndexCount, mPacManVertexOffset);
	}

	//// Draw the Ghosts
	for (int i = 0; i < mGhost.size(); ++i)
	{
		world = XMMatrixTranslation(mGhost[i].pos.x, mGhost[i].pos.y, mGhost[i].pos.z);
		worldInvTranspose = MathHelper::InverseTranspose(world);
		worldViewProj = world*view*proj;

		mLitMatEffect->SetPerObjectParams(world, worldInvTranspose, worldViewProj, mGhostMat);
		mLitMatEffect->Draw(md3dImmediateContext, mShapesVB, mShapesIB, mGhostIndexOffset, mGhostIndexCount, mGhostVertexOffset);
	}

	for (int i = 0; i < mPinky.size(); ++i)
	{
		world = XMMatrixTranslation(mPinky[i].pos.x, mPinky[i].pos.y, mPinky[i].pos.z);
		worldInvTranspose = MathHelper::InverseTranspose(world);
		worldViewProj = world*view*proj;

		mLitMatEffect->SetPerObjectParams(world, worldInvTranspose, worldViewProj, mPinkyMat);
		mLitMatEffect->Draw(md3dImmediateContext, mShapesVB, mShapesIB, mGhostIndexOffset, mGhostIndexCount, mGhostVertexOffset);
	}

	for (int i = 0; i < mInky.size(); ++i)
	{
		world = XMMatrixTranslation(mInky[i].pos.x, mInky[i].pos.y, mInky[i].pos.z);
		worldInvTranspose = MathHelper::InverseTranspose(world);
		worldViewProj = world*view*proj;

		mLitMatEffect->SetPerObjectParams(world, worldInvTranspose, worldViewProj, mInkyMat);
		mLitMatEffect->Draw(md3dImmediateContext, mShapesVB, mShapesIB, mGhostIndexOffset, mGhostIndexCount, mGhostVertexOffset);
	}

	for (int i = 0; i < mClyde.size(); ++i)
	{
		world = XMMatrixTranslation(mClyde[i].pos.x, mClyde[i].pos.y, mClyde[i].pos.z);
		worldInvTranspose = MathHelper::InverseTranspose(world);
		worldViewProj = world*view*proj;

		mLitMatEffect->SetPerObjectParams(world, worldInvTranspose, worldViewProj, mClydeMat);
		mLitMatEffect->Draw(md3dImmediateContext, mShapesVB, mShapesIB, mGhostIndexOffset, mGhostIndexCount, mGhostVertexOffset);
	}*/

	world = XMLoadFloat4x4(&mGridWorld);
	worldInvTranspose = MathHelper::InverseTranspose(world);
	worldViewProj = world*view*proj;
	/*
	// Draw the Fruit.
	for (int i = 0; i < mFruit.size(); ++i)
	{
		world = XMMatrixTranslation(mFruit[i].pos.x, mFruit[i].pos.y, mFruit[i].pos.z);
		worldInvTranspose = MathHelper::InverseTranspose(world);
		worldViewProj = world*view*proj;

		Effect::BasicFX->SetWorld(world);
		Effect::BasicFX->SetWorldInvTranspose(worldInvTranspose);
		Effect::BasicFX->SetWorldViewProj(worldViewProj);
		Effect::BasicFX->SetMaterial(mCherry);


	//	activeTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mFruitIndexCount, mFruitIndexOffset, mFruitVertexOffset);
	}
	for (int i = 0; i < mPeach.size(); ++i)
	{
		world = XMMatrixTranslation(mPeach[i].pos.x, mPeach[i].pos.y, mPeach[i].pos.z);
		worldInvTranspose = MathHelper::InverseTranspose(world);
		worldViewProj = world*view*proj;

		Effect::BasicFX->SetWorld(world);
		Effect::BasicFX->SetWorldInvTranspose(worldInvTranspose);
		Effect::BasicFX->SetWorldViewProj(worldViewProj);
		Effect::BasicFX->SetMaterial(mPeachMat);


		//activeTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mFruitIndexCount, mFruitIndexOffset, mFruitVertexOffset);
	}
	for (int i = 0; i < mApple.size(); ++i)
	{
		world = XMMatrixTranslation(mApple[i].pos.x, mApple[i].pos.y, mApple[i].pos.z);
		worldInvTranspose = MathHelper::InverseTranspose(world);
		worldViewProj = world*view*proj;

		Effect::BasicFX->SetWorld(world);
		Effect::BasicFX->SetWorldInvTranspose(worldInvTranspose);
		Effect::BasicFX->SetWorldViewProj(worldViewProj);
		Effect::BasicFX->SetMaterial(mAppleMat);


		//activeTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mFruitIndexCount, mFruitIndexOffset, mFruitVertexOffset);
	}
	for (int i = 0; i < mGrapes.size(); ++i)
	{
		world = XMMatrixTranslation(mGrapes[i].pos.x, mGrapes[i].pos.y, mGrapes[i].pos.z);
		worldInvTranspose = MathHelper::InverseTranspose(world);
		worldViewProj = world*view*proj;

		Effect::BasicFX->SetWorld(world);
		Effect::BasicFX->SetWorldInvTranspose(worldInvTranspose);
		Effect::BasicFX->SetWorldViewProj(worldViewProj);
		Effect::BasicFX->SetMaterial(mGrapesMat);


	//	activeTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mFruitIndexCount, mFruitIndexOffset, mFruitVertexOffset);
	}*/

	MazeLoader::OffsetsCountsMazeElements oc = MazeLoader::GetOffsetsCounts();

	md3dImmediateContext->IASetInputLayout(Vertex::GetNormalMatVertInstanceLayout());
	mLitMatInstanceEffect->SetEffectTech("LitMatTechInstanced");
	Material boxColour = Materials::BOX;
	//mLitMatInstanceEffect->SetPerObjectParams(world, worldInvTranspose, worldViewProj, viewProj, mBoxMat);
	mLitMatInstanceEffect->SetPerObjectParams(world, worldInvTranspose, worldViewProj, viewProj, boxColour);
	mLitMatInstanceEffect->DrawInstanced(md3dImmediateContext, mMazeModelInstanced->GetMesh()->GetVB(), mMazeModelInstanced->GetMesh()->GetIB(), mMazeModelInstanced->GetMesh()->GetInstanceBWalls(),
		mCountWalls, oc.walls.indexOffset, oc.walls.indexCount);
	//mLitMatInstanceEffect->Draw(md3dImmediateContext, mMazeModelInstanced->GetMesh()->GetVB(), mMazeModelInstanced->GetMesh()->GetIB(), oc.walls.indexOffset, oc.walls.indexCount);

	md3dImmediateContext->IASetInputLayout(Vertex::GetNormalMatVertInstanceLayout());
	mLitMatInstanceEffect->SetEffectTech("LitMatTechInstanced");
	Material pelletColour = Materials::PELLET;
	mLitMatInstanceEffect->SetPerObjectParams(world, worldInvTranspose, worldViewProj, viewProj, pelletColour);
	//mLitMatInstanceEffect->SetPerObjectParams(world, worldInvTranspose, worldViewProj, viewProj, mPelletMat);
	mLitMatInstanceEffect->DrawInstanced(md3dImmediateContext, mMazeModelInstanced->GetMesh()->GetVB(), mMazeModelInstanced->GetMesh()->GetIB(), mMazeModelInstanced->GetMesh()->GetInstanceBPellets(),
		mCountPellets, oc.pellets.indexOffset, oc.pellets.indexCount);

	md3dImmediateContext->IASetInputLayout(Vertex::GetNormalMatVertInstanceLayout());
	mLitMatInstanceEffect->SetEffectTech("LitMatTechInstanced");
	Material powerUpColour = Materials::POWERUP;
	mLitMatInstanceEffect->SetPerObjectParams(world, worldInvTranspose, worldViewProj, viewProj, powerUpColour);
	//mLitMatInstanceEffect->SetPerObjectParams(world, worldInvTranspose, worldViewProj, viewProj, mPowerUpMat);
	mLitMatInstanceEffect->DrawInstanced(md3dImmediateContext, mMazeModelInstanced->GetMesh()->GetVB(), mMazeModelInstanced->GetMesh()->GetIB(), mMazeModelInstanced->GetMesh()->GetInstanceBPowerUps(),
		mCountPowerUps, oc.powerUps.indexOffset, oc.powerUps.indexCount);
	//mLitMatInstanceEffect->Draw(md3dImmediateContext, mMazeModelInstanced->GetMesh()->GetVB(), mMazeModelInstanced->GetMesh()->GetIB(), oc.powerUps.indexOffset, oc.powerUps.indexCount);

	//for (int i = 0; i < mPacMan.size(); ++i)
	//{
		//world = XMMatrixTranslation(mPacMan[i].pos.x, mPacMan[i].pos.y, mPacMan[i].pos.z);
	std::vector<MazeLoader::MazeElementSpecs> pacMans = MazeLoader::GetPacManData();
	//world = XMMatrixTranslation(pacMans[0].pos.x, pacMans[0].pos.y, pacMans[0].pos.z);
	world = XMLoadFloat4x4(&mGridWorld);
	worldInvTranspose = MathHelper::InverseTranspose(world);
		worldViewProj = world*view*proj;
		Material pacManColour = Materials::PACMAN;
		mLitMatInstanceEffect->SetPerObjectParams(world, worldInvTranspose, worldViewProj, viewProj, pacManColour);
		//mLitMatInstanceEffect->SetPerObjectParams(world, worldInvTranspose, worldViewProj, viewProj, mPacManMat);
		mLitMatInstanceEffect->DrawInstanced(md3dImmediateContext, mMazeModelInstanced->GetMesh()->GetVB(), mMazeModelInstanced->GetMesh()->GetIB(), mMazeModelInstanced->GetMesh()->GetInstanceBPacMans(),
			mCountPacMans, oc.pacMan.indexOffset, oc.pacMan.indexCount);
		//mLitMatInstanceEffect->Draw(md3dImmediateContext, mMazeModelInstanced->GetMesh()->GetVB(), mMazeModelInstanced->GetMesh()->GetIB(), oc.pacMan.indexOffset, oc.pacMan.indexCount);
	//}

	world = XMMatrixTranslation(mBlinky->getPos().x, mBlinky->getPos().y, mBlinky->getPos().z);
	world = XMLoadFloat4x4(&mGridWorld);
	worldInvTranspose = MathHelper::InverseTranspose(world);
	worldViewProj = world*view*proj;
	Material blinkyColour = Materials::BLINKY;
	mLitMatInstanceEffect->SetPerObjectParams(world, worldInvTranspose, worldViewProj, viewProj, blinkyColour);
	//mLitMatInstanceEffect->SetPerObjectParams(world, worldInvTranspose, worldViewProj, viewProj, mGhostMat);
	mLitMatInstanceEffect->DrawInstanced(md3dImmediateContext, mMazeModelInstanced->GetMesh()->GetVB(), mMazeModelInstanced->GetMesh()->GetIB(), mMazeModelInstanced->GetMesh()->GetInstanceBGhosts(),
		mCountGhosts, oc.ghosts.indexOffset, oc.ghosts.indexCount);
	//mLitMatInstanceEffect->Draw(md3dImmediateContext, mMazeModelInstanced->GetMesh()->GetVB(), mMazeModelInstanced->GetMesh()->GetIB(), oc.ghosts.indexOffset, oc.pacMan.indexCount);

	world = XMMatrixTranslation(mInky->getPos().x, mInky->getPos().y, mInky->getPos().z);
	world = XMLoadFloat4x4(&mGridWorld);
	worldInvTranspose = MathHelper::InverseTranspose(world);
	worldViewProj = world*view*proj;
	Material inkyColour = Materials::INKY;
	mLitMatInstanceEffect->SetPerObjectParams(world, worldInvTranspose, worldViewProj, viewProj, inkyColour);
	//mLitMatInstanceEffect->SetPerObjectParams(world, worldInvTranspose, worldViewProj, viewProj, mInkyMat);
	mLitMatInstanceEffect->DrawInstanced(md3dImmediateContext, mMazeModelInstanced->GetMesh()->GetVB(), mMazeModelInstanced->GetMesh()->GetIB(), mMazeModelInstanced->GetMesh()->GetInstanceBGhosts(),
		1, oc.ghosts.indexOffset, oc.ghosts.indexCount);
	//mLitMatInstanceEffect->Draw(md3dImmediateContext, mMazeModelInstanced->GetMesh()->GetVB(), mMazeModelInstanced->GetMesh()->GetIB(), oc.ghosts.indexOffset + oc.pacMan.indexCount, oc.pacMan.indexCount);

	world = XMMatrixTranslation(mPinky->getPos().x, mPinky->getPos().y, mPinky->getPos().z);
	world = XMLoadFloat4x4(&mGridWorld);
	worldInvTranspose = MathHelper::InverseTranspose(world);
	worldViewProj = world*view*proj;
	Material pinkyColour = Materials::PINKY;
	mLitMatInstanceEffect->SetPerObjectParams(world, worldInvTranspose, worldViewProj, viewProj, pinkyColour);
	//mLitMatInstanceEffect->SetPerObjectParams(world, worldInvTranspose, worldViewProj, viewProj, mPinkyMat);
	mLitMatInstanceEffect->DrawInstanced(md3dImmediateContext, mMazeModelInstanced->GetMesh()->GetVB(), mMazeModelInstanced->GetMesh()->GetIB(), mMazeModelInstanced->GetMesh()->GetInstanceBGhosts(),
		1, oc.ghosts.indexOffset, oc.ghosts.indexCount);
	//mLitMatInstanceEffect->Draw(md3dImmediateContext, mMazeModelInstanced->GetMesh()->GetVB(), mMazeModelInstanced->GetMesh()->GetIB(), oc.ghosts.indexOffset + (oc.pacMan.indexCount * 2), oc.pacMan.indexCount);

	world = XMMatrixTranslation(mClyde->getPos().x, mClyde->getPos().y, mClyde->getPos().z);
	world = XMLoadFloat4x4(&mGridWorld);
	worldInvTranspose = MathHelper::InverseTranspose(world);
	worldViewProj = world*view*proj;
	Material clydeColour = Materials::CLYDE;
	mLitMatInstanceEffect->SetPerObjectParams(world, worldInvTranspose, worldViewProj, viewProj, clydeColour);
	//mLitMatInstanceEffect->SetPerObjectParams(world, worldInvTranspose, worldViewProj, viewProj, mClydeMat);
	mLitMatInstanceEffect->DrawInstanced(md3dImmediateContext, mMazeModelInstanced->GetMesh()->GetVB(), mMazeModelInstanced->GetMesh()->GetIB(), mMazeModelInstanced->GetMesh()->GetInstanceBGhosts(),
		1, oc.ghosts.indexOffset, oc.ghosts.indexCount);
	//mLitMatInstanceEffect->Draw(md3dImmediateContext, mMazeModelInstanced->GetMesh()->GetVB(), mMazeModelInstanced->GetMesh()->GetIB(), oc.ghosts.indexOffset + (oc.pacMan.indexCount * 3), oc.pacMan.indexCount);
	

	//mMazeCharacter->Draw(md3dImmediateContext, vp);

	//mSkyBox->Draw(md3dImmediateContext, vp, mCam->GetPos());

	/*mTestTerrain->Draw(md3dImmediateContext, vp);

	mTestPlayer->Draw(md3dImmediateContext, vp);

	for (int i = 0; i < mTestChars.size(); ++i)
	{
		mTestChars[i]->Draw(md3dImmediateContext, vp);
	}

	for (int i = 0; i < mProjectiles.size(); ++i)
	{
		mProjectiles[i]->Draw(md3dImmediateContext, vp);
	}

	vp = XMMatrixIdentity();
	proj = XMLoadFloat4x4(&m2DProj);
	view = m2DCam->GetView();

	vp = vp * view * proj;

	mTestTerrain->Draw(md3dImmediateContext, vp);
	DrawParticles();*/
	
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	md3dImmediateContext->OMSetBlendState(mTransparentBS, blendFactor, 0xffffffff);
	md3dImmediateContext->OMSetDepthStencilState(mFontDS, 0);

	std::stringstream os;
	os << "(" << pacMans[0].pos.x << ", " << pacMans[0].pos.z << ")" << "    " << mSpeed;
	mLitTexEffect->SetPerFrameParams(ambient, eyePos, mPointLights[0], mSpotLight);
	mFont->DrawFont(md3dImmediateContext, XMVectorSet(10.0f, 500.0f, 0.0f, 0.0f), 50, 75, 10, os.str());
	mFont->DrawFont(md3dImmediateContext, XMVectorSet(10.0f, 620.0f, 0.0f, 0.0f), 50, 75, 25, "Score: " + currScore.str());
	md3dImmediateContext->OMSetDepthStencilState(0, 0);
	md3dImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff);

	HR(mSwapChain->Present(1, 0));
}

void PuckMan3D::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	/*if ((btnState & MK_RBUTTON) != 0)
	{
		Projectile* newProjectile = new Projectile(mTestPlayer->GetLook() * 200.0f,
			mTestPlayer->GetPos() + mTestPlayer->GetUp() * 2.0f,
			mTestPlayer->GetLook(), mTestPlayer->GetUp(), 1.0f, *mBarnProjectile);

		mProjectiles.push_back(newProjectile);
	}*/

	/*if ((btnState & MK_LBUTTON) != 0)
	{
		TestParticle newParticle;
		XMStoreFloat3(&newParticle.pos, mTestPlayer->GetPos() + XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
		XMStoreFloat3(&newParticle.vel, mTestPlayer->GetLook() * 0.1f);
		newParticle.size.x = 2.0f;
		newParticle.size.y = 2.0f;
		mParticles.push_back(newParticle);
	}*/

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

		//mTestPlayer->Pitch(dy);
		//mTestPlayer->YawGlobal(dx);


		mLastMousePos.x = x;
		mLastMousePos.y = y;
	}
}

/*void PuckMan3D::UpdateKeyboardInput(float dt)
{
	if( GetAsyncKeyState('W') & 0x8000)
	{
		if(GetAsyncKeyState(VK_LSHIFT) & 0x8000)
		{
			mTestPlayer->MoveLook(mTestPlayer->GetSprintSpeed() * dt);
		}
		else
		{
			mTestPlayer->MoveLook(mTestPlayer->GetSpeed() * dt);
		}
	}
	if( GetAsyncKeyState('S') & 0x8000 )
		mTestPlayer->MoveLook(-mTestPlayer->GetSpeed()*dt);

	if( GetAsyncKeyState('A') & 0x8000 )
		mTestPlayer->MoveStrafe(-mTestPlayer->GetSpeed()*dt);

	if( GetAsyncKeyState('D') & 0x8000 )
		mTestPlayer->MoveStrafe(mTestPlayer->GetSpeed()*dt);

	if( GetAsyncKeyState(VK_SPACE) & 0x8000)
		mTestPlayer->Jump();

}*/

void PuckMan3D::UpdateKeyboardInput(float dt)
{
	std::vector<MazeLoader::MazeElementSpecs> pacMans = MazeLoader::GetPacManData();
	XMVECTOR vel;

<<<<<<< HEAD
	mIsKeyPressed = false;
	vel.m128_f32[0] = 0.0f;
	vel.m128_f32[1] = 0.0f;
	vel.m128_f32[2] = 0.0f;
	mFacingState = FCS_DEFAULT;
=======
>>>>>>> 11136601c3192845ad235668c741cbfe79fd37f6

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
	/*else
	{
	mIsKeyPressed = false;
	vel.m128_f32[0] = 0.0f;
	vel.m128_f32[1] = 0.0f;
	vel.m128_f32[2] = 0.0f;
	mFacingState = FCS_DEFAULT;
	}*/

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
	int transZ = MazeLoader::GetMazeHeight() - (int)floor(posZ + (MazeLoader::GetMazeHeight() * 0.5)) - 1; //invert the z
	int row = transZ;
	int col = transX;

	switch (mFacingState)
	{
	case FCS_FORWARD:
		if (MazeLoader::IsPellet(row - 1, col))
		{
			mSpeed = PUCKMAN_SPEED * 0.71f;
		}
		else
		{
			mSpeed = PUCKMAN_SPEED * 0.8f;
		}
		break;
	case FCS_BACKWARD:
		if (MazeLoader::IsPellet(row + 1, col))
		{
			mSpeed = PUCKMAN_SPEED * 0.71f;
		}
		else
		{
			mSpeed = PUCKMAN_SPEED * 0.8f;
		}
		break;
	case FCS_RIGHT:
		if (MazeLoader::IsPellet(row, col + 1))
		{
			mSpeed = PUCKMAN_SPEED * 0.71f;
		}
		else
		{
			mSpeed = PUCKMAN_SPEED * 0.8f;
		}
		break;
	case FCS_LEFT:
		if (MazeLoader::IsPellet(row, col - 1))
		{
			mSpeed = PUCKMAN_SPEED * 0.71f;
		}
		else
		{
			mSpeed = PUCKMAN_SPEED * 0.8f;
		}
		break;
	case FCS_DEFAULT:
		break;
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

/*void PuckMan3D::BuildPuckMaze()
{
	////positioning of the 55 boxes to create the Pac-Man Maze
	mBoxData.push_back(AABox(XMVectorSet(0.0f, 0.375f, -15.75f, 0.0f), 28.0f, 0.5f));
	mBoxData.push_back(AABox(XMVectorSet(-13.75f, 0.375f, -10.0f, 0.0f), 0.5f, 11.0f));
	mBoxData.push_back(AABox(XMVectorSet(-12.5f, 0.375f, -10.0f, 0.0f), 2.0f, 1.0f));
	mBoxData.push_back(AABox(XMVectorSet(-11.25f, 0.375f, -4.25f, 0.0f), 5.5f, 0.5f));
	mBoxData.push_back(AABox(XMVectorSet(-8.75f, 0.375f, -2.5f, 0.0f), 0.5f, 3.0f));
	mBoxData.push_back(AABox(XMVectorSet(-11.25f, 0.375f, -0.75f, 0.0f), 5.5f, 0.5f));
	mBoxData.push_back(AABox(XMVectorSet(-11.25f, 0.375f, 1.75f, 0.0f), 5.5f, 0.5f));
	mBoxData.push_back(AABox(XMVectorSet(-8.75f, 0.375f, 3.5f, 0.0f), 0.5f, 3.0f));
	mBoxData.push_back(AABox(XMVectorSet(-11.25f, 0.375f, 5.25f, 0.0f), 5.5f, 0.5f));
	mBoxData.push_back(AABox(XMVectorSet(-13.75f, 0.375f, 10.0f, 0.0f), 0.5f, 10.0f));
	mBoxData.push_back(AABox(XMVectorSet(0.0f, 0.375f, 14.75f, 0.0f), 28.0f, 0.5f));
	mBoxData.push_back(AABox(XMVectorSet(13.75f, 0.375f, 10.0f, 0.0f), 0.5f, 10.0f));
	mBoxData.push_back(AABox(XMVectorSet(11.25f, 0.375f, 5.25f, 0.0f), 5.5f, 0.5f));
	mBoxData.push_back(AABox(XMVectorSet(8.75f, 0.375f, 3.5f, 0.0f), 0.5f, 3.0f));
	mBoxData.push_back(AABox(XMVectorSet(11.25f, 0.375f, 1.75f, 0.0f), 5.5f, 0.5f));
	mBoxData.push_back(AABox(XMVectorSet(11.25f, 0.375f, -0.75f, 0.0f), 5.5f, 0.5f));
	mBoxData.push_back(AABox(XMVectorSet(8.75f, 0.375f, -2.5f, 0.0f), 0.5f, 3.0f));
	mBoxData.push_back(AABox(XMVectorSet(11.25f, 0.375f, -4.25f, 0.0f), 5.5f, 0.5f));
	mBoxData.push_back(AABox(XMVectorSet(13.75f, 0.375f, -10.0f, 0.0f), 0.5f, 11.0f));
	mBoxData.push_back(AABox(XMVectorSet(12.5f, 0.375f, -10.0f, 0.0f), 2.0f, 1.0f));
	mBoxData.push_back(AABox(XMVectorSet(7.0f, 0.375f, -13.0f, 0.0f), 9.0f, 1.0f));
	mBoxData.push_back(AABox(XMVectorSet(-7.0f, 0.375f, -13.0f, 0.0f), 9.0f, 1.0f));
	mBoxData.push_back(AABox(XMVectorSet(-6.0f, 0.375f, -11.0f, 0.0f), 1.0f, 3.0f));
	mBoxData.push_back(AABox(XMVectorSet(0.0f, 0.375f, -12.0f, 0.0f), 1.0f, 3.0f));
	mBoxData.push_back(AABox(XMVectorSet(6.0f, 0.375f, -11.0f, 0.0f), 1.0f, 3.0f));
	mBoxData.push_back(AABox(XMVectorSet(0.0f, 0.375f, -10.0f, 0.0f), 7.0f, 1.0f));
	mBoxData.push_back(AABox(XMVectorSet(-9.0f, 0.375f, -9.0f, 0.0f), 1.0f, 3.0f));
	mBoxData.push_back(AABox(XMVectorSet(-10.0f, 0.375f, -7.0f, 0.0f), 3.0f, 1.0f));
	mBoxData.push_back(AABox(XMVectorSet(-4.5f, 0.375f, -7.0f, 0.0f), 4.0f, 1.0f));
	mBoxData.push_back(AABox(XMVectorSet(0.0f, 0.375f, -6.0f, 0.0f), 1.0f, 3.0f));
	mBoxData.push_back(AABox(XMVectorSet(4.5f, 0.375f, -7.0f, 0.0f), 4.0f, 1.0f));
	mBoxData.push_back(AABox(XMVectorSet(9.0f, 0.375f, -9.0f, 0.0f), 1.0f, 3.0f));
	mBoxData.push_back(AABox(XMVectorSet(10.0f, 0.375f, -7.0f, 0.0f), 3.0f, 1.0f));
	mBoxData.push_back(AABox(XMVectorSet(6.0f, 0.375f, -2.5f, 0.0f), 1.0f, 4.0f));
	mBoxData.push_back(AABox(XMVectorSet(0.0f, 0.375f, -4.0f, 0.0f), 7.0f, 1.0f));
	mBoxData.push_back(AABox(XMVectorSet(-6.0f, 0.375f, -2.5f, 0.0f), 1.0f, 4.0f));
	mBoxData.push_back(AABox(XMVectorSet(0.0f, 0.375f, -1.25f, 0.0f), 7.0f, 0.5f));
	mBoxData.push_back(AABox(XMVectorSet(-3.25f, 0.375f, 0.5f, 0.0f), 0.5f, 3.0f));
	mBoxData.push_back(AABox(XMVectorSet(-2.25f, 0.375f, 2.25f, 0.0f), 2.5f, 0.5f));
	mBoxData.push_back(AABox(XMVectorSet(0.0f, 0.375f, 2.25f, 0.0f), 2.0f, 0.5f));
	mBoxData.push_back(AABox(XMVectorSet(2.25f, 0.375f, 2.25f, 0.0f), 2.5f, 0.5f));
	mBoxData.push_back(AABox(XMVectorSet(3.25f, 0.375f, 0.5f, 0.0f), 0.5f, 3.0f));
	mBoxData.push_back(AABox(XMVectorSet(6.0f, 0.375f, 5.0f, 0.0f), 1.0f, 7.0f));
	mBoxData.push_back(AABox(XMVectorSet(4.0f, 0.375f, 5.0f, 0.0f), 3.0f, 1.0f));
	mBoxData.push_back(AABox(XMVectorSet(0.0f, 0.375f, 6.0f, 0.0f), 1.0f, 3.0f));
	mBoxData.push_back(AABox(XMVectorSet(-4.0f, 0.375f, 5.0f, 0.0f), 3.0f, 1.0f));
	mBoxData.push_back(AABox(XMVectorSet(-6.0f, 0.375f, 5.0f, 0.0f), 1.0f, 7.0f));
	mBoxData.push_back(AABox(XMVectorSet(-10.0f, 0.375f, 8.0f, 0.0f), 3.0f, 1.0f));
	mBoxData.push_back(AABox(XMVectorSet(0.0f, 0.375f, 8.0f, 0.0f), 7.0f, 1.0f));
	mBoxData.push_back(AABox(XMVectorSet(10.0f, 0.375f, 8.0f, 0.0f), 3.0f, 1.0f));
	mBoxData.push_back(AABox(XMVectorSet(10.0f, 0.375f, 11.5f, 0.0f), 3.0f, 2.0f));
	mBoxData.push_back(AABox(XMVectorSet(4.5f, 0.375f, 11.5f, 0.0f), 4.0f, 2.0f));
	mBoxData.push_back(AABox(XMVectorSet(0.0f, 0.375f, 12.5f, 0.0f), 1.0f, 4.0f));
	mBoxData.push_back(AABox(XMVectorSet(-4.5f, 0.375f, 11.5f, 0.0f), 4.0f, 2.0f));
	mBoxData.push_back(AABox(XMVectorSet(-10.0f, 0.375f, 11.5f, 0.0f), 3.0f, 2.0f));

	for (int i = 0; i < mBoxData.size(); ++i)
	{
		XMStoreFloat4x4(&mBoxWorld[i], XMMatrixTranslation(mBoxData[i].pos.x, mBoxData[i].pos.y, mBoxData[i].pos.z));
	}
}*/

/*void PuckMan3D::BuildPellets()
{
	////Positioning the Pellets
	////Bottom Row
	for (int i = 0; i < 26; ++i)
	{
		mPellet.push_back(Pellet(XMVectorSet(-12.5f + i, 0.75f, -14.5f, 0.0f)));
	}
	////Second Row
	mPellet.push_back(Pellet(XMVectorSet(-12.5f, 0.75f, -13.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(-1.5f, 0.75f, -13.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(1.5f, 0.75f, -13.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(12.5f, 0.75f, -13.5f, 0.0f)));

	for (int i = 0; i < 29; ++i)
	{
		XMStoreFloat4x4(&mPelletWorld[i], XMMatrixTranslation(mPellet[i].pos.x, mPellet[i].pos.y, mPellet[i].pos.z));
	}
}*/

void PuckMan3D::BuildPuckMan()
{
	////Positioning the PacMans
	//mPacMan.push_back(PacMan(XMVectorSet(0.0f, 0.75f, -8.5f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)));
	MazeLoader::InitialPosition pacPos = MazeLoader::GetInitialPos();
	//std::vector<MazeLoader::MazeElementSpecs> pacMans = MazeLoader::GetPacManData();
	MazeLoader::SetPacManPos(XMVectorSet(-12.0f, 0.75f, -17.0f, 0.0f), 1);
	MazeLoader::SetPacManPos(XMVectorSet(-9.5f, 0.75f, -17.0f, 0.0f), 2);

	/*mPacMan.push_back(PacMan(XMVectorSet(pacPos.pacMan.x, pacPos.pacMan.y, -pacPos.pacMan.z, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)));
	mPacMan.push_back(PacMan(XMVectorSet(-12.0f, 0.75f, -17.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)));
	mPacMan.push_back(PacMan(XMVectorSet(-9.5f, 0.75f, -17.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)));

	for (int i = 0; i < mPacMan.size(); ++i)
	{
		XMStoreFloat4x4(&mPacManWorld[i], XMMatrixTranslation(mPacMan[i].pos.x, mPacMan[i].pos.y, mPacMan[i].pos.z));
	}*/
}

void PuckMan3D::BuildGhosts()
{
	////Positioning the Ghost
	MazeLoader::InitialPosition gPos = MazeLoader::GetInitialPos();
	std::vector<MazeLoader::MazeElementSpecs> ghosts = MazeLoader::GetGhostData();
	ghosts[0].world._41 = gPos.blinky.x;
	ghosts[0].world._43 = gPos.blinky.z;
	ghosts[1].world._41 = gPos.inky.x;
	ghosts[1].world._43 = gPos.inky.z;
	ghosts[2].world._41 = gPos.pinky.x;
	ghosts[2].world._43 = gPos.pinky.z;
	ghosts[3].world._41 = gPos.clyde.x;
	ghosts[3].world._43 = gPos.clyde.z;
	
	//mGhost.push_back(Ghost(XMVectorSet(gPos.blinky.x, gPos.blinky.y, gPos.blinky.z, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)));
	//mInky.push_back(Ghost(XMVectorSet(gPos.inky.x, gPos.inky.y, gPos.inky.z, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)));
	//mPinky.push_back(Ghost(XMVectorSet(gPos.pinky.x, gPos.pinky.y, gPos.pinky.z, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)));
	//mClyde.push_back(Ghost(XMVectorSet(gPos.clyde.x, gPos.clyde.y, gPos.clyde.z, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)));
	mBlinky = new Blinky(XMVectorSet(gPos.blinky.x, gPos.blinky.y, gPos.blinky.z, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), 0.75f);
	mInky = new Inky(XMVectorSet(gPos.inky.x, gPos.inky.y, gPos.inky.z, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), 0.75f);
	mPinky = new Pinky(XMVectorSet(gPos.pinky.x, gPos.pinky.y, gPos.pinky.z, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), 0.75f);
	mClyde = new Clyde(XMVectorSet(gPos.clyde.x, gPos.clyde.y, gPos.clyde.z, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), 0.75f);

	/*mGhost.push_back(Ghost(XMVectorSet(0.0f, 0.75f, 3.5f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)));
	mPinky.push_back(Ghost(XMVectorSet(0.0f, 0.75f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)));
	mInky.push_back(Ghost(XMVectorSet(-2.0f, 0.75f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)));
	mClyde.push_back(Ghost(XMVectorSet(2.0f, 0.75f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)));*/
}

/*void PuckMan3D::BuildPowerUps()
{
	////Positioning the PowerUps
	mPowerUp.push_back(PowerUp(XMVectorSet(-12.5f, 0.75f, -8.5f, 0.0f)));
	mPowerUp.push_back(PowerUp(XMVectorSet(-12.5f, 0.75f, 11.5f, 0.0f)));
	mPowerUp.push_back(PowerUp(XMVectorSet(12.5f, 0.75f, -8.5f, 0.0f)));
	mPowerUp.push_back(PowerUp(XMVectorSet(12.5f, 0.75f, 11.5f, 0.0f)));

	for (int i = 0; i < mPowerUp.size(); ++i)
	{
		XMStoreFloat4x4(&mPowerUpWorld[i], XMMatrixTranslation(mPowerUp[i].pos.x, mPowerUp[i].pos.y, mPowerUp[i].pos.z));
	}
}*/

void PuckMan3D::SetMaterials()
{
	/*mGridMat.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mGridMat.Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mGridMat.Specular = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);

	mPelletMat.Ambient = XMFLOAT4(1.0f, 0.72f, 0.68f, 1.0f);
	mPelletMat.Diffuse = XMFLOAT4(1.0f, 0.72f, 0.68f, 1.0f);
	mPelletMat.Specular = XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f);

	mPowerUpMat.Ambient = XMFLOAT4(1.0f, 0.9f, 0.9f, 1.0f);
	mPowerUpMat.Diffuse = XMFLOAT4(1.0f, 0.9f, 0.9f, 1.0f);
	mPowerUpMat.Specular = XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f);

	mPacManMat.Ambient = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	mPacManMat.Diffuse = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	mPacManMat.Specular = XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f);

	mGhostMat.Ambient = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	mGhostMat.Diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	mGhostMat.Specular = XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f);

	mPinkyMat.Ambient = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	mPinkyMat.Diffuse = XMFLOAT4(255.0f, 0.0f, 255.0f, 1.0f);
	mPinkyMat.Specular = XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f);

	mInkyMat.Ambient = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	mInkyMat.Diffuse = XMFLOAT4(0.0f, 105.0f, 225.0f, 1.0f);
	mInkyMat.Specular = XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f);

	mClydeMat.Ambient = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	mClydeMat.Diffuse = XMFLOAT4(1.0f, 0.45f, 0.001f, 1.0f);
	mClydeMat.Specular = XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f);

	mCherry.Ambient = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	mCherry.Diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	mCherry.Specular = XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f);

	mPeachMat.Ambient = XMFLOAT4(0.9f, 0.7f, 0.6f, 1.0f);
	mPeachMat.Diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	mPeachMat.Specular = XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f);

	mAppleMat.Ambient = XMFLOAT4(0.7f, 1.0f, 0.0f, 1.0f);
	mAppleMat.Diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	mAppleMat.Specular = XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f);

	mGrapesMat.Ambient = XMFLOAT4(0.1f, 0.0f, 1.0f, 1.0f);
	mGrapesMat.Diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	mGrapesMat.Specular = XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f);

	mBoxMat.Ambient = XMFLOAT4(0.12f, 0.12f, 0.6f, 1.0f);
	mBoxMat.Diffuse = XMFLOAT4(0.12f, 0.12f, 0.6f, 1.0f);
	mBoxMat.Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 16.0f);*/
}

void PuckMan3D::BuildShapeGeometryBuffers()
{
	// The blocks to make the Pac-Man maze
	/*GeometryGenerator::MeshData box1;
	GeometryGenerator::MeshData box2;
	GeometryGenerator::MeshData box3;
	GeometryGenerator::MeshData box4;
	GeometryGenerator::MeshData box5;
	GeometryGenerator::MeshData box6;
	GeometryGenerator::MeshData box7;
	GeometryGenerator::MeshData box8;
	GeometryGenerator::MeshData box9;
	GeometryGenerator::MeshData box10;
	GeometryGenerator::MeshData box11;
	GeometryGenerator::MeshData box12;
	GeometryGenerator::MeshData box13;
	GeometryGenerator::MeshData box14;
	GeometryGenerator::MeshData box15;
	GeometryGenerator::MeshData box16;
	GeometryGenerator::MeshData box17;
	GeometryGenerator::MeshData box18;
	GeometryGenerator::MeshData box19;
	GeometryGenerator::MeshData box20;
	GeometryGenerator::MeshData box21;
	GeometryGenerator::MeshData box22;
	GeometryGenerator::MeshData box23;
	GeometryGenerator::MeshData box24;
	GeometryGenerator::MeshData box25;
	GeometryGenerator::MeshData box26;
	GeometryGenerator::MeshData box27;
	GeometryGenerator::MeshData box28;
	GeometryGenerator::MeshData box29;
	GeometryGenerator::MeshData box30;
	GeometryGenerator::MeshData box31;
	GeometryGenerator::MeshData box32;
	GeometryGenerator::MeshData box33;
	GeometryGenerator::MeshData box34;
	GeometryGenerator::MeshData box35;
	GeometryGenerator::MeshData box36;
	GeometryGenerator::MeshData box37;
	GeometryGenerator::MeshData box38;
	GeometryGenerator::MeshData box39;
	GeometryGenerator::MeshData box40;
	GeometryGenerator::MeshData box41;
	GeometryGenerator::MeshData box42;
	GeometryGenerator::MeshData box43;
	GeometryGenerator::MeshData box44;
	GeometryGenerator::MeshData box45;
	GeometryGenerator::MeshData box46;
	GeometryGenerator::MeshData box47;
	GeometryGenerator::MeshData box48;
	GeometryGenerator::MeshData box49;
	GeometryGenerator::MeshData box50;
	GeometryGenerator::MeshData box51;
	GeometryGenerator::MeshData box52;
	GeometryGenerator::MeshData box53;
	GeometryGenerator::MeshData box54;
	GeometryGenerator::MeshData box55;

	GeometryGenerator::MeshData grid;
	GeometryGenerator::MeshData pellet;
	GeometryGenerator::MeshData powerUp;
	GeometryGenerator::MeshData pacMan;
	GeometryGenerator::MeshData ghost;*/
	GeometryGenerator::MeshData fruit;
	GeometryGenerator::MeshData peach;
	GeometryGenerator::MeshData apple;
	GeometryGenerator::MeshData grapes;


	// Dimensions the blocks for the Pac-Man maze
	/*GeometryGenerator geoGen;
	geoGen.CreateBox(28.0f, 0.75f, 0.5f, box1);
	//AABox aaBox1;
	//aaBox1.
	geoGen.CreateBox(0.5f, 0.75f, 11.0f, box2);
	geoGen.CreateBox(2.0f, 0.75f, 1.0f, box3);
	geoGen.CreateBox(5.5f, 0.75f, 0.5f, box4);
	geoGen.CreateBox(0.5f, 0.75f, 3.0f, box5);
	geoGen.CreateBox(5.5f, 0.75f, 0.5f, box6);
	geoGen.CreateBox(5.5f, 0.75f, 0.5f, box7);
	geoGen.CreateBox(0.5f, 0.75f, 3.0f, box8);
	geoGen.CreateBox(5.5f, 0.75f, 0.5f, box9);
	geoGen.CreateBox(0.5f, 0.75f, 10.0f, box10);
	geoGen.CreateBox(28.0f, 0.75f, 0.5f, box11);
	geoGen.CreateBox(0.5f, 0.75f, 10.0f, box12);
	geoGen.CreateBox(5.5f, 0.75f, 0.5f, box13);
	geoGen.CreateBox(0.5f, 0.75f, 3.0f, box14);
	geoGen.CreateBox(5.5f, 0.75f, 0.5f, box15);
	geoGen.CreateBox(5.5f, 0.75f, 0.5f, box16);
	geoGen.CreateBox(0.5f, 0.75f, 3.0f, box17);
	geoGen.CreateBox(5.5f, 0.75f, 0.5f, box18);
	geoGen.CreateBox(0.5f, 0.75f, 11.0f, box19);
	geoGen.CreateBox(2.0f, 0.75f, 1.0f, box20);
	geoGen.CreateBox(9.0f, 0.75f, 1.0f, box21);
	geoGen.CreateBox(9.0f, 0.75f, 1.0f, box22);
	geoGen.CreateBox(1.0f, 0.75f, 3.0f, box23);
	geoGen.CreateBox(1.0f, 0.75f, 3.0f, box24);
	geoGen.CreateBox(1.0f, 0.75f, 3.0f, box25);
	geoGen.CreateBox(7.0f, 0.75f, 1.0f, box26);
	geoGen.CreateBox(1.0f, 0.75f, 3.0f, box27);
	geoGen.CreateBox(3.0f, 0.75f, 1.0f, box28);
	geoGen.CreateBox(4.0f, 0.75f, 1.0f, box29);
	geoGen.CreateBox(1.0f, 0.75f, 3.0f, box30);
	geoGen.CreateBox(4.0f, 0.75f, 1.0f, box31);
	geoGen.CreateBox(1.0f, 0.75f, 3.0f, box32);
	geoGen.CreateBox(3.0f, 0.75f, 1.0f, box33);
	geoGen.CreateBox(1.0f, 0.75f, 4.0f, box34);
	geoGen.CreateBox(7.0f, 0.75f, 1.0f, box35);
	geoGen.CreateBox(1.0f, 0.75f, 4.0f, box36);
	geoGen.CreateBox(7.0f, 0.75f, 0.5f, box37);
	geoGen.CreateBox(0.5f, 0.75f, 3.0f, box38);
	geoGen.CreateBox(2.5f, 0.75f, 0.5f, box39);
	geoGen.CreateBox(2.0f, 0.75f, 0.5f, box40);
	geoGen.CreateBox(2.5f, 0.75f, 0.5f, box41);
	geoGen.CreateBox(0.5f, 0.75f, 3.0f, box42);
	geoGen.CreateBox(1.0f, 0.75f, 7.0f, box43);
	geoGen.CreateBox(3.0f, 0.75f, 1.0f, box44);
	geoGen.CreateBox(1.0f, 0.75f, 3.0f, box45);
	geoGen.CreateBox(3.0f, 0.75f, 1.0f, box46);
	geoGen.CreateBox(1.0f, 0.75f, 7.0f, box47);
	geoGen.CreateBox(3.0f, 0.75f, 1.0f, box48);
	geoGen.CreateBox(7.0f, 0.75f, 1.0f, box49);
	geoGen.CreateBox(3.0f, 0.75f, 1.0f, box50);
	geoGen.CreateBox(3.0f, 0.75f, 2.0f, box51);
	geoGen.CreateBox(4.0f, 0.75f, 2.0f, box52);
	geoGen.CreateBox(1.0f, 0.75f, 4.0f, box53);
	geoGen.CreateBox(4.0f, 0.75f, 2.0f, box54);
	geoGen.CreateBox(3.0f, 0.75f, 2.0f, box55);
	//floor of the Pac-Man Grid
	geoGen.CreateGrid(280.0f, 360.0f, 37, 29, grid);

	// Pellets of the Pac-Man grid
	geoGen.CreateSphere(MazeLoader::RADIUS_PELLET, 10, 10, pellet);

	// PowerUps of the Pac-Man grid
	geoGen.CreateSphere(MazeLoader::RADIUS_POWERUP, 10, 10, powerUp);

	// Pac-Man
	geoGen.CreateSphere(MazeLoader::RADIUS_PAC_MAN, 10, 10, pacMan);

	// Ghost
	geoGen.CreateSphere(MazeLoader::RADIUS_GHOST, 10, 10, ghost);*/

	// Fruit
	//geoGen.CreateSphere(fruitR, 10, 10, fruit);

	// peach
	//geoGen.CreateSphere(fruitR, 10, 10, peach);

	// apple
	//geoGen.CreateSphere(fruitR, 10, 10, apple);

	// grapes
	//geoGen.CreateSphere(fruitR, 10, 10, grapes);

	// Cache the vertex offsets to each object in the concatenated vertex buffer.
	/*mBoxVertexOffset = 0;
	mGridVertexOffset = box1.Vertices.size() + box2.Vertices.size() + box3.Vertices.size() + box4.Vertices.size() + box5.Vertices.size()
		+ box6.Vertices.size() + box7.Vertices.size() + box8.Vertices.size() + box9.Vertices.size() + box10.Vertices.size()
		+ box11.Vertices.size() + box12.Vertices.size() + box13.Vertices.size() + box14.Vertices.size() + box15.Vertices.size()
		+ box16.Vertices.size() + box17.Vertices.size() + box18.Vertices.size() + box19.Vertices.size() + box20.Vertices.size()
		+ box21.Vertices.size() + box22.Vertices.size() + box23.Vertices.size() + box24.Vertices.size() + box25.Vertices.size()
		+ box26.Vertices.size() + box27.Vertices.size() + box28.Vertices.size() + box29.Vertices.size() + box30.Vertices.size()
		+ box31.Vertices.size() + box32.Vertices.size() + box33.Vertices.size() + box34.Vertices.size() + box35.Vertices.size()
		+ box36.Vertices.size() + box37.Vertices.size() + box38.Vertices.size() + box39.Vertices.size() + box40.Vertices.size()
		+ box41.Vertices.size() + box42.Vertices.size() + box43.Vertices.size() + box44.Vertices.size() + box45.Vertices.size()
		+ box46.Vertices.size() + box47.Vertices.size() + box48.Vertices.size() + box49.Vertices.size() + box50.Vertices.size()
		+ box51.Vertices.size() + box52.Vertices.size() + box53.Vertices.size() + box54.Vertices.size() + box55.Vertices.size();

	mPelletVertexOffset = mGridVertexOffset + grid.Vertices.size();
	mPacManVertexOffset = mPelletVertexOffset + pellet.Vertices.size();
	mPowerUpVertexOffset = mPacManVertexOffset + pacMan.Vertices.size();
	mGhostVertexOffset = mPowerUpVertexOffset + powerUp.Vertices.size();
	//mFruitVertexOffset = mGhostVertexOffset + ghost.Vertices.size();


	// Cache the index count of each object.
	mBoxIndexCount = box1.Indices.size() + box2.Indices.size() + box3.Indices.size() + box4.Indices.size() + box5.Indices.size()
		+ box6.Indices.size() + box7.Indices.size() + box8.Indices.size() + box9.Indices.size() + box10.Indices.size()
		+ box11.Indices.size() + box12.Indices.size() + box13.Indices.size() + box14.Indices.size() + box15.Indices.size()
		+ box16.Indices.size() + box17.Indices.size() + box18.Indices.size() + box19.Indices.size() + box20.Indices.size()
		+ box21.Indices.size() + box22.Indices.size() + box23.Indices.size() + box24.Indices.size() + box25.Indices.size()
		+ box26.Indices.size() + box27.Indices.size() + box28.Indices.size() + box29.Indices.size() + box30.Indices.size()
		+ box31.Indices.size() + box32.Indices.size() + box33.Indices.size() + box34.Indices.size() + box35.Indices.size()
		+ box36.Indices.size() + box37.Indices.size() + box38.Indices.size() + box39.Indices.size() + box40.Indices.size()
		+ box41.Indices.size() + box42.Indices.size() + box43.Indices.size() + box44.Indices.size() + box45.Indices.size()
		+ box46.Indices.size() + box47.Indices.size() + box48.Indices.size() + box49.Indices.size() + box50.Indices.size()
		+ box51.Indices.size() + box52.Indices.size() + box53.Indices.size() + box54.Indices.size() + box55.Indices.size();

	mGridIndexCount = grid.Indices.size();
	mPelletIndexCount = pellet.Indices.size();
	mPacManIndexCount = pacMan.Indices.size();
	mPowerUpIndexCount = powerUp.Indices.size();
	mGhostIndexCount = ghost.Indices.size();
	//mFruitIndexCount = fruit.Indices.size();

	// Cache the starting index for each object in the concatenated index buffer.
	mBoxIndexOffset = 0;
	mGridIndexOffset = mBoxIndexCount;
	mPelletIndexOffset = mGridIndexOffset + mGridIndexCount;
	mPacManIndexOffset = mPelletIndexOffset + mPacManIndexCount;
	mPowerUpIndexOffset = mPacManIndexOffset + mPacManIndexCount;
	mGhostIndexOffset = mPowerUpIndexOffset + mPowerUpIndexCount;
//	mFruitIndexOffset = mGhostIndexOffset + mGhostIndexCount;

	UINT totalVertexCount = box1.Vertices.size() + box2.Vertices.size() + box3.Vertices.size() + box4.Vertices.size() + box5.Vertices.size()
		+ box6.Vertices.size() + box7.Vertices.size() + box8.Vertices.size() + box9.Vertices.size() + box10.Vertices.size()
		+ box11.Vertices.size() + box12.Vertices.size() + box13.Vertices.size() + box14.Vertices.size() + box15.Vertices.size()
		+ box16.Vertices.size() + box17.Vertices.size() + box18.Vertices.size() + box19.Vertices.size() + box20.Vertices.size()
		+ box21.Vertices.size() + box22.Vertices.size() + box23.Vertices.size() + box24.Vertices.size() + box25.Vertices.size()
		+ box26.Vertices.size() + box27.Vertices.size() + box28.Vertices.size() + box29.Vertices.size() + box30.Vertices.size()
		+ box31.Vertices.size() + box32.Vertices.size() + box33.Vertices.size() + box34.Vertices.size() + box35.Vertices.size()
		+ box36.Vertices.size() + box37.Vertices.size() + box38.Vertices.size() + box39.Vertices.size() + box40.Vertices.size()
		+ box41.Vertices.size() + box42.Vertices.size() + box43.Vertices.size() + box44.Vertices.size() + box45.Vertices.size()
		+ box46.Vertices.size() + box47.Vertices.size() + box48.Vertices.size() + box49.Vertices.size() + box50.Vertices.size()
		+ box51.Vertices.size() + box52.Vertices.size() + box53.Vertices.size() + box54.Vertices.size() + box55.Vertices.size()
		+ grid.Vertices.size() + pellet.Vertices.size() + pacMan.Vertices.size() + powerUp.Vertices.size() + ghost.Vertices.size() /*+ fruit.Vertices.size()*/;


	/*UINT totalIndexCount =
		mBoxIndexCount +
		mGridIndexCount +
		mPelletIndexCount +
		mPacManIndexCount +
		mPowerUpIndexCount +
		mGhostIndexCount;
		//mFruitIndexCount;


	//
	// Extract the vertex elements we are interested in and pack the
	// vertices of all the meshes into one vertex buffer.
	//

	std::vector<Vertex::NormalTexVertex> vertices(totalVertexCount);

	UINT k = 0;
	for (int i = 0; i < box1.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box1.Vertices[i].Position;
		vertices[k].normal = box1.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box2.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box2.Vertices[i].Position;
		vertices[k].normal = box2.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box3.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box3.Vertices[i].Position;
		vertices[k].normal = box3.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box4.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box4.Vertices[i].Position;
		vertices[k].normal = box4.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box5.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box5.Vertices[i].Position;
		vertices[k].normal = box5.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box6.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box6.Vertices[i].Position;
		vertices[k].normal = box6.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box7.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box7.Vertices[i].Position;
		vertices[k].normal = box7.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box8.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box8.Vertices[i].Position;
		vertices[k].normal = box8.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box9.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box9.Vertices[i].Position;
		vertices[k].normal = box9.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box10.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box10.Vertices[i].Position;
		vertices[k].normal = box10.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box11.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box11.Vertices[i].Position;
		vertices[k].normal = box11.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box12.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box12.Vertices[i].Position;
		vertices[k].normal = box12.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box13.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box13.Vertices[i].Position;
		vertices[k].normal = box13.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box14.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box14.Vertices[i].Position;
		vertices[k].normal = box14.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box15.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box15.Vertices[i].Position;
		vertices[k].normal = box15.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box16.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box16.Vertices[i].Position;
		vertices[k].normal = box16.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box17.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box17.Vertices[i].Position;
		vertices[k].normal = box17.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box18.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box18.Vertices[i].Position;
		vertices[k].normal = box18.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box19.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box19.Vertices[i].Position;
		vertices[k].normal = box19.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box20.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box20.Vertices[i].Position;
		vertices[k].normal = box20.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box21.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box21.Vertices[i].Position;
		vertices[k].normal = box21.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box22.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box22.Vertices[i].Position;
		vertices[k].normal = box22.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box23.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box23.Vertices[i].Position;
		vertices[k].normal = box23.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box24.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box24.Vertices[i].Position;
		vertices[k].normal = box24.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box25.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box25.Vertices[i].Position;
		vertices[k].normal = box25.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box26.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box26.Vertices[i].Position;
		vertices[k].normal = box26.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box27.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box27.Vertices[i].Position;
		vertices[k].normal = box27.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box28.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box28.Vertices[i].Position;
		vertices[k].normal = box28.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box29.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box29.Vertices[i].Position;
		vertices[k].normal = box29.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box30.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box30.Vertices[i].Position;
		vertices[k].normal = box30.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box31.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box31.Vertices[i].Position;
		vertices[k].normal = box31.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box32.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box32.Vertices[i].Position;
		vertices[k].normal = box32.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box33.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box33.Vertices[i].Position;
		vertices[k].normal = box33.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box34.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box34.Vertices[i].Position;
		vertices[k].normal = box34.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box35.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box35.Vertices[i].Position;
		vertices[k].normal = box35.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box36.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box36.Vertices[i].Position;
		vertices[k].normal = box36.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box37.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box37.Vertices[i].Position;
		vertices[k].normal = box37.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box38.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box38.Vertices[i].Position;
		vertices[k].normal = box38.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box39.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box39.Vertices[i].Position;
		vertices[k].normal = box39.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box40.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box40.Vertices[i].Position;
		vertices[k].normal = box40.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box41.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box41.Vertices[i].Position;
		vertices[k].normal = box41.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box42.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box42.Vertices[i].Position;
		vertices[k].normal = box42.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box43.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box43.Vertices[i].Position;
		vertices[k].normal = box43.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box44.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box44.Vertices[i].Position;
		vertices[k].normal = box44.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box45.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box45.Vertices[i].Position;
		vertices[k].normal = box45.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box46.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box46.Vertices[i].Position;
		vertices[k].normal = box46.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box47.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box47.Vertices[i].Position;
		vertices[k].normal = box47.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box48.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box48.Vertices[i].Position;
		vertices[k].normal = box48.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box49.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box49.Vertices[i].Position;
		vertices[k].normal = box49.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box50.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box50.Vertices[i].Position;
		vertices[k].normal = box50.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box51.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box51.Vertices[i].Position;
		vertices[k].normal = box51.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box52.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box52.Vertices[i].Position;
		vertices[k].normal = box52.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box53.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box53.Vertices[i].Position;
		vertices[k].normal = box53.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box54.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box54.Vertices[i].Position;
		vertices[k].normal = box54.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box55.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = box55.Vertices[i].Position;
		vertices[k].normal = box55.Vertices[i].Normal;
	}

	for (size_t i = 0; i < grid.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = grid.Vertices[i].Position;
		vertices[k].normal = grid.Vertices[i].Normal;
	}

	for (size_t i = 0; i < pellet.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = pellet.Vertices[i].Position;
		vertices[k].normal = pellet.Vertices[i].Normal;
	}

	for (size_t i = 0; i < pacMan.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = pacMan.Vertices[i].Position;
		vertices[k].normal = pacMan.Vertices[i].Normal;
	}

	for (size_t i = 0; i < powerUp.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = powerUp.Vertices[i].Position;
		vertices[k].normal = powerUp.Vertices[i].Normal;
	}

	for (size_t i = 0; i < ghost.Vertices.size(); ++i, ++k)
	{
		vertices[k].pos = ghost.Vertices[i].Position;
		vertices[k].normal = ghost.Vertices[i].Normal;
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::NormalTexVertex) * totalVertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mShapesVB));

	//
	// Pack the indices of all the meshes into one index buffer.
	//

	std::vector<UINT> indices;
	indices.insert(indices.end(), box1.Indices.begin(), box1.Indices.end());
	indices.insert(indices.end(), box2.Indices.begin(), box2.Indices.end());
	indices.insert(indices.end(), box3.Indices.begin(), box3.Indices.end());
	indices.insert(indices.end(), box4.Indices.begin(), box4.Indices.end());
	indices.insert(indices.end(), box5.Indices.begin(), box5.Indices.end());
	indices.insert(indices.end(), box6.Indices.begin(), box6.Indices.end());
	indices.insert(indices.end(), box7.Indices.begin(), box7.Indices.end());
	indices.insert(indices.end(), box8.Indices.begin(), box8.Indices.end());
	indices.insert(indices.end(), box9.Indices.begin(), box9.Indices.end());
	indices.insert(indices.end(), box10.Indices.begin(), box10.Indices.end());
	indices.insert(indices.end(), box11.Indices.begin(), box11.Indices.end());
	indices.insert(indices.end(), box12.Indices.begin(), box12.Indices.end());
	indices.insert(indices.end(), box13.Indices.begin(), box13.Indices.end());
	indices.insert(indices.end(), box14.Indices.begin(), box14.Indices.end());
	indices.insert(indices.end(), box15.Indices.begin(), box15.Indices.end());
	indices.insert(indices.end(), box16.Indices.begin(), box16.Indices.end());
	indices.insert(indices.end(), box17.Indices.begin(), box17.Indices.end());
	indices.insert(indices.end(), box18.Indices.begin(), box18.Indices.end());
	indices.insert(indices.end(), box19.Indices.begin(), box19.Indices.end());
	indices.insert(indices.end(), box20.Indices.begin(), box20.Indices.end());
	indices.insert(indices.end(), box21.Indices.begin(), box21.Indices.end());
	indices.insert(indices.end(), box22.Indices.begin(), box22.Indices.end());
	indices.insert(indices.end(), box23.Indices.begin(), box23.Indices.end());
	indices.insert(indices.end(), box24.Indices.begin(), box24.Indices.end());
	indices.insert(indices.end(), box25.Indices.begin(), box25.Indices.end());
	indices.insert(indices.end(), box26.Indices.begin(), box26.Indices.end());
	indices.insert(indices.end(), box27.Indices.begin(), box27.Indices.end());
	indices.insert(indices.end(), box28.Indices.begin(), box28.Indices.end());
	indices.insert(indices.end(), box29.Indices.begin(), box29.Indices.end());
	indices.insert(indices.end(), box30.Indices.begin(), box30.Indices.end());
	indices.insert(indices.end(), box31.Indices.begin(), box31.Indices.end());
	indices.insert(indices.end(), box32.Indices.begin(), box32.Indices.end());
	indices.insert(indices.end(), box33.Indices.begin(), box33.Indices.end());
	indices.insert(indices.end(), box34.Indices.begin(), box34.Indices.end());
	indices.insert(indices.end(), box35.Indices.begin(), box35.Indices.end());
	indices.insert(indices.end(), box36.Indices.begin(), box36.Indices.end());
	indices.insert(indices.end(), box37.Indices.begin(), box37.Indices.end());
	indices.insert(indices.end(), box38.Indices.begin(), box38.Indices.end());
	indices.insert(indices.end(), box39.Indices.begin(), box39.Indices.end());
	indices.insert(indices.end(), box40.Indices.begin(), box40.Indices.end());
	indices.insert(indices.end(), box41.Indices.begin(), box41.Indices.end());
	indices.insert(indices.end(), box42.Indices.begin(), box42.Indices.end());
	indices.insert(indices.end(), box43.Indices.begin(), box43.Indices.end());
	indices.insert(indices.end(), box44.Indices.begin(), box44.Indices.end());
	indices.insert(indices.end(), box45.Indices.begin(), box45.Indices.end());
	indices.insert(indices.end(), box46.Indices.begin(), box46.Indices.end());
	indices.insert(indices.end(), box47.Indices.begin(), box47.Indices.end());
	indices.insert(indices.end(), box48.Indices.begin(), box48.Indices.end());
	indices.insert(indices.end(), box49.Indices.begin(), box49.Indices.end());
	indices.insert(indices.end(), box50.Indices.begin(), box50.Indices.end());
	indices.insert(indices.end(), box51.Indices.begin(), box51.Indices.end());
	indices.insert(indices.end(), box52.Indices.begin(), box52.Indices.end());
	indices.insert(indices.end(), box53.Indices.begin(), box53.Indices.end());
	indices.insert(indices.end(), box54.Indices.begin(), box54.Indices.end());
	indices.insert(indices.end(), box55.Indices.begin(), box55.Indices.end());

	indices.insert(indices.end(), grid.Indices.begin(), grid.Indices.end());
	indices.insert(indices.end(), pellet.Indices.begin(), pellet.Indices.end());
	indices.insert(indices.end(), pacMan.Indices.begin(), pacMan.Indices.end());
	indices.insert(indices.end(), powerUp.Indices.begin(), powerUp.Indices.end());
	indices.insert(indices.end(), ghost.Indices.begin(), ghost.Indices.end());
	//indices.insert(indices.end(), fruit.Indices.begin(), fruit.Indices.end());

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * totalIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mShapesIB));*/
}



void PuckMan3D::updateGhosts(float dt)
{
	mCurrentTime += dt;

	switch (ghostState)
	{
		//set the ghost to their default colours
	case GS_NORMAL:
		/*mGhostMat.Diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		mPinkyMat.Diffuse = XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);
		mInkyMat.Diffuse = XMFLOAT4(0.0f, 0.98f, 1.0f, 1.0f);
		mClydeMat.Diffuse = XMFLOAT4(1.0f, 0.66f, 0.0f, 1.0f);*/
		//Materials::BLINKY.Diffuse = Materials::BLINKY.Diffuse;
		break;

		//set the Ghost blue
	case GS_BLUE:
		if (mCurrentTime < mTotalTime)
		{
			/*mGhostMat.Diffuse = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
			mPinkyMat.Diffuse = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
			mInkyMat.Diffuse = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
			mClydeMat.Diffuse = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);*/
			//Materials::BLINKY.Diffuse = Materials::GHOSTFRIGHTENED.Diffuse;
		
		}

		if (mCurrentTime >= mTotalTime)
		{
			timer.Stop();
			ghostState = GhostState::GS_FLASHING;
			flashingTimer.Reset();
			flashingTimer.Start();
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
			/*mGhostMat.Diffuse = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
			mPinkyMat.Diffuse = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
			mInkyMat.Diffuse = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
			mClydeMat.Diffuse = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);*/
		}
		else
		{
			/*mGhostMat.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			mPinkyMat.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			mInkyMat.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			mClydeMat.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);*/
		}

		if (mCurrentTime >= mTotalTime)
		{
			mNextTime = 0.0f;
			mCurrentTime = 0.0f;
			mTotalTime = 0.0f;
			ghostState = GhostState::GS_NORMAL;
			powerUpActivated = false;
			flashingTimer.Stop();
		}
		break;
	}
}

void PuckMan3D::resetGame()
{

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

	if (!isPlaying)
	{
		result = sys->playSound(sound[2], 0, false, &channel[2]);
		result = channel[2]->setChannelGroup(soundGroup);
		result = channel[2]->setPaused(false);
	}

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



