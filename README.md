# Proyecto1_Paralela
Screensaver with openMP

Para compilar el programa en secuencial basta con correr el make
```ssh
make
```
Para compilar el programa en paralelo es con el siguiente comando
```ssh
g++ -I src/include -L src/lib -o paralela paralela.cpp -lmingw32 -lSDL2main -lSDL2 -fopenmp
```
