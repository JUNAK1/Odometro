/*ODOMETRO CON ky-040                                                                                                                                                  *                                                                                                                                                                  *
*Juan Camilo Serrano Correa                          
 juancamiloserranocorrea@gmail.com 
 https://youtu.be/-JA2bEjkZhQ*/
 
/*ENCODER KY-040
  GND=GND
  +=5V
  SW=SIN USO
  DT=DIGITAL 2
  CLX=DIGITAL 4*/
#include <LiquidCrystal.h>  //Libreria para la utilizacion de la pantalla LCD  
LiquidCrystal lcd(8, 3, 10, 5, 6, 7);  // Pines RS, E, D4, D5, D6, D7,, vo=9 (VSS-RW-K a tierra), (VDD-A a 5v)  de modulo 1602A
int vo = 9; // Salida PWM
int de = 150; //Retardo para animacion de odometro
float t = 0; // Variable de control de animacion
float sensitivity = 2.59496; //2.59496cm

int DT = 2; 
int CLX = 4;
float distancia = 0; //Distancia en sistema SI
float distancia2 = 0;// Distancia en sistema americano

int PAST = 1; //Diferente valor de posicion para poder entrar en el if del void loop
volatile int NOW = 0; //variable global

void setup() {
  lcd.begin(16, 2);
  pinMode(vo, OUTPUT);
  analogWrite(vo, 120); //control de intesidad luminosa de la pantalla LCD por medio de señales PWM

  pinMode(DT, INPUT);
  pinMode(CLX, INPUT);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(DT), encoder, LOW);  //
  Serial.println("ODOMETRO");

  for (t = 0; t < 8; t += 2) { //Breve presentacion del odometro
    lcd.setCursor(t, 0);
    lcd.print("ODOMETRO");
    delay(de);
    lcd.clear();
  }
  for (t = 8; t > 0; t -= 2) {
    lcd.setCursor(t, 0);
    lcd.print("ODOMETRO");
    delay(de);
    lcd.clear();
  } lcd.clear();
  //  lcd.setCursor(0, 0);
  // lcd.print("ODOMETRO");  
}

void loop() {

  if (NOW != PAST) { //control de distancia; evita que un dato se lea dos veces.
    /*Serial.print(POSICION); // imprime el numero de veces que pasa de 0-1, 20 = 1 vuelta
      Serial.print("\t "); */
    PAST = NOW;
    distancia = NOW * sensitivity; //se multiplica por la circuferencia de la rueda en cm
   distancia2 = NOW * sensitivity; // (2.pi.r)/20=2.59496cm;
    if (distancia < 100) { // conversion automatica de cm-ft a m-yd a km-milla, a conveniencia
      distancia2 = distancia2 / 30.48;

      lcd.setCursor(0, 0);
      lcd.print(distancia2);
      lcd.print("ft");

      lcd.setCursor(0, 1);
      lcd.print(distancia);
      lcd.print("cm");
    }
    else if (distancia < 100000) {
      distancia = distancia / 100;
      distancia2 = distancia2 / 91.44;

      lcd.setCursor(0, 0);
      lcd.print(distancia2);
      lcd.print("yd");

      lcd.setCursor(0, 1);
      lcd.print(distancia);
      lcd.print("m");
    } else {
      distancia = distancia / 10000;
      distancia2 = distancia2 / 160934.4;

      lcd.setCursor(0, 0);
      lcd.print(distancia2);
      lcd.print("milla");

      lcd.setCursor(0, 1);
      lcd.print(distancia);
      lcd.print("km");
    }
  }
}

void encoder() {//Rutina anti rebote y conteo hacia delante y atras.

  static unsigned long delayy = 0;
  unsigned long tiempo = millis();

  if (tiempo - delayy > 5) {
    if (digitalRead(CLX) == HIGH) {
      NOW++ ;
    }
    else {
      NOW-- ;
    } 
    /* int sensorVal = digitalRead(12); //Opcion 2 de reinicio de conteo.
  if (sensorVal == LOW) {
    NOW = 0;
  }*/
    lcd.clear();
    delayy = tiempo;
  }
}
