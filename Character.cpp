#include "Character.h"


/*Character::Character(FXMVECTOR pos, FXMVECTOR look, FXMVECTOR up,
					ID3D11Device* device, LitTexEffect* effect, std::string filename,
					bool isRHS, bool isVFlipped, float speed, 
					float sprintSpeed, float health) :

GraphicalObject(pos, look, up, device, effect, filename, isRHS, isVFlipped),

	mSpeed(speed),
	mSprintSpeed(sprintSpeed),
	mHealth(health),
	mVelocity(0.0f, 0.0f, 0.0f),
	mGrounded(false)
{

}*/

Character::Character(FXMVECTOR pos, FXMVECTOR look, FXMVECTOR up, BasicModel& model,
	float speed, float sprintSpeed, float health)
	: 
	GraphicalObject(pos, look, up, model),
	mSpeed(speed),
	mSprintSpeed(sprintSpeed),
	mHealth(health),
	mVelocity(0.0f, 0.0f, 0.0f),
	mGrounded(false)
{
}

Character::~Character(void)
{
}

void Character::HitGround()
{
	mGrounded = true;
	mVelocity.y = 0.0f;
}

void Character::AddForce(FXMVECTOR force)
{
	XMVECTOR vel = XMLoadFloat3(&mVelocity);
	vel = vel + force;
	XMStoreFloat3(&mVelocity, vel);
}


void Character::Update(float dt)
{
	if (mPos.y > 0.0f)
	{
		mPos = mPos;
	}
	XMFLOAT3 prevPos = mPos;

	XMVECTOR pos = XMLoadFloat3(&mPos);
	XMVECTOR vel = XMLoadFloat3(&mVelocity);

	if (mGrounded)
	{
		vel = vel * 0.8f;
	}
	else
	{
		vel = vel * 0.97f;
	}

	mVelocityApplied = vel * dt;
	pos = pos + (vel * dt);
	mWorldUpdated = true;

	XMStoreFloat3(&mPos, pos);
	XMStoreFloat3(&mVelocity, vel);

	if (mPos.y > 0.0f)
	{
		mPos = mPos;
	}

	GraphicalObject::Update();
}

void Character::RemoveVelocityApplied()
{
	XMVECTOR pos = XMLoadFloat3(&mPos);
	XMVECTOR vel = XMLoadFloat3(&mVelocity);

	pos -= mVelocityApplied;
	//vel -= mVelocityApplied;
	vel = XMVectorZero();
	mVelocityApplied = XMVectorZero();

	mWorldUpdated = true;
	XMStoreFloat3(&mPos, pos);
	XMStoreFloat3(&mVelocity, vel);
	GraphicalObject::Update();
}

void Character::MoveLook(float amt)
{
	if (mGrounded)
	{
		XMVECTOR vel = XMLoadFloat3(&mVelocity);
		XMVECTOR look = XMLoadFloat3(&mLook);

		vel = vel + (look * amt);
		XMStoreFloat3(&mVelocity, vel);
	}
}

void Character::MoveStrafe(float amt)
{
	if (mGrounded)
	{
		XMVECTOR vel = XMLoadFloat3(&mVelocity);
		XMVECTOR right = XMLoadFloat3(&mRight);

		vel = vel + (right * amt);
		XMStoreFloat3(&mVelocity, vel);
	}

}

