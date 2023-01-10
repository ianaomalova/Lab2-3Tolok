#include <iostream>
#include "AppWindow.h"
#include "Examples.h"
#include "Functions.h"
#include "imgui.h"
#include "Texture.h"
class Window : public AppWindow
{
public:
	Window(SDL_Point windowSize) :
		AppWindow(windowSize),
		texture(GetRenderer(), windowSize)
	{
	}
	void Render() override
	{
		texture.Render();
	}
	void RenderGui() override
	{
		Example(texture);
	}
	Texture texture;
};
int main(int argc, char** argv)
{
	Window window({ 800, 600 });
	window.Show();
	return 0;
}
