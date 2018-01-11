#include "DHT.h"
#define pinIO 2


DHT sensor(pinIO);
uint8_t error;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
 
  /*
   * Возвращаемые значения getDataDHT()
   * Ошибки: 1   - без ошибок
   *         2-5 - таймаут
   *         6   - неверная контрольная сумма
   * если метод getDataDHT() возвращает ошибку,
   * то все данные возвращают 255
   */
  if ((error = sensor.getDataDHT()) == 1) {
    Serial.println(sensor.humidity);
    Serial.println(sensor.temperature);
    Serial.println(sensor.dewPoint());
  }
  else {
    Serial.print("Ошибка: ");
    Serial.println(error);
  }

  delay(3000);

}
