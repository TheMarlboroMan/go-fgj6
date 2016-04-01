#ifndef LOGICA_EDITOR_OBJETOS
#define LOGICA_EDITOR_OBJETOS

namespace App
{

class Objeto_editor
{
	public:

	typedef DLibH::Punto_2d<double> punto;

	virtual void		mover(double, double)=0;
	virtual void		dibujar(Representador&, DLibV::Pantalla&, const DLibV::Camara&, bool=false) const=0;
	virtual bool		es_bajo_cursor(punto) const=0;

	void			borrar() {para_borrar=true;}
	bool			es_borrar() const {return para_borrar;}
	
	static Espaciable::tpoligono 	cuadrado(double x, double y, int rad)
	{
		return Espaciable::tpoligono { {{x-rad, y-rad}, {x-rad, y+rad}, {x+rad, y+rad}, {x+rad, y-rad}}, {x, y}};
	}

	static const tcolor	color_seleccion;
	static const tcolor	color_obstaculo;
	static const tcolor	color_obstaculo_inocuo;
	static const tcolor	color_borde_obstaculo;

	protected:

	static void 		dibujar_poligono(
			Representador& r,DLibV::Pantalla& pantalla, 
			const DLibH::Poligono_2d<double> poligono, 
			const tcolor& color_fondo,
			const tcolor& color_linea,
			const DLibV::Camara& camara, 
			bool seleccionado)
	{
		if(seleccionado)
		{
			r.dibujar_poligono(pantalla, poligono, color_fondo, camara);
			r.dibujar_poligono_lineas(pantalla, poligono, color_seleccion, camara);
		}
		else
		{
			if(color_fondo==color_linea)
			{
				r.dibujar_poligono(pantalla, poligono, color_fondo, camara);
			}
			else
			{
				r.dibujar_poligono(pantalla, poligono, color_fondo, camara);
				r.dibujar_poligono_lineas(pantalla, poligono, color_linea, camara);
			}
		}
	}

	private:

	bool			para_borrar=false;
};

class Obstaculo_editor:
	public Objeto_editor
{
	public:

	Obstaculo_editor(const Obstaculo& o):elemento(o) {}

	Obstaculo		elemento;
	virtual void		mover(double x, double y) {elemento.mover(x, y);}
	virtual bool		es_bajo_cursor(punto pt) const {return punto_en_poligono(elemento.acc_poligono(), pt);}	
	virtual void		dibujar(Representador& r,DLibV::Pantalla& pantalla, const DLibV::Camara& camara, bool seleccionado=false) const
	{
		tcolor color=color_obstaculo;
		switch(elemento.acc_tipo())
		{
			case Obstaculo::ttipos::normal: break;
			case Obstaculo::ttipos::inocuo: color=color_obstaculo_inocuo; break;
		}

		dibujar_poligono(r, pantalla, elemento.acc_poligono(), color, color_borde_obstaculo, camara, seleccionado);
	}
	
};

class Decoracion_editor:
	public Objeto_editor
{
	public:

	Decoracion_editor(const Decoracion& o):elemento(o) {}

	Decoracion		elemento;
	virtual void		mover(double x, double y) {elemento.mover(x, y);}
	virtual bool		es_bajo_cursor(punto pt) const {return punto_en_poligono(elemento.acc_poligono(), pt);}	
	virtual void		dibujar(Representador& r,DLibV::Pantalla& pantalla, const DLibV::Camara& camara, bool seleccionado=false) const
	{
		dibujar_poligono(r, pantalla, elemento.acc_poligono(), elemento.acc_color(), elemento.acc_color_linea(), camara, seleccionado);
	}	
};
/*
class Punto_inicio_editor:
	public Objeto_editor
{
	public:

	Punto_inicio_editor(const Espaciable::tpunto& pi)
		:elemento(pi), poligono(cuadrado(pi.x, pi.y, 6))
	{
	}

	Espaciable::tpunto	elemento;
	virtual void		mover(double x, double y) 
	{
		elemento.x+=x; elemento.y+=y;
		poligono=cuadrado(elemento.x, elemento.y, 6);
	}
	virtual bool		es_bajo_cursor(punto pt) const {return punto_en_poligono(poligono, pt);}
	virtual void		dibujar(Representador& r,DLibV::Pantalla& pantalla, const DLibV::Camara& camara, bool seleccionado=false) const
	{
		dibujar_poligono(r, pantalla, poligono, color_punto_inicio_editor, color_punto_inicio_editor, camara, seleccionado);
	}

	private:

	Espaciable::tpoligono	poligono;
};
*/
}

#endif
