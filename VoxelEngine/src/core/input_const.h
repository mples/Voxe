#pragma once

enum Action {
	MOVE_RIGHT,
	MOVE_LEFT,
	MOVE_FRONT,
	MOVE_BACK
};

enum State {
	MOVING_FRONT,
	MOVING_BACK,
	MOVING_LEFT,
	MOVING_RIGHT

};

enum Range {
	LOOK_X,
	LOOK_Y
};