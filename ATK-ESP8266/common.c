#include "common.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//ATK-ESP8266 WIFI模块 公用驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/4/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 

/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//用户配置区

//连接端口号:8086,可自行修改为其他端口.
const u8* portnum="8086";		 

//WIFI STA模式,设置要去连接的路由器无线参数,请根据你自己的路由器设置,自行修改.
const u8* wifista_ssid="ikaros";			//路由器SSID号
const u8* wifista_encryption="wpawpa2_aes";	//wpa/wpa2 aes加密方式
const u8* wifista_password="12345678"; 	//连接密码

//WIFI AP模式,模块对外的无线参数,可自行修改.
const u8* wifiap_ssid="ATK-ESP8266";			//对外SSID号
const u8* wifiap_encryption="wpawpa2_aes";	//wpa/wpa2 aes加密方式
const u8* wifiap_password="12345678"; 		//连接密码 

/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//4个网络模式
const u8 *ATK_ESP8266_CWMODE_TBL[3]={"STA模式 ","AP模式 ","AP&STA模式 "};	//ATK-ESP8266,3种网络模式,默认为路由器(ROUTER)模式 
//4种工作模式
const u8 *ATK_ESP8266_WORKMODE_TBL[3]={"TCP服务器","TCP客户端"," UDP 模式"};	//ATK-ESP8266,4种工作模式
//5种加密方式
const u8 *ATK_ESP8266_ECN_TBL[5]={"OPEN","WEP","WPA_PSK","WPA2_PSK","WPA_WAP2_PSK"};
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 

//usmart支持部分
//将收到的AT指令应答数据返回给电脑串口
//mode:0,不清零USART3_RX_STA;
//     1,清零USART3_RX_STA;
void atk_8266_at_response(u8 mode)
{
	if(USART3_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//添加结束符
		printf("%s",USART3_RX_BUF);	//发送到串口
		if(mode)USART3_RX_STA=0;
	} 
}
//ATK-ESP8266发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果
//    其他,期待应答结果的位置(str的位置)
u8* atk_8266_check_cmd(u8 *str)
{
	
	char *strx=0;
	if(USART3_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)USART3_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}
//向ATK-ESP8266发送命令
//cmd:发送的命令字符串
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)
//       1,发送失败
u8 atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART3_RX_STA=0;
	u3_printf("%s\r\n",cmd);	//发送命令
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(atk_8266_check_cmd(ack))
				{
					printf("ack:%s\r\n",(u8*)ack);
					break;//得到有效数据 
				}
					USART3_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 
//向ATK-ESP8266发送指定数据
//data:发送的数据(不需要添加回车了)
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)luojian
u8 atk_8266_send_data(u8 *data,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART3_RX_STA=0;
	u3_printf("%s",data);	//发送命令
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(atk_8266_check_cmd(ack))break;//得到有效数据 
				USART3_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
}
//ATK-ESP8266退出透传模式
//返回值:0,退出成功;
//       1,退出失败
u8 atk_8266_quit_trans(void)
{
	while((USART3->SR&0X40)==0);	//等待发送空
	USART3->DR='+';      
	delay_ms(15);					//大于串口组帧时间(10ms)
	while((USART3->SR&0X40)==0);	//等待发送空
	USART3->DR='+';      
	delay_ms(15);					//大于串口组帧时间(10ms)
	while((USART3->SR&0X40)==0);	//等待发送空
	USART3->DR='+';      
	delay_ms(500);					//等待500ms
	return atk_8266_send_cmd("AT","OK",20);//退出透传判断.
}
//获取ATK-ESP8266模块的AP+STA连接状态
//返回值:0，未连接;1,连接成功
u8 atk_8266_apsta_check(void)
{
	if(atk_8266_quit_trans())return 0;			//退出透传 
	atk_8266_send_cmd("AT+CIPSTATUS",":",50);	//发送AT+CIPSTATUS指令,查询连接状态
	if(atk_8266_check_cmd("+CIPSTATUS:0")&&
		 atk_8266_check_cmd("+CIPSTATUS:1")&&
		 atk_8266_check_cmd("+CIPSTATUS:2")&&
		 atk_8266_check_cmd("+CIPSTATUS:4"))
		return 0;
	else return 1;
}
//获取ATK-ESP8266模块的连接状态
//返回值:0,未连接;1,连接成功.
u8 atk_8266_consta_check(void)
{
	u8 *p;
	u8 res;
	if(atk_8266_quit_trans())return 0;			//退出透传 
	atk_8266_send_cmd("AT+CIPSTATUS",":",50);	//发送AT+CIPSTATUS指令,查询连接状态
	p=atk_8266_check_cmd("+CIPSTATUS:"); 
	res=*p;									//得到连接状态	
	return res;
}
//键盘码表
const u8* kbd_tbl[13]={"1","2","3","4","5","6","7","8","9",".","0","#","DEL"}; 
u8* kbd_fn_tbl[2];
//加载键盘界面（尺寸为240*140）
//x,y:界面起始坐标（320*240分辨率的时候，x必须为0）
void atk_8266_load_keyboard(u16 x,u16 y)
{
	u16 i;
	POINT_COLOR=RED; 
	LCD_Fill(x,y,x+240,y+140,WHITE);
	LCD_DrawRectangle(x,y,x+240,y+140);						   
	LCD_DrawRectangle(x+80,y,x+160,y+140);	 
	LCD_DrawRectangle(x,y+28,x+240,y+56);
	LCD_DrawRectangle(x,y+84,x+240,y+112);
	POINT_COLOR=BLUE;
	for(i=0;i<15;i++)
	{
		if(i<13)printf("%s",(u8*)kbd_tbl[i]);
		else printf("%s",kbd_fn_tbl[i-13]); 
	}  		 					   
}
//按键状态设置
//x,y:键盘坐标
//key:键值（0~8）
//sta:状态，0，松开；1，按下；
void atk_8266_key_staset(u16 x,u16 y,u8 keyx,u8 sta)
{		  
	u16 i=keyx/3,j=keyx%3;
	if(keyx>15)return;
	if(sta)LCD_Fill(x+j*80+1,y+i*28+1,x+j*80+78,y+i*28+26,GREEN);
	else LCD_Fill(x+j*80+1,y+i*28+1,x+j*80+78,y+i*28+26,WHITE); 
	if(j&&(i>3))printf("%s",(u8*)kbd_fn_tbl[keyx-13]);
	else printf("%s",(u8*)kbd_tbl[keyx]);		 		 
}

