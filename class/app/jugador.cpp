#include "jugador.h"

#include <video/representacion/representacion_grafica/representacion_bitmap/representacion_bitmap.h>
#include <video/gestores/gestor_texturas.h>

using namespace App;

const float Jugador::factor_rotacion=220.0;
const float Jugador::factor_min_rotacion=60.0;
const float Jugador::factor_aceleracion=180.0f;

Jugador::Jugador()
	:angulo(0.0), velocidad(100.0f), indice_velocidad(0), 
	pieza_actual(0), max_velocidad(3), tiempo(0.0f),
	velocidades{80.0, 160.0, 320.0, 640.0}
{
	formar_poligono();
}

void Jugador::recibir_input(const Bloque_input& bi)
{
	input_actual=bi;
}

void Jugador::turno(float delta)
{
	if(input_actual.giro) girar(input_actual.giro, delta);

	if(input_actual.aceleracion) cambiar_velocidad(input_actual.aceleracion);

	if(velocidad) movimiento(delta);

	tiempo+=delta;

	if(tiempo > 0.15f) tiempo=0.0f;
}

void Jugador::colisionar(bool con_dano)
{
	//TODO...
}

void Jugador::girar(int dir, float delta)
{
	//La velocidad afecta a la capacidad de giro. No afecta parado o marcha atrás.	

	//TODO: Usar velocidad actual como int!.
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

	//Limite de velocidades...
	if(indice > max_velocidad) indice=max_velocidad;
		

	indice_velocidad=indice;
	velocidad=velocidades[indice_velocidad];	//TODO: Not really!!!!.

//std::cout<<indice_velocidad<<" "<<velocidad<<std::endl;

/*	velocidad+=dir > 0 ? delta * factor_aceleracion : delta * factor_freno;

	if(velocidad > max_vel) velocidad=max_vel;
	else if(velocidad < min_vel) velocidad=min_vel;
*/
}

void Jugador::movimiento(float delta)
{
	desplazar_angulo_velocidad(angulo, velocidad*delta);

//	DLibH::Vector_2d<double> v=vector_unidad_para_angulo_cartesiano(angulo);
//	DLibH::Punto_2d<double> pd{v.x * velocidad, v.y * velocidad};
}

void Jugador::formar_poligono()
{
	poligono.insertar_vertice({-15.0, 10.0});
	poligono.insertar_vertice({-10.0, 15.0});
	poligono.insertar_vertice({10.0, 15.0});
	poligono.insertar_vertice({15.0, 10.0});
	poligono.insertar_vertice({15.0, -10.0});
	poligono.insertar_vertice({10.0, -15.0});
	poligono.insertar_vertice({-10.0, -15.0});
	poligono.insertar_vertice({-15.0, -10.0});
	poligono.cerrar();
	poligono.mut_centro({0.0, 0.0});
}

void Jugador::dibujar(Representador& r, DLibV::Pantalla& pantalla, const DLibV::Camara& camara) const
{
	auto c=poligono.acc_centro();

	int x=0;
	if(tiempo <= 0.03f) x=0;
	else if(tiempo <= 0.06f) x=1;
	else if(tiempo <= 0.09f) x=2;
	else if(tiempo <= 0.12f) x=3;
	else if(tiempo <= 0.15f) x=4;

	DLibV::Representacion_bitmap sprite(DLibV::Gestor_texturas::obtener(5));
	sprite.establecer_modo_blend(DLibV::Representacion::BLEND_ALPHA);
	sprite.establecer_alpha(255);
	sprite.establecer_recorte(x*30, 0, 30, 30);
	sprite.establecer_posicion(c.x-15, -c.y-15, 30, 30);
	sprite.transformar_centro_rotacion(15 / camara.acc_zoom(), 15 / camara.acc_zoom());
	sprite.transformar_rotar(-angulo);
	sprite.volcar(pantalla, camara);
}

void Jugador::establecer_inicio(Espaciable::tpunto pt, int an)
{
	angulo=an;

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
