#include "director_estados.h"

#include <algorithm>

using namespace App;

extern DLibH::Log_base LOG;

Director_estados::Director_estados(DFramework::Kernel& kernel, App::App_config& c, DLibH::Log_base& log)
//	:Director_estados_interface(t_estados::intro, std::function<bool(int)>([](int v){return v > estado_min && v < estado_max;})),
	:Director_estados_interface(t_estados::controles, std::function<bool(int)>([](int v){return v > estado_min && v < estado_max;})),
	config(c), log(log), localizador("data/localizacion/loc")
{
	preparar_video(kernel);
	registrar_fuentes();
	registrar_controladores(kernel);
	virtualizar_joysticks(kernel.acc_input());
	localizador.inicializar(0);

//	int indice=kernel.acc_controlador_argumentos().buscar("file");
//	if(indice!=-1)
//	{
//		estados.validar_y_cambiar_estado(editor);
//		controlador_editor->iniciar_edicion_fichero(kernel.acc_controlador_argumentos().acc_argumento(indice+1));
//	}
//	else
//	{
//		estados.validar_y_cambiar_estado(intro);
//	}

	DFramework::Audio::reproducir_musica(DLibA::Gestor_recursos_audio::obtener_musica(1));
}

void Director_estados::preparar_video(DFramework::Kernel& kernel)
{
	auto& pantalla=kernel.acc_pantalla();

	int wf=config.acc_w_fisica_pantalla(), 
		hf=config.acc_h_fisica_pantalla(),
		wl=config.acc_w_logica_pantalla(),
		hl=config.acc_h_logica_pantalla();

	pantalla.inicializar(wf, hf);
	pantalla.establecer_medidas_logicas(wl, hl);
	pantalla.establecer_modo_ventana(config.acc_modo_pantalla());
}

void Director_estados::registrar_controladores(DFramework::Kernel& kernel)
{
	controlador_principal.reset(new Controlador_principal(log, fuentes, localizador, sistema_audio));
	controlador_editor.reset(new Controlador_editor(log, fuentes));
	controlador_ayuda_editor.reset(new Controlador_ayuda_editor(log, fuentes));
	controlador_intro.reset(new Controlador_intro(log, fuentes, localizador, sistema_audio));
	controlador_controles.reset(new Controlador_controles(log, config, fuentes, localizador, kernel.acc_input()));

	registrar_controlador(t_estados::principal, *controlador_principal);
	registrar_controlador(t_estados::editor, *controlador_editor);
	registrar_controlador(t_estados::ayuda_editor, *controlador_ayuda_editor);
	registrar_controlador(t_estados::intro, *controlador_intro);
	registrar_controlador(t_estados::controles, *controlador_controles);
}

void Director_estados::preparar_cambio_estado(int deseado, int actual)
{
	switch(deseado)
	{
		case t_estados::principal: 
			if(actual==t_estados::editor)
			{
				auto im=controlador_editor->acc_info_mapa();
				controlador_principal->iniciar_nivel(im.id, 1);
			}
			else if(actual==t_estados::intro)
			{
				controlador_principal->iniciar_juego();
			}
		break;
		case t_estados::editor: break;
		case t_estados::ayuda_editor: break;
		case t_estados::intro: 
			if(actual==t_estados::principal)
			{
				if(controlador_principal->es_juego_finalizado()) controlador_intro->establecer_finalizado();
			}
		break;


		case t_estados::game_over: break;
	}
}

void Director_estados::input_comun(DFramework::Input& input, float delta)
{
	sistema_audio.turno(delta);	//No es input, pero bueno...

	if(input.es_nuevo_joystick_conectado())
	{
		log<<"Detectado nuevo joystick..."<<std::endl;
		virtualizar_joysticks(input);
	}
}

void Director_estados::virtualizar_joysticks(DFramework::Input& input)
{
	for(int i=0; i < input.obtener_cantidad_joysticks(); ++i)
	{
		input.virtualizar_hats_joystick(i);
		input.virtualizar_ejes_joystick(i,15000);
		log<<"Virtualizado joystick "<<i<<std::endl;
	}
}

void Director_estados::registrar_fuentes()
{
	fuentes.registrar_fuente("akashi", 16);
	fuentes.registrar_fuente("akashi", 20);
	fuentes.registrar_fuente("akashi", 9);
	fuentes.registrar_fuente("imagination_station", 16);
	fuentes.registrar_fuente("inkburrow", 26);
	fuentes.registrar_fuente("bulldozer", 20);
}
