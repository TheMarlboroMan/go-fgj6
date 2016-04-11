#include "mapa.h"

#include "../app/framework_impl/input.h"

using namespace App;

Controlador_mapa::Controlador_mapa(DLibH::Log_base& l, Sistema_audio& s)
	:log(l), sistema_audio(s), rep_mapa(true)
{
	automapa.cargar("data/app/mapa.dnot");
	rep_mapa.establecer_posicion(400, 250);
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
	layout.volcar(pantalla);
}

void Controlador_mapa::despertar()
{
	//Montar vista?.
	for(const auto s : automapa.obtener_visitadas()) generar_representacion_sala(s);

	//Registrar con el layout.
	layout.registrar_externa("mapa", rep_mapa);
	layout.parsear("data/layout/layout_mapa.dnot", "layout");
}

void Controlador_mapa::dormir()
{
	//Desmontar vista?.
	layout.vaciar();

	//Vaciar representación agrupada?.
	rep_mapa.vaciar_grupo();
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

void Controlador_mapa::generar_representacion_sala(const Automapa_sala& sala)
{
	using namespace DLibV;

	static const int w=40;
	static const int h=25;
	static const int borde=2;

	//Recuadro...
	rep_mapa.insertar_representacion(new Representacion_primitiva_caja({sala.x*w, sala.y*h, sala.w*w, sala.h*h}, 0, 0, 0));
	rep_mapa.insertar_representacion(new Representacion_primitiva_caja({ (sala.x*w)+borde, (sala.y*h)+borde, (sala.w*w)-(2*borde), (sala.h*h)-(2*borde)}, 255, 255, 255));

	//Salidas...
	for(const auto& s : sala.salidas)
	{
		SDL_Rect caja{0,0,0,0};

		switch(s.orientacion)
		{
			case s.torientaciones::nada: break;
			case s.torientaciones::norte:	caja={(s.x*w)+(w/4), s.y*h, w/2, borde}; break;
			case s.torientaciones::oeste:	caja={s.x*w, (s.y*h)+(h/4), borde, h/2}; break;
			case s.torientaciones::sur:	caja={(s.x*w)+w-borde, (s.y*h)+(h/4), borde, h/2};
			case s.torientaciones::este:	caja={(s.x*w)+(w/4), (s.y*h)+h-borde, w/2, borde}; break;
		}

		if(caja.w && caja.h)
		{
			rep_mapa.insertar_representacion(new Representacion_primitiva_caja(caja, 255, 0, 0));
		}
	}

	//Marcadores...
	for(const auto& m : sala.marcadores)
	{
		switch(m)
		{
			case sala.tmarcadores::nada:	break;
			case sala.tmarcadores::arbol:	break;
			case sala.tmarcadores::metal:	break;
			case sala.tmarcadores::madera:	break;
			case sala.tmarcadores::aire:	break;
			case sala.tmarcadores::fuego:	break;
			case sala.tmarcadores::tierra:	break;
		}		
	}
}
