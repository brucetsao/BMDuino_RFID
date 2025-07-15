/*****************************************************************
  檔案名稱：display.ino
  功能說明：
  1. 使用 I2C 通訊介面與 BMD31M090 OLED 顯示模組通訊（頻率：400KHz）
  2. 使用硬體序列埠 Serial（115200 bps）與電腦序列監控器通訊
  硬體接腳說明：
    SCL 接腳：D19
    SDA 接腳：D18
******************************************************************/
#include "BMD31M090.h"  // OLED 顯示模組函式庫
#include "BestModuleLogo.h"     // 含 LOGO 圖示的點陣圖資料

// 定義顯示模組參數
#define BMD31M090_WIDTH   128     // OLED 寬度為 128 像素
#define BMD31M090_HEIGHT  64      // OLED 高度為 64 像素
#define BMD31M090_ADDRESS 0x3C    // 預設的 I2C 位址，可為 0x3C 或 0x3D（視模組設定）

uint8_t t = ' ';  // 用來顯示 ASCII 字元與編碼的變數

// 建立 OLED 物件（此例使用 Wire1，也可用 Wire / Wire2）
BMD31M090 BMD31(BMD31M090_WIDTH, BMD31M090_HEIGHT, &Wire1);

void setup()
{
  Serial.begin(9600);  // 啟動序列埠監控
  Serial.println("BMD31M090 0.96\" OLED Module Sketch");

  BMD31.begin(BMD31M090_ADDRESS);  // 初始化 OLED 顯示模組
  delay(100);  // 建議初始化延遲時間

  // 以下為測試各種顯示功能的函式呼叫
  test_drawString_6x8();             // 顯示 6x8 字型字串
  test_drawString_8x16();            // 顯示 8x16 字型字串
  test_drawString_drawChar_drawNum();// 同時顯示字串、單字元與數字
  test_drawPixel();                  // 像素繪製測試（白→黑→反轉）
  test_drawFastHLine_drawFastVLine();// 繪製水平/垂直線條
  test_drawBitmap();                 // 顯示 LOGO 點陣圖
  test_variousScroll();              // 滾動顯示效果
  test_invertDisplay();              // 顯示反白與恢復
  test_dim();                        // 顯示亮度切換（省電模式）
}

void loop()
{
  // 主程式不重複執行，測試功能已於 setup() 完成
}


void test_drawString_6x8(void)  //顯示字串（6x8）
{
  BMD31.clearDisplay();  // 清除畫面
  BMD31.display();       // 更新顯示

  BMD31.setFont(FontTable_6X8);  // 設定字型為 6x8
  uint8_t col = (128 - (6 * sizeof("Hello World!"))) / 2;  // 計算置中位置

  for (uint8_t row = 0; row < 8; row++)
    BMD31.drawString(col, row, (u8*)"Hello World!");

  delay(500);
}

void test_drawString_8x16(void) //顯示字串（8x16）
{
  BMD31.clearDisplay();
  BMD31.display();

  BMD31.setFont(FontTable_8X16);  // 設定字型為 8x16
  uint8_t col = (128 - (8 * sizeof("Hello World!"))) / 2;

  for (uint8_t row = 0; row < 8; row += 2)  // 每 2 行顯示一次
    BMD31.drawString(col, row, (u8*)"Hello World!");

  delay(500);
}


void test_drawString_drawChar_drawNum(void) //顯示字串 / 字元 / 數字
{
  BMD31.clearDisplay();
  BMD31.display();

  BMD31.drawString(16, displayROW0, (u8*)"Best Modules");
  BMD31.drawString(8, displayROW2, (u8*)"BMD31M090 TEST");
  /* Sequentially display ASCII ' ' (0x20/32) to ' ~' (0x7E/126) values for Font 6x8 & 8x16 */
  do
  {
    BMD31.setFont(FontTable_6X8);
    BMD31.drawString(0, displayROW5, (u8*)"ASCII:");
    BMD31.drawString(63, displayROW5, (u8*)"CODE:");
    BMD31.drawChar(48, displayROW5, t);
    BMD31.drawNum(103, displayROW5, t, 3);

    BMD31.setFont(FontTable_8X16);
    BMD31.drawString(0, displayROW6, (u8*)"ASCII:");
    BMD31.drawString(63, displayROW6, (u8*)"CODE:");
    BMD31.drawChar(48, displayROW6, t);
    BMD31.drawNum(103, displayROW6, t, 3);
    delay(10);
  } while (++t < '~');
  t = ' ';
}

