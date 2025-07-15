// OLED 顯示模組自訂函式庫
/*****************************************************************
  檔案名稱：display.ino
  功能說明：
  1. 使用 I2C 通訊介面與 BMD31M090 OLED 顯示模組通訊（頻率：400KHz）
  2. 使用硬體序列埠 Serial（115200 bps）與電腦序列監控器通訊
  硬體接腳說明：
    SCL 接腳：D19
    SDA 接腳：D18
******************************************************************/
//---------外部函式宣告區--------
#include "BMD31M090.h"  // OLED 顯示模組函式庫
#include "BestModuleLogo.h"     // 含 LOGO 圖示的點陣圖資料

//---------宣告變數--------
// 定義顯示模組參數
#define BMD31M090_WIDTH   128     // OLED 寬度為 128 像素
#define BMD31M090_HEIGHT  64      // OLED 高度為 64 像素
#define BMD31M090_ADDRESS 0x3C    // 預設的 I2C 位址，可為 0x3C 或 0x3D（視模組設定）

//---------全域變數--------
uint8_t t = ' ';  // 用來顯示 ASCII 字元與編碼的變數

// 建立 OLED 物件（此例使用 Wire1，也可用 Wire / Wire2）
BMD31M090 BMD31(BMD31M090_WIDTH, BMD31M090_HEIGHT, &Wire1);
//----------本程式函式宣告--------------------------------------
void initOled() ;//初始化OLED12864，0.96吋OLED顯示模組 BMD31M090
void drawPicture(int x,int y, const uint8_t *pp,int width, int height) ;
void clearScreen()  ;  //清除螢幕
void updateScreen() ;  //顯示當前緩衝區的內容
void setFont(const unsigned char* font) ;  //設定字形
    //   font：字體格式
    // FontTable_6X8：字體格式為 6×8
    // FontTable_8X16：字體格式為 8×16
    // FontTable_16X32：字體格式為 16×32
    // FontTable_32X64：字體格式為 32×64
void printText(int x,int y, uint8_t *str) ;  //在xcolumn,y row位置，印出文字
void printChar(int x,int y, char str) ;  //在xcolumn,y row位置，印出文字
void printNumber(int x,int y, int num);   //在xcolumn,y row位置，印出整數
void printFloat(int x,int y, float num);  //在xcolumn,y row位置，印出浮點數
void drawPoint(int x,int y,int pixelColor); //在xcolumn,y row位置，繪出一點
    //   x：x 座標，範圍 0~127
    // y：y 座標，範圍 0~63
    // pixelColor：像素顏色
    // 0 (pixelColor_BLACK)：像素顏色為黑
    // 1 (pixelColor_WHITE)：像素顏色為白
    // 2 (pixelColor_INVERSE)：像素顏色翻轉
void drawLine(int x1,int y1,int x2,int y2,int pixelColor); //畫(x1,y1) - (x2,y2) 的一條直線
    // x_Start：起點 x 座標
    // y_Start：起點 y 座標
    // x_End：終點 x 座標
    // y_End：終點 y 座標
    //   pixelColor：像素顏色
    //     0 (pixelColor_BLACK)：像素顏色為黑
    //     1 (pixelColor_WHITE)：像素顏色為白
    //     2 (pixelColor_INVERSE)：像素顏色翻轉
void drawfastVline(int x,int y, int width, int pixelColor);  //在xcolumn,y row位置，繪出width寬度的畫垂直線
    // x：x 座標，範圍 0~127
    // y：y 座標，範圍 0~63
    // height：高度，height(h)≤64
    //   pixelColor：像素顏色
    //     0 (pixelColor_BLACK)：像素顏色為黑
    //     1 (pixelColor_WHITE)：像素顏色為白
    //     2 (pixelColor_INVERSE)：像素顏色翻轉
void drawVline(int x,int y, int width, int pixelColor);  //在xcolumn,y row位置，繪出width寬度的畫垂直線
    // x：x 座標，範圍 0~127
    // y：y 座標，範圍 0~63
    // height：高度，height(h)≤64
    //   pixelColor：像素顏色
    //     0 (pixelColor_BLACK)：像素顏色為黑
    //     1 (pixelColor_WHITE)：像素顏色為白
    //     2 (pixelColor_INVERSE)：像素顏色翻轉
void drawVline(int x,int y, int width, int pixelColor) ; //在xcolumn,y row位置，繪出width寬度的畫垂直線
    // x：x 座標，範圍 0~127
    // y：y 座標，範圍 0~63
    // height：高度，height(h)≤64
    //   pixelColor：像素顏色
    //     0 (pixelColor_BLACK)：像素顏色為黑
    //     1 (pixelColor_WHITE)：像素顏色為白
    //     2 (pixelColor_INVERSE)：像素顏色翻轉


