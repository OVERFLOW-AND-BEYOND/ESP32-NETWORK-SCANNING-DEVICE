//PROGRAMMA PER ESP32
//ATTENZIONE: IL PIN SCK (DELLO SCHERMO) è STATO SPOSTATO DA PIN 18 A PIN 27 PER EVITARE DI INTEFERIRE CON I PIN DELLA SD CARD
//ATTENZIONE: IL PIN MOSI (DELLO SCHERMO) è STATO SPOSTATO DA PIN 23 A PIN 25 PER EVITARE DI INTERFERIRE CON I PIN DELLA SD CARD
//ATTENZIONE: QUESTI CAMBIAMENTI SONO VISIBILI NEL SEGUENTE FILE: C:\Users\Paride\Documents\Arduino\libraries\TFT_eSPI\User_Setup.h dentro la sezione "ESP32"




#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>
TFT_eSPI tft = TFT_eSPI();  // Invoke library
#define TFT_COLOR_1 0x00C2 // New colour

//LIBRERIE DEL GPS
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
TinyGPSPlus gps;
static const int RXPin = 19, TXPin = 18; //i pin per la connessione del gps
static const uint32_t GPSBaud = 9600; // il baudrate tra gps e esp32
SoftwareSerial ss(RXPin, TXPin); //la connessione tra il gps e il device

//IMPORTO LA LIBRERIA PER IL WIFI:
#include "WiFi.h"



//IMPORTO LA LIBRERIA PER LA EEPROM:
#include<EEPROM.h>
//DEFINISCO LE DIMENSIONI DELLA EEPROM:
#define EEPROM_SIZE 8192 //4096



//VARIABILI:
int start_screen = 1;


int random_address = -1;
int data_eeprom = 0;

//VARIABILE PER DETERMINARE IL CHIP ID
uint32_t chipId = 0;

//variabili_menu:

int active_menu = 0;
int active_menu_animated = 1;



int pointer_value = 0;
bool option_1 = false;
bool option_2 = false;
bool option_3 = false;
bool option_4 = false;
bool option_5 = false;
bool option_6 = false;
bool option_7 = false;
bool option_8 = false;
bool option_9 = false;

bool option_1_animated = true;
bool option_2_animated = true;
bool option_3_animated = true;
bool option_4_animated = true;
bool option_5_animated = true;
bool option_6_animated = true;
bool option_7_animated = true;
bool option_8_animated = true;
bool option_9_animated = true;


bool button_1_last_state = false;
bool button_2_last_state = false;
bool button_3_last_state = false;
bool button_4_last_state = false;


//variabili_option_1:

int valore_percentuale = 0;
int value_barra = 0;


//variabili_option_2:

int data_read = 0;
int corruptions = 0;


//variabili_option_5:

int change_ui = 0;
int frame = 0;
int change_ui_animated = 1;
int frame_total = 0;
int requires_rendering = 0;
int delete_screen = 0;
int lines_delay = 10;


//variabili_option_6:

int change_ui_6_1 = 0;
int change_ui_6_1_animated = 1; 
unsigned long millis_1 = 1;
int frame_networks = 0;
int n = 0;
int override_update = 0;
int data_view = 0;
int save_eeprom = 0;
int save_eeprom_animated = 1;
int code = 0;

int start_address = 0;
char carattere_inizio_pacchetto = '<';
char carattere_fine_pacchetto = '>';
int inWrite_int = 0;
String string_ssid;
String string_finale_net;

//variabili_option_7:

String string_finale_gps;


void setup(){

//IO:
pinMode(17,INPUT); //bottone_1
pinMode(23,INPUT); //bottone_2
pinMode(22,INPUT); //bottone_3
pinMode(21,INPUT); //bottone_4

#define Button_1 17
#define Button_2 23
#define Button_3 22
#define Button_4 21

//inizializzo la seriale per il debug:
Serial.begin(115200);

//inizializzo la seriale per il gps:
ss.begin(GPSBaud);


//inizializzo lo shermo.

  tft.init();
  tft.setRotation(1); 
  tft.fillScreen(TFT_BLACK);

// INIZIALIZZO LA EEPROM:
EEPROM.begin(EEPROM_SIZE);

//disegna le cose per il post screen
  tft.fillRect(0,0,320,16, TFT_COLOR_1);


  // Set WiFi to station mode and disconnect from an AP if it was previously connected.
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();


//salvo delle stringhe che mi serviranno per salvare le reti nella eeprom
string_ssid = String();
string_finale_net = String();
string_finale_gps = String();


}





