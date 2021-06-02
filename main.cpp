#ifdef WINCOMPIL
	//Terrible parche para SDL2_Gfx...
	#include <SDL2_gfxPrimitives.c>
	#include <SDL2_rotozoom.c>
	#include <SDL2_imageFilter.c>
	#include <libloaderapi.h>
	#include <fileapi.h>

#else 

	#include <unistd.h>
	#include <stdlib.h>
	#include <sys/stat.h>

#endif

#include <libDan2.h>
#include <string>
#include <fstream>
#include <defDanSDL.h>
#include <source/ficheros_utilidades.h>
#include "class/framework/kernel.h"
#include "class/app/framework_impl/kernel_config.h"
#include "class/app/framework_impl/app_config.h"
#include "class/controladores/director_estados.h"
#include "class/app/definiciones.h"

//Declaración del log de aplicación en espacio global.
DLibH::Log_base LOG;
std::string App::env::data_path="";
std::string App::env::usr_path="";

int iniciar_app(Herramientas_proyecto::Controlador_argumentos&);
void ready_system();
void setup_file(const std::string& _in, const std::string& _out);

int main(int argc, char ** argv)
{
	ready_system();

	//Inicializar control de logs.
	std::string info_log_path=App::env::usr_path+"/logs/info.log",
		lib_log_path=App::env::usr_path+"/logs/motor_log.log";

	LOG.inicializar(info_log_path.c_str());
	LOG.activar();
	DLibH::Log_motor::arrancar(lib_log_path.c_str());

	//Inicializar control de argumentos.
	Herramientas_proyecto::Controlador_argumentos CARG(argc, argv);

	bool res=iniciar_app(CARG);
	DLibH::Log_motor::finalizar();
	return res;
}

int iniciar_app(Herramientas_proyecto::Controlador_argumentos& CARG)
{
	using namespace App;

	std::string app_log_path=App::env::usr_path+"/logs/app_log.log";
	DLibH::Log_base log_app(app_log_path.c_str());

	log_app<<"Iniciando proceso principal..."<<std::endl;
	log_app<<"data path as "<<App::env::data_path<<std::endl;
	log_app<<"usr path as "<<App::env::usr_path<<std::endl;

	//Intentar inicializar sin cargar aún la SDL...
	try
	{
		log_app<<"Inicializando configuración aplicación..."<<std::endl;
		App_config config;

		//Si la inicialización ha tenido éxito podemos arrancar la SDL y el Kernel.
		log_app<<"Inicializando configuración kernel..."<<std::endl;
		Kernel_config kernel_config(config);

		log_app<<"Inicializando SDL2..."<<std::endl;
		if(!DLibH::Herramientas_SDL::iniciar_SDL(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK))
		{
			throw std::runtime_error("No ha sido posible inicializar la librería SD2L");
		}

		log_app<<"Inicializando kernel..."<<std::endl;
		DFramework::Kernel kernel(CARG, kernel_config, config);
		App::Director_estados APP(kernel, config, log_app);

		if(APP.interpretar_parametros(kernel))
		{
			APP.iniciar(kernel);
		}

		log_app<<"Finalizando proceso principal."<<std::endl;
	}
	catch(std::exception& e)
	{
		log_app<<"Ha ocurrido un error "<<e.what()<<std::endl;
		std::cout<<"Abandonando aplicación debido a error : "<<e.what()<<std::endl;

		log_app<<"Finalizando SDL2..."<<std::endl;
		DLibH::Herramientas_SDL::apagar_SDL();

		return 1;
	}

	log_app<<"Finalizando SDL2..."<<std::endl;
	DLibH::Herramientas_SDL::apagar_SDL();
	return 0;
}

void setup_file(
	const std::string& _in, 
	const std::string& _out
) {

	std::string res, lin;
	std::ifstream f{_in};
	std::ofstream out{_out};

	while(true) {
		std::getline(f, lin);
		if(f.eof()) {
			break;
		}
		out<<lin<<std::endl;
	}
}

#ifdef WINCOMPIL

void ready_system() {

	std::array<char, 1024> buff;
	int bytes=GetModuleFileNameA(nullptr, buff.data(), 1024);

	std::string executable_path=std::string{std::begin(buff), std::begin(buff)+bytes};
	
	auto last_slash=executable_path.find_last_of("\\");
	std::string executable_dir=executable_path.substr(0, last_slash)+"\\";
	
	App::env::data_path=executable_dir+"/";
	App::env::usr_path=executable_dir+"user\\";
	
	std::string logs_path=App::env::usr_path+"\\logs\\";
	CreateDirectoryA(logs_path.c_str(), nullptr);
	CreateDirectoryA(App::env::usr_path.c_str(), nullptr);
	
	setup_file(App::env::data_path+"data/config/configuracion.dnot", App::env::usr_path+"/configuracion.dnot");
}

#else 

void ready_system() {
		
	std::string executable_path, executable_dir;
	std::array<char, 1024> buff;

	int bytes=readlink("/proc/self/exe", buff.data(), 1024);
	if(-1==bytes) {

		throw std::runtime_error("could not locate proc/self/exe");
	}

	executable_path=std::string{std::begin(buff), std::begin(buff)+bytes};
	auto last_slash=executable_path.find_last_of("/");
	executable_dir=executable_path.substr(0, last_slash)+"/";

	App::env::data_path=executable_dir+"/";
	App::env::usr_path=std::string{getenv("HOME")}+"/.go-fgj6/";

#ifdef AS_APPIMAGE
#pragma message ("Building as AppImage!!")
		App::env::data_path=App::env::data_path+"../share/";
#endif

	struct stat st={0};
	if(stat(App::env::usr_path.c_str(), &st) == -1) {

		mkdir(App::env::usr_path.c_str(), 0700);

		std::string logs_path=App::env::usr_path+"logs";
		mkdir(logs_path.c_str(), 0700);

		setup_file(App::env::data_path+"data/config/configuracion.dnot", App::env::usr_path+"/configuracion.dnot");
	}
}

#endif
