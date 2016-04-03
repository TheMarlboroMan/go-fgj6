#include "tiempo.h"

#include <sstream>
#include <iomanip>
#include <cmath>

using namespace App;

Tiempo::Tiempo()
	:tiempo(0.0f), 
	t_aviso(0.0f, 0.0f, 0.0f, Herramientas_proyecto::Valor_limitado<float>::inferior)
{

}

void Tiempo::penalizar()
{
	tiempo+=30.0f;
	t_aviso=1.2f;
}

void Tiempo::turno(float delta)
{
	tiempo+=delta;
	if(t_aviso)
	{
		t_aviso-=delta;
	}
}

void Tiempo::reiniciar()
{
	tiempo=0.0f;
}

std::string Tiempo::a_cadena()
{
	int minutos=tiempo / 60.0;
	float fsegundos=fmod(tiempo, 60.0);
	int segundos=fsegundos;

	double dummy=0.0;
	double tdecimas=modf(fsegundos, &dummy);
	int decimas=tdecimas*1000;

	std::stringstream ss;
	ss.fill('0');
	ss<<std::setw(2)<<minutos<<":"<<
		std::setw(2)<<segundos<<":"<<
		std::setw(3)<<decimas;

	return ss.str();
}
