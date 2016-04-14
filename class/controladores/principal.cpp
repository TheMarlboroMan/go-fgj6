#include "principal.h"

#include <algorithm>

#include <class/generador_numeros.h>

#include "../app/cola_viento.h"
#include "../app/brillo.h"
#include "../app/hoja_arbol.h"
#include "../app/localizacion.h"
#include "../app/recursos.h"
#include "../app/importador.h"

#include "../app/framework_impl/input.h"
#include "estados_controladores.h"

#ifdef WINCOMPIL
/* Localización del parche mingw32... Esto debería estar en otro lado, supongo. */
#include <herramientas/herramientas/herramientas.h>
#endif

using namespace App;

Controlador_principal::Controlador_principal(DLibH::Log_base& log, const Fuentes& f, const Localizador& l, Sistema_audio& sa)
	:log(log),
	fuente(f.obtener_fuente("inkburrow", 26)),
	fuente_hud(f.obtener_fuente("bulldozer", 20)),
	localizador(l),
	sistema_audio(sa),
	modo(modos::juego),
	camara(0, 0, 800, 500), 
	fondo(DLibV::Gestor_texturas::obtener(r_graficos::g_fondo_defecto)),
	tiempo_ayuda(0.0f)
{
	layout_mensaje.mapear_fuente("akashi", fuente);
	layout_mensaje.parsear("data/layout/layout_mensaje.dnot", "layout");
}

void Controlador_principal::preloop(DFramework::Input& input, float delta)
{

}

void Controlador_principal::loop(DFramework::Input& input, float delta)
{
	if(input.es_senal_salida())
	{
		abandonar_aplicacion();
		return;
	}

	if(input.es_input_down(Input::cambio_logica) && info_juego.editor_activo)
	{
		solicitar_cambio_estado(editor);
		return;
	}

	if(input.es_input_down(Input::intercambiar_debug) && info_juego.editor_activo)
	{
		info_juego.debug_activo=!info_juego.debug_activo;
	}

	switch(modo)
	{
		case modos::juego:

			if(input.es_input_down(Input::mapa))
			{
				solicitar_cambio_estado(estado_mapa);
				return;
			}

		case modos::animacion_choque:

			if(input.es_input_down(Input::escape))
			{
				preparar_confirmar_salida();
				return;
			}
	
			representador.turno(delta);
			tiempo.turno(delta);
			procesar_interruptores(delta);
			procesar_ayudas(delta);
			procesar_estructuras(delta);
			procesar_particulas(delta);

			if(modo==modos::juego)
			{
				procesar_jugador(input, delta, jugador);
				ajustar_camara(delta);
				ajustar_parallax();
			}
			//Se usa el avisador para saber cuando empezar otra vez.
			else if(!tiempo.es_aviso())
			{
				iniciar_nivel(info_mapa.inicio_actual.acc_id());
				modo=modos::juego;
				return;
			}
		break;

		case modos::animacion_pieza:
			procesar_particulas(delta);
			procesar_animacion_pieza(delta);
		break;

		case modos::confirmar_salida:

			if(input.es_input_down(Input::escape))
			{
				solicitar_cambio_estado(intro);
				modo=modos::juego;
				return;
			}
			else if(hay_input_jugador(input))
			{
				modo=modos::juego;
			}

		break;

		case modos::ayuda:
			if(hay_input_jugador(input) && tiempo_ayuda >= 1.0f )
			{
				modo=modos::juego;
				tiempo_ayuda=0.f;
			}
			else
			{
				tiempo_ayuda+=delta;
			}
		break;

		case modos::florecimiento:
			procesar_florecimiento(delta);
			procesar_particulas(delta);
		break;

		case modos::recuento_final:

			procesar_particulas(delta);

			if(hay_input_jugador(input))
			{
				solicitar_cambio_estado(intro);
				modo=modos::juego;
				info_juego.juego_finalizado=true;
				return;
			}
		break;
	}
}