//获取Client ip地址
//ipbuf:ip地址输出缓存区
void atk_8266_get_wanip(u8* ipbuf)
{
	u8 *p,*p1;
		if(atk_8266_send_cmd("AT+CIFSR","OK",50))//获取WAN IP地址失败
		{
			ipbuf[0]=0;
			return;
		}		
		p=atk_8266_check_cmd("\"");
		p1=(u8*)strstr((const char*)(p+1),"\"");
		*p1=0;
		sprintf((char*)ipbuf,"%s",p+1);	
}

//获取AP+STA ip地址并在指定位置显示
//ipbuf:ip地址输出缓存区
void atk_8266_get_ip(u8 x,u8 y)
{
		u8 *p;
		u8 *p1;
		u8 *p2;
		u8 *ipbuf;
		u8 *buf;
		p=mymalloc(SRAMIN,32);							//申请32字节内存
		p1=mymalloc(SRAMIN,32);							//申请32字节内存
		p2=mymalloc(SRAMIN,32);							//申请32字节内存
	  ipbuf=mymalloc(SRAMIN,32);							//申请32字节内存
		buf=mymalloc(SRAMIN,32);							//申请32字节内存
		if(atk_8266_send_cmd("AT+CIFSR","OK",50))//获取WAN IP地址失败
		{ 
			*ipbuf=0;
		}
		else
		{		
			p=atk_8266_check_cmd("APIP,\"");
			p1=(u8*)strstr((const char*)(p+6),"\"");
			p2=p1;
			*p1=0;
			ipbuf=p+6;
			sprintf((char*)buf,"AP IP:%s 端口:%s",ipbuf,(u8*)portnum);
			printf("%s",buf);				//显示AP模式的IP地址和端口
			p=(u8*)strstr((const char*)(p2+1),"STAIP,\"");
			p1=(u8*)strstr((const char*)(p+7),"\"");
			*p1=0;
			ipbuf=p+7;
			sprintf((char*)buf,"STA IP:%s 端口:%s",ipbuf,(u8*)portnum);
			printf("%s",buf);				//显示STA模式的IP地址和端口
			myfree(SRAMIN,p);		//释放内存
			myfree(SRAMIN,p1);		//释放内存
			myfree(SRAMIN,p2);		//释放内存
			myfree(SRAMIN,ipbuf);		//释放内存
			myfree(SRAMIN,buf);		//释放内存
		}
}

