#ifndef GESTOR_AUDIO_H
#define GESTOR_AUDIO_H

#include <vector>
#include "audio_reproducir.h"
#include "audio_detener.h"

namespace App
{

class Gestor_audio
{
	///////////////////////
	// Interface pública.
	public:

	void					insertar(const Info_audio_reproducir&);
	void					insertar(const Audio_detener&);
	void					turno(float delta);

	///////////////////////
	//Privados:
	private:

	std::vector<Audio_detener>		detener;
	std::vector<Audio_reproducir>		reproducir;
};

}

#endif
