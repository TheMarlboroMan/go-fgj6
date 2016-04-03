#include "widget_editor_propiedades_pieza.h"

#include "framework_impl/input.h"

#include <def_video.h>

#ifdef WINCOMPIL
/* Localización del parche mingw32... Esto debería estar en otro lado, supongo. */
#include <herramientas/herramientas/herramientas.h>
#endif

using namespace App;

Widget_editor_propiedades_pieza::Widget_editor_propiedades_pieza(const DLibV::Fuente_TTF& fuente, Pieza& i)
	:ref(i), indice_pieza(ref.acc_indice()), cerrar(false),
	indice_actual(min_indice)
{
	layout.mapear_fuente("akashi", fuente);
	layout.parsear("data/layout/widget_pieza.dnot", "layout");

	actualizar_layout();
}
	
void Widget_editor_propiedades_pieza::dibujar(DLibV::Pantalla& pantalla)
{
	using namespace DLibH;
	layout.volcar(pantalla);
}

void Widget_editor_propiedades_pieza::input(DFramework::Input& input, float delta)
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

void Widget_editor_propiedades_pieza::cambiar_por_indice(int indice, int val)
{
	switch(indice)
	{
		case 0: cambiar(val, indice_pieza, "txt_indice"); break;
	}
}

int Widget_editor_propiedades_pieza::valor_por_indice(int indice) const
{
	switch(indice)
	{
		case 0: return indice_pieza; break;
	}

	return 0;
}

void Widget_editor_propiedades_pieza::actualizar_layout()
{
	cambiar(indice_pieza, indice_pieza, "txt_indice");
}

bool Widget_editor_propiedades_pieza::es_cerrar() const
{
	return cerrar;
}

void Widget_editor_propiedades_pieza::cambiar_seleccion(int dir)
{
	indice_actual+=dir;
	if(indice_actual < min_indice) indice_actual=min_indice;
	else if(indice_actual > max_indice) indice_actual=max_indice;

	int y=layout.const_int("y_selector")+(indice_actual * layout.const_int("salto_selector"));
	layout.obtener_por_id("selector")->ir_a(layout.const_int("x_selector"), y);
}

void Widget_editor_propiedades_pieza::cambiar(int val, int& ref, const std::string& id)
{
#ifdef WINCOMPIL
	using namespace parche_mingw;
#else
	using namespace std;
#endif

	ref=val;
	std::string cad_final=to_string(val);
	static_cast<DLibV::Representacion_TTF *>(layout.obtener_por_id(id))->asignar(cad_final);
}

void Widget_editor_propiedades_pieza::finalizar(DFramework::Input& input)
{
	input.vaciar_input_texto();
	input.finalizar_input_texto();
	ref.mut_indice(indice_pieza);
}

void Widget_editor_propiedades_pieza::inicializar(DFramework::Input& input)
{
	input.iniciar_input_texto();
}
