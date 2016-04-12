#include "app_config.h"

using namespace App;

const std::string App_config::CLAVE_W_FISICA_PANTALLA="config:video:w_fisica_ventana";
const std::string App_config::CLAVE_H_FISICA_PANTALLA="config:video:h_fisica_ventana";
const std::string App_config::CLAVE_W_LOGICA_PANTALLA="config:video:w_logica_ventana";
const std::string App_config::CLAVE_H_LOGICA_PANTALLA="config:video:h_logica_ventana";
const std::string App_config::CLAVE_AYUDA_ACTIVA="config:app:ayuda_activa";
const std::string App_config::CLAVE_DEBUG_ACTIVO="config:app:debug_activo";

App_config::App_config(): Configuracion_base(obtener_ruta_archivo())
{

}
