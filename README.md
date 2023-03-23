# Proyecto1_Paralela
Screensaver with openMP

Se debe tener los siguientes packetes para poder correr los programas:
* SDL2
* OpenMP (Para el programa paralelo)

Para compilar el programa en secuencial basta con correr el make
```ssh
make
```
Si el comando anterior no funciona intentar con el siguiente comando
```ssh
g++ -I src/include -L src/lib -o main main.cpp -lmingw32 -lSDL2main -lSDL2

```

Para compilar el programa en paralelo es con el siguiente comando
```ssh
g++ -I src/include -L src/lib -o paralela paralela.cpp -lmingw32 -lSDL2main -lSDL2 -fopenmp
```
