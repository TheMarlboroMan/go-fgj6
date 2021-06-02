#include "director_estados.h"
#include "../app/definiciones.h"

#include <algorithm>

using namespace App;

extern DLibH::Log_base LOG;

Director_estados::Director_estados(DFramework::Kernel& kernel, App::App_config& c, DLibH::Log_base& log)
	:Director_estados_interface(t_estados::intro, std::function<bool(int)>([](int v){return v > estado_min && v < estado_max;})),
	config(c), log(log), localizador(env::data_path+"/data/localizacion/loc")
{
	preparar_video(kernel);
	registrar_fuentes();
	registrar_controladores(kernel);
	virtualizar_joysticks(kernel.acc_input());
	localizador.inicializar(0);
}

bool Director_estados::interpretar_parametros(DFramework::Kernel& kernel)
{
	int indice_file=kernel.acc_controlador_argumentos().buscar("file");
	int indice_check=kernel.acc_controlador_argumentos().buscar("map_check");
	if(indice_file!=-1)
	{
		estados.validar_y_cambiar_estado(editor);
		controlador_editor->iniciar_edicion_fichero(kernel.acc_controlador_argumentos().acc_argumento(indice_file+1));
		controlador_principal->establecer_editor(true);
		return true;
	}
	else if(indice_check!=-1)
	{
		comprobar_mapas();
		return false;
	}

	DFramework::Audio::reproducir_musica(DLibA::Gestor_recursos_audio::obtener_musica(1));
	return true;
}

void Director_estados::preparar_video(DFramework::Kernel& kernel)
{
	auto& pantalla=kernel.acc_pantalla();

	int wf=config.acc_w_fisica_pantalla(),
		hf=config.acc_h_fisica_pantalla(),
		wl=config.acc_w_logica_pantalla(),
		hl=config.acc_h_logica_pantalla();

	pantalla.inicializar(wf, hf);
	pantalla.establecer_medidas_logicas(wl, hl);
	pantalla.establecer_modo_ventana(config.acc_modo_pantalla());
}

void Director_estados::registrar_controladores(DFramework::Kernel& kernel)
{
	controlador_principal.reset(new Controlador_principal(log, fuentes, localizador, sistema_audio));
	controlador_editor.reset(new Controlador_editor(log, fuentes));
	controlador_ayuda_editor.reset(new Controlador_ayuda_editor(log, fuentes));
	controlador_intro.reset(new Controlador_intro(log, fuentes, localizador, sistema_audio));
	controlador_controles.reset(new Controlador_controles(log, config, fuentes, localizador, kernel.acc_input()));
	controlador_config.reset(new Controlador_config(log, config, fuentes, localizador));
	controlador_mapa.reset(new Controlador_mapa(log, sistema_audio));
	controlador_credits.reset(new Controlador_credits(log, fuentes, localizador, sistema_audio));

	registrar_controlador(t_estados::principal, *controlador_principal);
	registrar_controlador(t_estados::editor, *controlador_editor);
	registrar_controlador(t_estados::ayuda_editor, *controlador_ayuda_editor);
	registrar_controlador(t_estados::intro, *controlador_intro);
	registrar_controlador(t_estados::controles, *controlador_controles);
	registrar_controlador(t_estados::config, *controlador_config);
	registrar_controlador(t_estados::estado_mapa, *controlador_mapa);
	registrar_controlador(t_estados::credits, *controlador_credits);
}

void Director_estados::preparar_cambio_estado(int deseado, int actual)
{
	switch(deseado)
	{
		case t_estados::principal:
			if(actual==t_estados::editor)
			{
				controlador_principal->cargar_nivel(controlador_editor->acc_nombre_fichero());
				controlador_principal->iniciar_nivel(1);
			}
			else if(actual==t_estados::intro)
			{
				controlador_mapa->reiniciar();
				controlador_principal->establecer_ayuda(config.es_ayuda_activa());
				controlador_principal->iniciar_juego();
			}
			else if(actual==t_estados::estado_mapa)
			{
				//Do nothing...
			}
		break;
		case t_estados::editor: break;
		case t_estados::estado_mapa:
			controlador_mapa->descubrir_salas(controlador_principal->obtener_salas_descubiertas(), controlador_principal->obtener_id_sala_actual());
		break;
		case t_estados::ayuda_editor: break;
		case t_estados::intro:
			if(actual==t_estados::principal)
			{
				if(controlador_principal->es_juego_finalizado()) controlador_intro->establecer_finalizado();
			}
		break;

		case t_estados::config: break;
		case t_estados::game_over: break;
	}
}

void Director_estados::input_comun(DFramework::Input& input, float delta)
{
	sistema_audio.turno(delta);	//No es input, pero bueno...

	if(input.es_nuevo_joystick_conectado())
	{
		log<<"Detectado nuevo joystick..."<<std::endl;
		virtualizar_joysticks(input);
	}
}

void Director_estados::virtualizar_joysticks(DFramework::Input& input)
{
	for(int i=0; i < input.obtener_cantidad_joysticks(); ++i)
	{
		input.virtualizar_hats_joystick(i);
		input.virtualizar_ejes_joystick(i,15000);
		log<<"Virtualizado joystick "<<i<<std::endl;
	}
}

void Director_estados::registrar_fuentes()
{
	fuentes.registrar_fuente("akashi", 16);
	fuentes.registrar_fuente("akashi", 20);
	fuentes.registrar_fuente("akashi", 9);
	fuentes.registrar_fuente("imagination_station", 16);
	fuentes.registrar_fuente("inkburrow", 26);
	fuentes.registrar_fuente("bulldozer", 20);
}

//Ejecuta una comprobación de los mapas, volcando los contenidos de las puertas
//de los mismos, para ver que no hemos repetido ninguna.

void Director_estados::comprobar_mapas()
{
#ifdef WINCOMPIL
	using namespace compat;
#else
	using namespace std;
#endif

	std::vector<int> ids;

	int i=1;
	while(i<=20)
	{
		const std::string nombre_fichero=env::data_path+"/data/app/mapas/mapa"+to_string(i)+".dat";

		Mapa mapa;
		Importador importador;
		importador.importar(nombre_fichero.c_str(), mapa);
		mapa.inicializar();

		if(mapa.puertas.size())
		{
			std::vector<int> actuales;

			std::cout<<"=="<<nombre_fichero<<"=="<<std::endl;
			for(const auto& p : mapa.puertas)
			{
				actuales.push_back(p.acc_id());

				if(std::find(std::begin(ids), std::end(ids), p.acc_id())==std::end(ids))
				{
					std::cout<<"\tID: "<<p.acc_id()<<std::endl;
				}
				else
				{
					std::cout<<"\t[Warning] ID: "<<p.acc_id()<<std::endl;
				}
			}

			std::sort(std::begin(actuales), std::end(actuales));
			actuales.erase(std::unique( std::begin(actuales), std::end(actuales) ), std::end(actuales));
			ids.insert(std::end(ids), std::begin(actuales), std::end(actuales));
		}

		++i;
	}

	std::cout<<"Finalizado"<<std::endl;
}
