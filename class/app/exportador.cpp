#include "exportador.h"

using namespace App;

std::string Exportador::serializar(Mapa& mapa)
{
	using namespace Herramientas_proyecto;

	Dnot_token::t_vector vobstaculos;
	for(const auto& o : mapa.obstaculos) vobstaculos.push_back(serializar(o));

	Dnot_token::t_vector vdecoraciones;
	for(const auto& o : mapa.decoraciones) vdecoraciones.push_back(serializar(o));

	Dnot_token::t_vector vinicios;
	for(const auto& o : mapa.inicios) vinicios.push_back(serializar(o));

	Dnot_token::t_vector vsalidas;
	for(const auto& o : mapa.salidas) vsalidas.push_back(serializar(o));

	Dnot_token::t_vector vpiezas;
	for(const auto& o : mapa.piezas) vpiezas.push_back(serializar(o));

	Dnot_token::t_vector vinterruptores;
	for(const auto& o : mapa.interruptores) vinterruptores.push_back(serializar(o));

	Dnot_token::t_vector vpuertas;
	for(const auto& o : mapa.puertas) vpuertas.push_back(serializar(o));

	Dnot_token::t_vector vmejoras_velocidad;
	for(const auto& o : mapa.mejoras_velocidad) vmejoras_velocidad.push_back(serializar(o));

	Dnot_token tok_obstaculos(vobstaculos);
	Dnot_token tok_decoraciones(vdecoraciones);
	Dnot_token tok_inicios(vinicios);
	Dnot_token tok_salidas(vsalidas);
	Dnot_token tok_piezas(vpiezas);
	Dnot_token tok_interruptores(vinterruptores);
	Dnot_token tok_puertas(vpuertas);
	Dnot_token tok_mejoras_velocidad(vmejoras_velocidad);

	Dnot_token::t_mapa mapa_final;
	mapa_final["geometria"]=tok_obstaculos;
	mapa_final["decoracion"]=tok_decoraciones;
	mapa_final["inicios"]=tok_inicios;
	mapa_final["salidas"]=tok_salidas;
	mapa_final["piezas"]=tok_piezas;
	mapa_final["interruptores"]=tok_interruptores;
	mapa_final["puertas"]=tok_puertas;
	mapa_final["mejoras_velocidad"]=tok_mejoras_velocidad;
	
	Dnot_token base(mapa_final);
	return base.serializar();
}

Herramientas_proyecto::Dnot_token::t_vector Exportador::generar_punto(double x, double y)
{
	using namespace Herramientas_proyecto;
	Dnot_token::t_vector arr;
	arr.push_back(Dnot_token(x));
	arr.push_back(Dnot_token(y));
	return arr;
}

Herramientas_proyecto::Dnot_token::t_vector Exportador::generar_color(int r, int g, int b, int a)
{
	using namespace Herramientas_proyecto;
	Dnot_token::t_vector arr;
	arr.push_back(Dnot_token(r));
	arr.push_back(Dnot_token(g));
	arr.push_back(Dnot_token(b));
	arr.push_back(Dnot_token(a));
	return arr;
}

Herramientas_proyecto::Dnot_token Exportador::serializar(const Obstaculo& o)
{
	using namespace Herramientas_proyecto;

	Dnot_token::t_vector puntos;
	for(const auto& v : o.acc_poligono().acc_vertices())
		puntos.push_back(Dnot_token(generar_punto(v.x, v.y)));

	auto centro=o.acc_poligono().acc_centro();

	Dnot_token::t_mapa propiedades;

	int tipo=0;
	switch(o.acc_tipo())
	{
		case Obstaculo::ttipos::normal: tipo=0; break;
		case Obstaculo::ttipos::inocuo: tipo=1; break;
	}

	propiedades["t"].asignar(tipo);
	
	Dnot_token::t_mapa mapa_objeto;
	mapa_objeto["p"].asignar(puntos);
	mapa_objeto["cen"].asignar(generar_punto(centro.x, centro.y));
	mapa_objeto["pr"].asignar(propiedades);

	return Dnot_token(mapa_objeto);
}

Herramientas_proyecto::Dnot_token Exportador::serializar(const Inicio& o)
{
	using namespace Herramientas_proyecto;

	auto pt=o.acc_punto();
	
	Dnot_token::t_mapa mapa_objeto;
	mapa_objeto["pos"].asignar(generar_punto(pt.x, pt.y));
	mapa_objeto["id"].asignar(o.acc_id());
	mapa_objeto["angulo"].asignar(o.acc_angulo());

	return Dnot_token(mapa_objeto);
}

