#pragma once
#include "SDL_rect.h"
#include <cmath>
namespace Functions
{
	// R-функциональное пересечение
	float RAnd(float f1, float f2)
	{
		return f1 + f2 - std::sqrt(powf(f1, 2) + powf(f2, 2));
	}

	//Прямоугольный треугольник
	float DrawTriangle(SDL_Point position, SDL_Point args)
	{
		float cat1 = -(args.y - position.y);
		float cat2 = (position.x - args.x);
		float hypotenuse = (args.y - position.y) - (position.x - args.x) + 100;

		return RAnd(RAnd(cat1, cat2), RAnd(cat2, hypotenuse));
	}
}
