#include "mapa.h"

#include "../app/framework_impl/input.h"

using namespace App;

Controlador_mapa::Controlador_mapa(DLibH::Log_base& l, Sistema_audio& s)
	:log(l), sistema_audio(s)
{
	automapa.cargar("data/app/mapa.dnot");
}

void Controlador_mapa::preloop(DFramework::Input& input, float delta)
{

}

void Controlador_mapa::loop(DFramework::Input& input, float delta)
{
	if(input.es_senal_salida())
	{
		abandonar_aplicacion();
		return;
	}
	else if(input.es_input_down(Input::escape))
	{
		solicitar_cambio_estado(principal);
		return;
	}
}

void Controlador_mapa::postloop(DFramework::Input& input, float delta)
{

}

void Controlador_mapa::dibujar(DLibV::Pantalla& pantalla)
{
	//Mostrar vista...
	pantalla.limpiar(255, 0, 0, 255);
}

void Controlador_mapa::despertar()
{
	//Montar vista?.
	//TODO.

	//TODO: Componer representación agrupada.
}

void Controlador_mapa::dormir()
{
	//Desmontar vista?.

	//TODO.
	//Vaciar representación agrupada?.
}

bool Controlador_mapa::es_posible_abandonar_estado() const
{
	return true;
}

void Controlador_mapa::reiniciar()
{
	automapa.reiniciar();
}

void Controlador_mapa::descubrir_salas(const std::vector<int>& v)
{
	for(auto i : v) automapa.visitar(i);
}
