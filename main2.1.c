//LAB4 Task2 Part1
//Chase Austin Deitner
//ECE 474 SP2020

//Replecates a traffic light with a pedestrian cross signal
//      uses external LCD touch screen. User must hold buttons
//      for at least 2 seconds for the system to register
//Initializes LCD Screen to support all traffic light funtions

#include <stdint.h> 
#include "SSD2119_Display.h"
#include "SSD2119_Touch.h"
#include "Lab_3_Driver.h"
#include "LAB4.h"

//traffic light states
enum TL_States {TL_Start, TL_Off, TL_TurnOn, TL_Go,     
                TL_Warn, TL_Stop, TL_TurnOff} TL_State; 

//User buttons Pressed
unsigned char pedX_button;
unsigned char SS_button;

//interupt flags
volatile unsigned int flag0; //interrupt flag 0
volatile unsigned int flag1; //interrupt flag 1
volatile unsigned int flag2; //interrupt flag 2


void init_timer0(void);
void timer(void);
void init_timerSS(void);
void init_timerPedX(void);
void green_on(void);
void yellow_on(void);
void red_on(void);
void LEDS_off(void);
void Timer0_ISR (void);
void Timer1_ISR (void);
void Timer2_ISR (void); 
unsigned char StartStop(void);
unsigned char PedX(void);
void Traffic_Light(void);
void updateTouch(void);
void init_TrafficLightLCD(void);


//Initialize then run FSM forever
int main()
{
  init_timer0();                //5 second timer
  init_timerSS();               //2 second timer (startstop)
  init_timerPedX();             //2 second timer (PedX) 
  NVIC_EN0_R = 0xA80000;        //enable timer interupts
  LCD_Init();                   //LCD Sreen
  Touch_Init();                 //Touch screen
  init_TrafficLightLCD();       //Set up user interface
  
  while(1) {
    Traffic_Light();
    updateTouch();
  }
  return 0;
}

//initializes screen with buttons and lights
void init_TrafficLightLCD(void)
{
  //create buttons
  LCD_ColorFill(Color4[0]);  
  LCD_DrawRect(0, 120, 159, 119, Color4[15]);
  LCD_DrawRect(160, 120, 159, 119, Color4[15]);
  char buffer[50];
  LCD_SetCursor(50,176);      
  sprintf(buffer, "PedX Cross");
  LCD_PrintString(buffer); 
  LCD_SetCursor(210,176);
  sprintf(buffer, "Start-Stop");
  LCD_PrintString(buffer); 
  
  //Create lights
  LCD_DrawCircle(100, 60, 20, Color4[12]);      //red light
  LCD_DrawCircle(160, 60, 20, Color4[14]);      //yellow light
  LCD_DrawCircle(220, 60, 20, Color4[10]);      //green light
}

//run the FSM
void Traffic_Light()
{
  //State Transitions
  switch(TL_State) {
    case TL_Start:              
      TL_State = TL_Off;        
      break;
      
    case TL_Off:
      if(StartStop()) {
        TL_State = TL_TurnOn;
      } else {
        TL_State = TL_Off;
      }
      break;
      
    case TL_TurnOn:
      if(SS_button) {                           //must release ss button
        TL_State = TL_TurnOn;
      } else {
        flag0 = 0;
        TL_State = TL_Stop;
        timer();                                //reset/start timer
      }
      break;
      
    case TL_Go:
      if(StartStop()) {
        TL_State = TL_TurnOff;
      } else if (PedX()){
        TL_State = TL_Warn;
        timer();                                //reset/start timer
      } else if (flag0) {                       //if timer flag is thrown
        flag0 = 0;
        TL_State = TL_Stop;
        timer();                                //reset/start timer        
      }
      break;
      
    case TL_Warn:
      if(StartStop()) {
        TL_State = TL_TurnOff;
      } else if (flag0) {                       //if timer flag is thrown
        flag0 = 0;
        TL_State = TL_Stop;
        timer();                                //reset/start timer        
      }
      break;
      
    case TL_Stop:                               
      if(StartStop()) {
        TL_State = TL_TurnOff;
      } else if (flag0) {                       //if timer flag is thrown
        flag0 = 0;
        TL_State = TL_Go;
        timer();                                //reset/start timer        
      }
      break;
      
    case TL_TurnOff:
      if(SS_button) {                           //must release ss button
        TL_State = TL_TurnOff;
      } else {
        TL_State = TL_Off;
      }
      break;
      
    default:
      TL_State = TL_Start;
      break;
  }
  
  //State Actions
  switch(TL_State) {
    case TL_Start:
      LEDS_off();
      break;
    case TL_Off:
      LEDS_off();
      break;
    case TL_TurnOn:
      LEDS_off();
      break;
    case TL_Go:
      green_on();
      break;
    case TL_Warn:
      yellow_on();
      break;
    case TL_Stop:
      red_on();
      break;
    case TL_TurnOff:
      LEDS_off();
      break;
    default:
      LEDS_off();
      break;
  }
}



