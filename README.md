Seed sorter Project
===============
Example of current state of the Project 
0. 	``` 
	https://www.youtube.com/watch?v=l2LMlc-uq7o
	```

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

