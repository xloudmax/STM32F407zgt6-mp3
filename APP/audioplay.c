#include "audioplay.h"
#include "ff.h"
#include "malloc.h"
#include "usart.h"
#include "wm8978.h"
#include "i2s.h"
#include "led.h"
#include "lcd.h"
#include "delay.h"
#include "key.h"
#include "exfuns.h"
#include "text.h"
#include "string.h"
#include "encoder.h"

//////////////////////////////////////////////////////////////////////////////////
// ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//////////////////////////////////////////////////////////////////////////////////

// ���ֲ��ſ�����
__audiodev audiodev;

// ��ʼ��Ƶ����
void audio_start(void)
{
    audiodev.status = 3 << 0; // ��ʼ����+����ͣ
    I2S_Play_Start();
}

// �ر���Ƶ����
void audio_stop(void)
{
    audiodev.status = 0;
    I2S_Play_Stop();
}

// �õ�path·����,Ŀ���ļ����ܸ���
u16 audio_get_tnum(u8 *path)
{
    u8 res;
    u16 rval = 0;
    DIR tdir; // ��ʱĿ¼
    FILINFO tfileinfo; // ��ʱ�ļ���Ϣ
    u8 *fn;

    res = f_opendir(&tdir, (const TCHAR*)path); // ��Ŀ¼
    tfileinfo.lfsize = _MAX_LFN * 2 + 1; // ���ļ�����󳤶�
    tfileinfo.lfname = mymalloc(SRAMIN, tfileinfo.lfsize); // Ϊ���ļ������������ڴ�
    if (res == FR_OK && tfileinfo.lfname != NULL)
    {
        while (1) // ��ѯ�ܵ���Ч�ļ���
        {
            res = f_readdir(&tdir, &tfileinfo); // ��ȡĿ¼�µ�һ���ļ�
            if (res != FR_OK || tfileinfo.fname[0] == 0) break; // ������/��ĩβ��,�˳�
            fn = (u8*)(*tfileinfo.lfname ? tfileinfo.lfname : tfileinfo.fname);
            res = f_typetell(fn);
            if ((res & 0XF0) == 0X40) // ȡ����λ,�����ǲ��������ļ�
            {
                rval++; // ��Ч�ļ�������1
            }
        }
    }
    myfree(SRAMIN, tfileinfo.lfname);
    return rval;
}


int Show_Num_Num(int num)//��ȡ��������λ��
{
	int count = 0;
  while(num>0)
		{
			num /= 10;
			count++;
	}
	return count;
};
// ��ʾ��Ŀ����
void audio_index_show(u16 index, u16 total)
{
	LCD_Fill(30,48,210,64,BLACK);
	Show_Str(30,48,94,16,"Current:",16,0);
	LCD_ShowxNum(94,48,index,Show_Num_Num(index),16,0X80);		//����
	Show_Str(205-8*Show_Num_Num(total)-6*8,48,205-8*Show_Num_Num(total),16,"Total:",16,0);
	LCD_ShowxNum(205-8*Show_Num_Num(total),48,total,Show_Num_Num(total),16,0X80); 	//����Ŀ		

}




// ��ʾ����ʱ�䡢�����ʡ�������EQ��Ϣ
void audio_msg_show(u32 totsec, u32 cursec, u32 bitrate)
{   
    static u16 playtime = 0X0000; // ����ʱ����
	
    

    if (playtime != cursec) // ��Ҫ������ʾʱ��
    {
        playtime = cursec;
      LCD_Fill(30,190+66,32,206+66,WHITE);//��������߿�
			LCD_Fill(32,190+66,205,193+66,WHITE);//�������ϱ߿�
			LCD_Fill(32,203+66,205,206+66,WHITE);//�������±߿�
			LCD_Fill(203,190+66,205,206+66,WHITE);//�������ұ߿�
			LCD_Fill(35,195+66,35+(playtime*165)/totsec,201+66,WHITE); //������ˢ��
			LCD_ShowxNum(30,210+66+8,playtime/60,2,16,0X80);		//����
			LCD_ShowChar(30+16,210+66+8,':',16,0);
			LCD_ShowxNum(30+24,210+66+8,playtime%60,2,16,0X80);	//����		
			//��ʾ��ʱ��    	   
			LCD_ShowxNum(205-40,210+66+8,totsec/60,2,16,0X80);	//����
			LCD_ShowChar(205-24,210+66+8,':',16,0);
			LCD_ShowxNum(205-16,210+66+8,totsec%60,2,16,0X80);	//����	  		    
			//��ʾλ��			   
   		LCD_ShowxNum(30,32,bitrate/1000,4,16,0X80);//��ʾλ��	 
			LCD_ShowString(62,32,200,16,16,"Kbps");	
	
    }
}


