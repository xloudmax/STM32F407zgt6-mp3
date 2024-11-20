#include "usmart.h"
#include "usart.h"
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 

/////////////////////////////////////////////////////////////////////////////////////
//ÏµÍ³ÃüÁî
u8 *sys_cmd_tab[]=
{
	"?",
	"help",
	"list",
	"id",
	"hex",
	"dec",
	"runtime",	   
};	    
//´¦ÀíÏµÍ³Ö¸Áî
//0,³É¹¦´¦Àí;ÆäËû,´íÎó´úÂë;
u8 usmart_sys_cmd_exe(u8 *str)
{
	u8 i;
	u8 sfname[MAX_FNAME_LEN];//´æ·Å±¾µØº¯ÊıÃû
	u8 pnum;
	u8 rval;
	u32 res;  
	res=usmart_get_cmdname(str,sfname,&i,MAX_FNAME_LEN);//µÃµ½Ö¸Áî¼°Ö¸Áî³¤¶È
	if(res)return USMART_FUNCERR;//´íÎóµÄÖ¸Áî 
	str+=i;	 	 			    
	for(i=0;i<sizeof(sys_cmd_tab)/4;i++)//Ö§³ÖµÄÏµÍ³Ö¸Áî
	{
		if(usmart_strcmp(sfname,sys_cmd_tab[i])==0)break;
	}
	switch(i)
	{					   
		case 0:
		case 1://°ïÖúÖ¸Áî
			printf("\r\n");
#if USMART_USE_HELP
			printf("------------------------USMART V3.1------------------------ \r\n");
			
			printf("Ëü,Äã¿ÉÒÔÍ¨¹ı´®¿ÚÖúÊÖµ÷ÓÃ³ÌĞòÀïÃæµÄÈÎºÎº¯Êı,²¢Ö´ĞĞ.Òò´Ë,Äã¿É\r\n");
			printf("ÒÔËæÒâ¸ü¸Äº¯ÊıµÄÊäÈë²ÎÊı(Ö§³ÖÊı×Ö(10/16½øÖÆ)¡¢×Ö·û´®¡¢º¯ÊıÈë\r\n");	  
			printf("¿ÚµØÖ·µÈ×÷Îª²ÎÊı),µ¥¸öº¯Êı×î¶àÖ§³Ö10¸öÊäÈë²ÎÊı,²¢Ö§³Öº¯Êı·µ \r\n");
			printf("»ØÖµÏÔÊ¾.ĞÂÔö²ÎÊıÏÔÊ¾½øÖÆÉèÖÃ¹¦ÄÜ,ĞÂÔö½øÖÆ×ª»»¹¦ÄÜ.\r\n");
			printf("¼¼ÊõÖ§³Ö:s8088.taobao.com\r\n");
			printf("USMARTÓĞ7¸öÏµÍ³ÃüÁî:\r\n");
			printf("?:      »ñÈ¡°ïÖúĞÅÏ¢\r\n");
			printf("help:   »ñÈ¡°ïÖúĞÅÏ¢\r\n");
			printf("list:   ¿ÉÓÃµÄº¯ÊıÁĞ±í\r\n\n");
			printf("id:     ¿ÉÓÃº¯ÊıµÄIDÁĞ±í\r\n\n");
			printf("hex:    ²ÎÊı16½øÖÆÏÔÊ¾,ºó¸ú¿Õ¸ñ+Êı×Ö¼´Ö´ĞĞ½øÖÆ×ª»»\r\n\n");
			printf("dec:    ²ÎÊı10½øÖÆÏÔÊ¾,ºó¸ú¿Õ¸ñ+Êı×Ö¼´Ö´ĞĞ½øÖÆ×ª»»\r\n\n");
			printf("runtime:1,¿ªÆôº¯ÊıÔËĞĞ¼ÆÊ±;0,¹Ø±Õº¯ÊıÔËĞĞ¼ÆÊ±;\r\n\n");
			printf("Çë°´ÕÕ³ÌĞò±àĞ´¸ñÊ½ÊäÈëº¯ÊıÃû¼°²ÎÊı²¢ÒÔ»Ø³µ¼ü½áÊø.\r\n");    
			printf("--------------------------------------------- \r\n");
#else
			printf("Ö¸ÁîÊ§Ğ§\r\n");
#endif
			break;
		case 2://²éÑ¯Ö¸Áî
			printf("\r\n");
			printf("-------------------------º¯ÊıÇåµ¥--------------------------- \r\n");
			for(i=0;i<usmart_dev.fnum;i++)printf("%s\r\n",usmart_dev.funs[i].name);
			printf("\r\n");
			break;	 
		case 3://²éÑ¯ID
			printf("\r\n");
			printf("-------------------------º¯Êı ID --------------------------- \r\n");
			for(i=0;i<usmart_dev.fnum;i++)
			{
				usmart_get_fname((u8*)usmart_dev.funs[i].name,sfname,&pnum,&rval);//µÃµ½±¾µØº¯ÊıÃû 
				printf("%s id is:\r\n0X%08X\r\n",sfname,usmart_dev.funs[i].func); //ÏÔÊ¾ID
			}
			printf("\r\n");
			break;
		case 4://hexÖ¸Áî
			printf("\r\n");
			usmart_get_aparm(str,sfname,&i);
			if(i==0)//²ÎÊıÕı³£
			{
				i=usmart_str2num(sfname,&res);	   	//¼ÇÂ¼¸Ã²ÎÊı	
				if(i==0)						  	//½øÖÆ×ª»»¹¦ÄÜ
				{
					printf("HEX:0X%X\r\n",res);	   	//×ªÎª16½øÖÆ
				}else if(i!=4)return USMART_PARMERR;//²ÎÊı´íÎó.
				else 				   				//²ÎÊıÏÔÊ¾Éè¶¨¹¦ÄÜ
				{
					printf("16½øÖÆ²ÎÊıÏÔÊ¾!\r\n");
					usmart_dev.sptype=SP_TYPE_HEX;  
				}

			}else return USMART_PARMERR;			//²ÎÊı´íÎó.
			printf("\r\n"); 
			break;
		case 5://decÖ¸Áî
			printf("\r\n");
			usmart_get_aparm(str,sfname,&i);
			if(i==0)//²ÎÊıÕı³£
			{
				i=usmart_str2num(sfname,&res);	   	//¼ÇÂ¼¸Ã²ÎÊı	
				if(i==0)						   	//½øÖÆ×ª»»¹¦ÄÜ
				{
					printf("DEC:%lu\r\n",res);	   	//×ªÎª10½øÖÆ
				}else if(i!=4)return USMART_PARMERR;//²ÎÊı´íÎó.
				else 				   				//²ÎÊıÏÔÊ¾Éè¶¨¹¦ÄÜ
				{
					printf("10½øÖÆ²ÎÊıÏÔÊ¾!\r\n");
					usmart_dev.sptype=SP_TYPE_DEC;  
				}

			}else return USMART_PARMERR;			//²ÎÊı´íÎó. 
			printf("\r\n"); 
			break;	 
		case 6://runtimeÖ¸Áî,ÉèÖÃÊÇ·ñÏÔÊ¾º¯ÊıÖ´ĞĞÊ±¼ä
			printf("\r\n");
			usmart_get_aparm(str,sfname,&i);
			if(i==0)//²ÎÊıÕı³£
			{
				i=usmart_str2num(sfname,&res);	   		//¼ÇÂ¼¸Ã²ÎÊı	
				if(i==0)						   		//¶ÁÈ¡Ö¸¶¨µØÖ·Êı¾İ¹¦ÄÜ
				{
					if(USMART_ENTIMX_SCAN==0)printf("\r\nError! \r\nTo EN RunTime function,Please set USMART_ENTIMX_SCAN = 1 first!\r\n");//±¨´í
					else
					{
						usmart_dev.runtimeflag=res;
						if(usmart_dev.runtimeflag)printf("Run Time Calculation ON\r\n");
						else printf("Run Time Calculation OFF\r\n"); 
					}
				}else return USMART_PARMERR;   			//Î´´ø²ÎÊı,»òÕß²ÎÊı´íÎó	 
 			}else return USMART_PARMERR;				//²ÎÊı´íÎó. 
			printf("\r\n"); 
			break;	    
		default://·Ç·¨Ö¸Áî
			return USMART_FUNCERR;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////
//ÒÆÖ²×¢Òâ:±¾ÀıÊÇÒÔstm32ÎªÀı,Èç¹ûÒªÒÆÖ²µ½ÆäËûmcu,Çë×öÏàÓ¦ĞŞ¸Ä.
//usmart_reset_runtime,Çå³ıº¯ÊıÔËĞĞÊ±¼ä,Á¬Í¬¶¨Ê±Æ÷µÄ¼ÆÊı¼Ä´æÆ÷ÒÔ¼°±êÖ¾Î»Ò»ÆğÇåÁã.²¢ÉèÖÃÖØ×°ÔØÖµÎª×î´ó,ÒÔ×î´óÏŞ¶ÈµÄÑÓ³¤¼ÆÊ±Ê±¼ä.
//usmart_get_runtime,»ñÈ¡º¯ÊıÔËĞĞÊ±¼ä,Í¨¹ı¶ÁÈ¡CNTÖµ»ñÈ¡,ÓÉÓÚusmartÊÇÍ¨¹ıÖĞ¶Ïµ÷ÓÃµÄº¯Êı,ËùÒÔ¶¨Ê±Æ÷ÖĞ¶Ï²»ÔÙÓĞĞ§,´ËÊ±×î´óÏŞ¶È
//Ö»ÄÜÍ³¼Æ2´ÎCNTµÄÖµ,Ò²¾ÍÊÇÇåÁãºó+Òç³öÒ»´Î,µ±Òç³ö³¬¹ı2´Î,Ã»·¨´¦Àí,ËùÒÔ×î´óÑÓÊ±,¿ØÖÆÔÚ:2*¼ÆÊıÆ÷CNT*0.1ms.¶ÔSTM32À´Ëµ,ÊÇ:13.1s×óÓÒ
//ÆäËûµÄ:TIM4_IRQHandlerºÍTimer4_Init,ĞèÒª¸ù¾İMCUÌØµã×ÔĞĞĞŞ¸Ä.È·±£¼ÆÊıÆ÷¼ÆÊıÆµÂÊÎª:10Khz¼´¿É.ÁíÍâ,¶¨Ê±Æ÷²»Òª¿ªÆô×Ô¶¯ÖØ×°ÔØ¹¦ÄÜ!!

#if USMART_ENTIMX_SCAN==1
//¸´Î»runtime
//ĞèÒª¸ù¾İËùÒÆÖ²µ½µÄMCUµÄ¶¨Ê±Æ÷²ÎÊı½øĞĞĞŞ¸Ä
void usmart_reset_runtime(void)
{
 
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);//Çå³ıÖĞ¶Ï±êÖ¾Î» 
	TIM_SetAutoreload(TIM4,0XFFFF);//½«ÖØ×°ÔØÖµÉèÖÃµ½×î´ó
	TIM_SetCounter(TIM4,0);		//Çå¿Õ¶¨Ê±Æ÷µÄCNT
	usmart_dev.runtime=0;	
}
//»ñµÃruntimeÊ±¼ä
//·µ»ØÖµ:Ö´ĞĞÊ±¼ä,µ¥Î»:0.1ms,×î´óÑÓÊ±Ê±¼äÎª¶¨Ê±Æ÷CNTÖµµÄ2±¶*0.1ms
//ĞèÒª¸ù¾İËùÒÆÖ²µ½µÄMCUµÄ¶¨Ê±Æ÷²ÎÊı½øĞĞĞŞ¸Ä
u32 usmart_get_runtime(void)
{
	if(TIM_GetFlagStatus(TIM4,TIM_FLAG_Update)==SET)//ÔÚÔËĞĞÆÚ¼ä,²úÉúÁË¶¨Ê±Æ÷Òç³ö
	{
		usmart_dev.runtime+=0XFFFF;
	}
	usmart_dev.runtime+=TIM_GetCounter(TIM4);
	return usmart_dev.runtime;		//·µ»Ø¼ÆÊıÖµ
}  
//ÏÂÃæÕâÁ½¸öº¯Êı,·ÇUSMARTº¯Êı,·Åµ½ÕâÀï,½ö½ö·½±ãÒÆÖ². 
//¶¨Ê±Æ÷4ÖĞ¶Ï·şÎñ³ÌĞò	 
void TIM4_IRQHandler(void)
{ 		    		  			    
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET)//Òç³öÖĞ¶Ï
	{
		usmart_dev.scan();	//Ö´ĞĞusmartÉ¨Ãè	
		TIM_SetCounter(TIM4,0);		//Çå¿Õ¶¨Ê±Æ÷µÄCNT
		TIM_SetAutoreload(TIM4,100);//»Ö¸´Ô­À´µÄÉèÖÃ		    				   				     	    	
	}				   
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //Çå³ıÖĞ¶Ï±êÖ¾Î»    
}
//Ê¹ÄÜ¶¨Ê±Æ÷4,Ê¹ÄÜÖĞ¶Ï.
void Timer4_Init(u16 arr,u16 psc)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///Ê¹ÄÜTIM4Ê±ÖÓ

	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //¶¨Ê±Æ÷·ÖÆµ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //ÏòÉÏ¼ÆÊıÄ£Ê½
	TIM_TimeBaseInitStructure.TIM_Period=arr;   //×Ô¶¯ÖØ×°ÔØÖµ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//³õÊ¼»¯¶¨Ê±Æ÷4
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //ÔÊĞí¶¨Ê±Æ÷4¸üĞÂÖĞ¶Ï
	TIM_Cmd(TIM4,ENABLE); //Ê¹ÄÜ¶¨Ê±Æ÷4
 
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;//Íâ²¿ÖĞ¶Ï4
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//ÇÀÕ¼ÓÅÏÈ¼¶3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;//×ÓÓÅÏÈ¼¶3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//Ê¹ÄÜÍâ²¿ÖĞ¶ÏÍ¨µÀ
  NVIC_Init(&NVIC_InitStructure);//ÅäÖÃNVIC
	 							 
}
#endif
////////////////////////////////////////////////////////////////////////////////////////
//³õÊ¼»¯´®¿Ú¿ØÖÆÆ÷
//sysclk:ÏµÍ³Ê±ÖÓ£¨Mhz£©
void usmart_init(u8 sysclk)
{
#if USMART_ENTIMX_SCAN==1
	Timer4_Init(1000,(u32)sysclk*100-1);//·ÖÆµ,Ê±ÖÓÎª10K ,100msÖĞ¶ÏÒ»´Î,×¢Òâ,¼ÆÊıÆµÂÊ±ØĞëÎª10Khz,ÒÔºÍruntimeµ¥Î»(0.1ms)Í¬²½.
#endif
	usmart_dev.sptype=1;	//Ê®Áù½øÖÆÏÔÊ¾²ÎÊı
}		
//´ÓstrÖĞ»ñÈ¡º¯ÊıÃû,id,¼°²ÎÊıĞÅÏ¢
//*str:×Ö·û´®Ö¸Õë.
//·µ»ØÖµ:0,Ê¶±ğ³É¹¦;ÆäËû,´íÎó´úÂë.
u8 usmart_cmd_rec(u8*str) 
{
	u8 sta,i,rval;//×´Ì¬	 
	u8 rpnum,spnum;
	u8 rfname[MAX_FNAME_LEN];//Ôİ´æ¿Õ¼ä,ÓÃÓÚ´æ·Å½ÓÊÕµ½µÄº¯ÊıÃû  
	u8 sfname[MAX_FNAME_LEN];//´æ·Å±¾µØº¯ÊıÃû
	sta=usmart_get_fname(str,rfname,&rpnum,&rval);//µÃµ½½ÓÊÕµ½µÄÊı¾İµÄº¯ÊıÃû¼°²ÎÊı¸öÊı	  
	if(sta)return sta;//´íÎó
	for(i=0;i<usmart_dev.fnum;i++)
	{
		sta=usmart_get_fname((u8*)usmart_dev.funs[i].name,sfname,&spnum,&rval);//µÃµ½±¾µØº¯ÊıÃû¼°²ÎÊı¸öÊı
		if(sta)return sta;//±¾µØ½âÎöÓĞÎó	  
		if(usmart_strcmp(sfname,rfname)==0)//ÏàµÈ
		{
			if(spnum>rpnum)return USMART_PARMERR;//²ÎÊı´íÎó(ÊäÈë²ÎÊı±ÈÔ´º¯Êı²ÎÊıÉÙ)
			usmart_dev.id=i;//¼ÇÂ¼º¯ÊıID.
			break;//Ìø³ö.
		}	
	}
	if(i==usmart_dev.fnum)return USMART_NOFUNCFIND;	//Î´ÕÒµ½Æ¥ÅäµÄº¯Êı
 	sta=usmart_get_fparam(str,&i);					//µÃµ½º¯Êı²ÎÊı¸öÊı	
	if(sta)return sta;								//·µ»Ø´íÎó
	usmart_dev.pnum=i;								//²ÎÊı¸öÊı¼ÇÂ¼
    return USMART_OK;
}
//usamrtÖ´ĞĞº¯Êı
//¸Ãº¯ÊıÓÃÓÚ×îÖÕÖ´ĞĞ´Ó´®¿ÚÊÕµ½µÄÓĞĞ§º¯Êı.
//×î¶àÖ§³Ö10¸ö²ÎÊıµÄº¯Êı,¸ü¶àµÄ²ÎÊıÖ§³ÖÒ²ºÜÈİÒ×ÊµÏÖ.²»¹ıÓÃµÄºÜÉÙ.Ò»°ã5¸ö×óÓÒµÄ²ÎÊıµÄº¯ÊıÒÑ¾­ºÜÉÙ¼ûÁË.
//¸Ãº¯Êı»áÔÚ´®¿Ú´òÓ¡Ö´ĞĞÇé¿ö.ÒÔ:"º¯ÊıÃû(²ÎÊı1£¬²ÎÊı2...²ÎÊıN)=·µ»ØÖµ".µÄĞÎÊ½´òÓ¡.
//µ±ËùÖ´ĞĞµÄº¯ÊıÃ»ÓĞ·µ»ØÖµµÄÊ±ºò,Ëù´òÓ¡µÄ·µ»ØÖµÊÇÒ»¸öÎŞÒâÒåµÄÊı¾İ.
void usmart_exe(void)
{
	u8 id,i;
	u32 res;		   
	u32 temp[MAX_PARM];//²ÎÊı×ª»»,Ê¹Ö®Ö§³ÖÁË×Ö·û´® 
	u8 sfname[MAX_FNAME_LEN];//´æ·Å±¾µØº¯ÊıÃû
	u8 pnum,rval;
	id=usmart_dev.id;
	if(id>=usmart_dev.fnum)return;//²»Ö´ĞĞ.
	usmart_get_fname((u8*)usmart_dev.funs[id].name,sfname,&pnum,&rval);//µÃµ½±¾µØº¯ÊıÃû,¼°²ÎÊı¸öÊı 
	printf("\r\n%s(",sfname);//Êä³öÕıÒªÖ´ĞĞµÄº¯ÊıÃû
	for(i=0;i<pnum;i++)//Êä³ö²ÎÊı
	{
		if(usmart_dev.parmtype&(1<<i))//²ÎÊıÊÇ×Ö·û´®
		{
			printf("%c",'"');			 
			printf("%s",usmart_dev.parm+usmart_get_parmpos(i));
			printf("%c",'"');
			temp[i]=(u32)&(usmart_dev.parm[usmart_get_parmpos(i)]);
		}else						  //²ÎÊıÊÇÊı×Ö
		{
			temp[i]=*(u32*)(usmart_dev.parm+usmart_get_parmpos(i));
			if(usmart_dev.sptype==SP_TYPE_DEC)printf("%lu",temp[i]);//10½øÖÆ²ÎÊıÏÔÊ¾
			else printf("0X%X",temp[i]);//16½øÖÆ²ÎÊıÏÔÊ¾ 	   
		}
		if(i!=pnum-1)printf(",");
	}
	printf(")");
	usmart_reset_runtime();	//¼ÆÊ±Æ÷ÇåÁã,¿ªÊ¼¼ÆÊ±
	switch(usmart_dev.pnum)
	{
		case 0://ÎŞ²ÎÊı(voidÀàĞÍ)											  
			res=(*(u32(*)())usmart_dev.funs[id].func)();
			break;
	    case 1://ÓĞ1¸ö²ÎÊı
			res=(*(u32(*)())usmart_dev.funs[id].func)(temp[0]);
			break;
	    case 2://ÓĞ2¸ö²ÎÊı
			res=(*(u32(*)())usmart_dev.funs[id].func)(temp[0],temp[1]);
			break;
	    case 3://ÓĞ3¸ö²ÎÊı
			res=(*(u32(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2]);
			break;
	    case 4://ÓĞ4¸ö²ÎÊı
			res=(*(u32(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3]);
			break;
	    case 5://ÓĞ5¸ö²ÎÊı
			res=(*(u32(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4]);
			break;
	    case 6://ÓĞ6¸ö²ÎÊı
			res=(*(u32(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5]);
			break;
	    case 7://ÓĞ7¸ö²ÎÊı
			res=(*(u32(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5],temp[6]);
			break;
	    case 8://ÓĞ8¸ö²ÎÊı
			res=(*(u32(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5],temp[6],temp[7]);
			break;
	    case 9://ÓĞ9¸ö²ÎÊı
			res=(*(u32(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5],temp[6],temp[7],temp[8]);
			break;
	    case 10://ÓĞ10¸ö²ÎÊı
			res=(*(u32(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5],temp[6],temp[7],temp[8],temp[9]);
			break;
	}
	usmart_get_runtime();//»ñÈ¡º¯ÊıÖ´ĞĞÊ±¼ä
	if(rval==1)//ĞèÒª·µ»ØÖµ.
	{
		if(usmart_dev.sptype==SP_TYPE_DEC)printf("=%lu;\r\n",res);//Êä³öÖ´ĞĞ½á¹û(10½øÖÆ²ÎÊıÏÔÊ¾)
		else printf("=0X%X;\r\n",res);//Êä³öÖ´ĞĞ½á¹û(16½øÖÆ²ÎÊıÏÔÊ¾)	   
	}else printf(";\r\n");		//²»ĞèÒª·µ»ØÖµ,Ö±½ÓÊä³ö½áÊø
	if(usmart_dev.runtimeflag)	//ĞèÒªÏÔÊ¾º¯ÊıÖ´ĞĞÊ±¼ä
	{ 
		printf("Function Run Time:%d.%1dms\r\n",usmart_dev.runtime/10,usmart_dev.runtime%10);//´òÓ¡º¯ÊıÖ´ĞĞÊ±¼ä 
	}	
}
//usmartÉ¨Ãèº¯Êı
//Í¨¹ıµ÷ÓÃ¸Ãº¯Êı,ÊµÏÖusmartµÄ¸÷¸ö¿ØÖÆ.¸Ãº¯ÊıĞèÒªÃ¿¸ôÒ»¶¨Ê±¼ä±»µ÷ÓÃÒ»´Î
//ÒÔ¼°Ê±Ö´ĞĞ´Ó´®¿Ú·¢¹ıÀ´µÄ¸÷¸öº¯Êı.
//±¾º¯Êı¿ÉÒÔÔÚÖĞ¶ÏÀïÃæµ÷ÓÃ,´Ó¶øÊµÏÖ×Ô¶¯¹ÜÀí.
//Èç¹»§,ÔòUSART_RX_STAºÍUSART_RX_BUF[]ĞèÒªÓÃ»§×Ô¼ºÊµÏÖ
void usmart_scan(void)
{
	u8 sta,len;  
	if(USART_RX_STA&0x8000)//´®¿Ú½ÓÊÕÍê³É£¿
	{					   
		len=USART_RX_STA&0x3fff;	//µÃµ½´Ë´Î½ÓÊÕµ½µÄÊı¾İ³¤¶È
		USART_RX_BUF[len]='\0';	//ÔÚÄ©Î²¼ÓÈë½áÊø·û. 
		sta=usmart_dev.cmd_rec(USART_RX_BUF);//µÃµ½º¯Êı¸÷¸öĞÅÏ¢
		if(sta==0)usmart_dev.exe();	//Ö´ĞĞº¯Êı 
		else 
		{  
			len=usmart_sys_cmd_exe(USART_RX_BUF);
			if(len!=USMART_FUNCERR)sta=len;
			if(sta)
			{
				switch(sta)
				{
					case USMART_FUNCERR:
						printf("º¯Êı´íÎó!\r\n");   			
						break;	
					case USMART_PARMERR:
						printf("²ÎÊı´íÎó!\r\n");   			
						break;				
					case USMART_PARMOVER:
						printf("²ÎÊıÌ«¶à!\r\n");   			
						break;		
					case USMART_NOFUNCFIND:
						printf("Î´ÕÒµ½Æ¥ÅäµÄº¯Êı!\r\n");   			
						break;		
				}
			}
		}
		USART_RX_STA=0;//×´Ì¬¼Ä´æÆ÷Çå¿Õ	    
	}
}

#if USMART_USE_WRFUNS==1 	//Èç¹ûÊ¹ÄÜÁË¶ÁĞ´²Ù×÷
//¶ÁÈ¡Ö¸¶¨µØÖ·µÄÖµ		 
u32 read_addr(u32 addr)
{
	return *(u32*)addr;//	
}
//ÔÚÖ¸¶¨µØÖ·Ğ´ÈëÖ¸¶¨µÄÖµ		 
void write_addr(u32 addr,u32 val)
{
	*(u32*)addr=val; 	
}
#endif













