#pragma once
#include "ProgramState.h"

using namespace std;

class MapState : public ProgramState
{
public:
	MapState(::Program& program);
	virtual ~MapState();
	

protected:
	int CameraX, CameraY, CameraWidth, CameraHeight;

	virtual void Input(char key, bool isCommand);
	virtual void OnInitialize() override;
	virtual void OnUpdate(char key, bool isCommand) override;
	virtual void OnDraw() override;

	void MoveCameraTo(int x, int y);
	void CenterCameraOn(int x, int y);

private:
	bool MovePlayerBy(int dx, int dy);
};

