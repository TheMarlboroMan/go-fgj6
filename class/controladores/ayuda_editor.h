#ifndef CONTROLADOR_AYUDA_EDITOR_H
#define CONTROLADOR_AYUDA_EDITOR_H

#include <def_video.h>
#include <class/compositor_vista.h>

#include "estados_controladores.h"
#include "../framework/controlador_interface.h"
#include "../app/fuentes.h"


namespace App
{

class Controlador_ayuda_editor:
	public DFramework::Controlador_interface
{
	public:
	
					Controlador_ayuda_editor(DLibH::Log_base&, const Fuentes&);
	virtual void 			preloop(DFramework::Input& input, float delta);
	virtual void 			loop(DFramework::Input& input, float delta);
	virtual void 			postloop(DFramework::Input& input, float delta);
	virtual void 			dibujar(DLibV::Pantalla& pantalla);
	virtual void 			despertar();
	virtual void 			dormir();
	virtual bool			es_posible_abandonar_estado() const;

	private:

	DLibH::Log_base&			log;

	double					y, min_y;

	Herramientas_proyecto::Compositor_vista		layout;
};

}
#endif
