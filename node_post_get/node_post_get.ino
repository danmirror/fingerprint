#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

//String WIFI_SSID = "BISKOM-UAD";
//String WIFI_PASS = "";
String WIFI_SSID = "danC0k";
String WIFI_PASS = "danu12345";

String username = "1700022001"; //username pada halaman login wifi
String password = "danu123"; //password pada halaman login wifi

String host = "http://192.168.43.60"; //IP PC
String url = "/api/v1/data?";
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


void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
 
 }

void loop() {
//  send_data();
  
  

}
void send_data(){
  WiFiClient client;
  if (!client.connect(host, 5000)) {
    Serial.println("connection failed");
    return;
  }
  
  url += "id_data=";
  url += "id";
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
