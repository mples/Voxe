#pragma once

typedef unsigned char BYTE;

class Color {
public:
	Color();
	Color(unsigned int color);
	Color(BYTE r, BYTE g, BYTE b);
	Color(BYTE r, BYTE g, BYTE b, BYTE a);
	Color(const Color& other);
	~Color();
	Color& operator=(const Color& other);
	bool operator==(const Color& other);
	bool operator!=(const Color& other);

	BYTE getR();
	void setR(BYTE r);

	BYTE getG();
	void setG(BYTE g);

	BYTE getB();
	void setB(BYTE b);

	BYTE getA();
	void setA(BYTE a);
private:
	union  {
		BYTE rgba_[4];
		unsigned int color_;
	};
};

namespace Colors {
	const Color UnloadedColor(100, 100, 100);
	const Color UnhandledColor(255, 0 , 0);
}