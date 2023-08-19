#include <Arduino.h>
#include <WiFi.h>
class WifiWokwi
{
private:
    char *ssid;
    char *password;
    int channel;
public:
    WifiWokwi();
    void init();
    void setSSiD(char *_ssid);
    void setPassword(char *pass);
    void setChannel(int _channel);
    void getSSID();
};

WifiWokwi::WifiWokwi()
{
}
void WifiWokwi::getSSID(){
    Serial.begin(9600);
    Serial.print(this->ssid);
}
void WifiWokwi::setSSiD(char *_ssid){
    this->ssid = _ssid;
}
void WifiWokwi::setPassword(char *pass){
    this->password = pass;
}
void WifiWokwi::setChannel(int _channel){
    this->channel = _channel;
}
void WifiWokwi::init()
{
    Serial.begin(9600);
    Serial.print("Connecting to WiFi");
    WiFi.begin(this->ssid, this->password, this->channel);
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print(".");
    }
    Serial.println(" Connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

