#include "sistema_audio.h"

using namespace App;

void Sistema_audio::turno(float delta)
{
	for(auto& r : v_reproducir) gestor_audio.insertar(r);
	for(auto& r : v_detener) gestor_audio.insertar(r);

	v_reproducir.clear();
	v_detener.clear();

	gestor_audio.turno(delta);
}
