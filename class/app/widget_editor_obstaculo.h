#ifndef WIDGET_EDITOR_OBSTACULO_H
#define WIDGET_EDITOR_OBSTACULO_H

#include <class/compositor_vista.h>

#include "widget_editor_interface.h"
#include "obstaculo.h"

namespace App
{

class Widget_editor_obstaculo: 
	public Widget_editor_interface
{
	public:

				Widget_editor_obstaculo(const DLibV::Fuente_TTF&, Obstaculo&);
	
	virtual void 		dibujar(DLibV::Pantalla&);
	virtual void		inicializar(DFramework::Input&);
	virtual void		input(DFramework::Input&, float);
	virtual bool		es_cerrar() const;
	virtual void		finalizar(DFramework::Input&);

	private:

	void			actualizar_layout();
	void			cambiar_seleccion(int);
	void			cambiar_valor(int, float);
	void			cambiar_tipo();

	Herramientas_proyecto::Compositor_vista		layout;
	Obstaculo&		elemento;
	Obstaculo::ttipos	tipo;
	bool			cerrar;

	int			indice_actual;
	float			t_pulsado;

	static const int	min_indice=0;
	static const int 	max_indice=0;
};

}

#endif