void loop(){

if (start_screen == 1){

  	for(int i=0; i<17; i=i+8) {
	  chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
	}

   
  tft.setCursor(37, 0, 2);
  tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
  tft.println("ESP32 NETWORK SCANNER -- by PARIDE");

  delay(200);

  tft.setCursor(2, 20, 2);
  tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
  tft.println("> Starting boot sequence");

  delay(100);

    tft.setCursor(2,40, 2);
  tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
  tft.println("Processor Model:");



  tft.setCursor(170,40, 2);
  tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
  tft.println(ESP.getChipModel());

  delay(100);


      tft.setCursor(2,55, 2);
  tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
  tft.println("Processor Revision:");

    tft.setCursor(170,55, 2);
  tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
  tft.println(ESP.getChipRevision());

  delay(100);

  tft.setCursor(2,70, 2);
  tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
  tft.println("Chip ID:");

  tft.setCursor(170,70, 2);
  tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
  tft.println(chipId);

  delay(100);

   tft.setCursor(2,90, 2);
  tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
  tft.println("----------------------------------------------------");

  delay(200);

  tft.setCursor(2,110, 2);
  tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
  tft.println(">> Testing eeprom:");

  tft.setCursor(150,110, 2);
  tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
  tft.println(EEPROM_SIZE);

  tft.setCursor(220,110, 2);
  tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
  tft.println("bytes");



  delay(200);

  tft.setCursor(2,130, 2);
  tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
  tft.println("testing random address (1/5): ");

  //testo la eeprom ad address_random_1:

while (random_address < 0 || random_address > EEPROM_SIZE){
random_address = esp_random();
}

  data_eeprom = EEPROM.read(random_address);
  EEPROM.write(random_address,255);
if (EEPROM.commit()){

  tft.setCursor(195,130, 2);
  tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
  tft.println("SUCCESS");

  tft.setCursor(260,130, 2);
  tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
  tft.println("@");

    tft.setCursor(270,130, 2);
  tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
  tft.println(random_address);

  }else{
  tft.setCursor(195,130, 2);
  tft.setTextColor(TFT_RED);  tft.setTextSize(1);
  tft.println("ERROR");

    tft.setCursor(260,130, 2);
  tft.setTextColor(TFT_RED);  tft.setTextSize(1);
  tft.println("@");

  tft.setCursor(270,130, 2);
  tft.setTextColor(TFT_RED);  tft.setTextSize(1);
  tft.println(random_address);

  }

  EEPROM.write(random_address,data_eeprom);
  EEPROM.commit();

//testo la eeprom ad address_random_2:

random_address = -1;

  delay(200);

  tft.setCursor(2,150, 2);
  tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
  tft.println("testing random address (2/5): ");


while (random_address < 0 || random_address > EEPROM_SIZE){
random_address = esp_random();
}

  data_eeprom = EEPROM.read(random_address);
  EEPROM.write(random_address,255);
if (EEPROM.commit()){

  tft.setCursor(195,150, 2);
  tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
  tft.println("SUCCESS");

  tft.setCursor(260,150, 2);
  tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
  tft.println("@");

    tft.setCursor(270,150, 2);
  tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
  tft.println(random_address);

  }else{
  tft.setCursor(195,150, 2);
  tft.setTextColor(TFT_RED);  tft.setTextSize(1);
  tft.println("ERROR");

  tft.setCursor(260,150, 2);
  tft.setTextColor(TFT_RED);  tft.setTextSize(1);
  tft.println("@");

  tft.setCursor(270,150, 2);
  tft.setTextColor(TFT_RED);  tft.setTextSize(1);
  tft.println(random_address);

  }

  EEPROM.write(random_address,data_eeprom);
  EEPROM.commit();


//testo la eeprom ad address_random_3:

random_address = -1;

  delay(200);

  tft.setCursor(2,170, 2);
  tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
  tft.println("testing random address (3/5): ");


while (random_address < 0 || random_address > EEPROM_SIZE){
random_address = esp_random();
}

  data_eeprom = EEPROM.read(random_address);
  EEPROM.write(random_address,255);
if (EEPROM.commit()){

  tft.setCursor(195,170, 2);
  tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
  tft.println("SUCCESS");

  tft.setCursor(260,170, 2);
  tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
  tft.println("@");

    tft.setCursor(270,170, 2);
  tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
  tft.println(random_address);

  }else{
  tft.setCursor(195,170, 2);
  tft.setTextColor(TFT_RED);  tft.setTextSize(1);
  tft.println("ERROR");

  tft.setCursor(260,170, 2);
  tft.setTextColor(TFT_RED);  tft.setTextSize(1);
  tft.println("@");

  tft.setCursor(270,170, 2);
  tft.setTextColor(TFT_RED);  tft.setTextSize(1);
  tft.println(random_address);

  }

  EEPROM.write(random_address,data_eeprom);
  EEPROM.commit();


//testo la eeprom ad address_random_4:

random_address = -1;

  delay(200);

  tft.setCursor(2,190, 2);
  tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
  tft.println("testing random address (4/5): ");


while (random_address < 0 || random_address > EEPROM_SIZE){
random_address = esp_random();
}

  data_eeprom = EEPROM.read(random_address);
  EEPROM.write(random_address,255);
if (EEPROM.commit()){

  tft.setCursor(195,190, 2);
  tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
  tft.println("SUCCESS");

  tft.setCursor(260,190, 2);
  tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
  tft.println("@");

    tft.setCursor(270,190, 2);
  tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
  tft.println(random_address);

  }else{
  tft.setCursor(195,190, 2);
  tft.setTextColor(TFT_RED);  tft.setTextSize(1);
  tft.println("ERROR");

  tft.setCursor(260,190, 2);
  tft.setTextColor(TFT_RED);  tft.setTextSize(1);
  tft.println("@");

  tft.setCursor(270,190, 2);
  tft.setTextColor(TFT_RED);  tft.setTextSize(1);
  tft.println(random_address);

  }

  EEPROM.write(random_address,data_eeprom);
  EEPROM.commit();


//testo la eeprom ad address_random_5:

random_address = -1;

  delay(200);

  tft.setCursor(2,210, 2);
  tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
  tft.println("testing random address (5/5): ");


while (random_address < 0 || random_address > EEPROM_SIZE){
random_address = esp_random();
}

  data_eeprom = EEPROM.read(random_address);
  EEPROM.write(random_address,255);
  if (EEPROM.commit()){
  

  tft.setCursor(195,210, 2);
  tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
  tft.println("SUCCESS");

  tft.setCursor(260,210, 2);
  tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
  tft.println("@");

    tft.setCursor(270,210, 2);
  tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
  tft.println(random_address);

  }else{
  tft.setCursor(195,210, 2);
  tft.setTextColor(TFT_RED);  tft.setTextSize(1);
  tft.println("ERROR");

  tft.setCursor(260,210, 2);
  tft.setTextColor(TFT_RED);  tft.setTextSize(1);
  tft.println("@");

  tft.setCursor(270,210, 2);
  tft.setTextColor(TFT_RED);  tft.setTextSize(1);
  tft.println(random_address);

  }

  EEPROM.write(random_address,data_eeprom);
  EEPROM.commit();

  delay(1000);

  //DOPO AVER FATTO IL TEST DELLA EEPROM FACCIO l'init DEL GPS:

  tft.fillRect(0,110,320,240, TFT_BLACK); //cancello la zona in cui avevo scritto della eeproom

  tft.setCursor(2,110, 2);
  tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
  tft.println(">> Starting GPS module: (serial only)");

  delay(100);

  tft.setCursor(2,130, 2);
  tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
  tft.println("Initializing the SoftwareSerial communication...");

  ss.begin(GPSBaud);

  delay(200);

  tft.setCursor(2,150, 2);
  tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
  tft.println("Done");

  delay(200);






start_screen = 0;
active_menu = 1;

}

// MENU --------------------------------------------------------------------------------------------------------------------------------


if (active_menu == 1){

if (active_menu_animated == 1 ){

// Infine comincio a fare il vero e proprio boot dentro il programma:

tft.fillRect(0,16,320,240, TFT_BLACK);
delay(100);
tft.drawLine(0,220, 360, 220,TFT_WHITE);

  tft.setCursor(2,30, 2);
  tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
  tft.println("ERASE EEPROM");

  delay(100);

  tft.setCursor(2,50, 2);
  tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
  tft.println("CHECK EEPROM INTEGRITY");

    delay(100);

tft.setCursor(2,70, 2);
  tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
  tft.println("DUMP EEPROM (SERIAL)");

  delay(100);

  tft.setCursor(2,90, 2);
  tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
  tft.println("DUMP EEPROM DATA (SERIAL)");

    delay(100);

  tft.setCursor(2,110, 2);
  tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
  tft.println("DUMP EEPROM DATA (SCREEN)");

    delay(100);

  tft.setCursor(2,130, 2);
  tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
  tft.println("SCAN ALL NETWORK SSID's");

    delay(100);

  tft.setCursor(2,150, 2);
  tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
  tft.println("SAVE CURRENT COORDINATES");

    delay(100);

    tft.setCursor(2,170, 2);
  tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
  tft.println("SAVE BOTH COORDINATES AND SSID's");

    delay(100);

  tft.setCursor(2,190, 2);
  tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
  tft.println("SHOW CPU INFO");

  delay(100);

  tft.setCursor(2,225, 2);
  tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
  tft.println("lat:");

    delay(100);

  tft.setCursor(120,225, 2);
  tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
  tft.println("lon:");

  delay(200);

  tft.setCursor(20,225, 2);
      tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
      tft.println(" --- ");

  delay(200);

    tft.setCursor(140,225, 2);
      tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
      tft.println(" --- ");

    delay(200);

    tft.setCursor(240,225, 2);
      tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
      tft.println("Sats:");

       delay(200);

    tft.setCursor(270,225, 2);
      tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
      tft.println(" -- ");


  tft.setCursor(300,30, 2);
  tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
  tft.println("<<");

active_menu_animated = 0;

}

//eseguo le istruzioni per il gps:
while (ss.available() > 0)
    if (gps.encode(ss.read()))
      if (gps.location.isValid()){

      tft.fillRect(20,225,90,20, TFT_BLACK);
      tft.fillRect(140,225,90,20, TFT_BLACK);
      tft.fillRect(270,225,50,20, TFT_BLACK);

      tft.setCursor(30,225, 2);
      tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
      tft.println(gps.location.lat(), 6);

      tft.setCursor(150,225, 2);
      tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
      tft.println(gps.location.lng(), 6);

      tft.setCursor(280,225, 2);
      tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
      tft.println(gps.satellites.value());
  

  }

      




//DOPO AVER DETERMINATO IL VALORE DEL POINTER, LO RENDRIZZO SULLO SCHERMO (DEFAULT 0)

  



//int pointer_value = 0;
//bool option_1 = false;
//bool option_2 = false;
//bool option_3 = false;
//bool option_4 = false;
//bool option_5 = false;
//bool option_6 = false;
//bool option_7 = false;
//bool option_8 = false;
//bool option_9 = false;

//bool button_1_last_state = false;
//bool button_2_last_state = false;
//bool button_3_last_state = false;
//bool button_4_last_state = false;

//DETERMINO IL VALORE DEL PUNTATORE:

if (digitalRead(Button_3) == HIGH && button_3_last_state == false ){
button_3_last_state = true;

tft.fillRect(300,30 + ( 10  * pointer_value),310,20+( 10 * pointer_value ), TFT_BLACK);

pointer_value = pointer_value +1;

if (pointer_value < 0){

pointer_value = 8;

}

if (pointer_value > 8){

pointer_value = 0;

}

tft.setCursor(300,30 + ( 20  * pointer_value), 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("<<");

}

if (digitalRead(Button_3) == LOW && button_3_last_state == true ){
button_3_last_state = false;


}


if (digitalRead(Button_4) == HIGH && button_4_last_state == false ){
button_4_last_state = true;

tft.fillRect(300,30 + ( 10  * pointer_value),310,20+( 10 * pointer_value ), TFT_BLACK);

pointer_value = pointer_value -1;

if (pointer_value < 0){

pointer_value = 8;

}

if (pointer_value > 8){

pointer_value = 0;

}
  
tft.setCursor(300,30 + ( 20  * pointer_value), 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("<<");



}

if (digitalRead(Button_4) == LOW && button_4_last_state == true ){
button_4_last_state = false;


}

//bottoni per seleziona


if (digitalRead(Button_2) == HIGH && button_2_last_state == false ){
button_2_last_state = true;

if (pointer_value == 0){
option_1 = true;
option_1_animated = true;
active_menu = 0;
delay(100);
tft.fillRect(0,16,320,240, TFT_BLACK);
}
if (pointer_value == 1){
option_2 = true;
option_2_animated = true;
active_menu = 0;
delay(100);
tft.fillRect(0,16,320,240, TFT_BLACK);
}
if (pointer_value == 2){
option_3 = true;
option_3_animated = true; 
active_menu = 0;
delay(100);
tft.fillRect(0,16,320,240, TFT_BLACK);
}
if (pointer_value == 3){
option_4 = true;
option_4_animated = true;
active_menu = 0;
delay(100);
tft.fillRect(0,16,320,240, TFT_BLACK);
}
if (pointer_value == 4){
option_5 = true;
option_5_animated = true;
active_menu = 0;
delay(100);
tft.fillRect(0,16,320,240, TFT_BLACK);
}
if (pointer_value == 5){
option_6 = true;
option_6_animated = true;
active_menu = 0;
delay(100);
tft.fillRect(0,16,320,240, TFT_BLACK);
}
if (pointer_value == 6){
option_7 = true;
option_7_animated = true;
active_menu = 0;
delay(100);
tft.fillRect(0,16,320,240, TFT_BLACK);
}
if (pointer_value == 7){
option_8 = true;
option_8_animated = true;
active_menu = 0;
delay(100);
tft.fillRect(0,16,320,240, TFT_BLACK);
}
if (pointer_value == 8){
option_9 = true;
option_9_animated = true;
active_menu = 0;
delay(100);
tft.fillRect(0,16,320,240, TFT_BLACK);
}

}

if (digitalRead(Button_2) == LOW && button_2_last_state == true ){
button_2_last_state = false;

}







  }

// OPTION_1 (ERASE EEPROM) --------------------------------------------------------------------------------------------------------------------------------

if (option_1){

if (option_1_animated){

delay(100);

tft.setCursor(2, 30, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Setting up for complete eeprom erase...");

delay(100);

tft.setCursor(2,50, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Checking EEPROM SIZE:");

delay(200);

tft.setCursor(200,50, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println(EEPROM_SIZE);

delay(100);

tft.setCursor(2,90, 2);
tft.setTextColor(TFT_RED);  tft.setTextSize(1);
tft.println("THIS OPERATION WILL DELETE ANY STORED DATA");

delay(100);

tft.drawLine(20,120, 300, 120,TFT_WHITE);
delay(50);
tft.drawLine(20,120, 20, 150,TFT_WHITE);
delay(50);
tft.drawLine(300,120, 300, 150,TFT_WHITE);
delay(50);
tft.drawLine(20,150, 300, 150,TFT_WHITE);

delay(100);

tft.setCursor(2,200, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Do you want to continue?");

delay(150);

tft.setCursor(50,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("YES");

delay(100);

tft.setCursor(250,220, 2);
tft.setTextColor(TFT_RED);  tft.setTextSize(1);
tft.println("NO");

delay(100);

tft.setCursor(280,170, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("%");

delay(100);

tft.setCursor(250,170, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("000");

delay(100);


tft.setCursor(40,170, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("@");

delay(100);

tft.setCursor(60,170, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("-----");

delay(200);

tft.setCursor(280,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("<<");


option_1_animated = false;
pointer_value = 0;
} 

if (digitalRead(Button_3) == HIGH && button_3_last_state == false ){
button_3_last_state = true;

tft.fillRect(280 - (200 * pointer_value),220,300 - (200 * pointer_value) ,230, TFT_BLACK);

pointer_value = pointer_value +1;

if (pointer_value > 1){
  pointer_value = 0;
}
if (pointer_value < 0){
  pointer_value = 1;
}

tft.setCursor(280 - (200 * pointer_value) ,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("<<");



}

if (digitalRead(Button_3) == LOW && button_3_last_state == true ){
button_3_last_state = false;

}


if (digitalRead(Button_4) == HIGH && button_4_last_state == false ){
button_4_last_state = true;

tft.fillRect(280 - (200 * pointer_value),220,300 - (200 * pointer_value) ,230, TFT_BLACK);

pointer_value = pointer_value -1;

if (pointer_value > 1){
  pointer_value = 0;
}
if (pointer_value < 0){
  pointer_value = 1;
}

tft.setCursor(280 - (200 * pointer_value) ,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("<<");



}

if (digitalRead(Button_4) == LOW && button_4_last_state == true ){
button_4_last_state = false;

}




if (digitalRead(Button_2) == HIGH && button_2_last_state == false ){
button_2_last_state = true;

if (pointer_value == 0){

option_1_animated = true;
option_1 = false;
active_menu = 1;
active_menu_animated = 1;
pointer_value = 0;

}

if (pointer_value == 1){

//variabili_option_1:
/*
int valore_percentuale = 0;
int value_barra = 0;
*/

for (int i = 0; i < EEPROM_SIZE; i++){ //LO FACCIO PARTIRE DA UNO PECHE' NELL'ADDRESS 0 CI SARAà MEMORIZZATA LA DIMENSIONE DELLA EEPROM.
EEPROM.write(i,255);

valore_percentuale = (100 * i) / (EEPROM_SIZE -1);
value_barra = (20 * i) / (EEPROM_SIZE -1);
value_barra -= 1;

tft.fillRect(60,170,60,20,TFT_BLACK); //CANCELLO L'ADDRESS
tft.fillRect(250,170,30,20,TFT_BLACK); //CANCELLO LA PERCENTUALE


//scrivo l'address
tft.setCursor(60,170, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println(i);

//scrivo la percentuale
tft.setCursor(250,170, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println(valore_percentuale);

//disegno la barra di caricamento

if (value_barra >= 0){

tft.fillRect(21  + (14 * value_barra) ,121,10,28,TFT_GREEN); //DISEGNO LA BARRA

}

}

EEPROM.commit();

delay(2000);

//leaves
option_1_animated = true;
option_1 = false;
active_menu = 1;
active_menu_animated = 1;
pointer_value = 0;


}


}

if (digitalRead(Button_2) == LOW && button_2_last_state == true ){
button_2_last_state = false;

}

}


// OPTION_2 (CHECK EEPROM) --------------------------------------------------------------------------------------------------------------------------------

if (option_2){

if (option_2_animated){

delay(100);

tft.setCursor(2, 30, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Setting up for eeprom check");

delay(100);

tft.setCursor(2,50, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Checking EEPROM SIZE:");

delay(200);

tft.setCursor(200,50, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println(EEPROM_SIZE);

delay(100);

tft.setCursor(30,90, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("NO STORED DATA WILL BE AFFECTED");

delay(100);

tft.drawLine(20,120, 300, 120,TFT_WHITE);
delay(50);
tft.drawLine(20,120, 20, 150,TFT_WHITE);
delay(50);
tft.drawLine(300,120, 300, 150,TFT_WHITE);
delay(50);
tft.drawLine(20,150, 300, 150,TFT_WHITE);

delay(100);

tft.setCursor(2,200, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Do you want to continue?");

delay(150);

tft.setCursor(50,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("YES");

delay(100);

tft.setCursor(250,220, 2);
tft.setTextColor(TFT_RED);  tft.setTextSize(1);
tft.println("NO");

delay(100);

tft.setCursor(280,170, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("%");

delay(100);

tft.setCursor(250,170, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("000");

delay(100);


tft.setCursor(40,170, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("@");

delay(100);

tft.setCursor(60,170, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("-----");

delay(200);

tft.setCursor(120,170, 2);
tft.setTextColor(TFT_RED);  tft.setTextSize(1);
tft.println("Corrupted:");

delay(200);

tft.setCursor(190,170, 2);
tft.setTextColor(TFT_RED);  tft.setTextSize(1);
tft.println("0");

delay(200);

tft.setCursor(280,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("<<");


option_2_animated = false;
pointer_value = 0;
} 

if (digitalRead(Button_3) == HIGH && button_3_last_state == false ){
button_3_last_state = true;

tft.fillRect(280 - (200 * pointer_value),220,300 - (200 * pointer_value) ,230, TFT_BLACK);

pointer_value = pointer_value +1;

if (pointer_value > 1){
  pointer_value = 0;
}
if (pointer_value < 0){
  pointer_value = 1;
}

tft.setCursor(280 - (200 * pointer_value) ,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("<<");



}

if (digitalRead(Button_3) == LOW && button_3_last_state == true ){
button_3_last_state = false;

}


if (digitalRead(Button_4) == HIGH && button_4_last_state == false ){
button_4_last_state = true;

tft.fillRect(280 - (200 * pointer_value),220,300 - (200 * pointer_value) ,230, TFT_BLACK);

pointer_value = pointer_value -1;

if (pointer_value > 1){
  pointer_value = 0;
}
if (pointer_value < 0){
  pointer_value = 1;
}

tft.setCursor(280 - (200 * pointer_value) ,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("<<");



}

if (digitalRead(Button_4) == LOW && button_4_last_state == true ){
button_4_last_state = false;

}




if (digitalRead(Button_2) == HIGH && button_2_last_state == false ){
button_2_last_state = true;

if (pointer_value == 0){

option_2_animated = true;
option_2 = false;
active_menu = 1;
active_menu_animated = 1;
pointer_value = 0;

}

if (pointer_value == 1){

//variabili_option_2:

/*
int data_read = 0;
int corruptions = 0;
*/

for (int i = 0; i < EEPROM_SIZE; i++){ //LO FACCIO PARTIRE DA UNO PECHE' NELL'ADDRESS 0 CI SARAà MEMORIZZATA LA DIMENSIONE DELLA EEPROM.

//ESEGUE LE OPERAZIONI

data_read = EEPROM.read(i);
EEPROM.write(i,255);
EEPROM.commit();
if (EEPROM.read(i) != 255){
  corruptions += 1;
}
EEPROM.write(i,data_read);
EEPROM.commit();



valore_percentuale = (100 * i) / (EEPROM_SIZE -1);
value_barra = (20 * i) / (EEPROM_SIZE -1);
value_barra -= 1;

tft.fillRect(60,170,60,20,TFT_BLACK); //CANCELLO L'ADDRESS
tft.fillRect(250,170,30,20,TFT_BLACK); //CANCELLO LA PERCENTUALE
tft.fillRect(190,170,30,20,TFT_BLACK); //CANCELLO LE CORRUPTIONS


//scrivo l'address
tft.setCursor(60,170, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println(i);

//scrivo la percentuale
tft.setCursor(250,170, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println(valore_percentuale);

//scrivo le corruptions:
tft.setCursor(190,170, 2);
tft.setTextColor(TFT_RED);  tft.setTextSize(1);
tft.println(corruptions);


//disegno la barra di caricamento

if (value_barra >= 0){

tft.fillRect(21  + (14 * value_barra) ,121,10,28,TFT_GREEN); //DISEGNO LA BARRA

}

}


delay(2000);

//leaves
option_2_animated = true;
option_2 = false;
active_menu = 1;
active_menu_animated = 1;
pointer_value = 0;


}


}

if (digitalRead(Button_2) == LOW && button_2_last_state == true ){
button_2_last_state = false;

}
}




// OPTION_3 (DUMP EEPROM (SERIAL)) --------------------------------------------------------------------------------------------------------------------------------

if (option_3){

if (option_3_animated){

delay(100);

tft.setCursor(2, 30, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Setting up for eeprom dump over serial");

delay(100);

tft.setCursor(2,50, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Checking EEPROM SIZE:");

delay(200);

tft.setCursor(200,50, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println(EEPROM_SIZE);

delay(100);

tft.setCursor(10,90, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("Serial baudrate is 115200 - connect over usb.");

delay(100);

tft.drawLine(20,120, 300, 120,TFT_WHITE);
delay(50);
tft.drawLine(20,120, 20, 150,TFT_WHITE);
delay(50);
tft.drawLine(300,120, 300, 150,TFT_WHITE);
delay(50);
tft.drawLine(20,150, 300, 150,TFT_WHITE);

delay(100);

tft.setCursor(2,200, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Do you want to continue?");

delay(150);

tft.setCursor(50,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("YES");

delay(100);

tft.setCursor(250,220, 2);
tft.setTextColor(TFT_RED);  tft.setTextSize(1);
tft.println("NO");

delay(100);

tft.setCursor(280,170, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("%");

delay(100);

tft.setCursor(250,170, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("000");

delay(100);


tft.setCursor(40,170, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("@");

delay(100);

tft.setCursor(60,170, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("-----");

delay(200);

tft.setCursor(280,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("<<");


option_3_animated = false;
pointer_value = 0;


}

if (digitalRead(Button_3) == HIGH && button_3_last_state == false ){
button_3_last_state = true;

tft.fillRect(280 - (200 * pointer_value),220,300 - (200 * pointer_value) ,230, TFT_BLACK);

pointer_value = pointer_value +1;

if (pointer_value > 1){
  pointer_value = 0;
}
if (pointer_value < 0){
  pointer_value = 1;
}

tft.setCursor(280 - (200 * pointer_value) ,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("<<");



}

if (digitalRead(Button_3) == LOW && button_3_last_state == true ){
button_3_last_state = false;

}


if (digitalRead(Button_4) == HIGH && button_4_last_state == false ){
button_4_last_state = true;

tft.fillRect(280 - (200 * pointer_value),220,300 - (200 * pointer_value) ,230, TFT_BLACK);

pointer_value = pointer_value -1;

if (pointer_value > 1){
  pointer_value = 0;
}
if (pointer_value < 0){
  pointer_value = 1;
}

tft.setCursor(280 - (200 * pointer_value) ,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("<<");



}

if (digitalRead(Button_4) == LOW && button_4_last_state == true ){
button_4_last_state = false;

}




if (digitalRead(Button_2) == HIGH && button_2_last_state == false ){
button_2_last_state = true;

if (pointer_value == 0){

option_3_animated = true;
option_3 = false;
active_menu = 1;
active_menu_animated = 1;
pointer_value = 0;

}

if (pointer_value == 1){

//variabili_option_2:

/*
int data_read = 0;
int corruptions = 0;
*/

Serial.println("--------------------------------------------");
Serial.println("Starting EEPROM dump over serial...");
Serial.println("Baudrate: 115200");
Serial.print("EEPROM SIZE:");
Serial.print(EEPROM_SIZE);
Serial.println("--------------------------------------------");
Serial.println("");


for (int i = 0; i < EEPROM_SIZE; i++){ //LO FACCIO PARTIRE DA UNO PECHE' NELL'ADDRESS 0 CI SARAà MEMORIZZATA LA DIMENSIONE DELLA EEPROM.

//ESEGUE LE OPERAZIONI
Serial.print("Address: @ ");
Serial.print(i);
Serial.print("   ");
Serial.print("Data: ");
Serial.println(EEPROM.read(i));

valore_percentuale = (100 * i) / (EEPROM_SIZE -1);
value_barra = (20 * i) / (EEPROM_SIZE -1);
value_barra -= 1;


tft.fillRect(60,170,60,20,TFT_BLACK); //CANCELLO L'ADDRESS
tft.fillRect(250,170,30,20,TFT_BLACK); //CANCELLO LA PERCENTUALE



//scrivo l'address
tft.setCursor(60,170, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println(i);

//scrivo la percentuale
tft.setCursor(250,170, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println(valore_percentuale);


//disegno la barra di caricamento

if (value_barra >= 0){

tft.fillRect(21  + (14 * value_barra) ,121,10,28,TFT_GREEN); //DISEGNO LA BARRA

}

}


Serial.println("TRANSMISSION ENDED");

delay(2000);

//leaves
option_3_animated = true;
option_3 = false;
active_menu = 1;
active_menu_animated = 1;
pointer_value = 0;

}

}




if (digitalRead(Button_2) == LOW && button_2_last_state == true ){
button_2_last_state = false;

}
}







// OPTION_4 (DUMP DATA (SERIAL)) --------------------------------------------------------------------------------------------------------------------------------

if (option_4){

if (option_4_animated){

delay(100);

tft.setCursor(2, 30, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Setting up for eeprom data dump over serial");

delay(100);

tft.setCursor(2,50, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Checking EEPROM SIZE:");

delay(200);

tft.setCursor(200,50, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println(EEPROM_SIZE);

delay(100);

tft.setCursor(10,70, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("Translation protocol: ASCII");

delay(100);

tft.setCursor(10,90, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("Serial baudrate is 115200 - connect over usb.");

delay(100);

tft.drawLine(20,120, 300, 120,TFT_WHITE);
delay(50);
tft.drawLine(20,120, 20, 150,TFT_WHITE);
delay(50);
tft.drawLine(300,120, 300, 150,TFT_WHITE);
delay(50);
tft.drawLine(20,150, 300, 150,TFT_WHITE);

delay(100);

tft.setCursor(2,200, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Do you want to continue?");

delay(150);

tft.setCursor(50,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("YES");

delay(100);

tft.setCursor(250,220, 2);
tft.setTextColor(TFT_RED);  tft.setTextSize(1);
tft.println("NO");

delay(100);

tft.setCursor(280,170, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("%");

delay(100);

tft.setCursor(250,170, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("000");

delay(100);


tft.setCursor(40,170, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("@");

delay(100);

tft.setCursor(60,170, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("-----");

delay(200);

tft.setCursor(280,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("<<");


option_4_animated = false;
pointer_value = 0;


}

if (digitalRead(Button_3) == HIGH && button_3_last_state == false ){
button_3_last_state = true;

tft.fillRect(280 - (200 * pointer_value),220,300 - (200 * pointer_value) ,230, TFT_BLACK);

pointer_value = pointer_value +1;

if (pointer_value > 1){
  pointer_value = 0;
}
if (pointer_value < 0){
  pointer_value = 1;
}

tft.setCursor(280 - (200 * pointer_value) ,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("<<");



}

if (digitalRead(Button_3) == LOW && button_3_last_state == true ){
button_3_last_state = false;

}


if (digitalRead(Button_4) == HIGH && button_4_last_state == false ){
button_4_last_state = true;

tft.fillRect(280 - (200 * pointer_value),220,300 - (200 * pointer_value) ,230, TFT_BLACK);

pointer_value = pointer_value -1;

if (pointer_value > 1){
  pointer_value = 0;
}
if (pointer_value < 0){
  pointer_value = 1;
}

tft.setCursor(280 - (200 * pointer_value) ,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("<<");



}

if (digitalRead(Button_4) == LOW && button_4_last_state == true ){
button_4_last_state = false;

}




if (digitalRead(Button_2) == HIGH && button_2_last_state == false ){
button_2_last_state = true;

if (pointer_value == 0){

option_4_animated = true;
option_4 = false;
active_menu = 1;
active_menu_animated = 1;
pointer_value = 0;

}

if (pointer_value == 1){

//variabili_option_2:

/*
int data_read = 0;
int corruptions = 0;
*/

Serial.println("--------------------------------------------");
Serial.println("Starting EEPROM dump over serial...");
Serial.println("Baudrate: 115200");
Serial.print("EEPROM SIZE:");
Serial.print(EEPROM_SIZE);
Serial.println("--------------------------------------------");
Serial.println("");


for (int i = 0; i < EEPROM_SIZE; i++){ 

//ESEGUE LE OPERAZIONI
Serial.print("Address: @ ");
Serial.print(i);
Serial.print("   ");
Serial.print("Data: ");
Serial.println((char) EEPROM.read(i));

valore_percentuale = (100 * i) / (EEPROM_SIZE -1);
value_barra = (20 * i) / (EEPROM_SIZE -1);
value_barra -= 1;


tft.fillRect(60,170,60,20,TFT_BLACK); //CANCELLO L'ADDRESS
tft.fillRect(250,170,30,20,TFT_BLACK); //CANCELLO LA PERCENTUALE



//scrivo l'address
tft.setCursor(60,170, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println(i);

//scrivo la percentuale
tft.setCursor(250,170, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println(valore_percentuale);


//disegno la barra di caricamento

if (value_barra >= 0){

tft.fillRect(21  + (14 * value_barra) ,121,10,28,TFT_GREEN); //DISEGNO LA BARRA

}

}

Serial.println("");
Serial.println("LINE DUMP:");
Serial.println("");

for (int i = 0; i < EEPROM_SIZE; i++){ 

//ESEGUE LE OPERAZIONI
Serial.print((char) EEPROM.read(i));

}

Serial.println("");
Serial.println("TRANSMISSION ENDED");

delay(2000);

//leaves
option_4_animated = true;
option_4 = false;
active_menu = 1;
active_menu_animated = 1;
pointer_value = 0;

}

}




if (digitalRead(Button_2) == LOW && button_2_last_state == true ){
button_2_last_state = false;

}
}










// OPTION_5 (DUMP DATA (SCREEN)) --------------------------------------------------------------------------------------------------------------------------------

//IDEA: Abbiamo un certo numero di caratteri che possono essere disengati sullo schermo, una volta che sono stati disegnati, per andare avanti bisongna usare un bottone per renderizzare il successivo range di caratteri.


if (option_5){

if (option_5_animated){

delay(100);

tft.setCursor(10, 30, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("ON SCREEN EEPROM DATA DUMP:");

delay(100);

tft.setCursor(230, 30, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println(EEPROM_SIZE);

delay(100);

tft.setCursor(280, 30, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("bytes");

delay(100);

tft.setCursor(2,200, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Do you want to continue?");

delay(150);

tft.setCursor(50,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("YES");

delay(100);

tft.setCursor(250,220, 2);
tft.setTextColor(TFT_RED);  tft.setTextSize(1);
tft.println("NO");

delay(200);



tft.drawLine(5,50, 315, 50,TFT_WHITE);
delay(50);
tft.drawLine(5,50, 5, 190,TFT_WHITE);
delay(50);
tft.drawLine(315,50, 315, 190,TFT_WHITE);
delay(50);
tft.drawLine(5,190, 315, 190,TFT_WHITE);

delay(200);

tft.setCursor(280,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("<<");



option_5_animated = false;
pointer_value = 0;

}

if (change_ui == 0){


if (digitalRead(Button_3) == HIGH && button_3_last_state == false ){
button_3_last_state = true;

tft.fillRect(280 - (200 * pointer_value),220,300 - (200 * pointer_value) ,230, TFT_BLACK);

pointer_value = pointer_value +1;

if (pointer_value > 1){
  pointer_value = 0;
}
if (pointer_value < 0){
  pointer_value = 1;
}

tft.setCursor(280 - (200 * pointer_value) ,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("<<");



}

if (digitalRead(Button_3) == LOW && button_3_last_state == true ){
button_3_last_state = false;

}


if (digitalRead(Button_4) == HIGH && button_4_last_state == false ){
button_4_last_state = true;

tft.fillRect(280 - (200 * pointer_value),220,300 - (200 * pointer_value) ,230, TFT_BLACK);

pointer_value = pointer_value -1;

if (pointer_value > 1){
  pointer_value = 0;
}
if (pointer_value < 0){
  pointer_value = 1;
}

tft.setCursor(280 - (200 * pointer_value) ,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("<<");



}

if (digitalRead(Button_4) == LOW && button_4_last_state == true ){
button_4_last_state = false;

}







if (digitalRead(Button_2) == HIGH && button_2_last_state == false ){
button_2_last_state = true;

if (pointer_value == 0){

option_5_animated = true;
option_5 = false;
active_menu = 1;
active_menu_animated = 1;
pointer_value = 0;
 change_ui = 0;
 frame = 0;
 change_ui_animated = 1;
 delete_screen = 0;
 requires_rendering = 0;

}

if (pointer_value == 1){

change_ui = 1;

}
}


if (digitalRead(Button_2) == LOW && button_2_last_state == true ){
button_2_last_state = false;

}
}

//se seleziono si comincia con tutto il processo:

if (change_ui == 1){

if (change_ui_animated == 1){

tft.fillRect(0,200,320,40,TFT_BLACK); //CANCELLO tutto quello che era presente sino a quel momento

delay(100);

tft.setCursor(2,200, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Frame:");

delay(100);

tft.setCursor(2,220, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Addresses:");

delay(100);

tft.setCursor(100,200, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("-  /  -");

delay(100);

tft.setCursor(100,220, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("-----  -  -----");

delay(100);

tft.setCursor(70,80, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Use button 3 and 4 to scroll");

delay(100);

tft.setCursor(110,100, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("between frames");

delay(100);

tft.setCursor(60,130, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Use button 1 to return to menu");


change_ui_animated = 0;
pointer_value = 0;
frame = -1;

}

frame_total = (EEPROM_SIZE / 180) +1;


if (frame == -1){

tft.fillRect(100,200,15,20,TFT_BLACK); //CANCELLO IL CONTATORE NELLA PARTE A SINISTRA

tft.setCursor(100,200, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("0");

tft.fillRect(130,200,15,25,TFT_BLACK); //CANCELLO IL CONTATORE DEI FRAME NELLA PARTE A DESTRA

tft.setCursor(135,200, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println(frame_total);


}

if (digitalRead(Button_1) == HIGH && button_1_last_state == false ){
button_1_last_state = true;

//esce

option_5_animated = true;
option_5 = false;
active_menu = 1;
active_menu_animated = 1;
pointer_value = 0;
 change_ui = 0;
 frame = 0;
 change_ui_animated = 1;
 delete_screen = 0;
 requires_rendering = 0;


}

if (digitalRead(Button_1) == LOW && button_1_last_state == true ){
button_1_last_state = false;

}




//verso alto

if (digitalRead(Button_4) == HIGH && button_4_last_state == false ){
button_4_last_state = true;

if (frame == -1){
  frame = frame_total;
}

frame -= 1;

if (frame < 1){
  frame = frame_total;
}

if (frame > frame_total){
  frame = 1;
}

//abilita il rendering del frame
requires_rendering = 1;
delete_screen = 1;

}

if (digitalRead(Button_4) == LOW && button_4_last_state == true ){
button_4_last_state = false;

}





//verso basso

if (digitalRead(Button_3) == HIGH && button_3_last_state == false ){
button_3_last_state = true;

if (frame == -1){
  frame = 1;
}

frame += 1;

if (frame < 1){
  frame = frame_total;
}

if (frame > frame_total){
  frame = 1;
}

//abilita il rendering del frame
requires_rendering = 1;
delete_screen = 1;


}

if (digitalRead(Button_3) == LOW && button_3_last_state == true ){
button_3_last_state = false;

}


// REQUIRES RENDERING

if (requires_rendering == 1){

//cancella lo schermo (una sola volta):
if (delete_screen == 1){

  tft.fillRect(6,51,309,139,TFT_BLACK); //CANCELLO TUTTA LA CASELLA DI RENDERING

   tft.fillRect(100,220,40,20,TFT_BLACK); //CANCELLO la parte del primo address
    tft.fillRect(160,220,55,20,TFT_BLACK); //CANCELLO la parte del secondo address

  //disegno il primo address:

  tft.setCursor(100,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println(180 * (frame -1));

tft.setCursor(90,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("@");

  //disegno il secondo address:

tft.setCursor(160,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("@");

  tft.setCursor(170,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println(180 * frame);


//render

tft.fillRect(100,200,15,20,TFT_BLACK); //CANCELLO IL CONTATORE NELLA PARTE A SINISTRA

tft.setCursor(100,200, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println(frame);

tft.fillRect(130,200,25,20,TFT_BLACK); //CANCELLO IL CONTATORE DEI FRAME NELLA PARTE A DESTRA

tft.setCursor(135,200, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println(frame_total);



  //comincia a creare la griglia di linee:




  //disegno le linee:

  tft.drawLine(5 +1 ,65, 315 -1 , 65,TFT_COLOR_1);
  delay(lines_delay);
  tft.drawLine(20,50 +1, 20, 190 -1,TFT_COLOR_1);
 delay(lines_delay);

  tft.drawLine(5 +1 ,80, 315 -1 , 80,TFT_COLOR_1);
  delay(lines_delay);
  tft.drawLine(35,50 +1, 35, 190 -1,TFT_COLOR_1);
  delay(lines_delay);

  tft.drawLine(5 +1 ,95, 315 -1 , 95,TFT_COLOR_1);
  delay(lines_delay);
  tft.drawLine(50,50 +1, 50, 190 -1,TFT_COLOR_1);
  delay(lines_delay);

  tft.drawLine(5 +1 ,110, 315 -1 , 110,TFT_COLOR_1);
  delay(lines_delay);
  tft.drawLine(65,50 +1, 65, 190 -1,TFT_COLOR_1);
  delay(lines_delay);

  tft.drawLine(5 +1 ,125, 315 -1 , 125,TFT_COLOR_1);
  delay(lines_delay);
  tft.drawLine(80,50 +1, 80, 190 -1,TFT_COLOR_1);
  delay(lines_delay);

  tft.drawLine(5 +1 ,140, 315 -1 , 140,TFT_COLOR_1);
  delay(lines_delay);
  tft.drawLine(95,50 +1, 95, 190 -1,TFT_COLOR_1);
  delay(lines_delay);

  tft.drawLine(5 +1 ,155, 315 -1 , 155,TFT_COLOR_1);
  delay(lines_delay);
  tft.drawLine(110,50 +1, 110, 190 -1,TFT_COLOR_1);
  delay(lines_delay);

    tft.drawLine(5 +1 ,170, 315 -1 , 170,TFT_COLOR_1);
  delay(lines_delay);
  tft.drawLine(125,50 +1, 125, 190 -1,TFT_COLOR_1);
  delay(lines_delay);

  tft.drawLine(5 +1 ,185, 315 -1 , 185,TFT_COLOR_1);
  delay(lines_delay);
  tft.drawLine(140,50 +1, 140, 190 -1,TFT_COLOR_1);
  delay(lines_delay);

  tft.drawLine(155,50 +1, 155, 190 -1,TFT_COLOR_1);
  delay(lines_delay);

  tft.drawLine(170,50 +1, 170, 190 -1,TFT_COLOR_1);
  delay(lines_delay);

    tft.drawLine(185,50 +1, 185, 190 -1,TFT_COLOR_1);
  delay(lines_delay);

  tft.drawLine(200,50 +1, 200, 190 -1,TFT_COLOR_1);
  delay(lines_delay);

  tft.drawLine(215,50 +1, 215, 190 -1,TFT_COLOR_1);
  delay(lines_delay);

  tft.drawLine(230,50 +1, 230, 190 -1,TFT_COLOR_1);
  delay(lines_delay);

  tft.drawLine(245,50 +1, 245, 190 -1,TFT_COLOR_1);
  delay(lines_delay);

    tft.drawLine(260,50 +1, 260, 190 -1,TFT_COLOR_1);
  delay(lines_delay);

  tft.drawLine(275,50 +1, 275, 190 -1,TFT_COLOR_1);
  delay(lines_delay);

  tft.drawLine(290,50 +1, 290, 190 -1,TFT_COLOR_1);
  delay(lines_delay);

  
  tft.drawLine(305,50 +1, 305, 190 -1,TFT_COLOR_1);
  delay(lines_delay);


//dopo aver fatto le linee... renderizzo i caratteri veri e propri sullo schermo

for (int y = 0; y < 9;y++){
for (int x = 0; x < 20; x++){

  tft.setCursor((15 * x) + 10, (15 * y) + 50, 2);
  tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
  tft.println(    (char) EEPROM.read(     (x + (20 * y) + (180*(frame -1)) )     )     );


}
}

  delete_screen = 0;
}
} //REQUIRES RENDERING
} //CHANGE UI
} //OPTION_5








// OPTION_6 NETWORK SCAN --------------------------------------------------------------------------------------------------------------------------------



if (option_6){

if (option_6_animated){


tft.setCursor(50,30, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("SCANNING FOR NEARBY NETWORKS:");

delay(200);

tft.drawLine(5,50, 315, 50,TFT_WHITE);
delay(50);
tft.drawLine(5,50, 5, 190,TFT_WHITE);
delay(50);
tft.drawLine(315,50, 315, 190,TFT_WHITE);
delay(50);
tft.drawLine(5,190, 315, 190,TFT_WHITE);

delay(100);

tft.setCursor(2,200, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Do you want to start the scanning procedure?");

delay(150);

tft.setCursor(50,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("YES");

delay(100);

tft.setCursor(250,220, 2);
tft.setTextColor(TFT_RED);  tft.setTextSize(1);
tft.println("NO");

delay(200);


tft.setCursor(15,60, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("-");

delay(50);


tft.setCursor(25,60, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Once the scanning procedure is started");

delay(50);

tft.setCursor(25,80, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("it will update network data every 5 seconds.");

delay(50);

tft.setCursor(15,100, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("-");

delay(50);

tft.setCursor(25,100, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("While scanning, use button 4 to see networks ");

delay(50);

tft.setCursor(25,120, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("and button 3 to scroll the network's data");

delay(50);

tft.setCursor(15,140, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("-");

delay(50);


tft.setCursor(25,140, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Use button 1 to exit at any moment");

delay(50);

tft.setCursor(15,160, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("-");

delay(50);

tft.setCursor(25,160, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("And button 2 to save the collected data");

delay(200);

tft.setCursor(280,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("<<");





option_6_animated = false;
pointer_value = 0;

}


if (change_ui_6_1 == 0 && save_eeprom == 0){

if (digitalRead(Button_3) == HIGH && button_3_last_state == false ){
button_3_last_state = true;

tft.fillRect(280 - (200 * pointer_value),220,300 - (200 * pointer_value) ,230, TFT_BLACK);

pointer_value = pointer_value +1;

if (pointer_value > 1){
  pointer_value = 0;
}
if (pointer_value < 0){
  pointer_value = 1;
}

tft.setCursor(280 - (200 * pointer_value) ,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("<<");



}

if (digitalRead(Button_3) == LOW && button_3_last_state == true ){
button_3_last_state = false;

}


if (digitalRead(Button_4) == HIGH && button_4_last_state == false ){
button_4_last_state = true;

tft.fillRect(280 - (200 * pointer_value),220,300 - (200 * pointer_value) ,230, TFT_BLACK);

pointer_value = pointer_value -1;

if (pointer_value > 1){
  pointer_value = 0;
}
if (pointer_value < 0){
  pointer_value = 1;
}

tft.setCursor(280 - (200 * pointer_value) ,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("<<");



}

if (digitalRead(Button_4) == LOW && button_4_last_state == true ){
button_4_last_state = false;

}




if (digitalRead(Button_2) == HIGH && button_2_last_state == false ){
button_2_last_state = true;

if (pointer_value == 0){

option_6_animated = true;
option_6 = false;
active_menu = 1;
active_menu_animated = 1;
pointer_value = 0;
change_ui_6_1_animated = 1;
change_ui_6_1 = 0;

}

if (pointer_value == 1){

//PASSA ALLA SECONDA FASE DELLA UI
change_ui_6_1 = 1;




}

}




if (digitalRead(Button_2) == LOW && button_2_last_state == true ){
button_2_last_state = false;

}


}//change_ui_6_1 == 0;

//SECONDA FASE DELLA UI
/*
int change_ui_6_1 = 0;
int change_ui_6_1_animated = 1; 
*/

if (change_ui_6_1 == 1){

if (change_ui_6_1_animated == 1){

  tft.fillRect(0,200,320,40,TFT_BLACK); //CANCELLO tutto quello in basso
  delay(100);
  tft.fillRect(6,51,309,139,TFT_BLACK); //CANCELLO TUTTA LA CASELLA DI RENDERING
  
tft.setCursor(2,200, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Networks found:");

delay(100);

tft.setCursor(120,200, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("-");


  delay(100);

  
tft.setCursor(2,220, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Visualised networks:      /");

delay(100);

tft.setCursor(140,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("--");

delay(100);

tft.setCursor(175,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("--");


  delay(100);

tft.setCursor(230,200, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Use btn 2");

  delay(100);

tft.setCursor(210,220, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("to save the data");

delay(50);

tft.drawLine(200,195, 200, 235,TFT_WHITE);


change_ui_6_1_animated = 0;
millis_1 = 1;

}//change_ui_animated

//variabili_option_6:
/*
int change_ui_6_1 = 0;
int change_ui_6_1_animated = 1; 
unsigned long millis_1 = 0;
int frame_networks = 0;
*/

//BOTTONE PER USCIRE

if (digitalRead(Button_1) == HIGH && button_1_last_state == false ){
button_1_last_state = true;



option_6_animated = true;
option_6 = false;
active_menu = 1;
active_menu_animated = 1;
pointer_value = 0;
change_ui_6_1_animated = 1;
change_ui_6_1 = 0;
millis_1 = 0;
frame_networks = 0;


}

if (digitalRead(Button_1) == LOW && button_1_last_state == true ){
button_1_last_state = false;

}


//bottone per cambiare network

if (digitalRead(Button_4) == HIGH && button_4_last_state == false ){
button_4_last_state = true;


if (n > 6){
  if (n - (6 * frame_networks) > 0){
    frame_networks += 1;
  }
}

if (n <= 6){

frame_networks = 0;

}


if (n - (6 * frame_networks) <= 0){

frame_networks = 0;

}

override_update = 1;


}

if (digitalRead(Button_4) == LOW && button_4_last_state == true ){
button_4_last_state = false;

}




//bottone per cambiare da ssid a data

if (digitalRead(Button_3) == HIGH && button_3_last_state == false ){
button_3_last_state = true;


if (data_view == 0){
  data_view = 1;
}else{
  data_view = 0;
}
override_update = 1;


}

if (digitalRead(Button_3) == LOW && button_3_last_state == true ){
button_3_last_state = false;

}




//bottone per scrivere dentro la eeeprom
if (digitalRead(Button_2) == HIGH && button_2_last_state == false ){
button_2_last_state = true;

change_ui_6_1 = 0;
change_ui_6_1_animated = 1;
save_eeprom = 1;
save_eeprom_animated = 1;


}

if (digitalRead(Button_2) == LOW && button_2_last_state == true ){
button_2_last_state = false;

}





// UPDATE_NETWORK_LIST

if (millis_1 != 0){

if (millis() - millis_1 >= 15000 || override_update == 1){


if (override_update == 0){

millis_1 = millis();


//AVVISO CHE STO FACENDO UPDATE
tft.setCursor(50,172, 2);
tft.setTextColor(TFT_RED);  tft.setTextSize(1);
tft.println("Updating... (buttons won't work)");

//ricansiona tutte le network
n = WiFi.scanNetworks();

tft.fillRect(50,172,200,13,TFT_BLACK); //CANCELLO IL MESSAGGIO UPDATING

}


tft.fillRect(6,51,309,139,TFT_BLACK); //CANCELLO TUTTA LA CASELLA DI RENDERING
tft.fillRect(120,200,30,20,TFT_BLACK); //CANCELLO la sezione "netwoks found"

tft.fillRect(140,220,20,20,TFT_BLACK); //CANCELLO la sezione "visualised_network"
tft.fillRect(175,220,20,20,TFT_BLACK); //CANCELLO la sezione "visualised_network"

tft.fillRect(35,30,280,20,TFT_BLACK); //CANCELLO la PARTE SOPRA


//scrivo la sezione "networks found"
tft.setCursor(120,200, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println(n);


if (n <= 6){


tft.setCursor(140,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println(n);

}else{

tft.setCursor(140,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("6");

}




tft.setCursor(175,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println(n);



if (data_view == 0){

tft.fillRect(35,30,280,20,TFT_BLACK); //CANCELLO la PARTE SOPRA

tft.setCursor(35,30, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("SSID:");


for (int i = (6 * frame_networks); i < (6 + (6 * frame_networks)); ++i) {
  
tft.setCursor(15,60 + (20 * (i - (6 * frame_networks)) ), 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println(i + 1);

tft.setCursor(35,60 + (20 * (i - (6 * frame_networks)) ), 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println(WiFi.SSID(i).c_str());

}

}else{

tft.fillRect(35,30,280,20,TFT_BLACK); //CANCELLO la PARTE SOPRA

tft.setCursor(35,30, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("RSSI:");

tft.setCursor(120,30, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("CHANNEL:");

tft.setCursor(200,30, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("ENCRYPTION:");


for (int i = (6 * frame_networks); i < (6 + (6 * frame_networks)); ++i) {

tft.setCursor(15,60 + (20 * (i - (6 * frame_networks)) ), 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println(i + 1);


tft.setCursor(35,60 + (20 * (i - (6 * frame_networks)) ), 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println(WiFi.RSSI(i));

tft.setCursor(120,60 + (20 * (i - (6 * frame_networks)) ), 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println(WiFi.channel(i));

switch (WiFi.encryptionType(i))
{
case WIFI_AUTH_OPEN:
tft.setCursor(200,60 + (20 * (i - (6 * frame_networks)) ), 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("OPEN");
    break;
case WIFI_AUTH_WEP:
tft.setCursor(200,60 + (20 * (i - (6 * frame_networks)) ), 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("WEP");
    break;
case WIFI_AUTH_WPA_PSK:
tft.setCursor(200,60 + (20 * (i - (6 * frame_networks)) ), 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("WPA");
    break;
case WIFI_AUTH_WPA2_PSK:
tft.setCursor(200,60 + (20 * (i - (6 * frame_networks)) ), 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("WPA2");
    break;
case WIFI_AUTH_WPA_WPA2_PSK:
tft.setCursor(200,60 + (20 * (i - (6 * frame_networks)) ), 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("WPA+WPA2");
    break;
case WIFI_AUTH_WPA2_ENTERPRISE:
tft.setCursor(200,60 + (20 * (i - (6 * frame_networks)) ), 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("WPA2-EAP");
    break;
case WIFI_AUTH_WPA3_PSK:
tft.setCursor(200,60 + (20 * (i - (6 * frame_networks)) ), 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("WPA3");
    break;
case WIFI_AUTH_WPA2_WPA3_PSK:
tft.setCursor(200,60 + (20 * (i - (6 * frame_networks)) ), 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("WPA2+WPA3");
    break;
case WIFI_AUTH_WAPI_PSK:
tft.setCursor(200,60 + (20 * (i - (6 * frame_networks)) ), 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("WAPI");
    break;
default:
tft.setCursor(200,60 + (20 * (i - (6 * frame_networks)) ), 2);
tft.setTextColor(TFT_RED);  tft.setTextSize(1);
tft.println("UNKNOWN PROTOCOL");
}




}
}

override_update = 0;

}
}





}//change_ui_6_1



if (save_eeprom == 1){
if (save_eeprom_animated == 1){


tft.fillRect(0,16,320,240, TFT_BLACK);
delay(100);

tft.setCursor(2,30, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("SAVE DETECTED NETWORKS DATA ONTO EEPROM:");

delay(100);

tft.setCursor(10,60, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("The data will be saved with the following code:");

delay(100);

tft.setCursor(10,80, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Button 4: increase");

delay(100);

tft.setCursor(10,100, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Button 3: decrease");

delay(100);

tft.setCursor(200,90, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("CODE:");

delay(100);

tft.setCursor(240,90, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("0");


delay(100);

tft.setCursor(60,160, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Use button 1 to return to menu");

delay(100);

tft.setCursor(60,180, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Use button 2 to save the data");




tft.setCursor(145,220, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("SAVE");

delay(100);

tft.setCursor(180,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("<<<");





save_eeprom_animated = 0;

}

//bottone aumento code

if (digitalRead(Button_4) == HIGH && button_4_last_state == false ){
button_4_last_state = true;

code += 1;

tft.fillRect(240,90,50,20, TFT_BLACK);

tft.setCursor(240,90, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println(code);

}

if (digitalRead(Button_4) == LOW && button_4_last_state == true ){
button_4_last_state = false;

}


//bottone diminuzione code

if (digitalRead(Button_3) == HIGH && button_3_last_state == false ){
button_3_last_state = true;

code -= 1;

tft.fillRect(240,90,50,20, TFT_BLACK);

tft.setCursor(240,90, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println(code);

}

if (digitalRead(Button_3) == LOW && button_3_last_state == true ){
button_3_last_state = false;

}



//bottone exit menu

if (digitalRead(Button_1) == HIGH && button_1_last_state == false ){
button_1_last_state = true;

option_6_animated = true;
option_6 = false;
active_menu = 1;
active_menu_animated = 1;
pointer_value = 0;
change_ui_6_1_animated = 1;
change_ui_6_1 = 0;
millis_1 = 0;
frame_networks = 0;
save_eeprom = 0;
save_eeprom_animated = 1;


}

if (digitalRead(Button_1) == LOW && button_1_last_state == true ){
button_1_last_state = false;

}



//bottone 2 (salva effettivamente nella eeprom,eseguendo le istruzioni necessarie).

if (digitalRead(Button_2) == HIGH && button_2_last_state == false ){
button_2_last_state = true;

//APPUNTI SULLA LOGICA:
/*
Per quanto riguarda l'inizializzazione della scittura del pacchetto, ci sono due casi:

 uno in cui sono ad address zero e non c'è scritto nulla, ed allora devo scrivere il carattere di inizio pacchetto e scirvere il pacchetto a partire da address 1
 uno in cui sono ad address zero, quindi devo cercare un segno di fine pacchetto che non è seguito da uno di inizio pacchetto.

 Queste sono le regole per la srittura dei pacchetti.

 Carattere di inizio: <
 Carattere di fine: >

data_eeprom = EEPROM.read(random_address);
  EEPROM.write(random_address,255);
  EEPROM.commit();


int start_address = 0;
char carattere_inizio_pacchetto = '<';
char carattere_fine_pacchetto = '>';
int inWrite_int = 0;
String string_ssid;
String string_finale_net;

*/


if (EEPROM.read(0) == 255){


for (int i = 0; i < n; i++){
string_ssid = string_ssid + "{ ";
string_ssid = string_ssid + (i + 1);
string_ssid = string_ssid + ") ";
string_ssid = string_ssid + WiFi.SSID(i).c_str();
string_ssid = string_ssid + " / ";
string_ssid = string_ssid + WiFi.RSSI(i);
string_ssid = string_ssid + " / ";
string_ssid = string_ssid + WiFi.channel(i);
string_ssid = string_ssid + " / ";

switch (WiFi.encryptionType(i))
{
case WIFI_AUTH_OPEN:

string_ssid = string_ssid + "OPEN";
    break;
case WIFI_AUTH_WEP:
string_ssid = string_ssid + "WEP";
    break;
case WIFI_AUTH_WPA_PSK:
string_ssid = string_ssid + "WPA";
    break;
case WIFI_AUTH_WPA2_PSK:
string_ssid = string_ssid + "WPA2";
    break;
case WIFI_AUTH_WPA_WPA2_PSK:
string_ssid = string_ssid + "WPA+WPA2";
    break;
case WIFI_AUTH_WPA2_ENTERPRISE:
string_ssid = string_ssid + "WPA2-EAP";
    break;
case WIFI_AUTH_WPA3_PSK:
string_ssid = string_ssid + "WPA3";
    break;
case WIFI_AUTH_WPA2_WPA3_PSK:
string_ssid = string_ssid + "WPA2+WPA3";
    break;
case WIFI_AUTH_WAPI_PSK:
string_ssid = string_ssid + "WAPI";
    break;
default:
string_ssid = string_ssid + "UNKNOWN PROTOCOL";

}

string_ssid = string_ssid + " }";

}

Serial.println(string_ssid);


//preparo la stringa finale da inserire nella eeprom:

string_finale_net = string_finale_net + "< NETWORK DATA DUMP - CODE: ";
string_finale_net = string_finale_net + code;
string_finale_net = string_finale_net + "  |||  ";
string_finale_net = string_finale_net + string_ssid;
string_finale_net = string_finale_net + "  ||| ";
string_finale_net = string_finale_net + ">";

Serial.println(string_finale_net);
Serial.println(string_finale_net[0]);


for (int i = 0; i < string_finale_net.length(); i++){
  EEPROM.write(i,string_finale_net[i]);
}
if  (EEPROM.commit()){
  Serial.println("TRUE");
}
if  (EEPROM.commit() == false){
  Serial.println("FALSE");
}

} else {






for (int i = 0; i < EEPROM_SIZE; i++){
  if (EEPROM.read(i) == 62 && EEPROM.read(i + 1) == 255){
    start_address = i+1;
  }
}


for (int i = 0; i < n; i++){
string_ssid = string_ssid + "{ ";
string_ssid = string_ssid + (i + 1);
string_ssid = string_ssid + ") ";
string_ssid = string_ssid + WiFi.SSID(i).c_str();
string_ssid = string_ssid + " / ";
string_ssid = string_ssid + WiFi.RSSI(i);
string_ssid = string_ssid + " / ";
string_ssid = string_ssid + WiFi.channel(i);
string_ssid = string_ssid + " / ";

switch (WiFi.encryptionType(i))
{
case WIFI_AUTH_OPEN:

string_ssid = string_ssid + "OPEN";
    break;
case WIFI_AUTH_WEP:
string_ssid = string_ssid + "WEP";
    break;
case WIFI_AUTH_WPA_PSK:
string_ssid = string_ssid + "WPA";
    break;
case WIFI_AUTH_WPA2_PSK:
string_ssid = string_ssid + "WPA2";
    break;
case WIFI_AUTH_WPA_WPA2_PSK:
string_ssid = string_ssid + "WPA+WPA2";
    break;
case WIFI_AUTH_WPA2_ENTERPRISE:
string_ssid = string_ssid + "WPA2-EAP";
    break;
case WIFI_AUTH_WPA3_PSK:
string_ssid = string_ssid + "WPA3";
    break;
case WIFI_AUTH_WPA2_WPA3_PSK:
string_ssid = string_ssid + "WPA2+WPA3";
    break;
case WIFI_AUTH_WAPI_PSK:
string_ssid = string_ssid + "WAPI";
    break;
default:
string_ssid = string_ssid + "UNKNOWN PROTOCOL";

}

string_ssid = string_ssid + " }";

}




//preparo la stringa finale da inserire nella eeprom:

string_finale_net = string_finale_net + "< NETWORK DATA DUMP - CODE: ";
string_finale_net = string_finale_net + code;
string_finale_net = string_finale_net + "  |||  ";
string_finale_net = string_finale_net + string_ssid;
string_finale_net = string_finale_net + "  ||| ";
string_finale_net = string_finale_net + ">";




for (int i = 0; i < string_finale_net.length(); i++){
  EEPROM.write(i + start_address ,string_finale_net[i]);
}
if  (EEPROM.commit()){
  Serial.println("TRUE");
}
if  (EEPROM.commit() == false){
  Serial.println("FALSE");
}


}





//dopo aver salvato tutto, esco.

option_6_animated = true;
option_6 = false;
active_menu = 1;
active_menu_animated = 1;
pointer_value = 0;
change_ui_6_1_animated = 1;
change_ui_6_1 = 0;
millis_1 = 0;
frame_networks = 0;
save_eeprom = 0;
save_eeprom_animated = 1;
string_finale_net = "";
string_ssid = "";




}

if (digitalRead(Button_1) == LOW && button_2_last_state == true ){
button_2_last_state = false;

}


}
} //option_6




// OPTION_7 SAVE CURRENT COORDINATES --------------------------------------------------------------------------------------------------------------------------------

if (option_7){
if (option_7_animated){

code = 0;
start_address = 0;

tft.setCursor(70,30, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("SAVE CURRENT GPS DATA:");

delay(50);

tft.setCursor(10,50, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("lat:");

delay(50);

tft.setCursor(10,70, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("lon:");

delay(50);

tft.setCursor(10,90, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("date:");

delay(50);

tft.setCursor(150,50, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("time:");

delay(50);

tft.setCursor(150,70, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("altitude:");

delay(50);

tft.setCursor(30,50, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println(" --- ");

delay(50);

tft.setCursor(30,70, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println(" --- ");

delay(50);

tft.setCursor(40,90, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println(" --- ");

delay(50);

tft.setCursor(180,50, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println(" --- ");

delay(50);

tft.setCursor(200,70, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println(" --- ");

delay(50);

tft.drawLine(25,120, 295, 120,TFT_WHITE);

delay(50);


tft.setCursor(10,130, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("The data will be saved with the following code:");

delay(100);

tft.setCursor(10,150, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Button 4: increase");

delay(100);

tft.setCursor(10,170, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Button 3: decrease");

delay(100);

tft.setCursor(200,160, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("CODE:");

delay(100);

tft.setCursor(240,160, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("0");


delay(100);

tft.setCursor(60,192, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Use button 1 to return to menu");

delay(100);

tft.setCursor(60,205, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Use button 2 to save the data");




tft.setCursor(145,225, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("SAVE");

delay(100);

tft.setCursor(180,225, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("<<<");



option_7_animated = false;

}//option_7_animated


//eseguo le istruzioni per il gps:
while (ss.available() > 0)
    if (gps.encode(ss.read()))
      if (gps.location.isValid()){

      tft.fillRect(30,50,90,20, TFT_BLACK);
      tft.fillRect(30,70,90,20, TFT_BLACK);
      tft.fillRect(40,90,90,20, TFT_BLACK);
      tft.fillRect(180,50,90,20, TFT_BLACK);
      tft.fillRect(200,70,90,20, TFT_BLACK);

      tft.setCursor(40,50, 2);
      tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
      tft.println(gps.location.lat(), 6);

      tft.setCursor(40,70, 2);
      tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
      tft.println(gps.location.lng(), 6);

      char sz[32];
      sprintf(sz, "%02d/%02d/%02d ", gps.date.month(), gps.date.day(), gps.date.year());

      tft.setCursor(50,90, 2);
      tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
      tft.println(sz);

      char tz[32];
      sprintf(tz, "%02d:%02d:%02d ", gps.time.hour(), gps.time.minute(), gps.time.second());
      

      tft.setCursor(190,50, 2);
      tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
      tft.println(tz);

       tft.setCursor(210,70, 2);
      tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
      tft.println(gps.altitude.meters());

  }




//bottone di uscita
  if (digitalRead(Button_1) == HIGH && button_1_last_state == false ){
button_1_last_state = true;

option_7_animated = true;
option_7 = false;
active_menu = 1;
active_menu_animated = 1;
pointer_value = 0;
code = 0;

}

if (digitalRead(Button_1) == LOW && button_1_last_state == true ){
button_1_last_state = false;

}


//update the code of the packet:

//bottone aumento code

if (digitalRead(Button_4) == HIGH && button_4_last_state == false ){
button_4_last_state = true;

code += 1;

tft.fillRect(240,160,50,20, TFT_BLACK);

tft.setCursor(240,160, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println(code);

}

if (digitalRead(Button_4) == LOW && button_4_last_state == true ){
button_4_last_state = false;

}


//bottone diminuzione code

if (digitalRead(Button_3) == HIGH && button_3_last_state == false ){
button_3_last_state = true;

code -= 1;

tft.fillRect(240,160,50,20, TFT_BLACK);

tft.setCursor(240,160, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println(code);

}

if (digitalRead(Button_3) == LOW && button_3_last_state == true ){
button_3_last_state = false;

}





//bottone salva

if (digitalRead(Button_2) == HIGH && button_2_last_state == false ){
button_2_last_state = true;

/*
String string_finale_gps;

tring_finale_net = string_finale_net + "< NETWORK DATA DUMP - CODE: ";
string_finale_net = string_finale_net + code;
string_finale_net = string_finale_net + "  |||  ";
string_finale_net = string_finale_net + string_ssid;
string_finale_net = string_finale_net + "  ||| ";
string_finale_net = string_finale_net + ">";

*/

string_finale_gps = "";


if (EEPROM.read(0) == 255){

string_finale_gps = string_finale_gps + "< GPS DATA DUMP - CODE: ";
string_finale_gps = string_finale_gps + code;
string_finale_gps = string_finale_gps + "  |||  ";

string_finale_gps = string_finale_gps + "{ ";
string_finale_gps = string_finale_gps + "lat: ";


//latitudine 
char buff[32];
sprintf(buff, "%f",gps.location.lat() );
string_finale_gps = string_finale_gps + buff; 

string_finale_gps = string_finale_gps + " / ";
string_finale_gps = string_finale_gps + "lon: ";

//longitudine
sprintf(buff, "%f",gps.location.lng() );
string_finale_gps = string_finale_gps + buff; 

string_finale_gps = string_finale_gps + " / ";
string_finale_gps = string_finale_gps + "date: ";

//date:
sprintf(buff, "%02d/%02d/%02d ", gps.date.month(), gps.date.day(), gps.date.year());
string_finale_gps = string_finale_gps + buff;

string_finale_gps = string_finale_gps + " / ";
string_finale_gps = string_finale_gps + "time: ";

//time
sprintf(buff, "%02d:%02d:%02d ", gps.time.hour(), gps.time.minute(), gps.time.second());
string_finale_gps = string_finale_gps + buff;

string_finale_gps = string_finale_gps + " / ";
string_finale_gps = string_finale_gps + "alt: ";

//altitude:
sprintf(buff, "%f",gps.altitude.meters());
string_finale_gps = string_finale_gps + buff;

string_finale_gps = string_finale_gps + " }";

string_finale_gps = string_finale_gps + "  |||  ";
string_finale_gps = string_finale_gps + ">";




for (int i = 0; i < string_finale_gps.length(); i++){
  EEPROM.write(i,string_finale_gps[i]);
}
if  (EEPROM.commit()){
  Serial.println("TRUE");
}
if  (EEPROM.commit() == false){
  Serial.println("FALSE");
}


} else{

for (int i = 0; i < EEPROM_SIZE; i++){
  if (EEPROM.read(i) == 62 && EEPROM.read(i + 1) == 255){
    start_address = i+1;
  }
}

string_finale_gps = string_finale_gps + "< GPS DATA DUMP - CODE: ";
string_finale_gps = string_finale_gps + code;
string_finale_gps = string_finale_gps + "  |||  ";

string_finale_gps = string_finale_gps + "{ ";
string_finale_gps = string_finale_gps + "lat: ";


//latitudine 
char buff[32];
sprintf(buff, "%f",gps.location.lat() );
string_finale_gps = string_finale_gps + buff; 

string_finale_gps = string_finale_gps + " / ";
string_finale_gps = string_finale_gps + "lon: ";

//longitudine
sprintf(buff, "%f",gps.location.lng() );
string_finale_gps = string_finale_gps + buff; 

string_finale_gps = string_finale_gps + " / ";
string_finale_gps = string_finale_gps + "date: ";

//date:
sprintf(buff, "%02d/%02d/%02d ", gps.date.month(), gps.date.day(), gps.date.year());
string_finale_gps = string_finale_gps + buff;

string_finale_gps = string_finale_gps + " / ";
string_finale_gps = string_finale_gps + "time: ";

//time
sprintf(buff, "%02d:%02d:%02d ", gps.time.hour(), gps.time.minute(), gps.time.second());
string_finale_gps = string_finale_gps + buff;

string_finale_gps = string_finale_gps + " / ";
string_finale_gps = string_finale_gps + "alt: ";

//altitude:
sprintf(buff, "%f",gps.altitude.meters());
string_finale_gps = string_finale_gps + buff;

string_finale_gps = string_finale_gps + " }";

string_finale_gps = string_finale_gps + "  |||  ";
string_finale_gps = string_finale_gps + ">";






for (int i = 0; i < string_finale_gps.length(); i++){
  EEPROM.write(i + start_address,string_finale_gps[i]);
}
if  (EEPROM.commit()){
  Serial.println("TRUE");
}
if  (EEPROM.commit() == false){
  Serial.println("FALSE");
}



}


//dovo aver finito la scrittura, esce
option_7_animated = true;
option_7 = false;
active_menu = 1;
active_menu_animated = 1;
pointer_value = 0;
code = 0;
string_finale_gps = "";
start_address = 0;

}

if (digitalRead(Button_2) == LOW && button_2_last_state == true ){
button_2_last_state = false;

}





}//option_7



// OPTION_8 SAVE COORDINATES AND SSID --------------------------------------------------------------------------------------------------------------------------------








if (option_8){

if (option_8_animated){


tft.setCursor(50,30, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("SCANNING FOR NEARBY NETWORKS:");

delay(200);

tft.drawLine(5,50, 315, 50,TFT_WHITE);
delay(50);
tft.drawLine(5,50, 5, 190,TFT_WHITE);
delay(50);
tft.drawLine(315,50, 315, 190,TFT_WHITE);
delay(50);
tft.drawLine(5,190, 315, 190,TFT_WHITE);

delay(100);

tft.setCursor(2,200, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Do you want to start the scanning procedure?");

delay(150);

tft.setCursor(50,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("YES");

delay(100);

tft.setCursor(250,220, 2);
tft.setTextColor(TFT_RED);  tft.setTextSize(1);
tft.println("NO");

delay(200);


tft.setCursor(15,60, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("-");

delay(50);


tft.setCursor(25,60, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Once the scanning procedure is started");

delay(50);

tft.setCursor(25,80, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("it will update network data every 5 seconds.");

delay(50);

tft.setCursor(15,100, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("-");

delay(50);

tft.setCursor(25,100, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("While scanning, use button 4 to see networks ");

delay(50);

tft.setCursor(25,120, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("and button 3 to scroll the network's data");

delay(50);

tft.setCursor(15,140, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("-");

delay(50);


tft.setCursor(25,140, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Use button 1 to exit at any moment");

delay(50);

tft.setCursor(15,160, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("-");

delay(50);

tft.setCursor(25,160, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("And button 2 to save the collected data");

delay(200);

tft.setCursor(280,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("<<");





option_8_animated = false;
pointer_value = 0;

}


if (change_ui_6_1 == 0 && save_eeprom == 0){

if (digitalRead(Button_3) == HIGH && button_3_last_state == false ){
button_3_last_state = true;

tft.fillRect(280 - (200 * pointer_value),220,300 - (200 * pointer_value) ,230, TFT_BLACK);

pointer_value = pointer_value +1;

if (pointer_value > 1){
  pointer_value = 0;
}
if (pointer_value < 0){
  pointer_value = 1;
}

tft.setCursor(280 - (200 * pointer_value) ,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("<<");



}

if (digitalRead(Button_3) == LOW && button_3_last_state == true ){
button_3_last_state = false;

}


if (digitalRead(Button_4) == HIGH && button_4_last_state == false ){
button_4_last_state = true;

tft.fillRect(280 - (200 * pointer_value),220,300 - (200 * pointer_value) ,230, TFT_BLACK);

pointer_value = pointer_value -1;

if (pointer_value > 1){
  pointer_value = 0;
}
if (pointer_value < 0){
  pointer_value = 1;
}

tft.setCursor(280 - (200 * pointer_value) ,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("<<");



}

if (digitalRead(Button_4) == LOW && button_4_last_state == true ){
button_4_last_state = false;

}




if (digitalRead(Button_2) == HIGH && button_2_last_state == false ){
button_2_last_state = true;

if (pointer_value == 0){

option_8_animated = true;
option_8 = false;
active_menu = 1;
active_menu_animated = 1;
pointer_value = 0;
change_ui_6_1_animated = 1;
change_ui_6_1 = 0;

}

if (pointer_value == 1){

//PASSA ALLA SECONDA FASE DELLA UI
change_ui_6_1 = 1;




}

}




if (digitalRead(Button_2) == LOW && button_2_last_state == true ){
button_2_last_state = false;

}


}//change_ui_6_1 == 0;

//SECONDA FASE DELLA UI
/*
int change_ui_6_1 = 0;
int change_ui_6_1_animated = 1; 
*/

if (change_ui_6_1 == 1){

if (change_ui_6_1_animated == 1){

  tft.fillRect(0,200,320,40,TFT_BLACK); //CANCELLO tutto quello in basso
  delay(100);
  tft.fillRect(6,51,309,139,TFT_BLACK); //CANCELLO TUTTA LA CASELLA DI RENDERING
  
tft.setCursor(2,200, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Networks found:");

delay(100);

tft.setCursor(120,200, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("-");


  delay(100);

  
tft.setCursor(2,220, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Visualised networks:      /");

delay(100);

tft.setCursor(140,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("--");

delay(100);

tft.setCursor(175,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("--");


  delay(100);

tft.setCursor(230,200, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Use btn 2");

  delay(100);

tft.setCursor(210,220, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("to save the data");

delay(50);

tft.drawLine(200,195, 200, 235,TFT_WHITE);


change_ui_6_1_animated = 0;
millis_1 = 1;

}//change_ui_animated

//variabili_option_6:
/*
int change_ui_6_1 = 0;
int change_ui_6_1_animated = 1; 
unsigned long millis_1 = 0;
int frame_networks = 0;
*/

//BOTTONE PER USCIRE

if (digitalRead(Button_1) == HIGH && button_1_last_state == false ){
button_1_last_state = true;



option_8_animated = true;
option_8 = false;
active_menu = 1;
active_menu_animated = 1;
pointer_value = 0;
change_ui_6_1_animated = 1;
change_ui_6_1 = 0;
millis_1 = 0;
frame_networks = 0;


}

if (digitalRead(Button_1) == LOW && button_1_last_state == true ){
button_1_last_state = false;

}


//bottone per cambiare network

if (digitalRead(Button_4) == HIGH && button_4_last_state == false ){
button_4_last_state = true;


if (n > 6){
  if (n - (6 * frame_networks) > 0){
    frame_networks += 1;
  }
}

if (n <= 6){

frame_networks = 0;

}


if (n - (6 * frame_networks) <= 0){

frame_networks = 0;

}

override_update = 1;


}

if (digitalRead(Button_4) == LOW && button_4_last_state == true ){
button_4_last_state = false;

}




//bottone per cambiare da ssid a data

if (digitalRead(Button_3) == HIGH && button_3_last_state == false ){
button_3_last_state = true;


if (data_view == 0){
  data_view = 1;
}else{
  data_view = 0;
}
override_update = 1;


}

if (digitalRead(Button_3) == LOW && button_3_last_state == true ){
button_3_last_state = false;

}




//bottone per scrivere dentro la eeeprom
if (digitalRead(Button_2) == HIGH && button_2_last_state == false ){
button_2_last_state = true;

change_ui_6_1 = 0;
change_ui_6_1_animated = 1;
save_eeprom = 1;
save_eeprom_animated = 1;


}

if (digitalRead(Button_2) == LOW && button_2_last_state == true ){
button_2_last_state = false;

}





// UPDATE_NETWORK_LIST

if (millis_1 != 0){

if (millis() - millis_1 >= 15000 || override_update == 1){


if (override_update == 0){

millis_1 = millis();


//AVVISO CHE STO FACENDO UPDATE
tft.setCursor(50,172, 2);
tft.setTextColor(TFT_RED);  tft.setTextSize(1);
tft.println("Updating... (buttons won't work)");

//ricansiona tutte le network
n = WiFi.scanNetworks();

tft.fillRect(50,172,200,13,TFT_BLACK); //CANCELLO IL MESSAGGIO UPDATING

}


tft.fillRect(6,51,309,139,TFT_BLACK); //CANCELLO TUTTA LA CASELLA DI RENDERING
tft.fillRect(120,200,30,20,TFT_BLACK); //CANCELLO la sezione "netwoks found"

tft.fillRect(140,220,20,20,TFT_BLACK); //CANCELLO la sezione "visualised_network"
tft.fillRect(175,220,20,20,TFT_BLACK); //CANCELLO la sezione "visualised_network"

tft.fillRect(35,30,280,20,TFT_BLACK); //CANCELLO la PARTE SOPRA


//scrivo la sezione "networks found"
tft.setCursor(120,200, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println(n);


if (n <= 6){


tft.setCursor(140,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println(n);

}else{

tft.setCursor(140,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("6");

}




tft.setCursor(175,220, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println(n);



if (data_view == 0){

tft.fillRect(35,30,280,20,TFT_BLACK); //CANCELLO la PARTE SOPRA

tft.setCursor(35,30, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("SSID:");


for (int i = (6 * frame_networks); i < (6 + (6 * frame_networks)); ++i) {
  
tft.setCursor(15,60 + (20 * (i - (6 * frame_networks)) ), 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println(i + 1);

tft.setCursor(35,60 + (20 * (i - (6 * frame_networks)) ), 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println(WiFi.SSID(i).c_str());

}

}else{

tft.fillRect(35,30,280,20,TFT_BLACK); //CANCELLO la PARTE SOPRA

tft.setCursor(35,30, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("RSSI:");

tft.setCursor(120,30, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("CHANNEL:");

tft.setCursor(200,30, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("ENCRYPTION:");


for (int i = (6 * frame_networks); i < (6 + (6 * frame_networks)); ++i) {

tft.setCursor(15,60 + (20 * (i - (6 * frame_networks)) ), 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println(i + 1);


tft.setCursor(35,60 + (20 * (i - (6 * frame_networks)) ), 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println(WiFi.RSSI(i));

tft.setCursor(120,60 + (20 * (i - (6 * frame_networks)) ), 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println(WiFi.channel(i));

switch (WiFi.encryptionType(i))
{
case WIFI_AUTH_OPEN:
tft.setCursor(200,60 + (20 * (i - (6 * frame_networks)) ), 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("OPEN");
    break;
case WIFI_AUTH_WEP:
tft.setCursor(200,60 + (20 * (i - (6 * frame_networks)) ), 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("WEP");
    break;
case WIFI_AUTH_WPA_PSK:
tft.setCursor(200,60 + (20 * (i - (6 * frame_networks)) ), 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("WPA");
    break;
case WIFI_AUTH_WPA2_PSK:
tft.setCursor(200,60 + (20 * (i - (6 * frame_networks)) ), 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("WPA2");
    break;
case WIFI_AUTH_WPA_WPA2_PSK:
tft.setCursor(200,60 + (20 * (i - (6 * frame_networks)) ), 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("WPA+WPA2");
    break;
case WIFI_AUTH_WPA2_ENTERPRISE:
tft.setCursor(200,60 + (20 * (i - (6 * frame_networks)) ), 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("WPA2-EAP");
    break;
case WIFI_AUTH_WPA3_PSK:
tft.setCursor(200,60 + (20 * (i - (6 * frame_networks)) ), 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("WPA3");
    break;
case WIFI_AUTH_WPA2_WPA3_PSK:
tft.setCursor(200,60 + (20 * (i - (6 * frame_networks)) ), 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("WPA2+WPA3");
    break;
case WIFI_AUTH_WAPI_PSK:
tft.setCursor(200,60 + (20 * (i - (6 * frame_networks)) ), 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("WAPI");
    break;
default:
tft.setCursor(200,60 + (20 * (i - (6 * frame_networks)) ), 2);
tft.setTextColor(TFT_RED);  tft.setTextSize(1);
tft.println("UNKNOWN PROTOCOL");
}




}
}

override_update = 0;

}
}





}//change_ui_6_1



if (save_eeprom == 1){
if (save_eeprom_animated == 1){


code = 0;
start_address = 0;

tft.fillRect(0,16,320,240, TFT_BLACK);
delay(100);

tft.setCursor(35,30, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("SAVE CURRENT GPS AND NETWORK DATA:");

delay(50);

tft.setCursor(10,50, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("lat:");

delay(50);

tft.setCursor(10,70, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("lon:");

delay(50);

tft.setCursor(10,90, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("date:");

delay(50);

tft.setCursor(150,50, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("time:");

delay(50);

tft.setCursor(150,70, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("altitude:");

delay(50);

tft.setCursor(30,50, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println(" --- ");

delay(50);

tft.setCursor(30,70, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println(" --- ");

delay(50);

tft.setCursor(40,90, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println(" --- ");

delay(50);

tft.setCursor(180,50, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println(" --- ");

delay(50);

tft.setCursor(200,70, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println(" --- ");

delay(50);

tft.drawLine(25,120, 295, 120,TFT_WHITE);

delay(50);


tft.setCursor(10,130, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("The data will be saved with the following code:");

delay(100);

tft.setCursor(10,150, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Button 4: increase");

delay(100);

tft.setCursor(10,170, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Button 3: decrease");

delay(100);

tft.setCursor(200,160, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("CODE:");

delay(100);

tft.setCursor(240,160, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("0");


delay(100);

tft.setCursor(60,192, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Use button 1 to return to menu");

delay(100);

tft.setCursor(60,205, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Use button 2 to save the data");




tft.setCursor(145,225, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("SAVE");

delay(100);

tft.setCursor(180,225, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("<<<");


save_eeprom_animated = 0;

}





//eseguo le istruzioni per il gps:
while (ss.available() > 0)
    if (gps.encode(ss.read()))
      if (gps.location.isValid()){

      tft.fillRect(30,50,90,20, TFT_BLACK);
      tft.fillRect(30,70,90,20, TFT_BLACK);
      tft.fillRect(40,90,90,20, TFT_BLACK);
      tft.fillRect(180,50,90,20, TFT_BLACK);
      tft.fillRect(200,70,90,20, TFT_BLACK);

      tft.setCursor(40,50, 2);
      tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
      tft.println(gps.location.lat(), 6);

      tft.setCursor(40,70, 2);
      tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
      tft.println(gps.location.lng(), 6);

      char sz[32];
      sprintf(sz, "%02d/%02d/%02d ", gps.date.month(), gps.date.day(), gps.date.year());

      tft.setCursor(50,90, 2);
      tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
      tft.println(sz);

      char tz[32];
      sprintf(tz, "%02d:%02d:%02d ", gps.time.hour(), gps.time.minute(), gps.time.second());
      

      tft.setCursor(190,50, 2);
      tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
      tft.println(tz);

       tft.setCursor(210,70, 2);
      tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
      tft.println(gps.altitude.meters());

  }


//update the code of the packet:

//bottone aumento code

if (digitalRead(Button_4) == HIGH && button_4_last_state == false ){
button_4_last_state = true;

code += 1;

tft.fillRect(240,160,50,20, TFT_BLACK);

tft.setCursor(240,160, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println(code);

}

if (digitalRead(Button_4) == LOW && button_4_last_state == true ){
button_4_last_state = false;

}


//bottone diminuzione code

if (digitalRead(Button_3) == HIGH && button_3_last_state == false ){
button_3_last_state = true;

code -= 1;

tft.fillRect(240,160,50,20, TFT_BLACK);

tft.setCursor(240,160, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println(code);

}

if (digitalRead(Button_3) == LOW && button_3_last_state == true ){
button_3_last_state = false;

}





//bottone exit menu

if (digitalRead(Button_1) == HIGH && button_1_last_state == false ){
button_1_last_state = true;

option_8_animated = true;
option_8 = false;
active_menu = 1;
active_menu_animated = 1;
pointer_value = 0;
change_ui_6_1_animated = 1;
change_ui_6_1 = 0;
millis_1 = 0;
frame_networks = 0;
save_eeprom = 0;
save_eeprom_animated = 1;


}

if (digitalRead(Button_1) == LOW && button_1_last_state == true ){
button_1_last_state = false;

}



//bottone 2 (salva effettivamente nella eeprom,eseguendo le istruzioni necessarie).

if (digitalRead(Button_2) == HIGH && button_2_last_state == false ){
button_2_last_state = true;

//APPUNTI SULLA LOGICA:
/*
Per quanto riguarda l'inizializzazione della scittura del pacchetto, ci sono due casi:

 uno in cui sono ad address zero e non c'è scritto nulla, ed allora devo scrivere il carattere di inizio pacchetto e scirvere il pacchetto a partire da address 1
 uno in cui sono ad address zero, quindi devo cercare un segno di fine pacchetto che non è seguito da uno di inizio pacchetto.

 Queste sono le regole per la srittura dei pacchetti.

 Carattere di inizio: <
 Carattere di fine: >

data_eeprom = EEPROM.read(random_address);
  EEPROM.write(random_address,255);
  EEPROM.commit();


int start_address = 0;
char carattere_inizio_pacchetto = '<';
char carattere_fine_pacchetto = '>';
int inWrite_int = 0;
String string_ssid;
String string_finale_net;

*/


if (EEPROM.read(0) == 255){


for (int i = 0; i < n; i++){
string_ssid = string_ssid + "{ ";
string_ssid = string_ssid + (i + 1);
string_ssid = string_ssid + ") ";
string_ssid = string_ssid + WiFi.SSID(i).c_str();
string_ssid = string_ssid + " / ";
string_ssid = string_ssid + WiFi.RSSI(i);
string_ssid = string_ssid + " / ";
string_ssid = string_ssid + WiFi.channel(i);
string_ssid = string_ssid + " / ";

switch (WiFi.encryptionType(i))
{
case WIFI_AUTH_OPEN:

string_ssid = string_ssid + "OPEN";
    break;
case WIFI_AUTH_WEP:
string_ssid = string_ssid + "WEP";
    break;
case WIFI_AUTH_WPA_PSK:
string_ssid = string_ssid + "WPA";
    break;
case WIFI_AUTH_WPA2_PSK:
string_ssid = string_ssid + "WPA2";
    break;
case WIFI_AUTH_WPA_WPA2_PSK:
string_ssid = string_ssid + "WPA+WPA2";
    break;
case WIFI_AUTH_WPA2_ENTERPRISE:
string_ssid = string_ssid + "WPA2-EAP";
    break;
case WIFI_AUTH_WPA3_PSK:
string_ssid = string_ssid + "WPA3";
    break;
case WIFI_AUTH_WPA2_WPA3_PSK:
string_ssid = string_ssid + "WPA2+WPA3";
    break;
case WIFI_AUTH_WAPI_PSK:
string_ssid = string_ssid + "WAPI";
    break;
default:
string_ssid = string_ssid + "UNKNOWN PROTOCOL";

}

string_ssid = string_ssid + " }";

}

Serial.println(string_ssid);


string_finale_gps = string_finale_gps + "{ ";
string_finale_gps = string_finale_gps + "lat: ";


//latitudine 
char buff[32];
sprintf(buff, "%f",gps.location.lat() );
string_finale_gps = string_finale_gps + buff; 

string_finale_gps = string_finale_gps + " / ";
string_finale_gps = string_finale_gps + "lon: ";

//longitudine
sprintf(buff, "%f",gps.location.lng() );
string_finale_gps = string_finale_gps + buff; 

string_finale_gps = string_finale_gps + " / ";
string_finale_gps = string_finale_gps + "date: ";

//date:
sprintf(buff, "%02d/%02d/%02d ", gps.date.month(), gps.date.day(), gps.date.year());
string_finale_gps = string_finale_gps + buff;

string_finale_gps = string_finale_gps + " / ";
string_finale_gps = string_finale_gps + "time: ";

//time
sprintf(buff, "%02d:%02d:%02d ", gps.time.hour(), gps.time.minute(), gps.time.second());
string_finale_gps = string_finale_gps + buff;

string_finale_gps = string_finale_gps + " / ";
string_finale_gps = string_finale_gps + "alt: ";

//altitude:
sprintf(buff, "%f",gps.altitude.meters());
string_finale_gps = string_finale_gps + buff;

string_finale_gps = string_finale_gps + " }";

string_finale_gps = string_finale_gps + "  |||  ";
string_finale_gps = string_finale_gps + ">";




//preparo la stringa finale da inserire nella eeprom:

string_finale_net = string_finale_net + "< NETWORK AND GPS DATA DUMP - CODE: ";
string_finale_net = string_finale_net + code;
string_finale_net = string_finale_net + "  |||  ";
string_finale_net = string_finale_net + string_ssid;
string_finale_net = string_finale_net + "  ||| ";
string_finale_net = string_finale_net + string_finale_gps;


Serial.println(string_finale_net);
Serial.println(string_finale_net[0]);


for (int i = 0; i < string_finale_net.length(); i++){
  EEPROM.write(i,string_finale_net[i]);
}
if  (EEPROM.commit()){
  Serial.println("TRUE");
}
if  (EEPROM.commit() == false){
  Serial.println("FALSE");
}

} else {






for (int i = 0; i < EEPROM_SIZE; i++){
  if (EEPROM.read(i) == 62 && EEPROM.read(i + 1) == 255){
    start_address = i+1;
  }
}


for (int i = 0; i < n; i++){
string_ssid = string_ssid + "{ ";
string_ssid = string_ssid + (i + 1);
string_ssid = string_ssid + ") ";
string_ssid = string_ssid + WiFi.SSID(i).c_str();
string_ssid = string_ssid + " / ";
string_ssid = string_ssid + WiFi.RSSI(i);
string_ssid = string_ssid + " / ";
string_ssid = string_ssid + WiFi.channel(i);
string_ssid = string_ssid + " / ";

switch (WiFi.encryptionType(i))
{
case WIFI_AUTH_OPEN:

string_ssid = string_ssid + "OPEN";
    break;
case WIFI_AUTH_WEP:
string_ssid = string_ssid + "WEP";
    break;
case WIFI_AUTH_WPA_PSK:
string_ssid = string_ssid + "WPA";
    break;
case WIFI_AUTH_WPA2_PSK:
string_ssid = string_ssid + "WPA2";
    break;
case WIFI_AUTH_WPA_WPA2_PSK:
string_ssid = string_ssid + "WPA+WPA2";
    break;
case WIFI_AUTH_WPA2_ENTERPRISE:
string_ssid = string_ssid + "WPA2-EAP";
    break;
case WIFI_AUTH_WPA3_PSK:
string_ssid = string_ssid + "WPA3";
    break;
case WIFI_AUTH_WPA2_WPA3_PSK:
string_ssid = string_ssid + "WPA2+WPA3";
    break;
case WIFI_AUTH_WAPI_PSK:
string_ssid = string_ssid + "WAPI";
    break;
default:
string_ssid = string_ssid + "UNKNOWN PROTOCOL";

}

string_ssid = string_ssid + " }";

}



string_finale_gps = string_finale_gps + "{ ";
string_finale_gps = string_finale_gps + "lat: ";


//latitudine 
char buff[32];
sprintf(buff, "%f",gps.location.lat() );
string_finale_gps = string_finale_gps + buff; 

string_finale_gps = string_finale_gps + " / ";
string_finale_gps = string_finale_gps + "lon: ";

//longitudine
sprintf(buff, "%f",gps.location.lng() );
string_finale_gps = string_finale_gps + buff; 

string_finale_gps = string_finale_gps + " / ";
string_finale_gps = string_finale_gps + "date: ";

//date:
sprintf(buff, "%02d/%02d/%02d ", gps.date.month(), gps.date.day(), gps.date.year());
string_finale_gps = string_finale_gps + buff;

string_finale_gps = string_finale_gps + " / ";
string_finale_gps = string_finale_gps + "time: ";

//time
sprintf(buff, "%02d:%02d:%02d ", gps.time.hour(), gps.time.minute(), gps.time.second());
string_finale_gps = string_finale_gps + buff;

string_finale_gps = string_finale_gps + " / ";
string_finale_gps = string_finale_gps + "alt: ";

//altitude:
sprintf(buff, "%f",gps.altitude.meters());
string_finale_gps = string_finale_gps + buff;

string_finale_gps = string_finale_gps + " }";

string_finale_gps = string_finale_gps + "  |||  ";
string_finale_gps = string_finale_gps + ">";


//preparo la stringa finale da inserire nella eeprom:

string_finale_net = string_finale_net + "< NETWORK AND GPS DATA DUMP - CODE: ";
string_finale_net = string_finale_net + code;
string_finale_net = string_finale_net + "  |||  ";
string_finale_net = string_finale_net + string_ssid;
string_finale_net = string_finale_net + "  ||| ";
string_finale_net = string_finale_net + string_finale_gps;






for (int i = 0; i < string_finale_net.length(); i++){
  EEPROM.write(i + start_address ,string_finale_net[i]);
}
if  (EEPROM.commit()){
  Serial.println("TRUE");
}
if  (EEPROM.commit() == false){
  Serial.println("FALSE");
}


}





//dopo aver salvato tutto, esco.

option_8_animated = true;
option_8 = false;
active_menu = 1;
active_menu_animated = 1;
pointer_value = 0;
change_ui_6_1_animated = 1;
change_ui_6_1 = 0;
millis_1 = 0;
frame_networks = 0;
save_eeprom = 0;
save_eeprom_animated = 1;
string_finale_net = "";
string_ssid = "";
pointer_value = 0;
code = 0;
string_finale_gps = "";
start_address = 0;




}

if (digitalRead(Button_2) == LOW && button_2_last_state == true ){
button_2_last_state = false;

}



}
} //option_8




// OPTION_9 SHOW CPU INFO --------------------------------------------------------------------------------------------------------------------------------


if (option_9){
if (option_9_animated){


  	for(int i=0; i<17; i=i+8) {
	  chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
	}


tft.setCursor(100,30, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("PROCESSOR INFO:");

delay(50);

tft.setCursor(10,50, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Processor model:");


delay(50);

tft.setCursor(10,70, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Processor id:");

delay(50);

tft.setCursor(10,90, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Processor revision:");


delay(50);

tft.setCursor(10,110, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Processor clock speed:");


delay(50);

tft.setCursor(10,130, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Configured eeprom size:");

delay(50);

tft.setCursor(10,150, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Time since boot (millis()): ");






delay(50);

tft.setCursor(180,50, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println(ESP.getChipModel());


delay(50);

tft.setCursor(180,70, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println(chipId);

delay(50);

tft.setCursor(180,90, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println(ESP.getChipRevision());


delay(50);

tft.setCursor(180,110, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println("240Mhz (no realtime)");

delay(50);

tft.setCursor(180,130, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println(EEPROM_SIZE);

delay(50);

tft.setCursor(180,150, 2);
tft.setTextColor(TFT_GREEN);  tft.setTextSize(1);
tft.println(millis());


delay(50);

tft.setCursor(90,190, 2);
tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
tft.println("Use button 1 to exit.");






option_9_animated = 0;

} // option_9_animated


//bottone exit menu

if (digitalRead(Button_1) == HIGH && button_1_last_state == false ){
button_1_last_state = true;

option_9_animated = true;
option_9 = false;
active_menu = 1;
active_menu_animated = 1;
pointer_value = 0;

}

if (digitalRead(Button_1) == LOW && button_1_last_state == true ){
button_1_last_state = false;

}



} // option_9







}//void loop