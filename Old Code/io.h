short keyOldState[256] = { 0 };
short keyNewState[256] = { 0 };

struct sKeyState
{
	char bPressed;
	char bReleased;
	char bHeld;
} keys[256];

char getKeys[256] = { 0 };