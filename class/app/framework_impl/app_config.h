#ifndef APP_CONFIGURACION_H
#define APP_CONFIGURACION_H

#include "../../framework/configuracion_base.h"
#include "../../framework/input.h"

#include <iostream>

#ifdef WINCOMPIL
/* Localización del parche mingw32... Esto debería estar en otro lado, supongo. */
#include <herramientas/herramientas/herramientas.h>
#endif

namespace App
{

class App_config:
	public DFramework::Configuracion_base
{
	////////////////////////////////
	// Interface pública.

	public:

	struct input_jugador
	{
		enum devs{teclado=0, joystick=1, raton=2, nada=3};
		int tipo, device, codigo;
	};

	//Traduce el tipo del framework al de la configuración.
	static input_jugador fw_a_config(DFramework::Input::Entrada& e)
	{
		int tipo=input_jugador::nada;

		switch(e.tipo)
		{
			case DFramework::Input::Entrada::ttipo::teclado: 	tipo=input_jugador::teclado; break;
			case DFramework::Input::Entrada::ttipo::joystick: 	tipo=input_jugador::joystick; break;
			case DFramework::Input::Entrada::ttipo::raton: 		tipo=input_jugador::raton; break;
			case DFramework::Input::Entrada::ttipo::nada: 		tipo=input_jugador::nada; break;
		}

		return input_jugador{tipo, e.dispositivo, e.codigo};
	};

	int acc_w_logica_pantalla() const {return token_por_ruta(CLAVE_W_LOGICA_PANTALLA);}
	int acc_h_logica_pantalla() const {return token_por_ruta(CLAVE_H_LOGICA_PANTALLA);}
	int acc_w_fisica_pantalla() const {return token_por_ruta(CLAVE_W_FISICA_PANTALLA);}
	int acc_h_fisica_pantalla() const {return token_por_ruta(CLAVE_H_FISICA_PANTALLA);}
	int acc_zoom_mas() const 	{return token_por_ruta("config:input:general:zoom_mas");}
	int acc_zoom_menos() const 	{return token_por_ruta("config:input:general:zoom_menos");}
	input_jugador acc_izquierda(int j) const	{return token_por_ruta_jugador("izquierda", j);}
	input_jugador acc_derecha(int j) const 	{return token_por_ruta_jugador("derecha", j);}
	input_jugador acc_arriba(int j) const 	{return token_por_ruta_jugador("arriba", j);}
	input_jugador acc_abajo(int j) const	{return token_por_ruta_jugador("abajo", j);}
	input_jugador acc_mapa(int j) const	{return token_por_ruta_jugador("mapa", j);}
	bool es_ayuda_activa() const		{return token_por_ruta(CLAVE_AYUDA_ACTIVA);}

	void mut_w_logica_pantalla(int p_valor) {configurar(CLAVE_W_LOGICA_PANTALLA, p_valor);}
	void mut_h_logica_pantalla(int p_valor) {configurar(CLAVE_H_LOGICA_PANTALLA, p_valor);}
	void mut_w_fisica_pantalla(int p_valor) {configurar(CLAVE_W_FISICA_PANTALLA, p_valor);}
	void mut_h_fisica_pantalla(int p_valor) {configurar(CLAVE_H_FISICA_PANTALLA, p_valor);}
	void mut_ayuda_activa(bool p_valor)	{configurar(CLAVE_AYUDA_ACTIVA, p_valor);}

	void mut_izquierda(int j, input_jugador p_valor) 	{configurar_jugador(ruta_jugador("izquierda", j), p_valor);}
	void mut_derecha(int j, input_jugador p_valor) 	 	{configurar_jugador(ruta_jugador("derecha", j), p_valor);}
	void mut_arriba(int j, input_jugador p_valor) 	 	{configurar_jugador(ruta_jugador("arriba", j), p_valor);}
	void mut_abajo(int j, input_jugador p_valor) 		{configurar_jugador(ruta_jugador("abajo", j), p_valor);}
	void mut_mapa(int j, input_jugador p_valor) 		{configurar_jugador(ruta_jugador("mapa", j), p_valor);}

	App_config();

	private:

	void configurar_jugador(const std::string& ruta, input_jugador j)
	{
		auto& tok=token_por_ruta(ruta);
		auto& l=tok.acc_lista();
		l[0].asignar(j.tipo);
		l[1].asignar(j.device);
		l[2].asignar(j.codigo);
	};

	std::string ruta_jugador(const std::string& tipo, int j) const
	{
#ifdef WINCOMPIL
		using namespace parche_mingw;
#else
		using namespace std;
#endif
		return "config:input:jugador_"+to_string(j)+":"+tipo;
	}

	input_jugador token_por_ruta_jugador(const std::string& tipo, int j) const
	{	
		const auto& tok=token_por_ruta(ruta_jugador(tipo, j));
		return input_jugador{tok[0], tok[1], tok[2]};
	}

	////////////////////////////////////
	// Definiciones...

	static const std::string CLAVE_W_LOGICA_PANTALLA;
	static const std::string CLAVE_H_LOGICA_PANTALLA;
	static const std::string CLAVE_W_FISICA_PANTALLA;
	static const std::string CLAVE_H_FISICA_PANTALLA;
	static const std::string CLAVE_AYUDA_ACTIVA;

	////////////////////////////////////
	// Implementacion...

	protected: 

	std::string obtener_clave_version_archivo() const {return "config:meta:v";}
	std::string obtener_version_archivo() const {return "1";}
	std::string obtener_clave_modo_pantalla() const {return "config:video:modo_pantalla";}
	std::string obtener_clave_modo_hardware() const {return "config:video:modo_hardware";}
	std::string obtener_clave_pantalla_doble_buffer() const {return "config:video:doble_buffer";}
	std::string obtener_clave_pantalla_anyformat() const {return "config:video:pantalla_anyformat";}
	std::string obtener_clave_volumen_audio() const {return "config:audio:volumen_audio";}
	std::string obtener_clave_volumen_musica() const {return "config:audio:volumen_musica";}
	std::string obtener_clave_audio_ratio() const {return "config:audio:audio_ratio";}
	std::string obtener_clave_audio_buffers() const {return "config:audio:audio_buffers";}
	std::string obtener_clave_audio_salidas() const {return "config:audio:audio_salidas";}
	std::string obtener_clave_audio_canales() const {return "config:audio:audio_canales";}

	///////////////////////////////////
	// Propiedades.

	private:

	std::string obtener_ruta_archivo() const {return "data/config/configuracion.dnot";}
};

}
#endif
