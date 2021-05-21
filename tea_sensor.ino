<<<<<<< HEAD
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <EEPROM.h>

#define BUZZER_PIN D5
#define WIFI_MODE_PIN D6

const char *ssid = "ssid";
const char *pass = "pass";

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer server(80);

bool wifiMode = false;

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
float targetTemp = 100.5;

struct SavedDataStruct {
	float targetTemp = 100;
	char networkName[20] = "";
} saveData;

// ======================= Misc Functions =======================
String GetStatus() {
	StaticJsonDocument<50> doc;
	String responseHTML;

	float ambientTemp = mlx.readAmbientTempF();
	float objectTemp = mlx.readObjectTempF();
	
	doc["ambientTemp"] = ambientTemp;
	doc["objectTemp"] = objectTemp;
	doc["targetTemp"] = saveData.targetTemp;
	
	serializeJson(doc, responseHTML);
	return responseHTML;
}

void SaveData() {
	EEPROM.begin(sizeof(saveData));
	EEPROM.put(0, saveData);
	EEPROM.commit();
	EEPROM.end();
}

void LoadSavedData() {
	String test;
	EEPROM.begin(sizeof(saveData));
	EEPROM.get(0, saveData);
	EEPROM.end();
}


// ======================= Setup =======================
void setup() {
	pinMode(BUZZER_PIN, OUTPUT);
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(WIFI_MODE_PIN, INPUT);
	wifiMode = digitalRead(WIFI_MODE_PIN);

	LoadSavedData();
	
	Serial.begin(9600);
	while (!Serial) {}
	delay(200);
	
	Serial.println("Adafruit MLX90614 test");  
	mlx.begin();
	
	Serial.println();
	
	Serial.println("ESP8266 Web Server Initializing...");
	Serial.println();

	// wifi
	if (wifiMode) {
		Serial.println("Starting WiFi... ");
		WiFi.mode(WIFI_STA);
		WiFi.begin(ssid, pass);
		
		Serial.print("Connecting to ");
		Serial.print(ssid);
		
		while (WiFi.status() != WL_CONNECTED) {
			Serial.print(".");
			digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
			delay(500);
		}
		digitalWrite(LED_BUILTIN, HIGH);
		
		Serial.println("");
		Serial.println("Connected!");
		Serial.print("IP Address: ");
		Serial.println(WiFi.localIP());
		Serial.print("Router IP: ");
		Serial.println(WiFi.gatewayIP().toString());
	}
	else {
		// AP
		
		WiFi.mode(WIFI_AP);
		WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
		WiFi.softAP(saveData.networkName);
		
		// if DNSServer is started with "*" for domain name, it will reply with
		// provided IP to all DNS request
		dnsServer.start(DNS_PORT, "*", apIP);
	}
	
	
	server.on("/", HTTP_GET, HandleRoot);
	server.on("/temperature", HTTP_GET, HandleGetTemperature);
	server.on("/set_temperature", HTTP_GET, HandleSetTemperature);
	server.on("/set_name", HTTP_GET, HandleSetNetworkName);
	server.on("/test_buzzer", HTTP_GET, HandleTestBuzzer);
	server.onNotFound(HandleRoot);
	server.begin();
	
	Serial.println("Initialized");
	digitalWrite(LED_BUILTIN, HIGH);
}


// ======================= Loop =======================
void loop() {
	if (!wifiMode)
		dnsServer.processNextRequest();
	server.handleClient();
	
	delay(1);
}
=======
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <EEPROM.h>

#define BUZZER_PIN D5
#define WIFI_MODE_PIN D6
#define BATTERY_PIN A0

const char *ssid = "ssid";
const char *pass = "pass";

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer server(80);

bool wifiMode = false;

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
float targetTemp = 100.5;

struct SavedDataStruct {
	float targetTemp = 100;
	char networkName[20] = "";
} saveData;

// ======================= Misc Functions =======================
String GetStatus() {
	StaticJsonDocument<100> doc;
	String responseHTML;

	float ambientTemp = mlx.readAmbientTempF();
	float objectTemp = mlx.readObjectTempF();
	
	doc["ambientTemp"] = ambientTemp;
	doc["objectTemp"] = objectTemp;
	doc["targetTemp"] = saveData.targetTemp;
	doc["battery"] = analogRead(BATTERY_PIN);
	
	serializeJson(doc, responseHTML);
	return responseHTML;
}

void SaveData() {
	EEPROM.begin(sizeof(saveData));
	EEPROM.put(0, saveData);
	EEPROM.commit();
	EEPROM.end();
}

void LoadSavedData() {
	String test;
	EEPROM.begin(sizeof(saveData));
	EEPROM.get(0, saveData);
	EEPROM.end();
}


// ======================= Setup =======================
void setup() {
	pinMode(BUZZER_PIN, OUTPUT);
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(WIFI_MODE_PIN, INPUT);
	//wifiMode = digitalRead(WIFI_MODE_PIN);

	LoadSavedData();
	
	Serial.begin(9600);
	while (!Serial) {}
	delay(200);
	
	Serial.println("Adafruit MLX90614 test");  
	mlx.begin();
	
	Serial.println();
	
	Serial.println("ESP8266 Web Server Initializing...");
	Serial.println();

	// wifi
	if (wifiMode) {
		Serial.println("Starting WiFi... ");
		WiFi.mode(WIFI_STA);
		WiFi.begin(ssid, pass);
		
		Serial.print("Connecting to ");
		Serial.print(ssid);
		
		while (WiFi.status() != WL_CONNECTED) {
			Serial.print(".");
			digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
			delay(500);
		}
		digitalWrite(LED_BUILTIN, HIGH);
		
		Serial.println("");
		Serial.println("Connected!");
		Serial.print("IP Address: ");
		Serial.println(WiFi.localIP());
		Serial.print("Router IP: ");
		Serial.println(WiFi.gatewayIP().toString());
	}
	else {
		// AP
		
		WiFi.mode(WIFI_AP);
		WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
		WiFi.softAP(saveData.networkName);
		
		// if DNSServer is started with "*" for domain name, it will reply with
		// provided IP to all DNS request
		dnsServer.start(DNS_PORT, "*", apIP);
	}
	
	
	server.on("/", HTTP_GET, HandleRoot);
	server.on("/temperature", HTTP_GET, HandleGetTemperature);
	server.on("/set_temperature", HTTP_GET, HandleSetTemperature);
	server.on("/set_name", HTTP_GET, HandleSetNetworkName);
	server.on("/test_buzzer", HTTP_GET, HandleTestBuzzer);
	server.onNotFound(HandleRoot);
	server.begin();
	
	Serial.println("Initialized");
	digitalWrite(LED_BUILTIN, HIGH);
}


// ======================= Loop =======================
void loop() {
	if (!wifiMode)
		dnsServer.processNextRequest();
	server.handleClient();

	delay(1);
}
>>>>>>> c2ba34d... battery voltage checking
