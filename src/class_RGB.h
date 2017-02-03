#include <math.h>
using namespace std;

class RGB
{
public:
	unsigned char R;
	unsigned char G;
	unsigned char B;

	RGB(unsigned char r, unsigned char g, unsigned char b)
	{
		R = r;
		G = g;
		B = b;
	}

	bool Equals(RGB rgb)
	{
		return (R == rgb.R) && (G == rgb.G) && (B == rgb.B);
	}
};

class HSV
{
public:
	double H;
	double S;
	double V;

	HSV(double h, double s, double v)
	{
		H = h;
		S = s;
		V = v;
	}

	bool Equals(HSV hsv)
	{
		return (H == hsv.H) && (S == hsv.S) && (V == hsv.V);
	}
};

static RGB HSVToRGB(HSV hsv) {
	double r = 0, g = 0, b = 0;

	if (hsv.S == 0)
	{
		r = hsv.V;
		g = hsv.V;
		b = hsv.V;
	}
	else
	{
		int i;
		double f, p, q, t;

		if (hsv.H == 360)
			hsv.H = 0;
		else
			hsv.H = hsv.H / 60;

		i = (int)trunc(hsv.H);
		f = hsv.H - i;

		p = hsv.V * (1.0 - hsv.S);
		q = hsv.V * (1.0 - (hsv.S * f));
		t = hsv.V * (1.0 - (hsv.S * (1.0 - f)));

		switch (i)
		{
		case 0:
			r = hsv.V;
			g = t;
			b = p;
			break;

		case 1:
			r = q;
			g = hsv.V;
			b = p;
			break;

		case 2:
			r = p;
			g = hsv.V;
			b = t;
			break;

		case 3:
			r = p;
			g = q;
			b = hsv.V;
			break;

		case 4:
			r = t;
			g = p;
			b = hsv.V;
			break;

		default:
			r = hsv.V;
			g = p;
			b = q;
			break;
		}

	}

	return RGB((unsigned char)(r * 255), (unsigned char)(g * 255), (unsigned char)(b * 255));
}