//註冊RFID裝置之雲端門禁裝置  regRFIDDevice_Clouding_BMduino
#include <String.h>
#include "RelayLib.h"  // 引入 TCP 控制邏輯相關函式與變數定義（應包含 WiFi 初始化、MAC 取得等）
#include "TCP.h"  // 引入 TCP 控制邏輯相關函式與變數定義（應包含 WiFi 初始化、MAC 取得等）
#include "OledLib.h"  // OLED 顯示模組自訂函式庫
#include "RFIDLib.h"  //NFC RFID 使用的函式庫，BMC11T001 RFID 讀寫模組通訊
#include "clouding.h"  // http GET 使用的函式庫
//---------全域變數--------
String SSIDData ;//AP SSID 名字
String IPData ;///from AP SSID 取得IP Address 

//----------本程式函式宣告--------------------------------------
void initSensor();   // 初始化所有感測模組
void initAll();   // 初始化整體系統
void PrintMAConOLED(String ss) ;  //顯示MAC Address在Oled上
void PrintSSIDonOLED(String ss);    //顯示AP SSID在Oled上
void PrintIPonOLED(String ss) ;  //顯示from AP取得的 IP Address在Oled上
void PrintCardonOLED(String ss) ;  //顯示卡號在Oled上
void PrintmsgonOLED(String ss) ;  //顯示結果資訊在Oled上


//---------------------------------------------------------------------

void setup() 
{
  initAll() ;  // 初始化整體系統
  Serial.println("");      // 印出空行，純粹格式美觀
  Serial.println("---wifi access point----"); // 分隔線，美觀用途
  SSIDData = GetSSID() ;//取得 SSID熱點字串
  Serial.println(SSIDData); //取得 SSID熱點字串
  // Serial.println(typeof(Wifi.SSID())) ; // 註解掉的除錯語句，用於查資料型別
  IPData = GetIP() ;// 印出取得 連接上的SSID熱點之後閘道器IP位址
    Serial.println("---Show IP Address----"); // 分隔線，美觀用途
  Serial.println( IPData); // 印出取得 連接上的SSID熱點之後閘道器IP位址
  MacData = GetMAC() ; //取得 MAC 位址字串
      Serial.println("---MAC Address----"); // 分隔線，美觀用途
  Serial.println(MacData); // 印出取得 連接上的SSID熱點之後閘道器IP位址
      initAll() ;
    drawPicture(0,0,BestModule_LOGO,128,64)  ;  // 顯示 LOGO 點陣圖
    delay(2000);
    clearScreen()  ;  //清除螢幕
  PrintMAConOLED(MacData); //印出網路卡MAC地址在OLED上
  //PrintSSIDonOLED(SSIDData);    //顯示AP SSID在Oled上
  PrintIPonOLED(IPData) ;  //顯示from AP取得的 IP Address在Oled上
}

void loop()
{
  // 取得目前卡片的 UID 並存入 uid_buf，回傳值為 UID 的長度（bytes）
   if (checkReadRFIDSuccess())  //讀取卡號，如果成功，傳回true，反之false
    {             
        uidStr = readRFIDUIDString() ; //建立讀取RFID UUID卡號函式
        //uidByteStr = readRFIDUUIDString() ; //建立讀取RFID UUID卡號函式(Byte內容字串回傳)
      // 輸出換行符號，讓每筆資料換行顯示
       Serial.print("read RFID Card Number is :(");
       Serial.print(uidStr);
       Serial.print(")\n");
        PrintCardonOLED(uidStr) ;  //顯示from AP取得的 IP Address在Oled上
      if  (Wifi.getStatus())
        {
            Serial.println("WIFI OK") ;
            SendtoClouding() ;    //傳送感測資料到雲端
        }
      //  Serial.print("read RFID Card Number2 is :(");
      //  Serial.print(uidByteStr);
      //  Serial.print(")\n");
       //Serial.write(uid_buf, nlens);
    }
  // 使用 Serial.write 將 UID 的二進位原始值輸出到序列埠監控工具

  // 每 2 秒讀取一次卡片 UID
  delay(2000);
}
void initSensor()   // 初始化所有感測模組
{
  initRelay();  // 初始化 BMP75M131 繼電器模組，啟用 I2C 通訊
  initOled() ;//初始化OLED12864，0.96吋OLED顯示模組 BMD31M090
   //initjson() ;  // 初始化 JSON 資料
   initWiFi() ; // 初始化 WiFi 自訂模組
  initRFID();   // 初始化 RFID 讀卡模組
}

void initAll()   // 初始化整體系統
{
  Serial.begin(9600); // 啟動序列埠，設定速率為 9600 bps（監控輸出與除錯使用）
  digitalWrite(LED, LOW);  // 將 LED 腳位設定為 LOW（熄滅），可視為開機狀態指示
  initSensor();       // 初始化所有感測器，目前僅包含按鍵矩陣模組
}

void PrintMAConOLED(String ss)   //顯示MAC Address在Oled上
{
       printText(0,0,"              ")  ;  //清空
        printText(0,0,ss)  ;  //轉換string字串到char array
       Serial.print("MAC on OLED:(") ;
      Serial.print(ss) ;
      Serial.print(")\n") ;     
}
void PrintSSIDonOLED(String ss)   //顯示AP SSID在Oled上
{
         printText(0,4,"              ")  ;  //清空
        printText(0,4,ss)  ;  //轉換string字串到char array
       Serial.print("SSID on OLED:(") ;
      Serial.print(ss) ;
      Serial.print(")\n") ;     
}
void PrintIPonOLED(String ss)   //顯示from AP取得的 IP Address在Oled上
{
        printText(0,2,"              ")  ;  //清空
        printText(0,2,ss)  ;  //轉換string字串到char array
       Serial.print("IP on OLED:(") ;
      Serial.print(ss) ;
      Serial.print(")\n") ;     
}
void PrintCardonOLED(String ss)   //顯示卡號在Oled上
{
       printText(0,4,"              ")  ;  //清空    
     printText(0,4,ss)  ;  //轉換string字串到char array
       Serial.print("RFID Number on OLED:(") ;
      Serial.print(ss) ;
      Serial.print(")\n") ;     
}
void PrintmsgonOLED(String ss)   //顯示結果資訊在Oled上
{
       printText(0,6,"              ")  ;  //清空  
     printText(0,6,ss)  ;  //轉換string字串到char array
       Serial.print("RFID Number on OLED:(") ;
      Serial.print(ss) ;
      Serial.print(")\n") ;     
}