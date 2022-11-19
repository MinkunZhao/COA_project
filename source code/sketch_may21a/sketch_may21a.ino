#include <stdio.h>
//控制时间一个库
#include "DS1302.h"
#include <Keypad.h>
const byte ROWS=4;
const byte COLS=4;
char keys[ROWS][COLS]={
{'7','8','9','/'},
{'4','5','6','*'},
{'1','2','3','-'},
{'D','0','=','+'}
};
byte rowPins[ROWS]={21,20,19,18};
byte colPins[COLS]={17,16,15,14};
Keypad keypad=Keypad(makeKeymap(keys),rowPins,colPins,ROWS,COLS);
DS1302 rtc(11,12,13);
void initRTCTime(void)
{
  rtc.writeProtect(false);
  rtc.halt(false); 
}

int setT1=0;
int setT2=0;
int setT3=0;
int setT4=0;
void setup() {

  // put your setup code here, to run once:
  pinMode(22,OUTPUT);
  
  for(int i=0;i<11;++i){
      pinMode(i,OUTPUT);
      digitalWrite(i,0);
      pinMode(i+30,OUTPUT);
      digitalWrite(i+30,0);
  }
  init1();
  init2();
  initRTCTime();
  
  }


void init1(){
writecom(56);
writecom(12);
writecom(6);
writecom(1);
}

void init2(){
writecom2(56);
writecom2(12);
writecom2(6);
writecom2(1); 
}


 
//
void setch(char ch){
    for(int i=0;i<8;++i){
      pinMode(i,OUTPUT);
      digitalWrite(i,0);
  }
int mm=int(ch);
int i=0;
while((mm/2)!=0){
digitalWrite(i,mm%2);
i++;
mm=mm/2;
}
digitalWrite(i,mm);
}
//
void setch2(char ch){
    for(int i=30;i<38;++i){
      pinMode(i,OUTPUT);
      digitalWrite(i,0);
  }
int mm=int(ch);
int i=30;
while((mm/2)!=0){
digitalWrite(i,mm%2);
i++;
mm=mm/2;
}
digitalWrite(i,mm);
}
//
void setint(int mm){
    for(int i=0;i<8;++i){
      pinMode(i,OUTPUT);
      digitalWrite(i,0);
  }
int i=0;
while((mm/2)!=0){
digitalWrite(i,mm%2);
i++;
mm=mm/2;
}
digitalWrite(i,mm);
 }
//
void setint2(int mm){
    for(int i=30;i<38;++i){
      pinMode(i,OUTPUT);
      digitalWrite(i,0);
  }
int i=30;
while((mm/2)!=0){
digitalWrite(i,mm%2);
i++;
mm=mm/2;
}
digitalWrite(i,mm);
 }
 //

void writecom(int a){
  digitalWrite(10,0);
  digitalWrite(9,0);
  digitalWrite(8,0);
  setint(a);
  //delay(10);
  digitalWrite(8,1);
  digitalWrite(8,0);
  }
  //
 void writecom2(int a){
  digitalWrite(40,0);
  digitalWrite(39,0);
  digitalWrite(38,0);
  setint2(a);
  //delay(10);
  digitalWrite(38,1);
  digitalWrite(38,0);
  }
 //

void writedat1(char a){
  digitalWrite(10,1);
  digitalWrite(9,0);
  digitalWrite(8,0);
  setch(a);
  //delay(10);
  digitalWrite(8,1);
  digitalWrite(8,0);
  }
//
void writedat2(char a){
  digitalWrite(40,1);
  digitalWrite(39,0);
  digitalWrite(38,0);
  setch2(a);
  //delay(10);
  digitalWrite(38,1);
  digitalWrite(38,0);
  }

char st1[]={'S','u','n','d','a','y',' ',' ',' '};
char st2[]={'M','o','n','d','a','y',' ',' ',' '};
char st3[]={'T','u','e','s','d','a','y',' ',' '};
char st4[]={'W','e','d','n','e','s','d','a','y'};
char st5[]={'T','h','u','r','s','d','a','y',' '};
char st6[]={'F','r','i','d','a','y',' ',' ',' '};
char st7[]={'S','a','t','u','r','d','a','y',' '};
char *dayASString(const Time::Day day){
  
  switch(day){
case Time::kSunday: return st1;
case Time::kMonday: return st2;
case Time::kTuesday: return st3;
case Time::kWednesday: return st4;
case Time::kThursday: return st5;
case Time::kFriday: return st6;
case Time::kSaturday: return st7; 
  }
}


 void display1(){
  Time tim = rtc.time();
  char buf1[50];
snprintf(buf1, sizeof(buf1), "%04d-%02d-%02d",
           tim.yr, tim.mon, tim.date);

 char buf2[50];
  snprintf(buf2,sizeof(buf2),"%02d:%02d:%02d",
           tim.hr,tim.min,tim.sec);
           
  writecom(128);
  //delay(10);
  for(int i=0;i<10;++i){
    writedat1(buf1[i]);
  }
writecom(192);

  //delay(10);
 for(int i=0;i<10;++i){
    writedat1(buf2[i]);

  }
  
  //writedat1(' ');
  //writedat1(' ');
   for(int i=0;i<3;++i){
    writedat1(dayASString(tim.day)[i]);
  }
  writedat1('.');
  }


char st10[]={'S','e','t','T','i','m','e',':'};
char st11[]={'H','o','u','r',':' };
char st12[]={'M','i','n',':'};
void display2(){
writecom2(128);
for(int i=0;i<8;++i){
  writedat2(st10[i]);
}
writecom2(192);
for(int j=0;j<5;++j){
  writedat2(st11[j]);
}
 }

int count=0;
int timeHour=0;
int timeMin=0;
void loop() {
  // put your main code here, to run repeatedly:
 display1();
 display2();
 //
 char key=keypad.getKey();
 if(key&&count<10){
  if(count==0)
    setT1=int(key)-48;
  else if(count==1){
    setT2=int(key)-48;
  }
  else if(count==8){
    setT3=int(key)-48;
  }
  else if(count==9){
    setT4=int(key)-48;
    timeHour=setT1*10+setT2;
    timeMin=setT3*10+setT4;
  }
  writecom2(count+197);
  count=count+1;
  writedat2(key);
  if(count==2){
    writedat2(' ');
    writedat2(' ');
    for(int t=0;t<4;++t){
      writedat2(st12[t]);
      
    }
    count=count+6;
  }
  
 }
 Time tim1 = rtc.time();
 if(timeHour==tim1.hr&&timeMin==tim1.min){
  digitalWrite(22,1);
 }
 else{
  digitalWrite(22,0);
 }
 //setint(128);
}
