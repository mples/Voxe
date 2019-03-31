#pragma once

struct MousePoint {
	int x;
	int y;
};

class MouseEvent {
public:
	enum Type {
		INVALID,
		LMB_PRESS,
		LMP_RELEASE,
		RMB_PRESS,
		RMB_RELEASE,
		MMB_PRESS,
		MMB_RELEASE,
		MWHEEL_UP,
		MWHEEL_DOWN,
		MOVE,
		RAW_MOVE
	};
	MouseEvent();
	MouseEvent(const Type type, const int x, const int y);
	~MouseEvent();
	bool isValid();
	Type getType();
	MousePoint getPos();
	int getPosX();
	int getPosY();
private:
	Type type_;
	int x_;
	int y_;
};

