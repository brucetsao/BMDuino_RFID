/*
BMK52T016 模組：通常是 BestModules 公司推出的 I2C 按鍵矩陣模組，支援中斷觸發。

getINT()：檢查中斷腳是否為低電位，代表有按鍵事件。

readKeyValue()：讀取當前按下的按鍵數值（可能是一個 16 鍵矩陣，每個鍵對應一個值）。

BIN：代表將數值以二進位格式印出，便於觀察每個 bit 狀態。

*/
#include "BMK52T016.h"  // 引入 BMK52T016 模組所需的函式庫

uint8_t buttondelay[] = {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2} ;
uint16_t keymask[] = {0b0000000000000001,0b0000000000000010,0b0000000000000100,0b0000000000001000,0b0000000000010000,0b0000000000100000,0b0000000001000000,0b0000000010000000,0b0000000100000000,0b0000001000000000,0b0000010000000000,0b0000100000000000,0b0001000000000000,0b0010000000000000,0b0100000000000000,0b1000000000000000} ;
// 宣告一個 BMK52T016 物件 BMK52，並指定中斷腳位為 2，並使用 I2C 介面（Wire）
// 若不使用 Wire（I2C），請將這一行註解掉
BMK52T016 BMK52(2, &Wire);

// 若使用 BMduino 開發板上的 Wire1 或 Wire2，可改用以下方式初始化模組
//BMK52T016 BMK52(22, &Wire1); // 使用 Wire1，對應的中斷腳位為 22
//BMK52T016 BMK52(25, &Wire2); // 使用 Wire2，對應的中斷腳位為 25
//--------------------------------------
void initKeyPad() ;//初始化BMK52T016 模組，  I2C 按鍵矩陣模組
boolean setbuttondelay();  ////設定 BMK52T016 模組讀取 I2C 按鍵矩陣模組按鍵延遲速度
int readKey();   //讀取按鍵
void listkeymask() ; //list keypad mask

//--------------------------------------

void initKeyPad() //初始化BMK52T016 模組，  I2C 按鍵矩陣模組
{
    BMK52.begin();         // 初始化 BMK52 模組
    setbuttondelay() ;  ////設定 BMK52T016 模組讀取 I2C 按鍵矩陣模組按鍵延遲速度

}

boolean setbuttondelay()  ////設定 BMK52T016 模組讀取 I2C 按鍵矩陣模組按鍵延遲速度
{
  BMK52.setThreshold(buttondelay) ; //設定 BMK52T016 模組讀取 I2C 按鍵矩陣模組按鍵延遲速度
}

void listkeymask()  //list keypad mask
{
     int rkey = -1;
     for(int i=0 ;i<16;i++)
      {
          rkey = keymask[i] ;
          Serial.print("key Mask:(");
          Serial.print(rkey,BIN);
          Serial.print(")\n");          
      }   
}

int readKey() //讀取按鍵
{  
    uint16_t rkey = -1;
    
    if(BMK52.getINT() == 0)
    {
        rkey = BMK52.readKeyValue() ;//讀取I2C 按鍵矩陣模組按鍵
        Serial.print("in key:(");
        Serial.print(rkey,BIN);
        Serial.print(")\n");

        for(int i=0 ; i<16;i++)
        {
            if ((rkey & keymask[i])==1)
              return (i+1) ;
        }

    }
    else
    {
      return (-1) ;
    }  
    return (-1) ;
}