//ATK-ESP8266模块信息显示
//x,y:显示信息的起始坐标.
//wanip:0,全部更新显示;1,仅更新wanip.
void atk_8266_msg_show(u16 x,u16 y,u8 wanip)
{
	u8 *p,*p1,*p2;
	p=mymalloc(SRAMIN,32);							//申请32字节内存
	p1=mymalloc(SRAMIN,32);							//申请32字节内存
	p2=mymalloc(SRAMIN,32);							//申请32字节内存
	POINT_COLOR=BLUE;
	atk_8266_send_cmd("AT+CWMODE=2","OK",20);
	atk_8266_send_cmd("AT+RST","OK",20);
	delay_ms(1000);//延时2s等待模块重启
	delay_ms(1000);//
	delay_ms(1000);
	delay_ms(1000);
	sprintf((char*)p,"AT+CWSAP=\"%s\",\"%s\",1,4",wifiap_ssid,wifiap_password);    //配置模块AP模式无线参数
	atk_8266_send_cmd(p,"OK",1000);
	if(wanip==0)//全更新
	{
		atk_8266_send_cmd("AT+GMR","OK",20);		//获取固件版本号
		p=atk_8266_check_cmd("SDK version:");
		p1=(u8*)strstr((const char*)(p+12),"(");
		*p1=0;
		printf("%s","固件版本:");
		printf("%s\n",p+12);
		atk_8266_send_cmd("AT+CWMODE?","+CWMODE:",20);	//获取网络模式
		p=atk_8266_check_cmd(":");
		printf("网络模式:");printf("%s\n",(u8*)ATK_ESP8266_CWMODE_TBL[*(p+1)-'1']);
		atk_8266_send_cmd("AT+CWSAP?","+CWSAP:",20);	//获取wifi配置
		p=atk_8266_check_cmd("\"");
		p1=(u8*)strstr((const char*)(p+1),"\"");
		p2=p1;
		*p1=0;
		printf("SSID号:");printf("%s\n",p+1);
		p=(u8*)strstr((const char*)(p2+1),"\"");
		p1=(u8*)strstr((const char*)(p+1),"\"");
		p2=p1;
		*p1=0;		
		printf("密码:");printf("%s\n",p+1);
		p=(u8*)strstr((const char*)(p2+1),",");
		p1=(u8*)strstr((const char*)(p+1),",");
		*p1=0;
		printf("通道号:");printf("%s\n",p+1);
		printf("加密方式:");printf("%s\n",(u8*)ATK_ESP8266_ECN_TBL[*(p1+1)-'0']);
	}
	myfree(SRAMIN,p);		//释放内存 
	myfree(SRAMIN,p1);		//释放内存 
	myfree(SRAMIN,p2);		//释放内存 
}
//ATK-ESP8266模块WIFI配置参数显示(仅WIFI STA/WIFI AP模式测试时使用)
//x,y:显示信息的起始坐标.
//rmd:提示信息
//ssid,encryption,password:无线网络的SSID,加密方式,密码
void atk_8266_wificonf_show(u16 x,u16 y,u8* rmd,u8* ssid,u8* encryption,u8* password)
{ 
	printf("%s",rmd);//显示提示信息 
	printf("SSID:");
	printf("%s\r\n",ssid);
	printf("加密方式:");
	printf("%s\r\n",encryption);
	printf("密码:"); 
	printf("%s\r\n",password); 	  
}
//工作模式选择
//返回值:
//0,TCP服务器
//1,TCP客户端
//2,UDP模式
u8 atk_8266_netpro_sel(u16 x,u16 y,u8* name) 
{
	u8 key=0,t=0,*p;
	u8 netpro=0;
	LCD_Clear(WHITE);
	POINT_COLOR=RED;
	p=mymalloc(SRAMIN,50);//申请50个字节的内存
	sprintf((char*)p,"%s 工作模式选择\r\n",name);
	printf("%s\r\n",p); 				    	 
	printf("KEY0:下一个\r\n"); 				    	 
	printf("KEY1:上一个\r\n");				    	 
	printf("WK_UP:确定\r\n"); 
	printf("请选择:\r\n"); 
 	printf("1. TCP服务器\r\n"); 				    	 
	printf("2. TCP客户端\r\n");				    	 
	printf("3. UDP模式\r\n");  
	printf("当前选择: 1\r\n");  
	while(1)
	{
		key=KEY_Scan(0); 
		if(key)
		{
			if(key==WKUP_PRES)break;       //WK_UP按下
			if(key==KEY0_PRES)//KEY0按下
			{
				if(netpro<2)netpro++;
				else netpro=0;
			}else if(key==KEY1_PRES)//KEY1按下
			{
				if(netpro>0)netpro--;
				else netpro=2; 
			}
			printf("KEY0:下一个\r\n"); 				    	 
			printf("KEY1:上一个\r\n");				    	 
			printf("WK_UP:确定\r\n"); 
			printf("请选择:\r\n"); 
			printf("1. TCP服务器\r\n"); 				    	 
			printf("2. TCP客户端\r\n");				    	 
			printf("3. UDP模式\r\n"); 
			printf("当前选择: %d\r\n", netpro + 1);	
		} 
		delay_ms(10);
		atk_8266_at_response(1);
		if((t++)==20){t=0;LED0=!LED0;}//LED闪烁
	} 
	myfree(SRAMIN,p);
	return netpro;//返回网络模式选择值 
} 

