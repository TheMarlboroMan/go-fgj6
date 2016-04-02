#include "principal.h"

#include <algorithm>

#include <class/generador_numeros.h>

#include "../app/representador.h"

#include "../app/importador.h"

#include "../app/framework_impl/input.h"
#include "estados_controladores.h"

#ifdef WINCOMPIL
/* Localización del parche mingw32... Esto debería estar en otro lado, supongo. */
#include <herramientas/herramientas/herramientas.h>
#endif

using namespace App;

Controlador_principal::Controlador_principal(DLibH::Log_base& log, const Fuentes& f)
	:log(log),
	fuente_akashi(f.obtener_fuente("akashi", 16)),
	camara(0, 0, 800, 500)
{

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

	if(input.es_input_down(Input::cambio_logica))
	{
		solicitar_cambio_estado(editor);
		return;
	}

	procesar_interruptores(delta);
	procesar_estructuras(delta);
	procesar_jugador(input, delta, jugador);
	ajustar_camara(delta);
}

void  Controlador_principal::postloop(DFramework::Input& input, float delta)
{

}

void  Controlador_principal::dibujar(DLibV::Pantalla& pantalla)
{
	pantalla.limpiar(0, 0, 0, 255);

	
	int id_recurso=2;
	switch(mapa.acc_id_fondo())
	{
		case 1: id_recurso=2; break;
		case 2: id_recurso=3; break;
	}

	DLibV::Representacion_bitmap fondo(DLibV::Gestor_texturas::obtener(id_recurso));
	fondo.volcar(pantalla);

	Representador r;

	for(const auto& o : mapa.decoraciones_fondo)	o->dibujar(r, pantalla, camara);
	for(const auto& o : mapa.puertas)		o.dibujar(r, pantalla, camara);
	for(const auto& o : mapa.interruptores)		o.dibujar(r, pantalla, camara);
	for(const auto& o : mapa.piezas)		o.dibujar(r, pantalla, camara);
	for(const auto& o : mapa.mejoras_velocidad)	o.dibujar(r, pantalla, camara);
	for(const auto& o : mapa.arboles)		o.dibujar(r, pantalla, camara);
	jugador.dibujar(r, pantalla, camara);
	for(const auto& o : mapa.decoraciones_frente)	o->dibujar(r, pantalla, camara);
}

void  Controlador_principal::despertar()
{
	log<<"Inicializando mapa..."<<std::endl;
	mapa.inicializar();
}

void  Controlador_principal::dormir()
{
	mapa.limpiar();
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

void Controlador_principal::ajustar_camara(float delta)
{
	const int w_pantalla=camara.acc_pos_w(), 
		h_pantalla=camara.acc_pos_h(),
		mitad_w_pantalla=w_pantalla / 2,
		mitad_h_pantalla=h_pantalla / 2;

	double fin_zoom=1.0;
	auto cam=mapa.acc_info_camara();
	auto c=jugador.acc_poligono().acc_centro();
	int	cam_x=0, cam_y=0;

	switch(jugador.acc_indice_velocidad())
	{
		case 0: fin_zoom=1.0; break;
		case 1: fin_zoom=1.2; break;
		case 2: fin_zoom=1.4; break;
		case 3: fin_zoom=1.6; break;
	}


	int 	ancho_foco=w_pantalla * fin_zoom,
		alto_foco=h_pantalla * fin_zoom;



	//El ancho o el alto de la cámara son mayores que el nivel:
	//tenemos que reajustar el zoom.
	if(ancho_foco > cam.max_cam_x || alto_foco > cam.max_cam_y)
	{
		double zoom_x=(double) w_pantalla / (double) cam.max_cam_x,
			zoom_y=(double) h_pantalla / (double) cam.max_cam_y;
		fin_zoom=std::max(zoom_x, zoom_y);
	}

	//Transicionar el zoom.
	double zoom_actual=camara.acc_zoom();
	double zoom_aplicar=zoom_actual;

	if(zoom_actual > fin_zoom)
	{
		zoom_aplicar-=delta;
		if(zoom_aplicar < fin_zoom) zoom_aplicar=fin_zoom;
	}
	else if(zoom_actual < fin_zoom)
	{
		zoom_aplicar+=delta;
		if(zoom_aplicar > fin_zoom) zoom_aplicar=fin_zoom;
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
	auto bl=obtener_bloque_input(input);
	j.recibir_input(bl);
	j.turno(delta);

	for(const auto& s : mapa.salidas)
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
				 
			//TODO... Llevar a otro método.
					iniciar_nivel(info_mapa.id_mapa, info_mapa.inicio_actual.acc_id());
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
			//TODO... Llevar a otro método.
			iniciar_nivel(info_mapa.id_mapa, info_mapa.inicio_actual.acc_id());
		}
	}
}