//initialize 5 second timer
void init_timer0(void)
{
  volatile unsigned short delay = 0;
  SYSCTL_RCGCTIMER_R |= 0x1;    //Enable GPTimer module 0
  delay++;
  delay++;
  
  TIMER0_CTL_R = 0x0;           //Disable Timer while initializing
  TIMER0_CFG_R = 0x0;           //Configure to 32-Bit timer
  TIMER0_TAMR_R |= 0x2;         //set to periodic mode
  TIMER0_TAMR_R &= ~0x10;       //set to cound down
  TIMER0_TAILR_R = 0x4C4B400;     //count down from 80 million (5sec)
  TIMER0_IMR_R = 0x1;           //configure the GPTMIMR register for interupts
  TIMER0_CTL_R &= ~0x1;           //disable Timer  
}

//reset the 5 second timer
void timer(void)
{
  TIMER0_CTL_R |= 0x1;           //enable Timer  
  TIMER0_TAILR_R = 0x4C4B400;     //set timer to 80 million (5sec) 
}

//initialize start stop 2 second timer
void init_timerSS(void)
{
  volatile unsigned short delay = 0;
  SYSCTL_RCGCTIMER_R |= 0x2;    //Enable GPTimer module 0
  delay++;
  delay++;
  
  TIMER1_CTL_R = 0x0;           //Disable Timer while initializing
  TIMER1_CFG_R = 0x0;           //Configure to 32-Bit timer
  TIMER1_TAMR_R |= 0x2;         //set to periodic mode
  TIMER1_TAMR_R &= ~0x10;       //set to cound down
  TIMER1_TAILR_R = 0x1E84800;     //count down from 32 million (2sec)
  TIMER1_IMR_R = 0x1;           //configure the GPTMIMR register for interupts
  TIMER1_CTL_R &= ~0x1;           //disable Timer  
}

void init_timerPedX(void)
{
  volatile unsigned short delay = 0;
  SYSCTL_RCGCTIMER_R |= 0x4;    //Enable GPTimer module 0
  delay++;
  delay++;
  
  TIMER2_CTL_R = 0x0;           //Disable Timer while initializing
  TIMER2_CFG_R = 0x0;           //Configure to 32-Bit timer
  TIMER2_TAMR_R |= 0x2;         //set to periodic mode
  TIMER2_TAMR_R &= ~0x10;       //set to cound down
  TIMER2_TAILR_R = 0x1E84800;     //count down from 32 million (2sec)
  TIMER2_IMR_R = 0x1;           //configure the GPTMIMR register for interupts
  TIMER2_CTL_R &= ~0x1;           //disable Timer  
}


