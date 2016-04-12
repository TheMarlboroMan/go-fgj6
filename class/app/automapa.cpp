#include "automapa.h"

#include <algorithm>

/**
La estructura del fichero a leer es algo como:

mapa: {salas : [{id: 1, x: 0, y:0, w:2, h:1, 
		salidas: [{x:0, y:0, ori: 1}, {x:0, y:0, ori: 2}],
		marker:[1,2]}
		...]}
*/

using namespace App;

Automapa_salida::torientaciones Automapa_salida::int_a_orientacion(int val)
{
	switch(val)
	{
		case 0: return torientaciones::nada; break; 
		case 1: return torientaciones::norte; break; 
		case 2: return torientaciones::este; break;
		case 3: return torientaciones::sur; break;
		case 4: return torientaciones::oeste; break;
	}

	return torientaciones::nada;
}

int Automapa_salida::orientacion_a_int(Automapa_salida::torientaciones val)
{
	switch(val)
	{
		case torientaciones::nada:	return 0; break; 
		case torientaciones::norte:	return 1; break; 
		case torientaciones::este: 	return 2; break;
		case torientaciones::sur: 	return 3; break;
		case torientaciones::oeste: 	return 4; break;
	}

	return 0;
}

/*
salidas: [{x:0, y:0, ori: 1}, {x:0, y:0, ori: 2}],
*/

Automapa_salida Automapa_salida::desde_token(const Herramientas_proyecto::Dnot_token& tok)
{
	return Automapa_salida{tok["x"], tok["y"], Automapa_salida::int_a_orientacion(tok["ori"])};
}

Automapa_sala::tmarcadores Automapa_sala::int_a_marcador(int val)
{
	switch(val)
	{
		case 0: return tmarcadores::nada; break;
		case 1: return tmarcadores::arbol; break;
		case 2: return tmarcadores::metal; break;
		case 3: return tmarcadores::madera; break;
		case 4: return tmarcadores::agua; break;
		case 5: return tmarcadores::fuego; break;
		case 6: return tmarcadores::tierra; break;
		case 7: return tmarcadores::velocidad; break;
	}

	return tmarcadores::nada;
}

int Automapa_sala::marcador_a_int(Automapa_sala::tmarcadores val)
{
	switch(val)
	{
		case tmarcadores::nada:		return 0; break;
		case tmarcadores::arbol: 	return 1; break;
		case tmarcadores::metal: 	return 2; break;
		case tmarcadores::madera: 	return 3; break;
		case tmarcadores::agua: 	return 4; break;
		case tmarcadores::fuego: 	return 5; break;
		case tmarcadores::tierra: 	return 6; break;
		case tmarcadores::velocidad: 	return 7; break;
	}

	return 0;
}

/*
{id: 1, x: 0, y:0, w:2, h:1, 
salidas: [{x:0, y:0, ori: 1}, {x:0, y:0, ori: 2}],
marker:[1,2]}
*/

Automapa_sala Automapa_sala::desde_token(const Herramientas_proyecto::Dnot_token& tok)
{
	Automapa_sala res{tok["id"], tok["x"], tok["y"], tok["w"], tok["h"]};

	for(int m : tok["marker"].acc_lista())
		res.marcadores.push_back(Automapa_sala::int_a_marcador(m));

	for(const auto& t : tok["salidas"].acc_lista())
		res.salidas.push_back(Automapa_salida::desde_token(t));

	return res;
}

void Automapa::cargar(const std::string& ruta)
{
	reiniciar();

	auto raiz=Herramientas_proyecto::parsear_dnot(ruta);
	for(const auto& ts : raiz["mapa"]["salas"].acc_lista())
	{
		auto s=Automapa_sala::desde_token(ts);
		salas.push_back(sala{s, false});
	}
}

void Automapa::visitar(int id_sala)
{
	auto it=std::find_if(std::begin(salas), std::end(salas), [id_sala](const sala& s) {return s.s.id==id_sala;});
	if(it!=std::end(salas))
	{
		it->visitada=true;
	}
}

void Automapa::reiniciar()
{
	for(auto& s : salas) s.visitada=false;
}

std::vector<Automapa_sala> Automapa::obtener_visitadas() const
{
	std::vector<Automapa_sala> res;
	for(auto &s : salas) if(s.visitada) res.push_back(s.s);		
	return res;
}
