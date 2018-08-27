#define DELLABEL
#include "AllHead.h"

#define DEL10MSLEN		0x0A
#define DEL_IDLE		0x00
#define DEL_RUN			0x01



u16 SignalPoorCount=0;
u8 WriteFreqTimeCount=0;
u8 *ucGPSSendToAtPort   ="AT+GPSFUNC=21";
u8 *ucGPSUploadTime_5s  ="AT+GPSFUNC=1";
u8 *ucRequestUserListInfo       = "0E000000000001";

u8 ShowTime_Flag=FALSE;
u8 DEL_500ms_Count=0;
u8 DEL_500ms_Count2=0;
u16 TimeCount=0;
u8 TimeCount2=0;
u16 TimeCount3=0;
u8 TimeCount_Light=0;
u8 ToneTimeCount=0;
u8 GpsReconnectionTimeCount=0;
u8 PowerOnCount=0;
u8 CSQTimeCount=0;
u8 LandingTimeCount=0;
u8 PrimaryLowPowerCount=0;
u8 ForbiddenSendPttCount=0;
u8 EnterKeyTimeCount=0;
u8 UpDownSwitchingCount=0;
u8 TaskStartDeleteDelay1Count=0;
u8 TaskStartDeleteDelay3Count=0;
u8 TaskStartDeleteDelay4Count=0;
u8 TaskStartDeleteDelay6Count=0;
u8 ApiAtCmd_TrumpetVoicePlayCount=0;
u8 POC_ReceivedVoiceCount=0;
u8 LobatteryTask_StartCount=0;
u8 PocNoOnlineMemberCount=0;
u8 GetNoOnlineMembersCount=0;
u8 POC_GetAllGroupNameDoneCount=0;
u8 ApiAtCmd_ZTTSCount=0;
u8 ShowTimeCount=0;
u8 huo_qu_zhong_flag_count=0;
u8 key_warning_flag_count=0;
u8 key_warning_bubiao_flag_count=0;
//u8 UpgradeNoATReturn_Count=0;
bool LockingState_Flag=FALSE;
u8 BacklightTimeCount;//=10;//�����ʱ��(��Ҫ���ý���eeprom)
u16 KeylockTimeCount;//=30;//������ʱ��(��Ҫ���ý���eeprom)
u8 PersonalCallingCount=0;
u8 KEY_4Count=0;
u8 get_group_name_done_count=0;
u8 get_group_list_loss_flag_count=0;
u8 allow_key_operation_flag=FALSE;
u8 ReadBufferA[1];//�����ʱ��(��Ҫ���ý���eeprom)
u8 ReadBufferB[1];//������ʱ��(��Ҫ���ý���eeprom)
typedef struct {
  union {
    struct {
      u16 b1ms  	: 1;
      u16 b10ms 	: 1;
      u16 b100ms	: 1;
      u16 b500ms	: 1;
      u16 b1S		: 1;
      u16 bTimeSet	: 1;
      u16 bTime0	: 1;
      u16 bTime1	: 1;
      u16 b500Alternate : 1;
      u16		: 7;
    }Bit;
    u16 Byte;
  }Msg;
  u8   c10msLen;
  u8   c100msLen;
  u8   c500msLen;
  u8   c1SLen;
  u8   c2SLen;
  u16  iTimer0;
  u16  iTimer1;
}DEL_DRV;

static DEL_DRV	DelDrvObj;
static void DEL_100msProcess(void);
static void DEL_500msProcess(void);
static void DEL_1msProcess(void);
static void DEL_10msProcess(void);
static void DEL_TimerRenew(void);

void DEL_PowerOnInitial(void)//ԭ������Ƭ�C���L�r�g�Mһ���Д�
{
  Tim3_Timer_Init();
  C_TEST_OUT_SET();
  DelDrvObj.Msg.Byte 	= 0x00;
  DelDrvObj.c10msLen  = DEL10MSLEN;
  DelDrvObj.c100msLen = 0x0A;
  DelDrvObj.c500msLen = 0x05;
  DelDrvObj.c1SLen    = 0x01;
  DelDrvObj.c2SLen    = 0x02;
  return;
}



