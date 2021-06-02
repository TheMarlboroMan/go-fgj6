#include "widget_editor_propiedades_interruptor.h"

#include "framework_impl/input.h"

#include <def_video.h>
#include "definiciones.h"

#ifdef WINCOMPIL
/* Localización del parche mingw32... Esto debería estar en otro lado, supongo. */
#include <templates/parches_compat.h>
#endif

using namespace App;

Widget_editor_propiedades_interruptor::Widget_editor_propiedades_interruptor(const DLibV::Fuente_TTF& fuente, Interruptor& i)
	:ref(i), nivel(ref.acc_nivel()), id_puerta(ref.acc_id_puerta()),
	id_grupo(ref.acc_id_grupo()), tiempo_grupo(ref.acc_tiempo_grupo()),
	cerrar(false),	indice_actual(min_indice)
{
	layout.mapear_fuente("akashi", fuente);
	layout.parsear(env::data_path+"/data/layout/widget_interruptor.dnot", "layout");

	actualizar_layout();
}

void Widget_editor_propiedades_interruptor::dibujar(DLibV::Pantalla& pantalla)
{
	using namespace DLibH;
	layout.volcar(pantalla);
}

void Widget_editor_propiedades_interruptor::input(DFramework::Input& input, float delta)
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

void Widget_editor_propiedades_interruptor::cambiar_por_indice(int indice, int val)
{
	switch(indice)
	{
		case 0: cambiar(val, nivel, "txt_nivel"); break;
		case 1: cambiar(val, id_puerta, "txt_id_puerta"); break;
		case 2: cambiar(val, id_grupo, "txt_id_grupo"); break;
		case 3: cambiar(val, tiempo_grupo, "txt_tiempo_grupo"); break;
	}
}

int Widget_editor_propiedades_interruptor::valor_por_indice(int indice) const
{
	switch(indice)
	{
		case 0: return nivel; break;
		case 1: return id_puerta; break;
		case 2: return id_grupo; break;
		case 3: return tiempo_grupo; break;
	}

	return 0;
}

void Widget_editor_propiedades_interruptor::actualizar_layout()
{
	cambiar(nivel, nivel, "txt_nivel");
	cambiar(id_puerta, id_puerta, "txt_id_puerta");
	cambiar(id_grupo, id_grupo, "txt_id_grupo");
	cambiar(tiempo_grupo, tiempo_grupo, "txt_tiempo_grupo");

}

bool Widget_editor_propiedades_interruptor::es_cerrar() const
{
	return cerrar;
}

void Widget_editor_propiedades_interruptor::cambiar_seleccion(int dir)
{
	indice_actual+=dir;
	if(indice_actual < min_indice) indice_actual=min_indice;
	else if(indice_actual > max_indice) indice_actual=max_indice;

	int y=layout.const_int("y_selector")+(indice_actual * layout.const_int("salto_selector"));
	layout.obtener_por_id("selector")->ir_a(layout.const_int("x_selector"), y);
}

void Widget_editor_propiedades_interruptor::cambiar(int val, int& ref, const std::string& id)
{
#ifdef WINCOMPIL
	using namespace compat;
#else
	using namespace std;
#endif


	ref=val;
	std::string cad_final=to_string(val);
	static_cast<DLibV::Representacion_TTF *>(layout.obtener_por_id(id))->asignar(cad_final);
}

void Widget_editor_propiedades_interruptor::finalizar(DFramework::Input& input)
{
	input.vaciar_input_texto();
	input.finalizar_input_texto();
	ref.mut_nivel(nivel);
	ref.mut_id_puerta(id_puerta);
	ref.mut_id_grupo(id_grupo);
	ref.mut_tiempo_grupo(tiempo_grupo);
}

void Widget_editor_propiedades_interruptor::inicializar(DFramework::Input& input)
{
	input.iniciar_input_texto();
}
