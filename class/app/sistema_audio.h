#ifndef SISTEMA_AUDIO_H
#define SISTEMA_AUDIO_H

#include "../app/gestor_audio.h"

namespace App
{

class Sistema_audio
{
	public:

	void 					insertar(const Info_audio_reproducir& a) {v_reproducir.push_back(a);}
	void 					detener(const Audio_detener& a) {v_detener.push_back(a);}
	void					turno(float);

	private:

	Gestor_audio				gestor_audio;
	std::vector<Info_audio_reproducir>	v_reproducir;
	std::vector<Audio_detener>		v_detener;
};

}

#endif