void DEL_Interrupt(void)
{
  
  DelDrvObj.c10msLen--;
  DelDrvObj.Msg.Bit.b1ms = DEL_RUN;
  //enableInterrupts();
  if (DelDrvObj.c10msLen == 0x00) //10ms interrupt process
  {
    DEL_TimerRenew();//delay timer renew process
    DelDrvObj.Msg.Bit.b10ms = DEL_RUN;
    DelDrvObj.c10msLen = DEL10MSLEN;
    DelDrvObj.c100msLen--;
    if (DelDrvObj.c100msLen == 0x00) //100ms interrupt process
    {
      DelDrvObj.Msg.Bit.b100ms = DEL_RUN;
      DelDrvObj.c100msLen = 0x0A;
      DelDrvObj.c500msLen--;
      if (DelDrvObj.c500msLen == 0x00) //500ms interrupt process
      {	
        DelDrvObj.Msg.Bit.b500ms = DEL_RUN;
        DelDrvObj.c500msLen = 0x05;
        DelDrvObj.c1SLen--;
        if (DelDrvObj.c1SLen == 0x00) //1s interrupt process
        {
          DelDrvObj.Msg.Bit.b1S = DEL_RUN;
          DelDrvObj.c1SLen = 0x02;	
          DelDrvObj.c2SLen--;
          if (DelDrvObj.c2SLen == 0x00)	//2s interrupt process
          {
            //DelDrvObj.Msg.Bit.b2S = DEL_RUN;
            DelDrvObj.c2SLen = 0x02;
          }
        }
      }
    }
  }
  
  return;
}

void DEL_Renew(void) 
{
  DEL_1msProcess();
  DEL_10msProcess();
  DEL_100msProcess();
  DEL_500msProcess();
  return;
}

void DEL_Soft1ms(u16 iLen) 
{
  u16 i;
  for(; iLen > 0; iLen--)
  {
    for( i = 0 ; i < 250; i++)
    {
      nop();
      nop();
      nop();
      nop();
      nop();
    }
  }
  return;
}

bool DEL_SetTimer(u8 cId,u16 iLen)
{
  DelDrvObj.Msg.Bit.bTimeSet = 0x01;
  switch (cId)
  {
  case 0x00:
    if(iLen == 0x00)
    {
      DelDrvObj.Msg.Bit.bTime0 = DEL_IDLE;
    }
    else
    {
      DelDrvObj.Msg.Bit.bTime0 = DEL_RUN;
    }
    DelDrvObj.iTimer0 = iLen;
    break;
  case 0x01:
    if(iLen == 0x00)
    {
      DelDrvObj.Msg.Bit.bTime1 = DEL_IDLE;
    }
    else
    {
      DelDrvObj.Msg.Bit.bTime1 = DEL_RUN;
    }
    DelDrvObj.iTimer1 = iLen;
    break;
  default:
    return FALSE;
  }
  DelDrvObj.Msg.Bit.bTimeSet = 0x00;
  return TRUE;
}

bool DEL_GetTimer(u8 cId)
{
	bool r;

	r = FALSE;
	//1
        switch (cId)
	{
	case 0x00:
		if ((DelDrvObj.Msg.Bit.bTime0 == DEL_RUN) && (DelDrvObj.iTimer0 == 0x00))
		{
			r = TRUE;
			DelDrvObj.Msg.Bit.bTime0 = DEL_IDLE;
		}
		break;
	case 0x01:
		if ((DelDrvObj.Msg.Bit.bTime1 == DEL_RUN) && (DelDrvObj.iTimer1 == 0x00))
		{
			r = TRUE;
			DelDrvObj.Msg.Bit.bTime1 = DEL_IDLE;			
		}
		break;
	default:
		break;
	}
	return r;
}

static void DEL_TimerRenew(void)
{
  if(DelDrvObj.Msg.Bit.bTimeSet == 0x00)
  {
    if (DelDrvObj.iTimer0 != 0x00)
    {
      DelDrvObj.iTimer0--;
    }
    if (DelDrvObj.iTimer1 != 0x00)
    {
      DelDrvObj.iTimer1--;
    }
  }
  return;
}

static void DEL_100msProcess(void)
{
  if (DelDrvObj.Msg.Bit.b100ms == DEL_RUN)
  {
    DelDrvObj.Msg.Bit.b100ms = DEL_IDLE;
    LED_IntOutputRenew();//LED output renew process
#ifdef BEIDOU
    ApiBeidou_Get_location_Information();
#else
    ApiAtCmd_Get_location_Information();
    if(xinhao_test_flag==0xFF)
    {
      sou_xing_count_for_display();
    }
#endif
    ApiAtCmd_Get_DateTime_Information();
    ApiGpsCmd_100msRenew();
    if(DelDrvObj.Msg.Bit.b500Alternate == DEL_IDLE)
    {
      DelDrvObj.Msg.Bit.b500Alternate = DEL_RUN;
      ApiAtCmd_100msRenew();
    }
    else
    {
      DelDrvObj.Msg.Bit.b500Alternate = DEL_IDLE;
      //ApiPocCmd_1sRenew();
    }
  }
  return;
}


