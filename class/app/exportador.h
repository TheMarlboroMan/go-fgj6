#ifndef EXPORTADOR_H
#define EXPORTADOR_H

#include <class/dnot_token.h>
#include "mapa.h"

namespace App
{
class Exportador
{
	public:

	std::string serializar(Mapa&);

	private:

	Herramientas_proyecto::Dnot_token 		serializar(const Obstaculo&);
	Herramientas_proyecto::Dnot_token 		serializar(const Decoracion&);
	Herramientas_proyecto::Dnot_token 		serializar(const Inicio&);
	Herramientas_proyecto::Dnot_token 		serializar(const Salida&);
	Herramientas_proyecto::Dnot_token 		serializar(const Pieza&);
	Herramientas_proyecto::Dnot_token 		serializar(const Interruptor&);
	Herramientas_proyecto::Dnot_token 		serializar(const Puerta&);
	Herramientas_proyecto::Dnot_token 		serializar(const Mejora_velocidad&);
	Herramientas_proyecto::Dnot_token 		serializar(const Arbol&);

	Herramientas_proyecto::Dnot_token::t_vector	generar_punto(double x, double y);
	Herramientas_proyecto::Dnot_token::t_vector	generar_color(int r, int g, int b, int a);
};
}
#endif
