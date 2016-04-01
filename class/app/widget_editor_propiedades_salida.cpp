#include "widget_editor_propiedades_salida.h"

#include "framework_impl/input.h"

#include <def_video.h>

using namespace App;

Widget_editor_propiedades_salida::Widget_editor_propiedades_salida(const DLibV::Fuente_TTF& fuente, Salida& i)
	:ref(i), id_mapa(ref.acc_id_mapa()), id_inicio(ref.acc_id_inicio()), cerrar(false),
	indice_actual(min_indice)
{
	layout.mapear_fuente("akashi", fuente);
	layout.parsear("data/layout/widget_salida.dnot", "layout");

	actualizar_layout();
}
	
void Widget_editor_propiedades_salida::dibujar(DLibV::Pantalla& pantalla)
{
	using namespace DLibH;
	layout.volcar(pantalla);
}

void Widget_editor_propiedades_salida::input(DFramework::Input& input, float delta)
{
	if(input.es_input_down(Input::escape)) 
	{
		cerrar=true;
		return;
	}

	if(input.es_input_down(Input::cursor_abajo)) cambiar_seleccion(1);
	else if(input.es_input_down(Input::cursor_arriba)) cambiar_seleccion(-1);

	if(input.es_eventos_input_texto ())
	{
		static_cast<DLibV::Representacion_TTF *>(layout.obtener_por_id("txt_input"))->asignar(input.acc_input_texto());
	}

	if(input.es_input_down(Input::enter) && input.acc_input_texto().size())
	{
		int val=0;

		try
		{
			val=std::stoi(input.acc_input_texto());
		}
		catch(std::exception& e){}

		cambiar_por_indice(indice_actual, val);
		input.vaciar_input_texto();
		static_cast<DLibV::Representacion_TTF *>(layout.obtener_por_id("txt_input"))->asignar("");
	}
}

void Widget_editor_propiedades_salida::cambiar_por_indice(int indice, int val)
{
	switch(indice)
	{
		case 0: cambiar(val, id_mapa, "txt_id_mapa"); break;
		case 1: cambiar(val, id_inicio, "txt_id_inicio"); break;
	}
}

int Widget_editor_propiedades_salida::valor_por_indice(int indice) const
{
	switch(indice)
	{
		case 0: return id_mapa; break;
		case 1: return id_inicio; break;
	}

	return 0;
}

bool Widget_editor_propiedades_salida::es_cerrar() const
{
	return cerrar;
}

void Widget_editor_propiedades_salida::cambiar_seleccion(int dir)
{
	indice_actual+=dir;
	if(indice_actual < min_indice) indice_actual=min_indice;
	else if(indice_actual > max_indice) indice_actual=max_indice;

	int y=layout.const_int("y_selector")+(indice_actual * layout.const_int("salto_selector"));
	layout.obtener_por_id("selector")->ir_a(layout.const_int("x_selector"), y);
}

void Widget_editor_propiedades_salida::cambiar(int val, int& ref, const std::string& id)
{
	ref=val;
	std::string cad_final=std::to_string(val);
	static_cast<DLibV::Representacion_TTF *>(layout.obtener_por_id(id))->asignar(cad_final);
}

void Widget_editor_propiedades_salida::actualizar_layout()
{
	cambiar(id_mapa, id_mapa, "txt_id_mapa");
	cambiar(id_inicio, id_inicio, "txt_id_inicio");
}

void Widget_editor_propiedades_salida::finalizar(DFramework::Input& input)
{
	input.vaciar_input_texto();
	input.finalizar_input_texto();
	ref.mut_id_mapa(id_mapa);
	ref.mut_id_inicio(id_inicio);
}

void Widget_editor_propiedades_salida::inicializar(DFramework::Input& input)
{
	input.iniciar_input_texto();
}
