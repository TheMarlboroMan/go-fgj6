#ifndef LOCALIZACION_H
#define LOCALIZACION_H

namespace App
{

struct Localizacion
{

	enum l{
intro_pulsa_algo=50,
intro_empezar=51,
intro_controles=52,
intro_salir=53,
intro_config=54,

confirmacion_salida=100,
mensaje_fin=101,

config_ventana_titulo=150,
config_ventana_opcion_10=151,
config_ventana_opcion_20=152,
config_ventana_opcion_30=153,
config_ayuda_titulo=160,
config_ayuda_opcion_10=161,
config_ayuda_opcion_20=162,
config_salir=170,

controles_teclado=200,
controles_raton=201,
controles_joystick_a=202,
controles_joystick_b=203,
controles_izquierda=204,
controles_derecha=205,
controles_arriba=206,
controles_abajo=207,
controles_desconocido=208,
controles_introducir=209,
controles_salir=210,
controles_mapa=211
};

};
}
#endif
