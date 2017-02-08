//**************BLYNK********************
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>

char auth[] = "750de721918f4a74a9dd3fe2ac225351";
//IPAddress server_ip (10, 0, 0, 10);

// COnfiguracion de tarjeta de red
byte arduino_mac[] = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress arduino_ip ( 10,   72,   110,  200);
IPAddress dns_ip     (  10,   72,   110,   25);
IPAddress gateway_ip ( 10,   72,   110,   12);
IPAddress subnet_mask(255, 255, 0,   0);

#define W5100_CS  10
#define SDCARD_CS 4
bool isFirstConnect = true;
//********************************************

//*************RTC****************************
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;

//*********************************************
//**************DHT y temperatura************
#include "DHT.h"
#define DHTPIN 22    
#define DHTTYPE DHT22  
DHT dht(DHTPIN, DHTTYPE);
float h = 0;
float t = 0;
//*****************************************

//*********millis************************
unsigned long previousMillisTemp = 0; 
unsigned long previousMillisAlertas = 0; 
unsigned long previousMillisMail25 = 0; 
unsigned long previousMillisMail30 = 0; 
unsigned long previousMillisLunes = 0;
unsigned long previousMillisMartes = 0;
unsigned long previousMillisMiercoles = 0;
unsigned long previousMillisJueves = 0;
unsigned long previousMillisViernes = 0;
unsigned long previousMillisSabado = 0;
unsigned long previousMillisDomingo = 0;
const long interval = 1000;  
//****************************************

//**********Leds de estado y variables necesarias****************
int encendidoAmarillo = 26;
int verdeOnline = 30;
int rojoTemperatura = 34;
int reset = 14;
int AireUno = 0;
int AireDos = 0;
int TemAlta = 0;
int HoyEs = 1;
int hora = 0;
int minutos = 0;
int BloqueodeAlertas = 0;
bool primerEncendido = true;
//*****************************************

void setup()
{
  Serial.begin(9600); 
  dht.begin();  //Configura el dht
  Wire.begin(); //configura el bus I2C para RTC
  pinMode(encendidoAmarillo, OUTPUT);
  pinMode(verdeOnline, OUTPUT);
  pinMode(rojoTemperatura, OUTPUT);
  pinMode(reset, OUTPUT);
  digitalWrite(verdeOnline,LOW);
  
  //*****************Setup Blynk************************
  pinMode(SDCARD_CS, OUTPUT);
  digitalWrite(SDCARD_CS, HIGH); // Deselect the SD card
  Blynk.begin(auth, "blynk-cloud.com", 8442, arduino_ip, dns_ip, gateway_ip, subnet_mask, arduino_mac);
  //*****************************************************
}

