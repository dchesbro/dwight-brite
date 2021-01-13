/**
 * ...
 */

// ...
#include <EEPROM.h>
#include <FastLED.h>
#include <FastLED_NeoMatrix.h>
#include <SPIFFS.h>
#include <WebServer.h>
#include <WiFi.h>

// ...
#include "settings.h"

	// Network settings.
	const char*    ssid           = "Dwight Brite"; // WiFi network SSID.
	const char*    password       = "password";     // WiFi network password

	// FastLED settings.
	const uint16_t led_pin        = 14;             // LED ouput pin.
	const uint16_t led_brightness = 25;             // Default LED brightness (0-255).
	const uint16_t led_w          = 40;             // LED matrix width.
	const uint16_t led_h          = 30;             // LED matrix height.
	const uint16_t led_num        = led_w * led_h;  // Total number of LEDs.
	const uint16_t led_milliamps  = 4000;           // Max milli-amps of power supply.

	// Unknown settings.
	const uint8_t output_led = 5;

// ...
WebServer server( 80 );

// ...
CRGB leds[led_num];
FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix( leds, led_w, led_h, 1, 1, 
	NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
	NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG );

// ...
#include "fonts/Jamboree4pt7b.h"

/**
 * SPIFFS
 * 
 * Setup ESP32 SPIF filesystem.
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
 * Setup software access point.
 */
void setup_network() {
	Serial.print( "Soft_Access_Point::begin ");

	// If software access point initialized, print OK...
	Serial.print( "Initializing: " );
	if ( WiFi.softAP( ssid, password ) ) {
		Serial.print( "OK " );

		// Print local network IP address.
		Serial.print( "IP Address: " + WiFi.softAPIP().toString() );

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
 * Setup asynchronous web server.
 */
void setup_webserv() {

	// Define static resources.
	server.serveStatic( "/css/bootstrap.min.css", SPIFFS, "/css/bootstrap.min.css" );
	server.serveStatic( "/js/jquery-3.5.1.slim.min.js", SPIFFS, "/js/jquery-3.5.1.slim.min.js" );
	server.serveStatic( "/js/bootstrap.bundle.min.js", SPIFFS, "/js/bootstrap.bundle.min.js" );

	// Define endpoints.
	server.serveStatic( "/", SPIFFS, "/index.html" );
	server.serveStatic( "/index.html", SPIFFS, "/index.html" );

	server.begin();
}

/**
 * FastLED
 * 
 * Setup leds with default values.
 */
void setup_fastled() {
	FastLED.addLeds<WS2812B, led_pin, GRB>( leds, led_num ).setCorrection( TypicalLEDStrip );
	
	/**
	 * TODO: calulate maximum milliamps.
	 * 
	 * FastLED.setMaxPowerInVoltsAndMilliamps( 5, led_milliamps );
	 */

	FastLED.setBrightness( led_brightness );
}

/**
 * ...
 */
uint16_t msg_index = 0;

void text_flasher() {
	String msg_array[] = {
		"EAT",
		"SHIT",
		"ANNA",
	};

	// ...
	int16_t  x, y, x1, y1;
	uint16_t w, h;
	uint16_t msg_size = ( sizeof( msg_array ) / sizeof( String ) ) - 1;

	// ...
	matrix->setFont( &Jamboree4pt7b );
	matrix->getTextBounds( msg_array[msg_index], x, y, &x1, &y1, &w, &h );

	// ...
	uint16_t cur_x = ( ( led_w - 1 ) / 2 ) - ( w / 2 );
	uint16_t cur_y = ( ( led_h - 1 ) / 2 ) + ( h / 2 );

	if ( msg_index < msg_size ) {
		matrix->fillScreen( matrix->Color( 0, 0, 0 ) );
		matrix->setTextColor( matrix->Color( 255, 0, 0 ) );
		matrix->setCursor( cur_x, cur_y );
		matrix->print( msg_array[msg_index] );

		// ...
		matrix->show();
		delay( 1000 );

		// ...
		msg_index++;
	} else {
		for ( int i = 0; i < 15; i++ ) {
			if ( i % 2 == 0 ) {
				matrix->fillScreen( matrix->Color( 255, 0, 0 ) );
				matrix->setTextColor( matrix->Color( 0, 0, 0 ) );
			} else {
				matrix->fillScreen( matrix->Color( 0, 0, 0 ) );
				matrix->setTextColor( matrix->Color( 255, 0, 0 ) );
			}

			matrix->setCursor( cur_x, cur_y );
			matrix->print( msg_array[msg_index] );

			// ...
			matrix->show();
			delay( 100 );
		}

		// ...
		msg_index = 0;
	} 
}

void setup() {

	// ???
	pinMode( output_led, OUTPUT );
	digitalWrite( output_led, LOW );

	// Start serial output.
	Serial.begin( 115200 );

	// Setup filesystem, network, web server, and LED matrix.
	setup_filesys();
	setup_network();
	setup_webserv();
	setup_fastled();
}

void loop() {

	// ...
	server.handleClient();

	text_flasher();

	// FastLED.show();

	// delay( 1000 );
}
