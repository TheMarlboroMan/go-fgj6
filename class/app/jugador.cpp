#include "jugador.h"

using namespace App;

const float Jugador::factor_rotacion=220.0;
const float Jugador::factor_min_rotacion=60.0;
const float Jugador::factor_aceleracion=180.0f;

Jugador::Jugador()
	:angulo(0.0), velocidad(100.0f), indice_velocidad(0), velocidades{40.0, 80.0, 120.0, 160.0}
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
	else if(indice >= (int) velocidades.size()) indice=velocidades.size();

	indice_velocidad=indice;
	velocidad=velocidades[indice_velocidad];	//TODO: Not really!!!!.

/*	velocidad+=dir > 0 ? delta * factor_aceleracion : delta * factor_freno;

	if(velocidad > max_vel) velocidad=max_vel;
	else if(velocidad < min_vel) velocidad=min_vel;
*/
}

void Jugador::movimiento(float delta)
{
	desplazar_angulo_velocidad(angulo, velocidad*delta);

	DLibH::Vector_2d<double> v=vector_unidad_para_angulo_cartesiano(angulo);
	DLibH::Punto_2d<double> pd{v.x * velocidad, v.y * velocidad};
}

void Jugador::formar_poligono()
{
	poligono.insertar_vertice({15.0, 0.0});
	poligono.insertar_vertice({-5.0, -7.0});
	poligono.insertar_vertice({-5.0, 7.0});
	poligono.cerrar();
	poligono.mut_centro({0.0, 0.0});
}

void Jugador::dibujar(Representador& r, DLibV::Pantalla& pantalla, const DLibV::Camara& camara) const
{
	//TODO...
	r.dibujar_poligono(pantalla, poligono, {255, 255, 255, 255}, camara);
}
