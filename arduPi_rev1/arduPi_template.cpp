//Include ArduPi library
#include "arduPi.h"

//Needed for Serial communication
SerialPi Serial;

//Needed for accesing GPIO (pinMode, digitalWrite, digitalRead, I2C functions)
WirePi Wire;

//Needed for SPI
SPIPi SPI;

/*********************************************************
 *  IF YOUR ARDUINO CODE HAS OTHER FUNCTIONS APART FROM  *
 *  setup() AND loop() YOU MUST DECLARE THEM HERE        *
 * *******************************************************/

/**************************
 * YOUR ARDUINO CODE HERE *
 * ************************/

int main (){
	setup();
	while(1){
		loop();
	}
	return (0);
}
