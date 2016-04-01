#ifndef BLOQUE_INPUT_H
#define BLOQUE_INPUT_H

namespace App
{

struct Bloque_input
{
	int			giro,
				aceleracion;
	bool			disparo,
				activar_habilidad,
				habilidad_velocidad,
				habilidad_escudo;
};

struct Traduccion_input
{
	int			arriba,
				abajo,
				izquierda,
				derecha,
				disparo,
				habilidad;
};

}

#endif
