/*
  Switches y detección de flancos:
  Se desea saber cuándo los switches deslizantes
  van de ON(1) a OFF(0) y de OFF(0) a ON(1).
  Es decir, detectar cuándo pasan de
  +5V a GND y viceversa.
  Se llama detección de cambio de estado
  o detección de flancos.
  Se debe utilizar este tipo de detecciones
  para simular los switches como sensores IR y
  comprobar si un auto que pasa en medio de los
  sensores IR entra o sale.

  LED:
  Los LED rojos muestran el estado de cada switch (1 o 0)

  Secuencias y Estados de los switches y del sistema:
  Entrar está representado por
  la secuencia de estados 10, 11, 01, 00.
  Salir, por la secuencia de estados 01, 11, 10, 00.
  El 1er bit del estado del sistema (izquierdo) está dado por el
  Switch 1, y representa el estado del 1er sensor IR(1 o 0).
  Lo mismo con el bit 2 del estado del sistema (derecho)
  La variable 'estado' marca el estado de el sistema
  conformado por ambos switches y es igual a bit2 + bit1
  corrido una posición a la izquierda (bit1<<1)
  La variable 'secuenciaEntrada' es un arreglo de estos
  estados que marca la entrada exitosa de un auto
  y es igual a {10,11,01,00}
  La variable 'secuenciaSalida' marca la salida
  y es igual a {01,11,10,00}
  En el momento en que una secuencia de estados obedezca
  (sea igual a) uno de estas 2 secuencias, se dice que
  un auto ha entrado o salido con éxito.
  La variable 'secuencia' es la secuencia actual, es decir
  guarda los estados uno tras otro mientras dezplaza
  los anteriores a posiciones hacia la izquierda en el arreglo.
  En otras palabras, cuando un switch cambia de estado,
  el estado del sistema también cambia, y la variable
  'secuencia' se ajusta así:
  secuencia[0] = secuencia[1]
  secuencia[1] = secuencia[2]
  secuencia[2] = secuencia[3]
  secuencia[3] = estado

  El circuito:
  * Los cables negros indican conexión a GND.
  * Los cables rojos indican conexión a +5V.
  * Los verdes, entrada de datos.
  * Los azules, salida de datos.
  * La salida de switch 1 va al pin 12, y sus entradas
  van a +5V y a GND.
  * La salida de switch 2 va al pin 2, y sus entradas
  van a +5V y a GND.
  * Pines 3 y 11 de escritura van a los ánodos de los LED.
  * 2 Resistencias 220 conectadas a los cátodos de los LED
  y a tierra.
  * Los switches comienzan conectados a GND marcando 00.
*/
// Librería que maneja los servomotores
#include <Servo.h>
// VARIABLES GLOBALES
// Servomotores (palanquera a la entrada y a la salida)
Servo servo1;
Servo servo2;
// Pines de los switches y leds, estados actuales y anteriores
// de los switches para detectar el cambio de estado
// Switch 1
int pinSwitch1 = 12;	
int led1 = 11;
int switch1 = 0;
int lastSwitch1 = 0;
// Switch 2
int pinSwitch2 = 9;
int led2 = 8;
int switch2 = 0;
int lastSwitch2 = 0;
// Switch 3
int pinSwitch3 = 6;
int led3 = 7;
int switch3 = 0;
int lastSwitch3 = 0;
// Switch 4
int pinSwitch4 = 3;
int led4 = 4;
int switch4 = 0;
int lastSwitch4 = 0;
// Estado inicial es 00.
// O sea, estamos en el estado en que ningún carro ha pasado
// por ningún acceso.
int estado = 0;
// La secuencia de referencia se comparará con las de entrada y salida:
int secuenciaEntrada[] = {0,0,0,0};
int secuenciaSalida[] = {0,0,0,0};
int secuenciaReferencia[] = {B10,B11,B01,B00};

void setup()
{
  Serial.begin(9600);
  // Pines de switches como entrada
  pinMode(pinSwitch1, INPUT);
  pinMode(pinSwitch2, INPUT);
  pinMode(pinSwitch3, INPUT);
  pinMode(pinSwitch4, INPUT);
  // Pines de LEDs como salida
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  // Pines de los servos y ángulos iniciales
  servo1.attach(10);
  servo2.attach(5);
  servo1.write(0);
  servo2.write(0);
  }

void loop()
{
  // Leer el estado actual de los Switches.
  switch1 = digitalRead(pinSwitch1);
  switch2 = digitalRead(pinSwitch2);
  switch3 = digitalRead(pinSwitch3);
  switch4 = digitalRead(pinSwitch4);
  // LEDs ON/OFF según el valor de los switches.
  digitalWrite(led1, switch1);
  digitalWrite(led2, switch2);
  digitalWrite(led3, switch3);
  digitalWrite(led4, switch4);
  
  // Comparar los estados actuales de los switches con los anteriores.
  // Si son diferentes, hay cambio de estado (detección de flanco).
  if ((switch1 != lastSwitch1) || (switch2 != lastSwitch2)) {
    // Poner un pequeño delay para evitar rebote por cambio de estado.
    delay(5);
    estado = (switch1<<1)|switch2;
    // Imprimir en binario para ver los estados.
    Serial.println(estado,BIN);
    actualizarSecuencia(estado,secuenciaEntrada);
  } else if ((switch3 != lastSwitch3) || (switch4 != lastSwitch4)) {
  	// Poner un pequeño delay para evitar rebote por cambio de estado
    delay(5);
    estado = (switch3<<1)|switch4;
    // Imprimir en binario para ver los estados.
    Serial.println(estado,BIN);
    actualizarSecuencia(estado,secuenciaSalida);
  }
  
  if(array_cmp(secuenciaEntrada,secuenciaReferencia)){
    Serial.println("ENTRADA EXITOSA");
    for (int pos = 0; pos <= 90; pos += 1) {
      // tell servo to go to position in variable 'pos'
      servo1.write(pos);
      // wait 15 ms for servo to reach the position
      delay(15); // Wait for 15 millisecond(s)
    }
    secuenciaEntrada[3] = 5;
  }
  else if(array_cmp(secuenciaSalida,secuenciaReferencia)){
    Serial.println("SALIDA EXITOSA");
    for (int pos = 0; pos <= 90; pos += 1) {
      // tell servo to go to position in variable 'pos'
      servo2.write(pos);
      // wait 15 ms for servo to reach the position
      delay(15); // Wait for 15 millisecond(s)
    }
    secuenciaSalida[3] = 5;
  }
  // Guarda la lectura actual de los pines como la anterior,
  // para la próxima iteración del ciclo.
  lastSwitch1 = switch1;
  lastSwitch2 = switch2;
  lastSwitch3 = switch3;
  lastSwitch4 = switch4;
  
}

// FUNCIONES

boolean array_cmp(int *a, int *b){
  // Devuelve true si dos arreglos de igual longitud son iguales.
  for (int n=0;n<4;n++){if (a[n]!=b[n]){return false;}}
  return true;
}

void actualizarSecuencia(int estado,int secuencia[]){
  // Esta función actualiza la secuencia de entrada y la de salida
  // Al guardar el 'estado' en la 'secuencia'
  secuencia[0] = secuencia[1];
  secuencia[1] = secuencia[2];
  secuencia[2] = secuencia[3];
  secuencia[3] = estado;
  /*
  // Imprimir la secuencia
  Serial.println("Secuencia:");
  Serial.println(secuencia[0],BIN);
  Serial.println(secuencia[1],BIN);
  Serial.println(secuencia[2],BIN);
  Serial.println(secuencia[3],BIN);
  */
}
