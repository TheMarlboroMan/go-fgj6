#ifndef AUDIO_REPRODUCIR_H
#define AUDIO_REPRODUCIR_H

#include "../framework/audio.h"


/**
* Estructura de control de audio que indica audio que puede reproducirse o
* que está en reproduccion.
*/

namespace App
{

class Gestor_audio;

struct Info_audio_reproducir
{
	private:
	static int				id_siguiente;

	public:

	enum class t_reproduccion {
		simple, 	//Se repite una vez.
		repetido	//Se repite en loop.
	};
	enum class t_sonido {
		unico,		//No se permite que suene otro si ya hay uno en un canal.
		repetible	//Se permite la repetición entre canales.
	};

						Info_audio_reproducir(t_reproduccion, t_sonido, int id_snd, int vol, int ppan);


	int 					id;
	t_reproduccion 				tipo_reproduccion;
	t_sonido 				tipo_sonido;
	int	 				id_sonido;
	int 					volumen;
	int 					pan;
};

class Audio_reproducir
{
	//////////////////////
	//Interface pública.
	public:

	enum class estados {sin_iniciar, reproduciendo, finalizado, desvinculado};

						Audio_reproducir(const Info_audio_reproducir&);
						~Audio_reproducir();

	void 					turno(float p_delta);
	void					detener_sonido();
	int					acc_id() const {return info.id;}
	bool					es_finalizado() const {return estado==estados::finalizado;}
	bool					es_desvinculado() const {return estado==estados::desvinculado;}

	//////////////////////
	//Privado.
	private:

	void 					iniciar_reproduccion();
	DLibA::Canal_audio 			canal;
	Info_audio_reproducir			info;
	estados 				estado;

	friend class Gestor_audio;

};

}

#endif
