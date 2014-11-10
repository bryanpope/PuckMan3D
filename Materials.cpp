
#include "Materials.h"


// Ambient, Diffuse, Specular, Reflect
const Material Materials::GRID = Material(XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f), XMFLOAT4(0.01f, 0.01f, 0.01f, 0.0f), XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f), XMFLOAT4(0.01f, 0.01f, 0.01f, 1.0f));
const Material Materials::BOX = Material(XMFLOAT4(0.12f, 0.12f, 0.6f, 1.0f), XMFLOAT4(0.12f, 0.12f, 0.6f, 1.0f), XMFLOAT4(0.2f, 0.2f, 0.2f, 16.0f), XMFLOAT4(0.01f, 0.01f, 0.01f, 1.0f));
const Material Materials::PELLET = Material(XMFLOAT4(1.0f, 0.72f, 0.68f, 1.0f), XMFLOAT4(1.0f, 0.72f, 0.68f, 1.0f), XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f), XMFLOAT4(0.01f, 0.01f, 0.01f, 1.0f));
const Material Materials::POWERUP = Material(XMFLOAT4(1.0f, 0.9f, 0.9f, 1.0f), XMFLOAT4(1.0f, 0.9f, 0.9f, 1.0f), XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f), XMFLOAT4(0.01f, 0.01f, 0.01f, 1.0f));
const Material Materials::PACMAN = Material(XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 0.01f, 1.0f), XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f), XMFLOAT4(0.01f, 0.01f, 0.01f, 1.0f));
const Material Materials::BLINKY = Material(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 0.01f, 0.01f, 1.0f), XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f), XMFLOAT4(0.01f, 0.01f, 0.01f, 1.0f));
const Material Materials::INKY = Material(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT4(0.01f, 0.41f, 0.88f, 1.0f), XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f), XMFLOAT4(0.01f, 0.01f, 0.01f, 1.0f));
const Material Materials::PINKY = Material(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 0.01f, 1.0f, 1.0f), XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f), XMFLOAT4(0.01f, 0.01f, 0.01f, 1.0f));
const Material Materials::CLYDE = Material(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 0.45f, 0.01f, 1.0f), XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f), XMFLOAT4(0.01f, 0.01f, 0.01f, 1.0f));
const Material Materials::CHERRY = Material(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f), XMFLOAT4(0.01f, 0.01f, 0.01f, 1.0f));
const Material Materials::PEACH = Material(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT4(0.9f, 0.7f, 0.6f, 1.0f), XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f), XMFLOAT4(0.01f, 0.01f, 0.01f, 1.0f));
const Material Materials::APPLE = Material(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT4(0.7f, 1.0f, 0.0f, 1.0f), XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f), XMFLOAT4(0.01f, 0.01f, 0.01f, 1.0f));
const Material Materials::GRAPES = Material(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT4(0.1f, 0.0f, 1.0f, 1.0f), XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f), XMFLOAT4(0.01f, 0.01f, 0.01f, 1.0f));
const Material Materials::GHOSTFRIGHTENED = Material(XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT4(0.01f, 0.01f, 1.0f, 1.0f), XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f), XMFLOAT4(0.01f, 0.01f, 0.01f, 1.0f));
const Material Materials::GHOSTFLASHING = Material(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f), XMFLOAT4(0.01f, 0.01f, 0.01f, 1.0f));
const Material Materials::TRIGGER = Material(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT4(0.0f, 0.9f, 0.0f, 1.0f), XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f), XMFLOAT4(0.01f, 0.01f, 0.01f, 1.0f));
const Material Materials::TRAPACTIVE = Material(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT4(0.9f, 0.0f, 0.0f, 1.0f), XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f), XMFLOAT4(0.01f, 0.01f, 0.01f, 1.0f));
const Material Materials::TRAPINACTIVE = Material(XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f), XMFLOAT4(0.01f, 0.01f, 0.01f, 1.0f), XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f), XMFLOAT4(0.91f, 0.91f, 0.91f, 1.0f));
/*mPowerUpMat.Ambient = XMFLOAT4(1.0f, 0.9f, 0.9f, 1.0f);
mPowerUpMat.Diffuse = XMFLOAT4(1.0f, 0.9f, 0.9f, 1.0f);
mPowerUpMat.Specular = XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f);

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
mGrapesMat.Specular = XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f);*/

