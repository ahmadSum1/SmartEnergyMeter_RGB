


#include<EEPROM.h>

//Used for GSM
#include <SoftwareSerial.h>
SoftwareSerial mySerial(7, 8);

//For current Sensor
#include "ACS712.h"
float error= 0.05;
 ACS712 ac1(A0, error);
int smsnum =27;
String readMsg = "AT+CMGR="; 
int smsTimeP=millis()/1000;
bool alert=true;

double current=0L;
double power=0L;
double load=0L;
float balance=1000.00f;
float unit =1000.00f;

float unitPrice =5.00f;

long lastPrint = millis();
long lastDtl = millis();
String BalanceSms = "This is an SMS from Smart Energy Meter & you balance is ";




void setup(){


  delay(100);
  mySerial.begin(9600);   // Setting the baud rate of GSM Module 
  Serial.begin(9600);

  reStoreInfo();

  printInfo();
//BalanceSms();



//call();
 delay(100);
}

void loop(){

if((millis() - lastDtl)>10000) //print balance and usage
{
 mySerial.begin(9600);
  lastDtl = millis();
}

 current = ac1.getACcurrent();
 power=current*220.00L;
 load =(power/(1000.0L*3600.0L));
 unit+=load;
  
 balance -=load*unitPrice;
    
storeInfo();
printDetails();





if((millis() - lastPrint)>5000) //print balance and usage
{
  printInfo();
  lastPrint = millis();
}

if(balance < 20 && alert)
{
    balanceSms();
    alert = false;
}
bool s=false;

      while (mySerial.available()>0)
       {
        Serial.write(mySerial.read());
    
        //    text back the user the balance if he ask for balance
        s=true;
    
       }


   if(s)
    {
      Serial.println("Got 1");
    balanceSms();
    
    }

{  //user interactions // comands
if (Serial.available()>0)
  {
    char cmd = Serial.read();
    Serial.println(cmd); 
    
    if(cmd=='R') 
    {
      balance=00.00f;
      unit=00.00f;
      for (int i = 0 ; i < EEPROM.length() ; i++) EEPROM.write(i, 0);    //clear all EEPROM
      storeInfo();
      printInfo();
    }            

    if(cmd=='U') {
      balance+=1000.00f;
      printInfo();
      alert = true;
      
      }
    if(cmd=='G') {
      unit+=10.00f;
      printInfo();
    
    }


    
    Serial.flush();
 }
}


 

}

void call(){
  
  mySerial.println("ATD+88001759848889;");
 }

void balanceSms(){
  
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+8801759848889\"\r"); // Replace x with mobile number
  delay(1000);
  BalanceSms+=balance;
  mySerial.println(BalanceSms);// The SMS text you want to send
  delay(100);
   mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  BalanceSms="This is an SMS from Smart Energy Meter & you balance is ";
  mySerial.end();
}









//printing...
void printInfo()
{
  Serial.print("\n\tBalance left:  ");
  Serial.print(balance,4);
  Serial.print("tk Total Use: ");
  Serial.print(unit,5);
  Serial.println(" Units\n");
}
void printDetails()
{
     Serial.print("\t");
     Serial.print(current);
     Serial.print(" Amps, ");
      Serial.print(power);
     Serial.print(" Watt,   ");
      Serial.print(unit,5);
     Serial.println(" KWatt-hour\t");
  
  }

//eeprom functions
void storeInfo()
{
//  EEPROM[100]=unit;
//  EEPROM[0]=balance;
    
  EEPROM.put(0,unit);
  EEPROM.put(100,balance);
  

  
}

void reStoreInfo()
{
//  unit= EEPROM[100];
//  balance= EEPROM[0];
    
  EEPROM.get(0,unit);
  EEPROM.get(100,balance);
  
}





  