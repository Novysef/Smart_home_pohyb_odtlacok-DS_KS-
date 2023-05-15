

#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>
#include <WiFi.h>

HardwareSerial serialPort(2); // TX/RX

const char* NAME;
const char* ID;

String Event_Name = "fingerprint";
String Key = "bhxV03_kVSiTFqiP_JhtlW";

String resource = "/trigger/" + Event_Name + "/with/key/" + Key;

const char* server ="maker.ifttt.com";

const char* ssid = "Katka";
const char* password = "0917245484";


Adafruit_Fingerprint finger = Adafruit_Fingerprint(&serialPort);




void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit test odtlackov prstov");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial.println("Naslo senzor odtlackov prsta!");
  } else {
    Serial.println("Nenaslo senzor odtlackov prsta :(");
    while (1) { delay(1); }
  }

  //Serial.println(F("Reading sensor parameters"));
  //finger.getParameters();
  //Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
  //Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
  //Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
  //Serial.print(F("Security level: ")); Serial.println(finger.security_level);
  //Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
  //Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
  //Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);

  finger.getTemplateCount();
  Serial.println("Senzor obsahuje "); Serial.print(finger.templateCount); Serial.println(" sablon");
  Serial.println("Cakam na spravny prst...");
  Serial.print("Pripajam ku: ");
  Serial.print(ssid);
  WiFi.begin(ssid,password);
  int timeout = 10*4;
  while(WiFi.status()!=WL_CONNECTED && (timeout-- > 0)){
    delay(250);
    Serial.print(",");
  }
  Serial.println("");
  if (WiFi.status() !=WL_CONNECTED){
    Serial.println("Nepodarilo sa pripojit, prechadzam do rezimu spanku!");
  }
  Serial.print("WiFi pripojena k: ");
  Serial.print(millis());
  Serial.print(", IP adresa: ");
  Serial.println(WiFi.localIP());
}

void loop()                     // run over and over again
{
  getFingerprintIDez();
  if (finger.fingerID==1){
    Serial.print("!!--");
    Serial.println(finger.fingerID);
    NAME = "Katka1";
    ID ="1";
    if (finger.confidence >=60){
      Serial.print("Totoznost: "); Serial.println(NAME);
      makeIFTTTRequest();
    }
    else {
    Serial.println("Odtlacok neznamy");
    }
  }           //don't ned to run this at full speed.
  if (finger.fingerID==2){
    Serial.print("!!--");
    Serial.println(finger.fingerID);
    NAME = "Katka2";
    ID ="2";
    if (finger.confidence >=60){
      Serial.print("Totoznost:"); Serial.println(NAME);
      makeIFTTTRequest();
    }
    else {
    Serial.println("Odtlacok neznamy");
    }
  }           //don't ned to run this at full speed.
  if (finger.fingerID==3){
    Serial.print("!!--");
    Serial.println(finger.fingerID);
    NAME = "David1";
    ID ="3";
    if (finger.confidence >=60){
      Serial.print("Totoznost: "); Serial.println(NAME);
      makeIFTTTRequest();
    }
    else {
    Serial.println("Odtlacok neznamy");
    }    
  }           //don't ned to run this at full speed.
  if (finger.fingerID==4){
    Serial.print("!!--");
    Serial.println(finger.fingerID);
    NAME = "David2";
    ID ="4";
    if (finger.confidence >=60){
      Serial.print("Totoznost: "); Serial.println(NAME);
      makeIFTTTRequest();
    }
    else {
    Serial.println("Odtlacok neznamy");
    } 
  }

  finger.fingerID =0;          //don't ned to run this at full speed.
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Obrazok je vytvoreny");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("Nenajdeny ziadny prst");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Chyba komunikacie");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Chyba zobrazenia ");
      return p;
    default:
      Serial.println("Neznama chyba");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Obrazok zmeneny");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Spinavy obrazok");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Chyba komunikacie");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Nenasli sa funkcie odtlackov prstov");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Nenasli sa funkcie odtlackov prstov");
      return p;
    default:
      Serial.println("Neznama chyba");
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Nasla sa zhoda!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Chyba komunikacie");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Nenasla sa zhoda");
    return p;
  } else {
    Serial.println("Neznama chyba");
    return p;
  }

  // found a match!
  Serial.print("Naslo ID #"); Serial.print(finger.fingerID);
  Serial.print(" zhodne s "); Serial.println(finger.confidence);

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  Serial.print("Naslo ID #"); Serial.print(finger.fingerID);
  Serial.print(" zhodne s "); Serial.println(finger.confidence);
  return finger.fingerID;
}
void makeIFTTTRequest(){
  Serial.print("Pripojene k: ");
  Serial.print(server);

  WiFiClient client;
  int retries = 5;
  while (!!!client.connect(server, 80)&&(retries-- > 0)) {
    Serial.print(".");
  }
  Serial.println();
  if(!!!client.connected()){
    Serial.println("Nepodarilo sa pripojit...");
  }
  Serial.print("Ziadost o odpoved: ");
  Serial.println(resource);

  String jsonObject = String("{\"value1\":\"") + NAME + "\",\"value2\":\"" + ID 
                      + "\"}";
  client.println(String("POST ") + resource + "HTTP/1.1");
  client.println(String("Host: ")+ server);
  client.println("Connection: close\r\nContent-Typ: application/json");
  client.print("Content-Length: ");
  client.println(jsonObject.length());
  client.println();
  client.println(jsonObject);

  int timeout = 5 * 10;
  while(!!!client.available() && (timeout-- > 0)){
    delay(100);
  }  
}

