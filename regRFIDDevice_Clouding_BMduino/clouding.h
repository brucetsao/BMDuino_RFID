//http://iot.arduino.org.tw:8888/bigdata/rfid/reglockdevice.php?MAC=B8D61A68DAFC

#define  HTTPGET_PORT_HTTP 80
#define  HTTPGET_PORT_HTTPS 443


#define ServerPort 8888
String ServerURL = "http://iot.arduino.org.tw";
#define dbagent "/bigdata/rfid/reglockdevice.php?MAC=%s"
char dbagentstr[300] ; //sprint 使用之暫存區
 String connectstr ;    //一個空的字串變數，後續用來動態組成完整的 RESTful 請求參數。
String MacData ;//網路卡編號
String webresponse ;//取得http get回傳值

//http://iot.arduino.org.tw:8888/bigdata/rfid/reglockdevice.php?MAC=B8D61A68DAFC
// host is  ==>nuk.arduino.org.tw:8888
//  app program is ==> /bigdata/rfid/reglockdevice.php?MAC=B8D61A68DAFC
//  App parameters ==> ?MAC=B8D61A68DAFC
//http://iot.arduino.org.tw:8888/bigdata/rfid/reglockdevice.php?MAC=B8D61A68DAFC
/*
   完整的 HTTP 請求範例，分解如下：
  主機位址：iot.arduino.org.tw:8888（域名與通訊埠）。
  應用程式路徑：/bigdata/rfid/reglockdevice.php（伺服器上的 PHP 程式，用來接收並處理資料）。
  參數：?MAC=B8D61A68DAFC，表示傳送的資料：
  MAC：設備的唯一識別碼（例如 112233445566）。
 
 */
//--------------------------------------------------------------------------
void SendtoClouding() ;    //傳送感測資料到雲端



//--------------------------------------------------------------------------


void SendtoClouding()     //傳送感測資料到雲端
{
  // 自訂函數，用來將感測器的資料（例如溫度和濕度）透過 HTTP GET 請求傳送到雲端伺服器。
//http://iot.arduino.org.tw:8888/bigdata/dhtdata/dht2json.php?MAC=556885568888&start=20100406&end=20250407
// host is  ==>iot.arduino.org.tw:8888
//  app program is ==> /bigdata/dhtdata/dht2json.php
//  App parameters ==> ?MAC=556885568888&start=20100406&end=20250407
          sprintf(dbagentstr,dbagent,MacData.c_str()) ;
          connectstr = String(dbagentstr) ;
   /*
   組成GET Format 的Resetful  的 Parameters 字串
   connectstr：動態組成 RESTful 請求的參數部分：
   MacData：設備的 MAC 位址（假設已在程式其他地方定義）。
   Tvalue：溫度值，轉換成字串格式。
   Hvalue：濕度值，轉換成字串格式。
   */
          
 Serial.println(connectstr) ;//將組合好的參數字串輸出到序列監控視窗，用於除錯
 
 if  (Wifi.getStatus())
 {
    Wifi.http_begin(ServerURL,ServerPort,connectstr);//begin http get 

    Wifi.http_get();//http get opration
   webresponse = Wifi.http_getString();//get http result
    Serial.println(webresponse);//get http result
    Wifi.http_end(); //end of http get
   }
 else
 {
    initWiFi();              // 執行 WiFi 模組初始化與連線（定義於 TCP.h / BMC81M001.h）
      MacData = GetMAC() ; //取得 MAC 位址字串
    Serial.println("---MAC Address----"); // 分隔線，美觀用途
    Serial.println(MacData); // 印出取得 連接上的SSID熱點之後閘道器IP位址
   Wifi.http_begin(ServerURL,ServerPort,connectstr);//begin http get 
    Wifi.http_get();//http get opration
   webresponse = Wifi.http_getString();//get http result
    Serial.println(webresponse);//get http result
    Wifi.http_end(); //end of http get
 }
}
 
