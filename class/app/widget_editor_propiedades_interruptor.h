#ifndef WIDGET_EDITOR_PROPIEDADES_INTERRUPTOR_H
#define WIDGET_EDITOR_PROPIEDADES_INTERRUPTOR_H

#include <class/compositor_vista.h>

#include "interruptor.h"
#include "widget_editor_interface.h"

namespace App
{

class Widget_editor_propiedades_interruptor: 
	public Widget_editor_interface
{
	public:

	Widget_editor_propiedades_interruptor(const DLibV::Fuente_TTF&, Interruptor&);
	
	virtual void 	dibujar(DLibV::Pantalla&);
	virtual void	inicializar(DFramework::Input&);
	virtual void	input(DFramework::Input&, float);
	virtual void	finalizar(DFramework::Input&);
	virtual bool	es_cerrar() const;

	private:

	void		actualizar_layout();
	void		cambiar_seleccion(int);
	void		cambiar(int dir, int& ref, const std::string&);
	void		cambiar_por_indice(int indice, int val);
	int		valor_por_indice(int indice) const;

	Herramientas_proyecto::Compositor_vista		layout;
	Interruptor&		ref;
	int			nivel,
				id_puerta,
				id_grupo,
				tiempo_grupo;
	bool			cerrar;

	int			indice_actual;

	static const int	min_indice=0;
	static const int 	max_indice=3;
};

}

#endif
