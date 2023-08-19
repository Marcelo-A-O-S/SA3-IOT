#include <Arduino.h>
#include <MonitorTemHum.h>
#include <WifiWokwi.h>
#include <MQTT.h>
WifiWokwi *wifi = new WifiWokwi();
MonitorTemHum *monitor = new MonitorTemHum();
MQTT *mqtt = new MQTT();
void setup(){
    wifi->setSSiD("Wokwi-GUEST");
    wifi->setPassword("");
    wifi->setChannel(6);
    wifi->init();
    mqtt->setClient();
    mqtt->setStringServer("mqtt-dashboard.com");
    mqtt->setPort(8884);
    mqtt->setTopic("Teste-tdes");
    mqtt->init();
    mqtt->connect();
    monitor->setPortSDALCD(21);
    monitor->setPortSLCLCD(22);
    monitor->setPinOutHum(2);
    monitor->setPinOutTemp(12);
    monitor->setPinSensor(14);
    monitor->Monitorinit();

}
void loop() {
    String payload = monitor->avaliableTempAndHum();
    if(!mqtt->client->connected()){
        mqtt->connect();
    }
    mqtt->Publish(payload);
    delay(1000);
}
