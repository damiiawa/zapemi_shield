#include "Zapemi_ShieldV1.h"

/*
    <IMPORTACIÓN DE LIBRERÍAS>
    Las librerías se encuentran en el
    USB, Google Drive de Zapemi 
    y Github de Damián Astroza.
*/

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <NewPing.h>
#include <Servo.h>

/*
    <DEFINICIÓN DE CONEXIONES>
    Estas definiciones son válidas para
    Zapemi Shield V1 y V2. Recordar seguir
    diagrama de conexiones ubicado en el
    USB, Google Drive de Zapemi
    y Github de Damián Astroza.
*/

/* Considerando L9110s */
#define A1B 3
#define A1A 11
#define B1B 6
#define B1A 5

/* Considerando HC-SR04 */
#define TRIGGER  13 
#define ECHO     12 
#define DISTANCIA_MAX 300
NewPing sonar_(TRIGGER, ECHO, DISTANCIA_MAX);

/* Considerando Servomotores metálicos 180° */
Servo Si; // Arriba y abajo ( 5° a 85° )
Servo No; // Derecha e izquierda ( 5° a 175° )
Servo Golpe; // Garra de lado a lado ( 5°; 90° ; 180° [MAP: -1 ; 0 ; 1] )

/* Considerando OLED 0.98" */
#define OLED_ANCHO 128
#define OLED_ALTO 64
#define OLED_RESET -1
#define OLED_ADDR 0x3C 

Adafruit_SSD1306 display(OLED_ANCHO, OLED_ALTO, &Wire, OLED_RESET);

/* Considerando TCRT5000 en digital con potenciómetro */
#define DIGITALR_IZQUIERDA 10
#define DIGITALR_CENTRO 4
#define DIGITALR_DERECHA 2

/* Considerando TCRT5000 en analógico con potenciómetro */
#define ANALOGR_IZQUIERDA A2
#define ANALOGR_CENTRO A1
#define ANALOGR_DERECHA A0

/*
    <VARIABLES GLOBALES>
    Funcionamiento de estados y
    almacenamiento de datos. Revisa
    la información en la wiki del Github
    de Damián Astroza.
*/

int estadoMovimiento = 0;
int estadoServoGolpe = 0;

/*
    <FUNCIONES PARA SETUP>
    Divididos para ahorro de energía
    y eficiencia educacional en el código.
*/

void usarMotores(){
    pinMode(A1A, OUTPUT);
    pinMode(A1B, OUTPUT);
    pinMode(B1A, OUTPUT);
    pinMode(B1B, OUTPUT);
    digitalWrite(A1A, LOW);
    digitalWrite(B1A, LOW);
}

void usarCabeza(){
    Si.attach(7);
    No.attach(8);
}

void usarGolpe(){
    Golpe.attach(9);
}

void usarPantalla(){
    if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println(F("No se encontró módulo de pantalla."));
        for(;;); 
  }
  display.clearDisplay();
  escribir(0,0,"Zapemi SpA - Temuco, Chile");
  delay(1500);
}

void usarSensores(){
    pinMode(DIGITALR_IZQUIERDA, INPUT);
    pinMode(DIGITALR_CENTRO, INPUT);
    pinMode(DIGITALR_DERECHA, INPUT);

    pinMode(ANALOGR_IZQUIERDA, INPUT);
    pinMode(ANALOGR_CENTRO, INPUT);
    pinMode(ANALOGR_DERECHA, INPUT);
}

void monitor(){
    Serial.begin(9600);
}


/*
    <MOVIMIENTOS DEL ROBOT>
    Funciones para desplazamientos.
    (Utilizando 2 ruedas)
*/


void avanzar(int velIzq, int velDer){ // Velocidades independientes

    if (estadoMovimiento != 1)
    {
        detener();
        estadoMovimiento = 1;
    }

    digitalWrite(A1A, HIGH);
    digitalWrite(B1A, HIGH);
    analogWrite(A1B, 255 - velIzq);
    analogWrite(B1B, 255 - velDer);
}

void avanzar(int vel){ // Velocidades "iguales"

    if (estadoMovimiento != 5)
    {
        detener();
        estadoMovimiento = 5;
    }

    digitalWrite(A1A, HIGH);
    digitalWrite(B1A, HIGH);
    analogWrite(A1B, 255 - vel);
    analogWrite(B1B, 255 - vel);
}

