#include "importador.h"

using namespace App;

void Importador::importar(const std::string& fichero, 
	std::vector<Obstaculo>& obstaculos,
	std::vector<Decoracion>& decoraciones)
{
	using namespace Herramientas_proyecto;

	auto raiz=parsear_dnot(fichero);

	try
	{
		auto& array_geometria=raiz["geometria"].acc_lista();
		for(const auto& i : array_geometria) deserializar_obstaculo(i, obstaculos);

		auto& array_decoraciones=raiz["decoracion"].acc_lista();
		for(const auto& i : array_decoraciones) deserializar_decoracion(i, decoraciones);
	}
	catch(std::exception &e)
	{
		//Simplemente por compatibilidad...
	}
}

void Importador::deserializar_obstaculo(const Herramientas_proyecto::Dnot_token& tok, std::vector<Obstaculo>& obstaculos)
{
	Espaciable::tpoligono poligono;
	Obstaculo::ttipos tipo=Obstaculo::ttipos::normal;

	try
	{
		const auto& lista_puntos=tok["p"].acc_lista();
		for(const auto& vp : lista_puntos)
		{
			const auto& pt=vp.acc_lista();
			poligono.insertar_vertice({pt[0], pt[1]});
		}
	
		const auto& centro=tok["cen"].acc_lista();
		poligono.mut_centro({centro[0], centro[1]});

		int val_tipo=tok["pr"]["t"];
		switch(val_tipo)
		{
			case 0:
			default:
				tipo=Obstaculo::ttipos::normal;
			break;
			case 1: 
				tipo=Obstaculo::ttipos::inocuo;
			break;
		}
	}
	catch(std::exception& e)
	{
		//Simplemente por compatibilidad...
	}

	obstaculos.push_back({poligono, tipo});
}

void Importador::deserializar_decoracion(const Herramientas_proyecto::Dnot_token& tok, std::vector<Decoracion>& decoraciones)
{
	Espaciable::tpoligono poligono;

	const auto& lista_puntos=tok["p"].acc_lista();
	for(const auto& vp : lista_puntos)
	{
		const auto& pt=vp.acc_lista();
		poligono.insertar_vertice({pt[0], pt[1]});
	}
	
	const auto& centro=tok["cen"].acc_lista();
	poligono.mut_centro({centro[0], centro[1]});

	tcolor color={128, 128, 128, 255};
	tcolor clinea=color;
	bool frente=false;
	int profundidad=100;

	try
	{
		const auto& colores=tok["col"].acc_lista();
		const auto& vcolor=colores[0].acc_lista();
		const auto& vcolorl=colores[1].acc_lista();

		color={vcolor[0], vcolor[1], vcolor[2], vcolor[3]};
		clinea={vcolorl[0], vcolorl[1], vcolorl[2], vcolorl[3]};
		frente=tok["pr"]["fr"];
		profundidad=tok["pr"]["pr"];
	}
	catch(std::exception& e)
	{
		//No hacer nada... Tenemos esto aquí sólo para que no explote
		//seǵun vamos guardando más propiedades en el objeto.
	}

	decoraciones.push_back({poligono, color, clinea, frente, profundidad});
}
