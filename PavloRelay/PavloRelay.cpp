#include "PavloRelay.h"
#include <Arduino.h>

PavloRelay::PavloRelay() {}

PavloRelay::PavloRelay(float new_setpoint, float new_hysteresis, boolean direction) {
	setpoint = new_setpoint;
	hysteresis = new_hysteresis;
	_direction = direction;
	_relayState = !_direction;		// выключить реле сразу
}

void PavloRelay::setDirection(boolean direction) {
	_direction = direction;
}

int8_t sign(float value) {
	if (value > 0) return 1;
	else if (value < 0) return -1;
}

boolean PavloRelay::compute(float dt) {
	rate = (float)(input - prevInput) / dt;		// производная от величины (величина/секунду)
	prevInput = input;
	signal = input + rate * k;
	
	// жуткая функция реле из лекций
	int8_t F = ((float)sign(signal - setpoint - hysteresis / 2) + (float)sign(signal - setpoint + hysteresis / 2)) / 2;
	
	if (F == 1) _relayState = !_direction;
	else if (F == -1) _relayState = _direction;
}

boolean PavloRelay::getResult() {
	PavloRelay::compute((float)(millis() - prevTime) / 1000);	
	prevTime = millis();
	return _relayState;
}
boolean PavloRelay::getResultTimer() {
	if ((long)millis() > prevTime) {
		prevTime = millis() + sampleTime;		
		PavloRelay::compute((float)sampleTime / 1000);
		return _relayState;
	} else {
		return _relayState;
	}
}

boolean PavloRelay::getResult(float new_input) {
	input = new_input;
	PavloRelay::getResult();
	return _relayState;
}
boolean PavloRelay::getResultTimer(float new_input) {
	input = new_input;
	PavloRelay::getResultTimer();
	return _relayState;
}