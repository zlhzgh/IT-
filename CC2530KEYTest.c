#include <ioCC2530.h>
#define uint  unsigned int
#define uchar unsigned char
//������ƵƵĶ˿�
#define LED1   P1_0	  //����LED1ΪP10�ڿ���
#define LED2   P1_1	  //����LED2ΪP11�ڿ���
#define LED3   P0_4	  //����LED3ΪP04�ڿ���
#define KEY1   P0_1       //���尴��ΪP01�ڿ���
//��������
void Delay(uint);		//��ʱ��������
void InitIO(void);		//��ʼ����������
void InitKey(void);             //��ʼ��������������
uchar KeyScan(void);            //����ɨ�躯������

uchar Keyvalue = 0 ;           //���������¼��������
uint  KeyTouchtimes = 0 ;           //���������¼��������
/****************************
//��ʱ
*****************************/
void Delay(uint n)
{
	uint i;
	for(i = 0;i<n;i++);
        for(i = 0;i<n;i++);
        for(i = 0;i<n;i++);
        for(i = 0;i<n;i++);
        for(i = 0;i<n;i++);
}
/****************************
//������ʼ��
*****************************/
void InitKey(void)
{
  P0SEL &= ~0X02;  
  P0DIR &= ~0X02;  //������P01�ڣ�����Ϊ����ģʽ
  P0INP |= 0x02;   //����      
  
}
/****************************
//��ʼ������,��P10��P11��P04����Ϊ����ڣ�����LED�Ƴ�ʼ��Ϊ��
*****************************/
void InitIO(void)
{
    P1DIR |= 0x13; //P10��P11����Ϊ���
    P0DIR |= 0x10; //P04����Ϊ���
    LED1 = 1;
    LED2 = 1;
    LED3 = 1;	    //LED�Ƴ�ʼ��Ϊ��
}
/*****************************************
//����������¼����
*****************************************/
uchar KeyScan(void)
{
   if(KEY1 == 1)    //�ߵ�ƽ��Ч
  {
    Delay(100);     //��⵽����
    if(KEY1 == 1)
    {
      while(KEY1);   //ֱ���ɿ�����      
      return(1);
    }
  }
  return(0);
}
/***************************
//������
***************************/
void main(void)
{
  InitIO();		//��ʼ��LED�ƿ���IO��	
  InitKey();             //��ʼ����������IO��
  while(1)
  {
    Keyvalue = KeyScan();       //��ȡ��������
    
    if(Keyvalue == 1)           //���°���S1����ΪLED3,LED2,LED1������ˮ��˸
    {           
      LED3 = !LED3;           
      Delay(50000); 
      LED2 = !LED2;                
      Delay(50000);           
      LED1 = !LED1;           
      Delay(50000);       
    }  
  }
}
  

