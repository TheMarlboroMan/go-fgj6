#include "input.h"

using namespace DFramework;

void Input::turno()
{
	controles_sdl.recoger();
}

Input::Resultado_lookup Input::obtener(int i) const
{
	//Esto puede dar problemas en el futuro si queremos usar el mismo
	//input para dos cosas distintas.
	//Vamos a cachear cada input en el dispositivo que le pertenece... 

	auto it=lookup.find(i);

	if(it!=lookup.end())
	{
		return it->second;
	}
	else
	{
		Resultado_lookup resultado(Resultado_lookup::NADA);

		auto f=[&resultado, this, i](const tipo_mapa& mapa, int tipo)
		{
			resultado.mapa=tipo;
			auto it=mapa.equal_range(i);
			for(auto r=it.first; r!=it.second; ++r)
			{
				resultado.val.push_back({r->second.sdl_clave, r->second.indice_dispositivo});
			}

			lookup.insert(std::make_pair(i, resultado));
		};

		//Buscamos en cada uno de los mapas si algo se corresponde con
		//la clave de aplicación de entrada. Si existe se meterá
		//en el mapa de lookup el dispositivo y el valor SDL. La
		//lambda además actualiza el resultado para devolverlo.

		if(mapa_teclado.count(i))
		{
			f(mapa_teclado, Resultado_lookup::TECLADO); 
		}
		else if(mapa_raton.count(i))
		{
			f(mapa_raton, Resultado_lookup::RATON); 
		}
		else if(mapa_joystick.count(i))
		{
			f(mapa_joystick, Resultado_lookup::JOYSTICK);
		}

		return resultado;
	}
}

bool Input::es_senal_salida() const
{
	return controles_sdl.es_senal_salida();
}

bool Input::es_input_down(int i) const
{
	Resultado_lookup rl=obtener(i);
	switch(rl.mapa)
	{
		case Resultado_lookup::TECLADO:
			for(auto val : rl.val) 
				if(controles_sdl.es_tecla_down(val.val)) return true;
		break;
		case Resultado_lookup::RATON:
			for(auto val : rl.val) 
				if(controles_sdl.es_boton_down(val.val)) return true;
		break;
		case Resultado_lookup::JOYSTICK:
			for(auto val : rl.val) 
				if(controles_sdl.es_joystick_boton_down(val.indice, val.val)) return true;
		break;
		default: break;
	}

	return false;
}

bool Input::es_input_up(int i) const
{
	Resultado_lookup rl=obtener(i);
	switch(rl.mapa)
	{
		case Resultado_lookup::TECLADO:
			for(auto val : rl.val) 
				if(controles_sdl.es_tecla_up(val.val)) return true;
		break;
		case Resultado_lookup::RATON:
			for(auto val : rl.val) 
				if(controles_sdl.es_boton_up(val.val)) return true;
		break;
		case Resultado_lookup::JOYSTICK:
			for(auto val : rl.val) 
				if(controles_sdl.es_joystick_boton_up(val.indice, val.val)) return true;
		break;
		default: break;
	}

	return false;
}

bool Input::es_input_pulsado(int i) const
{
	Resultado_lookup rl=obtener(i);
	switch(rl.mapa)
	{
		case Resultado_lookup::TECLADO:
			for(auto val : rl.val) 
				if(controles_sdl.es_tecla_pulsada(val.val)) return true;
		break;
		case Resultado_lookup::RATON:
			for(auto val : rl.val)
				if(controles_sdl.es_boton_pulsado(val.val)) return true;
		break;
		case Resultado_lookup::JOYSTICK:
			for(auto val : rl.val) 
				if(controles_sdl.es_joystick_boton_pulsado(val.indice, val.val)) return true;
		break;
		default: break;
	}

	return false;
}

//Esta es la función a la que se llama en primer lugar desde el Kernel. 
//Cargará todos los inputs registrados por la aplicación.
//Se mira primero el tipo de input y luego se hace corresponder la
//clave de aplicación con la clave SDL.

void Input::configurar(const std::vector<Par_input>& v)
{
	for(const auto& i : v)
	{
		configurar(i);
	}
}

void Input::configurar(Par_input i)
{
	tinput ti={i.sdl_clave, i.indice_dispositivo};
	auto par=std::make_pair(i.clave, ti);

	switch(i.tipo)
	{
		case Par_input::tipos::nada:		break;
		case Par_input::tipos::teclado: 	mapa_teclado.insert(par); break;
		case Par_input::tipos::raton:		mapa_raton.insert(par); break;
		case Par_input::tipos::joystick:	mapa_joystick.insert(par); break;
	}
}


//Elimina un input de los mapas y del lookup, liberándolo para poder volverlo
//a configurar.

void Input::limpiar(int clave)
{
	if(mapa_teclado.count(clave)) mapa_teclado.erase(clave);
	if(mapa_raton.count(clave)) mapa_raton.erase(clave);
	if(mapa_joystick.count(clave)) mapa_joystick.erase(clave);
	if(lookup.count(clave)) lookup.erase(clave);
}

Input::Entrada Input::obtener_entrada() const
{
	if(controles_sdl.recibe_eventos_teclado_down())
	{
		return Entrada{Entrada::ttipo::teclado, controles_sdl.obtener_tecla_down(), 0};
	}
	else if(controles_sdl.recibe_eventos_boton_raton())
	{
		return Entrada{Entrada::ttipo::raton, controles_sdl.obtener_boton_down(), 0};
	}
	else if(controles_sdl.recibe_eventos_boton_joystick_down())
	{
		int cantidad=controles_sdl.acc_cantidad_joysticks(), i=0;
		while(i < cantidad)
		{
			int btn=controles_sdl.obtener_joystick_boton_down(i);
			if(btn >= 0) return Entrada{Entrada::ttipo::joystick, btn, i};
			++i;
		}
	}

	return Entrada{Entrada::ttipo::nada, 0, 0};
}

Input::Entrada Input::localizar_entrada(int i) const
{
	Entrada res{Entrada::ttipo::nada, 0, 0};

	auto l=obtener(i);

	switch(l.mapa)
	{
		case Resultado_lookup::NADA:	
		break;
		
		case Resultado_lookup::TECLADO:
			res.tipo=Entrada::ttipo::teclado;
			res.codigo=l.val[0].val;
			
		break;

		case Resultado_lookup::RATON:
			res.tipo=Entrada::ttipo::raton;
			res.codigo=l.val[0].val;
		break;

		case Resultado_lookup::JOYSTICK:
			res.tipo=Entrada::ttipo::joystick;
			res.dispositivo=l.val[0].indice;
			res.codigo=l.val[0].val;
		break;
	}

	return res;
}

Par_input Input::desde_entrada(const Input::Entrada& e, int clave)
{
	Par_input::tipos t=Par_input::tipos::nada;

	switch(e.tipo)
	{
		case Entrada::ttipo::nada: break;
		case Entrada::ttipo::teclado: t=Par_input::tipos::teclado; break;
		case Entrada::ttipo::raton: t=Par_input::tipos::raton; break;
		case Entrada::ttipo::joystick: t=Par_input::tipos::joystick; break;
	}

	return Par_input{t, clave, e.codigo, e.dispositivo};
}
