#include <LiquidCrystal.h>
#include <SoftwareSerial.h> //FT232
#include <Adafruit_Fingerprint.h>
#define BUZZ 11

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial);
SoftwareSerial FSerial(2,3); // RX, TX    // WiFi
//*************************************************
LiquidCrystal lcd(4, 5, 6, 7, 8, 9);
int pp1Pin = A5;
int pp2Pin = A4;
int pp3Pin = A3;
int pp4Pin = A2;
int resPin = A1;
int buz = 11;

char ruser = 0;
int face = 0 ;
int pp1=0,pp2=0,pp3=0,pp4=0;
int uservoted[11] = {0,0,0,0,0,0,0,0,0,0,0};
int i;
int face_fleg = 0;
int Mode = 0;
char inbyte;
int id ;
//*************************************************
void face_match()
{
  if (FSerial.available()>0)
  {
    inbyte = FSerial.read();
    //FSerial.print(inbyte);
    if(inbyte == ruser)
    {
      lcd.clear();
      lcd.print("Match User:      ");
      lcd.setCursor(12, 0);lcd.print(inbyte);
      delay(2000);
      face_fleg =2;
    }
    else if(inbyte == 'X')
    {
       lcd.clear();
       lcd.print(" Not Match Face ");      
       delay(3000);
       lcd.clear();
       lcd.print("  Place Finger  ");
       face_fleg =3;
    }
    else
    {
       lcd.clear();
       lcd.print("Face != User   ");      
       delay(3000);
       lcd.clear();
       face_fleg =3;
    }
  }
}
//*************************************************
void(* resetFunc) (void) = 0; 
//*************************************************
void lcd_init()
{
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("   EVM using    ");
  lcd.setCursor(0, 1);
  lcd.print("   Biometrics   ");
  delay(2000);
  lcd.clear();
}   
//*************************************************
void getvote()
{
  int flag=0;
  lcd.clear();lcd.print("1: BJP    2: AAP");
  lcd.setCursor(0, 1);lcd.print("3: INC    4: BSP");
  digitalWrite(buz,HIGH);delay(500);digitalWrite(buz,LOW);delay(500);
  digitalWrite(buz,HIGH);delay(500);digitalWrite(buz,LOW);delay(500);
  digitalWrite(buz,HIGH);delay(500);digitalWrite(buz,LOW);delay(500);
     
  while(flag==0)
  {
    if(digitalRead(pp1Pin)==LOW){pp1++;flag=1;}
    if(digitalRead(pp2Pin)==LOW){pp2++;flag=1;}
    if(digitalRead(pp3Pin)==LOW){pp3++;flag=1;}
    if(digitalRead(pp4Pin)==LOW){pp4++;flag=1;}
  }
  //WSerial.print("<A");WSerial.print(pp1);WSerial.print(">");
  //WSerial.print("<B");WSerial.print(pp2);WSerial.print(">");
  //WSerial.print("<C");WSerial.print(pp3);WSerial.print(">");
  //WSerial.print("<D");WSerial.print(pp4);WSerial.print(">");
  lcd.clear();lcd.print("Thanks for vote");
  digitalWrite(buz,HIGH);
  delay(3000);
  digitalWrite(buz,LOW);
  lcd.clear();
}

