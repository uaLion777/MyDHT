/**
 * Библиотека получения данных с датчика DHT11, DHT22
 * под платформу Arduino IDE 
 * Дата: 25.12.2017
 * Версия: 1.1
 * Изменения: добавленна поддержка DHT22, 09.01.2018
 * Автор: uaLion777
 * Контакты: agro.sprava@gmail.com
 */

#pragma once

#ifndef DHT_H
#define DHT_H
typedef unsigned char uint8_t;

enum {
	DHT11 = 0x0B,
	DHT22 = 0x16
};

class DHT
{
private:
	const uint8_t
		pinIO_,
		typeDHT_;

public:
	float
		humidity,
		temperature;
	DHT(const uint8_t);
	DHT(const uint8_t, const uint8_t);
	uint8_t getDataDHT();
	float dewPoint();
};

#endif /* DHT_H */
