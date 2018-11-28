#pragma once

class RawButton {
public:
	RawButton(int key, int scancode, int action, int mods): key_(key), scancode_(scancode), action_(action), mods_(mods) {}
	int key_;
	int scancode_;
	int action_;
	int mods_;

};

class RawAxis {
public:
	RawAxis(int xpos, int ypos) : mouseX_(xpos), mouseY_(ypos) {};
	int mouseX_;
	int mouseY_;
};