void retroceder(int velIzq, int velDer){ // Velocidades independientes

    if (estadoMovimiento != 6)
    {
        detener();
        estadoMovimiento = 6;
    }

    digitalWrite(A1B, HIGH);
    digitalWrite(B1B, HIGH);
    analogWrite(A1A, 255 - velIzq);
    analogWrite(B1A, 255 - velDer);
}

void retroceder(int vel){ // Velocidades "iguales"

    if (estadoMovimiento != 2)
    {
        detener();
        estadoMovimiento = 2;
    }

    digitalWrite(A1B, HIGH);
    digitalWrite(B1B, HIGH);
    analogWrite(A1A, 255 - vel);
    analogWrite(B1A, 255 - vel);
}

void izquierda(int vel){
    
    if (estadoMovimiento != 4)
    {
        detener();
        estadoMovimiento = 4;
    }

    digitalWrite(A1B, HIGH);
    digitalWrite(B1A, HIGH);
    analogWrite(A1A, 255 - vel);
    analogWrite(B1B, 255 - vel);
}

void derecha(int vel){
    
    if (estadoMovimiento != 3)
    {
        detener();
        estadoMovimiento = 3;
    }

    digitalWrite(A1A, HIGH);
    digitalWrite(B1B, HIGH);
    analogWrite(A1B, 255 - vel);
    analogWrite(B1A, 255 - vel);
}

void detener(){
    digitalWrite(A1A, LOW);
    digitalWrite(A1B, LOW);
    digitalWrite(B1A, LOW);
    digitalWrite(B1B, LOW);

    delay(70);
    estadoMovimiento = 0;
}


/*
    <LECTURA DE SENSORES>
    Ultrasonido y reflexión de luz.
*/

int sensorIzquierdo(){
    return digitalRead(DIGITALR_IZQUIERDA);
}

int sensorCentro(){
    return digitalRead(DIGITALR_CENTRO);
}

int sensorDerecho(){
    return digitalRead(DIGITALR_DERECHA);
}

int sensorIzquierdoA(){
    return analogRead(ANALOGR_IZQUIERDA);
}

int sensorCentroA (){
    return analogRead(ANALOGR_CENTRO);
}

int sensorDerechoA(){
    return analogRead(ANALOGR_DERECHA);
}

int medirDistancia(){

    int dist =  sonar_.ping_cm();

    return dist;
}


/*
    <CONTROL DE SERVOMOTORES>
    Es probable que el "servoSi" presente problemas
    por ajustes de tornillería.
*/

void servoSi(int pos) {
  Si.write(pos);
}

void servoNo(int pos) {
  No.write(pos);
}

void servoGolpe(int pos){

    if (estadoServoGolpe == 0) {
        if (pos == -1) {
			Golpe.write(165);
			estadoServoGolpe = -1;
        } else if (pos == 1) {
			Golpe.write(15);
			estadoServoGolpe = 1;
		}
    } else if(estadoServoGolpe == -1) {
		if (pos == 0){
			Golpe.write(90);
			estadoServoGolpe = 0;
		} else if (pos == 1) {
			Golpe.write(15);
			estadoServoGolpe = 1;
		}	
	} else if(estadoServoGolpe == 1) {
		if (pos == 0){
			Golpe.write(90);
			estadoServoGolpe = 0;
		} else if (pos == -1) {
			Golpe.write(165);
			estadoServoGolpe = -1;
		}	
	}

}

/* 
    <FUNCIONES DE PANTALLA>
    Considerando básicos para OLED 0.98"
*/

void escribir(int xpos, int ypos, const char *texto) {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(xpos, ypos * 10);
  display.println(texto);
  display.display();
}

void escribir(int xpos, int ypos, float valor) {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(xpos, ypos * 10);
  display.println(valor);
  display.display();
}

void borrar() {
    display.clearDisplay();
}

/*
    <ACCIONES>
    Acciones simples y rápidas.
*/

void apagar(){

	detener();
	borrar();

	while(1){
	
	}

}

void pausa(int tiempo){

	delay(tiempo);

};