static void DEL_500msProcess(void)			//delay 500ms process server
{
  u8 i;
  u8 ShowTimeBuf1[6]={0,0,0,0,0,0};
  if (DelDrvObj.Msg.Bit.b500ms == DEL_RUN) 
  {
    DelDrvObj.Msg.Bit.b500ms = DEL_IDLE;
#if 1//�����ж�����
    VOICE_1sProcess();
    DEL_500ms_Count++;
    DEL_500ms_Count2++;
    TimeCount_Light++;
    CSQTimeCount++;
/******��������־λ��ʱ����ʾʹ��**********/
    if(KEY_4_Flag==TRUE)
    {
      KEY_4Count++;
      if(KEY_4Count>2*2)
      {
        KEY_4Count=0;
        KEY_4_Flag=FALSE;
      }
    }
    else
    {
      KEY_4Count=0;
    }
/******����Ⱥ��ģʽ5����ʾʱ��*******************/
    if(xinhao_test_flag==0xFF)
    {
      
    }
    else
    {
      if(POC_GetAllGroupNameDone_Flag==TRUE&&
         MenuMode_Flag==0&&
         POC_EnterPersonalCalling_Flag==0&&
         POC_QuitPersonalCalling_Flag==0&&
         POC_AtEnterPersonalCalling_Flag==0&&
         POC_AtQuitPersonalCalling_Flag==0&&
         KEY_4_Flag==FALSE&&
         KeyDownUpChoose_GroupOrUser_Flag==0)
      {
        ShowTimeCount++;
        if(ShowTimeCount>2*5)
        {
          ShowTime_Flag=TRUE;
          ShowTimeCount=11;
          if(Data_Time0()<=0x09&&Data_Time1()<=0x09)
          {
            ShowTimeBuf1[0]='0';
            COML_HexToAsc(Data_Time0(),ShowTimeBuf1+1);
            ShowTimeBuf1[2]=':';
            ShowTimeBuf1[3]='0';
            COML_HexToAsc(Data_Time1(),ShowTimeBuf1+4);
          }
          else if(Data_Time0()<=0x09&&Data_Time1()>0x09)
          {
            ShowTimeBuf1[0]='0';
            COML_HexToAsc(Data_Time0(),ShowTimeBuf1+1);
            ShowTimeBuf1[2]=':';
            COML_HexToAsc(Data_Time1(),ShowTimeBuf1+3);
            COML_StringReverse(2,ShowTimeBuf1+3);
          }
          else if(Data_Time0()>0x09&&Data_Time1()<=0x09)
          {
            COML_HexToAsc(Data_Time0(),ShowTimeBuf1);
            COML_StringReverse(2,ShowTimeBuf1);
            ShowTimeBuf1[2]=':';
            ShowTimeBuf1[3]='0';
            COML_HexToAsc(Data_Time1(),ShowTimeBuf1+4);
          }
          else//
          {
            COML_HexToAsc(Data_Time0(),ShowTimeBuf1);
            COML_StringReverse(2,ShowTimeBuf1);
            ShowTimeBuf1[2]=':';
            COML_HexToAsc(Data_Time1(),ShowTimeBuf1+3);
            COML_StringReverse(2,ShowTimeBuf1+3);
          }
          ShowTimeBuf1[5]='\0';
          api_disp_icoid_output( eICO_IDSCANOff, TRUE, TRUE);//
          api_lcd_pwr_on_hint7(ShowTimeBuf1);
        }
      }
      else
      {
        ShowTime_Flag=FALSE;
        ShowTimeCount=0;
        if(xinhao_test_flag==0xFF)
        {
        }
        else
        {
          if(MenuMode_Flag!=1)
          {
            if(NetworkType_2Gor3G_Flag==3)
              api_disp_icoid_output( eICO_IDEmergency, TRUE, TRUE);//3Gͼ��
            else
              api_disp_icoid_output( eICO_IDPOWERL, TRUE, TRUE);//ͼ�꣺2G
            if(VoiceType_FreehandOrHandset_Flag==0)
              api_disp_icoid_output( eICO_IDTemper, TRUE, TRUE);//����ģʽ
            else
              api_disp_icoid_output( eICO_IDMONITER, TRUE, TRUE);//��Ͳģʽͼ��
            if(PersonCallIco_Flag==0)
              api_disp_icoid_output( eICO_IDPOWERM, TRUE, TRUE);//��ʾ���ͼ��
            else
              api_disp_icoid_output( eICO_IDPOWERH, TRUE, TRUE);//��ʾ����ͼ��
            if(KeyDownUpChoose_GroupOrUser_Flag==0)
              api_disp_icoid_output( eICO_IDMESSAGEOff, TRUE, TRUE);//��ͼ��-��ѡ��Ӧ
            else
              api_disp_icoid_output( eICO_IDLOCKED, TRUE, TRUE);//ѡ
          }
        }
      }
    }

    
/*****5�����ȿ�����ر�����**************/
    if(ApiAtCmd_ZTTS_Flag==TRUE)
    {
      ApiAtCmd_ZTTSCount++;
      if(ApiAtCmd_ZTTSCount>2*15)
      {
        ApiAtCmd_ZTTS_Flag=FALSE;
        ApiAtCmd_ZTTSCount=0;
      }
    }
    else
    {
      ApiAtCmd_ZTTSCount=0;
    }

/**********������������(2*key_top_value)��ȡ����־λ�������ϱ���־10sȡ����־λ**********************************/
    if(key_warning_flag==TRUE)
    {
      key_warning_flag_count++;
      if(key_top_value==11)//�����ֵΪ11���򱨾���Ϊ���ñ���
      {
        key_warning_flag_count=0;
      }
      else
      {
        if(key_warning_flag_count>=2*(2*key_top_value))
        {
          key_warning_flag_count=0;
          key_warning_flag=FALSE;
        }
      }
    }
    else
    {
      key_warning_flag_count=0;
    }
    
    if(key_warning_bubiao_flag==TRUE)
    {
      key_warning_bubiao_flag_count++;
      if(key_top_value==11)//�����ֵΪ0x11���򱨾���Ϊ���ñ���
      {
        key_warning_bubiao_flag_count=0;
      }
      else
      {
        if(key_warning_bubiao_flag_count>=2*15)
        {
          key_warning_bubiao_flag=FALSE;
          key_warning_bubiao_flag_count=0;
        }
      }
    }
/*******��ȡȺ������ʧ����*******************/
    if(POC_GetAllGroupNameDone_Flag==TRUE&&get_group_list_loss_flag==TRUE)
    {
     get_group_list_loss_flag_count++;
     if(get_group_list_loss_flag_count>=2*3)//��ȡȺ�鶪ʧ��־λ����2s
     {
       get_group_list_loss_flag_count=0;
       ApiPocCmd_WritCommand(PocComm_GroupListInfo,0,0);
     }
    }
    else
    {
      get_group_list_loss_flag_count=0;
    }
/******�����ȡ����Ⱥ����ʱȺ�鶪ʧ������************/
    if(POC_GetAllGroupNameDone_Flag==TRUE)
    {
      get_group_name_done_count++;
      if(get_group_name_done_count>=2*5)
      {
        allow_key_operation_flag = TRUE;
        get_group_name_done_count=11;
      }
    }
    else
    {
      get_group_name_done_count=0;
    }
/*********��������δ��ȡ�����߳�Ա����ʱ�˻����ģʽ*************************/
    if(Key_PersonalCalling_Flag==1&&GettheOnlineMembersDone==FALSE)
    {
      GetNoOnlineMembersCount++;
      if(GetNoOnlineMembersCount>2*7)
      {
#if 1
        ApiPocCmd_WritCommand(PocComm_UserListInfo,0,0);
#else
        
        GetNoOnlineMembersCount=0;
        Key_PersonalCalling_Flag=0;//���������־λ
        api_disp_icoid_output( eICO_IDMESSAGEOff, TRUE, TRUE);//��ͼ��-��ѡ��Ӧ
        api_lcd_pwr_on_hint("                ");//����
        api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
        MenuMode_Flag=0;
        //����PTT�������¼�����Ĭ��״̬
        KeyUpDownCount=0;
        PersonalCallingNum=0;
        huo_qu_zhong_flag=FALSE;//δ��ѯ�����Ա���˱�־λ����
        KeyDownUpChoose_GroupOrUser_Flag=0;//����������������ؼ���OK��PTT����Ļ��ʾ�����BUG
        ApiPocCmd_PersonalCallingMode=FALSE;//���Ⱥ���Ա��ȡ��ȫʱ���ص����״̬����ok����Ч������
#endif
      }
    }
    else
    {
      if(GettheOnlineMembersDone==TRUE)
      {
        GetNoOnlineMembersCount=0;
      }
    }
/*�˵����߳�Ա�б�ģʽ�£���ʾ��ѯ�У�δ��ȡ���������û��б���ϢӦ�ȴ��ٴλ�ȡ*/
    if(huo_qu_zhong_flag==TRUE||get_online_user_list_num_flag==TRUE)
    {
      huo_qu_zhong_flag_count++;
      if(huo_qu_zhong_flag_count>2*3)
      {
        ApiPocCmd_WritCommand(PocComm_UserListInfo,0,0);
        huo_qu_zhong_flag_count=0;
      }
    }
    else
    {
      huo_qu_zhong_flag_count=0;
    }

/******��¼״̬�µĵ͵籨��**********************************************/
    if(LobatteryTask_StartFlag==TRUE)
    {
      LobatteryTask_StartCount++;
      if(LobatteryTask_StartCount==1)
      {
        VOICE_SetOutput(ATVOICE_FreePlay,"f78b45513575",12);//����������
      }
      if(LobatteryTask_StartCount>2*5)
      {
        LobatteryTask_StartCount=0;
        LobatteryTask_StartFlag=FALSE;
      }
    }
/**********����������쳣�������⣬�����̵�****************************/
    if(POC_ReceivedVoice_Flag==TRUE)
    {
      POC_ReceivedVoiceCount++;
      if(POC_ReceivedVoiceCount>2*30)
      {
        POC_ReceivedVoiceCount=0;
        POC_ReceivedVoice_Flag=FALSE;
        POC_ReceivedVoiceEnd_Flag=2;//0:���� 1���յ����� 2���ս�������
        POC_ReceivedVoiceEndForXTSF_Flag=2;
        POC_ReceivedVoiceStart_Flag=0;//0:���� 1���յ����� 2���տ�ʼ����
      }
    }
    else
    {
      POC_ReceivedVoiceCount=0;
    }
/*********�ܵ�������ָ���ӳ�����ر�******************************************/
#if 0
    //if(GetTaskId()==Task_NormalOperation)
    {
      if(ApiAtCmd_TrumpetVoicePlay_Flag==2)
      {
        ApiAtCmd_TrumpetVoicePlayCount++;
        if(ApiAtCmd_TrumpetVoicePlayCount>2*2)//ԭ��2s�����ڸ�Ϊ0.5s
        {
          ApiAtCmd_TrumpetVoicePlay_Flag=0;
          ApiAtCmd_TrumpetVoicePlayCount=0;
          AUDIO_IOAFPOW(OFF);
        }
      }
    }
#endif
/*******��ʼ��ȥ��ʱ�ö�ʱ**************************/
    if(TaskStartDeleteDelay1==2)//������Ǣ����Խ�
    {
      TaskStartDeleteDelay1Count++;
      if(TaskStartDeleteDelay1Count>=6)
      {
        TaskStartDeleteDelay1Count=0;
        TaskStartDeleteDelay2=1;
      }
    }
    if(TaskStartDeleteDelay3==2)//�첻����
    {
      TaskStartDeleteDelay3Count++;
      if(TaskStartDeleteDelay3Count>=2*10)
      {
        TaskStartDeleteDelay3Count=0;
        TaskStartDeleteDelay3=1;
      }
    }
    if(TaskStartDeleteDelay4==2)//�����˺���Ϣ
    {
      TaskStartDeleteDelay4Count++;
      if(TaskStartDeleteDelay4Count>=2*4)
      {
        TaskStartDeleteDelay4Count=0;
        TaskStartDeleteDelay5=1;
      }
    }
    if(TaskStartDeleteDelay6==0)
    {
      TaskStartDeleteDelay6Count++;
      if(TaskStartDeleteDelay6Count>=2*6)//6�벦һ����������
      {
        TaskStartDeleteDelay6Count=0;
        TaskStartDeleteDelay6=1;
      }
    }
/*******������з���һ�κ��У������������̵Ƶ�����**********************************************/
#if 0//ȥ��
    if(POC_ReceivedNoVoice_Flag==TRUE)
    {
      POC_ReceivedNoVoiceCount++;
      if(POC_ReceivedNoVoiceCount>=2)
      {
        POC_ReceivedNoVoiceCount=2;
        Set_GreenLed(LED_ON);
        api_disp_icoid_output( eICO_IDVOX, TRUE, TRUE);//�����ź�ͼ��
        api_disp_all_screen_refresh();// ȫ��ͳһˢ��
      }
    }
    else
    {
      POC_ReceivedNoVoiceCount=0;
    }
#endif
/*****************************************************/
#if 0
    if(UpDownSwitching_Flag==TRUE)
    {
      UpDownSwitchingCount++;
      if(UpDownSwitchingCount>1)
      {
        UpDownSwitchingCount=0;
        UpDownSwitching_Flag=FALSE;
        AUDIO_IOAFPOW(OFF);
      }
    }
#endif
/*****����дƵ״̬5s��дƵ��־λ����****************/
    if(WriteFreq_Flag==TRUE)
    {
      if(WriteFreqTimeCount>=10)
      {
        WriteFreq_Flag=FALSE;
        WriteFreqTimeCount=0;
      }
      WriteFreqTimeCount++;
    }
/*********������������30s����һ��********************************/
    if(PrimaryLowPower_Flag==TRUE)
    {
      PrimaryLowPowerCount++;
      if(PrimaryLowPowerCount>=2*30)
      {
        PrimaryLowPowerCount=0;
        VOICE_SetOutput(ATVOICE_FreePlay,"3575606c3575cf914e4f0cfff78b45513575",36);//������ص���������
        PrimaryLowPower_Flag=FALSE;
      }
    }
/*********��¼����60s����*********************************/
    if(Task_Landing_Flag==TRUE)
    {
      LandingTimeCount++;
      if(LandingTimeCount>=2*60)
      {
        LandingTimeCount=0;
        Task_Landing_Flag=FALSE;
        ApiAtCmd_WritCommand(ATCOMM3_GD83Reset,(void*)0, 0);
      }
    }
    else
    {
      LandingTimeCount=0;
    }
/*********��ʱ5s��һ��[AT+CSQ?]*************************************************/
    //if(KaiJi_Flag==TRUE)
    //{
      if(CSQTimeCount>=2*2)
      {
          CSQTimeCount=0;
          if(NetworkType_2Gor3G_Flag==3)//�����3G����HDRCSQ��2G����CSQ
          {
            if(BootProcess_SIMST_Flag!=2)
            {
              ApiAtCmd_WritCommand(ATCOMM15_HDRCSQ, (void*)0, 0);
            }  
          }
          else
          {
            if(NetworkType_2Gor3G_Flag==2)
            {
              ApiAtCmd_WritCommand(ATCOMM6_CSQ, (void*)0, 0);
            }
          }
          
          if(xinhao_test_flag==0xFF)
          {
            csq_value_for_display();
          }
          else
          {
            HDRCSQSignalIcons();
          }
          
      }
   // }

/******************************************************************************/
    if(GetTaskId()==Task_NormalOperation)
    {
      if(HDRCSQValue<=28)
      {
        SignalPoorCount++;
        if(SignalPoorCount==20*2||SignalPoorCount==40*2)
        {
          //���������ź���
          //VOICE_SetOutput(ATVOICE_FreePlay,"517fdc7ee14ff753315f",20);
        }
        if(SignalPoorCount>=120*2)//���ź���ʮ�룬60s����һ��
        {
          ApiAtCmd_WritCommand(ATCOMM3_GD83Reset,(void*)0, 0);
          SignalPoorCount=0;
        }
      }
      else
      {
        SignalPoorCount=0;
      }
    }

/***********************************************************/
    if(ApiAtCmd_GetLoginState()==TRUE)//��¼�ɹ�
      {
        GpsReconnectionTimeCount++;
#ifdef BEIDOU
        PowerOnCount++;
        if(PowerOnCount>=2*60)//������ʱ����1min�����ڱ���ģʽ
        {
          GPIO_WriteLow(GPIOB,GPIO_PIN_3);//NFC
          GPIO_WriteHigh(GPIOB,GPIO_PIN_4);//����
          PowerOnCount=2*60;
        }
#endif
        if(GpsReconnectionTimeCount==2*15)
        {
          //ApiPocCmd_WritCommand(PocComm_UserListInfo,ucRequestUserListInfo,strlen((char const *)ucRequestUserListInfo));
          switch(ApiGpsServerType)
          {
          case GpsServerType_BuBiao:
            NoUseNum=ApiAtCmd_WritCommand(ATCOMM5_CODECCTL,(u8 *)ucGPSUploadTime_5s,strlen((char const *)ucGPSUploadTime_5s));//����GPS��λ��Ϣ5s����һ��
            break;
          case GpsServerType_ZTE:
            break;
          case GpsServerType_BuBiaoAndZTE:
            break;
          }
          GpsReconnectionTimeCount=31;
        }
        if(GpsReconnectionTimeCount>=35)
        {GpsReconnectionTimeCount=31;}
      }
    if(ApiPocCmd_Tone_Flag==TRUE)
    {
      ToneTimeCount++;
      if(ToneTimeCount>1)
      {
        ApiPocCmd_Tone_Flag=FALSE;
        ToneTimeCount=0;
      }
    }
    else
    {
      ToneTimeCount=0;
    }
    
    FILE_Read(0x236,1,ReadBufferA);//����ʱ�䡾�롿
    FILE_Read(0x247,1,ReadBufferB);//������ʱ�䡾�롿
    BacklightTimeCount=5*ReadBufferA[0];
    if(ReadBufferB[0]==0)
      KeylockTimeCount=200*2;//���=200����Զ������
    else
      KeylockTimeCount=5*ReadBufferB[0];
    if(TimeCount_Light>=2*BacklightTimeCount)//10s
    {
      MCU_LCD_BACKLIGTH(OFF);//�رձ����
      TimeCount_Light=2*BacklightTimeCount;
    }
    else
    {
      MCU_LCD_BACKLIGTH(ON);//�򿪱����
    }
    if(NumberKeyboardPressDown_flag==TRUE||LockingState_EnterOK_Flag==TRUE)
    {
      TimeCount_Light=0;//����Ƽ���������
      //NumberKeyboardPressDown_flag=FALSE;
    }
    
    if(DEL_500ms_Count2>=10)
    {
      switch(ApiGpsServerType)
      {
      case GpsServerType_BuBiao:
        if(BootProcess_SIMST_Flag!=2)
        {
          ApiAtCmd_WritCommand(ATCOMM5_CODECCTL,(u8 *)"AT^CDMATIME",strlen((char const *)"AT^CDMATIME"));//���ͻ�ȡCDMATIME��ȡʱ��
          //ÿ��5�뷢��һ��gpsinfo��ȡ�ٶ�
          ApiAtCmd_WritCommand(ATCOMM5_CODECCTL,(u8 *)"AT^GPSINFO",strlen((char const *)"AT^GPSINFO"));//���ͻ�ȡCDMATIME��ȡʱ��
          ApiAtCmd_WritCommand(ATCOMM5_CODECCTL,(u8 *)"AT^GPSCNO",strlen((char const *)"AT^GPSCNO"));//���ͻ�ȡCDMATIME��ȡʱ��
          //UpgradeNoATReturn_Flag=TRUE;
        }  
        break;
      case GpsServerType_ZTE:
          ApiAtCmd_WritCommand(ATCOMM5_CODECCTL,(u8 *)"AT^CDMATIME",strlen((char const *)"AT^CDMATIME"));//���ͻ�ȡCDMATIME��ȡʱ��
          //ÿ��5�뷢��һ��gpsinfo��ȡ�ٶ�
          ApiAtCmd_WritCommand(ATCOMM5_CODECCTL,(u8 *)"AT^GPSINFO",strlen((char const *)"AT^GPSINFO"));//���ͻ�ȡCDMATIME��ȡʱ��
          ApiAtCmd_WritCommand(ATCOMM5_CODECCTL,(u8 *)"AT^GPSCNO",strlen((char const *)"AT^GPSCNO"));//���ͻ�ȡCDMATIME��ȡʱ��
        break;
      case GpsServerType_BuBiaoAndZTE:
        if(BootProcess_SIMST_Flag!=2)
        {
          ApiAtCmd_WritCommand(ATCOMM5_CODECCTL,(u8 *)"AT^CDMATIME",strlen((char const *)"AT^CDMATIME"));//���ͻ�ȡCDMATIME��ȡʱ��
          //ÿ��5�뷢��һ��gpsinfo��ȡ�ٶ�
          ApiAtCmd_WritCommand(ATCOMM5_CODECCTL,(u8 *)"AT^GPSINFO",strlen((char const *)"AT^GPSINFO"));//���ͻ�ȡCDMATIME��ȡʱ��
          ApiAtCmd_WritCommand(ATCOMM5_CODECCTL,(u8 *)"AT^GPSCNO",strlen((char const *)"AT^GPSCNO"));//���ͻ�ȡCDMATIME��ȡʱ��
        } 
        break;
      }
      DEL_500ms_Count2=0;
    }
      if(GetTaskId()==Task_NormalOperation)
      {
        if(KeylockTimeCount==200*2)
        {
          TimeCount=0;
        }
        else
        {
        TimeCount++;

        if(TimeCount>=KeylockTimeCount*2) //��ʱ������
        {
          if(TimeCount==KeylockTimeCount*2)
          {
            LockingState_Flag=TRUE;//��ʱ������־λ
            //���BUG���������Ӱ��һ�������˵���ʾ���ִ����취Ϊ�������˻�Ĭ��Ⱥ��״̬,���в˵���־λ��ʼ��
            //api_lcd_pwr_on_hint3("                ");//����
            MenuDisplay(Menu_RefreshAllIco);
            if(PersonCallIco_Flag==0)
            {
              api_lcd_pwr_on_hint("                ");//����
              //api_lcd_pwr_on_hint(HexToChar_MainGroupId());//��ʾ��ǰȺ��ID
              api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
              api_disp_all_screen_refresh();// ȫ��ͳһˢ��
            }
            else
            {
              api_lcd_pwr_on_hint("                ");//����
              if(UnicodeForGbk_MainUserName_english_flag()==TRUE)
              {
                api_lcd_pwr_on_hint(UnicodeForGbk_MainUserName());//��ʾ��ǰ�û��ǳ�
              }
              else
              {
                api_lcd_pwr_on_hint4(UnicodeForGbk_MainUserName());//��ʾ��ǰ�û��ǳ�
              }
              api_disp_all_screen_refresh();// ȫ��ͳһˢ��
            }

            MenuModeCount=1;
            TheMenuLayer_Flag=0;
            MenuMode_Flag=0;
            ApiMenu_SwitchGroup_Flag=0;
            ApiMenu_SwitchCallUser_Flag=0;
            ApiMenu_SwitchOnlineUser_Flag=0;
            ApiMenu_GpsInfo_Flag=0;
            ApiMenu_BacklightTimeSet_Flag=0;
            ApiMenu_KeylockTimeSet_Flag=0;
            ApiMenu_NativeInfo_Flag=0;
            ApiMenu_BeiDouOrWritingFrequency_Flag=0;
          }
          TimeCount=KeylockTimeCount*2+1;
        }
        else
        {
          //MCU_LCD_BACKLIGTH(ON);//�򿪱����
        }
        if(NumberKeyboardPressDown_flag==TRUE&&TimeCount>=KeylockTimeCount*2)//����10����ٰ�������ʾ����OK���ٰ�*����
        {
          TimeCount2++;
          api_lcd_pwr_on_hint("��OK��,�ٰ�#��  ");//
          if(TimeCount2>=2)//0.5s
          {
            TimeCount2=0;
            NumberKeyboardPressDown_flag=FALSE;
            MenuDisplay(Menu_Locking_NoOperation);
          }
        }
        if(LockingState_EnterOK_Flag==TRUE)//�������水��OK��
        {                                                           
          TimeCount3++;//�����ɹ�ҲӦ�����㣬
          //MCU_LCD_BACKLIGTH(ON);//�򿪱����
          if(TimeCount3>=4*2)//3s
          {
            TimeCount3=0;
            //MCU_LCD_BACKLIGTH(OFF);//�رձ����
            LockingState_EnterOK_Flag=FALSE;
            MenuDisplay(Menu_Locking_NoOperation);
          }
        }
      }
      if(NumberKeyboardPressDown_flag==TRUE&&TimeCount<KeylockTimeCount*2)//���������ּ��̰���
      {
        TimeCount=0;//���а������£�����������
        NumberKeyboardPressDown_flag=FALSE;
      }
      }
#endif
  }
  return;
}


static void DEL_1msProcess(void)
{
  if (DelDrvObj.Msg.Bit.b1ms == DEL_RUN)
  {
    DelDrvObj.Msg.Bit.b1ms = DEL_IDLE;
    //ApiPocCmd_83_1msRenew();
    ApiPocCmd_10msRenew();
    ApiCaretCmd_10msRenew();
    ApiAtCmd_10msRenew();
    
  }
  return;
}

static void DEL_10msProcess(void)
{
  if (DelDrvObj.Msg.Bit.b10ms == DEL_RUN) 
  {
    DelDrvObj.Msg.Bit.b10ms = DEL_IDLE;
    ApGpsCmd_10msRenew();
  }
  return;
}