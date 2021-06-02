#include "widget_editor_propiedades_inicio.h"

#include "framework_impl/input.h"

#include <def_video.h>
#include "definiciones.h"

#ifdef WINCOMPIL
/* Localización del parche mingw32... Esto debería estar en otro lado, supongo. */
#include <templates/parches_compat.h>
#endif

using namespace App;

Widget_editor_propiedades_inicio::Widget_editor_propiedades_inicio(const DLibV::Fuente_TTF& fuente, Inicio& i)
	:ref(i), id(ref.acc_id()), angulo(ref.acc_angulo()), cerrar(false),
	indice_actual(min_indice)
{
	layout.mapear_fuente("akashi", fuente);
	layout.parsear(env::data_path+"/data/layout/widget_inicio.dnot", "layout");

	actualizar_layout();
}

void Widget_editor_propiedades_inicio::dibujar(DLibV::Pantalla& pantalla)
{
	using namespace DLibH;
	layout.volcar(pantalla);
}

void Widget_editor_propiedades_inicio::input(DFramework::Input& input, float delta)
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
			val=stoi(input.acc_input_texto());
		}
		catch(std::exception& e){}

		cambiar_por_indice(indice_actual, val);
		input.vaciar_input_texto();
		static_cast<DLibV::Representacion_TTF *>(layout.obtener_por_id("txt_input"))->asignar("");
	}
}

void Widget_editor_propiedades_inicio::cambiar_por_indice(int indice, int val)
{
	switch(indice)
	{
		case 0: cambiar(val, id, "txt_id"); break;
		case 1: cambiar(val, angulo, "txt_angulo"); break;
	}
}

int Widget_editor_propiedades_inicio::valor_por_indice(int indice) const
{
	switch(indice)
	{
		case 0: return id; break;
		case 1: return angulo; break;
	}

	return 0;
}

bool Widget_editor_propiedades_inicio::es_cerrar() const
{
	return cerrar;
}

void Widget_editor_propiedades_inicio::cambiar_seleccion(int dir)
{
	indice_actual+=dir;
	if(indice_actual < min_indice) indice_actual=min_indice;
	else if(indice_actual > max_indice) indice_actual=max_indice;

	int y=layout.const_int("y_selector")+(indice_actual * layout.const_int("salto_selector"));
	layout.obtener_por_id("selector")->ir_a(layout.const_int("x_selector"), y);
}

void Widget_editor_propiedades_inicio::cambiar(int val, int& ref, const std::string& id)
{
	ref=val;
#ifdef WINCOMPIL
	using namespace compat;
#else
	using namespace std;
#endif

	std::string cad_final=to_string(val);
	static_cast<DLibV::Representacion_TTF *>(layout.obtener_por_id(id))->asignar(cad_final);
}

void Widget_editor_propiedades_inicio::actualizar_layout()
{
	cambiar(id, id, "txt_id");
	cambiar(angulo, angulo, "txt_angulo");
}

void Widget_editor_propiedades_inicio::finalizar(DFramework::Input& input)
{
	input.vaciar_input_texto();
	input.finalizar_input_texto();
	ref.mut_id(id);
	ref.mut_angulo(angulo);
}

void Widget_editor_propiedades_inicio::inicializar(DFramework::Input& input)
{
	input.iniciar_input_texto();
}
