#pragma once
#include "character.h"
class Player :
	public Character
{
private:
	const float JUMP_FORCE;
public:
	Player(void) : JUMP_FORCE(100.0f)
	{}

	/*Player(FXMVECTOR pos, FXMVECTOR look, FXMVECTOR up,
					ID3D11Device* device, LitTexEffect* effect, std::string filename,
					bool isRHS = false, bool isVFlipped = false,
					float speed = 0.0f, float sprintSpeed = 0.0f, float health = 0.0f)
					: Character(pos, look, up, device, effect, filename, isRHS,
									  isVFlipped, speed, sprintSpeed, health),
						JUMP_FORCE(10.0f)
	{
	}*/

	Player(FXMVECTOR pos, FXMVECTOR look, FXMVECTOR up, BasicModel& model,
		float speed = 0.0f, float sprintSpeed = 0.0f, float health = 0.0f)
		: Character(pos, look, up, model, speed, sprintSpeed, health),
		JUMP_FORCE(10.0f)
	{
	}

	~Player(void);

	void Jump();

};