void Controlador_principal::postloop(DFramework::Input& input, float delta)
{

}

void Controlador_principal::dibujar(DLibV::Pantalla& pantalla)
{
	pantalla.limpiar(0, 0, 0, 255);

	if(info_juego.debug_activo)
	{
		dibujar_debug(pantalla);
	}
	else
	{
		fondo.volcar(pantalla);

		Representador& r=representador;
	
		for(const auto& o : mapa.decoraciones_fondo)	o->dibujar(r, pantalla, camara);
		for(const auto& o : mapa.puertas)		o.dibujar(r, pantalla, camara);
		for(const auto& o : mapa.piezas)		o.dibujar(r, pantalla, camara);
		for(const auto& o : mapa.mejoras_velocidad)	o.dibujar(r, pantalla, camara);
		for(const auto& o : mapa.arboles)		o.dibujar(r, pantalla, camara);
		if(info_juego.ayuda_activa) for(const auto& o : mapa.ayudas) o.dibujar(r, pantalla, camara);
		for(const auto& o : mapa.interruptores)		o.dibujar(r, pantalla, camara);

		switch(modo)
		{
			case modos::juego:
			case modos::ayuda:
			case modos::confirmar_salida:
			case modos::animacion_pieza:
				jugador.dibujar(r, pantalla, camara);
			break;

			case modos::animacion_choque:
			case modos::florecimiento:
			case modos::recuento_final: break;
		}

		if(pieza_animacion.es_activa())	pieza_animacion.dibujar(r, pantalla, camara);
		if(!info_juego.debug_activo) for(const auto& o : mapa.decoraciones_frente)	o->dibujar(r, pantalla, camara);
		for(const auto& o : particulas)						o->dibujar(r, pantalla, camara);

		//Control de cuando mostrar el HUD.
		switch(modo)
		{
			case modos::juego:
			case modos::ayuda:
			case modos::confirmar_salida:
			case modos::animacion_choque:
			case modos::animacion_pieza:
				r.dibujar_hud(pantalla, fuente_hud, tiempo.a_cadena(), tiempo.es_aviso(), jugador.acc_max_velocidad(), jugador.acc_indice_velocidad(), jugador.acc_tope_velocidad());
			break;
			case modos::florecimiento:
			case modos::recuento_final: break;
		}

		//Control de cuándo mostrar la caja de mensajes.
		switch(modo)
		{	
			case modos::confirmar_salida:
			case modos::ayuda:
			case modos::recuento_final:
			{
				layout_mensaje.volcar(pantalla);
				auto mensaje=layout_mensaje.obtener_por_id("txt_mensaje");
				if(mensaje->acc_alpha()==0) centrar_mensaje();
			}
			break;
			case modos::juego:
			case modos::animacion_choque:
			case modos::animacion_pieza:
			case modos::florecimiento: break;
		}
	}
}


void  Controlador_principal::despertar()
{

}

void  Controlador_principal::dormir()
{
	//La memoria del mapa no se libera.
	//mapa.limpiar();
}

bool Controlador_principal::es_posible_abandonar_estado() const
{
	return true;
}

////////////////

Bloque_input Controlador_principal::obtener_bloque_input(DFramework::Input& input) const
{
	Bloque_input res{0, 0, false};

	if(input.es_input_down(Input::arriba)) res.aceleracion=1;
	else if(input.es_input_down(Input::abajo)) res.aceleracion=-1;

	if(input.es_input_pulsado(Input::izquierda)) res.giro=1;
	else if(input.es_input_pulsado(Input::derecha)) res.giro=-1;

	return res;
}

void Controlador_principal::ajustar_parallax()
{
	//TODO: ¿Qué ocurre cuando el nivel es del mismo tamaño?. Hay que centrarlo.

	//Necesitamos los ratios desde el mínimo al máximo.
	auto cam=mapa.acc_info_camara();

	int x_parallax=(camara.acc_x() * info_parallax.max_fondo_x) /cam.max_cam_x;
	int y_parallax=(camara.acc_y() * info_parallax.max_fondo_y) /cam.max_cam_y;

	fondo.establecer_recorte(x_parallax, y_parallax, 800, 500);
}

