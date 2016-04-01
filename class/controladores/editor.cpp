#include "editor.h"

#include <fstream>
#include <string>

#include "../app/exportador.h"
#include "../app/importador.h"

#include "../app/framework_impl/input.h"

#include "../app/widget_editor_decoracion.h"
#include "../app/widget_editor_color.h"
#include "../app/widget_editor_obstaculo.h"
#include "../app/widget_editor_propiedades_mapa.h"
#include "../app/widget_editor_propiedades_inicio.h"
#include "../app/widget_editor_propiedades_salida.h"
#include "../app/widget_editor_propiedades_pieza.h"
#include "../app/widget_editor_propiedades_interruptor.h"
#include "../app/widget_editor_propiedades_puerta.h"
#include "../app/widget_editor_propiedades_mejora_velocidad.h"

#ifdef WINCOMPIL
/* Localización del parche mingw32... Esto debería estar en otro lado, supongo. */
#include <herramientas/herramientas/herramientas.h>
#endif

using namespace App;

const tcolor Objeto_editor::color_seleccion={255, 255, 255, 255};
const tcolor Objeto_editor::color_punto_inicio_editor={255, 255, 192, 255};
const tcolor Objeto_editor::color_salida_editor={192, 255, 255, 255};
const tcolor Objeto_editor::color_obstaculo={255, 32, 32, 255};
const tcolor Objeto_editor::color_obstaculo_inocuo={128, 128, 128, 255};
const tcolor Objeto_editor::color_borde_obstaculo={210, 210, 128, 255};
const tcolor Objeto_editor::color_pieza_editor={64, 64, 128, 255};
const tcolor Objeto_editor::color_interruptor_editor={32, 128, 32, 255};
const tcolor Objeto_editor::color_puerta_editor={32, 128, 255, 255};
const tcolor Objeto_editor::color_mejora_velocidad_editor={128, 128, 32, 255};

Controlador_editor::Controlador_editor(DLibH::Log_base& l, const Fuentes& f)
	:log(l), 
	fuente_akashi(f.obtener_fuente("akashi", 16)),
	fuente_akashi_mensajes(f.obtener_fuente("akashi", 9)),
	camara(0, 0, 800, 500),
	mensajes(fuente_akashi_mensajes, 4, 16, 0),
	widget(nullptr),
	color_relleno({128, 128, 128, 255}),
	color_linea({128, 128, 128, 255}), grid(20), ver_flags(fvdeco_frente | fvdeco_fondo | fvobstaculos),
	tobjeto(tobjetocreado::obstaculo),
	decoracion_frente(false)
{
	mensajes.insertar_mensaje("F1 para ayuda.", 2.f);
}

void Controlador_editor::preloop(DFramework::Input& input, float delta)
{

}

