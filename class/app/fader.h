#ifndef FADER_H
#define FADER_H

#include <algorithm>

#include <templates/valor_limitado.h>

namespace App
{
template <typename T>
class Fader
{
	public:

			Fader():
				val(0, 0, 0), destino(0), salto(0)
	{

	}


			Fader(T a, T d, T s):
				val(std::min(a, d), std::max(a, d), a),
				destino(d), salto(s)
	{

	}

	operator T() const{return val;}

	void		reset(T a, T d, T s)
	{
		val=Herramientas_proyecto::Valor_limitado<T>(std::min(a, d), std::max(a, d), a);
		destino=d;
		salto=s;
	}

	void 		turno(float delta)
	{
		T sal=delta * salto;
		if(val > destino)
		{
			val-=sal;
		}
		else if(val < destino)
		{
			val+=sal;
		}
	}

	bool		es_finalizado() const {return val==destino;}

	private:

	Herramientas_proyecto::Valor_limitado<T>	val;
	T						destino,
							salto;
};
}

#endif
