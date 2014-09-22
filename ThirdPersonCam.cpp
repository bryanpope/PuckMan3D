#include "ThirdPersonCam.h"
#include "GraphicalObject.h"


ThirdPersonCam::ThirdPersonCam(void) : BaseCamera(),
	mOffsetLook(0.0f),
	mOffsetUp(0.0f),
	mTarget(0),
	mTargetYOffset(0.0f)
{
}

ThirdPersonCam::ThirdPersonCam(GraphicalObject* target, float offsetLook,
					float offsetUp, float targetYOffset) : BaseCamera(),
					mOffsetLook(offsetLook),
					mOffsetUp(offsetUp),
					mTarget(target),
					mTargetYOffset(targetYOffset)
{
}



ThirdPersonCam::~ThirdPersonCam(void)
{
}

void ThirdPersonCam::Update()
{
	if(mTarget != NULL)
	{
		XMVECTOR targetPos =  mTarget->GetPos();
		XMVECTOR targetOffset = XMVectorSet(0.0f, mTargetYOffset, 0.0f, 0.0f);
		targetPos = targetPos + targetOffset;

		XMVECTOR pos = targetPos - (mTarget->GetLook() * mOffsetLook)
			+ (mTarget->GetUp() * mOffsetUp);
		XMStoreFloat3(&mPos, pos);

		XMVECTOR look = targetPos - pos;
		look = XMVector3Normalize(look);
		XMStoreFloat3(&mLook, look);

		XMStoreFloat3(&mUp, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	}
	BaseCamera::Update();
}
