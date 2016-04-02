#ifndef DIRECTOR_ESTADOS
#define DIRECTOR_ESTADOS

#include <memory>

#include <herramientas/log_base/log_base.h>

#include "../framework/director_estados_interface.h"

#include "../app/framework_impl/app_config.h"
#include "../app/localizador.h"

#include "estados_controladores.h"
#include "principal.h"
#include "editor.h"
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
	void						registrar_controladores();
	void						registrar_fuentes();
	void 						virtualizar_joysticks(DFramework::Input& input);


	App::App_config&				config;
	DLibH::Log_base&				log;

	App::Fuentes					fuentes;
	Localizador 					localizador;
	
	std::unique_ptr<Controlador_principal>		controlador_principal;
	std::unique_ptr<Controlador_editor>		controlador_editor;
	std::unique_ptr<Controlador_ayuda_editor>	controlador_ayuda_editor;
};

}
#endif
