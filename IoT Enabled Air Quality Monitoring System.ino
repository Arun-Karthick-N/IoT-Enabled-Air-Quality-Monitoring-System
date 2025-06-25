#include <ESP8266WiFi.h>
#include <DHT.h>
#include <ThingSpeak.h>

DHT dht(D2, DHT11);
#define LED_GREEN D6
#define LED_YELLOW D7
#define LED_RED D8
#define MQ_135 A0
#define BUZZ D0

float m = -0.3376;
float b = 0.7165; 
float R0 = 3.12;   

WiFiClient client;
long myChannelNumber = ********; 
const char myWriteAPIKey[] = "***************";

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZ, OUTPUT);
  pinMode(MQ_135, INPUT);
     
  WiFi.begin("123", "72345678");
  while(WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("NodeMCU is connected!");
  Serial.println(WiFi.localIP());
  
  dht.begin();
  ThingSpeak.begin(client);
  
}

void loop() 
  {
  digitalWrite(LED_BUILTIN, HIGH);
  float h = dht.readHumidity();
  delay(150); 
  float t = dht.readTemperature();
  delay(150); 
  
  int sensorValue = analogRead(MQ_135); 
  float sensor_volt = sensorValue * (5.0 / 1023.0);
  float RS_gas = ((5.0 * 1.0) / sensor_volt) - 1.0;
  float ratio = RS_gas / R0; 
  float ppm_log = (log10(ratio) - b) / m; 
  float ppm = pow(10, ppm_log); 
  
  Serial.println("-----------------------------------------------------------------------------------");
  Serial.println("Temperature: " + (String) t);
  Serial.println("Humidity: " + (String) h);
  Serial.println("Our desired PPM = " + (String) ppm);
  Serial.println("-----------------------------------------------------------------------------------");
 
  ThingSpeak.writeField(myChannelNumber, 1, ppm, myWriteAPIKey);
  delay(150);
  ThingSpeak.writeField(myChannelNumber, 2, t, myWriteAPIKey);
  delay(150);
  ThingSpeak.writeField(myChannelNumber, 3, h, myWriteAPIKey);
  delay(150);
  
  if (ppm > 1000) 
  {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣");
    Serial.println("☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣");
    Serial.println("☣☣☣☣               𝐓𝐎𝐗𝐈𝐂 𝐆𝐀𝐒 𝐃𝐄𝐓𝐄𝐂𝐓𝐄𝐃               ☣☣☣☣");
    Serial.println("☣☣☣☣            𝐀𝐋𝐄𝐑𝐓𝐈𝐍𝐆 𝐔𝐒𝐄𝐑 : 𝐂𝐀𝐋𝐋𝐈𝐍𝐆              ☣☣☣☣");
    Serial.println("☣☣☣☣                𝐒𝐄𝐍𝐃𝐈𝐍𝐆 𝐌𝐄𝐒𝐒𝐀𝐆𝐄                 ☣☣☣☣");
    Serial.println("☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣");
    Serial.println("☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣☣");
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, HIGH);
    digitalWrite(BUZZ, HIGH);
  } 
  else if (ppm > 700) { 
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(LED_RED, LOW);
    digitalWrite(BUZZ, LOW);
  } 
  else 
  {
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, LOW);
    digitalWrite(BUZZ, LOW);
  }
  delay(150);
}