void Controlador_editor::loop(DFramework::Input& input, float delta)
{
	mensajes.turno(delta);

	if(input.es_senal_salida())
	{
		abandonar_aplicacion();
		return;
	}

	if(input.es_input_down(Input::ayuda))
	{
		solicitar_cambio_estado(ayuda_editor);
		return;
	}

	if(input.es_input_down(Input::cambio_logica))
	{
		solicitar_cambio_estado(principal);
		return;
	}

	//Una flag para saber si tenemos que llamar a inicializar. Cutre, pero nos vale.
	bool inicializar_widget=true;

	if(widget.get())
	{
		inicializar_widget=false;
		if(widget->es_cerrar())
		{
			widget->finalizar(input);
			//TODO: alguna forma de controlar esto???.
			reordenar_decoraciones();
			mapa.establecer_info_camara(info_mapa.minx, info_mapa.maxx, info_mapa.miny, info_mapa.maxy);
			mapa.mut_id(info_mapa.id);
			widget.reset(nullptr);
		}
		else
		{
			widget->input(input, delta);
			return;
		}
	}

	pos_raton=input.acc_posicion_raton();
	localizar_elementos_bajo_cursor();

	
	if(input.es_input_down(Input::escape)) poligono_construccion=Poligono_2d<double>{};

	if(input.es_input_down(Input::tab)) intercambiar_objeto_creado();

	if(input.es_input_down(Input::cargar_mapa)) cargar_mapa();	
	else if(input.es_input_down(Input::grabar_mapa)) grabar_mapa();

	if(input.es_input_pulsado(Input::control_izquierdo)) 
	{
		if(input.es_input_down(Input::tecla_1)) intercambiar_visibilidad(fvdeco_frente, "decoración frente");
		else if(input.es_input_down(Input::tecla_2)) intercambiar_visibilidad(fvdeco_fondo, "decoración fondo");
		else if(input.es_input_down(Input::tecla_3)) intercambiar_visibilidad(fvobstaculos, "obstáculos");
	}

	if(input.es_input_down(Input::click_i)) 
	{
		if(input.es_input_pulsado(Input::control_izquierdo)) seleccionar();
		else crear();
	}	
	else if(input.es_input_down(Input::click_d)) click_derecho();
	
	if(input.es_input_pulsado(Input::suprimir)) eliminar();

	if(input.es_input_pulsado(Input::control_izquierdo) && input.es_input_down(Input::copiar)) copiar();
	else if(input.es_input_pulsado(Input::control_izquierdo) && input.es_input_down(Input::pegar)) pegar();
	else if(input.es_input_down(Input::seleccion_color)) widget.reset(new Widget_editor_color(fuente_akashi, color_relleno, color_linea));
	else if(input.es_input_down(Input::propiedades_mapa)) widget.reset(new Widget_editor_propiedades_mapa(fuente_akashi, info_mapa.id, info_mapa.minx, info_mapa.maxx, info_mapa.miny, info_mapa.maxy));

	if(input.es_input_down(Input::profundidad_mas)) cambiar_profundidad(1);
	else if(input.es_input_down(Input::profundidad_menos)) cambiar_profundidad(-1);

	if(input.es_input_pulsado(Input::control_izquierdo))
	{
		if(input.es_input_down(Input::cursor_arriba)) mover_seleccion(0.0, grid);
		else if(input.es_input_down(Input::cursor_abajo)) mover_seleccion(0.0, -grid);
		else if(input.es_input_down(Input::cursor_derecha)) mover_seleccion(grid, 0.0);
		else if(input.es_input_down(Input::cursor_izquierda)) mover_seleccion(-grid, 0.0);
	}
	else
	{
		if(input.es_input_pulsado(Input::cursor_arriba)) camara.movimiento_relativo(0, -500.0 * (double) delta);
		else if(input.es_input_pulsado(Input::cursor_abajo)) camara.movimiento_relativo(0, 500.0 * (double) delta);

		if(input.es_input_pulsado(Input::cursor_derecha)) camara.movimiento_relativo(500.0 * (double) delta, 0);
		else if(input.es_input_pulsado(Input::cursor_izquierda)) camara.movimiento_relativo(-500.0 * (double) delta, 0);
	}

	if(input.es_input_pulsado(Input::zoom_mas)) 
	{
		double zoom=camara.acc_zoom()-(double)delta;
		if(zoom > 10.0) zoom=10.0;
		camara.mut_zoom(zoom);
	}
	else if(input.es_input_pulsado(Input::zoom_menos)) 
	{
		double zoom=camara.acc_zoom()+(double)delta;
		if(zoom < 0.10) zoom=0.10;
		camara.mut_zoom(zoom);
	}
	else if(input.es_input_down(Input::reset_zoom))
	{
		camara.mut_zoom(1.0);
	}

	if(input.es_input_down(Input::grid_menos)) cambiar_grid(-1);
	else if(input.es_input_down(Input::grid_mas)) cambiar_grid(1);

	if(inicializar_widget && widget.get()) widget->inicializar(input);
}

void Controlador_editor::postloop(DFramework::Input& input, float delta)
{

}

