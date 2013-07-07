//For Arduino Start kit
//Compatible with all Arduino version
//Last updated 2011-1-13
//www.dfrobot.com
//IR Control  Lesson

#define BUZZER 10//Connect Buzzer to Digital Pin 10
#define LED_RED 11//Connect a Red LED to Digital Pin 11
#define IR_IN  8  //Connect the Infrared receiver to Digital Pin 8

int Pulse_Width=0;//Pulse width
int  ir_code=0x00;//IR command code

void timer1_init(void)//Initilize timer
{
  TCCR1A = 0X00; 
  TCCR1B = 0X05; 
  TCCR1C = 0X00;
  TCNT1 = 0X00;
  TIMSK1 = 0X00;	
}
void remote_deal(void)//Get IR command
{ 
  switch(ir_code)
  {
  case 0xff00://Press stop button on the remote controller
    digitalWrite(LED_RED,LOW);//Turn off red led
    digitalWrite(BUZZER,LOW);//Silence the buzzer
    break;
  case 0xfe01://Press VOL+ button
    digitalWrite(LED_RED,HIGH);//Turn off Red LED
    break;
  case 0xf609://Press VOL- button
    digitalWrite(BUZZER,HIGH);//Turn on Buzzer
    break;
  }
}
char logic_value()//判断逻辑值“0”和“1”子函数
{
  while(!(digitalRead(8))); //低等待
  Pulse_Width=TCNT1;
  TCNT1=0;
  if(Pulse_Width>=7&&Pulse_Width<=10)//低电平560us
  {
    while(digitalRead(8));//是高就等待
    Pulse_Width=TCNT1;
    TCNT1=0;
    if(Pulse_Width>=7&&Pulse_Width<=10)//接着高电平560us
      return 0;
    else if(Pulse_Width>=25&&Pulse_Width<=27) //接着高电平1.7ms
      return 1;
  }
  return -1;
}
void pulse_deal()//接收地址码和命令码脉冲函数
{
  int i;
  //执行8个0
  for(i=0; i<8; i++)
  {
    if(logic_value() != 0) //不是0
        return;
  }
  //执行6个1
  for(i=0; i<6; i++)
  {
    if(logic_value()!= 1) //不是1
        return;
  }
  //执行1个0
  if(logic_value()!= 0) //不是0
      return;
  //执行1个1
  if(logic_value()!= 1) //不是1
      return;


  //解析遥控器编码中的command指令
  ir_code=0x00;//清零
  for(i=0; i<16;i++ )
  {
    if(logic_value() == 1)
    {
      ir_code |=(1<<i);
    }
  }
}
void remote_decode(void)//译码函数
{
  TCNT1=0X00;       
  while(digitalRead(8))//是高就等待
  {
    if(TCNT1>=1563)  //当高电平持续时间超过100ms，表明此时没有按键按下
    {
      ir_code = 0xff00;
      return;
    }  
  }

  //如果高电平持续时间不超过100ms
  TCNT1=0X00;

  while(!(digitalRead(8))); //低等待
  Pulse_Width=TCNT1;
  TCNT1=0;
  if(Pulse_Width>=140&&Pulse_Width<=141)//9ms
  {

    while(digitalRead(8));//是高就等待
    Pulse_Width=TCNT1;
    TCNT1=0;
    if(Pulse_Width>=68&&Pulse_Width<=72)//4.5ms
    {
      pulse_deal();
      return;
    }
    else if(Pulse_Width>=34&&Pulse_Width<=36)//2.25ms
    {
      while(!(digitalRead(8)));//低等待
      Pulse_Width=TCNT1;
      TCNT1=0;
      if(Pulse_Width>=7&&Pulse_Width<=10)//560us
      {
        return; 
      }
    }
  }
}
void setup()
{
  unsigned char i;
  pinMode(LED_RED,OUTPUT);//设置与红灯连接的引脚为输出模式
  pinMode(BUZZER,OUTPUT);//设置与蜂鸣器连接的引脚为输出模式
  pinMode(IR_IN,INPUT);//设置红外接收引脚为输入
}
void loop()
{  
  timer1_init();//定时器初始化
  while(1)
  {
    remote_decode();  //译码
    remote_deal();   //执行译码结果
  }  
}
