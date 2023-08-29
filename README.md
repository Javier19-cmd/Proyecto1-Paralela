# Proyecto 1 de Computación Paralela y Distribuída: Realizar un screensaver secuencial y paralelo.

### Descripción general del código secuencial.

Este proyecto consta de dos códigos, un código secuencial (screensaver_secuencial) y un código paralelo (screensaver_parelelo).

## screensaver_secuencial

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

Este código al igual que el screensaver_secuecial tiene una clase Element y un método main.

La clase Element posee las mismas caracterísitcas y lógica que el código secuencial.

El método main contiene la misma lógica que el código secuencial, sin embargo, se hicieron algunas modificaciones para optimizar el código. Lo anterior se logró hacer de la siguiente manera: 

1. Se generó una sección paralela, para que así se pueda generar de manera más rápida y eficiente la creación de los elementos a renderizar en la pantalla.

2. La parte de actualización se generó de manera paralela al igual que el renderizado de los objetos. Para realizar esto, se usaron las instrucciones de update y de render, con el objetivo de agilizar las operaciones en pantalla.

3. Por último, se generó una sección en la cual se calcula el speedup y la eficiencia del programa. Lo anterior se hace luego de que la ejecución termina por completo.
