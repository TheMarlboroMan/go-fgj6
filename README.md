#Go.

Small game for the Familiar Game Jam 6, made with C++ and SDL2.

Code available at https://github.com/TheMarlboroMan/go-fgj6. If you want to build it, be sure to tinkle with the makefile routes and build these before:

	https://github.com/TheMarlboroMan/libdansdl2 (classic branch)
	https://github.com/TheMarlboroMan/herramientas_proyecto (classic branch)

Go contains code from "Vs shooter" (https://github.com/TheMarlboroMan/vs_shooter). You could say that I lifted part of my own "engine" (mainly the level editor and part of the entities). Lots of new coding tough.

If you're building for windows, good luck with SDL2_gfx. I personally ended up including the c files in main.cpp and doing some nasty #ifdef work. If you want to go that way make sure to put the required files in the same directory as your SDL2 headers are (save yourself the headache :P). I may help if you're willing to walk the Mingw32 road.

##Credits:

###Music:
	Gymnopedie 1: Written by Eric Satie. Played by Peter Johnston (https://musopen.org/music/performer/peter-j/).

###Sounds:
	Various free sounds found in flashkit.com

###Fonts:
	Akashi by Ed Merritt, Bulldozer by Dcoxy, Imagination Station by Jonathan S. Harris and InkBurrow by Uddi Uddi. All free for non commercial uses, except for Inkburrow: completely free.

###Graphics:
	Background images downloaded from Filemorgue.com, photos taken by lauramusikanski and kconnors.

###Coding and artwork:
	The Marlboro Man.
