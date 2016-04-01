#ifndef MAPA_H
#define MAPA_H

#include <vector>

#include "decoracion.h"
#include "obstaculo.h"

namespace App
{

struct Mapa_info_camara
{
	int					min_cam_x, max_cam_x, min_cam_y, max_cam_y;
};

class Mapa
{
	public:

	std::vector<Obstaculo>			obstaculos;
	std::vector<Decoracion>			decoraciones;
	std::vector<const Decoracion *>		decoraciones_frente;
	std::vector<const Decoracion *>		decoraciones_fondo;


	int					acc_id() const {return id;}
	Mapa_info_camara			acc_info_camara() const {return info_camara;}

	void					establecer_info_camara(int, int, int, int);
	void					mut_id(int v) {id=v;}


	void					inicializar();
	void					limpiar();


	private:

	int					id;
	Mapa_info_camara			info_camara;


};

}

#endif