//STA模式下的AP的TCP、UDP工作模式配置
u8 atk_8266_mode_cofig(u8 netpro)
{
	//u8 netpro;
	u8 ipbuf[16] = "192.168.1.1"; 	//IP缓存
	u8 *p;
	u8 key;
	p=mymalloc(SRAMIN,32);//申请32个字节的内存
PRESTA:		
	netpro|=(atk_8266_netpro_sel(50,30,(u8*)ATK_ESP8266_CWMODE_TBL[1]))<<4;	//网络模式选择
	if(netpro&0X20)
	{
		LCD_Clear(WHITE);
		if(netpro&0X03)sprintf((char*)p,"AT+CIPSTART=1,\"UDP\",\"%s\",%s",ipbuf,(u8*)portnum);    //配置目标UDP服务器,及ID号，STA模式下为0
		else sprintf((char*)p,"AT+CIPSTART=0,\"UDP\",\"%s\",%s",ipbuf,(u8*)portnum);    //配置目标UDP服务器,及ID号，STA模式下为0
		delay_ms(200);
		LCD_Clear(WHITE);
		atk_8266_send_cmd(p,"OK",200);
	}
	else if(netpro&0X10)     //AP TCP Client    透传模式测试
	{
		LCD_Clear(WHITE);
		POINT_COLOR=RED;
		printf("ATK-ESP WIFI-STA 测试\r\n"); 
		printf("正在配置ATK-ESP模块,请稍等...\r\n");
		if(netpro&0X03)sprintf((char*)p,"AT+CIPSTART=1,\"TCP\",\"%s\",%s",ipbuf,(u8*)portnum);    //配置目标TCP服务器,及ID号，STA模式为client时，为1
		else sprintf((char*)p,"AT+CIPSTART=0,\"TCP\",\"%s\",%s",ipbuf,(u8*)portnum);    //配置目标TCP服务器,及ID号，STA模式为server时，为0
		while(atk_8266_send_cmd(p,"OK",200))
			{
				LCD_Clear(WHITE);
				POINT_COLOR=RED;
				printf("WK_UP:返回重选\r\n");
				printf("ATK-ESP 连接TCP Server失败\r\n"); //连接失败	 
				key=KEY_Scan(0);
				if(key==WKUP_PRES)goto PRESTA;
			}				
	}
	else;   //服务器模式不用配置
	myfree(SRAMIN,p);
	return netpro;
}

//测试界面主UI
void atk_8266_mtest_ui(u16 x,u16 y)
{ 
	printf("ATK_ESP8266 WIFI模块测试\r\n"); 
	printf("请选择:\r\n"); 				    	 
	printf("KEY0:WIFI STA+AP\r\n"); 				    	 
	printf("KEY1:WIFI STA\r\n");				    	 
	printf("WK_UP:WIFI AP\r\n"); 
 	atk_8266_msg_show(x,y+125,0);
}
//ATK-ESP8266模块测试主函数
void atk_8266_test(void)
{
//	u16 rlen=0;
	u8 key;
	u8 timex = 0;
	printf("ATK-ESP8266 WIFI模块测试\r\n"); 
	while(atk_8266_send_cmd("AT","OK",20))//检查WIFI模块是否在线
	{
		atk_8266_quit_trans();//退出透传
		atk_8266_send_cmd("AT+CIPMODE=0","OK",200);  //关闭透传模式	
		printf("未检测到模块!!!\r\n");
		delay_ms(800);
		printf("尝试连接模块...\r\n"); 
	} 
	while(atk_8266_send_cmd("ATE0","OK",20));//关闭回显
	atk_8266_mtest_ui(32,30);
	while(1)
	{
		delay_ms(10); 
		atk_8266_at_response(1);//检查ATK-ESP8266模块发送过来的数据,及时上传给电脑
		key=KEY_Scan(0); 
		if(key)
		{
			switch(key)
			{
				case 1://KEY0
					printf("ATK-ESP WIFI-AP+STA 测试\r\n");
					printf("正在配置ATK-ESP8266模块，请稍等...\r\n");
					atk_8266_apsta_test();	//串口以太网测试
					break;
				case 2://KEY1
					printf("ATK-ESP WIFI-STA 测试\r\n");
					printf("正在配置ATK-ESP8266模块，请稍等...\r\n");
					atk_8266_wifista_test();//WIFI STA测试
					break;
				case 4://WK_UP
					printf("ATK-ESP WIFI-AP 测试\r\n");
					printf("正在配置ATK-ESP8266模块，请稍等...\r\n");
					atk_8266_wifiap_test();	//WIFI AP测试
					break;
			}
			atk_8266_mtest_ui(32,30);
			timex=0;
		}
		
		if((timex%20)==0) LED0=!LED0;//200ms闪烁 
		if((timex%200)==0)
		{
			printf("ATK_ESP8266 WIFI模块测试\r\n"); 
			printf("请选择:\r\n"); 				    	 
			printf("KEY0:WIFI STA+AP\r\n"); 				    	 
			printf("KEY1:WIFI STA\r\n");				    	 
			printf("WK_UP:WIFI AP\r\n");
			timex = 0;
		}
		timex++;	 
	} 
}

