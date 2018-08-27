#include "AllHead.h"

#define KeyCountNum 50//���ּ�������-��������¼�������ּ�����
u8 AkeyvolumeCount=7;
u8 *ucVGR1                       = "AT+VGR=1";//��������1
u8 *ucVGR7                       = "AT+VGR=7";//��������Ĭ��7
u8 *ucKeyUp                = "10000003";
u8 *ucKeyDown              = "10000004";
u8 *ucQuitPersonalCalling  = "0A0000ffffffff";
s8 GroupCallingNum=1;
s8 PersonalCallingNum=0;
s8 KeyUpDownCount=0;//������¼�����
s8 KeyPersonalCallingCount=0;//�������¼�����
u32 get_key_value(u8 scan_value);
u8 Key_Flag_1=0;
u8 MenuMode_Flag=0;
u8 MenuModeCount=1;
u8 BacklightTimeSetCount=1;//Ĭ�Ͻ�ѡ����1
u8 KeylockTimeSetCount=0x11;//Ĭ�Ͻ�ѡ����1
u8 TheMenuLayer_Flag=0;//�����˵��㼶��Ĭ��״̬��1 һ���˵���1 �����˵���2
bool NumberKeyboardPressDown_flag=FALSE;
bool LockingState_EnterOK_Flag=FALSE;
u8 VoiceType_FreehandOrHandset_Flag=0;
u8 TestNum1,TestNum2,TestNum3,TestNum4,TestNum5,TestNum6;
u8 TestNum7,TestNum8,TestNum9,TestNum10,TestNum11,TestNum12;
u8 TestBuf1[6];//������ʾ���̺ź���ʹ��
  u8 num1=0;//������ʾ���̺ź���ʹ��
  u8 num2=0;//������ʾ���̺ź���ʹ��
  u8 num3=0;//������ʾ���̺ź���ʹ��
  u8 num4=0;//������ʾ���̺ź���ʹ��
  u8 num5=0;//������ʾ���̺ź���ʹ��
  u8 num6=0;//������ʾ���̺ź���ʹ��
  u8 numCount=0;//������ʾ���̺ź���ʹ��
bool PressButton;//���Զ̺Ź���ʹ��
bool KeyBoardState;//���Զ̺Ź���ʹ��
bool UpDownSwitching_Flag=FALSE;
bool get_online_user_list_num_flag=FALSE;
bool get_online_user_list_num_for_key_flag=FALSE;
bool huo_qu_zhong_flag=FALSE;
u8 xinhao_test_flag=0;
static void GeHuTest(u32 KeyID);