void loop(){

  Blynk.run();
  digitalWrite(encendidoAmarillo, HIGH);
  unsigned long currentMillis = millis();
  DateTime now = RTC.now(); //obtiene datos del modulo RTC

 //**********Modulo obtener Tem y Humedad************* 
 if (currentMillis - previousMillisTemp >= 5*interval) {
   previousMillisTemp = currentMillis;
   h = dht.readHumidity();
   t = dht.readTemperature();
   Serial.print("Hum: ");
   Serial.print(h);
   Serial.print(" %\t");
   Serial.print("Tem: ");
   Serial.print(t);
   Serial.println(" *C ");
   Serial.println(previousMillisTemp/1000);
   Serial.print(AireUno);
   Serial.print(" ");
   Serial.println(AireDos);
   Serial.print(HoyEs);
   Serial.print(" ");
   Serial.print(hora);
   Serial.print(" ");
   Serial.println(minutos);
  }
//****************************************************

//*************Modulo Obtener dia y Hora********************
  HoyEs = now.dayOfTheWeek();
  hora = now.hour();
  minutos = now.minute();
//***************************************************

//*************Configuracion de primer encendido, tiene que ser entre las 6am y las 23pm con ambos aires apagados******
if (primerEncendido && hora > 6 && hora < 9){
	switch(HoyEs){
		case 1:
			AireUno = 0;
			AireDos = 1;
			//AccionarAireDos(); si se enciende el aire un lunes antes de las 9am tiene que estar el aire dos encendido 
			primerEncendido = false;
		break;
		case 2: 
			AireUno = 0;
			AireDos = 1;
			AccionarAireDos();
			primerEncendido = false;
		break;
		case 3:
			AireUno = 1;
			AireDos = 0;
			AccionarAireUno();
			primerEncendido = false;
		break;
		case 4:
			AireUno = 0;
			AireDos = 1;
			AccionarAireDos();
			primerEncendido = false;
		break;
		case 5:
			AireUno = 1;
			AireDos = 0;
			AccionarAireUno();
			primerEncendido = false;
		break;
		case 6:
			AireUno = 0;
			AireDos = 1;
			AccionarAireDos();
			primerEncendido = false;
		break;
		case 7:
			AireUno = 1;
			AireDos = 0;
			AccionarAireUno();
			primerEncendido = false;
		break;		
	}
}
if (primerEncendido && hora > 9 && hora < 23){
	switch(HoyEs){
		case 1:
			AireUno = 0;
			AireDos = 1;
			AccionarAireDos();
			primerEncendido = false;
		break;
		case 2: 
			AireUno = 1;
			AireDos = 0;
			AccionarAireUno();
			primerEncendido = false;
		break;
		case 3:
			AireUno = 0;
			AireDos = 1;
			AccionarAireDos();
			primerEncendido = false;
		break;
		case 4:
			AireUno = 1;
			AireDos = 0;
			AccionarAireUno();
			primerEncendido = false;
		break;
		case 5:
			AireUno = 0;
			AireDos = 1;
			AccionarAireDos();
			primerEncendido = false;
		break;
		case 6:
			AireUno = 1;
			AireDos = 0;
			AccionarAireUno();
			primerEncendido = false;
		break;
		case 7:
			AireUno = 0;
			AireDos = 1;
			AccionarAireDos();
			primerEncendido = false;
		break;		
	}
}
//**********************************************************************
//***********Configuracion general para cada dia************
if (!TemAlta && !primerEncendido){  //si la temperatura es alta se TemAlta = 1 => !TempAlta = 0 entonces se desactiva esta parte del algoritmo para que no estorbe
  switch(HoyEs){
    case 1: 
      if (hora == 9 && minutos == 0) {
        delay (59000);
        digitalWrite(reset, HIGH);
      }  
    break;
       case 2: //martes
          if (hora == 9 && minutos == 0) {
            if (AireUno && AireDos){     //Si hay dos aires encendidos solo apaga el que tiene que estar apagado       
              if (currentMillis - previousMillisMartes >= 70*interval) {          //Este if maneja el delay para que no se ejecute la misma porcion de codigo mas de una vez al dia
                previousMillisMartes = currentMillis;
                AccionarAireUno ();
                //AccionarAireDos();
                AireUno = 0;
                AireDos = 1;
              } 
            }
            else {
              if (currentMillis - previousMillisMartes >= 70*interval) {
                previousMillisMartes = currentMillis;
                AccionarAireUno ();
                AccionarAireDos();
                AireUno = 0;
                AireDos = 1;
              }  
            }
          }
      break;
    case 3:  //Miercoles
          if (hora == 9 && minutos == 0) {
            if (AireUno && AireDos){            
              if (currentMillis - previousMillisMiercoles >= 70*interval) {
                previousMillisMiercoles = currentMillis;
                AccionarAireDos();
                AireUno = 1;
                AireDos = 0;
              } 
            }
            else {
              if (currentMillis - previousMillisMiercoles >= 70*interval) {
                previousMillisMiercoles = currentMillis;
                AccionarAireUno ();
                AccionarAireDos();
                AireUno = 1;
                AireDos = 0;
              }  
            }
          }
      break;
    case 4: //Jueves
          if (hora == 10 && minutos == 0) {
            if (AireUno && AireDos){            
              if (currentMillis - previousMillisJueves >= 70*interval) {
                previousMillisJueves = currentMillis;
                AccionarAireUno();
                AireUno = 0;
                AireDos = 1;
              } 
            }
            else {
              if (currentMillis - previousMillisJueves >= 70*interval) {
                previousMillisJueves = currentMillis;
                AccionarAireUno ();
                AccionarAireDos();
                AireUno = 0;
                AireDos = 1;
              }  
            }
          }
      break;
    case 5:   //Viernes
          if (hora == 9 && minutos == 0) {
            if (AireUno && AireDos){            
              if (currentMillis - previousMillisViernes >= 70*interval) {
                previousMillisViernes = currentMillis;
                //AccionarAireUno ();
                AccionarAireDos();
                AireUno = 1;
                AireDos = 0;
              } 
            }
            else {
              if (currentMillis - previousMillisViernes >= 70*interval) {
                previousMillisViernes = currentMillis;
                AccionarAireUno ();
                AccionarAireDos();
                AireUno = 1;
                AireDos = 0;
              }  
            }
          }
      break;
    case 6:   //Sabado
          if (hora == 9 && minutos == 0) {
            if (AireUno && AireDos){            
              if (currentMillis - previousMillisSabado >= 70*interval) {
                previousMillisSabado = currentMillis;
                AccionarAireUno ();
                //AccionarAireDos();
                AireUno = 0;
                AireDos = 1;
              } 
            }
            else {
              if (currentMillis - previousMillisSabado >= 70*interval) {
                previousMillisSabado = currentMillis;
                AccionarAireUno ();
                AccionarAireDos();
                AireUno = 0;
                AireDos = 1;
              }  
            }
          }
      break;
    case 7:    //Domingo
          if (hora == 9 && minutos == 0) {
            if (AireUno && AireDos){            
              if (currentMillis - previousMillisDomingo >= 70*interval) {
                previousMillisDomingo = currentMillis;
                //AccionarAireUno ();
                AccionarAireDos();
                AireUno = 1;
                AireDos = 0;
              } 
            }
            else {
              if (currentMillis - previousMillisDomingo >= 70*interval) {
                previousMillisDomingo = currentMillis;
                AccionarAireUno ();
                AccionarAireDos();
                AireUno = 1;
                AireDos = 0;
              }  
            }
          }
      break;
  }
}
//******************************************************  

//***************Alertas, Email y push*******************
if (!BloqueodeAlertas){
if (currentMillis - previousMillisAlertas >= 60*interval){
    previousMillisAlertas = currentMillis;
    if (t>=25 && t<30){
      digitalWrite(rojoTemperatura, HIGH);
        if (AireUno && !AireDos){
          AccionarAireDos();
          AireDos = 1;
          TemAlta=1;
          //DosAiresEncendidos=1;
        }
        if (!AireUno && AireDos){
          AccionarAireUno();
          AireUno=1;
          TemAlta=1;
          //DosAiresEncendidos=1;
        }
        if (currentMillis - previousMillisMail25 >= 600*interval){
            previousMillisMail25 = currentMillis;            
            Serial.println("25*C Alcanzados."); 
            Blynk.email("walterwallpinto@gmail.com", "Alerta 25", "Se alcanzaron los 25C, se encendera el aire acondicionado de respaldo");
            Blynk.notify("La temperatura del datacenter alcanzo los 25*C");
        }
    }
    if (t>=30){
      digitalWrite(rojoTemperatura, HIGH);
        if (AireUno && !AireDos){
          AireDos = 1; 
          AccionarAireDos();
          TemAlta=1;
          //DosAiresEncendidos=1;
        }
        if (!AireUno && AireDos){
          AccionarAireUno();
          AireUno = 1;
          TemAlta=1;
          //DosAiresEncendidos=1;
        }
        if (currentMillis - previousMillisMail30 >= 600*interval){
            previousMillisMail30 = currentMillis;
            digitalWrite(rojoTemperatura, HIGH);
            Serial.println("30*C Alcanzados."); 
            Blynk.email("sistemasit@armetal.com.ar", "Subject: Alerta 25°", "Se alcanzaron los 30°C en el centro de datos, por favor revise la situación");
            Blynk.notify("La temperatura del datacenter alcanzo los 30*C");
        }
    }  
} 
}
//***********************************************************************

//**********Si la temperatura es menor a 25 grados estas variables se mantienen constantes**************
    if (t<25){
      digitalWrite(rojoTemperatura, LOW);
      TemAlta = 0;
    }
//***************************************************************************************************

//*************Preparacion de arduino y aires para el reseteo cuando la temperatura es alta****************************************
    if (TemAlta && HoyEs == 1){
      if (hora == 8 && minutos == 55) {
            AireUno = 0;
            AireDos = 1;
            AccionarAireUno(); //el dos sigue encendido 
            BloqueodeAlertas = 1; 
            TemAlta= 0;       
      }
    }
//***************************************************

} 

BLYNK_READ(V1){
  if (AireUno && !AireDos){
    Blynk.virtualWrite(V1, "Aire Uno Encendido");
  }
  if (!AireUno && AireDos){
    Blynk.virtualWrite(V1, "Aire Dos Encendido");
  }
  if (AireUno && AireDos){
    Blynk.virtualWrite(V1, "Ambos aires Encendidos");
  }
  if (!AireUno && !AireDos){
    Blynk.virtualWrite(V1, "Se produzco un error");
  }
}
BLYNK_READ(V5){
  Blynk.virtualWrite(V5, t);
}
BLYNK_READ(V6){
  Blynk.virtualWrite(V6, h);
}
BLYNK_CONNECTED() {  //sincronizacion con la nube despues de reinicio 
    if (isFirstConnect) {
      Blynk.syncAll();
      isFirstConnect = false;
    }
  }
void AccionarAireUno (){
  //Acciona Aire uno pin 38
}
void AccionarAireDos(){
  //Acciona Aire dos pin 42
}


