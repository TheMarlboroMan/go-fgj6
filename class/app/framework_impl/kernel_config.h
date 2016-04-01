#ifndef KERNEL_CONFIG_H
#define KERNEL_CONFIG_H

#include "app_config.h"
#include "../../framework/kernel_driver_interface.h"
#include <input/controles_sdl/controles_sdl.h>

namespace App
{

class Kernel_config
	:public DFramework::Kernel_driver_interface
{
	public:

	Kernel_config(const App_config&);

	virtual std::vector<std::string> 	obtener_entradas_audio() const;
	virtual std::vector<std::string> 	obtener_entradas_musica() const;
	virtual std::vector<std::string> 	obtener_entradas_texturas() const;
	virtual std::vector<std::string> 	obtener_entradas_superficies() const;

	//Información para ventana.
	virtual DFramework::Info_ventana	obtener_info_ventana() const;

	//Información del input.
	virtual std::vector<DFramework::Par_input> 	obtener_pares_input() const;

	private:

	const App_config&			config;
};

}

#endif
