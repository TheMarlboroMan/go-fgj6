#ifndef MENSAJES_H
#define MENSAJES_H

#include <string>
#include <vector>

#include <def_video.h>

namespace App
{

class Widget_mensajes
{
	public:

					Widget_mensajes(const DLibV::Fuente_TTF&, size_t, int, int);
	void				insertar_mensaje(const std::string&, float);
	void				dibujar(DLibV::Pantalla&);
	void				turno(float);

	private:

	struct Mensaje
	{
		Mensaje(const DLibV::Fuente_TTF& f, const std::string& c, float t)
			:tiempo(t), txt(f, {255, 255, 255, 255}, c)
		{}

		float				tiempo;
		DLibV::Representacion_TTF 	txt;
	};

	const DLibV::Fuente_TTF&	fuente;
	std::vector<Mensaje> 		mensajes;
	size_t				max_mensajes;
	int 				x, y;
};

}

#endif