void Controlador_principal::procesar_interruptores(float delta)
{
	for(auto& i : mapa.interruptores) i.turno(delta);
}

void Controlador_principal::procesar_estructuras(float delta)
{
	for(auto& i : info_interruptores) i.second.turno(delta);
}

void Controlador_principal::jugador_en_salida(const Salida& s, Jugador&)
{
	iniciar_nivel(s.acc_id_mapa(), s.acc_id_inicio());
}

void Controlador_principal::jugador_en_arbol(Arbol& a, Jugador& j)
{
	a.colocar_pieza(j.acc_pieza_actual());
	j.mut_pieza_actual(0);

	if(a.es_finalizado())
	{
		//TODO...
		throw std::runtime_error("FIN DE JUEGO LOOL");
	}

}

void Controlador_principal::jugador_en_pieza(const Pieza& p, Jugador&)
{
	if(!jugador.acc_pieza_actual())
	{
		mapa.recoger_pieza(p.acc_indice());
		jugador.mut_pieza_actual(p.acc_indice());
		info_persistente.recoger_pieza(p.acc_indice());
	}
}

void Controlador_principal::jugador_en_mejora_velocidad(const Mejora_velocidad& p, Jugador& j)
{
	j.establecer_max_velocidad(p.acc_nivel());
}

void Controlador_principal::jugador_en_interruptor(Interruptor& i, Jugador& j)
{
	if(i.acc_nivel() > j.acc_indice_velocidad())
	{
		return;
	}

	if(!i.es_activo())
	{
		i.activar();

		//Comprobar que existe puerta aún...
		auto& s=info_interruptores[i.acc_id_grupo()];

		if(!mapa.existe_puerta(s.id_puerta))
		{
			log<<"Warning: no existe la puerta "<<s.id_puerta<<std::endl;
		}
		else
		{
			log<<"La puerta existe... activando..."<<std::endl;
			s.activar(i.acc_tiempo_grupo());
	
			if(s.es_completo())
			{
				log<<"Puerta "<<s.id_puerta<<" completa"<<std::endl;
				info_persistente.abrir_puerta(s.id_puerta);
				mapa.abrir_puerta(s.id_puerta);
				s.finalizar();
			}
		}
	}
}

void Controlador_principal::iniciar_nivel(int nivel, int id_inicio)
{
	info_mapa.id_mapa=nivel;

	const std::string nombre_fichero="data/mapas/mapa"+std::to_string(nivel)+".dat";

	mapa.limpiar();
	Importador importador;
	importador.importar(nombre_fichero.c_str(), mapa);
	mapa.inicializar();

	//TODO: Procesar cosas persistentes.
	for(auto id_p : info_persistente.puertas_abiertas) mapa.abrir_puerta(id_p);
	for(auto id_p : info_persistente.piezas_recogidas) mapa.recoger_pieza(id_p);
	mapa.actualizar_arbol(info_persistente.piezas_recogidas);

	//Colocar a jugador en punto de inicio.
	auto it=std::find_if(std::begin(mapa.inicios), std::end(mapa.inicios), [id_inicio](const Inicio& i){return i.acc_id()==id_inicio;});

	if(it==std::end(mapa.inicios))
	{
		throw std::runtime_error("Imposible localizar punto de inicio en mapa");
	}

	info_mapa.inicio_actual=*it;

	jugador.establecer_inicio(info_mapa.inicio_actual.acc_punto(), info_mapa.inicio_actual.acc_angulo());

	//Preparar info interruptores...
	info_interruptores.clear();
	for(const auto& i : mapa.interruptores)
	{
		size_t id_grupo=i.acc_id_grupo();
		if(!info_interruptores.count(id_grupo))
		{
			info_interruptores[id_grupo]=info_interruptor(1, 0, i.acc_id_puerta());
			log<<"Nuevo grupo de puertas "<<id_grupo<<std::endl;
		}
		else
		{
			++info_interruptores[id_grupo].total;
			log<<"Grupo de puertas "<<id_grupo<<":"<<info_interruptores[id_grupo].total<<std::endl;
		}
	}

	camara.mut_zoom(1.0);
}

void Controlador_principal::iniciar_juego()
{
	info_persistente.reiniciar();
	iniciar_nivel(1, 1);
}
