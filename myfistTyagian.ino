#include <POP32.h> 

#define baseSpd 30
#define kp 10
#define ki 0
#define kd 0
//Values 
int sensorValues[5] ;
int threshold[5] = {1425,1970,2125,2150,1400};
short sensorsIsBlack[5];
int lSpd,rSpd = baseSpd;
//config 
//PID
int P,I,D,prevP,PID_value = 0 ;

int timer;

void updateSensor(){
  sensorValues[0] = analog(0) ; 
  sensorValues[1] = analog(1) ; 
  sensorValues[2] = analog(2) ; 
  sensorValues[3] = analog(3) ; 
  sensorValues[4] = analog(4) ; 
  for (int i = 0 ; i<= 4 ; i++) {  
    if (sensorValues[i] < threshold[i]) sensorsIsBlack[i] = 1 ; else sensorsIsBlack[i] = 0 ; 
  }
}
void cross(){
  while (1) {
    oled.clear();
    updateSensor();
    if  (sensorsIsBlack[0] && sensorsIsBlack[1] && sensorsIsBlack[2] && sensorsIsBlack[3] && sensorsIsBlack[4]) {
      ao();
      beep();
      break;
    }
    else if (sensorsIsBlack[0] && !sensorsIsBlack[1] && !sensorsIsBlack[2] && !sensorsIsBlack[3] && !sensorsIsBlack[4]) P = -4 ;
    else if (sensorsIsBlack[0] && sensorsIsBlack[1] && !sensorsIsBlack[2] && !sensorsIsBlack[3] && !sensorsIsBlack[4]) P = -3 ;
    else if (!sensorsIsBlack[0] && sensorsIsBlack[1] && !sensorsIsBlack[2] && !sensorsIsBlack[3] && !sensorsIsBlack[4]) P = -2 ;
    else if (!sensorsIsBlack[0] && sensorsIsBlack[1] && sensorsIsBlack[2] && !sensorsIsBlack[3] && !sensorsIsBlack[4]) P = -1 ;
    else if (!sensorsIsBlack[0] && !sensorsIsBlack[1] && sensorsIsBlack[2] && !sensorsIsBlack[3] && !sensorsIsBlack[4]) P = 0 ;
    else if (!sensorsIsBlack[0] && !sensorsIsBlack[1] && sensorsIsBlack[2] && sensorsIsBlack[3] && !sensorsIsBlack[4]) P = 1 ;
    else if (!sensorsIsBlack[0] && !sensorsIsBlack[1] && !sensorsIsBlack[2] && sensorsIsBlack[3] && !sensorsIsBlack[4]) P = 2 ;
    else if (!sensorsIsBlack[0] && !sensorsIsBlack[1] && !sensorsIsBlack[2] && sensorsIsBlack[3] && sensorsIsBlack[4]) P = 3 ;
    else if (!sensorsIsBlack[0] && !sensorsIsBlack[1] && !sensorsIsBlack[2] && !sensorsIsBlack[3] && sensorsIsBlack[4]) P = 4 ;
    
    prevP = P ;
    PID_value = (kp * P) + (kd * (P-prevP)); //in this case we dont use ki term , may be add in future
    PID_value = constrain(PID_value, -100, 100);
    lSpd = constrain(baseSpd + PID_value ,-100,100);
    rSpd = constrain(baseSpd - PID_value ,-100,100);

    motor(1,lSpd);
    motor(2,rSpd);
    motor(3,rSpd);
    motor(4,lSpd);

    oled.text(1, 1," %d",P);
    oled.show();
   // Serial.printf("error : %d \n %d %d %d %d %d \n",P,sensorValues[0],sensorValues[1],sensorValues[2],sensorValues[3],sensorValues[4]);
  }//while
}


void lineTrac (){
  /*errorValue: 
  L           |M|          R
  -4 -3 -2 -1 |0| 1  2  3  4 
  */

  updateSensor();
  if (sensorsIsBlack[0] && !sensorsIsBlack[1] && !sensorsIsBlack[2] && !sensorsIsBlack[3] && !sensorsIsBlack[4]) P = -4 ;
  else if (sensorsIsBlack[0] && sensorsIsBlack[1] && !sensorsIsBlack[2] && !sensorsIsBlack[3] && !sensorsIsBlack[4]) P = -3 ;
  else if (!sensorsIsBlack[0] && sensorsIsBlack[1] && !sensorsIsBlack[2] && !sensorsIsBlack[3] && !sensorsIsBlack[4]) P = -2 ;
  else if (!sensorsIsBlack[0] && sensorsIsBlack[1] && sensorsIsBlack[2] && !sensorsIsBlack[3] && !sensorsIsBlack[4]) P = -1 ;
  else if (!sensorsIsBlack[0] && !sensorsIsBlack[1] && sensorsIsBlack[2] && sensorsIsBlack[3] && !sensorsIsBlack[4]) P = 1 ;
  else if (!sensorsIsBlack[0] && !sensorsIsBlack[1] && !sensorsIsBlack[2] && sensorsIsBlack[3] && !sensorsIsBlack[4]) P = 2 ;
  else if (!sensorsIsBlack[0] && !sensorsIsBlack[1] && !sensorsIsBlack[2] && sensorsIsBlack[3] && sensorsIsBlack[4]) P = 3 ;
  else if (!sensorsIsBlack[0] && !sensorsIsBlack[1] && !sensorsIsBlack[2] && !sensorsIsBlack[3] && sensorsIsBlack[4]) P = 4 ;
  else P = 0 ;


  prevP = P ;
  PID_value = (kp * P) + (kd * (P-prevP)); //in this case we dont use ki term , may be add in future
  PID_value = constrain(PID_value, 0, 100-baseSpd);
  lSpd = baseSpd + PID_value ;
  rSpd = baseSpd - PID_value ;

  motor(1,lSpd);
  motor(2,rSpd);
  motor(3,lSpd);
  motor(4,rSpd);
}


void setup() {
  // put your setup code here, to run once:
  
}

void loop() {
  

  waitAnykey_bmp();
  /*oled.text(1,6,"error : %d and %d %d %d %d %d",P,sensorValues[0],sensorValues[1],sensorValues[2],sensorValues[3],sensorValues[4]);
  oled.show();
  waitAnykey_bmp();*/
  cross();
  ao();
  
  


 
}
