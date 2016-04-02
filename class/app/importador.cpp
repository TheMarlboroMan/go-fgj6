#include "importador.h"

using namespace App;

void Importador::importar(const std::string& fichero, Mapa& mapa)
{
	using namespace Herramientas_proyecto;

	auto raiz=parsear_dnot(fichero);

	try
	{
		mapa.mut_id(raiz["propiedades"]["id"]);
		mapa.mut_id_fondo(raiz["propiedades"]["idf"]);
		mapa.establecer_info_camara(raiz["propiedades"]["cam"][0], raiz["propiedades"]["cam"][1], raiz["propiedades"]["cam"][2], raiz["propiedades"]["cam"][3]);

		for(const auto& i : raiz["geometria"].acc_lista()) deserializar(i, mapa.obstaculos);
		for(const auto& i : raiz["decoracion"].acc_lista()) deserializar(i, mapa.decoraciones);
		for(const auto& i : raiz["inicios"].acc_lista()) deserializar(i, mapa.inicios);
		for(const auto& i : raiz["salidas"].acc_lista()) deserializar(i, mapa.salidas);
		for(const auto& i : raiz["piezas"].acc_lista()) deserializar(i, mapa.piezas);
		for(const auto& i : raiz["interruptores"].acc_lista()) deserializar(i, mapa.interruptores);
		for(const auto& i : raiz["puertas"].acc_lista()) deserializar(i, mapa.puertas);
		for(const auto& i : raiz["mejoras_velocidad"].acc_lista()) deserializar(i, mapa.mejoras_velocidad);
	}
	catch(std::exception &e)
	{
		//Simplemente por compatibilidad...
	}
}

void Importador::deserializar(const Herramientas_proyecto::Dnot_token& tok, std::vector<Obstaculo>& obstaculos)
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

void Importador::deserializar(const Herramientas_proyecto::Dnot_token& tok, std::vector<Decoracion>& decoraciones)
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

void Importador::deserializar(const Herramientas_proyecto::Dnot_token& tok, std::vector<Inicio>& inicios)
{
	Espaciable::tpunto centro{0.0, 0.0};
	int id=0, angulo=0;

	const auto& c=tok["pos"].acc_lista();
	centro.x=c[0], centro.y=c[1];
	id=tok["id"].acc_int();
	angulo=tok["angulo"].acc_int();

	inicios.push_back({centro, id, angulo});
}

void Importador::deserializar(const Herramientas_proyecto::Dnot_token& tok, std::vector<Salida>& salidas)
{
	Espaciable::tpoligono poligono;

	const auto& lista_puntos=tok["p"].acc_lista();
	for(const auto& vp : lista_puntos)
	{
		const auto& pt=vp.acc_lista();
		poligono.insertar_vertice({pt[0], pt[1]});
	}
	
	int id_mapa=tok["idm"];
	int id_inicio=tok["idi"];

	salidas.push_back({poligono, id_mapa, id_inicio});
}

void Importador::deserializar(const Herramientas_proyecto::Dnot_token& tok, std::vector<Pieza>& piezas)
{
	Espaciable::tpunto centro{0.0, 0.0};
	const auto& c=tok["pos"].acc_lista();
	centro.x=c[0], centro.y=c[1];

	int indice=tok["i"].acc_int();
	piezas.push_back({centro, indice});
}

void Importador::deserializar(const Herramientas_proyecto::Dnot_token& tok, std::vector<Interruptor>& interruptores)
{
	Espaciable::tpunto centro{0.0, 0.0};
	const auto& c=tok["pos"].acc_lista();
	centro.x=c[0], centro.y=c[1];
	int nivel=tok["n"].acc_int();
	int idp=tok["idp"].acc_int();
	int idg=tok["idg"].acc_int();
	int tgr=tok["tgr"].acc_int();

	interruptores.push_back({centro, nivel, idp, idg, tgr});
}

void Importador::deserializar(const Herramientas_proyecto::Dnot_token& tok, std::vector<Puerta>& puertas)
{
	Espaciable::tpoligono poligono;

	const auto& lista_puntos=tok["p"].acc_lista();
	for(const auto& vp : lista_puntos)
	{
		const auto& pt=vp.acc_lista();
		poligono.insertar_vertice({pt[0], pt[1]});
	}
	
	int id=tok["id"];

	puertas.push_back({poligono, id});
}

void Importador::deserializar(const Herramientas_proyecto::Dnot_token& tok, std::vector<Mejora_velocidad>& mejoras_velocidad)
{
	Espaciable::tpunto centro{0.0, 0.0};
	const auto& c=tok["pos"].acc_lista();
	centro.x=c[0], centro.y=c[1];
	int nivel=tok["n"].acc_int();

	mejoras_velocidad.push_back({centro, nivel});
}
