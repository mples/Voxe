#include "Color.h"


Color::Color() : color_(0) {
}

Color::Color(unsigned int color) : color_(color) {
}

Color::Color(BYTE r, BYTE g, BYTE b) : Color(r, g, b, 255){

}

Color::Color(BYTE r, BYTE g, BYTE b, BYTE a) {
	rgba_[0] = r;
	rgba_[1] = g;
	rgba_[2] = b;
	rgba_[3] = a;
}

Color::Color(const Color & other) : Color(other.color_){
}


Color::~Color() {
}

Color & Color::operator=(const Color & other) {
	color_ = other.color_;
	return *this;
}

bool Color::operator==(const Color & other) {
	return color_ == other.color_;
}

bool Color::operator!=(const Color & other) {
	return color_ != other.color_;
}

BYTE Color::getR() {
	return rgba_[0];
}

void Color::setR(BYTE r) {
	rgba_[0] = r;
}

BYTE Color::getG() {
	return rgba_[1];
}

void Color::setG(BYTE g) {
	rgba_[1] = g;
}

BYTE Color::getB() {
	return rgba_[2];
}

void Color::setB(BYTE b) {
	rgba_[2] = b;
}

BYTE Color::getA() {
	return rgba_[3];
}

void Color::setA(BYTE a) {
	rgba_[3] = a;
}
