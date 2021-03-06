// Этот скетч с илюстрацией Бага

/*    22/11/2020 Основное изменение это то что теперь reconnectBlynk(); не войдет
 *    в бесконечный цикл вызова. И это очень повышает стабильность.
 *     
 *    Автор: Обушенков Алексей Андреевич 
 *    Группа в ВК https://vk.com/engineer24
 *    Канал на YouTube https://www.youtube.com/channel/UCih0Gcl9IEdkR8deDJCiodg
 *    Инженерка Engineering room
 *    
 *    
 *****************************************************************************
 * Скетч собран по материалам сайта http://docs.blynk.cc/
 *****************************************************************************
 * Модули в составе проекта
 * ESP8266 - 12E WIFI RobotDyn (3.68 $) https://goo.gl/k6TRUz
 * ESP8266 - 12E WIFI GREAT WALL (3.44 $) https://goo.gl/DcqYMg
 *  
 * Датчики для улицы:
 *  DHT 11 (1.00 $) https://goo.gl/sCBn3d  Тут дешево но долго шло
 *  DHT 11 (3.11 $) https://goo.gl/rBFbBD  Тут дороже но дойдет быстрее
 *  Желательно брать этот, скоро будет вариант и с ним.
 *  GY-BME280-3.3 (3.78 $) https://goo.gl/1eyGmg
 *  
 * Модуль часов реального времени
 *  DS3231 Модуль RTC (1.95 $) https://goo.gl/3jMusY
 *  RTC DS3231 (часы реального времени)RobotDyn + аккумулятор (2.90 $) https://goo.gl/gGMRak
 *  
 * Датчики температуры в помещении и в полу
 *  DS18B20 Датчик на плате (2.11 $) https://goo.gl/T4AmmR
 *  DS18B20 Датчик температуры в корпусе (2.07 $) https://goo.gl/HmbgWM
 *  
 * Реле (выбор реле зависит от мощности нагрузки) 
 *  1-канальное реле с управлением Высоким и Низким уровнем 10Ампер (0,99 $) https://goo.gl/SnFuXY
 *  1-канальное реле с управлением Высоким и Низким уровнем 30Ампер (3.96 $) https://goo.gl/PW1uYL
 * 
 *****************************************************************************
 * Библиотеки
 *****************************************************************************
 * BLYNK   
 *    http://docs.blynk.cc/#downloads-blynk-library
 *   
 *****************************************************************************
 * В чем суть скетча
 *****************************************************************************
 *  При обрыве Интернета по любой причине, было бы неплохо, что бы наше устройство переподключилось.
 *  Раздельно отслеживается подключение к WiFi и серверу Blynk.
 *  Есть вкладка Connect.h в ней находятся функции для подключения и переподключения
 *  Есть вкладка Variables.h в ней можно указать каким сервером пользоваться (Основной или Локальный)
 *  Там же задается Токен и Лоин с Паролем от WiFi
 *  
 *  Но это не панацея, так как наш ESP8266 однозадачный (однопоточный), то весь скетч будет подвисать
 *  на какое то время при попытке переподключения.
 *  
 *  К примеру в этом скетче каждые 10 секунд (лучше установить побольше например 30), 
 *  будет проверятся есть ли связь с сервером Blynk и если нет будем пытаться переподключится,
 *  но если это невозможно (да да же если и возможно), то 5 секунд, 
 *  а то и больше наша ESP !!! НИЧЕГО НЕ БУДЕТ БОЛЬШЕ ДЕЛАТЬ!!!. Будет только подключаться к Blynk.
 *  Равносильно delay думаю так понятней.
 */

/* Закомментируйте это, чтобы отключить BLYNK_LOG и сэкономить место */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include "Variables.h" // Вкладка со всеми переменными

BlynkTimer timer;       // Объявляем класс Таймер
#include "Connect.h"   // Вкладка с функциями подключения к WiFi и Blynk

void Blink(){
  digitalWrite(D0, !digitalRead(D0)); // Инвертируем состояние D0
}

void setup(){
  pinMode(D0, OUTPUT);
// Открываем монитор порта
  Serial.begin(115200);

// Проверяем есть ли связь с сервером каждые 10 секунд
  IDt_reconnectBlynk = timer.setInterval(10*1000, reconnectBlynk);

  // Вызываем функцию мигание светодиодом
  timer.setInterval(1000, Blink);
// Вызываем функцию подключения к Blynk
  reconnectBlynk();
  
}

void loop(){
if (Blynk.connected()){ Blynk.run();} 
  timer.run();
  
  delay(1);
}
