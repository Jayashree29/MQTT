#include <WiFi.h>
#include <PubSubClient.h>




const char* ssid = "Robocraze";
const char* password = "crazeindia";
 const char* mqtt_server = "m14.cloudmqtt.com";


WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {
   delay(100);
  // We start by connecting to a WiFi network
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, String* payload, unsigned int length) 
{
  Serial.print("Command from MQTT broker is : [");
  Serial.print(topic);
  
  
  if(payload=="read") 
  {
      int n=WiFi.scanNetworks();
      for(int i=0;i<n;i++)
      {
        if(WiFi.SSID(i)=="ESP")
        {
          Jayashree.publish("MAC address is"+WiFi.BSSIDstr(i));
          Jayashree.publish("signal strength is"+WiFi.RSSI(i));
        }
      }
  } 
  
} 

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");

    if (client.connect("Jay cloudMQTT","fmxunydg","gpTMTdFM484y"))
    {
      Serial.println("connected");
     //once connected to MQTT broker, subscribe command if any
      client.subscribe("Jayashree");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 6 seconds before retrying
      delay(6000);
    }
  }
} //end reconnect()

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 10466);
  client.setCallback(callback);
    
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

}