void Controlador_editor::dibujar(DLibV::Pantalla& pantalla)
{
	pantalla.limpiar(0, 0, 0, 255);

	Representador r;
	r.dibujar_rejilla(pantalla, grid, {255, 255, 255, 64}, camara.acc_x(), -camara.acc_y(), camara.acc_zoom(), camara.acc_pos_w(), camara.acc_pos_h());

	auto pt_raton=punto_desde_pos_pantalla(pos_raton.x, pos_raton.y);

	//TODO
	if(ver_flags & fvobstaculos) for(const auto& eo : obstaculos) eo.dibujar(r, pantalla, camara);
	if(ver_flags & fvdeco_fondo) for(const auto& ed : decoraciones) if(!ed.elemento.es_frente()) ed.dibujar(r, pantalla, camara);
	if(ver_flags & fvdeco_frente) for(const auto& ed : decoraciones) if(ed.elemento.es_frente()) ed.dibujar(r, pantalla, camara);
	for(const auto& o : inicios) o.dibujar(r, pantalla, camara);
	for(const auto& o : salidas) o.dibujar(r, pantalla, camara);
	for(const auto& o : piezas) o.dibujar(r, pantalla, camara);
	for(const auto& o : interruptores) o.dibujar(r, pantalla, camara);
	for(const auto& o : puertas) o.dibujar(r, pantalla, camara);
	for(const auto& o : mejoras_velocidad) o.dibujar(r, pantalla, camara);

	for(const auto& oc : objetos_cursor) oc->dibujar(r, pantalla, camara, true);
	for(const auto& os : objetos_seleccionados) os->dibujar(r, pantalla, camara, true);

	for(const auto& s : poligono_construccion.acc_segmentos())
		r.dibujar_segmento(pantalla, s, {0, 255, 0, 128}, camara);
	
	//Segmento en construcción...
	if(poligono_construccion.acc_vertices().size())
	{
		const auto& v=poligono_construccion.acc_vertices().back();
		Segmento_2d<double> s{ {v.x, v.y}, {pt_raton.x, pt_raton.y}};
		r.dibujar_segmento(pantalla, s, {0, 255, 0, 128}, camara);
	}

	std::string texto="GRID: "+std::to_string((int)grid)
		+" CAM: "+std::to_string((int)camara.acc_x())+","+std::to_string((int)camara.acc_y())
		+" POS: "+std::to_string((int)pt_raton.x)+","+std::to_string((int)pt_raton.y);

	switch(tobjeto)
	{
	//TODO
		case tobjetocreado::obstaculo: texto+=" [geometry]"; break;
		case tobjetocreado::decoracion: texto+=" [decoration]"; break;
		case tobjetocreado::inicio: texto+=" [spawn point]"; break;
		case tobjetocreado::salida: texto+=" [exit]"; break;
		case tobjetocreado::pieza: texto+=" [pieces]"; break;
		case tobjetocreado::interruptor: texto+=" [switch]"; break;
		case tobjetocreado::puerta: texto+=" [door]"; break;
		case tobjetocreado::mejora_velocidad: texto+=" [speed]"; break;
	}

	DLibV::Representacion_TTF txt(fuente_akashi, {255, 255, 255, 255}, texto);
	txt.ir_a(16, camara.acc_pos_h()-32);
	txt.volcar(pantalla);

	r.dibujar_poligono(pantalla, Objeto_editor::cuadrado(pt_raton.x, pt_raton.y, 3), {255, 255, 255, 128}, camara);

	//Color fondo y de línea...
	r.dibujar_poligono_sin_transformar(pantalla, Objeto_editor::cuadrado(570, 30, 10), color_relleno);
	r.dibujar_poligono_sin_transformar(pantalla, Objeto_editor::cuadrado(580, 40, 10), color_linea);

	mensajes.dibujar(pantalla);
	if(widget.get()) widget->dibujar(pantalla);
}

void Controlador_editor::despertar()
{

}

