#pragma once
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
#include "BlurFilter.h"
#include "Character.h"
#include "FireBallParticles.h"

//#include "BasicMeshGeometry.h"
#include "MazeLoader.h"

#include "xnacollision.h"


struct TestParticle
{
	XMFLOAT3 pos;
	XMFLOAT3 vel;
	XMFLOAT2 size;
	float age;
	float lifetime;
};

const int MAX_PARTICLES = 100000;

class PuckMan3D : public D3DApp
{
public:
	PuckMan3D(HINSTANCE hInstance);
	~PuckMan3D();

	enum FacingState
	{
		FCS_FORWARD,
		FCS_BACKWARD,
		FCS_RIGHT,
		FCS_LEFT,
		FCS_DEFAULT
	};

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

	int GetLevelNumber()
	{
		return mLevelCounter;
	}

	void OnKeyUp(WPARAM btnState);
private:
	//const float PUCKMAN_SPEED = 1000.0f;
	void BuildTestPyramid();
	void BuildVertexLayout();
	void BuildSceneLights();
	void BuildParticleVB();
	void BuildBlendStates();
	void BuildDSStates();
	void BuildScreenQuadGeometryBuffers();
	void BuildOffscreenViews();

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

	XMVECTOR CylToCyl(FXMVECTOR c1Pos, float c1Rad, float c1Height, FXMVECTOR c2Pos, float c2Rad, float c2Height);

	void DrawParticles();

	//void BuildFireBallParticleVB();
	//void UpdateFireBallParticleVB();
	//void DrawFireBall();

	XNA::OrientedBox* GetOrientedBox(FXMVECTOR extents, const GraphicalObject* obj);
	XNA::Sphere* GetBoundingSphere(const GraphicalObject* obj, float radius);

	void BuildPuckMan();
	void BuildGhosts();
	void SetMaterials();
	void BuildShapeGeometryBuffers();
	void ResetPuckMan();
	void ResetGhosts();
	void MuteAllAudio();
	void MuteGhostSFX();
	void MuteEatingSFX();
	void MuteDeathSFX();
	void MuteBackGroundSFX();
	void DrawWrapper();
	void DrawScreenQuad();
	void BuildCherry();
	void BuildGrapes();
	void BuildApple();
	void BuildPeach();
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

	LitMatEffect* mCherry;
	LitMatEffect* mGrapes;
	LitMatEffect* mApple;
	LitMatEffect* mPeach;
	LitMatEffect* mLitMatInstanceEffect;
	LitTexEffect* mLitTexEffect;
	ParticleEffect* mParticleEffect;
	
	FireBallParticles *mFireBallPac;
	
	//ParticleEffect* mFireBallEffect;
	//std::vector<TestParticle> mFireBallParticles;
	//ID3D11Buffer* mFireBallParticleVB;

	ThirdPersonCam* mCam;
	BaseCamera* m2DCam;

	FontRasterizer* mFont;

	XMFLOAT4 mAmbientColour;
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;
	XMFLOAT4X4 m2DProj;

	PointLightOptimized mPointLights[3];
	SpotLightOptimized mSpotLight;

	PuckMan* mPuckMan;
	Terrain* mTestTerrain;
	SkyBox* mSkyBox;
	BasicModel* mBarnProjectile;
	BasicModel* mFarmModel;
	BasicModel* mMonsterModel;

	BasicModel* mMazeModel;
	Character* mMazeCharacter;
	BasicModel* mMazeModelInstanced;
	Character* mMazeCharacterInstanced;

	//std::vector<Character*> mTestChars;
	//std::vector<PuckMan*> mPuckMen;

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
	bool mMuteAll = false;
	//float mSpeed;
	bool mMuteEatingSFX = false;
	bool mMuteBackGroundSFX = false;
	bool mMuteGhostSFX = false;
	bool mMuteDeathSFX = false;
	bool mCanDrawFruit = true;
	float fruitR = 0.60f;
	float mNextTime = 0.0f;
	float mCurrentTime = 0.0f;
	float mTotalTime = 0.0f;
	float mPauseTime = 0.0f;
	float mTotalDeathTime = 0.0f;
	float mBeginningTime = 0.0f;
	float mFruitTime = 0.f;
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
	UINT mCountFloors;
	UINT mCountWallsBent;
	UINT mCountWallsStraight;
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

	enum GameState
	{
		GS_MAINMENU = 0,
		GS_ATTRACT,
		GS_PLAY,
		GS_GAMEOVER,
		GS_OPTIONS,
		GS_CREDITS,
		GS_SOUNDOPTIONS
	};
	GameState mGameState = GameState::GS_MAINMENU;
	FacingState mFacingState = FCS_DEFAULT;
	GhostState ghostState = GhostState::GS_NORMAL;
	SoundsState soundStates = SoundsState::SS_DEFAULT;
	FruitState fruitState = FruitState::FS_DEFAULT;

	std::vector<XMFLOAT3> mFruit;
	XMFLOAT3 mFruitPos = XMFLOAT3(-0.5f, 1.0f, -2.2f);

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

	ID3D11ShaderResourceView* mOffscreenSRV;
	ID3D11UnorderedAccessView* mOffscreenUAV;
	ID3D11RenderTargetView* mOffscreenRTV;

	BlurFilter mBlur;
	BasicMeshGeometry *mGeometryQuadFullScreen;
	BasicMeshGeometry *mCherryGeometry;
	BasicMeshGeometry *mAppleGeometry;
	BasicMeshGeometry *mGrapesGeometry;
	BasicMeshGeometry *mPeachGeometry;
	//BlurEffect* mBlurEffect;
};
