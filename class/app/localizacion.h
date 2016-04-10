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
};

};
}
#endif
