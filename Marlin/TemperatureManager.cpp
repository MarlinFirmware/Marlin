#include "TemperatureManager.h" 
#include "Configuration.h"
#include "temperature.h"
#ifdef DOGLCD
	#include "TemperatureControl.h"
#endif

TemperatureManager::TemperatureManager()
	: Subject<float>()
	, m_current_temperature(0)
	, m_accumulate(0)
	, m_control()

{	
setTargetTemperature(0);
SET_OUTPUT(HEATER_0_PIN);
#ifdef FAN_BOX_PIN
	pinMode(FAN_BOX_PIN, OUTPUT);
	digitalWrite(FAN_BOX_PIN, HIGH);
#endif //FAN_BOX_PIN

#ifdef FAN_BLOCK_PIN
	pinMode(FAN_BLOCK_PIN, OUTPUT);
	digitalWrite(FAN_BLOCK_PIN, HIGH);
#endif //FAN_BLOCK_PIN
	m_control = new TemperatureControl();
}

TemperatureManager::~TemperatureManager()
{
	delete m_control;
}

void TemperatureManager::init()
{
	// Init ADC
	ADCSRA = 0x87;
	ADCSRB = 0x08;
	ADMUX = 0x45;
	
	ADCSRA |= 0x40;
	ADCSRA |= 0x10;
	delay(1);

	while ((ADCSRA & 0x10) == 0) {}

	uint16_t initial_raw = 0;
	initial_raw |= ADC;
	initial_raw *= OVERSAMPLENR;

	uint8_t i;
	short (*tt)[][2] = (short (*)[][2]) temptable_99;
	for (i = 1; i < 61; i++)
	{
		if ((short)pgm_read_word(&(*tt)[i][0]) > initial_raw)
		{
			break;
		}
	}

	float initial_temperature = (short)pgm_read_word(&(*tt)[i-1][1]) +
		(initial_raw - (short)pgm_read_word(&(*tt)[i-1][0])) *
		(float)((short)pgm_read_word(&(*tt)[i][1]) - (short)pgm_read_word(&(*tt)[i-1][1])) /
		(float)((short)pgm_read_word(&(*tt)[i][0]) - (short)pgm_read_word(&(*tt)[i-1][0]));

	updateCurrentTemperature(initial_temperature);
	updateLUTCache();


	ADCSRA |= 0x08;
	TCCR2A = 0x23;
	TCCR2B = 0x07;
	TIMSK2 = 0x01;
}

void TemperatureManager::TemperatureConversion(uint16_t accumulate)
{
	for (uint8_t i = 0; i < 4; i++)
		{
			if ( accumulate < TemperatureManager::single::instance().getRawLUTCache(i) )
			{
				float temperature = TemperatureManager::single::instance().getTemperatureLUTCache(i-1) +
					(accumulate - TemperatureManager::single::instance().getRawLUTCache(i-1)) *
					( (float) (TemperatureManager::single::instance().getTemperatureLUTCache(i) - TemperatureManager::single::instance().getTemperatureLUTCache(i-1)) ) /
					( (float) (TemperatureManager::single::instance().getRawLUTCache(i) - TemperatureManager::single::instance().getRawLUTCache(i-1)) );

				updateCurrentTemperature(temperature);
				break;
			}
		}
}

void TemperatureManager::updateLUTCache()
{
	uint8_t i;
	short (*tt)[][2] = (short (*)[][2]) temptable_99;

	for (i = 1; i < 61; i++)
	{
		if ((short)pgm_read_word(&(*tt)[i][1]) <= m_current_temperature)
		{
			i -= 2;
			break;
		}
	}

    for (uint8_t j = 0; j < 4; i++, j++)
    {
    	m_cache[j].raw = pgm_read_word(&(*tt)[i][0]);
    	m_cache[j].temperature = pgm_read_word(&(*tt)[i][1]);
    }
}

short TemperatureManager::getRawLUTCache(uint8_t index)
{
	return m_cache[index].raw;
}

short TemperatureManager::getTemperatureLUTCache(uint8_t index)
{
	return m_cache[index].temperature;
}

void TemperatureManager::updateCurrentTemperature(float temp)
{
	m_control->setCurrentTemperature(temp);
	if (m_current_temperature != temp)
	{
		//SERIAL_ECHOLN(m_current_temperature);
		m_current_temperature = temp;
		notify();
	}
	updateLUTCache();
}

uint16_t TemperatureManager::getCurrentTemperature()
{
	return m_current_temperature;
}

void TemperatureManager::setTargetTemperature(uint16_t target)
{
	#ifdef DOGLCD
		m_control->setTargetControl(target);
	#else
		target_temperature[0] = target;
	#endif
}

const uint16_t TemperatureManager::getTargetTemperature()
{
	#ifdef DOGLCD
		return m_control->getTargetControl();
	#else
		return target_temperature[0];
	#endif
}

void TemperatureManager::notify()
{
	if (this->m_observer != 0)
	{
		this->m_observer->update(m_current_temperature);
	}
}

void TemperatureManager::manageTemperatureControl()
{
	//updateTemperaturesFromRawValues();
	//TemperatureConversion(m_accumulate);
	#ifdef DOGLCD
		m_control->manageControl();
	#else
		manage_heater();
	#endif
}

static bool control_flag = false;

ISR(TIMER2_OVF_vect)
{
	static uint8_t temp_counter = 0;
	if (control_flag == true)
	{
		TemperatureManager::single::instance().manageTemperatureControl();
	 	control_flag = false;
	}

	temp_counter++;
	if (temp_counter == 1)
	{
		ADCSRA |= 0x40;
		temp_counter = 0;
	}
}

ISR(ADC_vect)
{
	static uint8_t sample_number = 0;
	static uint16_t accumulate = 0;

	uint16_t current_sample = ADC;

	accumulate += current_sample;
	sample_number++;

	if (sample_number == OVERSAMPLENR)
	{
		for (uint8_t i = 0; i < 4; i++)
		{
			if ( accumulate < TemperatureManager::single::instance().getRawLUTCache(i) )
			{
				float temperature = TemperatureManager::single::instance().getTemperatureLUTCache(i-1) +
					(accumulate - TemperatureManager::single::instance().getRawLUTCache(i-1)) *
					( (float) (TemperatureManager::single::instance().getTemperatureLUTCache(i) - TemperatureManager::single::instance().getTemperatureLUTCache(i-1)) ) /
					( (float) (TemperatureManager::single::instance().getRawLUTCache(i) - TemperatureManager::single::instance().getRawLUTCache(i-1)) );

				TemperatureManager::single::instance().updateCurrentTemperature(temperature);
				break;
			}
		}
		TemperatureManager::single::instance().updateLUTCache();
		//TemperatureManager::single::instance().m_accumulate = accumulate;
		control_flag = true;
		sample_number = 0;
		accumulate = 0;
	}
}