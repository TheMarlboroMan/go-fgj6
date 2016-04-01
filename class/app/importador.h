#ifndef IMPORTADOR_H
#define IMPORTADOR_H

#include <class/dnot_parser.h>
#include "obstaculo.h"
#include "decoracion.h"

namespace App
{
class Importador
{
	public:

	void 		importar(const std::string&, std::vector<Obstaculo>&, std::vector<Decoracion>&);

	private:

	void 		deserializar_obstaculo(const Herramientas_proyecto::Dnot_token&, std::vector<Obstaculo>&);
	void 		deserializar_decoracion(const Herramientas_proyecto::Dnot_token&, std::vector<Decoracion>&);
};
}

#endif
