#include "KeypadLib.h"  // 引入自定義的按鍵矩陣函式庫（應該包含 inputKey() 等相關函式）
/*
inputKey()：此函式應該來自您先前的程式碼或 KeypadLib.h，它會偵測哪顆鍵被按下，並回傳對應的 char 字元，例如 '1'、'A'、'#' 等，若無按鍵則回傳 'E'。

Serial.begin(9600)：這是與序列監控視窗通訊的初始化步驟，在 Arduino IDE 裡你可以透過 工具 > 序列監控視窗 查看輸出的按鍵值。

此程式架構非常簡潔，適合作為初學者練習按鍵讀取與處理邏輯。
*/

//----------------------
// 前置宣告函式與變數
void initSensor();   // 初始化所有感測模組
void initAll();      // 初始化整體系統（包含序列埠與感測模組）
char keycmd;         // 用來儲存目前讀取到的按鍵命令（char 形式）


//----------------------
void setup() 
{
  initAll();  // 在程式啟動時，先初始化整個系統
  // listkeymask(); //（註解掉）列出每個按鍵對應的 keymask，用於偵錯
}

void loop() {
  // 主循環程式，會一直重複執行

  keycmd = inputKey();  // 透過 inputKey() 讀取按鍵（回傳型別為 char）
                        // 若無按鍵則會回傳 'E'

  if (keycmd != 'E')    // 如果有按鍵被按下（不是 'E'）
    Serial.println(keycmd); // 將按下的按鍵字元輸出至序列監控視窗

  delay(200); // 延遲 300 毫秒，可避免重複觸發（目前為註解狀態）
}

void initSensor()   // 初始化所有感測模組
{
  initKeyPad();     // 初始化按鍵矩陣模組（BMK52T016），透過 I2C 通訊
}

void initAll()   // 初始化整體系統
{
  Serial.begin(9600); // 啟動序列埠，設定速率為 9600 bps（監控輸出與除錯使用）
  initSensor();       // 初始化所有感測器，目前僅包含按鍵矩陣模組
}
