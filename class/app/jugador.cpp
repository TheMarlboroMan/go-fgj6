#include "jugador.h"

#include <video/representacion/representacion_grafica/representacion_bitmap/representacion_bitmap.h>
#include <video/gestores/gestor_texturas.h>
#include <class/valor_limitado.h>

#include "recursos.h"

using namespace App;

const float Jugador::factor_rotacion=280.0;
const float Jugador::factor_min_rotacion=60.0;
const float Jugador::factor_aceleracion=100.0f;

Jugador::Jugador()
	:angulo(0.0), velocidad(0.0f), velocidad_destino(0.0f), indice_velocidad(0), 
	pieza_actual(0), max_velocidad(3), tiempo(0.0f), t_cola(0.0f),
	velocidades{80.0, 120.0, 160.0, 200.0, 240.0, 280.0, 320.0, 360.0, 400.0, 420.0, 460.0, 500.0, 540.0, 580.0, 620.0, 660.0}, sonido_velocidad(false)
{
	formar_poligono();
	cambiar_velocidad(0);
}

void Jugador::recibir_input(const Bloque_input& bi)
{
	input_actual=bi;
}

void Jugador::turno(float delta)
{
	sonido_velocidad=false;

	if(input_actual.giro) girar(input_actual.giro, delta);

	if(velocidad_destino != velocidad)
	{
		Herramientas_proyecto::Valor_limitado<float> velocidad_aplicar(velocidad, velocidad_destino, velocidad, Herramientas_proyecto::Valor_limitado<float>::ambos);

		if(velocidad_destino > velocidad_aplicar)
		{
			velocidad_aplicar+=factor_aceleracion * delta;
		}
		else if(velocidad_destino < velocidad_aplicar)
		{
			velocidad_aplicar-=factor_aceleracion * delta;
		}

		velocidad=velocidad_aplicar;
	}

	if(input_actual.aceleracion) cambiar_velocidad(input_actual.aceleracion);
	if(velocidad) movimiento(delta);

	tiempo+=delta;
	t_cola+=delta;

	if(t_cola > 0.05) t_cola=0.0f;
	if(tiempo > 0.15f) tiempo=0.0f;
}

void Jugador::girar(int dir, float delta)
{
	//La velocidad afecta a la capacidad de giro. No afecta parado o marcha atrás.	
	double factor=velocidad <= 0.0 ? factor_rotacion : factor_rotacion - (velocidad / 3.0);
	if(factor < factor_min_rotacion) factor=factor_min_rotacion;

	double giro=((double)delta * factor) * (double)dir;
	angulo+=giro;
	poligono.rotar(giro);
}

void Jugador::cambiar_velocidad(int dir)
{
	int indice=indice_velocidad+dir;
	if(indice < 0) indice=0;
	else if(indice >= (int) velocidades.size()) indice=velocidades.size()-1;
	else sonido_velocidad=true;

	//Limite de velocidades...
	if(indice > max_velocidad) indice=max_velocidad;
		
	indice_velocidad=indice;
	velocidad_destino=velocidades[indice_velocidad];
}

void Jugador::movimiento(float delta)
{
	desplazar_angulo_velocidad(angulo, velocidad*delta);

//	DLibH::Vector_2d<double> v=vector_unidad_para_angulo_cartesiano(angulo);
//	DLibH::Punto_2d<double> pd{v.x * velocidad, v.y * velocidad};
}

void Jugador::formar_poligono()
{
	poligono.insertar_vertice({-11.0, 6.0});
	poligono.insertar_vertice({-6.0, 11.0});
	poligono.insertar_vertice({6.0, 11.0});
	poligono.insertar_vertice({11.0, 6.0});
	poligono.insertar_vertice({11.0, -6.0});
	poligono.insertar_vertice({6.0, -11.0});
	poligono.insertar_vertice({-6.0, -11.0});
	poligono.insertar_vertice({-11.0, -6.0});

	poligono.cerrar();
	poligono.mut_centro({0.0, 0.0});
}

void Jugador::dibujar(Representador& r, DLibV::Pantalla& pantalla, const DLibV::Camara& camara) const
{
	auto c=poligono.acc_centro();

//	r.dibujar_poligono(pantalla, poligono, {255, 255, 255, 255}, camara);

	int x=0;
	if(tiempo <= 0.03f) x=0;
	else if(tiempo <= 0.06f) x=1;
	else if(tiempo <= 0.09f) x=2;
	else if(tiempo <= 0.12f) x=3;
	else if(tiempo <= 0.15f) x=4;

	DLibV::Representacion_bitmap sprite(DLibV::Gestor_texturas::obtener(r_graficos::g_viento));
	sprite.establecer_modo_blend(DLibV::Representacion::BLEND_ALPHA);
	sprite.establecer_alpha(255);
	sprite.establecer_recorte(x*30, 0, 30, 30);
	sprite.establecer_posicion(c.x-15, -c.y-15, 30, 30);
	sprite.transformar_centro_rotacion(15 / camara.acc_zoom(), 15 / camara.acc_zoom());
	sprite.transformar_rotar(-angulo);
	sprite.volcar(pantalla, camara);

	if(pieza_actual)
	{
		DLibV::Representacion_bitmap flare(DLibV::Gestor_texturas::obtener(r_graficos::g_lens_flare));
		DLibV::Representacion_bitmap bmp(DLibV::Gestor_texturas::obtener(r_graficos::g_sprites));

		flare.establecer_modo_blend(DLibV::Representacion::BLEND_ALPHA);
		flare.establecer_alpha(255);
		flare.ir_a(c.x - 25, -c.y - 25);
		flare.volcar(pantalla, camara);

		bmp.establecer_modo_blend(DLibV::Representacion::BLEND_ALPHA);
		bmp.establecer_recorte((pieza_actual-1) * 30, 0, 30, 30);
		bmp.establecer_posicion(c.x - 15, -c.y - 15, 30, 30);
		bmp.establecer_alpha(128);
		bmp.volcar(pantalla, camara);
	}


}

void Jugador::establecer_inicio(Espaciable::tpunto pt)
{
	//reiniciar polígono para evitar problemas con rotaciones...
	poligono=DLibH::Poligono_2d<double> {};
	formar_poligono();
	poligono.rotar(angulo);

	establecer_posicion(pt.x, pt.y);
} 

void Jugador::cancelar_movimiento(float delta)
{
	desplazar_angulo_velocidad(angulo+180.0, velocidad*delta);
}

void Jugador::reiniciar(float ang)
{
	indice_velocidad=0;
	velocidad=velocidades[0];
	velocidad_destino=velocidad;
	angulo=ang;
}
