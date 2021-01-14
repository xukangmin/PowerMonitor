#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library


void setup_background()
{
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
}

void setup_grid(int x0,int y0, int x1, int y1, int hor_grid_num, int vert_grid_num)
{
   float hor_space = (float)(x1 - x0) / (float)hor_grid_num;
   
   float ver_space = (float)(y1 - y0) / (float)vert_grid_num;

   // first draw vertical grid lines
   for(int i = 0; i < hor_grid_num; i++){

      for(int y = y0; y < y1; y += 2)
      {
        
         int x = x0  + i * hor_space;
         tft.drawPixel(x, y, TFT_GREEN);
      }
   }
  
   
}

void setup_ticks()
{
  
}

void setup() {
  // put your setup code here, to run once:
  tft.init();
  
  setup_background();

  setup_grid(10,10, 300, 220, 5, 5);  
}



void loop() {
  // put your main code here, to run repeatedly:

}
