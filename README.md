# Proyecto 1 Computación Paralela y Distribuída

### Descripción general del código secuencial.

El presente código secuencial consta de una clase Element, la cual se encarga de hacer posible la representación de los elementos en la pantalla que el usuario verá para el screensaver.
Los elementos que representan esta clase son principalmente círculos, los cuales tienen una posición, un radio y velocidades en los ejes x e y. Además, cada elemento tiene un color, 
el cual es asignado de manera aleatoria al momento de su creación. 

Por otro lado, es importante mencionar que la clase elemento tiene un método update, el cual se encarga de actualizar la posición del elemento en la pantalla, en función al rebote que tiene 
cada uno de los elementos con los bordes de la pantalla y en función a la velocidad que tiene cada elemento en los ejes x e y antes de la colisión con los bordes.

El último método que tiene esta clase es un método llamado render el cual se encargar de dibujar el elemento en la pantalla, en función a la posición que tiene el elemento en la pantalla y el 
color que tiene asignado. Lo anterior lo hacer en base a su posición y radio ya asignados anteriormente.

También es importante mencionar que el código cuenta con una función main, la cual se encarga de poder inicializar el código y de trabajar toda la lógica que el código requiera para poder 
funcionar. 