void Controlador_principal::ajustar_camara(float delta)
{
	const int w_pantalla=camara.acc_pos_w(), 
		h_pantalla=camara.acc_pos_h(),
		mitad_w_pantalla=w_pantalla / 2,
		mitad_h_pantalla=h_pantalla / 2;

	const double base_zoom=0.9, divisor_zoom=25.0;

	auto cam=mapa.acc_info_camara();
	auto c=jugador.acc_poligono().acc_centro();
	int	cam_x=0, cam_y=0;
	double fin_zoom=(base_zoom + ((double) jugador.acc_indice_velocidad() / divisor_zoom));

	int 	ancho_foco=w_pantalla * fin_zoom,
		alto_foco=h_pantalla * fin_zoom;

	double 	zoom_actual=camara.acc_zoom(),
		zoom_aplicar=zoom_actual;

	bool transicionar=true;

	//El ancho o el alto de la cámara son mayores que el nivel:
	//tenemos que reajustar el zoom.
	if(ancho_foco > cam.max_cam_x || alto_foco > cam.max_cam_y)
	{
		double zoom_x=(double) cam.max_cam_x / (double) w_pantalla,
			zoom_y=(double) cam.max_cam_y / (double) h_pantalla;
		fin_zoom=std::min(zoom_x, zoom_y);
	
		//En este caso al reducir no hay que transicionar: es lo que pasa cuando
		//cambiamos de una sala con zoom grande, por ejemplo.
		if(zoom_actual > fin_zoom) transicionar=false;
	}

	//Transicionar el zoom.
	if(transicionar)
	{
		zoom_aplicar=zoom_actual;

		if(zoom_actual > fin_zoom)
		{
			zoom_aplicar-=delta * 0.10;
			if(zoom_aplicar < fin_zoom) zoom_aplicar=fin_zoom;
		}
		else if(zoom_actual < fin_zoom)
		{
			zoom_aplicar+=delta * 0.10;
			if(zoom_aplicar > fin_zoom) zoom_aplicar=fin_zoom;
		}
	}
	else
	{
		zoom_aplicar=fin_zoom;
	}

	//Y ahora colocar la cámara...

	//Comprobaremos que no nos salimos por la izquierda, en cuyo caso ajustaremos.
	cam_x=c.x - (mitad_w_pantalla * zoom_aplicar);
	cam_y=-(c.y + (mitad_h_pantalla * zoom_aplicar));

	if(cam_x < cam.min_cam_x) 
	{
		cam_x=cam.min_cam_x;
	}

	if(cam_y < cam.min_cam_y) 
	{		
		cam_y=cam.min_cam_y;
	}

	double ancho=((double)w_pantalla * zoom_aplicar);
	double alto=((double)h_pantalla * zoom_aplicar);

	if(ancho + cam_x > cam.max_cam_x)
	{
		cam_x=cam.max_cam_x-(w_pantalla * zoom_aplicar);
	}

	if(alto + cam_y > cam.max_cam_y) 
	{
		cam_y=cam.max_cam_y-(h_pantalla * zoom_aplicar);
	}

	//Y finalmente podemos enfocar.
	camara.mut_zoom(zoom_aplicar);
	camara.enfocar_a(cam_x, cam_y);
}

