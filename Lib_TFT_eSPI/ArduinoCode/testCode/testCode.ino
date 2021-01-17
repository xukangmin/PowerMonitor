#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

#define UPDATE_INTERVAL 20 // ms
#define MOVE_PIXEL 50
#define GRID_START_X 40
#define GRID_START_Y 10

#define GRID_END_X 280
#define GRID_END_Y 220

uint32_t updateTime = 0;       // time for next update

float currentValue = 0;

uint16_t current_plot_x = 0;
uint16_t current_plot_y = 0;

uint8_t dir = 0;

float chart_data[300];

uint16_t current_data_pos = 0;

uint16_t current_point_pos = 0;

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

uint16_t points_on_chart_x[1000];
uint16_t points_on_chart_y[1000];



void setup_background()
{
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(1);
}


// left corner x0, y0, right bottom corner: x1,y1, number of horizental grid, number of vertical grid
void setup_grid(int x0,int y0, int x1, int y1, int hor_grid_num, int vert_grid_num)
{
  
   float hor_space = (float)(x1 - x0) / (float)hor_grid_num;
   
   float ver_space = (float)(y1 - y0) / (float)vert_grid_num;

   // first draw vertical grid lines
   for(int i = 0; i < hor_grid_num + 1; i++){

      for(int y = y0; y < y1; y += 3)
      {
        
         int x = x0  + i * hor_space;
         tft.drawPixel(x, y, TFT_GREEN);
      }
   }

   // second draw horizontal grid lines
   for(int i = 0; i < vert_grid_num + 1; i++){

      for(int x = x0; x < x1; x += 3)
      {
        
         int y = y0  + i * ver_space;
         tft.drawPixel(x, y, TFT_GREEN);
      }
   }
   
}

void setup_ticks()
{
  
}

void gen_data()
{
    if (dir == 0)
    {
      currentValue++;
    }
    else{
      currentValue--;
    }

    if (currentValue > 254) {
      dir = 1;
    }
    else if (currentValue < 0) {
      dir = 0;
    }

    chart_data[current_data_pos++] = currentValue;
    Serial.println(currentValue);
}
  

uint16_t map_y_axis(float value)
{
  return 220.0f - (value * 210.0f  / 255.0f);
}

void plot_chart()
{
   // map currentValue to y axis
   // 0 - 255 to 220 - 10
   current_plot_y = map_y_axis(currentValue);

   points_on_chart_x[current_point_pos] = current_plot_x;
   points_on_chart_y[current_point_pos] = current_plot_y;
  
   tft.drawPixel(current_plot_x, current_plot_y, TFT_WHITE);
   current_point_pos++;
   current_plot_x++;

   if (current_plot_x > GRID_END_X) {
      // move data
      // clear all current points
      for(int i = 0; i < current_point_pos; i++){
        tft.drawPixel(points_on_chart_x[i], points_on_chart_y[i], TFT_BLACK);
      }
      current_point_pos = 0;
      
      setup_grid(GRID_START_X,GRID_START_Y, GRID_END_X, GRID_END_Y, 4, 4);  

      current_plot_x -= MOVE_PIXEL;

      for(int i = 0; i < current_data_pos - MOVE_PIXEL; i++)
      {
           points_on_chart_x[current_point_pos] = i + GRID_START_X;
           points_on_chart_y[current_point_pos] = map_y_axis(chart_data[i+MOVE_PIXEL]);
          tft.drawPixel(i + GRID_START_X, map_y_axis(chart_data[i+MOVE_PIXEL]), TFT_WHITE);
          chart_data[i] = chart_data[i+MOVE_PIXEL];
          current_point_pos++;
      }
      current_data_pos -= MOVE_PIXEL;
//      current_plot_x -= MOVE_PIXEL;
//      // plot previous data
//      for(int i = 0; i < current_data_pos - MOVE_PIXEL; i++){
//        tft.drawPixel(i + 10, map_y_axis(chart_data[i+MOVE_PIXEL]), TFT_WHITE);
//        chart_data[i] = chart_data[i+MOVE_PIXEL];
//      }
//      current_data_pos -= MOVE_PIXEL;
   }
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  tft.init();
  
  setup_background();

  setup_grid(GRID_START_X,GRID_START_Y, GRID_END_X, GRID_END_Y, 4, 4);  


  current_point_pos = 0;
  
  current_plot_x = 0;
  current_plot_x = GRID_START_X;
}



void loop() {
  // put your main code here, to run repeatedly:
  if (updateTime <= millis()) {
    updateTime = millis() + UPDATE_INTERVAL;

    gen_data();
    
    plot_chart();
  }
}
