#ifndef DEF_COLOR_H
#define DEF_COLOR_H

namespace App
{

struct tcolor
{
	int r, g, b, a;

	bool operator==(const tcolor& o) const
	{
		return o.r==r &&
			o.g==g &&
			o.b==b &&
			o.a==a;
	}

	bool operator!=(const tcolor& o) const
	{
		return o.r!=r ||
			o.g!=g ||
			o.b!=b ||
			o.a!=a;
	}
};

}

#endif