void Controlador_principal::procesar_jugador(DFramework::Input& input, float delta, Jugador &j)
{
	if(jugador.es_generar_cola())
	{
		particulas.push_back(std::move(std::unique_ptr<Particula>(new Cola_viento(j.acc_poligono().acc_centro(), j.acc_angulo(), -j.acc_velocidad(), .5f) ) ) );
	}

	auto bl=obtener_bloque_input(input);
	j.recibir_input(bl);
	j.turno(delta);

	if(j.es_sonido_velocidad())
	{
		sistema_audio.insertar(Info_audio_reproducir(
			Info_audio_reproducir::t_reproduccion::simple,
			Info_audio_reproducir::t_sonido::repetible,
			r_sonidos::s_viento, 127, 127));
	}

	for(auto& s : mapa.salidas)
	{
		if(j.en_colision_con(s))
		{
			jugador_en_salida(s, j);
			return;
		}
	}

	for(auto& i : mapa.interruptores)
	{
		if(j.en_colision_con(i))
		{
			jugador_en_interruptor(i, j);
		}
	}

	for(auto& a : mapa.arboles)
	{
		if(j.en_colision_con(a) && j.acc_pieza_actual())
		{
			jugador_en_arbol(a, j);
		}
	}

	for(auto& p : mapa.piezas)
	{
		if(j.en_colision_con(p))
		{
			jugador_en_pieza(p, j);
			break;	//No va a haber dos piezas seguidas y nos quitamos un crash...
		}
	}

	for(auto& m : mapa.mejoras_velocidad)
	{
		if(j.en_colision_con(m))
		{
			jugador_en_mejora_velocidad(m, j);
		}
	}

	for(const auto& o : mapa.obstaculos)
	{
		if(j.en_colision_con(o))
		{
			switch(o.acc_tipo())
			{
				case Obstaculo::ttipos::normal:
					chocar_jugador(j);
					return;
				break;
				case Obstaculo::ttipos::inocuo: 
					jugador.cancelar_movimiento(delta);
				break;
			}
		}
	}

	for(const auto& p : mapa.puertas)
	{
		if(j.en_colision_con(p))
		{
			chocar_jugador(j);
		}
	}

	if(info_juego.ayuda_activa) 
	{
		for(auto& a : mapa.ayudas)
		{
			if(j.en_colision_con(a))
			{
				jugador_en_ayuda(a, j);
				return;
			}
		}
	}
}

void Controlador_principal::procesar_particulas(float delta)
{
	for(auto& mv : mapa.mejoras_velocidad)
	{
		mv.turno(delta);
		if(mv.es_generar_particula()) crear_brillo(mv.acc_poligono().acc_centro());
	}

	for(auto& i : particulas) i->turno(delta);
	auto it=std::remove_if(std::begin(particulas), std::end(particulas), [](const std::unique_ptr<Particula>& c) {return c->es_borrar();});
	particulas.erase(it, std::end(particulas));
}

void Controlador_principal::procesar_interruptores(float delta)
{
	for(auto& i : mapa.interruptores) i.turno(delta);
}

void Controlador_principal::procesar_estructuras(float delta)
{
	for(auto& i : info_interruptores) i.second.turno(delta);
}

void Controlador_principal::procesar_ayudas(float delta)
{
	for(auto& i : mapa.ayudas) i.turno(delta);
}

void Controlador_principal::procesar_animacion_pieza(float delta)
{
	if(!mapa.arboles.size())
	{
		throw std::runtime_error("Animación de pieza sin árbol detectada");
	}

	//Mover.
	pieza_animacion.turno(delta);

	Herramientas_proyecto::Generador_int ang(10, 170), tmp(1000, 4000), vel(40, 200), cen(-10, 10);

	if(pieza_animacion.es_generar_hoja())
	{
		for(int i=0; i<6; ++i)
		{
			auto pt=pieza_animacion.acc_poligono().acc_centro();
			pt+={double(cen()), double(cen())};
			particulas.push_back(std::move(std::unique_ptr<Particula>(new Hoja_arbol(pt, ang(), vel(), tmp() / 1000.f) ) ) );
		}
	}

	if(!pieza_animacion.es_activa())
	{
		auto &a=mapa.arboles[0];
		a.colocar_pieza(pieza_animacion.acc_pieza_actual());
		pieza_animacion.reiniciar();

		if(a.es_finalizado())
		{
			sistema_audio.insertar(Info_audio_reproducir(
				Info_audio_reproducir::t_reproduccion::simple,
				Info_audio_reproducir::t_sonido::unico,
				r_sonidos::s_campana, 127, 127));

			sistema_audio.insertar(Info_audio_reproducir(
				Info_audio_reproducir::t_reproduccion::simple,
				Info_audio_reproducir::t_sonido::unico,
				r_sonidos::s_viento_arbol, 127, 127));

			modo=modos::florecimiento;
		}
		else
		{
			modo=modos::juego;
		}
	}
}

