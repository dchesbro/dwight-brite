/**
 * ...
 */

// ...
#include <EEPROM.h>
#include <FastLED.h>
#include <FS.h>
#include <SPIFFS.h>
#include <WebServer.h>
#include <WiFi.h>

// Unknown variables.
const int output_led = 5;

// Define network variables.
const char* ssid     = "Dwight Brite";
const char* password = "password";

// Define web server variables.
String header;

// ...
WebServer webServer( 80 );

/**
 * SPIFFS setup
 * 
 * Initialize ESP32 SPIF filesystem.
 */
void setup_filesys() {
	Serial.print( "SPIF_Filesystem::begin ");

	// If filesystem initialized, print OK...
	Serial.print( "Initializing: " );
	if ( SPIFFS.begin() ) {
		Serial.print( "OK " );
	
	// ...else, print failed and exit.
	} else {
		Serial.print( "Failed " );
		exit( 0 );
	}
	Serial.println();
}

/**
 * SoftAP
 * 
 * Initialize software access point.
 */
void setup_network() {
	Serial.print( "Soft_Access_Point::begin ");

	// If software access point initialized, print OK...
	Serial.print( "Initializing: " );
	if ( WiFi.softAP( ssid, password ) ) {
		Serial.print( "OK " );

		// Print local network IP address.
		IPAddress soft_ap_ip = WiFi.softAPIP();
		Serial.print( "IP Address: " );
		Serial.print( soft_ap_ip );

	// ...else, print failed and exit.
	} else {
		Serial.print( "Failed " );
		exit( 0 );
	}
	Serial.println();
}

/**
 * Web server
 * 
 * Initialize asynchronous web server.
 */
void setup_webserv() {

	// Define static resources.
	webServer.serveStatic( "/css/bootstrap.min.css", SPIFFS, "/css/bootstrap.min.css" );
	webServer.serveStatic( "/js/jquery-3.5.1.slim.min.js", SPIFFS, "/js/jquery-3.5.1.slim.min.js" );
	webServer.serveStatic( "/js/bootstrap.bundle.min.js", SPIFFS, "/js/bootstrap.bundle.min.js" );

	// Define endpoints.
	webServer.serveStatic( "/", SPIFFS, "/index.html" );
	webServer.serveStatic( "/index.html", SPIFFS, "/index.html" );

	webServer.begin();
}

void handleWeb() {
	webServer.handleClient();
}

void setup() {

	// ???
	pinMode( output_led, OUTPUT );
	digitalWrite( output_led, LOW );

	// Initialize serial output.
	Serial.begin( 115200 );

	// ...
	setup_filesys();

	// ...
	setup_network();
	
	// ...
	setup_webserv();
}

void loop() {
	webServer.handleClient();
}