//--------------------------------------
void initOled() //初始化OLED12864，0.96吋OLED顯示模組 BMD31M090
{
  BMD31.begin(BMD31M090_ADDRESS);  // 初始化 OLED 顯示模組
  delay(100);  // 建議初始化延遲時間

}
void setFont(const unsigned char* font)   //設定字形
{
    //   font：字體格式
    // FontTable_6X8：字體格式為 6×8
    // FontTable_8X16：字體格式為 8×16
    // FontTable_16X32：字體格式為 16×32
    // FontTable_32X64：字體格式為 32×64
    BMD31.setFont(font);  // 設定字型為 6x8
}

void updateScreen()   //顯示當前緩衝區的內容
{
  BMD31.display();  //顯示當前緩衝區的內容
}

void printText(int x,int y, String str)  //在xcolumn,y row位置，印出文字
{
  BMD31.drawString((uint8_t)x, (uint8_t)y, (uint8_t*)str.c_str());
}

void printChar(int x,int y, char str)  //在xcolumn,y row位置，印出文字
{
  BMD31.drawChar((uint8_t)x, (uint8_t)y, (uint8_t)str);
}

void printNumber(int x,int y, int num)  //在xcolumn,y row位置，印出整數
{
  BMD31.drawNum((uint8_t)x, (uint8_t)y, (uint32_t)num,(uint8_t)(String(num).length()) );
}

void printFloat(int x,int y, float num)  //在xcolumn,y row位置，印出浮點數
{
  printText(x, y, String(num) );
}

void drawPoint(int x,int y,int pixelColor) //在xcolumn,y row位置，繪出一點
{
    //   x：x 座標，範圍 0~127
    // y：y 座標，範圍 0~63
    // pixelColor：像素顏色
    // 0 (pixelColor_BLACK)：像素顏色為黑
    // 1 (pixelColor_WHITE)：像素顏色為白
    // 2 (pixelColor_INVERSE)：像素顏色翻轉
    BMD31.drawPixel((uint8_t)x, (uint8_t)y, (uint8_t)pixelColor) ;
    BMD31.display();
}

void drawLine(int x1,int y1,int x2,int y2,int pixelColor) //畫(x1,y1) - (x2,y2) 的一條直線
{
    // x_Start：起點 x 座標
    // y_Start：起點 y 座標
    // x_End：終點 x 座標
    // y_End：終點 y 座標
    //   pixelColor：像素顏色
    //     0 (pixelColor_BLACK)：像素顏色為黑
    //     1 (pixelColor_WHITE)：像素顏色為白
    //     2 (pixelColor_INVERSE)：像素顏色翻轉
  BMD31.drawLine((uint8_t)x1,(uint8_t)y1,(uint8_t)x2,(uint8_t)y2,(uint8_t)pixelColor) ;//畫(x1,y1) - (x2,y2) 的一條直線
  BMD31.display();
}
void drawfastVline(int x,int y, int width, int pixelColor)  //在xcolumn,y row位置，繪出width寬度的畫垂直線
{
    // x：x 座標，範圍 0~127
    // y：y 座標，範圍 0~63
    // height：高度，height(h)≤64
    //   pixelColor：像素顏色
    //     0 (pixelColor_BLACK)：像素顏色為黑
    //     1 (pixelColor_WHITE)：像素顏色為白
    //     2 (pixelColor_INVERSE)：像素顏色翻轉
    BMD31.drawFastVLine((uint8_t)x,(uint8_t)y,(uint8_t)width,(uint8_t)pixelColor) ;//在xcolumn,y row位置，繪出width寬度的畫垂直線
  BMD31.display();
}

void drawfastHline(int x,int y, int width, int pixelColor)  //在xcolumn,y row位置，繪出width寬度的畫水平線
{
    // x：x 座標，範圍 0~127
    // y：y 座標，範圍 0~63
    // height：高度，height(h)≤64
    //   pixelColor：像素顏色
    //     0 (pixelColor_BLACK)：像素顏色為黑
    //     1 (pixelColor_WHITE)：像素顏色為白
    //     2 (pixelColor_INVERSE)：像素顏色翻轉
    BMD31.drawFastHLine((uint8_t)x,(uint8_t)y,(uint8_t)width,(uint8_t)pixelColor) ;//在xcolumn,y row位置，繪出width寬度的畫水平線
  BMD31.display();
}

void drawVline(int x,int y, int width, int pixelColor)  //在xcolumn,y row位置，繪出width寬度的畫垂直線
{
    // x：x 座標，範圍 0~127
    // y：y 座標，範圍 0~63
    // height：高度，height(h)≤64
    //   pixelColor：像素顏色
    //     0 (pixelColor_BLACK)：像素顏色為黑
    //     1 (pixelColor_WHITE)：像素顏色為白
    //     2 (pixelColor_INVERSE)：像素顏色翻轉
    BMD31.drawFastVLine((uint8_t)x,(uint8_t)y,(uint8_t)width,(uint8_t)pixelColor) ;//在xcolumn,y row位置，繪出width寬度的畫垂直線
  BMD31.display();
}