void Controlador_editor::dormir()
{
	tobjeto=tobjetocreado::obstaculo;
	poligono_construccion=Poligono_2d<double>{};
}

bool Controlador_editor::es_posible_abandonar_estado() const
{
	return true;
}

//////////////////////

void Controlador_editor::cargar_mapa()
{
	const std::string nombre_fichero="mapa.dat";

	mapa.limpiar();
	Importador importador;
	importador.importar(nombre_fichero.c_str(), mapa);
	obtener_desde_mapa();
	mapa.limpiar();
	reordenar_decoraciones();

	mensajes.insertar_mensaje(nombre_fichero+" cargado de disco", 2.f);
}

void Controlador_editor::grabar_mapa()
{
	const std::string nombre_fichero="mapa.dat";

	std::ofstream fichero(nombre_fichero.c_str());
	Exportador exportador;
	aplicar_a_mapa(mapa);
	fichero<<exportador.serializar(mapa);

	mensajes.insertar_mensaje(nombre_fichero+" guardado en disco", 2.f);
}

void Controlador_editor::intercambiar_objeto_creado()
{
	switch(tobjeto)
	{
		//TODO
		case tobjetocreado::obstaculo: 		tobjeto=tobjetocreado::decoracion; break;
		case tobjetocreado::decoracion: 	tobjeto=tobjetocreado::inicio; break;
		case tobjetocreado::inicio: 		tobjeto=tobjetocreado::salida; break;
		case tobjetocreado::salida: 		tobjeto=tobjetocreado::pieza; break;
		case tobjetocreado::pieza: 		tobjeto=tobjetocreado::interruptor; break;
		case tobjetocreado::interruptor: 	tobjeto=tobjetocreado::puerta; break;
		case tobjetocreado::puerta: 		tobjeto=tobjetocreado::mejora_velocidad; break;
		case tobjetocreado::mejora_velocidad: 	tobjeto=tobjetocreado::obstaculo; break;
	}
}

void Controlador_editor::crear()
{
	switch(tobjeto)
	{
		case tobjetocreado::obstaculo:
		case tobjetocreado::decoracion:
		case tobjetocreado::salida:
		case tobjetocreado::puerta:
			nuevo_punto(punto_desde_pos_pantalla(pos_raton.x, pos_raton.y));
		break;
		case tobjetocreado::inicio:
			nuevo_inicio(punto_desde_pos_pantalla(pos_raton.x, pos_raton.y));
		break;
		case tobjetocreado::pieza:
			nueva_pieza(punto_desde_pos_pantalla(pos_raton.x, pos_raton.y));
		break;
		case tobjetocreado::interruptor:
			nuevo_interruptor(punto_desde_pos_pantalla(pos_raton.x, pos_raton.y));
		break;
		case tobjetocreado::mejora_velocidad:
			nueva_mejora_velocidad(punto_desde_pos_pantalla(pos_raton.x, pos_raton.y));
		break;
	//TODO
	}
}