// turn on only the green light
void green_on(void) {
  LCD_DrawFilledCircle(100, 60, 19, 0x000000);   //red light off
  LCD_DrawFilledCircle(160, 60, 19, 0x000000);  //yellow light off
  LCD_DrawFilledCircle(220, 60, 19, Color4[10]);  //green light on
} 
 
// turn on only the yellow light
void yellow_on(void) {
  LCD_DrawFilledCircle(220, 60, 19, 0x000000);  //green light off
  LCD_DrawFilledCircle(100, 60, 19, 0x000000);   //red light off
  LCD_DrawFilledCircle(160, 60, 19, Color4[14]);  //yellow light on
} 

// turn on only the red light
void red_on(void) {
  LCD_DrawFilledCircle(220, 60, 19, 0x000000);  //green light off
  LCD_DrawFilledCircle(160, 60, 19, 0x000000);  //yellow light off  
  LCD_DrawFilledCircle(100, 60, 19, Color4[12]);   //red light on
} 

// turn off all lights
void LEDS_off(void) {
  LCD_DrawFilledCircle(100, 60, 19, 0x000000);   //red light off
  LCD_DrawFilledCircle(160, 60, 19, 0x000000);  //yellow light off
  LCD_DrawFilledCircle(220, 60, 19, 0x000000);  //green light off
} 


void Timer0_ISR (void) 
{
  flag0 = 1;
  TIMER0_ICR_R |= 0x1;
}

void Timer1_ISR (void) 
{
  flag1 = 1;
  TIMER1_ICR_R |= 0x1;
}
void Timer2_ISR (void) 
{
  flag2 = 1;
  TIMER2_ICR_R |= 0x1;
}


//Start Stop button check
unsigned char StartStop(void)
{ 
  if (SS_button) {      //if button is pressed
    TIMER1_CTL_R |= 0x1;                //turn timer on
  } else {
    TIMER1_CTL_R &= ~0x1;               //turn timer off
    TIMER1_TAILR_R = 0x1E84800;         //reset timer
  }

  if (SS_button && flag1) { //if button is still pressed after 2 sec
    flag1 = 0;                          //reset flag
    TIMER1_CTL_R &= ~0x1;               //turn timer off
    TIMER1_TAILR_R = 0x1E84800;         //reset timer
    return 1;
  } else {
    return 0;
  }

}


//Pedestrian Crossing button check
unsigned char PedX(void)
{
  if (pedX_button) {      //if button is pressed
    TIMER2_CTL_R |= 0x1;                //turn timer on
  } else {
    TIMER2_CTL_R &= ~0x1;               //turn timer off
    TIMER2_TAILR_R = 0x1E84800;         //reset timer
  }

  if (pedX_button && flag2) { //if button is still pressed after 2 sec
    flag2 = 0;                          //reset flag
    TIMER2_CTL_R &= ~0x1;               //turn timer off
    TIMER2_TAILR_R = 0x1E84800;         //reset timer
    return 1;
  } else {
    return 0;
  }
   //return (GPIO_PORTE_DATA_R & 0x2); // 0x2 (pressed) or 0 (not pressed)
}

void updateTouch (void) 
{
  unsigned long touch_X;
  unsigned long touch_Y;
  touch_X = Touch_ReadX();
  touch_Y = Touch_ReadY();  
  
  //if(pedX_button)
  if (touch_X < 1500 && touch_X > 100 && touch_Y > 800) {   
    pedX_button = 0x1;
    LCD_DrawRect(5, 125, 149, 109, Color4[15]);        //show button is pressed
  } else {
    pedX_button = 0x0;
    LCD_DrawRect(5, 125, 149, 109, Color4[0]);
  }
  
  //if(SS_button)
  if (touch_X > 1500 && touch_Y < 1200) {                     
    SS_button = 0x1;
    LCD_DrawRect(165, 125, 149, 109, Color4[15]);      //show button is pressed
  } else {
    SS_button = 0x0;
    LCD_DrawRect(165, 125, 149, 109, Color4[0]);
  }
  
}


    
 