#pragma once
#include "../Common.hpp"
#include "../GTA/Invoker/Types.hpp"

namespace Sentinel::Math
{
	inline NativeVector3 Multiply(NativeVector3 vector, float inc)
	{
		vector.x *= inc;
		vector.y *= inc;
		vector.z *= inc;
		return vector;
	}

	inline NativeVector3 Add(NativeVector3 vector, NativeVector3 vector2)
	{
		vector.x += vector2.x;
		vector.y += vector2.y;
		vector.z += vector2.z;
		return vector;
	}

	inline NativeVector3 Subtract(NativeVector3 vector, NativeVector3 vector2)
	{
		vector.x -= vector2.x;
		vector.y -= vector2.y;
		vector.z -= vector2.z;
		return vector;
	}

	inline bool Empty(const NativeVector3& vector)
	{
		return (vector.x == 0.f && vector.y == 0.f && vector.z == 0.f);
	}

	inline float DistanceBetweenVectors(NativeVector3 a, NativeVector3 b)
	{
		return (float)sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2) + pow((a.z - b.z), 2));
	}

	inline float DegreeToRadian(float deg)
	{
		double radian = (3.14159265359 / 180) * deg;
		return (float)radian;
	}
}