// USART3收到的数据进行解析
void send_data_to_usart3(u8 netpro, char *data)
{
	u8 len = 0;
	u8 buf[20] = {0};
	
	len = strlen(data);
	
	if((netpro==3)||(netpro==2))   //UDP
	{
		sprintf((char*)buf, "AT+CIPSEND=%d", len);
		atk_8266_send_cmd(buf,"OK",200);  //发送指定长度的数据
		delay_ms(200);
		atk_8266_send_data((u8 *)data,"OK",100);  //发送指定长度的数据
	}
	else if((netpro==1))   //TCP Client
	{
		atk_8266_quit_trans();
		atk_8266_send_cmd("AT+CIPSEND","OK",20);         //开始透传
		u3_printf("%s", data);
	}
	else // TCP server
	{
		sprintf((char*)buf, "AT+CIPSEND=0,%d", len);
		atk_8266_send_cmd(buf,"OK",200);  //发送指定长度的数据
		delay_ms(200);
		atk_8266_send_data((u8 *)data,"OK",100);  //发送指定长度的数据
	}
}

// USART3收到的数据进行解析
void recv_data_analysis(u8 netpro, u8 *USART3_RX_BUF)
{
	char *ptr = NULL;
	char buf[100] = {0};
	u8 temperature = 0;
    u8 humidity = 0;
	
	ptr = mymalloc(SRAMIN, 600); //申请600字节内存
	
	if((netpro==1))   //TCP Client
	{
		sprintf(ptr, "%s", USART3_RX_BUF);
	}
	else    //TCP Server
	{
		ptr = strstr((char *)USART3_RX_BUF, ":");
		if(NULL != ptr)
		{
			// ptr指针后移一位
			ptr++; 
			
			printf("解析后的命令为(<>内):<%s>\r\n", ptr);
		}
	}
	
	// 接收到的数据进行处理
	if(strcmp(ptr, "LED1 ON") == 0)
	{
		LED1 = 0;
		printf("打开LED1\r\n");
		
		send_data_to_usart3(netpro, "打开LED1成功\r\n");
	}
	else if(strcmp(ptr, "LED1 OFF") == 0)
	{
		LED1 = 1;
		printf("关闭LED1\r\n");
		
		send_data_to_usart3(netpro, "关闭LED1成功\r\n");
	}
	else if(strcmp(ptr, "BEEP OFF") == 0)
	{
		BEEP = 0;
		printf("关闭BEEP\r\n");
		
		send_data_to_usart3(netpro, "关闭BEEP成功\r\n");
	}
	else if(strcmp(ptr, "BEEP ON") == 0)
	{
		BEEP = 1;
		printf("开启BEEP\r\n");
		
		send_data_to_usart3(netpro, "开启BEEP成功\r\n");
	}
	else if(strcmp(ptr, "GET T&H") == 0)
	{
		DHT11_Read_Data(&temperature, &humidity);	//读取温湿度值
		sprintf(buf, "读取，温度:%d℃ 湿度:%d%%RH\r\n", temperature, humidity);
		printf("%s", buf);
		
		send_data_to_usart3(netpro, buf);
	}
	
	myfree(SRAMIN, ptr);		// 释放内存
}

