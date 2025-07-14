#include "KeypadLib.h"


//----------------------
void initSensor();   //初始化所有感測模組
void initAll() ;  //初始化系統
int keycmd;

//----------------------
void setup() 
{
  initAll() ;  //初始化系統
  listkeymask() ; //list keypad mask
}

void loop() {
  // put your main code here, to run repeatedly:
    keycmd = readKey() ;//讀取按鍵)
    if (keycmd != -1)
      Serial.println(keycmd) ;
    delay(300) ;
}

void initSensor()   //初始化所有感測模組
{
    initKeyPad(); //初始化BMK52T016 模組，  I2C 按鍵矩陣模組
}

void initAll()   //初始化系統
{
  Serial.begin(9600);    // 啟動序列埠，並設定傳輸速率為 9600 bps
  initSensor() ;  //初始化所有感測模組
}