#include "widget_editor_obstaculo.h"

#include "framework_impl/input.h"

#include <def_video.h>

using namespace App;

Widget_editor_obstaculo::Widget_editor_obstaculo(const DLibV::Fuente_TTF& fuente, Obstaculo& o)
	:elemento(o), tipo(elemento.acc_tipo()),
	cerrar{false}, indice_actual{min_indice}, t_pulsado{0.0f}
{
	layout.mapear_fuente("akashi", fuente);
	layout.parsear("data/layout/widget_obstaculo.dnot", "layout");

	actualizar_layout();
}
	
void Widget_editor_obstaculo::dibujar(DLibV::Pantalla& pantalla)
{
	using namespace DLibH;

	layout.volcar(pantalla);	
}

void Widget_editor_obstaculo::input(DFramework::Input& input, float delta)
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
}

bool Widget_editor_obstaculo::es_cerrar() const
{
	return cerrar;
}

void Widget_editor_obstaculo::cambiar_seleccion(int dir)
{
	indice_actual+=dir;
	if(indice_actual < min_indice) indice_actual=min_indice;
	else if(indice_actual > max_indice) indice_actual=max_indice;

	int y=layout.const_int("y_selector")+(indice_actual * layout.const_int("salto_selector"));
	layout.obtener_por_id("selector")->ir_a(layout.const_int("x_selector"), y);
}

void Widget_editor_obstaculo::cambiar_valor(int dir, float delta)
{
	auto f=[this](int dir, int indice)
	{
		switch(indice)
		{
			case 0: cambiar_tipo(); break;
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

void Widget_editor_obstaculo::cambiar_tipo()
{
	switch(tipo)
	{
		case Obstaculo::ttipos::normal:	tipo=Obstaculo::ttipos::inocuo; break;
		case Obstaculo::ttipos::inocuo:	tipo=Obstaculo::ttipos::normal; break;
	}
}

void Widget_editor_obstaculo::actualizar_layout()
{
	auto actualizar_tipo=[this](const std::string& id, Obstaculo::ttipos tipo)
	{
		std::string cad_final="";

		switch(tipo)
		{
			case Obstaculo::ttipos::normal:	cad_final="Normal"; break;
			case Obstaculo::ttipos::inocuo:	cad_final="Inocuo"; break;
		}

		static_cast<DLibV::Representacion_TTF *>(layout.obtener_por_id(id))->asignar(cad_final);
	};

	actualizar_tipo("txt_tipo", tipo);
}

void Widget_editor_obstaculo::finalizar(DFramework::Input&)
{
	elemento.mut_tipo(tipo);
}

void Widget_editor_obstaculo::inicializar(DFramework::Input&)
{

}


