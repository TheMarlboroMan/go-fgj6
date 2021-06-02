#include "credits.h"

#include "../app/framework_impl/input.h"
#include "../app/localizacion.h"
#include "../app/recursos.h"
#include "../app/definiciones.h"
#include <sstream>

using namespace App;

Controlador_credits::Controlador_credits(DLibH::Log_base& log, const Fuentes& fuentes, const Localizador& loc, Sistema_audio& sa)
	:log(log), fuente(fuentes.obtener_fuente("imagination_station", 16)),
	localizador(loc), sistema_audio(sa)
{
	layout.mapear_fuente("fuente", fuente);
}

void Controlador_credits::preloop(DFramework::Input& input, float delta)
{

}

void Controlador_credits::postloop(DFramework::Input& input, float delta)
{

}

void Controlador_credits::loop(DFramework::Input& input, float delta)
{
	if(input.es_senal_salida()) {
	
		abandonar_aplicacion();
	};
	
	switch(modo) {
	
		case modos::fadein:
		case modos::fadeout:
		
			fader.turno(delta);
			layout.obtener_por_id("credits")->establecer_alpha((int)fader);
			layout.obtener_por_id("version")->establecer_alpha((int)fader);
			if(fader.es_finalizado()) {
				
				if(modo==modos::fadein) {
					modo=modos::credits;
				}
				else {
					solicitar_cambio_estado(intro);
				}
			}
		
		break;
		case modos::credits:
		
			if(input.es_input_down(Input::escape)) {
	
				sistema_audio.insertar(Info_audio_reproducir(
				Info_audio_reproducir::t_reproduccion::simple,
				Info_audio_reproducir::t_sonido::repetible,
				r_sonidos::s_viento, 127, 127)
				);
			
				fader.reset(255.0f, 1.0f, 200.f);
				modo=modos::fadeout;
			}
		break;
	}
}

void Controlador_credits::dibujar(DLibV::Pantalla& pantalla)
{
	layout.volcar(pantalla);
}

void Controlador_credits::despertar()
{
	layout.parsear(env::data_path+"/data/layout/layout_credits.dnot", "layout");

	static_cast<DLibV::Representacion_TTF *>(layout.obtener_por_id("credits"))->asignar(localizador.obtener(Localizacion::credits));
			
	std::stringstream ss;
	ss<<localizador.obtener(Localizacion::version)<<App::version::major<<"."<<App::version::minor<<"."<<App::version::patch;
	static_cast<DLibV::Representacion_TTF *>(layout.obtener_por_id("version"))->asignar(ss.str());
	
	fader.reset(1.0f, 255.0f, 200.f);

	modo=modos::fadein;
}

void Controlador_credits::dormir()
{
	layout.vaciar_vista();
	layout.vaciar_constantes();
}

bool Controlador_credits::es_posible_abandonar_estado() const
{
	return true;
}

