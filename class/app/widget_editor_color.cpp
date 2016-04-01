#include "widget_editor_color.h"

#include "framework_impl/input.h"

#include <def_video.h>

using namespace App;

Widget_editor_color::Widget_editor_color(const DLibV::Fuente_TTF& fuente, tcolor& f, tcolor& l)
	:color_fondo(f), color_linea(l), cerrar(false), indice_actual(min_indice), t_pulsado(0.0f)
{
	layout.mapear_fuente("akashi", fuente);
	layout.parsear("data/layout/widget_color.dnot", "layout");

	actualizar_layout();
}
	
void Widget_editor_color::dibujar(DLibV::Pantalla& pantalla)
{
	using namespace DLibH;

	layout.volcar(pantalla);	
}

void Widget_editor_color::input(DFramework::Input& input, float delta)
{
	if(input.es_input_down(Input::escape)) 
	{
		cerrar=true;
		return;
	}

	if(input.es_input_down(Input::cursor_abajo)) cambiar_seleccion(1);
	else if(input.es_input_down(Input::cursor_arriba)) cambiar_seleccion(-1);
	else if(input.es_input_pulsado(Input::cursor_izquierda)) cambiar_color(-1, delta);
	else if(input.es_input_pulsado(Input::cursor_derecha)) cambiar_color(1, delta);
	else t_pulsado=0.0f;

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

void Widget_editor_color::cambiar_por_indice(int indice, int val)
{
	switch(indice)
	{
		case 0: cambiar(val, color_fondo.r, "txt_r_frente"); break;
		case 1: cambiar(val, color_fondo.g, "txt_g_frente"); break;
		case 2: cambiar(val, color_fondo.b, "txt_b_frente"); break;
		case 3: cambiar(val, color_fondo.a, "txt_a_frente"); break;
		case 4: cambiar(val, color_linea.r, "txt_r_linea"); break;
		case 5: cambiar(val, color_linea.g, "txt_g_linea"); break;
		case 6: cambiar(val, color_linea.b, "txt_b_linea"); break;
		case 7: cambiar(val, color_linea.a, "txt_a_linea"); break;
	}
}

int Widget_editor_color::valor_por_indice(int indice) const
{
	switch(indice)
	{
		case 0: return color_fondo.r; break;
		case 1: return color_fondo.g; break;
		case 2: return color_fondo.b; break;
		case 3: return color_fondo.a; break;
		case 4: return color_linea.r; break;
		case 5: return color_linea.g; break;
		case 6: return color_linea.b; break;
		case 7: return color_linea.a; break;
	}

	return 0;
}

bool Widget_editor_color::es_cerrar() const
{
	return cerrar;
}

void Widget_editor_color::cambiar_seleccion(int dir)
{
	indice_actual+=dir;
	if(indice_actual < min_indice) indice_actual=min_indice;
	else if(indice_actual > max_indice) indice_actual=max_indice;

	int y=layout.const_int("y_selector")+(indice_actual * layout.const_int("salto_selector"));
	layout.obtener_por_id("selector")->ir_a(layout.const_int("x_selector"), y);
}

void Widget_editor_color::cambiar_color(int dir, float delta)
{
	if(t_pulsado==0.0f)
	{
		cambiar_por_indice(indice_actual, valor_por_indice(indice_actual)+dir);
		t_pulsado+=delta;
	}
	else
	{
		if(t_pulsado > 0.6f)
		{
			cambiar_por_indice(indice_actual, valor_por_indice(indice_actual)+dir);
		}
		else
		{
			t_pulsado+=delta;
		}
	}
}

void Widget_editor_color::cambiar(int val, int& ref, const std::string& id)
{
	if(val < 0) val=0;
	else if(val > 255) val=255;

	ref=val;

	std::string cad_final=std::to_string(val);
	static_cast<DLibV::Representacion_TTF *>(layout.obtener_por_id(id))->asignar(cad_final);

	actualizar_colores();
}

void Widget_editor_color::actualizar_layout()
{
	cambiar(color_fondo.r, color_fondo.r, "txt_r_frente");
	cambiar(color_fondo.g, color_fondo.g, "txt_g_frente");
	cambiar(color_fondo.b, color_fondo.b, "txt_b_frente");
	cambiar(color_fondo.a, color_fondo.a, "txt_a_frente");
	cambiar(color_linea.r, color_linea.r, "txt_r_linea");
	cambiar(color_linea.g, color_linea.g, "txt_g_linea");
	cambiar(color_linea.b, color_linea.b, "txt_b_linea");
	cambiar(color_linea.a, color_linea.a, "txt_a_linea");

	actualizar_colores();
}

void Widget_editor_color::actualizar_colores()
{
	DLibV::Representacion_primitiva_caja * fondo=static_cast<DLibV::Representacion_primitiva_caja *>(layout.obtener_por_id("caja_frente"));
	DLibV::Representacion_primitiva_caja * linea=static_cast<DLibV::Representacion_primitiva_caja *>(layout.obtener_por_id("caja_linea"));

	auto f=[](DLibV::Representacion_primitiva_caja& rep, tcolor col)
	{
		rep.mut_rgb(col.r, col.g, col.b);
		rep.establecer_alpha(col.a);
	};

	f(*fondo, color_fondo);
	f(*linea, color_linea);
}

void Widget_editor_color::finalizar(DFramework::Input& input)
{
	input.vaciar_input_texto();
	input.finalizar_input_texto();
}

void Widget_editor_color::inicializar(DFramework::Input& input)
{
	input.iniciar_input_texto();
}
