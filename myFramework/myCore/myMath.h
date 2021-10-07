#pragma once
#include <cmath>

#include "SimpleMath.h"
using namespace DirectX;
using namespace SimpleMath;

class myMath
{
public:
	static const float PI;
	static const float EPSILON;
	static const float PI2M;
	static const float PI2D;
	static const float PI4D;
	static const float PI8D;
	static const float PI16D;
	static const float PI32D;
	static const float PI64D;
	static const float PI128D;
	
	static float RA_TO_DE(float f)
	{
		return (180.0f / PI * f);
	}
	
	static float DE_TO_RA(float f)
	{
		return (PI / 180.0f * f);
	}
	
	static int FLOAT_TO_INT(float f)
	{
		return static_cast<int>(f + EPSILON);
	}
	
	static bool FLOAT_EQUAL(float f1, float f2)
	{
		if (fabs(f1 - f2) <= EPSILON)
			return true;
		return false;
	}
};

struct float2
{
	union
	{
		struct { float x, y; };
		float v[2];
	};
};

struct float3
{
	union
	{
		struct { float x, y, z; };
		float v[3];
	};
};
struct float4
{
	union
	{
		struct { float x, y, z, w; };
		float v[4];
	};
};

struct myPoint : float2
{
	myPoint() = default;
	myPoint(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
	float& operator [] (int index)
	{
		if (index == 0) return x;
		return y;
	}
	float* get()
	{
		return v;
	}
	void operator = (float* arr)
	{
		x = arr[0];
		y = arr[1];
	}

	bool operator == (myPoint& p)
	{
		if (myMath::FLOAT_EQUAL(x, p.x))
		{
			if (myMath::FLOAT_EQUAL(y, p.y))
			{
				return true;
			}
		}
		return false;
	}
	bool operator != (myPoint& p)
	{
		if (myMath::FLOAT_EQUAL(x, p.x))
		{
			if (myMath::FLOAT_EQUAL(y, p.y))
			{
				return false;
			}
		}
		return true;
	}
	myPoint operator + (myPoint p)
	{
		myPoint pt;
		pt.x = x + p.x;
		pt.y = y + p.y;
		return pt;
	}
	myPoint operator + (float fValue)
	{
		myPoint pt;
		pt.x = x + fValue;
		pt.y = y + fValue;
		return pt;
	}
	myPoint operator - (myPoint p)
	{
		myPoint pt;
		pt.x = x - p.x;
		pt.y = y - p.y;
		return pt;
	}
	myPoint operator - (float fValue)
	{
		myPoint pt;
		pt.x = x - fValue;
		pt.y = y - fValue;
		return pt;
	}
	myPoint operator * (float fValue)
	{
		myPoint pt;
		pt.x = x * fValue;
		pt.y = y * fValue;
		return pt;
	}
	myPoint operator * (myPoint p)
	{
		myPoint pt;
		pt.x = x * p.x;
		pt.y = y * p.y;
		return pt;
	}
	float Length()
	{
		return sqrt(x * x + y * y);
	}
	myPoint Normalize()
	{
		float fLength = Length();
		if (fLength == 0.0f)
			fLength = myMath::EPSILON;
		x /= fLength;
		y /= fLength;
		return myPoint(x, y);
	}
	float GetAngle(myPoint& pA, myPoint& pB)
	{
		myPoint pt = pB - pA;
		float dist = pt.Length();
		float angle = acosf(pt.x / dist);

		if (pt.y > 0)
			angle = myMath::PI2M - angle;
		return angle;
	}
};