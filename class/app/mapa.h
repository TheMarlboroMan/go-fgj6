#ifndef MAPA_H
#define MAPA_H

#include <vector>

#include "decoracion.h"
#include "obstaculo.h"
#include "inicio.h"
#include "salida.h"
#include "pieza.h"
#include "interruptor.h"
#include "puerta.h"
#include "mejora_velocidad.h"

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
	std::vector<Inicio>			inicios;
	std::vector<Salida>			salidas;
	std::vector<Pieza>			piezas;
	std::vector<Interruptor>		interruptores;
	std::vector<Puerta>			puertas;
	std::vector<Mejora_velocidad>		mejoras_velocidad;
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
