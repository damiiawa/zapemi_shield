#ifndef Zapemi_ShieldV1_h
#define Zapemi_ShieldV1_h

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>

extern Adafruit_SSD1306 display;
extern Servo Si;
extern Servo No;
extern Servo Golpe;

void usarMotores();
void usarCabeza();
void usarGolpe();
void usarPantalla();
void usarSensores();
void monitor();
void avanzar(int velIzq, int velDer);
void avanzar(int vel);
void retroceder(int velIzq, int velDer);
void retroceder(int vel);
void izquierda(int vel);
void derecha(int vel);
void detener();
int sensorIzquierdo();
int sensorCentro();
int sensorDerecho();
int sensorIzquierdoA();
int sensorCentroA();
int sensorDerechoA();
int medirDistancia();
void servoSi(int pos);
void servoNo(int pos);
void servoGolpe(int pos);
void escribir(int xpos, int ypos, const char *texto);
void escribir(int xpos, int ypos, float valor);
void borrar();
void apagar();
void pausa(int tiempo);

#endif
