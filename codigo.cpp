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
// Switch 1
int switch1Pin = 12;		// Pin Switch 1
int led1Pin = 11;			// Pin LED 1
int switch1State = 0;		// Estado actual del Switch 1
int lastSwitch1State = 0;	// Estado anterior de S1
int bit1 = B0;				// Valor bit1 del estado
// Switch 2
int switch2Pin = 9;			// Pin Switch 2
int led2Pin = 8;			// Pin LED 2
int switch2State = 0;		// Estado actual del Switch 2
int lastSwitch2State = 0;	// Estado anterior de S2
int bit2 = B0;				// Valor bit2 del estado
// Switch 3
int switch3Pin = 9;			// Pin Switch 3
int led3Pin = 8;			// Pin LED 3
int switch3State = 0;		// Estado actual del Switch 3
int lastSwitch3State = 0;	// Estado anterior de S3
int bit3 = B0;				// Valor bit3 del estado
// Switch 4
int switch4Pin = 9;			// Pin Switch 4
int led4Pin = 8;			// Pin LED 4
int switch4State = 0;		// Estado actual del Switch 4
int lastSwitch4State = 0;	// Estado anterior de S4
int bit4 = B0;				// Valor bit4 del estado
// Con ambos bits = 0, el estado inicial es 00.
// O sea, estamos en el estado en que ningún carro se ha asomado
int estado = (bit1<<1)+bit2;
// Y con el primer estado configurado, la secuencia es:
int secuencia[] = {0,0,0,estado};
// La secuencia se comparará con las de entrada y salida:
int secuenciaEntrada[] = {B10,B11,B01,B00};
int secuenciaSalida[] = {B01,B11,B10,B00};

void setup()
{
  Serial.begin(9600);
  pinMode(switch1Pin, INPUT);	// Pin de switch 1 como entrada
  pinMode(switch2Pin, INPUT);	// Pin de switch 2 como entrada
  pinMode(led1Pin, OUTPUT);		// Pin de led 1 como salida
  pinMode(led2Pin, OUTPUT);		// Pin de led 2 como salida
  servo1.attach(10);			// Pin Servo 1
  //rvo2.attach(10);			// Pin Servo 2
  servo1.write(5);				
  /*
  Serial.println("Secuencia de Entrada:");
  Serial.println(secuenciaEntrada[0],BIN);
  Serial.println(secuenciaEntrada[1],BIN);
  Serial.println(secuenciaEntrada[2],BIN);
  Serial.println(secuenciaEntrada[3],BIN);
  */
  }

void loop()
{
  // Leer el estado actual de los Switches o pines.
  switch1State = digitalRead(switch1Pin);
  switch2State = digitalRead(switch2Pin);
  
  // Comparar los estados actuales de los switches con los anteriores.
  // Si son diferentes, hay cambio de estado (detección de flanco).
  if ((switch1State != lastSwitch1State) || (switch2State != lastSwitch2State)) {
    if (switch1State == HIGH) {
      // Si el estado actual del switch1 es HIGH,
      // el switch pasó de 0V a +5V.
      bit1 = B1;
    } else {
      // Si el estado actual está en OFF, entonces
      // el switch pasó de ON a OFF.
      bit1 = B0;
    }
    if (switch2State == HIGH) {
      // Si el estado actual del switch2 es HIGH,
      // el switch pasó de 0V a +5V.
      bit2 = B1;
    } else {
      // Si el estado actual está en OFF, entonces
      // el switch pasó de ON a OFF.
      bit2 = B0;
    }
    // 'estado' es la concatenación de los bits.
    // Es decir, estado del sistema es la unión de los estados de los switches.
    estado = (bit1<<1)+bit2;
    // Imprimir en binario para ver los estados.
    //Serial.println(estado,BIN);
    // Poner un pequeño delay para evitar rebote.
    delay(5);
    // Guarda el 'estado' en el historial de estados, o sea, en 'secuencia'
    secuencia[0] = secuencia[1];
    secuencia[1] = secuencia[2];
    secuencia[2] = secuencia[3];
    secuencia[3] = estado;
/*    // Imprimir la secuencia
	Serial.println("Secuencia:");
    Serial.println(secuencia[0],BIN);
    Serial.println(secuencia[1],BIN);
    Serial.println(secuencia[2],BIN);
    Serial.println(secuencia[3],BIN);
*/
    if(array_cmp(secuencia,secuenciaEntrada)){
      Serial.println("ENTRADA EXITOSA");
      for (int pos = 0; pos <= 90; pos += 1) {
        // tell servo to go to position in variable 'pos'
        servo1.write(pos);
        // wait 15 ms for servo to reach the position
        delay(15); // Wait for 15 millisecond(s)
      }
    }
    else if(array_cmp(secuencia,secuenciaSalida)){
      Serial.println("SALIDA EXITOSA");
      for (int pos = 90; pos >= 0; pos -= 1) {
        // tell servo to go to position in variable 'pos'
        servo1.write(pos);
        // wait 15 ms for servo to reach the position
        delay(15); // Wait for 15 millisecond(s)
      }
    }
  }
  // Guarda la lectura actual de los pines como la anterior,
  // para la próxima iteración del ciclo.
  lastSwitch1State = switch1State;
  lastSwitch2State = switch2State;
  // LEDs ON/OFF según el valor de los switches.
  digitalWrite(led1Pin, bit1);
  digitalWrite(led2Pin, bit2);
}

// FUNCIONES
boolean array_cmp(int *a, int *b){
  for (int n=0;n<4;n++){if (a[n]!=b[n]){return false;}}
  return true;  
}
