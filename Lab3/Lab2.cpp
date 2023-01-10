﻿#include <iostream>
#include "AppWindow.h"
#include "Examples.h"
#include "Functions.h"
#include "imgui.h"
#include "Texture.h"
#include "MImageCalculator.h"


float MyAwesomeFunction(SDL_Point args)
{
	const SDL_Point pos = { 400, 200 };

	float cat1 = -(args.y - pos.y);
	float cat2 = (pos.x - args.x);
	float hypotenuse = (args.y - pos.y) - (pos.x - args.x) + 100;

	return Functions::RAnd(Functions::RAnd(cat1, cat2), Functions::RAnd(cat2, hypotenuse));

}

class Window: public AppWindow
{
public:
	Window(SDL_Point windowSize) :
		AppWindow(windowSize),
		texture(GetRenderer(), windowSize),
		calculator(&MyAwesomeFunction) // передаем функцию которую будем рассчитывать
		
	{
		ComputeFunction();
	}
	
	void Render() override
	{
		texture.Render();
	}
	
	void RenderGui() override
	{
		ImGui::Begin("MyWindow");
		ImGui::Text("Mouse position");
		ImGui::Text("\t%d, %d", mousePosition.x, mousePosition.y); 
		ImGui::Text("Mouse MImage data	");
		ImGui::Text("\t%f, %f", data.nx, data.ny); 
		ImGui::End();
	}
	
	void ProcessEvent(const SDL_Event& e) override
	{
		if (e.type == SDL_MOUSEMOTION)
		{
			mousePosition.x = e.motion.x;
			mousePosition.y = e.motion.y;
			data = calculator.GetPixelData({ mousePosition.x, mousePosition.y = e.motion.y });
		}
		else if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			if (e.button.button == SDL_BUTTON_RIGHT)
			{
				SDL_Point startPoint;
				startPoint.x = e.button.x;
				startPoint.y = e.button.y;
				GradientDescent(startPoint, 2);
			}
		}
	}

	void ComputeFunction()
	{
		std::vector<std::vector<MImagePixelData>> data = calculator.GetSpaceData(texture.GetSize());
		for (size_t i = 0; i < data.size(); ++i)
		{
			for (size_t j = 0; j < data[i].size(); ++j)
			{
				MImagePixelData mimageData = data[i][j];
				uint8_t colorValue = (mimageData.nx + 1.f) * 127.f;
				if (mimageData.zone == FunctionZone::Positive)
					texture.SetPixel(SDL_Point{ (int)i, (int)j }, SDL_Color{ colorValue, 0, 0, 255 });
				else if (mimageData.zone == FunctionZone::Negative)
					texture.SetPixel(SDL_Point{ (int)i, (int)j}, SDL_Color{ 0, 0, colorValue, 255 });
			}
		}
		texture.UpdateTexture();
	}
	


	void GradientDescent(SDL_Point startPoint, int step, SDL_Color pathColor = { 0, 255, 0, 255 })
	{
		// Спускаемся пока не выйдем за пределы пространства текстуры
		while (startPoint.x >= 0 && startPoint.x < texture.GetSize().x &&
			startPoint.y >= 0 && startPoint.y < texture.GetSize().y)
		{
			// Проверяем, не попали ли мы в точку, которая уже была рассчитана
			SDL_Color newColor = texture.GetColor({ startPoint.x, startPoint.y });
			if (newColor.r == pathColor.r &&
				newColor.g == pathColor.g &&
				newColor.b == pathColor.b &&
				newColor.a == pathColor.a)
			{
				break;
			}

			// Окрашивание пути
			texture.SetPixel({ startPoint.x, startPoint.y }, pathColor);

			// Рассчет ЛГХ в текущей точке
			MImagePixelData data = calculator.GetPixelData({ startPoint.x, startPoint.y });
			// Движение по направлению убывания с шагом
			startPoint.x -= step * data.nx;
			startPoint.y += step * data.ny;
		}
		texture.UpdateTexture();
	}


	Texture texture;
	MImageCalculator calculator;
	SDL_Point mousePosition;
	MImagePixelData data;
};


int main(int argc, char** argv)
{
	Window window({800, 600});
	
	window.Show();
	
	return 0;
}