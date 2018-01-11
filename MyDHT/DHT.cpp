#include "Arduino.h"
#include "DHT.h"

DHT::DHT(const uint8_t pin) : pinIO_(pin), typeDHT_(DHT11)
{
}
DHT::DHT(const uint8_t pin, const uint8_t typeDHT) : pinIO_(pin), typeDHT_(typeDHT)
{
}


uint8_t DHT::getDataDHT()
{
	uint8_t
		data[5] = { 0 },  // массив для данных 5 байт
		indxData = 0,     // индекс для перехода к записи следующего байта
		cntBit = 7;	      // счетчик бит по 8 для создания байта данных

	unsigned long int
		timeOut;	      // таймаут

	// значения в случае ошибки    
	humidity = 255;
	temperature = 255;

	// 1) - - -
	// Запрашиваем соединение с датчиком, прижимаем линию к 0, согласно даташиту, DHT11 на 18мс, DHT22 - 1мс, и отпускаем к 1 - 20мкс
	pinMode(pinIO_, OUTPUT);
	switch (typeDHT_)
	{
		case DHT11:
		{
			delay(18);
			break;
		}
		case DHT22:
		{
			delay(1);
			break;
		}
	}

	// Отпускаем линию и ждем 20мкс
	pinMode(pinIO_, INPUT);
	delayMicroseconds(20);
	// - - -

	// 2) - - -
	timeOut = micros();
	// Слушаем ответ 0 - 80мкс, 1 - 80мкс
	while (digitalRead(pinIO_) == LOW)
	{
		if ( (micros() - timeOut) > 100)
		{
			return 1;
		}
	}
	
	timeOut = micros();
	while (digitalRead(pinIO_) == HIGH)
	{
		if (micros() - timeOut > 100)
		{
			return 2;
		}
	}
	
	// - - -

	// 3) - - -
	// Принимаем данные 40 бит и пишем в 5 байт
	for (int i = 0; i < 40; i++)
	{
		// 0 - 50мкс
		timeOut = micros();
		while (digitalRead(pinIO_) == LOW)
		{
			if ((micros() - timeOut) > 60)
			{
				return 3;
			}
		}

		// сам прием, если длительность импульса 1-цы больше 40мкс, то пишем как 1, иначе 0
		// 0 - 28мкс, 1 - 70мкс
		// засекаем время удержания 1
		timeOut = micros();
		
		while (digitalRead(pinIO_) == HIGH)
		{
			if ((micros() - timeOut) > 100)
			{
				return 4;
			}
		}
		// ... если время удержания больше 40, то пишем в байт 1
		if ((micros() - timeOut) > 40)
		{
			data[indxData] |= 0x01 << cntBit;
		}

		// считаем количество бит, если больше 8 - обнуляем счетчик и записываем новый байт в массив
		if (cntBit-- == 0)
		{
			cntBit = 7;
			indxData++;
		}
	}
	// - - -

	// 4) - - -
	// проверка контрольной суммы
	if ((data[0] + data[1] + data[2] + data[3]) != data[4])
	{
		return 5;
	}
	// 5) - - -
	/*
	data[0] = 0x02;
	data[1] = 0x92;
	data[2] = 0x80;
	data[3] = 0x65;
	*/
	// если все верно, то пишем данные в публичные свойства класса
	switch (typeDHT_)
	{
		case DHT11: 
		{
			humidity = data[0];
			temperature = data[2];
			break;
		}
		case DHT22:
		{
			humidity = (float)((data[0] << 8) | data[1]) / 10 ;
			
			if (data[2] >> 7 == 0x01) 
			{ 	
				temperature = ((data[2] ^ 1 << 7) | data[3]) / -10;
			}
			else temperature = ((data[2] << 8) | data[3]) / 10;
			break;
		}
	}
	return 0;
	// - - -
}

// Метод определения точки росы
float DHT::dewPoint()
{
	if (temperature == 255 && humidity == 255)
		return 255;
	float
		a = 17.27,
		b = 237.7,
		T = temperature,
		RH = humidity / 100;

	return (b * ((a * T) / (b + T) + log(RH))) /
		   (a - ((a * T) / (b + T) + log(RH)));
}