Herramientas_proyecto::Dnot_token Exportador::serializar(const Decoracion& o)
{
	using namespace Herramientas_proyecto;

	Dnot_token::t_vector puntos;
	for(const auto& v : o.acc_poligono().acc_vertices())
		puntos.push_back(Dnot_token(generar_punto(v.x, v.y)));

	auto centro=o.acc_poligono().acc_centro();
	auto color=o.acc_color();
	auto colorl=o.acc_color_linea();

	Dnot_token::t_mapa mapa_polig;
	mapa_polig["p"].asignar(puntos);
	mapa_polig["cen"].asignar(generar_punto(centro.x, centro.y));

	auto ncolor=generar_color(color.r, color.g, color.b, color.a);
	auto ncolorl=generar_color(colorl.r, colorl.g, colorl.b, colorl.a);

	Dnot_token::t_vector arr_colores;
	arr_colores.push_back(Dnot_token(ncolor));
	arr_colores.push_back(Dnot_token(ncolorl));

	Dnot_token::t_mapa propiedades;
	propiedades["fr"].asignar(o.es_frente());
	propiedades["pr"].asignar(o.acc_profundidad());

	mapa_polig["col"].asignar(arr_colores);
	mapa_polig["pr"].asignar(propiedades);

	return Dnot_token(mapa_polig);
}

Herramientas_proyecto::Dnot_token Exportador::serializar(const Salida& o)
{
	using namespace Herramientas_proyecto;

	Dnot_token::t_vector puntos;
	for(const auto& v : o.acc_poligono().acc_vertices())
		puntos.push_back(Dnot_token(generar_punto(v.x, v.y)));

	Dnot_token::t_mapa mapa_objeto;
	mapa_objeto["p"].asignar(puntos);
	mapa_objeto["idm"].asignar(o.acc_id_mapa());
	mapa_objeto["idi"].asignar(o.acc_id_inicio());

	return Dnot_token(mapa_objeto);
}

Herramientas_proyecto::Dnot_token Exportador::serializar(const Pieza& o)
{
	using namespace Herramientas_proyecto;

	Dnot_token::t_mapa mapa_objeto;

	auto centro=o.acc_poligono().acc_centro();
	
	mapa_objeto["pos"].asignar(generar_punto(centro.x, centro.y));
	mapa_objeto["i"].asignar(o.acc_indice());

	return Dnot_token(mapa_objeto);
}

Herramientas_proyecto::Dnot_token Exportador::serializar(const Interruptor& o)
{
	using namespace Herramientas_proyecto;

	Dnot_token::t_mapa mapa_objeto;

	auto centro=o.acc_poligono().acc_centro();
	
	mapa_objeto["pos"].asignar(generar_punto(centro.x, centro.y));
	mapa_objeto["n"].asignar(o.acc_nivel());
	mapa_objeto["idp"].asignar(o.acc_id_puerta());
	mapa_objeto["idg"].asignar(o.acc_id_grupo());
	mapa_objeto["tgr"].asignar(o.acc_tiempo_grupo());

	return Dnot_token(mapa_objeto);
}

Herramientas_proyecto::Dnot_token Exportador::serializar(const Puerta& o)
{
	using namespace Herramientas_proyecto;

	Dnot_token::t_vector puntos;
	for(const auto& v : o.acc_poligono().acc_vertices())
		puntos.push_back(Dnot_token(generar_punto(v.x, v.y)));

	Dnot_token::t_mapa mapa_objeto;
	mapa_objeto["p"].asignar(puntos);
	mapa_objeto["id"].asignar(o.acc_id());

	return Dnot_token(mapa_objeto);
}

Herramientas_proyecto::Dnot_token Exportador::serializar(const Mejora_velocidad& o)
{
	using namespace Herramientas_proyecto;

	Dnot_token::t_mapa mapa_objeto;

	auto centro=o.acc_poligono().acc_centro();
	mapa_objeto["pos"].asignar(generar_punto(centro.x, centro.y));
	mapa_objeto["n"].asignar(o.acc_nivel());

	return Dnot_token(mapa_objeto);
}