void Controlador_editor::click_derecho()
{
	switch(tobjeto)
	{
		case tobjetocreado::obstaculo:
			if(ver_flags & fvobstaculos)
			{
				std::vector<Obstaculo_editor *> v;
				localizar_elementos_bajo_cursor_helper(obstaculos, v, punto_desde_pos_pantalla(pos_raton.x, pos_raton.y, false));
				if(v.size()==1) widget.reset(new Widget_editor_obstaculo(fuente_akashi, v[0]->elemento));
			}
		break;
		case tobjetocreado::decoracion:
			if(ver_flags & fvdeco_fondo || ver_flags & fvdeco_frente)
			{
				std::vector<Decoracion_editor *> v;
				localizar_elementos_bajo_cursor_helper(decoraciones, v, punto_desde_pos_pantalla(pos_raton.x, pos_raton.y, false));
				if(v.size()==1) widget.reset(new Widget_editor_decoracion(fuente_akashi, v[0]->elemento));
			}
		break;
		case tobjetocreado::inicio:
		{
			std::vector<Inicio_editor *> v;
			localizar_elementos_bajo_cursor_helper(inicios, v, punto_desde_pos_pantalla(pos_raton.x, pos_raton.y, false));
			if(v.size()==1) widget.reset(new Widget_editor_propiedades_inicio(fuente_akashi, v[0]->elemento));
		}
		break;
		case tobjetocreado::salida:
		{
			std::vector<Salida_editor *> v;
			localizar_elementos_bajo_cursor_helper(salidas, v, punto_desde_pos_pantalla(pos_raton.x, pos_raton.y, false));
			if(v.size()==1) widget.reset(new Widget_editor_propiedades_salida(fuente_akashi, v[0]->elemento));
		}
		break;
		case tobjetocreado::pieza:
		{
			std::vector<Pieza_editor *> v;
			localizar_elementos_bajo_cursor_helper(piezas, v, punto_desde_pos_pantalla(pos_raton.x, pos_raton.y, false));
			if(v.size()==1) widget.reset(new Widget_editor_propiedades_pieza(fuente_akashi, v[0]->elemento));
		}
		break;
		case tobjetocreado::interruptor:
		{
			std::vector<Interruptor_editor *> v;
			localizar_elementos_bajo_cursor_helper(interruptores, v, punto_desde_pos_pantalla(pos_raton.x, pos_raton.y, false));
			if(v.size()==1) widget.reset(new Widget_editor_propiedades_interruptor(fuente_akashi, v[0]->elemento));
		}
		break;
		case tobjetocreado::puerta:
		{
			std::vector<Puerta_editor *> v;
			localizar_elementos_bajo_cursor_helper(puertas, v, punto_desde_pos_pantalla(pos_raton.x, pos_raton.y, false));
			if(v.size()==1) widget.reset(new Widget_editor_propiedades_puerta(fuente_akashi, v[0]->elemento));
		}
		break;
		case tobjetocreado::mejora_velocidad:
		{
			std::vector<Mejora_velocidad_editor *> v;
			localizar_elementos_bajo_cursor_helper(mejoras_velocidad, v, punto_desde_pos_pantalla(pos_raton.x, pos_raton.y, false));
			if(v.size()==1) widget.reset(new Widget_editor_propiedades_mejora_velocidad(fuente_akashi, v[0]->elemento));
		}
		break;
	
		//TODO
	}
}

void Controlador_editor::seleccionar()
{	
	if(objetos_cursor.size())
	{
		for(auto& o : objetos_cursor)
		{
			auto it=std::find(std::begin(objetos_seleccionados), std::end(objetos_seleccionados), o);

			if(it!=std::end(objetos_seleccionados))
			{
				objetos_seleccionados.erase(it);
			}
			else
			{
				objetos_seleccionados.push_back(o);
			}
		}
	}
	else
	{
		objetos_seleccionados.clear();
	}
}

DLibH::Punto_2d<double>	Controlador_editor::punto_desde_pos_pantalla(int x, int y, bool a_rejilla)
{

	int px=camara.acc_x() + (x * camara.acc_zoom());
	int py=-camara.acc_y() - (y * camara.acc_zoom());

	if(a_rejilla)
	{
		px=round((double)px / (double)grid) * grid;
		py=round((double)py / (double)grid) * grid;
	}

	return DLibH::Punto_2d<double>{(double)px, (double)py};
}

void Controlador_editor::nuevo_punto(DLibH::Punto_2d<double> p)
{
	if(poligono_construccion.acc_vertices().size() > 2 && p==poligono_construccion.acc_vertices()[0])
	{
		cerrar_poligono();
	}
	else
	{
		const auto& v=poligono_construccion.acc_vertices();
		if(std::any_of(std::begin(v), std::end(v), [p](const DLibH::Punto_2d<double>& b) {return p==b;}))
		{
			mensajes.insertar_mensaje("Vértice repetido, no se crea.", 2.f);
		}
		else
		{
			poligono_construccion.insertar_vertice(p);
		}
	}
}

