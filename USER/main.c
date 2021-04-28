#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "usmart.h"
#include "malloc.h"
#include "usart3.h"
#include "common.h"
#include "beep.h"
#include "dht11.h"

// APģʽ����
void ap_demo(void);

int main(void)
{
    delay_init();	    	 //��ʱ������ʼ��
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
    usmart_dev.init(72);		//��ʼ��USMART
    LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
    KEY_Init();					//��ʼ������
	BEEP_Init();				//��ʼ��������
    //LCD_Init();			   	//��ʼ��LCD
    usart3_init(115200);		//��ʼ������3
    my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
    //LCD_Clear(BLACK);
	/*
	while(DHT11_Init())			//DHT11��ʼ�� DATA -> PG11
	{
		printf("DHT11 Error\r\n");
	}
	*/

    delay_ms(1000);
	
	BEEP = 0;
	LED1 = 0;
	ap_demo();
	
    // atk_8266_test();		//����ATK_ESP8266����
}

// APģʽ����
void ap_demo(void)
{
    u8 timex = 0;
	u8 netpro = 0;	//����ģʽ 0,TCP������ 1,TCP�ͻ��� 2,UDPģʽ
	u8 key;
	u8 ipbuf[16] = "192.168.4.2"; 	//IP (��������豸����ģ�����䵽��IP��д��
	u8 *p;
	u16 t = 999;		//���ٵ�һ�λ�ȡ����״̬
	u16 rlen = 0;
	u8 constate = 0;	//����״̬
    while(atk_8266_send_cmd("AT","OK",20))//���WIFIģ���Ƿ�����
    {
        atk_8266_quit_trans();//�˳�͸��
        atk_8266_send_cmd("AT+CIPMODE=0","OK",200);  //�ر�͸��ģʽ
        printf("δ��⵽ģ��!!!\r\n");
        delay_ms(800);
        printf("��������ģ��...\r\n");
    }
    while(atk_8266_send_cmd("ATE0","OK",20));//�رջ���
    printf("ATK_ESP8266 WIFIģ�����\r\n");
    printf("WIFI AP\r\n");
    atk_8266_msg_show(0, 0, 0);

    while(1)
    {
        delay_ms(10);
        atk_8266_at_response(1);//���ATK-ESP8266ģ�鷢�͹���������,��ʱ�ϴ�������
        printf("ATK-ESP WIFI-AP ����\r\n");
        printf("��������ATK-ESP8266ģ�飬���Ե�...\r\n");

        p=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�

PRESTA:
        if(netpro&0X02)   //UDP
        {
            printf("ATK-ESP WIFI-AP ����\r\n");
            printf("��������ATK-ESPģ��,���Ե�...\r\n");

            sprintf((char*)p,"AT+CIPSTART=\"UDP\",\"%s\",%s",ipbuf,(u8*)portnum);    //����Ŀ��UDP������
            atk_8266_send_cmd("AT+CIPMUX=0","OK",100);  //������ģʽ
            while(atk_8266_send_cmd(p,"OK",500));
        }
        else     //TCP
        {
            if(netpro&0X01)     //TCP Client    ͸��ģʽ����
            {
                printf("ATK-ESP WIFI-AP ����\r\n");
                printf("��������ATK-ESPģ��,���Ե�...\r\n");
                atk_8266_send_cmd("AT+CIPMUX=0","OK",20);   //0�������ӣ�1��������
                sprintf((char*)p,"AT+CIPSTART=\"TCP\",\"%s\",%s",ipbuf,(u8*)portnum);    //����Ŀ��TCP������
                while(atk_8266_send_cmd(p,"OK",200))
                {
                    printf("WK_UP:������ѡ");
                    printf("ATK-ESP ����TCP Serverʧ��"); //����ʧ��
                    key=KEY_Scan(0);
                    if(key==WKUP_PRES)goto PRESTA;
                }
                atk_8266_send_cmd("AT+CIPMODE=1","OK",200);      //����ģʽΪ��͸��
            }
            else					//TCP Server
            {
                printf("ATK-ESP WIFI-AP ����\r\n");
                printf("��������ATK-ESPģ��,���Ե�...\r\n");
                atk_8266_send_cmd("AT+CIPMUX=1","OK",20);   //0�������ӣ�1��������
                sprintf((char*)p,"AT+CIPSERVER=1,%s",(u8*)portnum);
                atk_8266_send_cmd(p,"OK",20);     //����Serverģʽ���˿ں�Ϊ8086
            }
        }

        printf("����ATK-ESPģ��ɹ�!\r\n");
        delay_ms(200);
        printf("WK_UP:�˳�����  KEY0:��������\r\n");
        atk_8266_get_wanip(ipbuf);//������ģʽ,��ȡWAN IP
        sprintf((char*)p,"IP��ַ:%s �˿�:%s",ipbuf,(u8*)portnum);
        printf("%s\r\n",p);				//��ʾIP��ַ�Ͷ˿�
        atk_8266_wificonf_show(30,180,"�����豸����WIFI�ȵ�:",(u8*)wifiap_ssid,(u8*)wifiap_encryption,(u8*)wifiap_password);
        printf("״̬:%s\r\n",(u8*)ATK_ESP8266_WORKMODE_TBL[netpro]); 		//����״̬
        USART3_RX_STA=0;
        while(1)
        {
            key=KEY_Scan(0);
            if(key==WKUP_PRES)			//WK_UP �˳�����
            {
                atk_8266_quit_trans();	//�˳�͸��
                atk_8266_send_cmd("AT+CIPMODE=0","OK",20);   //�ر�͸��ģʽ
				myfree(SRAMIN,p);		//�ͷ��ڴ�
                return;
            }
            else if(key==KEY0_PRES)	//KEY0 ��������
            {

                if((netpro==3)||(netpro==2))   //UDP
                {
                    sprintf((char*)p,"ATK-8266%s����%02d\r\n",ATK_ESP8266_WORKMODE_TBL[netpro],t/10);//��������
                    printf("��������:%s\r\n",p);
                    atk_8266_send_cmd("AT+CIPSEND=25","OK",200);  //����ָ�����ȵ�����
                    delay_ms(200);
                    atk_8266_send_data(p,"OK",100);  //����ָ�����ȵ�����
                    timex=100;
                }
                else if((netpro==1))   //TCP Client
                {
                    atk_8266_quit_trans();
                    atk_8266_send_cmd("AT+CIPSEND","OK",20);       //��ʼ͸��
                    sprintf((char*)p,"ATK-8266%s����%02d\r\n",ATK_ESP8266_WORKMODE_TBL[netpro],t/10);//��������
                    printf("��������:%s\r\n",p);
                    u3_printf("%s",p);
                    timex=100;
                }
                else    //TCP Server
                {
                    sprintf((char*)p,"ATK-8266%s����%02d\r\n",ATK_ESP8266_WORKMODE_TBL[netpro],t/10);//��������
                    printf("��������:%s\r\n",p);
                    atk_8266_send_cmd("AT+CIPSEND=0,25","OK",200);  //����ָ�����ȵ�����
                    delay_ms(200);
                    atk_8266_send_data(p,"OK",100);  //����ָ�����ȵ�����
                    timex=100;
                }
            }

            if(timex)timex--;

            t++;
            delay_ms(5);
            if(USART3_RX_STA&0X8000)		//���յ�һ��������
            {
                rlen=USART3_RX_STA&0X7FFF;	//�õ����ν��յ������ݳ���
                USART3_RX_BUF[rlen]=0;		//��ӽ�����
                sprintf((char*)p,"�յ�%d�ֽ�,��������",rlen);//���յ����ֽ���
                printf("%s\r\n",p); 			//��ʾ���յ������ݳ���
                printf("��������:%s\r\n",USART3_RX_BUF);//��ʾ���յ�������
				USART3_RX_STA=0;
				
				// USART3�յ������ݽ��н���
				recv_data_analysis(netpro, USART3_RX_BUF);
				
                if(constate!='+')t=1000;		//״̬Ϊ��δ����,������������״̬
                else t=0;                   //״̬Ϊ�Ѿ�������,10����ټ��
            }
            if(t==1000)//����10����û���յ��κ�����,��������ǲ��ǻ�����.
            {
                constate=atk_8266_consta_check();//�õ�����״̬
                if(constate=='+')printf("���ӳɹ�\r\n");  //����״̬
                else printf("����ʧ��\r\n");
                t=0;
            }
            if((t%20)==0)LED0=!LED0;
            atk_8266_at_response(1);
        }
    }
}
