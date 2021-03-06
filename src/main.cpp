/*
 * Copyright (C) 2022 Patrick Pedersen, TUDO Makerspace

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 * Description: Main code for the WS2812-Mini-Light Controller
 * 
 */

#include <ws2812_cpp.h>

#ifdef WS2812_TARGET_PLATFORM_ARDUINO_AVR
#include <Arduino.h>
#define DATA_PINS {8}    	///< Arduino pin(s) used to program the WS2812 device(s). Must share same port! (See https://www.arduino.cc/en/Reference/PortManipulation)
#elif defined(WS2812_TARGET_PLATFORM_AVR)
#include <avr/io.h>
#define DATA_PINS_PORT PORTB    ///< Port register used to communicate with the WS2812 device(s)
#define DATA_PINS_DDR DDRB      ///< Data direction register of the pin(s) used to communicate with the WS2812 device(s)
#define DATA_PINS {PB0}    	///< Pin(s) used to communicate with the WS2812 device(s)
#endif

#define N_LEDS N        ///< Number of LEDs on your WS2812 device(s)
#define RESET_TIME 50    ///< Reset time in microseconds (50us recommended by datasheet)
#define COLOR_ORDER grb  ///< Color order of your WS2812 LEDs (Typically grb or rgb)

// Set color here
#define R 255
#define G 255
#define B 255

int main() {
	uint8_t pins[] = DATA_PINS; /// Data pins
	ws2812_cfg cfg; // Device config

#ifdef WS2812_TARGET_PLATFORM_AVR
	// Configure the WS2812 device struct
	cfg.port = &DATA_PINS_PORT;
	cfg.ddr = &DATA_PINS_DDR;
#endif
	cfg.pins = pins;
	cfg.n_dev = sizeof(pins);         // Number of devices driven by this struct
	cfg.rst_time_us = RESET_TIME;
	cfg.order = grb;

	ws2812_cpp ws2812_dev(cfg, nullptr); // WS2812 device
	ws2812_rgb color = {R, G, B}; // Color to set

	// Prepare for color data transmission
	ws2812_dev.prep_tx();

	// Fills strip with color
	for (uint8_t i = 0; i < N_LEDS; i++)
		ws2812_dev.tx(&color, sizeof(color)/sizeof(ws2812_rgb));
	
	// Complete color data transmission
	ws2812_dev.close_tx();
}