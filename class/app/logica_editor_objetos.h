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
	static const tcolor	color_punto_inicio_editor;
	static const tcolor	color_salida_editor;
	static const tcolor	color_pieza_editor;
	static const tcolor	color_interruptor_editor;
	static const tcolor	color_puerta_editor;
	static const tcolor	color_mejora_velocidad_editor;
	static const tcolor	color_arbol_editor;

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

class Inicio_editor:
	public Objeto_editor
{
	public:

	Inicio_editor(const Inicio& i)
		:elemento(i), poligono(cuadrado(i.acc_punto().x, i.acc_punto().y, 6))
	{

	}

	Inicio	elemento;
	virtual void		mover(double x, double y) 
	{
		elemento.mut_punto(elemento.acc_punto()+Espaciable::tpunto{x, y});
		poligono=cuadrado(elemento.acc_punto().x, elemento.acc_punto().y, 6);
	}
	virtual bool		es_bajo_cursor(punto pt) const {return punto_en_poligono(poligono, pt);}
	virtual void		dibujar(Representador& r,DLibV::Pantalla& pantalla, const DLibV::Camara& camara, bool seleccionado=false) const
	{
		dibujar_poligono(r, pantalla, poligono, color_punto_inicio_editor, color_punto_inicio_editor, camara, seleccionado);
	}

	private:

	Espaciable::tpoligono	poligono;
};

class Pieza_editor:
	public Objeto_editor
{
	public:

	Pieza_editor(const Pieza& i)
		:elemento(i)
	{

	}

	Pieza	elemento;
	virtual void		mover(double x, double y) {elemento.mover(x, y);}
	virtual bool		es_bajo_cursor(punto pt) const {return punto_en_poligono(elemento.acc_poligono(), pt);}
	virtual void		dibujar(Representador& r,DLibV::Pantalla& pantalla, const DLibV::Camara& camara, bool seleccionado=false) const
	{
		dibujar_poligono(r, pantalla, elemento.acc_poligono(), color_pieza_editor, color_pieza_editor, camara, seleccionado);
	}
};

class Interruptor_editor:
	public Objeto_editor
{
	public:

	Interruptor_editor(const Interruptor& i)
		:elemento(i)
	{

	}

	Interruptor	elemento;
	virtual void		mover(double x, double y) {elemento.mover(x, y);}
	virtual bool		es_bajo_cursor(punto pt) const {return punto_en_poligono(elemento.acc_poligono(), pt);}
	virtual void		dibujar(Representador& r,DLibV::Pantalla& pantalla, const DLibV::Camara& camara, bool seleccionado=false) const
	{
		dibujar_poligono(r, pantalla, elemento.acc_poligono(), color_interruptor_editor, color_interruptor_editor, camara, seleccionado);
	}
};

class Salida_editor:
	public Objeto_editor
{
	public:

	Salida_editor(const Salida& o):elemento(o) {}

	Salida			elemento;
	virtual void		mover(double x, double y) {elemento.mover(x, y);}
	virtual bool		es_bajo_cursor(punto pt) const {return punto_en_poligono(elemento.acc_poligono(), pt);}	
	virtual void		dibujar(Representador& r,DLibV::Pantalla& pantalla, const DLibV::Camara& camara, bool seleccionado=false) const
	{
		dibujar_poligono(r, pantalla, elemento.acc_poligono(), color_salida_editor, color_salida_editor, camara, seleccionado);
	}
	
};

class Puerta_editor:
	public Objeto_editor
{
	public:

	Puerta_editor(const Puerta& o):elemento(o) {}

	Puerta			elemento;
	virtual void		mover(double x, double y) {elemento.mover(x, y);}
	virtual bool		es_bajo_cursor(punto pt) const {return punto_en_poligono(elemento.acc_poligono(), pt);}	
	virtual void		dibujar(Representador& r,DLibV::Pantalla& pantalla, const DLibV::Camara& camara, bool seleccionado=false) const
	{
		dibujar_poligono(r, pantalla, elemento.acc_poligono(), color_puerta_editor, color_salida_editor, camara, seleccionado);
	}
	
};

class Mejora_velocidad_editor:
	public Objeto_editor
{
	public:

	Mejora_velocidad_editor(const Mejora_velocidad& i)
		:elemento(i)
	{

	}

	Mejora_velocidad	elemento;
	virtual void		mover(double x, double y) {elemento.mover(x, y);}
	virtual bool		es_bajo_cursor(punto pt) const {return punto_en_poligono(elemento.acc_poligono(), pt);}
	virtual void		dibujar(Representador& r,DLibV::Pantalla& pantalla, const DLibV::Camara& camara, bool seleccionado=false) const
	{
		dibujar_poligono(r, pantalla, elemento.acc_poligono(), color_mejora_velocidad_editor, color_mejora_velocidad_editor, camara, seleccionado);
	}
};

class Arbol_editor:
	public Objeto_editor
{
	public:

	Arbol_editor(const Arbol& i)
		:elemento(i)
	{

	}

	Arbol			elemento;
	virtual void		mover(double x, double y) {elemento.mover(x, y);}
	virtual bool		es_bajo_cursor(punto pt) const {return punto_en_poligono(elemento.acc_poligono(), pt);}
	virtual void		dibujar(Representador& r,DLibV::Pantalla& pantalla, const DLibV::Camara& camara, bool seleccionado=false) const
	{
		dibujar_poligono(r, pantalla, elemento.acc_poligono(), color_arbol_editor, color_arbol_editor, camara, seleccionado);
	}
};

}

#endif
