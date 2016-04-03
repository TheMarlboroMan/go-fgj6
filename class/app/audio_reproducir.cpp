#include "audio_reproducir.h"

using namespace App;
using namespace DLibA;

int Info_audio_reproducir::id_siguiente=1;

Info_audio_reproducir::Info_audio_reproducir(t_reproduccion tr, t_sonido ts, int id_snd, int vol, int ppan)
	:id(id_siguiente++), 
	tipo_reproduccion(tr), 
	tipo_sonido(ts),
	id_sonido(id_snd), 
	volumen(vol), 
	pan(ppan)
{

}

/***********/

Audio_reproducir::Audio_reproducir(const Info_audio_reproducir& i)
	:info(i),
	estado(estados::sin_iniciar)
{

}

/**
* Atención, cuando metamos esto dentro de un vector se va a copiar y a destruir.
* El canal lo vamos a duplicar pero nos interesará NO detener el sonido porque
* aún puede tener que estar sonando en la nueva copia.
*/

Audio_reproducir::~Audio_reproducir()
{
	if(canal.es_vinculado())
	{
		canal.desactivar_monitoreado();
//		canal.forzar_parada();
		canal.desvincular();
		estado=estados::desvinculado;
	}
}

void Audio_reproducir::detener_sonido()
{
	canal.forzar_parada();
	estado=estados::finalizado;
}

void Audio_reproducir::turno(float p_delta)
{
	//TODO TODO TODO: Subclasear.

	switch(info.tipo_reproduccion)
	{
		case Info_audio_reproducir::t_reproduccion::simple:
		case Info_audio_reproducir::t_reproduccion::repetido:
			if(estado==estados::sin_iniciar)
			{
				iniciar_reproduccion();
			}
			else if(!canal.es_reproduciendo())
			{
				estado=estados::finalizado;
			}
		break;
	}
}

void Audio_reproducir::iniciar_reproduccion()
{
	if(!canal.es_vinculado()) 
	{
		try
		{
			canal=DFramework::Audio::obtener_canal_libre();
			canal.activar_monitoreado();
		}
		catch(const DFramework::Excepcion_audio& e)
		{
			LOG<<e.mensaje<<std::endl;
			return;
		}
		catch(const DLibA::Excepcion_controlador_audio& e)
		{
			LOG<<"WARNING: Imposible obtener canal de audio para Audio_reproducir."<<std::endl;
			return;
		}
	}

	int repeticiones=0;

	//TODO: Subclass this bitch.
	switch(info.tipo_reproduccion)
	{
		case Info_audio_reproducir::t_reproduccion::simple:	repeticiones=0; break;
		case Info_audio_reproducir::t_reproduccion::repetido: 	repeticiones=-1; break;
	}
	
	int pan_d=(info.pan / 2);
	int pan_i=255-pan_d;

	canal.iniciar_reproduccion(
		DLibA::Estructura_sonido(
				DLibA::Gestor_recursos_audio::obtener_sonido(info.id_sonido), 
			info.volumen, repeticiones, pan_i, pan_d));

	estado=estados::reproduciendo;
}
