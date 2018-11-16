Seed sorter Project
===============

To compile this project, you need openCV-3.2 for raspberry pi 3
1. Follow this guide by Pklab
	```bash 
	http://pklab.net/?id=392&lang=EN
	```
2. Update build command in geany by typing :
	```bash
	geany &
	```
in terminal. Then Build >> Set Build Commands.

Build : g++ -Wall -o "%e" "%f" $(pkg-config --libs --cflags opencv)

3. clone my git and you are ready to compile!

