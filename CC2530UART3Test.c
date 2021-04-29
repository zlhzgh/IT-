#include <iocc2530.h>
#include <string.h>

#define uint unsigned int
#define uchar unsigned char

//������ƵƵĶ˿�
#define LED1   P1_0
#define LED2   P1_1
#define LED3   P0_4

void initUART0(void);

uchar Recdata[3]="000";
uchar RXTXflag = 1;
uchar temp;
uint  datanumber = 0;
uint  stringlen;

/****************************************************************
���ڣ�UART0)��ʼ��������				
****************************************************************/
void initUART0(void)
{
    CLKCONCMD &= ~0x40;                           //����ϵͳʱ��ԴΪ32MHZ����
    while(CLKCONSTA & 0x40);                      //�ȴ������ȶ�
    CLKCONCMD &= ~0x47;                           //����ϵͳ��ʱ��Ƶ��Ϊ32MHZ
    
    PERCFG = 0x00;			          //λ��1 P0��
    P0SEL = 0x0c;				  //P0��������
    P2DIR &= ~0XC0;                               //P0������ΪUART0    
    
    U0CSR |= 0x80;				  //��������ΪUART��ʽ
    U0GCR |= 8;				
    U0BAUD |= 59;				  //��������Ϊ9600
    UTX0IF = 1;                                   //UART0 TX�жϱ�־��ʼ��λ1    
    
    U0CSR |= 0X40;				  //�������
    IEN0 |= 0x84;				 //�����жϣ������ж�
}
/*****************************************
��ʼ��LED IO��
*****************************************/
void Init_LED_IO(void)
{
  	P1DIR = 0x03; 	         //P10 P11 Ϊ���
        P0DIR = 0x10;            //P04Ϊ���
	LED1 = 1;
	LED3 = 1;		
        LED2 = 1;		//��LED
}
/****************************************************************
������				
****************************************************************/
void main(void)
{	
        uchar i;
        Init_LED_IO();
	initUART0();
	while(1)
	{
          if(RXTXflag == 1)			 //����״̬
          {
            if( temp != 0)
            {
                if((temp!='#')&&(datanumber<3))
                {                                //������������Ϊ�����ַ�
                                                 //����ܽ���3���ַ�
                  Recdata[datanumber++] = temp;
                }
                else
                {
                  RXTXflag = 3;                   //����ı�С�Ƶĳ���
                }
                if(datanumber == 3)RXTXflag = 3;
              temp  = 0;
            }
          }
          if(RXTXflag == 3)
          {
            if(Recdata[0]=='R')
            {
              if(Recdata[1]=='0')LED1 = 1;      // R0# ��D1
              else
                LED1 = 0;                       // R1# ��D1
            }
             if(Recdata[0]=='Y')
            {
              if(Recdata[1]=='0')LED3 = 1;      // Y0# ��D3
              else
                LED3 = 0;                       // Y1# ��D3
            }
            if(Recdata[0]=='G')
            {
              if(Recdata[1]=='0')LED2 = 1;      // G0# ��D2
              else
                LED2 = 0;                       // G1# ��D2
            }
            if(Recdata[0]=='A')
            {
              if(Recdata[1]=='0')
              {  
                 LED1 = 1;  
                 LED3 = 1;  
                 LED2 = 1;                        // A0# ������LED
              }
              else
              {  
                  LED1 = 0;  
                  LED3 = 0;
                  LED2 = 0;                       // A1# ������LED
              }
            }
            RXTXflag = 1;
            for(i=0;i<3;i++)Recdata[i]=' '; //����ղŵ�����
            datanumber = 0;                     //ָ���0
          }
        }//while
}
/****************************************************************
���ڽ���һ���ַ�:һ�������ݴӴ��ڴ���CC2530,������жϣ������յ������ݸ�ֵ������temp.		
****************************************************************/
#pragma vector = URX0_VECTOR
 __interrupt void UART0_ISR(void)
 {
 	URX0IF = 0;				//���жϱ�־
	temp = U0DBUF;
 }