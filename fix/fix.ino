#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(13, 15); // d7,d8

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

//String WIFI_SSID = "BISKOM-UAD";
//String WIFI_PASS = "";
String WIFI_SSID = "danC0k";
String WIFI_PASS = "danu12345";

String username = "1700022001"; //username pada halaman login wifi
String password = "danu123"; //password pada halaman login wifi
String host = "192.168.43.60"; //IP PC
String url = "";
String postData;


void login_wifi(){
  
  HTTPClient http;    //Declare object of class HTTPClient 
  postData = "username=" + username + "&password=" + password ;
  http.begin("http://2.2.2.2");           //Specify request destination
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header
  int httpCode = http.POST(postData);   //Send the request
  String payload = http.getString();    //Get the response payload
 
  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload
 
  http.end();  //Close connection
  
  delay(5000);  //Post Data at every 5 seconds
}

void setup()  
{
 
  Serial.begin(115200);
  
  while (!Serial){
    delay(100);
    Serial.println(".");
  }
//  ==============wifi====================
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
//    login_wifi();
  }
//---------------------------------------
  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }
  Serial.println("Waiting for valid finger...");
}

void loop()                     // run over and over again
{
  
  
  int data_id = getFingerprintID();
  delay(50);                    //don't ned to run this at full speed.
  if(data_id){
    send_data(data_id);
  }
}

void send_data(int id){
  WiFiClient client;
  if (!client.connect(host, 80)) {
    Serial.println("connection failed");
    return;
  }
  
  url += "id_mahasiswa=";
  url += id;
  Serial.print("Requesting URL: ");
  Serial.println(url);
 
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
 
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
}


uint8_t getFingerprintID() {
//  Serial.println("put your finger");
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.println(".");
      return 0;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return 0;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return 0;
    default:
      Serial.println("Unknown error");
      return 0;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return 0;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return 0;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return 0;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return 0;
    default:
      Serial.println("Unknown error");
      return 0;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return 0;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    delay(1000);
    return 0;
  } else {
    Serial.println("Unknown error");
    return 0;
  }   
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence); 
  delay(1000);
  return finger.fingerID;
}
