#ifndef STRUCT_INPUT_FRAMEWORK_H
#define STRUCT_INPUT_FRAMEWORK_H

namespace App
{

struct Input
{
	enum inputs{
escape=0,
tab,

//Editor...

tecla_1,
tecla_2,
tecla_3,
tecla_4,
ayuda,
cambio_logica,
seleccion_color,
propiedades_mapa,
intercambiar_decoracion,
copiar,
pegar,
cargar_mapa,
grabar_mapa,
cursor_izquierda,
cursor_derecha,
cursor_arriba,
cursor_abajo,
enter,
suprimir,
control_izquierdo,
zoom_mas,
zoom_menos,
reset_zoom,
grid_mas,
grid_menos,
profundidad_mas,
profundidad_menos,
click_i,
click_d,

//De juego...
menu_arriba, 
menu_abajo,
menu_ok,
izquierda,
derecha,
arriba,
abajo,
};

};

}

#endif
