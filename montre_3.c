// Connections de LCD
sbit LCD_D4 at RB0_bit;
sbit LCD_D5 at RB1_bit;
sbit LCD_D6 at RB2_bit;
sbit LCD_D7 at RB3_bit;
sbit LCD_RS at RB4_bit;
sbit LCD_EN at RB5_bit;
sbit LCD_D4_Direction at TRISB0_bit;
sbit LCD_D5_Direction at TRISB1_bit;
sbit LCD_D6_Direction at TRISB2_bit;
sbit LCD_D7_Direction at TRISB3_bit;
sbit LCD_RS_Direction at TRISB4_bit;
sbit LCD_EN_Direction at TRISB5_bit;
// Fin de connections

volatile unsigned int seconds = 0;

unsigned int timer1_overflows, minutes = 0, hours = 0, digit1, digit2, old_digit1, old_digit2, col;

void init_timer1(){
/*
  1s avec TMR1
  quartz: 12MHz
  0.1s = (1/12*10^6)*4*8*37500 et 0.1s*10=1s
  Prescaler = 8
  TMR1 = 37500
  TMR1 = 256TMRH+TMRL, avec TMRH = 255-TMRH_init = 146
                         et TMRL = 256-TMRL_init = 124
  donc TMRH_init = 109
       TMRL_init = 132
*/
 T1CON = 0x31;
 TMR1H = 109;
 TMR1L = 132;
 TMR1IF_bit = 0;
 TMR1IE_bit = 1;
 PEIE_bit = 1;
 GIE_bit = 1;
}

void interrupt(){
 if(TMR1IF_bit){
   timer1_overflows++;

   if(timer1_overflows == 10){  //10*0.1s = 1s
     seconds++;
     if(seconds >= 60){
       seconds = 0;
       minutes++;
       if(minutes >= 60){
          minutes = 0;
          hours++;
          if(hours >= 24) hours = 0;
       }
     }

     timer1_overflows = 0;
    }

   TMR1H = 109;
   TMR1L = 132;
   TMR1IF_bit = 0;
 }
}

void initialization_pic(){
  Lcd_Init();
  delay_ms(200);
  Lcd_Cmd(_LCD_CLEAR);
  Lcd_Cmd(_LCD_CURSOR_OFF);
  Lcd_Out(1, 7, "Time");
  Lcd_Chr(2, 7, ':');
  Lcd_Chr(2, 10, ':');
  init_timer1();
}

void display(unsigned int t, unsigned int col){
     digit1 = t/10 + '0';
     digit2 = t % 10 + '0';
     Lcd_Chr(2, col, digit1);
     Lcd_Chr(2, col + 1, digit2);
}

void main() {
  initialization_pic();
  
  hours = 12;
  minutes = 59;
  seconds = 20;
  
   while(1){
      display(hours, 5);
      display(minutes, 8);
      display(seconds, 11);
   }
}
