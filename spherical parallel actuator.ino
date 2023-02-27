
// ##########################################################################
// ################## Spherical_Parallel_Actuator ###########################
// ##########################################################################

//http://fluxogramadigital.pt/project_arduino.html
//info@fluxogramadigital.pt



#include <Servo.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();

int i,j;

//leds
const int led_amarelo_m1=12; //motor1
const int led_verde_m2=4; //motor2
const int led_vermelho_m3=2; //motor3

const int interruptor=7;


//buzzer
const int buzzerPin=13;
const int tones[]={262, 294, 330, 349};
bool parar_alarme;

Servo ServoMotor1;
Servo ServoMotor2;
Servo ServoMotor3;

int acel_X;
int acel_Y;
int acel_Z;

const int porta_servo1=9;
const int porta_servo2=10;
const int porta_servo3=11;

int pos_inicial=0;

int angulo_m1=0;
int angulo_m2=0;
int angulo_m3=0;

//## movimento automático ##
int pos_final_m1=110;
int pos_final_m2=70;
int pos_final_m3=50;

int delay_m1=10;
int delay_m2=10;
int delay_m3=10;
//###############


void setup(){

  pinMode(interruptor, INPUT); //--interruptor
  
  pinMode(led_amarelo_m1, OUTPUT); // ---leds
  pinMode(led_verde_m2, OUTPUT);
  pinMode(led_vermelho_m3, OUTPUT);

  pinMode(buzzerPin, OUTPUT); //-- buzzer

  Serial.begin(9600); 
 
  preparar_servo_motores();

  detetar_acelerometro();

  parar_alarme=false;

}

void loop(){

  if ((digitalRead(interruptor) == true)) {
    Serial.println("--------automático");
    //tocar alarme de aviso para inicio de movimentação automática
    alarme();

    //###########  mover automáticamente  ########### 
    mover_m1();
    mover_m2();
    mover_m3();
   
      

    } else if ((digitalRead(interruptor) == false)) {
      Serial.println("acelerometro");
      //###########  mover com acelerometro  ###########
      mover_com_acelerometro();
      parar_alarme=false;
      

    }


}

void mover_com_acelerometro(){
  //---------- acelerometro
  sensors_event_t event; 
  accel.getEvent(&event);
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");
  Serial.println("m/s^2 ");
  //delay(100);

//controlar os leds de acordo com o motor que vai ser ativado
  if(event.acceleration.x>0){ //MOTOR1
    digitalWrite(led_amarelo_m1, HIGH);
  }else{
    digitalWrite(led_amarelo_m1, LOW);
  }

   if(event.acceleration.y>0){ //MOTOR2
     digitalWrite(led_verde_m2, HIGH);
  }else{
    digitalWrite(led_verde_m2, LOW);
  }

   if(event.acceleration.z>0){ //MOTOR3
     digitalWrite(led_vermelho_m3, HIGH);
  }else{
    digitalWrite(led_vermelho_m3, LOW);
  }

  //---------- mover servos com cada eixo do acelerometro
  acel_X=event.acceleration.x;
  angulo_m1=map(acel_X, 0, 10, 0, 100);
  ServoMotor1.write(angulo_m1);

  acel_Y=event.acceleration.y;
  angulo_m2=map(acel_Y, 0, 10, 0, 100);
  ServoMotor2.write(angulo_m2);

  acel_Z=event.acceleration.z;
  angulo_m3=map(acel_Z, 0, 10, 0, 100);
  ServoMotor3.write(angulo_m3);

  delay(20);

}

void mover_m1(){
  if (angulo_m1>=pos_inicial && angulo_m1<pos_final_m1){
    angulo_m1+=1;
    ServoMotor1.write(angulo_m1);
    digitalWrite(led_amarelo_m1, HIGH);
    delay(delay_m1);
  } else {
    ServoMotor1.write(pos_inicial);
    angulo_m1=0;
    digitalWrite(led_amarelo_m1, LOW);
  }
}


void mover_m2(){
  if (angulo_m2>=pos_inicial && angulo_m2<pos_final_m2){
    angulo_m2+=1;
    ServoMotor2.write(angulo_m2);
    digitalWrite(led_verde_m2, HIGH);
    delay(delay_m2);
  } else {
    ServoMotor2.write(pos_inicial);
    angulo_m2=0;
    digitalWrite(led_verde_m2, LOW);
  }
}

void mover_m3(){
  if (angulo_m3>=pos_inicial && angulo_m3<pos_final_m3){
    angulo_m3+=1;
    ServoMotor3.write(angulo_m3);
    digitalWrite(led_vermelho_m3, HIGH);
    delay(delay_m3);
  } else {
    ServoMotor3.write(pos_inicial);
    angulo_m3=0;
    digitalWrite(led_vermelho_m3, LOW);
  }
}

void preparar_servo_motores(){

  ServoMotor1.attach(porta_servo1);
  ServoMotor1.write(pos_inicial);

  ServoMotor2.attach(porta_servo2);
  ServoMotor2.write(pos_inicial);

  ServoMotor3.attach(porta_servo3);
  ServoMotor3.write(pos_inicial);
}

void detetar_acelerometro(){
  // -------------- detetar acelerometro
  if(!accel.begin())
   {
      Serial.println("Sensor não encontrado");
      while(1);
   } 
}

void alarme(){

if(parar_alarme==false){

  for(j=0;j<3;j++){//repetir 3 vezes o toque de alarme
    for(i=0;i<4;i++){ //usar 4 tons
     tone(buzzerPin, tones[i]);
     delay(150);
     noTone(buzzerPin);
    }
    delay(350);
  }
  parar_alarme=true;//parar o alarme até que voltemos ao modo manual
    
}
}
