//---------外部函式宣告區--------
#ifndef _BMC81M001_H__  // 防止重複定義 Header
#define _BMC81M001_H__


//*********************************************//
#include "BMC81M001.h"  // 引入 WiFi 模組控制函式庫
#include <String.h>     // 引入 String 類別（注意 Arduino 已預設內建）

//---------宣告變數--------
//*********** wifi information ****************//
/*** Connect的wifi的Access Point SSID & SSIDPWD*/
#define WIFI_SSID "NUKIOT"     // 無線網路的 SSID (名稱)
#define WIFI_PASS "iot12345"   // 無線網路的密碼


//---------全域變數--------

int LED = 13;   // 內建 LED 腳位，通常可用於指示狀態

#define DEB_CNT     50                // 除錯延遲常數：50 毫秒
#define RES_MAX_LENGTH 200           // 串列緩衝區最大長度

char  SerialBuff[RES_MAX_LENGTH];   // 串列接收資料緩衝區
char  data[30];                     // 用來儲存關鍵資料的緩衝區
int   resLen;                       // 接收資料的實際長度
int   nKeyBuf;                      // Key data 的緩衝處理指標
String DATA_BUF;                   // 暫存資料用的字串變數
String tcpBuff;                    // TCP 傳輸用資料緩衝字串
String ReciveBuff;
int ReciveBufflen;
                //
String topic ; 
#endif

// BMC81M001 Wifi(6,7); // rxPin 6 , txPin 7，若使用軟體序列埠請取消註解此行
// BMC81M001 Wifi(&Serial1); // 若使用 BMduino 板上的硬體 Serial1，請取消註解此行
BMC81M001 Wifi(&Serial2); // 使用 BMduino 板上的硬體 Serial2 腳位控制 WiFi 模組（例如 RX2, TX2）

//----------本程式函式宣告--------------------------------------
void initWiFi() ; // 初始化 WiFi 自訂模組
String GetMAC() ; //取得 MAC 位址字串
String GetSSID() ;  //取得 SSID熱點字串
String GetIP()  ; //取得 連接上的SSID熱點之後由DHCP取得的IP ADDRESS
String GetGateWay() ;  //取得 連接上的SSID熱點之後閘道器IP位址
String GetsubMask() ;  //取得 連接上的SSID熱點之後子遮罩位址
String ScanAP() ;  //取得附近可以連接到的所有熱點
//----------本程式函式宣告--------------------------------------

void initWiFi()   // 初始化 WiFi 自訂模組
{
   Wifi.begin();  // 初始化 WiFi 模組

   Serial.print("init WFIF："); 
   
   // 嘗試連線至指定的 WiFi 熱點
   if(!Wifi.connectToAP(WIFI_SSID, WIFI_PASS)) 
   {
      Serial.print("WIFI fail,");  // 連線失敗
   }  
   else
   {
      Serial.print("WIFI success,");  // 連線成功
   }
   delay(500) ;//wait for all wifi and is ok
}



String GetMAC()   //取得 SSID熱點字串
{
   delay(500); // 等待模組穩定
   String tmp = "" ;  //產生暫存字串
   tmp = Wifi.getMacAddress(); // 從模組取得 MAC 位址
  tmp.toUpperCase(); // 將字串轉成大寫
  return tmp;        // 回傳 MAC 位址字串
}

String GetSSID()   //取得 連接上的SSID熱點字串
{
   delay(500); // 等待模組穩定
    String tmp = "" ;  //產生暫存字串
   if  (Wifi.getStatus())
   {
      tmp = Wifi.getSSID(); // 從模組取得SSID熱點字串
      tmp.toUpperCase(); // 將字串轉成大寫
   }
   else
   {
      tmp = "" ;   
   }

   return tmp;        // 回傳 SSID熱點字串
}

String GetIP()   //取得 連接上的SSID熱點之後由DHCP取得的IP ADDRESS
{
   delay(500); // 等待模組穩定
    String tmp = "" ;  //產生暫存字串
   if  (Wifi.getStatus())
   {
      tmp = Wifi.getIP(); // 從模組取得SSID熱點字串
      tmp.toUpperCase(); // 將字串轉成大寫
   }
   else
   {
      tmp = "" ;   
   }

   return tmp;        // 回傳 SSID熱點字串
}

String GetGateWay()   //取得 連接上的SSID熱點之後閘道器IP位址
{
   delay(500); // 等待模組穩定
    String tmp = "" ;  //產生暫存字串
   if  (Wifi.getStatus())
   {
      tmp = Wifi.getGateway(); // 從模組取得 連接上的SSID熱點之後閘道器IP位址
      tmp.toUpperCase(); // 將字串轉成大寫
   }
   else
   {
      tmp = "" ;   
   }

   return tmp;        // 回傳  連接上的SSID熱點之後閘道器IP位址
}

String GetsubMask()   //取得 連接上的SSID熱點之後子遮罩位址
{
   delay(500); // 等待模組穩定
    String tmp = "" ;  //產生暫存字串
   if  (Wifi.getStatus())
   {
      tmp = Wifi.getMask(); // 從模組取得 連接上的SSID熱點之後子遮罩位址
      tmp.toUpperCase(); // 將字串轉成大寫
   }
   else
   {
      tmp = "" ;   
   }

   return tmp;        // 回傳  連接上的SSID熱點之後子遮罩位址
}

String ScanAP()   //取得附近可以連接到的所有熱點
{
   delay(500); // 等待模組穩定
    String tmp = "" ;  //產生暫存字串
   if  (Wifi.getStatus())
   {
      tmp = Wifi.SSID(); // 取得附近可以連接到的所有熱點
      tmp.toUpperCase(); // 將字串轉成大寫
   }
   else
   {
      tmp = "" ;   
   }

   return tmp;        // 回傳  連接上的SSID熱點之後子遮罩位址
}