//La salida no es una referencia: para cuando carguemos el nivel el objeto
//original habrá desaparecido!.
void Controlador_principal::jugador_en_salida(Salida s, Jugador&)
{
	cargar_nivel(s.acc_id_mapa());
	iniciar_nivel(s.acc_id_inicio());
}

void Controlador_principal::jugador_en_ayuda(Ayuda& a, Jugador&)
{
	if(a.es_activable())
	{
		sistema_audio.insertar(Info_audio_reproducir(
			Info_audio_reproducir::t_reproduccion::simple,
			Info_audio_reproducir::t_sonido::unico,
			r_sonidos::s_campana, 127, 127));

		a.activar();
		asignar_mensaje(localizador.obtener(a.acc_indice()));
		modo=modos::ayuda;
	}

}

void Controlador_principal::jugador_en_arbol(Arbol& a, Jugador& j)
{
	pieza_animacion.activar(j.acc_pieza_actual(), a.acc_poligono().acc_centro(), j.acc_poligono().acc_centro());
	j.mut_pieza_actual(0);

	sistema_audio.insertar(Info_audio_reproducir(
		Info_audio_reproducir::t_reproduccion::simple,
		Info_audio_reproducir::t_sonido::unico,
		r_sonidos::s_poner_pieza, 128, 127));

	sistema_audio.insertar(Info_audio_reproducir(
		Info_audio_reproducir::t_reproduccion::simple,
		Info_audio_reproducir::t_sonido::unico,
		r_sonidos::s_viento_arbol, 127, 127));

	modo=modos::animacion_pieza;
}

void Controlador_principal::jugador_en_pieza(const Pieza& p, Jugador&)
{
	if(!jugador.acc_pieza_actual())
	{
		sistema_audio.insertar(Info_audio_reproducir(
			Info_audio_reproducir::t_reproduccion::simple,
			Info_audio_reproducir::t_sonido::unico,
			r_sonidos::s_poner_pieza, 127, 127));

		//El orden, muy importante.
		jugador.mut_pieza_actual(p.acc_indice());
		info_persistente.recoger_pieza(p.acc_indice());		
		mapa.recoger_pieza(p.acc_indice());
	}
}

void Controlador_principal::jugador_en_mejora_velocidad(const Mejora_velocidad& p, Jugador& j)
{
	if(j.acc_max_velocidad() < p.acc_nivel())
	{
		sistema_audio.insertar(Info_audio_reproducir(
			Info_audio_reproducir::t_reproduccion::simple,
				Info_audio_reproducir::t_sonido::unico,
				r_sonidos::s_poner_pieza, 127, 127));

		j.establecer_max_velocidad(p.acc_nivel());
	}
}

