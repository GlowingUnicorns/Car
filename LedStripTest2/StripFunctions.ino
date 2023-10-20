void strip1(int counter){
  if(future[0] > 125){
    if(counter < 100){
      leds[0][0] = CRGB::Yellow;
      leds[0][1] = CRGB::Black;
      leds[0][2] = CRGB::Black;
      leds[0][3] = CRGB::Yellow;

    }
    else if(counter < 200){
      leds[0][0] = CRGB::Black;
      leds[0][1] = CRGB::Yellow;
      leds[0][2] = CRGB::Black;
      leds[0][3] = CRGB::Black;
    }
    else{
      leds[0][0] = CRGB::Black;
      leds[0][1] = CRGB::Black;
      leds[0][2] = CRGB::Yellow;
      leds[0][3] = CRGB::Black;
    }
  }
  else for(int i =0;i<4;i++){leds[0][i] = CRGB::Black;};
}
void strip2(){
  uint8_t x = 1;
  for(int i = 0; i < nums[x];i++){
    if(i < map(vals[x],0,255,0,nums[x])){
      if( i < 6) leds[x][i] = CRGB::Yellow;
      else if( i < 9) leds[x][i] = CHSV( 15,255,255);
      else leds[x][i] = CRGB::Red;

    }

    else leds[x][i] = CRGB::Black;
    
  }
}
void strip3(){
  uint8_t x = 2;
  for(int i = 0; i < nums[x];i++){
    if(i < map(vals[x],0,255,0,nums[x])){
      if( i < 2) leds[x][i] = CRGB::Green;
      else if( i < 6) leds[x][i] = CHSV( 15,255,255);
      else leds[x][i] = CRGB::Red;

    }

    else leds[x][i] = CRGB::Black;
    
    if(i>3){if(3 < map(vals[x],0,255,0,nums[x])){
      leds[x][3] = CHSV( 15,255,255);}
      else{
        leds[x][3]=CRGB::Black;
      }
    }
  }
}
void strip4(){
  uint8_t x = 3;
  for(int i = 0; i < nums[x];i++){
    if(i < map(vals[x],0,255,0,nums[x])){
      if( i < 3) leds[x][i] = CRGB::Green;
      else if( i < 6) leds[x][i] = CRGB::Blue;
      else if( i < 9) leds[x][i] = CRGB::Purple;
      else leds[x][i] = CRGB::Red;

    }

    else leds[x][i] = CRGB::Black;
    

  }
}
void strip5(){
  uint8_t x = 4;
//  vals[x] = 200;
  for(int i = 0; i < nums[x];i++){
    if(i < map(vals[x],0,255,0,nums[x])){
      leds[x][i] = CRGB::Red;
    }

    else leds[x][i] = CRGB::Black;
    
    if(i>3){if(3 < map(vals[x],0,255,0,nums[x])){
      leds[x][3] = CRGB::Red;}
      else{
        leds[x][3]=CRGB::Black;
      }
    }
  }
}
