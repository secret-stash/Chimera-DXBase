#pragma once
#include <cstdint>

using Any = int;
using Hash = std::uint32_t;
using Entity = std::int32_t;
using Player = std::int32_t;
using FireId = std::int32_t;
using Interior = std::int32_t;
using Ped = Entity;
using Vehicle = Entity;
using Cam = std::int32_t;
using Object = Entity;
using Pickup = Object;
using Blip = std::int32_t;
using Camera = Entity;
using ScrHandle = Entity;

#pragma pack(push, 1)
struct NativeVector3
{
public:
	NativeVector3() = default;
	NativeVector3(float x, float y, float z) :
		x(x), y(y), z(z)
	{}
public:
	float x{};
private:
	char paddingX[4];
public:
	float y{};
private:
	char paddingY[4];
public:
	float z{};
private:
	char paddingZ[4];
};
#pragma pack(pop)

#pragma pack(push, 1)
struct ImageDimensions
{
	int x, y;
	ImageDimensions() { x = y = 0; }
	ImageDimensions(int _x, int _y) { x = _x; y = _y; }
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Color
{
	std::uint8_t r{ 255 };
	std::uint8_t g{ 255 };
	std::uint8_t b{ 255 };
	std::uint8_t a{ 255 };
};
#pragma pack(pop)