void Controlador_principal::jugador_en_interruptor(Interruptor& i, Jugador& j)
{
	if(i.acc_nivel() > j.acc_indice_velocidad())
	{
		sistema_audio.insertar(Info_audio_reproducir(
			Info_audio_reproducir::t_reproduccion::simple,
				Info_audio_reproducir::t_sonido::unico,
				r_sonidos::s_no_activable, 127, 127));

		representador.avisar_velocidad_minima(i.acc_nivel());

		return;
	}

	if(!i.es_activo())
	{
		i.activar();

		sistema_audio.insertar(Info_audio_reproducir(
			Info_audio_reproducir::t_reproduccion::simple,
			Info_audio_reproducir::t_sonido::repetible,
			r_sonidos::s_molino, 64, 127));

		//Comprobar que existe puerta aún...
		auto& s=info_interruptores[i.acc_id_grupo()];

		if(!mapa.existe_puerta(s.id_puerta))
		{
//			log<<"Warning: no existe la puerta "<<s.id_puerta<<std::endl;
		}
		else
		{
			s.activar(i.acc_tiempo_grupo(), i.acc_poligono().acc_centro());
	
			if(s.es_completo())
			{
				info_persistente.abrir_puerta(s.id_puerta);
				mapa.abrir_puerta(s.id_puerta);
				s.finalizar();
				
				sistema_audio.insertar(Info_audio_reproducir(
					Info_audio_reproducir::t_reproduccion::simple,
					Info_audio_reproducir::t_sonido::unico,
					r_sonidos::s_puerta, 127, 127));
			}
		}
	}
}

void Controlador_principal::cargar_nivel(int nivel)
{
	info_mapa.id_mapa=nivel;

#ifdef WINCOMPIL
	using namespace parche_mingw;
#else
	using namespace std;
#endif

	const std::string nombre_fichero="data/app/mapas/mapa"+to_string(nivel)+".dat";

	particulas.clear();
	mapa.limpiar();
	Importador importador;
	importador.importar(nombre_fichero.c_str(), mapa);
	mapa.inicializar();

	for(auto id_p : info_persistente.puertas_abiertas) mapa.abrir_puerta(id_p);
	for(auto id_p : info_persistente.piezas_recogidas) mapa.recoger_pieza(id_p);
	mapa.actualizar_arbol(info_persistente.piezas_recogidas, jugador.acc_pieza_actual());

	//Trabajar el fondo...
	int id_recurso=100+(mapa.acc_id_fondo()-1);
	if(id_recurso < 100)
	{
		id_recurso=100;
		log<<"Warning: id_recurso < 100 "<<id_recurso<<" en mapa "<<info_mapa.id_mapa<<std::endl;
	}

	fondo.establecer_textura(DLibV::Gestor_texturas::obtener(id_recurso));
	fondo.establecer_recorte(0, 0, 800, 500);
	fondo.establecer_posicion(0, 0, 800, 500);

	info_parallax.max_fondo_x=fondo.acc_w_textura()-camara.acc_pos_w();
	info_parallax.max_fondo_y=fondo.acc_h_textura()-camara.acc_pos_h();
}

void Controlador_principal::iniciar_nivel(int id_inicio)
{
	//Marcar sala como visitada...
	info_persistente.visitar_mapa(info_mapa.id_mapa);

	//Colocar a jugador en punto de inicio.
	auto it=std::find_if(std::begin(mapa.inicios), std::end(mapa.inicios), [id_inicio](const Inicio& i){return i.acc_id()==id_inicio;});

	if(it==std::end(mapa.inicios))
	{
		throw std::runtime_error("Imposible localizar punto de inicio en mapa");
	}

	info_mapa.inicio_actual=*it;
	jugador.establecer_inicio(info_mapa.inicio_actual.acc_punto());

	//Preparar info interruptores...
	info_interruptores.clear();
	for(const auto& i : mapa.interruptores)
	{
		size_t id_grupo=i.acc_id_grupo();
		if(!info_interruptores.count(id_grupo))
		{
			info_interruptores[id_grupo]=info_interruptor(1, i.acc_id_puerta());
//			log<<"Nuevo grupo de puertas "<<id_grupo<<std::endl;
		}
		else
		{
			++info_interruptores[id_grupo].total;
//			log<<"Grupo de puertas "<<id_grupo<<":"<<info_interruptores[id_grupo].total<<std::endl;
		}
	}

	//camara.mut_zoom(1.0);
}

void Controlador_principal::iniciar_juego()
{
	info_persistente.reiniciar();
	jugador.reiniciar(0.f);
	tiempo.reiniciar();
	cargar_nivel(1);
	iniciar_nivel(1);
}

