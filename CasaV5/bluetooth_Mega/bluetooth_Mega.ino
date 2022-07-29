//#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>

 //variables proximidad
 int TRIG = 10;
 int ECO=9;
 //int LED=3;
 int DURACION;
 int DISTANCIA;
 const int Buzzer=13;
 int bandera=0;

//Variables Bluetooth

char buffer[10]; //No olivar ponerlo
int boton=0;;

int pinTx=19;
int pinRx=18;
//SoftwareSerial bluetooth(pinTx,pinRx);


//Luces
int LSala=3;
int Lprueba=6;


//Variables temperatura

#define ONE_WIRE_BUS 2
int vent = 12;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
float tempC;

//lanza llamas
const int FLAME=A1;
int valorSensor=0;
int emergencia=0;


void setup() {
  //Variables proximidad
  pinMode (TRIG, OUTPUT);
  pinMode (ECO, INPUT);
  pinMode(Buzzer, OUTPUT);

  //Variables luz
  pinMode(LSala,OUTPUT);
  Serial.begin(9600);  

  //Bluethoot
  Serial1.begin(9600);
  //bluetooth.begin(9600);

  //Temperatura
  sensors.begin();
  //pinMode(vent, OUTPUT); //¿Esta línea va a o no???
  pinMode(Lprueba, OUTPUT);

  //lanza llamas 
  pinMode(FLAME,INPUT);
  

}

void loop() {
  
  int dist = leer_proximidad(); 
  int temp = leer_temperatura();
  int emergenciaFuego = lanza_llamas();
  int mensajeCerradura = 3;
 
  sensorTemperatura();//IMPORTANTE se tiene que poner antes de comprobar que el bluetooth esté disponible sino solo toma un solo dato
  if(bandera==0){    
      sensorProximidad();  
  }
  
    //Para varias variables
  sprintf(buffer, "%d,%d,%d,%d" ,dist,temp,emergenciaFuego,mensajeCerradura);
  Serial1.println(buffer);
  //Serial1.flush(); //eserar a que el comando salga, si hay error también puede ser por el serial.println quitarlo
  Serial.println(buffer);

  
  if(Serial1.available()>0){
      
      boton=Serial1.read();
      //Serial.println(boton);

      if(boton=='6'){
        bandera=1;
        //Serial.print("Bandera: ");
        //Serial.println(bandera);
      }

      luces();                 
  }
}



float leer_proximidad(){
  
  digitalWrite(TRIG, HIGH);
  delay(1); 
  digitalWrite(TRIG, LOW);
  DURACION= pulseIn(ECO,HIGH);
  DISTANCIA= DURACION/58.2; 
  //Serial.println(DISTANCIA); 
  delay(200);

  return DISTANCIA;
}

float leer_temperatura(){
  
  sensors.requestTemperatures(); // Send the command to get temperatures
  tempC = sensors.getTempCByIndex(0);
  
  return tempC;
}

void luces(){

  if(boton=='2'){
    //Serial.println("Luz sala encendida");
    digitalWrite(LSala, HIGH);
  }
  if(boton=='3'){
    //Serial.println("Luz sala apagada");
    digitalWrite(LSala, LOW);
  }
}
void sensorProximidad(){
 
  if (DISTANCIA<=20 && DISTANCIA >=0){
     delay(DISTANCIA*10);
     analogWrite(Buzzer,225);
     delay(100);
     analogWrite(Buzzer,LOW);
    }

  if(DISTANCIA<=4 && DISTANCIA >=0){
     analogWrite(Buzzer,225);
     delay(2000);
     analogWrite(Buzzer,LOW);
  }
}

void sensorTemperatura(){
    if(tempC>30.0){
      analogWrite(vent, 255);
      digitalWrite(Lprueba, HIGH);     
    }
    else{
      analogWrite(vent, 0);
      digitalWrite(Lprueba, LOW);
    }  
 }

 int lanza_llamas(){
  
  valorSensor=analogRead(A1);
  //Serial.println(valorSensor);
  emergencia=0;
  
  if (valorSensor<400){
    Serial.println(valorSensor);
    digitalWrite(Buzzer,HIGH);
    delay(500);
    digitalWrite(Buzzer,LOW);
    delay(50);
    emergencia=1;
    //Serial.println(emergencia);
  } 
  return emergencia;
}
