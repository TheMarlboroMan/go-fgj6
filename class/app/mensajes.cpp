#include "mensajes.h"

using namespace App;

Widget_mensajes::Widget_mensajes(const DLibV::Fuente_TTF& f, size_t maxm, int x, int y)
	:fuente(f), max_mensajes(maxm), x(x), y(x)
{

}

void Widget_mensajes::insertar_mensaje(const std::string& c, float t)
{
	if(mensajes.size()==max_mensajes)
	{
		mensajes.erase(std::begin(mensajes));
	}

	mensajes.push_back(Mensaje{fuente, c, t});
}

void Widget_mensajes::dibujar(DLibV::Pantalla& pantalla)
{
	int py=y;
	int margen=fuente.acc_tamano_fuente()+2;

	for(auto& m : mensajes)
	{
		m.txt.ir_a(x, py);
		m.txt.volcar(pantalla);
		py+=margen;
	}
}

void Widget_mensajes::turno(float delta)
{
	auto it=std::begin(mensajes);
	
	while(it < std::end(mensajes))
	{
		it->tiempo-=delta;
		if(it->tiempo <=0.0f) it=mensajes.erase(it);
		else ++it;
	}
}