void Controlador_editor::nuevo_inicio(DLibH::Punto_2d<double> p)
{
	inicios.push_back(Inicio_editor({p, 0, 0}));
}

void Controlador_editor::nuevo_interruptor(DLibH::Punto_2d<double> p)
{
	interruptores.push_back(Interruptor_editor({p, 0, 0, 0, 0}));
}

void Controlador_editor::nueva_pieza(DLibH::Punto_2d<double> p)
{
	piezas.push_back(Pieza_editor({p, 0}));
}

void Controlador_editor::nueva_mejora_velocidad(DLibH::Punto_2d<double> p)
{
	mejoras_velocidad.push_back(Mejora_velocidad_editor({p, 0}));
}

void Controlador_editor::cerrar_poligono()
{
	if(poligono_construccion.size() < 3)
	{
		mensajes.insertar_mensaje("Polígono con menos de dos vértices", 2.f);
	}
	else
	{
		if(!poligono_construccion.es_sentido_horario())
		{
			mensajes.insertar_mensaje("Polígono no trazado en sentido horario", 2.f);
		}
		else
		{
			if(tobjeto==tobjetocreado::obstaculo)
			{
				if(poligono_construccion.es_concavo())
				{
					mensajes.insertar_mensaje("Polígono cóncavo o erróneo", 2.f);
				}
				else
				{
					poligono_construccion.cerrar();
					obstaculos.push_back(Obstaculo_editor(Obstaculo(poligono_construccion, Obstaculo::ttipos::normal)));
				}
			}
			else if(tobjeto==tobjetocreado::salida)
			{
				if(poligono_construccion.es_concavo())
				{
					mensajes.insertar_mensaje("Polígono cóncavo o erróneo", 2.f);
				}
				else
				{
					poligono_construccion.cerrar();
					salidas.push_back(Salida_editor(Salida(poligono_construccion, 0, 0)));
				}
			}
			else if(tobjeto==tobjetocreado::puerta)
			{
				if(poligono_construccion.es_concavo())
				{
					mensajes.insertar_mensaje("Polígono cóncavo o erróneo", 2.f);
				}
				else
				{
					poligono_construccion.cerrar();
					puertas.push_back(Puerta_editor(Puerta(poligono_construccion, 0)));
				}
			}
			else if(tobjeto==tobjetocreado::decoracion)
			{
				poligono_construccion.cerrar();
				decoraciones.push_back(Decoracion_editor(Decoracion(poligono_construccion, color_relleno, color_linea, decoracion_frente, 100)));
			}
		}
	}

	poligono_construccion=Poligono_2d<double>{};
}

void Controlador_editor::localizar_elementos_bajo_cursor()
{
	auto pt_raton=punto_desde_pos_pantalla(pos_raton.x, pos_raton.y, false);
	objetos_cursor.clear();

	if(ver_flags & fvobstaculos) localizar_elementos_bajo_cursor_helper(obstaculos, objetos_cursor, pt_raton);
	if(ver_flags & fvdeco_fondo || ver_flags & fvdeco_frente) localizar_elementos_bajo_cursor_helper(decoraciones, objetos_cursor, pt_raton);

	//TODO
	localizar_elementos_bajo_cursor_helper(inicios, objetos_cursor, pt_raton);
	localizar_elementos_bajo_cursor_helper(salidas, objetos_cursor, pt_raton);
	localizar_elementos_bajo_cursor_helper(piezas, objetos_cursor, pt_raton);
	localizar_elementos_bajo_cursor_helper(interruptores, objetos_cursor, pt_raton);
	localizar_elementos_bajo_cursor_helper(puertas, objetos_cursor, pt_raton);
	localizar_elementos_bajo_cursor_helper(mejoras_velocidad, objetos_cursor, pt_raton);
}

void Controlador_editor::mover_seleccion(double x, double y)
{
	for(auto& o : objetos_seleccionados) o->mover(x, y);
}

