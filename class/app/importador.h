#ifndef IMPORTADOR_H
#define IMPORTADOR_H

#include <class/dnot_parser.h>
#include "mapa.h"

namespace App
{
class Importador
{
	public:

	void 		importar(const std::string&, Mapa&);

	private:

	void 		deserializar(const Herramientas_proyecto::Dnot_token&, std::vector<Obstaculo>&);
	void 		deserializar(const Herramientas_proyecto::Dnot_token&, std::vector<Decoracion>&);
	void 		deserializar(const Herramientas_proyecto::Dnot_token&, std::vector<Inicio>&);
	void 		deserializar(const Herramientas_proyecto::Dnot_token&, std::vector<Salida>&);
	void 		deserializar(const Herramientas_proyecto::Dnot_token&, std::vector<Pieza>&);
	void 		deserializar(const Herramientas_proyecto::Dnot_token&, std::vector<Interruptor>&);
	void 		deserializar(const Herramientas_proyecto::Dnot_token&, std::vector<Puerta>&);
	void 		deserializar(const Herramientas_proyecto::Dnot_token&, std::vector<Mejora_velocidad>&);
	void 		deserializar(const Herramientas_proyecto::Dnot_token&, std::vector<Arbol>&);
};
}

#endif
