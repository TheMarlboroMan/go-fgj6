#include "pieza_animacion.h"

#include <video/representacion/representacion_grafica/representacion_bitmap/representacion_bitmap.h>
#include <video/gestores/gestor_texturas.h>

#include "recursos.h"

using namespace App;

Pieza_animacion::Pieza_animacion()
	:estado{estados::inactiva}, id_pieza_actual(0), factor_tam(1.f), alpha(255.f), tiempo(0.0f)
{
	formar_poligono();
}

void Pieza_animacion::activar(int id_p, Espaciable::tpunto c, Espaciable::tpunto pos)
{
	estado=estados::movimiento;
	id_pieza_actual=id_p;
	destino=c;
	establecer_posicion(pos);

	angulo=DLibH::obtener_para_puntos_cartesiano<double>(pos.x, pos.y, destino.x, destino.y).angulo_grados();
}

void Pieza_animacion::turno(float delta)
{
	tiempo+=delta;
	if(tiempo >=.035f) tiempo=0.0f;

	switch(estado)
	{
		case estados::inactiva:

		break;
		
		case estados::movimiento:
			desplazar_angulo_velocidad(angulo, 40.0*delta);

			if(DLibH::punto_en_poligono<double>(acc_poligono(), destino))
			{
				estado=estados::crecimiento;
			}
		break;

		case estados::crecimiento:

			factor_tam+=delta;
			alpha-=60.*delta;

			if(alpha <= 1.f)
			{
				estado=estados::inactiva;
			}
		break;
	}
}

void Pieza_animacion::dibujar(Representador& r, DLibV::Pantalla& pantalla, const DLibV::Camara& camara) const
{
	auto c=poligono.acc_centro();

	DLibV::Representacion_bitmap flare(DLibV::Gestor_texturas::obtener(r_graficos::g_lens_flare));
	DLibV::Representacion_bitmap bmp(DLibV::Gestor_texturas::obtener(r_graficos::g_sprites));

	flare.establecer_modo_blend(DLibV::Representacion::BLEND_ALPHA);
	flare.establecer_alpha(255);
	bmp.establecer_posicion(c.x - (25 * factor_tam), -c.y - (25 * factor_tam), 30 * factor_tam, 30 *  factor_tam);
	flare.volcar(pantalla, camara);

	bmp.establecer_modo_blend(DLibV::Representacion::BLEND_ALPHA);
	bmp.establecer_recorte((id_pieza_actual-1) * 30, 0, 30, 30);
	bmp.establecer_alpha(alpha);
	bmp.establecer_posicion(c.x - (15 * factor_tam), -c.y - (15 * factor_tam), 30 * factor_tam, 30 *  factor_tam);
	bmp.volcar(pantalla, camara);
}

void Pieza_animacion::formar_poligono()
{
	poligono.insertar_vertice({-15.0, 15.0});
	poligono.insertar_vertice({15.0, 15.0});
	poligono.insertar_vertice({15.0, -15.0});
	poligono.insertar_vertice({-15.0, -15.0});
	poligono.cerrar();
	poligono.mut_centro({0.0, 0.0});
}

void Pieza_animacion::reiniciar()
{
	alpha=255.f;
	factor_tam=1.0;
	id_pieza_actual=0;
	angulo=0.0;
}