void Keyboard_Test(void)
{
  u8 scanvalue = 0;
  u32 ulAllKeyID = 0x00000000;

  scanvalue = drv_keypad_scan();
  ulAllKeyID = get_key_value(scanvalue);
  switch(ulAllKeyID)
  {
  case 0x00000002://1
    TestNum1++;
    if(TestNum1>=KeyCountNum)
    {
      TestNum1=0;
    NumberKeyboardPressDown_flag=TRUE;
    }
    break; 
  case 0x00000008://3
    TestNum2++;
    if(TestNum2>=KeyCountNum)
    {
      TestNum2=0;
    NumberKeyboardPressDown_flag=TRUE;
    }
    break;
  case 0x00000080://4
    NumberKeyboardPressDown_flag=TRUE;
    if(xinhao_test_flag==0x09)//��294��������ź�ģʽ
    {
      xinhao_test_flag=0x04;
    }
    break;
  case 0x00000200://6
    TestNum4++;
    if(TestNum4>=KeyCountNum)
    {
      TestNum4=0;
    NumberKeyboardPressDown_flag=TRUE;
    }
    break;
  case 0x00002000://7
    TestNum5++;
    if(TestNum5>=KeyCountNum)
    {
      TestNum5=0;
    NumberKeyboardPressDown_flag=TRUE;
    }
     break;
  case 0x00008000://9
    NumberKeyboardPressDown_flag=TRUE;
    if(xinhao_test_flag==0x02)//��294��������ź�ģʽ
    {
      xinhao_test_flag=0x09;
    }
    break;
  case 0x00010000://dn
    NumberKeyboardPressDown_flag=TRUE;
    if(LockingState_Flag==TRUE)
    {}
    else
    {
    if(MenuMode_Flag==1)
    {
      if(ApiMenu_BacklightTimeSet_Flag==1)//��������ñ���ƶ����˵�
      {
        BacklightTimeSetCount=BacklightTimeSetCount-1;
        if(BacklightTimeSetCount<1) {BacklightTimeSetCount=7;}
        Level3MenuDisplay(BacklightTimeSetCount);
      }
      else if(ApiMenu_KeylockTimeSet_Flag==1)//��������ü����������˵�
      {
        KeylockTimeSetCount=KeylockTimeSetCount-1;
        if(KeylockTimeSetCount<0x10) {KeylockTimeSetCount=0x16;}
        Level3MenuDisplay(KeylockTimeSetCount);
      }
      else if(ApiMenu_BeiDouOrWritingFrequency_Flag==1)//�������������ģʽ
      {
        if(network_mode_2gor3g_SetCount==0x17)
        {
          network_mode_2gor3g_SetCount=0x18;
        }
        else
        {
          network_mode_2gor3g_SetCount=0x17;
        }
        Level3MenuDisplay(network_mode_2gor3g_SetCount);
      }
      else if(ApiMenu_SwitchOnlineUser_Flag==1)//�������ʾ�����û��������˵�---------------------------------------------------------------
      {
        KeyPersonalCallingCount--;
        PersonalCallingNum=KeyPersonalCallingCount;//�����������㿪ʼ
        if(PersonalCallingNum<0)
        {
          PersonalCallingNum=ApiAtCmd_GetUserNum()-1;
          KeyPersonalCallingCount=ApiAtCmd_GetUserNum()-1;
        }
        if((PersonalCallingNum==ApiAtCmd_GetUserNum()-1)||PersonalCallingNum%poc_get_once_group_and_user_num==poc_get_once_group_and_user_num-1)
        {
          //ApiPocCmd_WritCommand(PocComm_UserListInfo,0,0);
          api_lcd_pwr_on_hint("��ȡ��...       ");//����
          huo_qu_zhong_flag=TRUE;
        }
        else
        {
          VOICE_SetOutput(ATVOICE_FreePlay,ApiAtCmd_GetUserName(PersonalCallingNum%poc_get_once_group_and_user_num),ApiAtCmd_GetUserNameLen(PersonalCallingNum%poc_get_once_group_and_user_num));//�������ϼ�֮���Ӧ���û���
          api_lcd_pwr_on_hint4("                ");//����
          if(UnicodeForGbk_AllUserName_english_flag(PersonalCallingNum%poc_get_once_group_and_user_num)==TRUE)
          {
            api_lcd_pwr_on_hint(UnicodeForGbk_AllUserName(PersonalCallingNum%poc_get_once_group_and_user_num));//��ʾ��ǰѡ�е�Ⱥ����
          }
          else
          {
            api_lcd_pwr_on_hint4(UnicodeForGbk_AllUserName(PersonalCallingNum%poc_get_once_group_and_user_num));//��ʾ��ǰѡ�е�Ⱥ����
          }
        }
        UpDownSwitching_Flag=TRUE;
        UpDownSwitchingCount=0;
      }
      else if(ApiMenu_GpsInfo_Flag==1||ApiMenu_NativeInfo_Flag==1)//�����GPS��Ϣ��������Ϣ������дƵ�л������˵�
      {}
      else
      {
        MenuModeCount=MenuModeCount-1;
        if(MenuModeCount<1) {MenuModeCount=8;}
        MenuDisplay(MenuModeCount);
      }
    }
    else
    {
    if(Key_PersonalCalling_Flag==1)//������¸�����
    {
      if(GettheOnlineMembersDone==TRUE)
      {
        KeyPersonalCallingCount--;
        PersonalCallingNum=KeyPersonalCallingCount;//�����������㿪ʼ
        if(PersonalCallingNum<0)
        {
          PersonalCallingNum=ApiAtCmd_GetUserNum()-1;
          KeyPersonalCallingCount=ApiAtCmd_GetUserNum()-1;
        }
        
        if((PersonalCallingNum==ApiAtCmd_GetUserNum()-1)||PersonalCallingNum%poc_get_once_group_and_user_num==poc_get_once_group_and_user_num-1)
        {
          //ApiPocCmd_WritCommand(PocComm_UserListInfo,0,0);
          api_lcd_pwr_on_hint("��ȡ��...       ");//����
          huo_qu_zhong_flag=TRUE;
        }
        else
        {
          VOICE_SetOutput(ATVOICE_FreePlay,ApiAtCmd_GetUserName(PersonalCallingNum%poc_get_once_group_and_user_num),ApiAtCmd_GetUserNameLen(PersonalCallingNum%poc_get_once_group_and_user_num));//�������ϼ�֮���Ӧ���û���
          api_lcd_pwr_on_hint4("                ");//����
          if(UnicodeForGbk_AllUserName_english_flag(PersonalCallingNum%poc_get_once_group_and_user_num)==TRUE)
          {
            api_lcd_pwr_on_hint(UnicodeForGbk_AllUserName(PersonalCallingNum%poc_get_once_group_and_user_num));//��ʾ��ǰѡ�е�Ⱥ����
          }
          else
          {
            api_lcd_pwr_on_hint4(UnicodeForGbk_AllUserName(PersonalCallingNum%poc_get_once_group_and_user_num));//��ʾ��ǰѡ�е�Ⱥ����
          }
        }
        UpDownSwitching_Flag=TRUE;
        UpDownSwitchingCount=0;
        /*api_lcd_pwr_on_hint("����:   ����ѡ��");
        api_lcd_pwr_on_hint2(HexToChar_PersonalCallingNum());*/
        KeyDownUpChoose_GroupOrUser_Flag=2;
      }
    }
    else
    {
      KeyUpDownCount--;
      GroupCallingNum=ApiAtCmd_GetMainGroupId()+KeyUpDownCount;
      if(GroupCallingNum<=0)
      {
        GroupCallingNum=ApiAtCmd_GetGroupNum();
        KeyUpDownCount=ApiAtCmd_GetGroupNum()-ApiAtCmd_GetMainGroupId();//
      }
      //api_disp_icoid_output( eICO_IDLOCKED, TRUE, TRUE);//ѡ
      VOICE_SetOutput(ATVOICE_FreePlay,ApiAtCmd_GetGroupName( GroupCallingNum),ApiAtCmd_GetGroupNameLen(GroupCallingNum));
      api_lcd_pwr_on_hint4("                ");//����
      api_lcd_pwr_on_hint4(UnicodeForGbk_AllGrounpName(GroupCallingNum));//��ʾ��ǰѡ�е�Ⱥ����
      
      UpDownSwitching_Flag=TRUE;
      UpDownSwitchingCount=0;
      //api_lcd_pwr_on_hint("Ⱥ��:   ѡ��Ⱥ��");//��ʾ����
      //api_lcd_pwr_on_hint2(HexToChar_GroupCallingNum());//��ʾ����
      KeyDownUpChoose_GroupOrUser_Flag=1;
    }
    }
    Key_Flag_1=1;
    }
    //api_lcd_pwr_on_hint("ŷ�갴��:Down  ");
    break;  
  case 0x00000010://ok
    if(LockingState_Flag==TRUE)
    {
      MenuDisplay(Menu_UnlockStep1_Ok);
      LockingState_EnterOK_Flag=TRUE;
    }
    else
    { 
      if(ApiPocCmd_PersonalCallingMode==TRUE)//�������������ͬʱ���¼���OK�����������
      {
        if(KeyDownUpChoose_GroupOrUser_Flag!=0)//������ڻ������ѡ�����״̬
        {
          switch(KeyDownUpChoose_GroupOrUser_Flag)
          {
          case 0://Ĭ��PTT״̬
          break;
          case 1://=1������ĳȺ��
          break;
          case 2://=2,����ĳ�û�
            if(GettheOnlineMembersDone==TRUE)
            {
              //GettheOnlineMembersDone=FALSE;
              VOICE_SetOutput(ATVOICE_FreePlay,"f25d09902d4e",12);//������ѡ��
              UpDownSwitchingCount=0;//���ѡ�е������л�Ⱥ�飬�����жϵ�����
              Delay_100ms(6);//��Ӱ��
              ApiPocCmd_WritCommand(PocComm_Invite,"0000000101",strlen((char const *)"0000000101"));
              KeyDownUpChoose_GroupOrUser_Flag=0;
              TASK_Ptt_StartPersonCalling_Flag=TRUE;//�ж���������״̬��0a��
              EnterKeyTimeCount=0;
            }
            break;
          case 3:
            break;
          default:
            break;
          }
        }
      }
      else
      {
      NumberKeyboardPressDown_flag=TRUE;
      if(KeyDownUpChoose_GroupOrUser_Flag!=0)//������ڻ������ѡ�����״̬
      {
        switch(KeyDownUpChoose_GroupOrUser_Flag)
        {
        case 0://Ĭ��PTT״̬
          break;
        case 1://=1������ĳȺ��
          VOICE_SetOutput(ATVOICE_FreePlay,"f25d09902d4e",12);//������ѡ��
          UpDownSwitchingCount=0;//���ѡ�е������л�Ⱥ�飬�����жϵ�����
          Delay_100ms(4);//DEL_SetTimer(0,40);
          //while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
          ApiPocCmd_WritCommand(PocComm_EnterGroup,"0000000101",strlen((char const *)"0000000101"));
          KeyDownUpChoose_GroupOrUser_Flag=0;
          EnterKeyTimeCount=0;
          KeyUpDownCount=0;
          break;
        case 2://=2,����ĳ�û�
          if(GettheOnlineMembersDone==TRUE)
          {
            //GettheOnlineMembersDone=FALSE;
            VOICE_SetOutput(ATVOICE_FreePlay,"f25d09902d4e",12);//������ѡ��
            UpDownSwitchingCount=0;//���ѡ�е������л�Ⱥ�飬�����жϵ�����
            Delay_100ms(6);//DEL_SetTimer(0,60);
            //while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
            ApiPocCmd_WritCommand(PocComm_Invite,"0000000101",strlen((char const *)"0000000101"));
            KeyDownUpChoose_GroupOrUser_Flag=0;
            TASK_Ptt_StartPersonCalling_Flag=TRUE;//�ж���������״̬��0a��
            EnterKeyTimeCount=0;
            
          }
          break;
        case 3:
          break;
        default:
          break;
        }
      }
      else//����ͽ���˵�ģʽ
      {
        switch(MenuModeCount)//Ĭ�ϰ�ok������һ���˵�
        {
        case 1://Ⱥ��ѡ��
          Key_PersonalCalling_Flag=0;//���������־λ
          switch(ApiMenu_SwitchGroup_Flag)
          {
          case 0://Ĭ��ģʽ��OK������һ���˵�
            MenuDisplay(MenuModeCount);
            MenuMode_Flag=1;
            ApiMenu_SwitchGroup_Flag=1;
            TheMenuLayer_Flag=1;//����һ���˵�
            break;
          case 1://һ���˵��ٰ�ok��Ĭ��ģʽ
            SubmenuMenuDisplay(GroupSwitch);
            VOICE_SetOutput(ATVOICE_FreePlay,"07526263A47FC47E",16);//�л�Ⱥ��
            ApiMenu_SwitchGroup_Flag=0;
            TheMenuLayer_Flag=0;//����0���˵������뻻��Ϊ�˵��⹦��
            MenuMode_Flag=0;
            break;
          }

          break;
        case 2://��Աѡ��
          switch(ApiMenu_SwitchCallUser_Flag)
          {
          case 1://Ĭ�ϲ˵���OK������һ���˵�
            MenuDisplay(MenuModeCount);
            MenuMode_Flag=1;
            ApiMenu_SwitchCallUser_Flag=0;
            TheMenuLayer_Flag=1;//����һ���˵�
            break;
          case 0://һ���˵���ok�����뵥��ģʽ
            MenuDisplay(Menu_RefreshAllIco);
            ApiMenu_SwitchCallUser_Flag=1;
            MenuMode_Flag=0;
            /*******ֱ�Ӱ������״̬���ĳ���***************************************************************************************************************************************/
            if(POC_EnterPersonalCalling_Flag==1)//�������״̬�£�����������Ч��Ӧ���Ǳ���״̬�£��ø�����ʧЧ��
            {
              VOICE_SetOutput(ATVOICE_FreePlay,"ab887c542d4e",12);//������
              Delay_100ms(5);//��Ӱ��
              api_lcd_pwr_on_hint("                ");//����
              
              if(UnicodeForGbk_MainUserName_english_flag()==TRUE)
              {
                api_lcd_pwr_on_hint(UnicodeForGbk_MainUserName());//��ʾ��ǰ�û��ǳ�
              }
              else
              {
                api_lcd_pwr_on_hint4(UnicodeForGbk_MainUserName());//��ʾ��ǰ�û��ǳ�
              }
              
            }
            else
            {
              api_lcd_pwr_on_hint("    ����ģʽ    ");
              PersonalCallingNum=0;//�����������ֱ��ѡ�У������û������ǲ������û�
              Key_PersonalCalling_Flag=1;
              VOICE_SetOutput(ATVOICE_FreePlay,"C5627C54216A0F5F",16);//����ģʽ
              Delay_100ms(6);//��Ӱ��
              get_online_user_list_num_for_key_flag=TRUE;
              KeyDownUpChoose_GroupOrUser_Flag=2;
              TheMenuLayer_Flag=0;//����0���˵������뵥��ģʽΪ�˵��⹦��
              KeyPersonalCallingCount=0;//�������ģʽ�����¼���Ա������˳�򣬵�һ����Ա���л�ʱ��ڶ�������������
            }
            break;
          }
          break;
        case 3://���߳�Ա�б�
          switch(ApiMenu_SwitchOnlineUser_Flag)
          {
          case 2:
            ApiMenu_SwitchOnlineUser_Flag=0;
            MenuDisplay(MenuModeCount);
            MenuMode_Flag=1;
            break;
          case 1://Ĭ�ϲ˵���OK������һ���˵�
            MenuDisplay(MenuModeCount);
            MenuMode_Flag=1;
            ApiMenu_SwitchOnlineUser_Flag=2;
            TheMenuLayer_Flag=1;//����һ���˵�
            break;
          case 0://һ���˵���ok�����뵥��ģʽ
            ApiMenu_SwitchOnlineUser_Flag=1;
            MenuMode_Flag=1;
            /*******ֱ�Ӱ������״̬���ĳ���***************************************************************************************************************************************/
            api_lcd_pwr_on_hint3("���߳�Ա��:     ");
            api_lcd_pwr_on_hint("                ");
            get_online_user_list_num_flag=TRUE;
            get_online_user_list_num_for_key_flag=TRUE;
            break;
          }
          break;
        case 4://GPS����
              switch(ApiMenu_GpsInfo_Flag)
              {
               //�ͻ�Ҫ��GPS��γ�ȼ�������Ϣ���水���¼���OK����Ч��ֻ�а��˳����˳�������������
              /*case 1://�����˵���OK������һ���˵�
                MenuDisplay(MenuModeCount);
                MenuMode_Flag=1;
                ApiMenu_GpsInfo_Flag=0;
                TheMenuLayer_Flag=1;//����һ���˵�
                break;*/
              case 0://һ���˵���ok����������˵�
                SubmenuMenuDisplay(GpsInfoMenu);
                ApiMenu_GpsInfo_Flag=1;
                TheMenuLayer_Flag=2;//���ڶ����˵�
                break;
              }
          break;
        case 5://���������
              switch(ApiMenu_BacklightTimeSet_Flag)
              {
              case 2:
                ApiMenu_BacklightTimeSet_Flag=0;
                MenuDisplay(MenuModeCount);
                MenuMode_Flag=1;
                break;
              case 0://��һ���˵���ok����������˵�
                ApiMenu_BacklightTimeSet_Flag=1;//�����¼��д���
                SubmenuMenuDisplay(BacklightTimeSet);
                TheMenuLayer_Flag=2;//���ڶ����˵�
                break;
              case 1://�����˵���ok������һ���˵�
                ApiMenu_BacklightTimeSet_Flag=2;
                MenuDisplay(MenuModeCount);
                MenuMode_Flag=1;
                TheMenuLayer_Flag=1;//����һ���˵�
                break;
              }
          break;
        case 6://��������
              switch(ApiMenu_KeylockTimeSet_Flag)
              {
              case 2://Ĭ��״̬��OK������һ���˵�
                ApiMenu_KeylockTimeSet_Flag=0;
                MenuDisplay(MenuModeCount);
                MenuMode_Flag=1;
                break;
              case 0://��һ���˵���ok����������˵�
                ApiMenu_KeylockTimeSet_Flag=1;//�����¼��д���
                SubmenuMenuDisplay(KeylockTimeSet);
                TheMenuLayer_Flag=2;//���ڶ����˵�
                break;
              case 1:
                ApiMenu_KeylockTimeSet_Flag=2;
                MenuDisplay(MenuModeCount);
                MenuMode_Flag=1;
                TheMenuLayer_Flag=1;//����һ���˵�
                break;
              }
          break;
        case 7://������Ϣ
              switch(ApiMenu_NativeInfo_Flag)
              {
                //�ͻ�Ҫ��GPS��γ�ȼ�������Ϣ���水���¼���OK����Ч��ֻ�а��˳����˳�������������
              /*case 1://Ĭ��״̬��OK������һ���˵�
                MenuDisplay(MenuModeCount);
                MenuMode_Flag=1;
                ApiMenu_NativeInfo_Flag=0;
                TheMenuLayer_Flag=1;//����һ���˵�
                break;*/
              case 0://��gps��Ϣһ���˵���ok����������˵�
                SubmenuMenuDisplay(NativeInfoMenu);
                ApiMenu_NativeInfo_Flag=1;
                TheMenuLayer_Flag=2;//���ڶ����˵�
                break;
              }
          break;
        case 8://����/дƵ�л�
          switch(ApiMenu_BeiDouOrWritingFrequency_Flag)
          {
          case 2://Ĭ��״̬��OK������һ���˵�
            ApiMenu_BeiDouOrWritingFrequency_Flag=0;
            MenuDisplay(MenuModeCount);
            MenuMode_Flag=1;
            break;
          case 0://��һ���˵���ok����������˵�
            ApiMenu_BeiDouOrWritingFrequency_Flag=1;//�����¼��д���
            SubmenuMenuDisplay(BeiDouOrWritingFrequencySwitch);
            TheMenuLayer_Flag=2;//���ڶ����˵�
            break;
          case 1://�����˵�ȷ�Ϸ���һ���˵�
            ApiMenu_BeiDouOrWritingFrequency_Flag=2;
            MenuDisplay(MenuModeCount);
            MenuMode_Flag=1;
            TheMenuLayer_Flag=1;//����һ���˵�
            break;
          }
          break;
        default:
          break;
        }
    }
    }
    }
    Key_Flag_1=1;//������ʱ��־λ
    break;
  case 0x00800000://menu
    NumberKeyboardPressDown_flag=TRUE;
    if(LockingState_Flag==TRUE)
    {}
    else
    {
      KEY_4_Flag=TRUE;
    if(AkeyvolumeCount==7)
    {
      
      VOICE_SetOutput(ATVOICE_FreePlay,"2c54527b216a0f5f",16);//��Ͳģʽ
      if(MenuMode_Flag==0)
      {
        api_disp_icoid_output( eICO_IDMONITER, TRUE, TRUE);//��Ͳģʽͼ��
      }
      VoiceType_FreehandOrHandset_Flag=1;
      api_disp_all_screen_refresh();// ȫ��ͳһˢ��
      Delay_100ms(3);//DEL_SetTimer(0,30);
      //while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
      NoUseNum=ApiAtCmd_WritCommand(ATCOMM7_VGR,(u8 *)ucVGR1,strlen((char const *)ucVGR1));//
      AkeyvolumeCount=1;
    }
    else
    {
      if(AkeyvolumeCount==1)
      {
        VOICE_SetOutput(ATVOICE_FreePlay,"4d51d063216a0f5f",16);//����ģʽ
        if(MenuMode_Flag==0)
        {
          api_disp_icoid_output( eICO_IDTemper, TRUE, TRUE);//����ģʽͼ��          
        }
        VoiceType_FreehandOrHandset_Flag=0;
        api_disp_all_screen_refresh();// ȫ��ͳһˢ��
        Delay_100ms(3);//DEL_SetTimer(0,30);
        //while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
        NoUseNum=ApiAtCmd_WritCommand(ATCOMM7_VGR,(u8 *)ucVGR7,strlen((char const *)ucVGR7));//
        AkeyvolumeCount=7;
      }
    }
    }

    break;   
  case 0x00000004://2
    NumberKeyboardPressDown_flag=TRUE;
    if(LockingState_Flag==TRUE)
    {}
    else
    {
      if(xinhao_test_flag==0x00)//��294��������ź�ģʽ
      {
        xinhao_test_flag=0x02;
      }
    }
    break;  
  case 0x00080000://*
    TestNum8++;
    if(TestNum8>=KeyCountNum)
    {
      TestNum8=0;
    }
    break;  
  case 0x00000100://5
    TestNum9++;
    if(TestNum9>=KeyCountNum)
    {
      TestNum9=0;
    NumberKeyboardPressDown_flag=TRUE;
    }
    break;  
  case 0x00100000://0
    TestNum10++;
    if(TestNum10>=KeyCountNum)
    {
      TestNum10=0;
    NumberKeyboardPressDown_flag=TRUE;
    }
    break;  
  case 0x00004000://8
    TestNum11++;
    if(TestNum11>=KeyCountNum)
    {
      TestNum11=0;
    NumberKeyboardPressDown_flag=TRUE;
    }
    break;  
  case 0x00200000://#
    TestNum12++;
    if(TestNum12>=KeyCountNum)
    {
      if(LockingState_EnterOK_Flag==TRUE)//�������״̬�°���OK��Ȼ����#��
      {
        LockingState_EnterOK_Flag=FALSE;
        LockingState_Flag=FALSE;
        TimeCount=0;
        TimeCount3=0;//���������ż��ʧЧ������
        MenuDisplay(Menu_unLocking);
        api_disp_icoid_output(eICO_IDBANDWIDTHN, TRUE, TRUE);//��������ͼ��
        api_disp_all_screen_refresh();// ȫ��ͳһˢ��
      }
      else
      {
        NumberKeyboardPressDown_flag=TRUE;
        if(xinhao_test_flag==0x04)//��294��������ź�ģʽ
        {
          xinhao_test_flag=0xFF;
        }
      }
      TestNum12=0;
    }
    break;  
  case 0x00000400://up
    NumberKeyboardPressDown_flag=TRUE;
    if(LockingState_Flag==TRUE)
    {}
    else
    {
      if(MenuMode_Flag==1)
      {
        if(ApiMenu_BacklightTimeSet_Flag==1)//��������ñ���ƶ����˵�
        {
          BacklightTimeSetCount=BacklightTimeSetCount+1;
          if(BacklightTimeSetCount>7) {BacklightTimeSetCount=1;}
          Level3MenuDisplay(BacklightTimeSetCount);
        }
        else if(ApiMenu_KeylockTimeSet_Flag==1)//��������ü����������˵�
        {
          KeylockTimeSetCount=KeylockTimeSetCount+1;
          if(KeylockTimeSetCount>0x16) {KeylockTimeSetCount=0x10;}
          Level3MenuDisplay(KeylockTimeSetCount);
        }
        else if(ApiMenu_BeiDouOrWritingFrequency_Flag==1)//�������������ģʽ
        {
          if(network_mode_2gor3g_SetCount==0x17)
          {
            network_mode_2gor3g_SetCount=0x18;
          }
          else
          {
            network_mode_2gor3g_SetCount=0x17;
          }
          Level3MenuDisplay(network_mode_2gor3g_SetCount);
        }
        else if(ApiMenu_SwitchOnlineUser_Flag==1)//�������ʾ�����û��������˵�---------------------------------------------------------------
        {
          KeyPersonalCallingCount++;
          PersonalCallingNum=KeyPersonalCallingCount;//�����������㿪ʼ
          if(PersonalCallingNum>ApiAtCmd_GetUserNum()-1)
          {
            KeyPersonalCallingCount=0;
            PersonalCallingNum=0;
          }
          
        if(PersonalCallingNum%poc_get_once_group_and_user_num==0)
        {
          //ApiPocCmd_WritCommand(PocComm_UserListInfo,0,0);
          api_lcd_pwr_on_hint("��ȡ��...       ");//����
          huo_qu_zhong_flag=TRUE;
        }
        else
        {
          VOICE_SetOutput(ATVOICE_FreePlay,ApiAtCmd_GetUserName(PersonalCallingNum%poc_get_once_group_and_user_num),ApiAtCmd_GetUserNameLen(PersonalCallingNum%poc_get_once_group_and_user_num));//�������ϼ�֮���Ӧ���û���
          api_lcd_pwr_on_hint4("                ");//����
          if(UnicodeForGbk_AllUserName_english_flag(PersonalCallingNum%poc_get_once_group_and_user_num)==TRUE)
          {
            api_lcd_pwr_on_hint(UnicodeForGbk_AllUserName(PersonalCallingNum%poc_get_once_group_and_user_num));//��ʾ��ǰѡ�е�Ⱥ����
          }
          else
          {
            api_lcd_pwr_on_hint4(UnicodeForGbk_AllUserName(PersonalCallingNum%poc_get_once_group_and_user_num));//��ʾ��ǰѡ�е�Ⱥ����
          }
        }

          UpDownSwitching_Flag=TRUE;
          UpDownSwitchingCount=0;
        }
        else if(ApiMenu_GpsInfo_Flag==1||ApiMenu_NativeInfo_Flag==1)//�����GPS��Ϣ��������Ϣ������дƵ�л������˵�
        {}
        else
        {
          MenuModeCount=MenuModeCount+1;
          if(MenuModeCount>8) {MenuModeCount=1;}
          MenuDisplay(MenuModeCount);
        }
      }
      else
      {
        if(Key_PersonalCalling_Flag==1)//������¸�����
        {
          if(GettheOnlineMembersDone==TRUE)
          {
            KeyPersonalCallingCount++;
            PersonalCallingNum=KeyPersonalCallingCount;//�����������㿪ʼ
            if(PersonalCallingNum>ApiAtCmd_GetUserNum()-1)
            {
              KeyPersonalCallingCount=0;
              PersonalCallingNum=0;
            }
        if(PersonalCallingNum%poc_get_once_group_and_user_num==0)
        {
          //ApiPocCmd_WritCommand(PocComm_UserListInfo,0,0);
          api_lcd_pwr_on_hint("��ȡ��...       ");//����
          huo_qu_zhong_flag=TRUE;
        }
        else
        {
          VOICE_SetOutput(ATVOICE_FreePlay,ApiAtCmd_GetUserName(PersonalCallingNum%poc_get_once_group_and_user_num),ApiAtCmd_GetUserNameLen(PersonalCallingNum%poc_get_once_group_and_user_num));//�������ϼ�֮���Ӧ���û���
          api_lcd_pwr_on_hint4("                ");//����
          if(UnicodeForGbk_AllUserName_english_flag(PersonalCallingNum%poc_get_once_group_and_user_num)==TRUE)
          {
            api_lcd_pwr_on_hint(UnicodeForGbk_AllUserName(PersonalCallingNum%poc_get_once_group_and_user_num));//��ʾ��ǰѡ�е�Ⱥ����
          }
          else
          {
            api_lcd_pwr_on_hint4(UnicodeForGbk_AllUserName(PersonalCallingNum%poc_get_once_group_and_user_num));//��ʾ��ǰѡ�е�Ⱥ����
          }
        }
            UpDownSwitching_Flag=TRUE;
            UpDownSwitchingCount=0;
            /*api_lcd_pwr_on_hint("����:   ����ѡ��");
            api_lcd_pwr_on_hint2(HexToChar_PersonalCallingNum());*/
            KeyDownUpChoose_GroupOrUser_Flag=2;
          }
        }
        else
        {
          KeyUpDownCount++;
          GroupCallingNum=ApiAtCmd_GetMainGroupId()+KeyUpDownCount;
          if(GroupCallingNum>ApiAtCmd_GetGroupNum())
          {
            GroupCallingNum=1;
            KeyUpDownCount=1-ApiAtCmd_GetMainGroupId();
          }
          //api_disp_icoid_output( eICO_IDLOCKED, TRUE, TRUE);//ѡ
          VOICE_SetOutput(ATVOICE_FreePlay,ApiAtCmd_GetGroupName(GroupCallingNum),ApiAtCmd_GetGroupNameLen(GroupCallingNum));
          api_lcd_pwr_on_hint4("                ");//����
          api_lcd_pwr_on_hint4(UnicodeForGbk_AllGrounpName(GroupCallingNum));//��ʾ��ǰѡ�е�Ⱥ����
          UpDownSwitching_Flag=TRUE;
          UpDownSwitchingCount=0;
          //api_lcd_pwr_on_hint("Ⱥ��:   Ⱥ��ѡ��");//��ʾ����
          //api_lcd_pwr_on_hint2(HexToChar_GroupCallingNum());//��ʾ����
          KeyDownUpChoose_GroupOrUser_Flag=1;
        }
      }
      Key_Flag_1=1;
    }
   
    break;
  case 0x00400000://cancel
    if(xinhao_test_flag==0xFF)
    {
      api_lcd_pwr_on_hint("                ");
      api_lcd_pwr_on_hint3("                ");
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
    xinhao_test_flag=0x00;
    NumberKeyboardPressDown_flag=TRUE;
    if(LockingState_Flag==TRUE)
    {}
    else
    {
      if(MenuMode_Flag==1)
      {
        if(TheMenuLayer_Flag==2)//�����˵������ؼ�����1���˵�
        {
          if(ApiMenu_GpsInfo_Flag==1)
          {
            MenuDisplay(MenuModeCount);
            MenuMode_Flag=1;
            ApiMenu_GpsInfo_Flag=0;
            TheMenuLayer_Flag=1;//����һ���˵�
          }
          else if(ApiMenu_SwitchOnlineUser_Flag==1)
          {
            MenuDisplay(MenuModeCount);
            MenuMode_Flag=1;
            ApiMenu_SwitchOnlineUser_Flag=0;
            TheMenuLayer_Flag=1;//����һ���˵�
          }
          else if(ApiMenu_NativeInfo_Flag==1)
          {
            MenuDisplay(MenuModeCount);
            MenuMode_Flag=1;
            ApiMenu_NativeInfo_Flag=0;
            TheMenuLayer_Flag=1;//����һ���˵�
          }
          else if(ApiMenu_BeiDouOrWritingFrequency_Flag==1)
          {
            MenuDisplay(MenuModeCount);
            MenuMode_Flag=1;
            ApiMenu_BeiDouOrWritingFrequency_Flag=0;
            TheMenuLayer_Flag=1;//����һ���˵�
          }
          else if(ApiMenu_BacklightTimeSet_Flag==1)
          {
            ApiMenu_BacklightTimeSet_Flag=0;
            MenuDisplay(MenuModeCount);
            MenuMode_Flag=1;
            TheMenuLayer_Flag=1;//����һ���˵�
          }
          else if(ApiMenu_KeylockTimeSet_Flag==1)
          {
            ApiMenu_KeylockTimeSet_Flag=0;
            MenuDisplay(MenuModeCount);
            MenuMode_Flag=1;
            TheMenuLayer_Flag=1;//����һ���˵�
          }
          else
          {}
        }
        else if(TheMenuLayer_Flag==1)
        {
#if 1
          MenuDisplay(Menu_RefreshAllIco);
          api_lcd_pwr_on_hint("                ");//����
          //api_lcd_pwr_on_hint(HexToChar_MainGroupId());//��ʾ��ǰȺ��ID
          api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
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
#else
          switch(MenuModeCount)//Ĭ�ϰ�ok������һ���˵�
          {
          case 1://Ⱥ��ѡ��
            if(ApiMenu_SwitchGroup_Flag==1)//����Ϊһ���˵������ؼ�����Ĭ�Ͻ���
            {
              TheMenuLayer_Flag=0;//����һ���˵�
              MenuMode_Flag=0;
              ApiMenu_SwitchGroup_Flag=0;
              MenuDisplay(Menu_RefreshAllIco);
              api_lcd_pwr_on_hint("                ");//����
              api_lcd_pwr_on_hint(HexToChar_MainGroupId());//��ʾ��ǰȺ��ID
              api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
            }
            break;
          case 2://��Աѡ��
            if(ApiMenu_SwitchCallUser_Flag==0)//����Ϊһ���˵������ؼ�����Ĭ�Ͻ���
            {
              TheMenuLayer_Flag=0;//����һ���˵�
              MenuMode_Flag=0;
              ApiMenu_SwitchCallUser_Flag=1;
              MenuDisplay(Menu_RefreshAllIco);
              api_lcd_pwr_on_hint("                ");//����
              api_lcd_pwr_on_hint(HexToChar_MainGroupId());//��ʾ��ǰȺ��ID
              api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
            }
            break;
          case 3://GPS��Ϣ
            if(ApiMenu_GpsInfo_Flag==0)//����Ϊһ���˵������ؼ�����Ĭ�Ͻ���
            {
              TheMenuLayer_Flag=0;//����һ���˵�
              MenuMode_Flag=0;
              ApiMenu_GpsInfo_Flag=1;
              MenuDisplay(Menu_RefreshAllIco);
              api_lcd_pwr_on_hint("                ");//����
              api_lcd_pwr_on_hint(HexToChar_MainGroupId());//��ʾ��ǰȺ��ID
              api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
            }
            break;
          case 4://���������
            if(ApiMenu_BacklightTimeSet_Flag==0)
            {
              TheMenuLayer_Flag=0;//����һ���˵�
              MenuMode_Flag=0;
              ApiMenu_BacklightTimeSet_Flag=1;
              MenuDisplay(Menu_RefreshAllIco);
              api_lcd_pwr_on_hint("                ");//����
              api_lcd_pwr_on_hint(HexToChar_MainGroupId());//��ʾ��ǰȺ��ID
              api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
            }
            break;
          case 5://��������
            if(ApiMenu_KeylockTimeSet_Flag==0)
            {
              TheMenuLayer_Flag=0;//����һ���˵�
              MenuMode_Flag=0;
              ApiMenu_KeylockTimeSet_Flag=1;
              MenuDisplay(Menu_RefreshAllIco);
              api_lcd_pwr_on_hint("                ");//����
              api_lcd_pwr_on_hint(HexToChar_MainGroupId());//��ʾ��ǰȺ��ID
              api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
            }
            break;
          case 6://������Ϣ
            if(ApiMenu_NativeInfo_Flag==0)
            {
              TheMenuLayer_Flag=0;//����һ���˵�
              MenuMode_Flag=0;
              ApiMenu_NativeInfo_Flag=1;
              MenuDisplay(Menu_RefreshAllIco);
              api_lcd_pwr_on_hint("                ");//����
              api_lcd_pwr_on_hint(HexToChar_MainGroupId());//��ʾ��ǰȺ��ID
              api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
            }
            break;
          case 7://����/дƵ�л�
            if(ApiMenu_BeiDouOrWritingFrequency_Flag==0)
            {
              TheMenuLayer_Flag=0;//����һ���˵�
              MenuMode_Flag=0;
              ApiMenu_BeiDouOrWritingFrequency_Flag=1;
              MenuDisplay(Menu_RefreshAllIco);
              api_lcd_pwr_on_hint("                ");//����
              api_lcd_pwr_on_hint(HexToChar_MainGroupId());//��ʾ��ǰȺ��ID
              api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
            }
            break;
          }
#endif
        }
        else
        {}
        Key_Flag_1=1;//������ʱ��־λ
      }
      else
      {
        if(Key_PersonalCalling_Flag==1||POC_EnterPersonalCalling_Flag==1||POC_AtEnterPersonalCalling_Flag==1)//������ڵ���ģʽ�������ؼ��������
        {
          ApiPocCmd_WritCommand(PocComm_Cancel,(u8 *)ucQuitPersonalCalling,strlen((char const *)ucQuitPersonalCalling));
          Key_Flag_1=1;//������ʱ��־λ
          Key_PersonalCalling_Flag=0;//���������־λ
          KeyDownUpChoose_GroupOrUser_Flag=0;//����������������ؼ���OK��PTT����Ļ��ʾ�����BUG
          huo_qu_zhong_flag=FALSE;
        }
        else//����������ģʽ��Ӧ���ޱ仯
        {
          MenuMode_Flag=0;
          if(ShowTime_Flag==FALSE)
          {
            api_disp_icoid_output(eICO_IDMESSAGEOff, TRUE, TRUE);//��ͼ��-��ѡ��Ӧ
          }
          api_lcd_pwr_on_hint("                ");//����
          //api_lcd_pwr_on_hint(HexToChar_MainGroupId());//��ʾ��ǰȺ��ID
          api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
          Key_Flag_1=1;//������ʱ��־λ
          //����PTT�������¼�����Ĭ��״̬
          KeyDownUpChoose_GroupOrUser_Flag=0;
          KeyUpDownCount=0;
        }
        key_warning_flag=FALSE;//�����ؼ�������־λ����
        key_warning_bubiao_flag=FALSE;//�����ؼ����걨����־λ����
      }

    }
    break;  
  default:
    break;
  }
  GeHuTest(ulAllKeyID);
  if(Key_Flag_1==1)//���°����ӳ�1��
  {
    Delay_100ms(1);//DEL_SetTimer(1,30);//influence get group name
    Key_Flag_1=0;
    //if(KeyDownUpChoose_GroupOrUser_Flag==2)
    {
      if(huo_qu_zhong_flag==TRUE)
      {
        ApiPocCmd_WritCommand(PocComm_UserListInfo,0,0);
      }
    }
    if(get_online_user_list_num_for_key_flag==TRUE)
    {
      ApiPocCmd_WritCommand(PocComm_UserListInfo,0,0);
      get_online_user_list_num_for_key_flag=FALSE;
    }
  }
}



u32 get_key_value(u8 scan_value)
{
  u32 value = 0;
  switch (scan_value)
  {
  case 0x11://"7"
    value = 0x00002000;
    break;
  case 0x12://"4"
    value = 0x00000080;
    break;
  case 0x13://"1"
    value = 0x00000002;
    break;
  case 0x14://OK
    value = 0x00000010;
    break;
  case 0x21://"8"
    value = 0x00004000;
    break;
  case 0x22://"5"
    value = 0x00000100;
    break;
  case 0x23://"2"
    value = 0x00000004;
    break;
  case 0x24://"DN"
    value = 0x00010000;
    break;
  case 0x31://"9"
    value = 0x00008000;
    break;
  case 0x32://"6"
    value = 0x00000200;
    break;
  case 0x33://"3"
    value = 0x00000008;
    break;
  case 0x34://"UP"
    value = 0x00000400;
    break;
  case 0x41://"#"
    value = 0x00200000;
    break;
  case 0x42://"0"
    value = 0x00100000;
    break;
  case 0x43://"*"
    value = 0x00080000;
    break;
  case 0x44://"Cancel"
    value = 0x00400000;
    break;
  case 0x54://"Menu"
    value = 0x00800000;
    break;
  default:
    break;
  }
  return value;
}

void GeHuTest(u32 KeyID)
{
#if 0
   switch(KeyID)
  {
  case 0x00000002://1��
    TestNum1++;
    if(TestNum1>=KeyCountNum)
    {
      TestNum1=0;
      num1=1;
      PressButton=TRUE;
    }
    break; 
  case 0x00000004://2��
    TestNum7++;
    if(TestNum7>=KeyCountNum)
    {
      TestNum7=0;
      num1=2;
      PressButton=TRUE;
    }
    break;  
  case 0x00000008://3��
    TestNum2++;
    if(TestNum2>=KeyCountNum)
    {
      TestNum2=0;
      num1=3;
      PressButton=TRUE;
    }
    break;
  case 0x00000080://4��
    TestNum3++;
    if(TestNum3>=KeyCountNum)
    {
      TestNum3=0;
      num1=4;
      PressButton=TRUE;
    }
    break;
  case 0x00000100://5��
    TestNum9++;
    if(TestNum9>=KeyCountNum)
    {
      TestNum9=0;
      num1=5;
      PressButton=TRUE;
    }
    break;  
  case 0x00000200://6��
    TestNum4++;
    if(TestNum4>=KeyCountNum)
    {
      TestNum4=0;
      num1=6;
      PressButton=TRUE;
    }
    break;
  case 0x00002000://7��
    TestNum5++;
    if(TestNum5>=KeyCountNum)
    {
      TestNum5=0;
      num1=7;
      PressButton=TRUE;
    }
     break;
  case 0x00004000://8��
    TestNum11++;
    if(TestNum11>=KeyCountNum)
    {
      TestNum11=0;
      num1=8;
      PressButton=TRUE;
    }
    break;  
  case 0x00008000://9��
    TestNum6++;
    if(TestNum6>=KeyCountNum)
    {
      TestNum6=0;
      num1=9;
      PressButton=TRUE;
    }
    break;
  case 0x00100000://0
    TestNum10++;
    if(TestNum10>=KeyCountNum)
    {
      TestNum10=0;
      num1=0;
      PressButton=TRUE;
    }
    break;
  case 0x00010000://dn
    break;  
  case 0x00000010://ok
    break;
  case 0x00800000://menu   
    break;   
  case 0x00080000://*
    break;  
  case 0x00200000://#
    break;  
  case 0x00000400://up
    break;
  case 0x00400000://cancel
    numCount=0;
    break;  
  default:
    if(PressButton==TRUE)
    {
      KeyBoardState=TRUE;
      PressButton  =FALSE;
    }
    else
    {
      KeyBoardState=FALSE;
    }
    break;
  }
  
if(KeyBoardState==TRUE)//ʶ���°������ɿ������Ĺ���
{
  numCount++;
  num6=num5;
  num5=num4;
  num4=num3;
  num3=num2;
  num2=num1;
    switch(numCount)
    {
    case 1:
      TestBuf1[0]=num2+0x30;
      TestBuf1[1]='\0';
      break;
    case 2:
      TestBuf1[0]=num3+0x30;
      TestBuf1[1]=num2+0x30;
      TestBuf1[2]='\0';
      break;
    case 3:
      TestBuf1[0]=num4+0x30;
      TestBuf1[1]=num3+0x30;
      TestBuf1[2]=num2+0x30;
      TestBuf1[3]='\0';
      break;
    case 4:
      TestBuf1[0]=num5+0x30;
      TestBuf1[1]=num4+0x30;
      TestBuf1[2]=num3+0x30;
      TestBuf1[3]=num2+0x30;
      TestBuf1[4]='\0';
      break;
    case 5:
      TestBuf1[0]=num6+0x30;
      TestBuf1[1]=num5+0x30;
      TestBuf1[2]=num4+0x30;
      TestBuf1[3]=num3+0x30;
      TestBuf1[4]=num2+0x30;
      TestBuf1[5]='\0';
      break;
    default:
      break;
    }
   //�̺ź��������⣬��ʱ����
    api_lcd_pwr_on_hint3("�����̺�        ");
     api_lcd_pwr_on_hint("                ");
     api_lcd_pwr_on_hint(TestBuf1);
 
}
  #endif

}