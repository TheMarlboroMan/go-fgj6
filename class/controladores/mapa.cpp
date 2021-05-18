#include "mapa.h"

#include "../app/framework_impl/input.h"

#include "../app/recursos.h"

using namespace App;

Controlador_mapa::Controlador_mapa(DLibH::Log_base& l, Sistema_audio& s)
	:log(l), sistema_audio(s), rep_mapa(true)
{
	automapa.cargar("data/app/mapa.dnot");
	layout.mapear_textura("arbol", DLibV::Gestor_texturas::obtener(r_graficos::g_cover));
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
	else if(input.es_input_down(Input::escape) || input.es_input_down(Input::mapa))
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
	//Registrar con el layout.
	layout.registrar_externa("mapa", rep_mapa);
	layout.parsear("data/layout/layout_mapa.dnot", "layout");

	//Montar vista?.
	for(const auto s : automapa.obtener_visitadas()) generar_representacion_sala(s);
	auto v=rep_mapa.obtener_grupo();

	//Centrar...
	auto pos=v[0]->acc_posicion();
	int 	minx=pos.x, miny=pos.y, maxx=minx+pos.w, maxy=miny+pos.y;
	for(auto& r : v)
	{
		auto pos=r->acc_posicion();
		int tminx=pos.x, tminy=pos.y, tmaxx=minx+pos.w, tmaxy=miny+pos.y;
		if(tminx < minx) minx=tminx;
		if(tminy < miny) miny=tminy;
		if(tmaxx < maxx) maxx=tmaxx;
		if(tmaxy < maxy) maxy=tmaxy;
	}

	rep_mapa.ir_a( (800 / 2) - (maxx - minx) / 2, (500 / 2) - (maxy - miny) / 2);
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

void Controlador_mapa::descubrir_salas(const std::vector<int>& v, int ids)
{
	for(auto i : v) automapa.visitar(i);
	id_sala_actual=ids;
}

void Controlador_mapa::generar_representacion_sala(const Automapa_sala& sala)
{
	using namespace DLibV;

	static const int w=40;
	static const int h=25;
	static const int borde=2;

	int 	x_sala=sala.x*w,
		y_sala=sala.y*h,
		w_sala=sala.w*w,
		h_sala=sala.h*h,
		r=sala.id==id_sala_actual ? layout.const_int("r_actual") : layout.const_int("r_fondo"),
		g=sala.id==id_sala_actual ? layout.const_int("g_actual") : layout.const_int("g_fondo"),
		b=sala.id==id_sala_actual ? layout.const_int("b_actual") : layout.const_int("b_fondo"),
		rm=layout.const_int("r_muro"),
		gm=layout.const_int("g_muro"),
		bm=layout.const_int("b_muro");

	//Recuadro...
	rep_mapa.insertar_representacion(new Representacion_primitiva_caja({x_sala, y_sala, w_sala, h_sala}, rm, gm, bm));
	rep_mapa.insertar_representacion(new Representacion_primitiva_caja({x_sala+borde, y_sala+borde, w_sala-(2*borde), h_sala-(2*borde)}, r, g, b));

	//Salidas...
	for(const auto& s : sala.salidas)
	{
		SDL_Rect caja{0,0,0,0};

		int 	x_salida=s.x*w+x_sala,
			y_salida=s.y*h+y_sala;

		switch(s.orientacion)
		{
			case s.torientaciones::nada: break;
			case s.torientaciones::norte:	caja={x_salida+(w/4), y_salida, w/2, borde}; break;
			case s.torientaciones::sur:	caja={x_salida+(w/4), y_salida+h-borde, w/2, borde}; break;
			case s.torientaciones::oeste:	caja={x_salida, y_salida+(h/4), borde, h/2}; break;
			case s.torientaciones::este:	caja={x_salida+w-borde, y_salida+(h/4), borde, h/2}; break;
		}

		if(caja.w && caja.h)
		{
			rep_mapa.insertar_representacion(new Representacion_primitiva_caja(caja, r, g, b));
		}
	}

	//Marcadores...
	size_t tot_marcadores=sala.marcadores.size();
	int i_marcador=0;

	for(const auto& m : sala.marcadores)
	{
		SDL_Rect caja{0,0,0,0};

		switch(m)
		{
			case sala.tmarcadores::nada:	break;
			case sala.tmarcadores::arbol:	caja={105, 60, 45, 30}; break;
			case sala.tmarcadores::metal:	caja={0, 0, 30, 30}; break;
			case sala.tmarcadores::madera:	caja={30, 0, 30, 30}; break;
			case sala.tmarcadores::agua:	caja={60, 0, 30, 30};break;
			case sala.tmarcadores::fuego:	caja={90, 0, 30, 30};break;
			case sala.tmarcadores::tierra:	caja={120, 0, 30, 30};break;
			case sala.tmarcadores::velocidad: caja={65, 30, 30, 30};break;
		}

		if(caja.w && caja.h)
		{
			int 	sx=x_sala + (w_sala / (1 + tot_marcadores)) + (i_marcador * caja.w) - (caja.w / 2),
				sy=y_sala + (h_sala / 2 ) - (caja.h / 2);

			Representacion_bitmap * bmp = new Representacion_bitmap(Gestor_texturas::obtener(r_graficos::g_sprites));
			bmp->establecer_modo_blend(DLibV::Representacion::BLEND_ALPHA);
			bmp->establecer_recorte(caja);
			bmp->establecer_posicion(sx, sy, caja.w, caja.h);
			rep_mapa.insertar_representacion(bmp);
		}

		++i_marcador;
	}
}