void test_drawPixel(void) // 像素顯示測試
{
  BMD31.clearDisplay();
  for (uint8_t col = 0; col < 128; col++)
    for (uint8_t row = 0; row < 64; row++)
      BMD31.drawPixel(col, row, pixelColor_WHITE);
  BMD31.display();
  delay(500);

  for (uint8_t col = 0; col < 128; col++)
    for (uint8_t row = 0; row < 32; row++)
      BMD31.drawPixel(col, row, pixelColor_BLACK);
  BMD31.display();
  delay(500);

  for (uint8_t col = 0; col < 128; col++)
    for (uint8_t row = 0; row < 64; row++)
      BMD31.drawPixel(col, row, pixelColor_INVERSE);
  BMD31.display();
  delay(500);
}

void test_drawFastHLine_drawFastVLine(void) //水平與垂直線測試
{
  int8_t col = 112;
  BMD31.clearDisplay();

  for (int8_t row = 0; row < 64; row += 8) {
    BMD31.drawFastHLine(0, row, col, pixelColor_WHITE);
    BMD31.drawFastVLine(col, row, 64 - row, pixelColor_WHITE);
    col -= 14;
  }

  BMD31.display();
  delay(500);

  BMD31.clearDisplay();
  col = 112;

  for (int8_t row = 56; row >= 0; row -= 8) {
    BMD31.drawFastHLine(col, row, 128 - col, pixelColor_WHITE);
    BMD31.drawFastVLine(col, row, 64 - row, pixelColor_WHITE);
    col -= 14;
  }

  BMD31.display();
  delay(500);
}


void test_drawBitmap(void)
{
  /*==========================================================
    |Using Image2LCD Software conversion, setting as bellow    |
    |1. Output file type: C array (*.c)                        |
    |2. Scan mode: Horizon Scan                                |
    |3. Bits Pixel: monochrome                                 |
    |4. Max Width and Height: 128 (Width) / 64 (Height)        |
    |5. Select "MSB First"                                     |
    ----------------------------------------------------------
    |Using LCD Assistant Software conversion, setting as bellow|
    |1. Byte orientation: Horizontal                           |
    |2. Size: According your image size                        |
    |3. Size endianness: Little                                |
    |4. Pixels/byte: 8                                         |
    ==========================================================*/

  // 顯示 BestModule_LOGO
  BMD31.clearDisplay();
  BMD31.drawBitmap(0, 0, BestModule_LOGO, 128, 64, pixelColor_WHITE);
  BMD31.display();
  delay(300);
  BMD31.drawBitmap(0, 0, BestModule_LOGO, 128, 64, pixelColor_BLACK);
  BMD31.display();
  delay(300);
  BMD31.drawBitmap(0, 0, BestModule_LOGO, 128, 64, pixelColor_INVERSE);
  BMD31.display();
  delay(300);

  BMD31.clearDisplay();
  BMD31.drawBitmap(0, 0, BestModule_LOGOandName, 128, 64, pixelColor_WHITE);
  BMD31.display();
  delay(300);
  BMD31.drawBitmap(0, 0, BestModule_LOGOandName, 128, 64, pixelColor_BLACK);
  BMD31.display();
  delay(300);
  BMD31.drawBitmap(0, 0, BestModule_LOGOandName, 128, 64, pixelColor_INVERSE);
  BMD31.display();
  delay(300);
}


void test_variousScroll(void) //滾動顯示測試
{
  /* Scroll Function Test */
  uint8_t startrow = displayROW0;
  uint8_t endrow = displayROW7;

  BMD31.startScrollRight(startrow, endrow, SCROLL_2FRAMES);
  delay(500);
  BMD31.startScrollRight(startrow, endrow, SCROLL_2FRAMES, SCROLLV_TOP);
  delay(500);
  BMD31.startScrollRight(startrow, endrow, SCROLL_2FRAMES, SCROLLV_BOTTOM);
  delay(500);

  BMD31.startScrollLeft(startrow, endrow, SCROLL_2FRAMES);
  delay(500);
  BMD31.startScrollLeft(startrow, endrow, SCROLL_2FRAMES, SCROLLV_TOP);
  delay(500);
  BMD31.startScrollLeft(startrow, endrow, SCROLL_2FRAMES, SCROLLV_BOTTOM);
  delay(500);

  BMD31.stopScroll();
}

void test_invertDisplay(void) // 顯示反白與亮度切換
{
  BMD31.invertDisplay(TRUE); // invert Display Mode:black-on-white
  delay(500);
  BMD31.invertDisplay(FALSE); // normal Display Mode:white-on-black
  delay(500);
  BMD31.invertDisplay(TRUE); // invert Display Mode:black-on-white
  delay(500);
  BMD31.invertDisplay(FALSE); // normal Display Mode:white-on-black
  delay(500);
}

void test_dim(void)
{
  BMD31.dim(TRUE);   // 降低亮度（省電模式）
  delay(500);
  BMD31.dim(FALSE);  // 回復正常亮度
  delay(500);
  BMD31.dim(TRUE);
  delay(500);
  BMD31.dim(FALSE);
  delay(500);
}