// ��������
void audio_play(void)
{
    u8 res;
    DIR wavdir; // Ŀ¼
    FILINFO wavfileinfo; // �ļ���Ϣ
    u8 *fn; // ���ļ���
    u8 *pname; // ��·�����ļ���
    u16 totwavnum; // �����ļ�����
    u16 curindex; // ��ǰ����
    u8 key; // ��ֵ
    u16 temp;
    u16 *wavindextbl; // ����������

    WM8978_ADDA_Cfg(1, 0); // ����DAC
    WM8978_Input_Cfg(0, 0, 0); // �ر�����ͨ��
    WM8978_Output_Cfg(1, 0); // ����DAC���
    Show_Str(30, 16, 32, 16, "EQ:0", 16, 0); // ��ʾ��ǰEQ��λ


    while (f_opendir(&wavdir, "0:/MUSIC")) // �������ļ���
    {
				Show_Str(30,240,240,16,"��MUSIC�ļ��д���!",16,0);
        delay_ms(200);
        LCD_Fill(30, 240, 240, 206, BLACK); // �����ʾ
        delay_ms(200);
    }
    totwavnum = audio_get_tnum("0:/MUSIC"); // �õ�����Ч�ļ���
    while (totwavnum == NULL) // �����ļ�����Ϊ0
    {
        Show_Str(30, 240, 240, 16, "û�������ļ�!", 16, 0);
        delay_ms(200);
        LCD_Fill(30, 240, 240, 146, BLACK); // �����ʾ
        delay_ms(200);
    }
    wavfileinfo.lfsize = _MAX_LFN * 2 + 1; // ���ļ�����󳤶�
    wavfileinfo.lfname = mymalloc(SRAMIN, wavfileinfo.lfsize); // Ϊ���ļ������������ڴ�
    pname = mymalloc(SRAMIN, wavfileinfo.lfsize); // Ϊ��·�����ļ��������ڴ�
    wavindextbl = mymalloc(SRAMIN, 2 * totwavnum); // ����2*totwavnum�ֽڵ��ڴ�,���ڴ�������ļ�����
    while (wavfileinfo.lfname == NULL || pname == NULL || wavindextbl == NULL) // �ڴ�������
    {
        Show_Str(30, 240, 240, 16, "�ڴ����ʧ��!", 16, 0);
        delay_ms(200);
        LCD_Fill(30, 240, 240, 146, BLACK); // �����ʾ
        delay_ms(200);
    }
    // ��¼����
    res = f_opendir(&wavdir, "0:/MUSIC"); // ��Ŀ¼
    if (res == FR_OK)
    {
        curindex = 0; // ��ǰ����Ϊ0
        while (1) // ȫ����ѯһ��
        {
            temp = wavdir.index; // ��¼��ǰindex
            res = f_readdir(&wavdir, &wavfileinfo); // ��ȡĿ¼�µ�һ���ļ�
            if (res != FR_OK || wavfileinfo.fname[0] == 0) break; // ����/��ĩβ,�˳�
            fn = (u8*)(*wavfileinfo.lfname ? wavfileinfo.lfname : wavfileinfo.fname);
            res = f_typetell(fn);
            if ((res & 0XF0) == 0X40) // ����λΪ0X40���ж��Ƿ�Ϊ�����ļ�
            {
                wavindextbl[curindex] = temp; // ��¼����
                curindex++;
            }
        }
    }
    curindex = 0; // ��0��ʼ��ʾ
    res = f_opendir(&wavdir, (const TCHAR*)"0:/MUSIC"); // ��Ŀ¼
    while (res == FR_OK) // �򿪳ɹ�
    {
        dir_sdi(&wavdir, wavindextbl[curindex]); // �ı䵱ǰĿ¼����
        res = f_readdir(&wavdir, &wavfileinfo); // ��ȡĿ¼�µ�һ���ļ�
        if (res != FR_OK || wavfileinfo.fname[0] == 0) break; // ����/��ĩβ,�˳�
        fn = (u8*)(*wavfileinfo.lfname ? wavfileinfo.lfname : wavfileinfo.fname);
        strcpy((char*)pname, "0:/MUSIC/"); // ����·��(Ŀ¼)
        strcat((char*)pname, (const char*)fn); // ���ļ������ں���
        LCD_Fill(30, 240, 240, 240 + 16, BLACK); // ���֮ǰ����ʾ
        Show_Str(30, 240, 240 - 30, 16, fn, 16, 0); // ��ʾ��������
			  LCD_Fill(35,195+66,200,201+66,BLACK);
        audio_index_show(curindex + 1, totwavnum); // ��ʾ����
        key = audio_play_song(pname); // ���������Ƶ�ļ�
        if (key == KEY2_PRES) // ��һ��
        {
            if (curindex) curindex--;
            else curindex = totwavnum - 1;
        }
        else if (key == KEY0_PRES) // ��һ��
        {
            curindex++;
            if (curindex >= totwavnum) curindex = 0; // ��ĩβʱ����ͷ��ʼ
        }				
        else break; // ��������
    }
    myfree(SRAMIN, wavfileinfo.lfname); // �ͷ��ڴ�
    myfree(SRAMIN, pname); // �ͷ��ڴ�
    myfree(SRAMIN, wavindextbl); // �ͷ��ڴ�
}

// ����ĳ����Ƶ�ļ�
u8 audio_play_song(u8* fname)
{
    u8 res;
    res = f_typetell(fname);
    switch (res)
    {
        case T_WAV:
            res = wav_play_song(fname); // ����WAV�ļ�
            break;
        default: // �����ļ����Զ���ת����һ��
            printf("can't play: %s\r\n", fname);
            res = KEY0_PRES;
            break;
    }
    return res;
}
