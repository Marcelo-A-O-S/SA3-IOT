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
    boolean init();
    void setSSiD(char *_ssid);
    void setPassword(char *pass);
    void setChannel(int _channel);
    void getSSID();
};

WifiWokwi::WifiWokwi()
{
}
void WifiWokwi::getSSID(){
    //Metodo que exibe no console serial o ssid
    Serial.begin(9600);
    Serial.print(this->ssid);
}
void WifiWokwi::setSSiD(char *_ssid){
    //Inserindo o ssid
    this->ssid = _ssid;
}
void WifiWokwi::setPassword(char *pass){
    //Inserindo o password
    this->password = pass;
}
void WifiWokwi::setChannel(int _channel){
    //Inserindo o canal
    this->channel = _channel;
}
boolean WifiWokwi::init()
{
    //Iniciando a conexão serial
    Serial.begin(9600);
    Serial.println("Connecting to WiFi");
    //Condição caso o canal não seja informado
    if(!this->channel){
        Serial.println("Informações do canal do Getway não informadas!");
        WiFi.begin(this->ssid, this->password);
        while (WiFi.status() != WL_CONNECTED) {
            delay(100);
            Serial.print(".");
        }
        Serial.println(" Connected!");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        return true;
    //Condição caso o SSID não seja informado
    }else if(!this->ssid){
        Serial.println("Informações do SSID do Getway não informadas!");
        return false;
    //Condição caso o Password não seja informado
    }else if(!this->password){
        Serial.println("Informações senha do Getway não informadas!");
        return false;
    //Condição caso todas as informações sejam passadas corretamente
    }else{
        WiFi.begin(this->ssid, this->password, this->channel);
        while (WiFi.status() != WL_CONNECTED) {
            delay(100);
            Serial.print(".");
        }
        Serial.println(" Connected!");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        return true;
    }

}

