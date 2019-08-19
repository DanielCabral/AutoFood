//Includes
//RTC
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <time.h>
#include <sys/time.h>
//WIFI e NPT
#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>


#include <Wire.h>
WiFiMulti wifiMulti;
//Variavel que controla a quantidade
int quantidade=0;
long horarios[32];
int posicao=0;
struct tm data;//Cria a estrutura que contem as informacoes da data.
 
// Configurações do WiFi
const char* ssid     = "DESKTOP-MY";
const char* password = "12345678";
 
// Configurações do Servidor NTP
const char* servidorNTP = "a.st1.ntp.br"; // Servidor NTP para pesquisar a hora
 
const int fusoHorario = -10800; // Fuso horário em segundos (-03h = -10800 seg)
const int taxaDeAtualizacao = 1800000; // Taxa de atualização do servidor NTP em milisegundos
 
WiFiUDP ntpUDP; // Declaração do Protocolo UDP
NTPClient timeClient(ntpUDP, servidorNTP, fusoHorario, 60000);


 
void setup()
{  
    Serial.begin(115200);
    wifiMulti.addAP("DESKTOP-MY", "12345678");
    //pegar banco de dados de horarios e inserir no vetor horarios
    requisicaoHTTP();
 
 
  //Iniciar cliente de aquisição do tempo
  //Inicia o servidor NTP
  timeClient.begin();
  //Atualiza o horario atravez do servidor NTP
  timeClient.update();

  
  timeval tv;//Cria a estrutura temporaria para funcao abaixo.
  tv.tv_sec = timeClient.getEpochTime();//Atribui minha data atual em formato Unix Timestamp, 
  settimeofday(&tv, NULL);//Configura o RTC para manter a data atribuida atualizada.
  //escolher no vetor de horarios
  definirPosicao();
}
  
void loop()
{
    vTaskDelay(pdMS_TO_TICKS(1000));//Espera 1 seg
 
 
    time_t tt = time(NULL);//Obtem o tempo atual em segundos. Utilize isso sempre que precisar obter o tempo atual
    data = *gmtime(&tt);//Converte o tempo atual e atribui na estrutura
    
    
    char data_formatada[64];
    strftime(data_formatada, 64, "%H:%M:%S", &data);//Cria uma String formatada da estrutura "data"
 
 
    //printf("\nUnix Time: %d\n", int32_t(tt));//Mostra na Serial o Unix time
    printf("Horario: %s\n", data_formatada);//Mostra na Serial a data formatada

    int hour = (tt / 3600) % 24;
    int m = (tt / 60) % 60;
    int sc=tt%60;
    long s = hour*3600+m*60+sc;
    long tempo=hour+m+s;
    Serial.println(hour);
    Serial.println(m);
    Serial.println(sc);
    imprimir();
    long horarioComSegundos=(s+sc);
    long limite=horarios[posicao]+2;
    if (s >= horarios[posicao] &&  s <horarios[posicao]+3)//Use sua data atual, em segundos, para testar o acionamento por datas e horarios
    {
      printf("Acionando motor durante 3 segundos...\n");
    }
    //Serial.println(quantidade);
    if(s==(horarios[posicao]+3)){
      proximaPosicao();
    }
}

void requisicaoHTTP(){
  // wait for WiFi connection
    if((wifiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;

        Serial.print("[HTTP] begin...\n");

        // configure server and url
       String url = "/AutoFood/pegarHorarios.php?";
        url += "requisicao=";
        url += 10;
        http.begin("http://192.168.0.103"+url);
        //http.begin("192.168.1.12", 80, "/test.html");
        Serial.println("http://10.215.136.193");
        Serial.println(url);

        Serial.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {

                // get lenght of document (is -1 when Server sends no Content-Length header)
                int len = http.getSize();

                // create buffer for read
                char buff[128] = { 0 };

                // get tcp stream
                WiFiClient * stream = http.getStreamPtr();

                // read all data from server
                while(http.connected() && (len > 0 || len == -1)) {
                    // get available data size
                    size_t size = stream->available();

                    if(size) {
                        // read up to 128 byte
                        int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));
                        // write it to Serial
                        Serial.println(buff);
                        quebrarHorarios(buff,c);
                        if(len > 0) {
                            len -= c;
                        }
                    }
                    delay(1);
                }

                Serial.println();
                Serial.print("[HTTP] connection closed or file end.\n");

            }
        } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
    }else{
       Serial.printf("Sem Conexão");
    }

    delay(1000);
  }
  int definirPosicao(){
    time_t tt = time(NULL);
    int hour = (tt / 3600) % 24;
    int m = (tt / 60) % 60;
    long s = hour*3600+m*60;
    for(int i=0;i<quantidade;i++){
      if(horarios[i]>s){
        posicao=i;
        break;
      }else{
        posicao=i;
      }
    }
  }
  int proximaPosicao(){
  if(posicao==quantidade)
    posicao=0;
  else
    posicao+=1;
  }
  void quebrarHorarios(char* s,int c){
  String str(s);
  int i;
  int pos=0;
  for(i=0;i<str.length();i++){
    if(str.substring(i,i+1)=="@"){
      String quebra=str.substring(pos,i);
      int h=quebra.toInt();
      inserirHorario(h);
      pos=i+1;
    }
  }
   String quebra=str.substring(pos,i);
   long h=quebra.toInt();
   inserirHorario(h);
  }

  void inserirHorario(long horario){
    if(quantidade<32){
      horarios[quantidade]=horario;
      int i=quantidade;
      while(i > 0){
         i-=1;
        long horaAnterior=horarios[i];
        if(horario<horaAnterior){
          int aux=horarios[i+1];
          horarios[i+1]=horarios[i];
          horarios[i]=aux;
        }
       }
      quantidade++;
    }
    imprimir();
  }
void imprimir(){
Serial.print("Vetor de horarios- pos[");
Serial.print(posicao);
Serial.println("]");
for(int i=0;i<quantidade;i++){
  Serial.println(horarios[i]);
}
}
 void atualizarHoraRTCdoNTP(){
    timeval tv;//Cria a estrutura temporaria para funcao abaixo.
    tv.tv_sec = timeClient.getEpochTime();//Atribui minha data atual. Voce pode usar o NTP para isso ou o site citado no artigo!
    settimeofday(&tv, NULL);//Configura o RTC para manter a data atribuida atualizada.
 }
