#ifndef DIRECTOR_ESTADOS
#define DIRECTOR_ESTADOS

#include <memory>

#include <herramientas/log_base/log_base.h>

#include "../framework/director_estados_interface.h"

#include "../app/framework_impl/app_config.h"
#include "../app/localizador.h"
#include "../app/sistema_audio.h"

#include "estados_controladores.h"
#include "principal.h"
#include "editor.h"
#include "intro.h"
#include "controles.h"
#include "ayuda_editor.h"


namespace App
{

class Director_estados:
	public DFramework::Director_estados_interface
{
	public:

							Director_estados(DFramework::Kernel& kernel, App::App_config& config, DLibH::Log_base&);
	
	virtual void					input_comun(DFramework::Input& input, float delta);
	virtual void					preparar_cambio_estado(int deseado, int actual);

	private:

	void						preparar_video(DFramework::Kernel& kernel);
	void						registrar_controladores(DFramework::Kernel& kernel);
	void						registrar_fuentes();
	void 						virtualizar_joysticks(DFramework::Input& input);


	App::App_config&				config;
	DLibH::Log_base&				log;

	Sistema_audio					sistema_audio;
	Fuentes						fuentes;
	Localizador 					localizador;
	
	std::unique_ptr<Controlador_principal>		controlador_principal;
	std::unique_ptr<Controlador_editor>		controlador_editor;
	std::unique_ptr<Controlador_intro>		controlador_intro;
	std::unique_ptr<Controlador_controles>		controlador_controles;
	std::unique_ptr<Controlador_ayuda_editor>	controlador_ayuda_editor;
};

}
#endif
