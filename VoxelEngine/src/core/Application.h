#pragma once
class Application {
public:
	Application();
	~Application();

	void run();
private:
	bool shouldClose_ = false;
};