void Controlador_editor::aplicar_a_mapa(Mapa& m)
{
	m.limpiar();

	//TODO
	for(auto& o : obstaculos) m.obstaculos.push_back(o.elemento);
	for(auto& o : decoraciones) m.decoraciones.push_back(o.elemento);
	for(auto& o : inicios) m.inicios.push_back(o.elemento);
	for(auto& o : salidas) m.salidas.push_back(o.elemento);
	for(auto& o : piezas) m.piezas.push_back(o.elemento);
	for(auto& o : interruptores) m.interruptores.push_back(o.elemento);
	for(auto& o : puertas) m.puertas.push_back(o.elemento);
	for(auto& o : mejoras_velocidad) m.mejoras_velocidad.push_back(o.elemento);
}

void Controlador_editor::obtener_desde_mapa()
{
	obstaculos.clear();
	decoraciones.clear();

	//TODO
	for(auto& o : mapa.obstaculos) obstaculos.push_back(Obstaculo_editor(o));
	for(auto& o : mapa.decoraciones) decoraciones.push_back(Decoracion_editor(o));
	for(auto& o : mapa.inicios) inicios.push_back(Inicio_editor(o));
	for(auto& o : mapa.salidas) salidas.push_back(Salida_editor(o));
	for(auto& o : mapa.piezas) piezas.push_back(Pieza_editor(o));
	for(auto& o : mapa.interruptores) interruptores.push_back(Interruptor_editor(o));
	for(auto& o : mapa.puertas) puertas.push_back(Puerta_editor(o));
	for(auto& o : mapa.mejoras_velocidad) mejoras_velocidad.push_back(Mejora_velocidad_editor(o));

}

void Controlador_editor::eliminar()
{
	//Esto los marca para borrar...
	for(auto& o : objetos_seleccionados) o->borrar();
	objetos_seleccionados.clear();

	//Sólo borramos bajo el cursor los del tipo activo.
	auto pt_raton=punto_desde_pos_pantalla(pos_raton.x, pos_raton.y, false);
	std::vector<Objeto_editor *> cursor;

	switch(tobjeto)
	{
	//TODO
		case tobjetocreado::obstaculo: localizar_elementos_bajo_cursor_helper(obstaculos, cursor, pt_raton); break;
		case tobjetocreado::decoracion: localizar_elementos_bajo_cursor_helper(decoraciones, cursor, pt_raton); break;
		case tobjetocreado::inicio: localizar_elementos_bajo_cursor_helper(inicios, cursor, pt_raton); break;
		case tobjetocreado::salida: localizar_elementos_bajo_cursor_helper(salidas, cursor, pt_raton); break;
		case tobjetocreado::pieza: localizar_elementos_bajo_cursor_helper(piezas, cursor, pt_raton); break;
		case tobjetocreado::interruptor: localizar_elementos_bajo_cursor_helper(interruptores, cursor, pt_raton); break;
		case tobjetocreado::puerta: localizar_elementos_bajo_cursor_helper(puertas, cursor, pt_raton); break;
		case tobjetocreado::mejora_velocidad: localizar_elementos_bajo_cursor_helper(mejoras_velocidad, cursor, pt_raton); break;
	}
	
	for(auto& o : cursor) o->borrar();

	//Esto los borra... Pasamos por todos los tipos ya que podríamos tener más cosas seleccionadas.
	eliminar_helper(obstaculos);
	eliminar_helper(decoraciones);
}

void Controlador_editor::cambiar_grid(int dir)
{
	if(dir < 0) grid-=10.0;
	else grid+=10;

	if(grid < 10.0) grid=10.0;
	else if(grid > 50.0) grid=50.0;
}

