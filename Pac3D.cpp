//***************************************************************************************
// LitSkullDemo.cpp by Frank Luna (C) 2011 All Rights Reserved.
//
// Demonstrates lighting using 3 directional lights.
//
// Controls:
//		Hold the left mouse button down and move the mouse to rotate.
//      Hold the right mouse button down to zoom in and out.
//      Press '1', '2', '3' for 1, 2, or 3 lights enabled.
//
//***************************************************************************************

#include "d3dApp.h"
#include "d3dx11Effect.h"
#include "GeometryGenerator.h"
#include "MathHelper.h"
#include "LightHelper.h"
#include "Effects.h"
#include "Vertex.h"
#include <iostream>
#include "Common\GameTimer.h"

class Pac3D : public D3DApp
{
public:
	struct PowerUp
	{
		XMFLOAT3 pos;

		PowerUp(FXMVECTOR pos)
		{
			XMStoreFloat3(&this->pos, pos);
		}
	};
public:
	struct Pellet
	{
		XMFLOAT3 pos;

		Pellet(FXMVECTOR pos)
		{
			XMStoreFloat3(&this->pos, pos);
		}
	};
public:
	struct PacMan
	{
		XMFLOAT3 pos;
		XMFLOAT3 vel;

		PacMan(FXMVECTOR pos, FXMVECTOR vel)
		{
			XMStoreFloat3(&this->pos, pos);
			XMStoreFloat3(&this->vel, vel);
		}

	};
public:
	struct Ghost
	{
		XMFLOAT3 pos;
		XMFLOAT3 vel;

		Ghost(FXMVECTOR pos, FXMVECTOR vel)
		{
			XMStoreFloat3(&this->pos, pos);
			XMStoreFloat3(&this->vel, vel);
		}
	};

public:
	struct AABox
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
	};
public:
	Pac3D(HINSTANCE hInstance);
	~Pac3D();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

private:
	void BuildShapeGeometryBuffers();
	void UpdateKeyboardInput(float dt);
	void updateGhosts(float dt);
	bool isKeyPressed = false;
	bool PacManPelletOverlapTest(XMVECTOR s1Center, XMVECTOR s2Center);
	bool PacManPowerUpOverlapTest(XMVECTOR s1Center, XMVECTOR s2Center);
	XMVECTOR PacManAABoxOverLap(XMVECTOR s1Center);
	bool PacManGhostOverlapTest(XMVECTOR s1Center, XMVECTOR s2Center);


private:
	ID3D11Buffer* mShapesVB;
	ID3D11Buffer* mShapesIB;

	DirectionalLight mDirLights[3];
	PointLight mPointLights[4];
	Material mGridMat;
	Material mBoxMat;
	Material mPelletMat;
	Material mPowerUpMat;
	Material mPacManMat;
	Material mGhostMat;
	Material mPinkyMat;
	Material mInkyMat;
	Material mClydeMat;

	// Define transformations from local spaces to world space.
	XMFLOAT4X4 mPelletWorld[240];
	XMFLOAT4X4 mPowerUpWorld[4];
	XMFLOAT4X4 mPacManWorld[3];
	XMFLOAT4X4 mGhostWorld[4];
	XMFLOAT4X4 mBoxWorld[55];
	XMFLOAT4X4 mGridWorld;

	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;

	int mBoxVertexOffset;
	int mGridVertexOffset;
	int mPelletVertexOffset;
	int mPacManVertexOffset;
	int mPowerUpVertexOffset;
	int mGhostVertexOffset;

	UINT mBoxIndexOffset;
	UINT mGridIndexOffset;
	UINT mPelletIndexOffset;
	UINT mPacManIndexOffset;
	UINT mPowerUpIndexOffset;
	UINT mGhostIndexOffset;

	UINT mBoxIndexCount;
	UINT mGridIndexCount;
	UINT mPelletIndexCount;
	UINT mPacManIndexCount;
	UINT mPowerUpIndexCount;
	UINT mGhostIndexCount;

	UINT mLightCount;

	XMFLOAT3 mEyePosW;

	float mTheta;
	float mPhi;
	float mRadius;

	float mSpeed = 1000.0f;
	float pacManR = 0.75f;
	float ghostR = 0.75f;
	float pelletR = 0.125f;
	float powerUpR = 0.25f;

	bool powerUpActivated = false;
	bool isBlue = false;

	GameTimer timer;
	GameTimer flashingTimer;

	float nextTime = 0.0f;
	float currentTime = 0.0f;
	float totalTime = 0.0f;

	std::vector<AABox> mBoxData;
	std::vector<PacMan> mPacMan;
	std::vector<Pellet> mPellet;
	std::vector<PowerUp> mPowerUp;
	std::vector<Ghost> mGhost;
	std::vector<Ghost> mPinky;
	std::vector<Ghost> mInky;
	std::vector<Ghost> mClyde;

	enum GhostState
	{
		normalState = 0,
		blueState,
		flashingState
	};
	GhostState ghostState = GhostState::normalState;


	POINT mLastMousePos;
};

std::ostream& operator<<(std::ostream& os, FXMVECTOR v)
{
	XMFLOAT4 dest;
	XMStoreFloat4(&dest, v);

	os << "(" << dest.x << ", " << dest.y << ", " << dest.z << ", " << dest.w << ")";
	return os;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	Pac3D theApp(hInstance);

	if (!theApp.Init())
		return 0;

	return theApp.Run();
}


