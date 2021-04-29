#include <ioCC2530.h>
#include <string.h>

#define  uint  unsigned int
#define  uchar unsigned char

//������ƵƵĶ˿�
#define LED1  P1_0
#define LED2  P1_1
#define LED3  P0_4
//��������
void Delay(uint);
void initUARTSEND(void);
void UartTX_Send_String(char *Data,int len);

char Txdata[25]="HELLO! zigbee!\r\n";
/****************************************************************
    ��ʱ����											
****************************************************************/
void Delay(uint n)
{
	uint i;
	for(i=0;i<n;i++);
	for(i=0;i<n;i++);
	for(i=0;i<n;i++);
	for(i=0;i<n;i++);
	for(i=0;i<n;i++);
}
/****************************************************************
   ���ڳ�ʼ������				
****************************************************************/
void initUARTSEND(void)
{

    CLKCONCMD &= ~0x40;                          //����ϵͳʱ��ԴΪ32MHZ����
    while(CLKCONSTA & 0x40);                     //�ȴ������ȶ�
    CLKCONCMD &= ~0x47;                          //����ϵͳ��ʱ��Ƶ��Ϊ32MHZ
   
  
    PERCFG = 0x00;				//λ��1 P0��
    P0SEL = 0x0c;				//P0_2,P0_3��������
    P2DIR &= ~0XC0;                             //P0������ΪUART0

    U0CSR |= 0x80;				//UART��ʽ
    U0GCR |= 8;				        //U0GCR.BAUD_E
    U0BAUD |= 59;				//��������Ϊ9600 UxBAUD.BAUD_M
    UTX0IF = 0;                                 //UART0 TX�жϱ�־��ʼ��λ0
}
/****************************************************************
���ڷ����ַ�������			
****************************************************************/
void UartTX_Send_String(char *Data,int len)
{
  int j;
  for(j=0;j<len;j++)
  {
    U0DBUF = *Data++;
    while(UTX0IF == 0);
    UTX0IF = 0;
  }
}
/****************************************************************
������						
****************************************************************/
void main(void)
{	
	uchar i;
       	P1DIR = 0x03; 				//P1����LED		
        P0DIR |= 0x10;                          //P04����Ϊ���
	initUARTSEND();
        
      	LED1 = 1;                               //��LED
        LED3 = 1;                               //��LED
	LED2 = 0;	
	UartTX_Send_String(Txdata,25);	       
        for(i=0;i<30;i++)Txdata[i]=' ';
        strcpy(Txdata,"HELLO! zigbee!\r\n");       //��UART0 TX test����Txdata;
	while(1)
	{
            UartTX_Send_String(Txdata,strlen(Txdata)); //���ڷ�������
            Delay(50000);                                                       //��ʱ
            LED2=!LED2;                                           //D2����������־����״̬
            Delay(50000);
            Delay(50000);
	}
}