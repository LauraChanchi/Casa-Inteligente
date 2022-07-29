#include <Keypad.h>
//#include <SoftwareSerial.h>
#include <Servo.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//Cerradura
const byte FILAS = 4;
const byte COLUMNAS = 3;
char keys[FILAS][COLUMNAS]= {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte pinesFilas[FILAS] = {53,51,49,47};
byte pinesColumnas[COLUMNAS] = {45,43,41};

Keypad teclado = Keypad(makeKeymap(keys), pinesFilas, pinesColumnas, FILAS, COLUMNAS);

char TECLA;
char enter;
char CLAVE[7];



char CLAVE_MAESTRA[7] = "123456";
byte INDICE = 0;
int ledVerde = 5;
int ledRojo = 4;
int Buzzer =3; 
int intentos = 1;
int bandera=0;
int cerradura_abierta=0;

//Servo
Servo servo;
int pinServo=37;

//Luces
int LSala=7;
int LGaraje=6;

//variables proximidad
int TRIG = 10;
int ECO=9;
//int LED=3;
int DURACION;
int DISTANCIA;
//const int Buzzer=13;
int banderita=0;

//Bluetooth

char buffer[10]; //No olivar ponerlo
int boton=0;

int pinTx=19;
int pinRx=18;
//SoftwareSerial bluetooth(pinTx,pinRx);

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

void setup(){

  Serial.begin(9600);
  //Para el Bluetooth en el arduino
  
  Serial1.begin(9600);
  
  //Cerradura
  pinMode(ledVerde, OUTPUT);
  pinMode(ledRojo, OUTPUT);
  pinMode(Buzzer, OUTPUT);

  //Servo
  servo.attach(pinServo); //se asocia el servo con su pin
  servo.write(90);

  //Bluetooth
  //bluetooth.begin(9600);

  //Luces
  pinMode(LSala,OUTPUT);
  pinMode(LGaraje, OUTPUT);

  //Variables proximidad
  pinMode (TRIG, OUTPUT);
  pinMode (ECO, INPUT);
  pinMode(Buzzer, OUTPUT);

  //Temperatura
  sensors.begin();
  //pinMode(vent, OUTPUT); //¿Esta línea va a o no???
  //pinMode(Lprueba, OUTPUT);

  //lanza llamas 
  pinMode(FLAME,INPUT);
}

void loop(){
  int dist = leer_proximidad(); 
  int temp = leer_temperatura();
  int emergenciaFuego = lanza_llamas();
  int mensajeCerradura = Cerradura();
 
  sensorTemperatura();
  if(banderita==0){    
      sensorProximidad();  
  }
  
    //Para varias variables
  sprintf(buffer, "%d,%d,%d,%d" ,dist,temp,emergenciaFuego,mensajeCerradura);
  Serial1.println(buffer);
  Serial.println(buffer);
   
  if(Serial1.available()>0){
        boton=Serial1.read();

        if(boton=='6'){
          banderita=1;
        }
        ServoMotor();
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

int Cerradura(){
  
  analogWrite(Buzzer,LOW);
   
  TECLA = teclado.getKey();
 
  if(TECLA){
    pulsarTecla();
    CLAVE[INDICE] = TECLA;
    INDICE++;
    Serial.print(TECLA);    
  }    
    
  if(INDICE == 6){
       
    if(!strcmp(CLAVE, CLAVE_MAESTRA)){
      Serial.println("  Correcta");
      digitalWrite(ledVerde, HIGH);
      tone(Buzzer, 700);
      delay(2000);
      noTone(Buzzer);
      digitalWrite(ledVerde, LOW);
      bandera=1;
    }
    
    else if(intentos<3){
      Serial.println("  Incorrecta");
      digitalWrite(ledRojo, HIGH);
      tone(Buzzer, 50);
      delay(1000);
      noTone(Buzzer);
      digitalWrite(ledRojo, LOW);
      intentos++;
      bandera=2;
    }
    
    else if(intentos=3){
      Serial.println("  Bloqueada");
      digitalWrite(ledRojo, HIGH);
      alarma();
      delay(100);
      alarma();
      delay(100);
      alarma();
      delay(100);
      alarma();
      digitalWrite(ledRojo, LOW);
      bandera=3;  
    }
    
    INDICE = 0;
  }
  return bandera; 
}

//Auxiliares Cerradura
void alarma(){
  analogWrite(Buzzer, 225);
  delay(500);
  analogWrite(Buzzer,LOW);
  }
void pulsarTecla(){
  analogWrite(Buzzer, 500);
  delay(10);
  analogWrite(Buzzer,LOW);
  }

void ServoMotor(){
 
  if(boton=='0'){
    Serial.println("Abierto");
    AbrirPuerta();
    //servo.write(0);
    }
  if(boton=='1'){
    Serial.println("Cerrada");
    CerrarPuerta();
    //servo.write(0);
    }
 }

//Auxiliares Servo

void CerrarPuerta(){
  servo.write(0);
  delay(500);
  
  servo.write(10);
  delay(500);
  
  servo.write(20);
  delay(500);

  servo.write(30);
  delay(500);
  
  servo.write(40);
  delay(500);
  
  servo.write(50);
  delay(500);

  servo.write(60);
  delay(500);

  servo.write(70);
  delay(500);

  servo.write(80);
  delay(500);

  servo.write(90);
  delay(500);

  servo.write(100);
  delay(500);
}

void AbrirPuerta(){
  servo.write(80);
  delay(500);
  
  servo.write(70);
  delay(500);
  
  servo.write(60);
  delay(500);

  servo.write(50);
  delay(500);
  
  servo.write(40);
  delay(500);
  
  servo.write(30);
  delay(500);

  servo.write(20);
  delay(500);

  servo.write(10);
  delay(500);

  servo.write(0);
  delay(500);
}


void luces(){

  if(boton=='2'){
    Serial.println("Luz sala encendida");
    digitalWrite(LSala, HIGH);
    delay(2000);
  }
  if(boton=='3'){
    Serial.println("Luz sala apagada");
    digitalWrite(LSala, LOW);
  }
  if(boton=='4'){
    Serial.println("Luz Garaje Encendida");
    digitalWrite(LGaraje, HIGH);
  }
  if(boton=='5'){
    Serial.println("Luz Garaje apagada");
    digitalWrite(LGaraje, LOW);
  }
}

void sensorProximidad(){
 
  if (DISTANCIA<=18 && DISTANCIA >=0){
     delay(DISTANCIA*15);
     tone(Buzzer,750);
     delay(100);
     noTone(Buzzer);
     //analogWrite(Buzzer,LOW);
    }

  if(DISTANCIA<=4 && DISTANCIA >=0){
     analogWrite(Buzzer,225);
     delay(2000);
     analogWrite(Buzzer,LOW);
  }
}

void sensorTemperatura(){
    if(tempC>26.0){
      analogWrite(vent, 255);
      
      //digitalWrite(LSala, HIGH);     
    }
    else{
      analogWrite(vent, 0);
      //digitalWrite(LSala, LOW);
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
