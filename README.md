# educational-robot-remote-chat #

> RC robot w/ chat capabilies for educational use  

* * *

## Resumen ##

Robot interactivo diseñado por los alumnos de la actividad de robótica [Taller
de Inventores](http://tallerinventores.github.io/). Ha sido desarrollado y
optimizado para impresión 3D y pensando en su bajo coste y facilidad de montaje
y uso.

## Instalación ##

En esta sección se cubre la instalación de software y hardware para el montaje
recomendado, mediante [Arduino](http://www.arduino.cc/).

### Requerimientos de software ###

Para poser usar Arduino (v1.6+) con el código fuente proporcionado, son
necesarias las librerías [NewPing](http://playground.arduino.cc/Code/NewPing) y
[NewLiquidCrystal](https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads).
Para generar los archivos aptos para la impresora 3D desde los archivos `.stl`
será necesario adicionalmente un software para esta función, como puede ser
[Slic3r](http://slic3r.org/).

### Requerimientos de hardware ###

Será necesario para su montaje y uso cuatro pilas tipo AA, una pantalla LCD de
dieciséis por dos con interfaz I2C, un sensor de distancia por ultrasonidos tipo
HC-SR04, seis LED de colores, seis resistencias de 330Ω, una de 2,7kΩ, un
transistor BC547, un conjunto de motor Tamiya Twin-Motor y cuatro ruedas Tamiya
Off-road, una placa Robgednaut Zond con un Arduino Nano o compatible y un módulo
de bluetooth HC05. Adicionalmente harán falta tornillos y arandelas de métrica
tres. Se recomienda un macho de misma métrica.

## Uso ##

Instrucciones de montaje y puesta en marcha.

### Montaje del robot ###

1.  Imprimir una unidad de los archivos `chassis.stl` y `base.stl`. Imprimir
    cuatro unidades de `spacer.stl`. Puede ser necesario repasar las piezas
    impresas con el fin de eliminar rebabas.  
2.  Montar la pantalla LCD sobre la parte superior del robot (_chassis_) usando
    cuatro tornillos de métrica tres por doce con arandela, insertados de la
    parte plástica hacia la pantalla, y asegurados con una tuerca cada uno.  
3.  Montar los LED en sus orificios y fijarlos con plástico termofusible si
    fuera necesario.  
4.  Montar el sensor de distancia en el frontal del chasis.  
5.  Presentar el portapilas sobre la parte interior de la parte inferior del
    robot (_base_) y alinear los agujeros de fijación, usar dos tornillos de
    métrica tres por seis, roscando sobre el la estructura del portapilas.  
6.  Montar el motor en la parte externa de la base, y fijarlo mediante dos
    tornillos de métrica tres por seis sobre la rosca que se realiza en sus
    agujeros correspondientes, de forma que quede alineado el eje motriz con el
    centro del hueco de las ruedas.  
7.  Realizar la gestión de cables del motor y del sensor de distancia para que
    entren al cuerpo del robot a través del agujero destinado a ello.  
8.  Montar las ruedas traseras en los ejes del motor.  
9.  Posicionar los ejes delanteros en los agujeros de la base, y montar un
    anillo de retención a cada extremo con tornillos de métrica tres por seis.
    Colocar las ruedas delanteras.  
10. Realizar todas las uniones eléctricas.  
11. Cerrar el robot con tornillos de métrica tres por treinta con arandela,
    añadiendo un espaciador (_spacer_) en cada tornillo entre ambas partes
    principales del robot. Roscar dichos tornillos sobre la rosca previamente
    hecha en los orificios para este propósito de la mitad superior.

### Conexión eléctrica ###

Este proyecto usa [Robgednaut](http://akornsys-rdi.github.io/robgednaut/) Zond
como placa de conexionado, si no dispones de ella puedes ver su esquema de
conexiones en la página oficial.  

Las alimentaciones para todos los dispositivos las provee la placa de control.
Los cables de señal de la pantalla SDA y SCL están conectados a C y D
respectivamente. Las señales del sensor de distancia por ultrasonidos ECHO y
TRIGGER están conectadas a B y A respectivamente. Cada motor está conectado a un
canal de la salida para motores, respetando su polaridad.  

Los LED del robot llevan una resistencia de 330Ω en el ánodo, todos ellos
conectados entre sí y al pin de alimentación de la pantalla. Los cátodos van al
colector del transistor. El emisor está conectado a la masa de la pantalla, y la
base va conectada al pin del expansor I2C que controla la retroiluminación de la
pantalla LCD a través de una resistencia de 2,7kΩ.  

### Firmware ###

El Arduino deberá tener cargado el firmware de control que se incluye en la
carpeta `src` de este repositorio.  

### Software ###

Con la comunicación bluetooth establecida (elementos emparejados), abrir una
aplicación terminal en el dispositivo de control (por ejemplo cutecom) y 
escribir la frase o comando que se desee enviar, finalizada por el caracter
retorno de carro (`0x13`).  

### Bugs ###

Se han observado los siguientes problemas:  

-   Debido a todo el hardware que va dentro del robot, han sido necesarios
    añadir unos separadores entre el chasis y la base.  
-   El interruptor de encendido/apagado es difícil de accionar al quedar dentro
    de la estructura.  
-   Enviar palabras sueltas que no sean comandos, parecen no funcionar.  
-   El software de control provoca al inicio movimientos extraños.  

## Enlaces externos ##

Este proyecto también está presente en:  

-   [Thingiverse](http://www.thingiverse.com/thing:1656508).  

## Créditos ##

Proyecto realizado por Taller de Inventores 2016, representado por:  

-   kwendenarmo <kwendenarmo@akornsys-rdi.net>  

Alumnos del Centro de Formación Padre Piquer:  

-   G, Ayman El-  
-   S, Rubén  
-   S, Sergio  
