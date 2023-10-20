
void ringMeter(Arduino_GFX *fx,uint8_t x,uint8_t y,uint8_t r1,uint8_t r2,int16_t a1,int16_t a2,uint8_t s,uint8_t g,uint8_t scheme,uint16_t val, uint16_t mini, uint16_t maxi){
  if(val > maxi) val = maxi;
  if(val < mini) val = mini;
  uint16_t angle = a1; 
  uint16_t COLOR = WHITE;
  while(angle < a2){
    if (val > mini && map(val,mini,maxi,a1,a2) >= angle)
      COLOR = findColor(angle,a1,a2,scheme);
    else COLOR = GREY;

    fx->fillArc(x,y,r1,r2,angle,angle+s,COLOR);
    angle += g + s;
  }
  fx->fillArc(120,120,120,118,0,360,WHITE);
}

void updateRing(Arduino_GFX *fx,uint8_t x,uint8_t y,uint8_t r1,uint8_t r2,int16_t a1,int16_t a2,uint8_t s,uint8_t g,uint8_t scheme,uint16_t val, uint16_t mini, uint16_t maxi,uint16_t old){
  uint16_t angle = a1; 
  uint16_t COLOR = WHITE;
  if(val == old)return;
  if(val > maxi) val = maxi;
  if(val < mini) val = mini;

  if(val > old){
    while(angle < a2){
      if (map(old,mini,maxi,a1,a2)<= angle &&angle <= map(val,mini,maxi,a1,a2)){
        COLOR = findColor(angle,a1,a2,scheme);
        fx->fillArc(x,y,r1,r2,angle,angle+s,COLOR);
      }
      angle += g + s;
    }
  }
  else{
    while(angle < a2){
      if (map(old,mini,maxi,a1,a2)>= angle &&angle >= map(val,mini,maxi,a1,a2)){
        fx->fillArc(x,y,r1,r2,angle,angle+s,GREY);
      }
      angle += g + s;
    }
  }
}


void numMeter(Arduino_GFX *fx, uint8_t x, uint8_t y, uint16_t val, uint16_t min, uint16_t max, uint8_t scheme, uint16_t unit,int size,int old){
  String str = String(val) + char(unit); 
  uint16_t len = size*6*str.length();
  uint16_t hei = size*8;
  uint16_t len1 = String(val).length();
  uint16_t len2 = String(old).length();
  
  if(len2 > len1){
    if(len1 == 1){str =" "+  str + " ";
        fx->setCursor(x-((len1+3)*size*6)/2,y-hei/2);
    }
    else if(len1 == 2){str= " " + str + " ";
        fx->setCursor(x-((len1+3)*size*6)/2,y-hei/2);
    }
    else
      fx->setCursor(x-((len1+1)*size*6)/2,y-hei/2);
  }
  else{
    fx->setCursor(x-((len1+1)*size*6)/2,y-hei/2);
  }

  //fx->setFont(&FreeSerifBoldItalic12pt7b);
  fx->setTextSize(size);
  uint16_t COLOR = WHITE;
  COLOR = findColor(val,min,max,scheme);
  fx->setTextColor(COLOR,BLACK);
  fx->print(str);
  //248 deg;
  //37 percent

}

void labelMeter(Arduino_GFX *fx, uint8_t x, uint8_t y,uint8_t scheme,String str,uint8_t size){
  uint16_t len = size*6*str.length();
  uint16_t hei = size*8;
  fx->setCursor(x-len/2,y-hei/2);
  fx->setTextSize(size);
  fx->setTextColor(findColor(1,0,2,scheme));
  fx->print(str);
}

uint16_t findColor(uint16_t val, uint16_t min, uint16_t max, uint8_t scheme){
    uint16_t COLOR = WHITE;
    switch(scheme){
      case 1: COLOR = BLUE;
        break;
      case 2: COLOR = GREEN;
        break;
      case 3: COLOR = RED;
        break;
      case 4: COLOR = ORANGE;
        break;
      case 5:
        COLOR = rainbow(map(val, min, max, 0, 127));
        break;
      case 6:
        COLOR = rainbow(map(val, min, max, 16, 32));
        break;
      case 7:
        COLOR = rainbow(map(val, min, max, 32, 48));
        break;
      case 8:
        COLOR = rainbow(map(val, min, max, 48, 64));
        break;
      case 9:
        COLOR = rainbow(map(val, min, max, 64, 96));
        break;
      case 10:
        COLOR = rainbow(map(val, min, max, 32, 127));
        break;
  }
  return COLOR;
}

uint16_t rainbow(byte value){ // Value is expected to be in range 0-127
  if(value > 127) value = 127;
  if(value < 0) value = 0; 
  byte red = 0; //  top    5 bits
  byte green = 0;// middle 6 bits
  byte blue = 0; // bottom 5 bits
  
  byte quadrant = value / 32;
  if (quadrant == 0) {//Q1S1GREEN        ///GREEN
    if(value%32 < 16){
      blue = 0;
      green = 62;
      red =0 ;
    }else{//Q1S2GREEN2CYAN               /// CYAN
      blue = (value%32-16)*2;
      green = 62;
      red = 0; 
  }}
  if (quadrant == 1) {
    if(value%32 < 16){//Q2S1CYAN2BLUE    /// BLUE
      blue = 31;
      green = 62-value%32*4;
      red = 0;      
    }
    else{//Q2S2BLUE2PURPLE                ///PURPLE
      blue = 31;
      green = 0;
      red = (value%32-15)*2;
      if(red > 31) red = 31; 
  }}
  if (quadrant == 2) {//Q3PURPLE2RED      ///RED
    blue = 31-value%32;
    green = 0;
    red = 31;
  }if (quadrant == 3) {//Q4 RED
    blue = 0;
    green = 0;
    red = 31;
  }
  return (red << 11) + (green << 5) + blue;
}