//*************************************************  
uint8_t getFingerprintEnroll(int id) {

  int p = -1;
  //Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      //Serial.println("Imaging error");
      break;
    default:
      //Serial.println("Unknown error");
      break;
    }
  }
  // OK success!

  p = finger.image2Tz(1);//converts to feature template
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      //Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      //Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      //Serial.println("Could not find fingerprint features");
      return p;
    default:
      //Serial.println("Unknown error");
      return p;
  }
  lcd.clear();lcd.print("Remove Finger");
  //Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  //Serial.print("ID "); Serial.println(id);
  p = -1;
  lcd.clear();lcd.print("Place Same Finger");
  //Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      //Serial.println("Imaging error");
      break;
    default:
      //Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      //Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      //Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      //Serial.println("Could not find fingerprint features");
      return p;
    default:
      //Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  //Serial.print("Creating model for #");  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    //Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    //Serial.println("Fingerprints did not match");
    return p;
  } else {
    //Serial.println("Unknown error");
    return p;
  }   
  
  //Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    lcd.clear();lcd.print("Stored");
    //Serial.println("Stored!");
    //ok();
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //Serial.println("Communication error");
    //error();
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    //Serial.println("Could not store in that location");
    //error();
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    //Serial.println("Error writing to flash");
    //error();
    return p;
  } else {
    //Serial.println("Unknown error");
    //error();
    return p;
  }   
}
//*************************************************
void clear_database()
{
  finger.emptyDatabase();
}
//*************************************************
//Use for status of fingerprint
uint8_t getFingerprintID3() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      //Serial.println("Imaging error");
      return p;
    default:
      //Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      //error();
      //Serial.write(0x00); 
      //Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      //error();
      //Serial.write(0x00); 
      //Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      //error();
      //Serial.write(0x00); 
      //Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      //error();
      //Serial.write(0x00); 
      //Serial.println("Could not find fingerprint features");
      return p;
    default:
      //error();
      //Serial.write(0x00); 
      //Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    //Serial.println("Found a print match!");
    //ok2();
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //error();
    //Serial.write(0x00); 
    //Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    digitalWrite(buz,HIGH);
    lcd.clear();lcd.print("User Not Match  ");
    delay(3000);
    lcd.clear();lcd.print("  Place Finger  ");
    digitalWrite(buz,LOW);
    //error();
    //Serial.write(0x00); 
    //Serial.println("Did not find a match");
    return p;
  } else {
    //error();
    //Serial.write(0x00); 
    //Serial.println("Unknown error");
    return p;
  }
  
  id = finger.fingerID;
  //FSerial.print(id);   
  if (id>0 && id <6)
  {
    lcd.clear();
    ruser = id+48;
    //FSerial.print(id+48);FSerial.print("  ");FSerial.print(ruser);
    if(uservoted[id]==0)
    {
      //Proceed to vote
      
      lcd.print("User : ");lcd.print(ruser);
      digitalWrite(buz,HIGH);delay(500);digitalWrite(buz,LOW);delay(500);
     
      //lcd.setCursor(0, 1);lcd.print("Proceed to vote");
      delay(3000);
      // send face 
      FSerial.print(ruser);
      face_fleg = 0;
      lcd.clear();
      lcd.print("Face Matching   ");
      lcd.setCursor(0, 1);lcd.print("Please Wait.... ");
      while(face_fleg ==0)
      {
        // face matching
        face_match();
      }
      while(face_fleg == 2)
      {
        getvote();
        uservoted[id]=1;
        face_fleg = 3;
      }
    }
    else
    {
      digitalWrite(buz,HIGH);
      lcd.print("User : ");lcd.print(ruser);
      lcd.setCursor(0, 1);lcd.print("Already voted");
      delay(3000);
      digitalWrite(buz,LOW);
    }
    delay(3000);
    lcd.clear();
    lcd.print("  Place Finger  ");
  }
  return finger.fingerID;
}
//*************************************************
void enroll()
{
  clear_database();
  lcd.clear();lcd.print("Enroll User 1");getFingerprintEnroll(1);delay(3000);
  lcd.clear();lcd.print("Enroll User 2");getFingerprintEnroll(2);delay(3000);
  lcd.clear();lcd.print("Enroll User 3");getFingerprintEnroll(3);delay(3000);
  lcd.clear();lcd.print("Enroll User 4");getFingerprintEnroll(4);delay(3000);
  lcd.clear();lcd.print("Enroll User 5");getFingerprintEnroll(5);delay(3000);
  /*lcd.clear();lcd.print("Enroll User 6");getFingerprintEnroll(6);delay(3000);
  lcd.clear();lcd.print("Enroll User 7");getFingerprintEnroll(7);delay(3000);
  lcd.clear();lcd.print("Enroll User 8");getFingerprintEnroll(8);delay(3000);
  lcd.clear();lcd.print("Enroll User 9");getFingerprintEnroll(9);delay(3000);
  lcd.clear();lcd.print("Enroll User 10");getFingerprintEnroll(10);delay(3000);*/
  lcd.clear();
}

//*************************************************
void setup()
{
  FSerial.begin(9600);
  Serial.begin(57600);
  lcd_init();
  pinMode(buz,OUTPUT);
  pinMode(pp1Pin,INPUT);digitalWrite(pp1Pin,HIGH);
  pinMode(pp2Pin,INPUT);digitalWrite(pp2Pin,HIGH);
  pinMode(pp3Pin,INPUT);digitalWrite(pp3Pin,HIGH);
  pinMode(pp4Pin,INPUT);digitalWrite(pp4Pin,HIGH);
  pinMode(resPin,INPUT);digitalWrite(resPin,HIGH);
  lcd.clear();
  finger.begin(57600);
  digitalWrite(buz,HIGH);
  delay(1000);
  finger.verifyPassword();finger.verifyPassword();finger.verifyPassword();
  delay(1000);
  digitalWrite(buz,LOW);
  if (finger.verifyPassword()) {
    lcd.print("Fingerprint...OK");
  } else {
    lcd.print("Fingerprint...X");
    while (1) { delay(2000);resetFunc(); }
  }
  if(digitalRead(resPin)==LOW)
  {
    enroll();
  }
  delay(2000);
  lcd.clear();
  lcd.print("  Place Finger  ");
  Mode = 0;
  uservoted[1]=0;uservoted[2]=0;uservoted[3]=0;uservoted[4]=0;uservoted[5]=0;
  FSerial.print("7");
}
//*************************************************
void loop()
{
   
   getFingerprintID3();
   if(digitalRead(resPin)==LOW)
    {
     digitalWrite(buz,HIGH);delay(500);digitalWrite(buz,LOW);delay(500);
     digitalWrite(buz,HIGH);delay(500);digitalWrite(buz,LOW);delay(500);
     lcd.clear();lcd.print("BJP:       AAP: ");
     lcd.setCursor(0, 1);lcd.print("INC:       BSP: ");
     lcd.setCursor(4, 0);lcd.print(pp1);
     lcd.setCursor(15, 0);lcd.print(pp2);
     lcd.setCursor(4, 1);lcd.print(pp3);
     lcd.setCursor(15, 1);lcd.print(pp4);
     delay(5000);
     while(digitalRead(resPin)==LOW);
     lcd.clear();
     lcd.print("  Place Finger  ");
     uservoted[1]=0;uservoted[2]=0;uservoted[3]=0;uservoted[4]=0;uservoted[5]=0;
     pp1=0;pp2=0;pp3=0;pp4=0;
    }
  
}
//*************************************************
