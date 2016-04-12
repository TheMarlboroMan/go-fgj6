#ifndef AUTOMAPA_H
#define AUTOMAPA_H

#include <string>
#include <vector>

#include <class/dnot_parser.h>

namespace App
{

//Todos los valores de dimensiones no están en píxeles. sino en medidas de sala.
//Más o menos 1x equivale a un ancho de pantalla y 1y equivale a un alto.
//Las coordenadas están expresadas en formato de pantalla (w es hacia la derecha
//mientras que h es hacia abajo. 0,0 es la esquina superior izquierda.

//Estructura para representar una salida. 
struct Automapa_salida
{
	
	int		x, y;
	enum class torientaciones{nada, norte, oeste, sur, este} orientacion;

	static torientaciones		int_a_orientacion(int);
	static int			orientacion_a_int(torientaciones);
	static Automapa_salida		desde_token(const Herramientas_proyecto::Dnot_token&);
};

//Estructura para representar una sala.
struct Automapa_sala
{
	enum class tmarcadores{nada, arbol, metal, madera, agua, fuego, tierra, velocidad};
	int 				id, x, y, w, h;
	std::vector<Automapa_salida>	salidas;
	std::vector<tmarcadores>	marcadores;

	static tmarcadores		int_a_marcador(int);
	static int	 		marcador_a_int(tmarcadores);
	static Automapa_sala		desde_token(const Herramientas_proyecto::Dnot_token&);
};

//Clase de control de las salas. No tiene nada que ver con la representación:
//sólo contiene el mapa del mundo y se encarga de marcar las salas como 
//visitadas o no.

class Automapa
{
	public:

	void				cargar(const std::string&);
	void				visitar(int);
	void				reiniciar();
	size_t				size() const {return salas.size();}

	std::vector<Automapa_sala>	obtener_visitadas() const;

	private:

	struct sala{
		Automapa_sala 	s;
		bool		visitada;
//		sala(const Automapa_sala& s):s(s), visitada(false) {};
	};

	std::vector<sala>	salas;
};

}

#endif