//Mejor "pegar propiedades"?
void Controlador_editor::pegar()
{
	switch(tobjeto)
	{
		case tobjetocreado::obstaculo: 
		{
			std::vector<Obstaculo_editor *> v;
			localizar_elementos_bajo_cursor_helper(obstaculos, v, punto_desde_pos_pantalla(pos_raton.x, pos_raton.y, false));
			if(v.size())
			{
				mensajes.insertar_mensaje("Propiedades pegadas", 2.0f);
				for(auto& e : v)
				{
					auto& item=e->elemento;
					item.mut_tipo(info_obstaculo.tipo);
				}
			}
		}
		break;
		case tobjetocreado::decoracion:
		{
			std::vector<Decoracion_editor *> v;
			localizar_elementos_bajo_cursor_helper(decoraciones, v, punto_desde_pos_pantalla(pos_raton.x, pos_raton.y, false));
			if(v.size())
			{
				mensajes.insertar_mensaje("Propiedades copiadas", 2.0f);
				for(auto &e : v)
				{
					auto& item=e->elemento;
					item.mut_color(info_decoracion.relleno);
					item.mut_color_linea(info_decoracion.linea);
					item.mut_profundidad(info_decoracion.orden);
					item.mut_frente(info_decoracion.frente);
				}
			}
		}
		break;
		case tobjetocreado::inicio:
		case tobjetocreado::salida:
		case tobjetocreado::pieza:
		case tobjetocreado::interruptor:
		case tobjetocreado::puerta:
		case tobjetocreado::mejora_velocidad:

		break;

		//TODO
	}
}

void Controlador_editor::copiar()
{
	switch(tobjeto)
	{
		case tobjetocreado::obstaculo: 
		{
			std::vector<Obstaculo_editor *> v;
			localizar_elementos_bajo_cursor_helper(obstaculos, v, punto_desde_pos_pantalla(pos_raton.x, pos_raton.y, false));
			if(v.size()==1)
			{
				mensajes.insertar_mensaje("Propiedades copiadas", 2.0f);
				const auto& item=v[0]->elemento;
				info_obstaculo={item.acc_tipo()};
			}
		}
		break;
		case tobjetocreado::decoracion:
		{
			std::vector<Decoracion_editor *> v;
			localizar_elementos_bajo_cursor_helper(decoraciones, v, punto_desde_pos_pantalla(pos_raton.x, pos_raton.y, false));
			if(v.size()==1)
			{
				mensajes.insertar_mensaje("Propiedades copiadas", 2.0f);
				const auto& item=v[0]->elemento;
				info_decoracion={item.acc_color(), item.acc_color_linea(), item.acc_profundidad(), item.es_frente()};
			}
		}
		break;
		case tobjetocreado::inicio:
		case tobjetocreado::salida:
		case tobjetocreado::pieza:
		case tobjetocreado::interruptor:
		case tobjetocreado::puerta:
		case tobjetocreado::mejora_velocidad:

		break;

		//TODO
	}
}

void Controlador_editor::intercambiar_visibilidad(int val, const std::string& tipo)
{
	if(ver_flags & val) 
	{
		ver_flags-=val;
		mensajes.insertar_mensaje("Ocultando "+tipo, 2.0f);
	}
	else 
	{
		ver_flags+=val;
		mensajes.insertar_mensaje("Mostrando "+tipo, 2.0f);
	}
}

void Controlador_editor::cambiar_profundidad(int dir)
{
	std::vector<Decoracion_editor *> v;
	localizar_elementos_bajo_cursor_helper(decoraciones, v, punto_desde_pos_pantalla(pos_raton.x, pos_raton.y, false));

	if(v.size()==1)
	{
		if(dir > 0) v[0]->elemento.subir_profundidad();
		else v[0]->elemento.bajar_profundidad();

		reordenar_decoraciones();
		mensajes.insertar_mensaje("Nueva profundidad de "+std::to_string(v[0]->elemento.acc_profundidad()), 2.0f);
	}
}

void Controlador_editor::reordenar_decoraciones()
{
	std::sort(
		std::begin(decoraciones), 
		std::end(decoraciones), 
		[](const Decoracion_editor& a, const Decoracion_editor& b) {return a.elemento < b.elemento;});
}
