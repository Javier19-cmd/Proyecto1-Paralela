# Proyecto 1 de Computación Paralela y Distribuída: Realizar un screensaver secuencial y paralelo.

### Descripción general del código secuencial.

Este proyecto consta de dos códigos, un código secuencial (screensaver_secuencial) y un código paralelo (screensaver_parelelo).

## screensaver_secuencial

### Resumen del código

Este código es una implementación de un screensaver secuencial en C++ utlilizando la librería de SDL2 para la representación gráfica de los elementos. Este screensaver 
enseña los elementos gráficos (en este caso círculos) en la pantalla con cierta velocidad y los mueve con rebote en las paredes. Además, el código también mide los 
cuadros por segundo (FPS) en tiempo real.

### Características

- Elementos gráficos (círculos) generados con propiedades aleatorias (color y velocidad).
- Movimiento de los elementos (o sea los círculos) con rebote en las paredes.
- Medición de los cuadros por segundo (FPS) en tiempo real.

### Requisitos
- SDL2 para la representación gráfica de los elementos.

### Uso
1. Compilar el programa utilizando un compilador de C++ (g++ o clang++). Para la ejecución se debe utilizar el siguiente comando en la terminal:
    g++ -o screensaver_secuencial screensaver_secuencial.cpp `sdl2-config --cflags --libs`
2. Ejecutar el programa con el siguiente comando:
    ./screensaver_secuencial
3. Ingresar la cantidad de elementos que se desean ver en la pantalla. (Esta debe ser mayor que 0).
4. Observar el screensaver en la pantalla.
5. Se puede cerrar el programa haciendo clic en la X de la ventana o haciendo Ctrl + C en la terminal.

# Notas
- Este programa usa SDL2 para la representacion gráfica, lo que permite crear una ventana y dibujar elementos en la misma. 
- Los elementos se generan con propiedades aleatorias, como color y velocidad. Lo anterior produce un screensaver diferente cada vez que se ejecuta el programa.
- El programa mide los cuadros por segundo (FPS) en tiempo real y lo imprime en la consola.
- El programa se ejecuta en bucle hasta que el usuario cierra la ventana.

# Descripción general del código paralelo.

El presente código secuencial consta de una clase Element, la cual se encarga de hacer posible la representación de los elementos en la pantalla que el usuario verá para el screensaver.
Los elementos que representan esta clase son principalmente círculos, los cuales tienen una posición, un radio y velocidades en los ejes x e y. Además, cada elemento tiene un color, 
el cual es asignado de manera aleatoria al momento de su creación. 

Por otro lado, es importante mencionar que la clase elemento tiene un método update, el cual se encarga de actualizar la posición del elemento en la pantalla, en función al rebote que tiene 
cada uno de los elementos con los bordes de la pantalla y en función a la velocidad que tiene cada elemento en los ejes x e y antes de la colisión con los bordes.

El último método que tiene esta clase es un método llamado render el cual se encargar de dibujar el elemento en la pantalla, en función a la posición que tiene el elemento en la pantalla y el 
color que tiene asignado. Lo anterior lo hacer en base a su posición y radio ya asignados anteriormente.

También es importante mencionar que el código cuenta con una función main, la cual se encarga de poder inicializar el código y de trabajar toda la lógica que el código requiera para poder 
funcionar.

El método main realiza lo siguiente:

1. SDL_Init(SDL_INIT_VIDEO): Inicializa la librería SDL2 para poder trabajar la interfaz gráfica.
2. SDL_CreateWindow: Crea una ventana con el nombre "Screensaver" y con las dimensiones de la pantalla definidas en el código (SCREEN_WIDTH y SCREEN_HEIGHT).
3. SDL_CreateRenderer: Crea un renderizador para poder dibujar los elementos en la ventana creada anteriormente.
4. Ciclo de configuración: 
    - En este ciclo lo que se hace es solicitar la cantidad de elementos que el usuario desea ver en la pantalla. 
    - Se revisa que el usuario haya ingresado datos válidos, o sea que haya ingresado un número entero positivo y que no haya ingresado letras o que no se haya ingresado nada.
