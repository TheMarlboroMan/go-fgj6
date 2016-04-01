#ifndef EXPORTADOR_H
#define EXPORTADOR_H

#include <class/dnot_token.h>
#include "decoracion.h"
#include "obstaculo.h"

namespace App
{
class Exportador
{
	public:

	std::string serializar(const std::vector<Obstaculo>&, const std::vector<Decoracion>&);

	private:

	Herramientas_proyecto::Dnot_token 		serializar_obstaculo(const Obstaculo&);
	Herramientas_proyecto::Dnot_token 		serializar_decoracion(const Decoracion&);
	Herramientas_proyecto::Dnot_token 		serializar_punto(DLibH::Punto_2d<double>);

	Herramientas_proyecto::Dnot_token::t_vector	generar_punto(double x, double y);
	Herramientas_proyecto::Dnot_token::t_vector	generar_color(int r, int g, int b, int a);
};
}
#endif
