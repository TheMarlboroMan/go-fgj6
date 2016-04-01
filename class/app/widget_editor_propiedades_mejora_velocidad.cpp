#include "widget_editor_propiedades_mejora_velocidad.h"

#include "framework_impl/input.h"

#include <def_video.h>

using namespace App;

Widget_editor_propiedades_mejora_velocidad::Widget_editor_propiedades_mejora_velocidad(const DLibV::Fuente_TTF& fuente, Mejora_velocidad& i)
	:ref(i), nivel(ref.acc_nivel()), cerrar(false),
	indice_actual(min_indice)
{
	layout.mapear_fuente("akashi", fuente);
	layout.parsear("data/layout/widget_mejora_velocidad.dnot", "layout");

	actualizar_layout();
}
	
void Widget_editor_propiedades_mejora_velocidad::dibujar(DLibV::Pantalla& pantalla)
{
	using namespace DLibH;
	layout.volcar(pantalla);
}

void Widget_editor_propiedades_mejora_velocidad::input(DFramework::Input& input, float delta)
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

void Widget_editor_propiedades_mejora_velocidad::cambiar_por_indice(int indice, int val)
{
	switch(indice)
	{
		case 0: cambiar(val, nivel, "txt_nivel"); break;
	}
}

int Widget_editor_propiedades_mejora_velocidad::valor_por_indice(int indice) const
{
	switch(indice)
	{
		case 0: return nivel; break;
	}

	return 0;
}

void Widget_editor_propiedades_mejora_velocidad::actualizar_layout()
{
	cambiar(nivel, nivel, "txt_nivel");
}

bool Widget_editor_propiedades_mejora_velocidad::es_cerrar() const
{
	return cerrar;
}

void Widget_editor_propiedades_mejora_velocidad::cambiar_seleccion(int dir)
{
	indice_actual+=dir;
	if(indice_actual < min_indice) indice_actual=min_indice;
	else if(indice_actual > max_indice) indice_actual=max_indice;

	int y=layout.const_int("y_selector")+(indice_actual * layout.const_int("salto_selector"));
	layout.obtener_por_id("selector")->ir_a(layout.const_int("x_selector"), y);
}

void Widget_editor_propiedades_mejora_velocidad::cambiar(int val, int& ref, const std::string& id)
{
	ref=val;
	std::string cad_final=std::to_string(val);
	static_cast<DLibV::Representacion_TTF *>(layout.obtener_por_id(id))->asignar(cad_final);
}

void Widget_editor_propiedades_mejora_velocidad::finalizar(DFramework::Input& input)
{
	input.vaciar_input_texto();
	input.finalizar_input_texto();
	ref.mut_nivel(nivel);
}

void Widget_editor_propiedades_mejora_velocidad::inicializar(DFramework::Input& input)
{
	input.iniciar_input_texto();
}
