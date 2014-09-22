#pragma once

#include "graphicalobject.h"

class Character : public GraphicalObject
{
public:
	Character(void){}
	/*Character(FXMVECTOR pos, FXMVECTOR look, FXMVECTOR up,
					ID3D11Device* device, LitTexEffect* effect, std::string filename,
					bool isRHS = false, bool isVFlipped = false,
					float speed = 0.0f, float sprintSpeed = 0.0f, float health = 0.0f);*/

	Character(FXMVECTOR pos, FXMVECTOR look, FXMVECTOR up, BasicModel& model,
		float speed = 0.0f, float sprintSpeed = 0.0f, float health = 0.0f);

	virtual ~Character(void);

	virtual void Attack(){}
	virtual void Update(float dt);

	virtual void RemoveVelocityApplied();

	float GetSpeed() const
	{
		return mSpeed;
	}

	float GetSprintSpeed() const
	{
		return mSprintSpeed;
	}

	virtual void MoveLook(float amt);
	virtual void MoveStrafe(float amt);

	virtual void AddForce(FXMVECTOR force);
	virtual void HitGround();
	virtual void LeaveGround()
	{
		mGrounded = false;
	}

protected:
	float mHealth;
	XMFLOAT3 mVelocity;
	float mSpeed;
	float mSprintSpeed;

	XMVECTOR mVelocityApplied;

	bool mGrounded;

};