Pac3D::Pac3D(HINSTANCE hInstance)
: D3DApp(hInstance), mShapesVB(0), mShapesIB(0), mLightCount(3),
mEyePosW(0.0f, 0.0f, 0.0f), mTheta(1.5f*MathHelper::Pi), mPhi(0.276f*MathHelper::Pi), mRadius(45.0f)
{
	mMainWndCaption = L"Pac3D Demo";



	mLastMousePos.x = 0;
	mLastMousePos.y = 0;

	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mGridWorld, I);
	XMStoreFloat4x4(&mView, I);
	XMStoreFloat4x4(&mProj, I);

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

	////Third Row
	mPellet.push_back(Pellet(XMVectorSet(-12.5f, 0.75f, -12.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(-1.5f, 0.75f, -12.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(1.5f, 0.75f, -12.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(12.5f, 0.75f, -12.5f, 0.0f)));

	////Fourth Row
	for (int i = 0; i < 6; ++i)
	{
		mPellet.push_back(Pellet(XMVectorSet(-12.5f + i, 0.75f, -11.5f, 0.0f)));
	}
	for (int i = 0; i < 4; ++i)
	{
		mPellet.push_back(Pellet(XMVectorSet(-4.5f + i, 0.75f, -11.5f, 0.0f)));
	}
	for (int i = 0; i < 4; ++i)
	{
		mPellet.push_back(Pellet(XMVectorSet(1.5f + i, 0.75f, -11.5f, 0.0f)));
	}
	for (int i = 0; i < 6; ++i)
	{
		mPellet.push_back(Pellet(XMVectorSet(7.5f + i, 0.75f, -11.5f, 0.0f)));
	}

	////Fifth Row
	mPellet.push_back(Pellet(XMVectorSet(-10.5f, 0.75f, -10.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(-7.5f, 0.75f, -10.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(-4.5f, 0.75f, -10.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(4.5f, 0.75f, -10.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(7.5f, 0.75f, -10.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(10.5f, 0.75f, -10.5f, 0.0f)));

	////Sixth Row
	mPellet.push_back(Pellet(XMVectorSet(-10.5f, 0.75f, -9.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(-7.5f, 0.75f, -9.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(-4.5f, 0.75f, -9.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(4.5f, 0.75f, -9.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(7.5f, 0.75f, -9.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(10.5f, 0.75f, -9.5f, 0.0f)));

	////Seventh Row
	for (int i = 0; i < 2; ++i)
	{
		mPellet.push_back(Pellet(XMVectorSet(-11.5f + i, 0.75f, -8.5f, 0.0f)));
	}
	for (int i = 0; i < 7; ++i)
	{
		mPellet.push_back(Pellet(XMVectorSet(-7.5f + i, 0.75f, -8.5f, 0.0f)));
	}
	for (int i = 0; i < 7; ++i)
	{
		mPellet.push_back(Pellet(XMVectorSet(1.5f + i, 0.75f, -8.5f, 0.0f)));
	}
	for (int i = 0; i < 2; ++i)
	{
		mPellet.push_back(Pellet(XMVectorSet(10.5f + i, 0.75f, -8.5f, 0.0f)));
	}

	////Eighth Row
	mPellet.push_back(Pellet(XMVectorSet(-12.5f, 0.75f, -7.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(-7.5f, 0.75f, -7.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(-1.5f, 0.75f, -7.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(1.5f, 0.75f, -7.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(7.5f, 0.75f, -7.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(12.5f, 0.75f, -7.5f, 0.0f)));

	////Ninth Row
	mPellet.push_back(Pellet(XMVectorSet(-12.5f, 0.75f, -6.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(-7.5f, 0.75f, -6.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(-1.5f, 0.75f, -6.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(1.5f, 0.75f, -6.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(7.5f, 0.75f, -6.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(12.5f, 0.75f, -6.5f, 0.0f)));

	////Tenth Row
	for (int i = 0; i < 12; ++i)
	{
		mPellet.push_back(Pellet(XMVectorSet(-12.5f + i, 0.75f, -5.5f, 0.0f)));
	}
	for (int i = 0; i < 12; ++i)
	{
		mPellet.push_back(Pellet(XMVectorSet(1.5f + i, 0.75f, -5.5f, 0.0f)));
	}

	////Eleventh to Twenty First Row
	for (int i = 0; i < 11; ++i)
	{
		mPellet.push_back(Pellet(XMVectorSet(-7.5f, 0.75f, -4.5f + i, 0.0f)));
		mPellet.push_back(Pellet(XMVectorSet(7.5f, 0.75f, -4.5f + i, 0.0f)));
	}

	////Twenty Second Row
	for (int i = 0; i < 6; ++i)
	{
		mPellet.push_back(Pellet(XMVectorSet(-12.5f + i, 0.75f, 6.5f, 0.0f)));
	}
	for (int i = 0; i < 4; ++i)
	{
		mPellet.push_back(Pellet(XMVectorSet(-4.5f + i, 0.75f, 6.5f, 0.0f)));
	}
	for (int i = 0; i < 4; ++i)
	{
		mPellet.push_back(Pellet(XMVectorSet(1.5f + i, 0.75f, 6.5f, 0.0f)));
	}
	for (int i = 0; i < 6; ++i)
	{
		mPellet.push_back(Pellet(XMVectorSet(7.5f + i, 0.75f, 6.5f, 0.0f)));
	}

	////Twenty Third Row
	mPellet.push_back(Pellet(XMVectorSet(-12.5f, 0.75f, 7.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(-7.5f, 0.75f, 7.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(-4.5f, 0.75f, 7.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(4.5f, 0.75f, 7.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(7.5f, 0.75f, 7.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(12.5f, 0.75f, 7.5f, 0.0f)));

	////Twenty Fourth Row
	mPellet.push_back(Pellet(XMVectorSet(-12.5f, 0.75f, 8.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(-7.5f, 0.75f, 8.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(-4.5f, 0.75f, 8.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(4.5f, 0.75f, 8.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(7.5f, 0.75f, 8.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(12.5f, 0.75f, 8.5f, 0.0f)));

	////Twenty Fifth Row
	for (int i = 0; i < 26; ++i)
	{
		mPellet.push_back(Pellet(XMVectorSet(-12.5f + i, 0.75f, 9.5f, 0.0f)));
	}

	////Twenty Sixth Row
	mPellet.push_back(Pellet(XMVectorSet(-12.5f, 0.75f, 10.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(-7.5f, 0.75f, 10.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(-1.5f, 0.75f, 10.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(1.5f, 0.75f, 10.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(7.5f, 0.75f, 10.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(12.5f, 0.75f, 10.5f, 0.0f)));

	////Twenty Seventh Row
	mPellet.push_back(Pellet(XMVectorSet(-7.5f, 0.75f, 11.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(-1.5f, 0.75f, 11.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(1.5f, 0.75f, 11.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(7.5f, 0.75f, 11.5f, 0.0f)));

	////Twenty Eighth Row
	mPellet.push_back(Pellet(XMVectorSet(-12.5f, 0.75f, 12.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(-7.5f, 0.75f, 12.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(-1.5f, 0.75f, 12.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(1.5f, 0.75f, 12.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(7.5f, 0.75f, 12.5f, 0.0f)));
	mPellet.push_back(Pellet(XMVectorSet(12.5f, 0.75f, 12.5f, 0.0f)));

	////Twenty Ninth Row
	for (int i = 0; i < 12; ++i)
	{
		mPellet.push_back(Pellet(XMVectorSet(-12.5f + i, 0.75f, 13.5f, 0.0f)));
		mPellet.push_back(Pellet(XMVectorSet(1.5f + i, 0.75f, 13.5f, 0.0f)));
	}

	////Add Pellets to the world
	for (int i = 0; i < 240; ++i)
	{
		XMStoreFloat4x4(&mPelletWorld[i], XMMatrixTranslation(mPellet[i].pos.x, mPellet[i].pos.y, mPellet[i].pos.z));
	}


	////Positioning the PacMans
	mPacMan.push_back(PacMan(XMVectorSet(0.0f, 0.75f, -8.5f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)));
	mPacMan.push_back(PacMan(XMVectorSet(-12.0f, 0.75f, -17.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)));
	mPacMan.push_back(PacMan(XMVectorSet(-9.5f, 0.75f, -17.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)));

	for (int i = 0; i < mPacMan.size(); ++i)
	{
		XMStoreFloat4x4(&mPacManWorld[i], XMMatrixTranslation(mPacMan[i].pos.x, mPacMan[i].pos.y, mPacMan[i].pos.z));
	}

	////Positioning the Ghost
	mGhost.push_back(Ghost(XMVectorSet(0.0f, 0.75f, 3.5f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)));
	mPinky.push_back(Ghost(XMVectorSet(0.0f, 0.75f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)));
	mInky.push_back(Ghost(XMVectorSet(-2.0f, 0.75f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)));
	mClyde.push_back(Ghost(XMVectorSet(2.0f, 0.75f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)));

	for (int i = 0; i < mGhost.size(); ++i)
	{
		XMStoreFloat4x4(&mGhostWorld[i], XMMatrixTranslation(mGhost[i].pos.x, mGhost[i].pos.y, mGhost[i].pos.z));
	}

	for (int i = 0; i < mPinky.size(); ++i)
	{
		XMStoreFloat4x4(&mGhostWorld[i], XMMatrixTranslation(mPinky[i].pos.x, mPinky[i].pos.y, mPinky[i].pos.z));
	}

	for (int i = 0; i < mInky.size(); ++i)
	{
		XMStoreFloat4x4(&mGhostWorld[i], XMMatrixTranslation(mInky[i].pos.x, mInky[i].pos.y, mInky[i].pos.z));
	}

	for (int i = 0; i < mClyde.size(); ++i)
	{
		XMStoreFloat4x4(&mGhostWorld[i], XMMatrixTranslation(mClyde[i].pos.x, mClyde[i].pos.y, mClyde[i].pos.z));
	}

	////Positioning the PowerUps
	mPowerUp.push_back(PowerUp(XMVectorSet(-12.5f, 0.75f, -8.5f, 0.0f)));
	mPowerUp.push_back(PowerUp(XMVectorSet(-12.5f, 0.75f, 11.5f, 0.0f)));
	mPowerUp.push_back(PowerUp(XMVectorSet(12.5f, 0.75f, -8.5f, 0.0f)));
	mPowerUp.push_back(PowerUp(XMVectorSet(12.5f, 0.75f, 11.5f, 0.0f)));

	for (int i = 0; i < mPowerUp.size(); ++i)
	{
		XMStoreFloat4x4(&mPowerUpWorld[i], XMMatrixTranslation(mPowerUp[i].pos.x, mPowerUp[i].pos.y, mPowerUp[i].pos.z));
	}

	mDirLights[0].Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[0].Diffuse = XMFLOAT4(0.20f, 0.20f, 0.20f, 1.0f);
	mDirLights[0].Specular = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	mDirLights[0].Direction = XMFLOAT3(0.57735f, -0.57735f, 0.6f);

	mDirLights[1].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[1].Diffuse = XMFLOAT4(0.20f, 0.20f, 0.20f, 1.0f);
	mDirLights[1].Specular = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	mDirLights[1].Direction = XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);

	mDirLights[2].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[2].Diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[2].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[2].Direction = XMFLOAT3(0.57735f, -0.57735f, -0.57735f);

	mGridMat.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
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
	mPinkyMat.Diffuse = XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);
	mPinkyMat.Specular = XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f);

	mInkyMat.Ambient = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	mInkyMat.Diffuse = XMFLOAT4(0.0f, 0.98f, 1.0f, 1.0f);
	mInkyMat.Specular = XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f);

	mClydeMat.Ambient = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	mClydeMat.Diffuse = XMFLOAT4(1.0f, 0.66f, 0.0f, 1.0f);
	mClydeMat.Specular = XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f);

	mBoxMat.Ambient = XMFLOAT4(0.12f, 0.12f, 0.6f, 1.0f);
	mBoxMat.Diffuse = XMFLOAT4(0.12f, 0.12f, 0.6f, 1.0f);
	mBoxMat.Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 16.0f);
}

Pac3D::~Pac3D()
{
	ReleaseCOM(mShapesVB);
	ReleaseCOM(mShapesIB);

	Effects::DestroyAll();
	InputLayouts::DestroyAll();
}

bool Pac3D::Init()
{
	if (!D3DApp::Init())
		return false;

	// Must init Effects first since InputLayouts depend on shader signatures.
	Effects::InitAll(md3dDevice);
	InputLayouts::InitAll(md3dDevice);

	BuildShapeGeometryBuffers();


	return true;
}

void Pac3D::OnResize()
{
	D3DApp::OnResize();

	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&mProj, P);
}

void Pac3D::UpdateScene(float dt)
{
	float eyeOffset = 25.0f;

	UpdateKeyboardInput(dt);


	XMVECTOR pos = XMLoadFloat3(&mPacMan[0].pos);
	XMVECTOR vel = XMLoadFloat3(&mPacMan[0].vel);

	pos = pos + (vel * mSpeed * dt);

	XMStoreFloat3(&mPacMan[0].pos, pos);
	XMStoreFloat3(&mPacMan[0].vel, vel);

	//// Checking PacMan collision with maze
	XMStoreFloat3(&mPacMan[0].pos, PacManAABoxOverLap(pos));

	////Checking PacMan Collision with Ghost
	for (int i = 0; i < mGhost.size(); ++i)
	{
		XMVECTOR ghostPos = XMLoadFloat3(&mGhost[i].pos);
		XMVECTOR pinkyPos = XMLoadFloat3(&mPinky[i].pos);
		XMVECTOR inkyPos = XMLoadFloat3(&mInky[i].pos);
		XMVECTOR clydePos = XMLoadFloat3(&mClyde[i].pos);
		if (!powerUpActivated)
		{//kill PuckMan
			if (PacManPelletOverlapTest(pos, ghostPos) == true)
			{
				mPacMan.pop_back();
				mPacMan[0].pos.x = 0.0f;
				mPacMan[0].pos.y = 0.75f;
				mPacMan[0].pos.z = -8.5f;
				break;
			}
		}
		else 
		{//kill ghosts
			if (PacManPelletOverlapTest(pos, ghostPos) == true)
			{
				mGhost.pop_back();
				mGhost.push_back(Ghost(XMVectorSet(0.0f, 0.75f, 3.5f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)));
				break;
			}
			else if (PacManPelletOverlapTest(pos, pinkyPos) == true)
			{
				mPinky.pop_back();
				mPinky.push_back(Ghost(XMVectorSet(0.0f, 0.75f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)));
				break;
			}
			else if (PacManPelletOverlapTest(pos, inkyPos) == true)
			{
				mInky.pop_back();
				mInky.push_back(Ghost(XMVectorSet(-2.0f, 0.75f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)));
				break;
			}
			else if (PacManPelletOverlapTest(pos, clydePos) == true)
			{
				mClyde.pop_back();
				mClyde.push_back(Ghost(XMVectorSet(2.0f, 0.75f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)));
				break;
			}
		}
	}

	////checking PacMan collision with Pellets
	//// If collision is true remove the pellet.
	for (int i = 0; i < mPellet.size(); ++i)
	{
		XMVECTOR pelPos = XMLoadFloat3(&mPellet[i].pos);

		if (PacManPelletOverlapTest(pos, pelPos) == true)
		{
			mPellet.erase(mPellet.begin() + i);
			--i;
		}

	}
	////checking PacMan collision with PowerUps
	//// If collision is true remove the powerup.
	for (int i = 0; i < mPowerUp.size(); ++i)
	{
		XMVECTOR pUpPos = XMLoadFloat3(&mPowerUp[i].pos);

		if (PacManPowerUpOverlapTest(pos, pUpPos) == true)
		{
			powerUpActivated = true;
			ghostState = GhostState::blueState;
			currentTime = 0.0f;
			totalTime = 3.0f;
			mPowerUp.erase(mPowerUp.begin() + i);
			--i;
			timer.Reset();
			timer.Start();
		}
	}

	////PacMan Tunnel Check

	if (mPacMan[0].pos.x < -14)
	{
		mPacMan[0].pos.x = 14;
	}
	if (mPacMan[0].pos.x > 14)
	{
		mPacMan[0].pos.x = -14;
	}

	// Camera X, Y, Z Positioning.
	float x = mPacMan[0].pos.x;
	float y = mPacMan[0].pos.y + eyeOffset;
	float z = mPacMan[0].pos.z - eyeOffset;

	mEyePosW = XMFLOAT3(x, y, z);

	// Build the view matrix.
	XMVECTOR eyePos = XMVectorSet(x, y, z, 1.0f);
	XMVECTOR target = XMVectorSet(mPacMan[0].pos.x, mPacMan[0].pos.y + 5, mPacMan[0].pos.z, 0.0f);//XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX V = XMMatrixLookAtLH(eyePos, target, up);
	XMStoreFloat4x4(&mView, V);

	updateGhosts(dt);

	//
	// Switch the number of lights based on key presses.
	//
	if (GetAsyncKeyState('0') & 0x8000)
		mLightCount = 0;

	if (GetAsyncKeyState('1') & 0x8000)
		mLightCount = 1;

	if (GetAsyncKeyState('2') & 0x8000)
		mLightCount = 2;

	if (GetAsyncKeyState('3') & 0x8000)
		mLightCount = 3;

	std::cout << "Time: " << timer.DeltaTime() << std::endl;
}

void Pac3D::DrawScene()
{
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::Black));
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	md3dImmediateContext->IASetInputLayout(InputLayouts::PosNormal);
	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(Vertex::PosNormal);
	UINT offset = 0;

	XMMATRIX view = XMLoadFloat4x4(&mView);
	XMMATRIX proj = XMLoadFloat4x4(&mProj);
	XMMATRIX viewProj = view*proj;

	//// Set per frame constants.
	Effects::BasicFX->SetDirLights(mDirLights);
	Effects::BasicFX->SetEyePosW(mEyePosW);

	//// Figure out which technique to use.
	ID3DX11EffectTechnique* activeTech = Effects::BasicFX->Light1Tech;
	switch (mLightCount)
	{
	case 1:
		activeTech = Effects::BasicFX->Light1Tech;
		break;
	case 2:
		activeTech = Effects::BasicFX->Light2Tech;
		break;
	case 3:
		activeTech = Effects::BasicFX->Light3Tech;
		break;
	}

	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		md3dImmediateContext->IASetVertexBuffers(0, 1, &mShapesVB, &stride, &offset);
		md3dImmediateContext->IASetIndexBuffer(mShapesIB, DXGI_FORMAT_R32_UINT, 0);

		//// Draw the grid.
		XMMATRIX world = XMLoadFloat4x4(&mGridWorld);
		XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
		XMMATRIX worldViewProj = world*view*proj;

		Effects::BasicFX->SetWorld(world);
		Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::BasicFX->SetWorldViewProj(worldViewProj);
		Effects::BasicFX->SetMaterial(mGridMat);

		activeTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mGridIndexCount, mGridIndexOffset, mGridVertexOffset);

		int vertexOffset = 0;
		int indexOffset = 0;

		//// Draw the Pac-Man Maze with boxes
		for (int i = 0; i < mBoxData.size(); ++i)
		{
			world = XMMatrixTranslation(mBoxData[i].pos.x, mBoxData[i].pos.y, mBoxData[i].pos.z);
			worldInvTranspose = MathHelper::InverseTranspose(world);
			worldViewProj = world*view*proj;

			Effects::BasicFX->SetWorld(world);
			Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
			Effects::BasicFX->SetWorldViewProj(worldViewProj);
			Effects::BasicFX->SetMaterial(mBoxMat);

			activeTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
			md3dImmediateContext->DrawIndexed(36, indexOffset, vertexOffset);
			vertexOffset += 24;
			indexOffset += 36;
		}

		//// Draw the Pellets.
		for (int i = 0; i < mPellet.size(); ++i)
		{
			world = XMMatrixTranslation(mPellet[i].pos.x, mPellet[i].pos.y, mPellet[i].pos.z);
			worldInvTranspose = MathHelper::InverseTranspose(world);
			worldViewProj = world*view*proj;

			Effects::BasicFX->SetWorld(world);
			Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
			Effects::BasicFX->SetWorldViewProj(worldViewProj);
			Effects::BasicFX->SetMaterial(mPelletMat);

			activeTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
			md3dImmediateContext->DrawIndexed(mPelletIndexCount, mPelletIndexOffset, mPelletVertexOffset);
		}

		//// Draw the PowerUps
		for (int i = 0; i < mPowerUp.size(); ++i)
		{
			world = XMMatrixTranslation(mPowerUp[i].pos.x, mPowerUp[i].pos.y, mPowerUp[i].pos.z);
			worldInvTranspose = MathHelper::InverseTranspose(world);
			worldViewProj = world*view*proj;

			Effects::BasicFX->SetWorld(world);
			Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
			Effects::BasicFX->SetWorldViewProj(worldViewProj);
			Effects::BasicFX->SetMaterial(mPowerUpMat);

			activeTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
			md3dImmediateContext->DrawIndexed(mPowerUpIndexCount, mPowerUpIndexOffset, mPowerUpVertexOffset);
		}

		//// Draw the PacMans.
		for (int i = 0; i < mPacMan.size(); ++i)
		{
			world = XMMatrixTranslation(mPacMan[i].pos.x, mPacMan[i].pos.y, mPacMan[i].pos.z);
			worldInvTranspose = MathHelper::InverseTranspose(world);
			worldViewProj = world*view*proj;

			Effects::BasicFX->SetWorld(world);
			Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
			Effects::BasicFX->SetWorldViewProj(worldViewProj);
			Effects::BasicFX->SetMaterial(mPacManMat);


			activeTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
			md3dImmediateContext->DrawIndexed(mPacManIndexCount, mPacManIndexOffset, mPacManVertexOffset);
		}

		//// Draw the Ghosts
		for (int i = 0; i < mGhost.size(); ++i)
		{
			world = XMMatrixTranslation(mGhost[i].pos.x, mGhost[i].pos.y, mGhost[i].pos.z);
			worldInvTranspose = MathHelper::InverseTranspose(world);
			worldViewProj = world*view*proj;

			Effects::BasicFX->SetWorld(world);
			Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
			Effects::BasicFX->SetWorldViewProj(worldViewProj);
			Effects::BasicFX->SetMaterial(mGhostMat);

			activeTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
			md3dImmediateContext->DrawIndexed(mGhostIndexCount, mGhostIndexOffset, mGhostVertexOffset);
		}

		for (int i = 0; i < mPinky.size(); ++i)
		{
			world = XMMatrixTranslation(mPinky[i].pos.x, mPinky[i].pos.y, mPinky[i].pos.z);
			worldInvTranspose = MathHelper::InverseTranspose(world);
			worldViewProj = world*view*proj;

			Effects::BasicFX->SetWorld(world);
			Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
			Effects::BasicFX->SetWorldViewProj(worldViewProj);
			Effects::BasicFX->SetMaterial(mPinkyMat);

			activeTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
			md3dImmediateContext->DrawIndexed(mGhostIndexCount, mGhostIndexOffset, mGhostVertexOffset);
		}

		for (int i = 0; i < mInky.size(); ++i)
		{
			world = XMMatrixTranslation(mInky[i].pos.x, mInky[i].pos.y, mInky[i].pos.z);
			worldInvTranspose = MathHelper::InverseTranspose(world);
			worldViewProj = world*view*proj;

			Effects::BasicFX->SetWorld(world);
			Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
			Effects::BasicFX->SetWorldViewProj(worldViewProj);
			Effects::BasicFX->SetMaterial(mInkyMat);

			activeTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
			md3dImmediateContext->DrawIndexed(mGhostIndexCount, mGhostIndexOffset, mGhostVertexOffset);
		}

		for (int i = 0; i < mClyde.size(); ++i)
		{
			world = XMMatrixTranslation(mClyde[i].pos.x, mClyde[i].pos.y, mClyde[i].pos.z);
			worldInvTranspose = MathHelper::InverseTranspose(world);
			worldViewProj = world*view*proj;

			Effects::BasicFX->SetWorld(world);
			Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
			Effects::BasicFX->SetWorldViewProj(worldViewProj);
			Effects::BasicFX->SetMaterial(mClydeMat);

			activeTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
			md3dImmediateContext->DrawIndexed(mGhostIndexCount, mGhostIndexOffset, mGhostVertexOffset);
		}


	}

	HR(mSwapChain->Present(0, 0));
}


void Pac3D::UpdateKeyboardInput(float dt)
{

	// Move Forward
	if (GetAsyncKeyState('W') & 0x8000)
	{
		isKeyPressed = true;
		mPacMan[0].vel.x = 0.0f * dt;
		mPacMan[0].vel.y = 0.0f * dt;
		mPacMan[0].vel.z = 1.0f * dt;
		if (mPacMan[0].vel.z < 0.00826695096f)
		{
			mPacMan[0].vel.z = 0.00826695096f;
		}
	}
	else
	{
		isKeyPressed = false;
		mPacMan[0].vel.x = 0.0f;
		mPacMan[0].vel.y = 0.0f;
		mPacMan[0].vel.z = 0.0f;
	}

	// Move Backwards 
	if (GetAsyncKeyState('S') & 0x8000)
	{
		isKeyPressed = true;
		mPacMan[0].vel.x = 0.0f * dt;
		mPacMan[0].vel.y = 0.0f * dt;
		mPacMan[0].vel.z = -1.0f * dt;
		if (mPacMan[0].vel.z > -0.00826695096f)
		{
			mPacMan[0].vel.z = -0.00826695096f;
		}
	}

	// Move Left
	if (GetAsyncKeyState('A') & 0x8000)
	{
		isKeyPressed = true;
		mPacMan[0].vel.x = -1.0f * dt;
		if (mPacMan[0].vel.x > -0.00826695096f)
		{
			mPacMan[0].vel.x = -0.00826695096f;
		}
		mPacMan[0].vel.y = 0.0f * dt;
		mPacMan[0].vel.z = 0.0f * dt;
	}

	// Move Right
	if (GetAsyncKeyState('D') & 0x8000)
	{
		isKeyPressed = true;
		mPacMan[0].vel.x = 1.0f * dt;
		if (mPacMan[0].vel.x < 0.00826695096f)
		{
			mPacMan[0].vel.x = 0.00826695096f;
		}

		mPacMan[0].vel.y = 0.0f * dt;
		mPacMan[0].vel.z = 0.0f * dt;
	}



}

XMVECTOR Pac3D::PacManAABoxOverLap(XMVECTOR s1Center)
{
	float s1Radius = pacManR;

	for (int i = 0; i < mBoxData.size(); ++i)
	{
		XMVECTOR min = XMLoadFloat3(&mBoxData[i].min);
		XMVECTOR max = XMLoadFloat3(&mBoxData[i].max);

		XMVECTOR A = XMVectorClamp(s1Center, min, max);

		XMVECTOR d = s1Center - A; //difference between the closest point on the box  and sphere center

		float distance = sqrt((d.m128_f32[0] * d.m128_f32[0]) /*+ (d.m128_f32[1] * d.m128_f32[1])*/ + (d.m128_f32[2] * d.m128_f32[2])); //Magnitude of the difference

		float overLap = s1Radius - distance;

		if (overLap > 0) // Have Collision
		{
			s1Center += XMVector3Normalize(d) * overLap; //correct collision by moving sphere out of box
			return s1Center;
		}
	}
	return s1Center;

}

bool Pac3D::PacManGhostOverlapTest(XMVECTOR s1Center, XMVECTOR s2Center)
{
	float s1Radius = pacManR;

	float s2Radius = ghostR;

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

bool Pac3D::PacManPelletOverlapTest(XMVECTOR s1Center, XMVECTOR s2Center)
{
	float s1Radius = pacManR;

	float s2Radius = pelletR;

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

bool Pac3D::PacManPowerUpOverlapTest(XMVECTOR s1Center, XMVECTOR s2Center)
{
	float s1Radius = pacManR;

	float s2Radius = powerUpR;

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


void Pac3D::BuildShapeGeometryBuffers()
{
	// The blocks to make the Pac-Man maze
	GeometryGenerator::MeshData box1;
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
	GeometryGenerator::MeshData ghost;


	// Dimensions the blocks for the Pac-Man maze
	GeometryGenerator geoGen;
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
	geoGen.CreateSphere(pelletR, 10, 10, pellet);

	// PowerUps of the Pac-Man grid
	geoGen.CreateSphere(powerUpR, 10, 10, powerUp);

	// Pac-Man
	geoGen.CreateSphere(pacManR, 10, 10, pacMan);

	// Ghost
	geoGen.CreateSphere(ghostR, 10, 10, ghost);


	// Cache the vertex offsets to each object in the concatenated vertex buffer.
	mBoxVertexOffset = 0;
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


	// Cache the starting index for each object in the concatenated index buffer.
	mBoxIndexOffset = 0;
	mGridIndexOffset = mBoxIndexCount;
	mPelletIndexOffset = mGridIndexOffset + mGridIndexCount;
	mPacManIndexOffset = mPelletIndexOffset + mPacManIndexCount;
	mPowerUpIndexOffset = mPacManIndexOffset + mPacManIndexCount;
	mGhostIndexOffset = mPowerUpIndexOffset + mPowerUpIndexCount;


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
		+ grid.Vertices.size() + pellet.Vertices.size() + pacMan.Vertices.size() + powerUp.Vertices.size() + ghost.Vertices.size();


	UINT totalIndexCount =
		mBoxIndexCount +
		mGridIndexCount +
		mPelletIndexCount +
		mPacManIndexCount +
		mPowerUpIndexCount +
		mGhostIndexCount;


	//
	// Extract the vertex elements we are interested in and pack the
	// vertices of all the meshes into one vertex buffer.
	//

	std::vector<Vertex::PosNormal> vertices(totalVertexCount);

	UINT k = 0;
	for (int i = 0; i < box1.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box1.Vertices[i].Position;
		vertices[k].Normal = box1.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box2.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box2.Vertices[i].Position;
		vertices[k].Normal = box2.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box3.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box3.Vertices[i].Position;
		vertices[k].Normal = box3.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box4.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box4.Vertices[i].Position;
		vertices[k].Normal = box4.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box5.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box5.Vertices[i].Position;
		vertices[k].Normal = box5.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box6.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box6.Vertices[i].Position;
		vertices[k].Normal = box6.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box7.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box7.Vertices[i].Position;
		vertices[k].Normal = box7.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box8.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box8.Vertices[i].Position;
		vertices[k].Normal = box8.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box9.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box9.Vertices[i].Position;
		vertices[k].Normal = box9.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box10.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box10.Vertices[i].Position;
		vertices[k].Normal = box10.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box11.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box11.Vertices[i].Position;
		vertices[k].Normal = box11.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box12.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box12.Vertices[i].Position;
		vertices[k].Normal = box12.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box13.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box13.Vertices[i].Position;
		vertices[k].Normal = box13.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box14.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box14.Vertices[i].Position;
		vertices[k].Normal = box14.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box15.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box15.Vertices[i].Position;
		vertices[k].Normal = box15.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box16.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box16.Vertices[i].Position;
		vertices[k].Normal = box16.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box17.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box17.Vertices[i].Position;
		vertices[k].Normal = box17.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box18.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box18.Vertices[i].Position;
		vertices[k].Normal = box18.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box19.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box19.Vertices[i].Position;
		vertices[k].Normal = box19.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box20.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box20.Vertices[i].Position;
		vertices[k].Normal = box20.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box21.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box21.Vertices[i].Position;
		vertices[k].Normal = box21.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box22.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box22.Vertices[i].Position;
		vertices[k].Normal = box22.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box23.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box23.Vertices[i].Position;
		vertices[k].Normal = box23.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box24.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box24.Vertices[i].Position;
		vertices[k].Normal = box24.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box25.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box25.Vertices[i].Position;
		vertices[k].Normal = box25.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box26.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box26.Vertices[i].Position;
		vertices[k].Normal = box26.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box27.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box27.Vertices[i].Position;
		vertices[k].Normal = box27.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box28.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box28.Vertices[i].Position;
		vertices[k].Normal = box28.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box29.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box29.Vertices[i].Position;
		vertices[k].Normal = box29.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box30.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box30.Vertices[i].Position;
		vertices[k].Normal = box30.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box31.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box31.Vertices[i].Position;
		vertices[k].Normal = box31.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box32.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box32.Vertices[i].Position;
		vertices[k].Normal = box32.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box33.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box33.Vertices[i].Position;
		vertices[k].Normal = box33.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box34.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box34.Vertices[i].Position;
		vertices[k].Normal = box34.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box35.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box35.Vertices[i].Position;
		vertices[k].Normal = box35.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box36.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box36.Vertices[i].Position;
		vertices[k].Normal = box36.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box37.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box37.Vertices[i].Position;
		vertices[k].Normal = box37.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box38.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box38.Vertices[i].Position;
		vertices[k].Normal = box38.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box39.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box39.Vertices[i].Position;
		vertices[k].Normal = box39.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box40.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box40.Vertices[i].Position;
		vertices[k].Normal = box40.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box41.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box41.Vertices[i].Position;
		vertices[k].Normal = box41.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box42.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box42.Vertices[i].Position;
		vertices[k].Normal = box42.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box43.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box43.Vertices[i].Position;
		vertices[k].Normal = box43.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box44.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box44.Vertices[i].Position;
		vertices[k].Normal = box44.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box45.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box45.Vertices[i].Position;
		vertices[k].Normal = box45.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box46.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box46.Vertices[i].Position;
		vertices[k].Normal = box46.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box47.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box47.Vertices[i].Position;
		vertices[k].Normal = box47.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box48.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box48.Vertices[i].Position;
		vertices[k].Normal = box48.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box49.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box49.Vertices[i].Position;
		vertices[k].Normal = box49.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box50.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box50.Vertices[i].Position;
		vertices[k].Normal = box50.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box51.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box51.Vertices[i].Position;
		vertices[k].Normal = box51.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box52.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box52.Vertices[i].Position;
		vertices[k].Normal = box52.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box53.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box53.Vertices[i].Position;
		vertices[k].Normal = box53.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box54.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box54.Vertices[i].Position;
		vertices[k].Normal = box54.Vertices[i].Normal;
	}

	for (size_t i = 0; i < box55.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box55.Vertices[i].Position;
		vertices[k].Normal = box55.Vertices[i].Normal;
	}

	for (size_t i = 0; i < grid.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = grid.Vertices[i].Position;
		vertices[k].Normal = grid.Vertices[i].Normal;
	}

	for (size_t i = 0; i < pellet.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = pellet.Vertices[i].Position;
		vertices[k].Normal = pellet.Vertices[i].Normal;
	}

	for (size_t i = 0; i < pacMan.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = pacMan.Vertices[i].Position;
		vertices[k].Normal = pacMan.Vertices[i].Normal;
	}

	for (size_t i = 0; i < powerUp.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = powerUp.Vertices[i].Position;
		vertices[k].Normal = powerUp.Vertices[i].Normal;
	}

	for (size_t i = 0; i < ghost.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = ghost.Vertices[i].Position;
		vertices[k].Normal = ghost.Vertices[i].Normal;
	}


	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::PosNormal) * totalVertexCount;
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


	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT)* totalIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mShapesIB));
}

void Pac3D::updateGhosts(float dt)
{
	currentTime += dt;

	switch (ghostState)
	{
		//set the ghost to their default colours
		case normalState:
			mGhostMat.Diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
			mPinkyMat.Diffuse = XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);
			mInkyMat.Diffuse = XMFLOAT4(0.0f, 0.98f, 1.0f, 1.0f);
			mClydeMat.Diffuse = XMFLOAT4(1.0f, 0.66f, 0.0f, 1.0f);
			break;
		
		//set the Ghost blue
		case blueState:
			//timer.Tick();

			if (currentTime < totalTime)
			{
				mGhostMat.Diffuse = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
				mPinkyMat.Diffuse = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
				mInkyMat.Diffuse = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
				mClydeMat.Diffuse = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

			}

			if (currentTime >= totalTime)
			{
				timer.Stop();
				ghostState = GhostState::flashingState;
				flashingTimer.Reset();
				flashingTimer.Start();
				currentTime = 0.0f;
				nextTime = 0.3f;
				totalTime = 3.0f;
			}
			break;

		//Set the Ghost to be flashing once they are close to being in normalMode
		case flashingState:

			//flashingTimer.Tick();

			if (currentTime >= nextTime)
			{
				nextTime += 0.3f;
				isBlue = !isBlue;
			}
			//else
			//{
			//	nextTime += 0.3f;
			//	isBlue = !isBlue;
			//}

			if (isBlue)
			{
				mGhostMat.Diffuse = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
				mPinkyMat.Diffuse = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
				mInkyMat.Diffuse = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
				mClydeMat.Diffuse = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
			}
			else
			{
				mGhostMat.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				mPinkyMat.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				mInkyMat.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				mClydeMat.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			}

			if (currentTime >= totalTime)
			{
				nextTime = 0.0f;
				currentTime = 0.0f;
				totalTime = 0.0f;
				ghostState = GhostState::normalState;
				powerUpActivated = false;
				flashingTimer.Stop();
			}
			break;
	}
}





