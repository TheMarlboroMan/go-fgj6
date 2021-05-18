#ifndef TIEMPO_H
#define TIEMPO_H


#include <string>
#include <templates/valor_limitado.h>

namespace App
{

class Tiempo
{
	public:

				Tiempo();

	void			reiniciar();
	void			turno(float);
	void			penalizar();
	bool			es_aviso() const {return t_aviso;}
	std::string		a_cadena();

	private:

	float			tiempo;
	Herramientas_proyecto::Valor_limitado<float>	t_aviso;
};

}

#endif
