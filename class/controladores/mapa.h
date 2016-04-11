#ifndef CONTROLADOR_MAPA_H
#define CONTROLADOR_MAPA_H

#include <class/compositor_vista.h>

#include "estados_controladores.h"
#include "../framework/controlador_interface.h"

#include "../app/sistema_audio.h"
#include "../app/automapa.h"

namespace App
{

class Controlador_mapa:
	public DFramework::Controlador_interface
{
	public:
	
					Controlador_mapa(DLibH::Log_base&, Sistema_audio&);
	virtual void 			preloop(DFramework::Input& input, float delta);
	virtual void 			loop(DFramework::Input& input, float delta);
	virtual void 			postloop(DFramework::Input& input, float delta);
	virtual void 			dibujar(DLibV::Pantalla& pantalla);
	virtual void 			despertar();
	virtual void 			dormir();
	virtual bool			es_posible_abandonar_estado() const;

	void				reiniciar();
	void				descubrir_salas(const std::vector<int>&);

	private:

	DLibH::Log_base&			log;
	Sistema_audio&				sistema_audio;

	Automapa				automapa;
};

}
#endif
