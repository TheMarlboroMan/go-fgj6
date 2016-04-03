#include "widget_editor_propiedades_mapa.h"

#include "framework_impl/input.h"

#include <def_video.h>

#ifdef WINCOMPIL
/* Localización del parche mingw32... Esto debería estar en otro lado, supongo. */
#include <herramientas/herramientas/herramientas.h>
#endif

using namespace App;

Widget_editor_propiedades_mapa::Widget_editor_propiedades_mapa(const DLibV::Fuente_TTF& fuente, Info_mapa& ref)
	:ref(ref), id(ref.id), id_fondo(ref.id_fondo), minx(ref.minx), maxx(ref.maxx), miny(ref.miny), maxy(ref.maxy), cerrar(false),
	indice_actual(min_indice)
{
	layout.mapear_fuente("akashi", fuente);
	layout.parsear("data/layout/widget_mapa.dnot", "layout");

	actualizar_layout();
}
	
void Widget_editor_propiedades_mapa::dibujar(DLibV::Pantalla& pantalla)
{
	using namespace DLibH;

	layout.volcar(pantalla);
}

void Widget_editor_propiedades_mapa::input(DFramework::Input& input, float delta)
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

void Widget_editor_propiedades_mapa::cambiar_por_indice(int indice, int val)
{
	switch(indice)
	{
		case 0: cambiar(val, id, "txt_id"); break;
		case 1: cambiar(val, id_fondo, "txt_id_fondo"); break;
		case 2: cambiar(val, minx, "txt_minx"); break;
		case 3: cambiar(val, maxx, "txt_maxx"); break;
		case 4: cambiar(val, miny, "txt_miny"); break;
		case 5: cambiar(val, maxy, "txt_maxy"); break;
	}
}

int Widget_editor_propiedades_mapa::valor_por_indice(int indice) const
{
	switch(indice)
	{
		case 0: return id; break;
		case 1: return id_fondo; break;
		case 2: return minx; break;
		case 3: return maxx; break;
		case 4: return miny; break;
		case 5: return maxy; break;
	}

	return 0;
}

bool Widget_editor_propiedades_mapa::es_cerrar() const
{
	return cerrar;
}

void Widget_editor_propiedades_mapa::cambiar_seleccion(int dir)
{
	indice_actual+=dir;
	if(indice_actual < min_indice) indice_actual=min_indice;
	else if(indice_actual > max_indice) indice_actual=max_indice;

	int y=layout.const_int("y_selector")+(indice_actual * layout.const_int("salto_selector"));
	layout.obtener_por_id("selector")->ir_a(layout.const_int("x_selector"), y);
}

void Widget_editor_propiedades_mapa::cambiar(int val, int& ref, const std::string& id)
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

void Widget_editor_propiedades_mapa::actualizar_layout()
{
	cambiar(id, id, "txt_id");
	cambiar(id_fondo, id_fondo, "txt_id_fondo");
	cambiar(minx, minx, "txt_minx");
	cambiar(maxx, maxx, "txt_maxx");
	cambiar(miny, miny, "txt_miny");
	cambiar(maxy, maxy, "txt_maxy");
}

void Widget_editor_propiedades_mapa::finalizar(DFramework::Input& input)
{
	input.vaciar_input_texto();
	input.finalizar_input_texto();

	ref.id=id;
	ref.id_fondo=id_fondo;
	ref.minx=minx;
	ref.maxx=maxx;
	ref.miny=miny;
	ref.maxy=maxy;
}

void Widget_editor_propiedades_mapa::inicializar(DFramework::Input& input)
{
	input.iniciar_input_texto();
}
