void strip1(){
  uint8_t x = 1;
  for(int i = 0; i < nums[x];i++){
    if(i < map(vals[x],0,255,0,nums[x])){
      if( i < 1) leds[x][i] = CRGB::Green;
      else if( i < 3) leds[x][i] = CRGB::Blue;
      else if( i < 5) leds[x][i] = CRGB::Purple;
      else leds[x][i] = CRGB::Red;

    }

    else leds[x][i] = CRGB::Black;
    

  }
}
void strip2(){
  uint8_t x = 0;
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