void Controlador_principal::chocar_jugador(Jugador& j)
{
	sistema_audio.insertar(Info_audio_reproducir(
		Info_audio_reproducir::t_reproduccion::simple,
		Info_audio_reproducir::t_sonido::unico,
		r_sonidos::s_chocar, 127, 127));

	modo=modos::animacion_choque;

	tiempo.penalizar();
	jugador.reiniciar(info_mapa.inicio_actual.acc_angulo());

	auto centro=j.acc_poligono().acc_centro();
	Herramientas_proyecto::Generador_int g(0, 360), t(0, 1000);

	for(int i=0; i < 20; ++i)
	{
		double velocidad=100;
		float angulo=g();
		float tiempo=1.0f+( (float)t() / 1000.0);
		particulas.push_back(std::move(std::unique_ptr<Particula>(new Cola_viento(centro, angulo, velocidad, tiempo) ) ) );
	}
}

void Controlador_principal::preparar_confirmar_salida()
{
	modo=modos::confirmar_salida;
	asignar_mensaje(localizador.obtener(Localizacion::confirmacion_salida));
}

void Controlador_principal::asignar_mensaje(const std::string& m)
{
	auto r=static_cast<DLibV::Representacion_TTF *>(layout_mensaje.obtener_por_id("txt_mensaje"));
	r->asignar(m);
	r->establecer_alpha(0);
}

void Controlador_principal::centrar_mensaje()
{
	auto mensaje=layout_mensaje.obtener_por_id("txt_mensaje");

	//Centrar...
	int 	w_txt=layout_mensaje.const_int("w_texto"),
		h_txt=layout_mensaje.const_int("h_texto"),
		x_txt=layout_mensaje.const_int("x_texto"),
		y_txt=layout_mensaje.const_int("y_texto");

	int x=x_txt+( (w_txt / 2) - (mensaje->acc_posicion().w / 2));
	int y=y_txt+( (h_txt / 2) - (mensaje->acc_posicion().h / 2));

	mensaje->ir_a(x, y);
	mensaje->establecer_alpha(255);
}

void Controlador_principal::crear_brillo(Espaciable::tpunto centro)
{
	Herramientas_proyecto::Generador_int g(0, 100), t(0, 1000), desp(-20, 20), ang(45, 135);

	float angulo_v=ang();
	float tiempo_v=1.0f+( float(t()) / 1000.0);
	double velocidad_v=40.0+(double(g()) / 100.0);
	particulas.push_back(std::move(std::unique_ptr<Particula>(new Cola_viento(centro, angulo_v, velocidad_v, tiempo_v) ) ) );

	float tiempo_b=0.5f+( float(t()) / 1000.0);
	double velocidad_b=100.0+(double(g()) / 100.0);
	
	centro+=DLibH::Punto_2d<double>(desp(), desp());
	particulas.push_back(std::move(std::unique_ptr<Particula>(new Brillo(centro, 90.0, velocidad_b, tiempo_b) ) ) );
}

void Controlador_principal::procesar_florecimiento(float delta)
{
	auto &a=mapa.arboles[0];

	a.turno(delta);

	if(a.es_florecido())
	{
		asignar_mensaje(localizador.obtener(Localizacion::mensaje_fin)+tiempo.a_cadena());
		modo=modos::recuento_final;
	}
}

bool Controlador_principal::hay_input_jugador(DFramework::Input& input) const
{
	return input.es_input_down(Input::arriba) ||
		input.es_input_down(Input::abajo) ||
		input.es_input_down(Input::izquierda) ||
		input.es_input_down(Input::derecha) ||
		input.es_input_down(Input::mapa);
}

void Controlador_principal::dibujar_debug(DLibV::Pantalla& p)
{
	for(const auto& o : mapa.obstaculos)
	{
		representador.dibujar_poligono(p, o.acc_poligono(), tcolor{255,0,0,255}, camara);
	}

	representador.dibujar_poligono(p, jugador.acc_poligono(), tcolor{255,255,255,255}, camara);
}
