#include "ColorUtil.h"

#include <math.h>

namespace funk
{
	const float PI = 3.14159f;

	v3 HSVtoRGB( v3 HSV )
	{
		v3 RGB;

		const float Chroma = HSV.y * HSV.z;
		const float Hdash = (HSV.x*180.0f/PI) / 60.0f;
		const float X = Chroma * (1.0f - fabs(fmod(Hdash, 2.0f) - 1.0f));

		if(Hdash < 1.0f)
		{
			RGB.x = Chroma;
			RGB.y = X;
		}
		else if(Hdash < 2.0f)
		{
			RGB.x = X;
			RGB.y = Chroma;
		}
		else if(Hdash < 3.0f)
		{
			RGB.y = Chroma;
			RGB.z = X;
		}
		else if(Hdash < 4.0f)
		{
			RGB.y= X;
			RGB.z = Chroma;
		}
		else if(Hdash < 5.0f)
		{
			RGB.x = X;
			RGB.z = Chroma;
		}
		else if(Hdash < 6.0f)
		{
			RGB.x = Chroma;
			RGB.z = X;
		}

		float Min = HSV.z - Chroma;

		RGB.x += Min;
		RGB.y += Min;
		RGB.z += Min;

		return RGB;
	}

	v3 RGBtoHSV( v3 RGB )
	{
		v3 HSV;

		float Min = min(min(RGB.x, RGB.y), RGB.z);
		float Max = max(max(RGB.x, RGB.y), RGB.z);
		float Chroma = Max - Min;

		//If Chroma is 0, then S is 0 by definition, and H is undefined but 0 by convention.
		if(Chroma != 0)
		{
			if(RGB.x == Max)
			{
				HSV.x = (RGB.y - RGB.z) / Chroma;

				if(HSV.x < 0.0f)
				{
					HSV.x += 6.0f;
				}
			}
			else if(RGB.y == Max)
			{
				HSV.x = ((RGB.z - RGB.x) / Chroma) + 2.0f;
			}
			else //RGB.z == Max
			{
				HSV.x = ((RGB.x - RGB.y) / Chroma) + 4.0f;
			}

			HSV.x *= 60.0f;
			HSV.y = Chroma / Max;
		}

		HSV.z = Max;
		HSV.x = HSV.x*PI/180.0f;

		return HSV;
	}
}