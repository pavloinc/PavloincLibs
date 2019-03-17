#ifndef PavloRelay_h
#define PavloRelay_h
#include <Arduino.h>

/*
	PavloRelay - библиотека классического релейного регулятора для Arduino
	- Обратная связь по скорости изменения величины
	- Настройка гистерезиса, коэффициента усиления ОС, направления регулирования
	- Возвращает результат по встроенному таймеру или в ручном режиме
*/

#define NORMAL 0
#define REVERSE 1

class PavloRelay
{
  public:
	// принимает установку, ширину гистерезиса, направление (NORMAL, REVERSE)
	// NORMAL - включаем нагрузку при переходе через значение снизу (пример: охлаждение)
	// REVERSE - включаем нагрузку при переходе через значение сверху (пример: нагрев)
    PavloRelay(float new_setpoint, float new_hysteresis, boolean direction);
	PavloRelay();
	
	// расчёт возвращает состояние для управляющего устройства (реле, транзистор) (1 вкл, 0 выкл)
	boolean getResult();						// расчёт
	boolean getResult(float new_input);			// расчёт, принимает текущую величину с датчика
	boolean getResultTimer();					// расчёт по встроенному таймеру
	boolean getResultTimer(float new_input);	// расчёт, принимает текущую величину с датчика (+ по встроенному таймеру)
	
	void setDirection(boolean);					// направление регулирования (NORMAL, REVERSE)
	
	float input;						// сигнал с датчика (например температура, которую мы регулируем)
	float setpoint;						// заданная величина, которую должен поддерживать регулятор (температура)
	
	float signal;						// сигнал (для отладки)
	float hysteresis;					// ширина гистерезиса (половина в минус, половина в плюс)
	float k = 0;						// коэффициент усиления	по скорости (по умолч. 0)
	float rate;							// скорость изменения величины (производная)
	int16_t sampleTime = 1000;			// время итерации, мс (по умолч. секунда)
		
  private:
	boolean compute(float);
	uint32_t prevTime = 0;
	float prevInput = 0.0;
	boolean _direction = false;
	boolean _relayState = false;
};

#endif