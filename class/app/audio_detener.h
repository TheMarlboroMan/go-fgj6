#ifndef AUDIO_DETENER_H
#define AUDIO_DETENER_H

namespace App
{

class Audio_detener
{
	//////////////////////
	//Interface pública...
	public:
					Audio_detener(int);
	int				acc_id_destino() const {return id_destino;}
	
	//////////////////////
	//Privadas...
	private:

	int 				id_destino;
};

}

#endif
