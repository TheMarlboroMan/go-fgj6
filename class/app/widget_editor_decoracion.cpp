#include "widget_editor_decoracion.h"

#include "framework_impl/input.h"

#include <def_video.h>

using namespace App;

Widget_editor_decoracion::Widget_editor_decoracion(const DLibV::Fuente_TTF& fuente, Decoracion& d)
	:elemento(d), color_frente(d.acc_color()), color_linea(d.acc_color_linea()),
	profundidad(d.acc_profundidad()), frente(d.es_frente()),
	cerrar{false}, indice_actual{min_indice}, t_pulsado{0.0f}
{
	layout.mapear_fuente("akashi", fuente);
	layout.parsear("data/layout/widget_decoracion.dnot", "layout");

	actualizar_layout();
}
	
void Widget_editor_decoracion::dibujar(DLibV::Pantalla& pantalla)
{
	using namespace DLibH;

	layout.volcar(pantalla);	
}

void Widget_editor_decoracion::input(DFramework::Input& input, float delta)
{
	if(input.es_input_down(Input::escape)) 
	{
		cerrar=true;
		return;
	}

	if(input.es_input_down(Input::cursor_abajo)) cambiar_seleccion(1);
	else if(input.es_input_down(Input::cursor_arriba)) cambiar_seleccion(-1);
	else if(input.es_input_pulsado(Input::cursor_izquierda)) cambiar_valor(-1, delta);
	else if(input.es_input_pulsado(Input::cursor_derecha)) cambiar_valor(1, delta);
	else t_pulsado=0.0f;

	if(input.es_eventos_input_texto ())
	{
		static_cast<DLibV::Representacion_TTF *>(layout.obtener_por_id("txt_input"))->asignar(input.acc_input_texto());
	}

	if(input.es_input_down(Input::enter) && input.acc_input_texto().size() && es_indice_texto(indice_actual))
	{


		int val=0;

		try{val=std::stoi(input.acc_input_texto());}catch(std::exception& e){}

		cambiar_numero(val, min_max_por_indice(indice_actual), referencia_por_indice(indice_actual));
		actualizar_layout();
		input.vaciar_input_texto();
		static_cast<DLibV::Representacion_TTF *>(layout.obtener_por_id("txt_input"))->asignar("");
	}
}

bool Widget_editor_decoracion::es_cerrar() const
{
	return cerrar;
}

void Widget_editor_decoracion::cambiar_seleccion(int dir)
{
	indice_actual+=dir;
	if(indice_actual < min_indice) indice_actual=min_indice;
	else if(indice_actual > max_indice) indice_actual=max_indice;

	int y=layout.const_int("y_selector")+(indice_actual * layout.const_int("salto_selector"));
	layout.obtener_por_id("selector")->ir_a(layout.const_int("x_selector"), y);
}

Widget_editor_decoracion::min_max Widget_editor_decoracion::min_max_por_indice(int indice) const
{
	switch(indice)
	{
		case 8: return {min_profundidad, max_profundidad}; break;
		default: return {min_color, max_color}; break;
	}
	
	return {0,0};
}

void Widget_editor_decoracion::cambiar_valor(int dir, float delta)
{
	auto f=[this](int dir, int indice)
	{		
		switch(indice)
		{
			case 9: frente=!frente; break;
			default: cambiar_numero(referencia_por_indice(indice)+dir, min_max_por_indice(indice), referencia_por_indice(indice)); break;
		}
	};

	if(t_pulsado==0.0f)
	{
		f(dir, indice_actual);
		t_pulsado+=delta;
	}
	else
	{
		if(t_pulsado > 0.6f)
		{
			f(dir, indice_actual);
		}
		else
		{
			t_pulsado+=delta;
		}
	}

	actualizar_layout();
}

int& Widget_editor_decoracion::referencia_por_indice(int indice)
{
	switch(indice)
	{
		case 0: return color_frente.r; break;
		case 1: return color_frente.g; break;
		case 2: return color_frente.b; break;
		case 3: return color_frente.a; break;
		case 4: return color_linea.r; break;
		case 5: return color_linea.g; break;
		case 6: return color_linea.b; break;
		case 7: return color_linea.a; break;
		case 8: return profundidad; break;
	}

	return color_frente.r;
}

void Widget_editor_decoracion::cambiar_numero(int val, min_max mm, int& ref)
{
	if(val < mm.vmin) val=mm.vmin;
	else if(val > mm.vmax) val=mm.vmax;
	ref=val;
}

void Widget_editor_decoracion::actualizar_layout()
{
	auto actualizar_numero=[this](const std::string& id, int val)
	{
		std::string cad_final=std::to_string(val);
		static_cast<DLibV::Representacion_TTF *>(layout.obtener_por_id(id))->asignar(cad_final);
	};

	auto actualizar_bool=[this](const std::string& id, bool val, const std::string& cad_true, const std::string& cad_false)
	{
		static_cast<DLibV::Representacion_TTF *>(layout.obtener_por_id(id))->asignar(val ? cad_true : cad_false);
	};

	auto f_color=[](DLibV::Representacion_primitiva_caja& rep, tcolor col)
	{
		rep.mut_rgb(col.r, col.g, col.b);
		rep.establecer_alpha(col.a);
	};

	actualizar_numero("txt_r_frente", color_frente.r);
	actualizar_numero("txt_g_frente", color_frente.g);
	actualizar_numero("txt_b_frente", color_frente.b);
	actualizar_numero("txt_a_frente", color_frente.a);
	actualizar_numero("txt_r_linea", color_linea.r);
	actualizar_numero("txt_g_linea", color_linea.g);
	actualizar_numero("txt_b_linea", color_linea.b);
	actualizar_numero("txt_a_linea", color_linea.a);
	actualizar_numero("txt_profundidad", profundidad);
	actualizar_bool("txt_frente_fondo", frente, "Frente", "Fondo");

	DLibV::Representacion_primitiva_caja * fondo=static_cast<DLibV::Representacion_primitiva_caja *>(layout.obtener_por_id("caja_frente"));
	DLibV::Representacion_primitiva_caja * linea=static_cast<DLibV::Representacion_primitiva_caja *>(layout.obtener_por_id("caja_linea"));
	f_color(*fondo, color_frente);
	f_color(*linea, color_linea);
}

void Widget_editor_decoracion::finalizar(DFramework::Input& input)
{
	elemento.mut_profundidad(profundidad);
	elemento.mut_color_linea(color_linea);
	elemento.mut_color(color_frente);
	elemento.mut_frente(frente);

	input.vaciar_input_texto();
	input.finalizar_input_texto();
}

void Widget_editor_decoracion::inicializar(DFramework::Input& input)
{
	input.iniciar_input_texto();
}

bool Widget_editor_decoracion::es_indice_texto(int indice) const
{
	return indice!=9;
}
