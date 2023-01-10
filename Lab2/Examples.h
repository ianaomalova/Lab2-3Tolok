#pragma once
#include "Functions.h"
#include "Texture.h"

void Example(Texture& texture)
{
	// Задаем позицию нашей фигуры
	const SDL_Point trianPos = { texture.GetSize().x / 2,
	texture.GetSize().y / 2 };
	float minVal = 0, maxVal = 0;

	// Проходимся по массиву пикселей в текстуре
	for (int y = 0; y < texture.GetSize().y; ++y)
	{
		for (int x = 0; x < texture.GetSize().x; ++x)
		{
			// Расчитываем площадку из трех значений
			int x1 = x, y1 = y;
			int x2 = x, y2 = y + 1;
			int x3 = x + 1, y3 = y;
			float z1 = Functions::DrawTriangle(trianPos, SDL_Point{ x1, y1 });
			float z2 = Functions::DrawTriangle(trianPos, SDL_Point{ x2, y2 });
			float z3 = Functions::DrawTriangle(trianPos, SDL_Point{ x3, y3 });

			// Дискременанты
			float Ai = y1 * (z2 - z3) - y2 * (z1 - z3) + y3 * (z1 - z2);
			float Bi = -(x1 * (z2 - z3) - x2 * (z1 - z3) + x3 * (z1 - z2));
			float Ci = x1 * (y2 - y3) - x2 * (y1 - y3) + x3 * (y1 - y2);
			float Di = x1 * (y2 * z3 - y3 * z2) - x2 * (y1 * z3 - y3 * z1) + x3 *
				(y1 * z2 - y2 * z1);

			// Компоненты нормалей к площадке
			float Nx = Ai / std::sqrtf(Ai * Ai + Bi * Bi + Ci * Ci);
			float Ny = -Bi / std::sqrtf(Ai * Ai + Bi * Bi + Ci * Ci);
			float Nz = -Ci / std::sqrtf(Ai * Ai + Bi * Bi + Ci * Ci);
			float Nt = Di / std::sqrtf(Ai * Ai + Bi * Bi + Ci * Ci + Di * Di);

			// Рисуем значения нормали по оси Y
			Uint8 Cx = (Ny + 1.f) * 127.f;
			int isPositive = (z1 >= 0) + (z2 >= 0) + (z3 >= 0);
			if (isPositive > 2) // больше половины значений положительные
				texture.SetPixel({ x, y }, SDL_Color{ Cx, 0, 0, 255 });
			else
				texture.SetPixel({ x, y }, SDL_Color{ 0, 0, Cx, 255 });
		}
	}
	texture.UpdateTexture();
}