5. Creación de elementos: 
    -  Para poder inicializar correctamente los elementos, se usa la función srand(time(nullptr)) la cual ayuda a inicializar la semilla de generación de números pseudoaleatorios.
6. Ciclo principal del Screensaver: 
    - Este ciclo lo que permite es poder mantener la ventana abierta y el ciclo principal ejecutándose hasta que el usuario desee cerrar la misma.
    - Aquí también se manejan eventos para poder detectar la acción del usuario de cerrar la ventana.
7. Actualización y renderización de los elementos: 
    - En esta parte del código se borra el renderizador y se llena con un color negro para poder limpiar la vetana en cada ciclo.
    - Por otro lado, también se actualiza y se renderizan los elementos en el arreglo elements.
8. Presentación de la ventana: 
    - Aquí se enseña el renderizado de todos los elementos del programa en la ventana.
9. Cálculo e impresión de FPS: 
    - En este trozo del código lo que se hace es calcular el número de FPS en cada momento de la ejecución de la ventana y se imprime el resultado en la consola.
10. Limitación de velocidad de actualización: 
    - En esta parte se calcula el tiempo transcurrido en el ciclo y se agrega un poco de demora para mantener bien la velocidad de los FPS dentro de los límites posibles.
11. Liberación de memoria y cierre del SDL:
    - Aquí, en esta parte final, lo que se hace es que, luego de que se sale del ciclo principal, se liberan los elementos creados y se cierra el renderizador y la ventana.

## screensaver_paralelo

En este código se presentan las mismas características que el screensaver_secuencial implementado en C++, pero en este caso se uso también la libreria de OpenMP para poder paralelizar el código y,
a su vez, optimizarlo en rendimiento en cara a los FPS. El presente código presenta igualmente la funcionalidad de crear y renderizar los elementos en la pantalla.

### Características

- Generación de círculos con propiedades aleatorias. (En este caso velocidades y colores aleatorios).
- Movimiento de los elementos (o sea los circulos) con rebote en las paredes.
- Medición de los cuadros por segundo (FPS) en tiempo real.
- Ejecución en modo secuencial y en modo paralelo para la comparacion del rendimiento en FPS.

### Requisitos

- SDL2 para la representación gráfica de los elementos.
- OpenMP para la paralelizacion del código.

### Uso

1. Compilar el programa utilizando un compilador de C++ (g++ o clang++) que admita OpenMP. Para la ejecución se debe utilizar el siguiente comando:
    g++ -o screensaver_paralelo screensaver_paralelo.cpp `sdl2-config --cflags --libs` -fopenmp
2. Ejecutar el programa con el siguiente comando:
    ./screensaver_paralelo
3. Ingresar la cantidad de elementos que se desean ver en la pantalla. (Esta debe ser mayor que 0).
4. Se puede cerrar el programa haciendo clic en la X de la ventana o haciendo Ctrl + C en la terminal.

### Referencias
- Paralelismo en pipeline execution con OpenMP: https://stackoverflow.com/questions/44169351/employing-parallelism-in-pipelined-execution
- Uso de taskyield para permitir que otras tareas trabajen en el equipo: https://www.openmp.org/spec-html/5.0/openmpsu49.html

### Puntos extra

1. En la clase 'Element', la alineaciòn de memoria se configurò en 64 bytes.
2. Se usó la optimizaciòn del uso de memoria compartida en el 'main' con directivas OpenMP (#pragma omp parallel for shared(elements))
3. Liberación de memoria optimizada usando #pragma omp task.
4. Se usaron mecanismos de pragma no vistos en clse, como #pragma omp taskyield en conjunto con #pragma omp parallel for shared(elements).
