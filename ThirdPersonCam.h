#pragma once
#include "BaseCamera.h"

class GraphicalObject;

class ThirdPersonCam : public BaseCamera
{
public:
	ThirdPersonCam(void);
	ThirdPersonCam(GraphicalObject* target, float offsetLook,
					float offsetUp, float targetYOffset);
	~ThirdPersonCam(void);

	virtual void Update();

	void SetTarget(GraphicalObject* target)
	{
		mTarget = target;
	}

protected:
	float mOffsetLook;
	float mOffsetUp;
	float mTargetYOffset;
	GraphicalObject* mTarget;
};

