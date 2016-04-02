#include "director_estados.h"

#include <algorithm>

using namespace App;

extern DLibH::Log_base LOG;

Director_estados::Director_estados(DFramework::Kernel& kernel, App::App_config& c, DLibH::Log_base& log)
	:Director_estados_interface(t_estados::editor, std::function<bool(int)>([](int v){return v > estado_min && v < estado_max;})),
	config(c), log(log)
{
	preparar_video(kernel);
	registrar_fuentes();
	registrar_controladores();
	virtualizar_joysticks(kernel.acc_input());

	int indice=kernel.acc_controlador_argumentos().buscar("file");
	if(indice!=-1)
	{
		estados.validar_y_cambiar_estado(editor);
		controlador_editor->iniciar_edicion_fichero(kernel.acc_controlador_argumentos().acc_argumento(indice+1));
	}
	else
	{
		//TODO: Have main controller precache all info...
		controlador_principal->iniciar_juego();
	}
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

void Director_estados::registrar_controladores()
{
	controlador_principal.reset(new Controlador_principal(log, fuentes));
	controlador_editor.reset(new Controlador_editor(log, fuentes));
	controlador_ayuda_editor.reset(new Controlador_ayuda_editor(log, fuentes));

	registrar_controlador(t_estados::principal, *controlador_principal);
	registrar_controlador(t_estados::editor, *controlador_editor);
	registrar_controlador(t_estados::ayuda_editor, *controlador_ayuda_editor);
}

void Director_estados::preparar_cambio_estado(int deseado, int actual)
{
	switch(deseado)
	{
		case t_estados::principal: 
			controlador_editor->aplicar_a_mapa(controlador_principal->acc_mapa());
		break;
		case t_estados::editor: break;
		case t_estados::ayuda_editor: break;
	}
}

void Director_estados::input_comun(DFramework::Input& input, float delta)
{
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
	fuentes.registrar_fuente("akashi", 9);
}