void drawHline(int x,int y, int width, int pixelColor)  //在xcolumn,y row位置，繪出width寬度的畫水平線
{
    // x：x 座標，範圍 0~127
    // y：y 座標，範圍 0~63
    // height：高度，height(h)≤64
    //   pixelColor：像素顏色
    //     0 (pixelColor_BLACK)：像素顏色為黑
    //     1 (pixelColor_WHITE)：像素顏色為白
    //     2 (pixelColor_INVERSE)：像素顏色翻轉
    BMD31.drawFastHLine((uint8_t)x,(uint8_t)y,(uint8_t)width,(uint8_t)pixelColor) ;//在xcolumn,y row位置，繪出width寬度的畫水平線
  BMD31.display();
}

void drawBox(int x1,int y1,int x2,int y2,int pixelColor) //畫(x1,y1) - (x2,y2) 的對角的一個矩形
{
    // x_Start：起點 x 座標
    // y_Start：起點 y 座標
    // x_End：終點 x 座標
    // y_End：終點 y 座標
    //   pixelColor：像素顏色
    //     0 (pixelColor_BLACK)：像素顏色為黑
    //     1 (pixelColor_WHITE)：像素顏色為白
    //     2 (pixelColor_INVERSE)：像素顏色翻轉
  BMD31.drawLine((uint8_t)x1,(uint8_t)y1,(uint8_t)x2,(uint8_t)y1,(uint8_t)pixelColor) ;//畫(x1,y1) - (x2,y1) 的一條直線
  BMD31.drawLine((uint8_t)x2,(uint8_t)y1,(uint8_t)x2,(uint8_t)y2,(uint8_t)pixelColor) ;//畫(x2,y1) - (x2,y2) 的一條直線
  BMD31.drawLine((uint8_t)x1,(uint8_t)y1,(uint8_t)x1,(uint8_t)y2,(uint8_t)pixelColor) ;//畫(x1,y1) - (x1,y2) 的一條直線
  BMD31.drawLine((uint8_t)x1,(uint8_t)y2,(uint8_t)x2,(uint8_t)y2,(uint8_t)pixelColor) ;//畫(x1,y2) - (x2,y2) 的一條直線
  BMD31.display();
}


void clearScreen()    //清除螢幕
{
  BMD31.clearDisplay(); //清除螢幕資訊
  BMD31.display();  //更新螢幕資訊到螢幕硬體
}




void invdrawPicture(int x,int y, const uint8_t *pp,int width, int height) //使用黑色畫圖
{

    // x：x 座標，範圍 0~127
    // y：y 座標，範圍 0~63
    // *Bitmap：點陣圖名稱
    // w：點陣圖寬度
    // h：點陣圖高度
    // pixelColor：像素顏色
    // 0 (pixelColor_BLACK)：像素顏色為黑
    // 1 (pixelColor_WHITE)：像素顏色為白
    // 2 (pixelColor_INVERSE)：像素顏色翻轉

  BMD31.clearDisplay();
  BMD31.drawBitmap((uint8_t)x, (uint8_t)y, pp, (uint8_t)width, (uint8_t)height, pixelColor_BLACK);
  BMD31.display();


}
void drawPicture(int x,int y, const uint8_t *pp,int width, int height)  //使用白色畫圖
{

    // x：x 座標，範圍 0~127
    // y：y 座標，範圍 0~63
    // *Bitmap：點陣圖名稱
    // w：點陣圖寬度
    // h：點陣圖高度
    // pixelColor：像素顏色
    // 0 (pixelColor_BLACK)：像素顏色為黑
    // 1 (pixelColor_WHITE)：像素顏色為白
    // 2 (pixelColor_INVERSE)：像素顏色翻轉
  BMD31.clearDisplay();
  BMD31.drawBitmap((uint8_t)x, (uint8_t)y, pp, (uint8_t)width, (uint8_t)height, pixelColor_WHITE);
  BMD31.display();


}

void setsaveMode()  // 降低亮度（省電模式）
{
  // dim：亮度選擇
  //   true：暗
  //   false：正常亮度
  BMD31.dim(TRUE);   // 降低亮度（省電模式）
}

void setlightMode() // 回復正常亮度
{
    // dim：亮度選擇
  //   true：暗
  //   false：正常亮度

    BMD31.dim(FALSE);  // 回復正常亮度
}

void setdisplayInverse()  //設定螢幕反白模式
{
  //  BMD31.invertDisplay(parameter)：是否反白
  // true：白底黑字 (black-on-white)
  // false：黑底白字 (white-on-black)
  BMD31.invertDisplay(TRUE); // invert Display Mode:black-on-white
}

void setdisplayNormal()  //設定螢幕正常模式
{
  //  BMD31.invertDisplay(parameter)：是否反白
  // true：白底黑字 (black-on-white)
  // false：黑底白字 (white-on-black)  
  BMD31.invertDisplay(FALSE); // invert Display Mode:black-on-white
}

//--------------------

