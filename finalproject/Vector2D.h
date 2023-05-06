#pragma once
#include <iostream>

class Vector2D
{
public:
	float X, Y;
	Vector2D(float x = 0, float y = 0) : X(x), Y(y) {}
	inline Vector2D operator+(const Vector2D& v2) const { return Vector2D(X + v2.X, Y + v2.Y); }
	inline Vector2D operator-(const Vector2D& v2) const { return Vector2D(X - v2.X, Y - v2.Y); }
	inline Vector2D operator*(const float a) const { return Vector2D(X * a, Y * a); }

	void Log(std::string mag = "") 
	{
		std::cout << mag << "X Y = " << X << " " << Y << std::endl;
	}
};

