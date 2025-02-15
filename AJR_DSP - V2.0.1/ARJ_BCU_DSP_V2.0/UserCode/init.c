/*
 * init.c
 *
 *  Created on: 2022年1月5日
 *      Author: 15755
 */
#include "user_main.h"
unsigned short Board_ID=0;
unsigned short Other_Board_ID=0;
unsigned short SDRAM_READ_DATA_LEN=0;
unsigned short SDRAM_WRITE_DATA_LEN=0;
unsigned short Device_DATA_LEN=0;
/*******************************
 * void initialization(void)
 * 设备初始化。
 ******************************/
void initialization(void)
{
    uint8_t i = 20;
    _enable_interrupt_();   /*使能中断*/
    _enable_IRQ();          /*使能中断*/
    rtiInit();              /*初始化定时器*/
    DAC5689_Init();         /*DAC5689初始化*/
    gioInit();              /*I/O 初始化*/
    muxInit();              /*引脚分配 初始化*/
    Sdram_Init();           /*SDRAM总线 初始化*/
    sciInit();              /*RS232 初始化*/
    delay_ms(100);
    SD_GPIO_Init();         /*SD卡I/O 口初始化*/
    Other_GPIO_Init();      /*其它IO口初始化*/
    Get_Master_Slave();     /*获取DSP主从模式（内轮/外轮）*/
    device_status_init();   /*设备变量初始化*/
    Get_Bench_Mode();       /*获取Bench_Mode信号*/
    while (i && sd_card_status != Fatfs_Load_Success)
    {
       SdCard_Maintenance();
       i--;
    }
    rtiEnableNotification(rtiNOTIFICATION_COMPARE0); /*使能定时器COMPARE0 5ms*/
    rtiStartCounter(rtiCOUNTER_BLOCK0);              /*开始定时器BLOCK0计数*/
//    rtiEnableNotification(rtiNOTIFICATION_COMPARE3);
//    rtiStartCounter(rtiCOUNTER_BLOCK1);
    Set_SOV(0);   /*设置SOV上电状态*/
    Set_ABSW(0);  /*设置ABSW上电状态*/
}

/**********************
 * void device_status_init(void)
 * 设备状态参数和变量初始化
 * ********************/
void device_status_init(void)
{
    unsigned short add_num=0;
    unsigned short temp=0;
    /*read Master coll*/
    MCB_Data[add_num].Addr=ADDR_INBD_LT_TEMP;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_INBD_RT_TEMP;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_INBD_LT_BPSI;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_INBD_RT_BPSI;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_LEFTPPEDAL;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_RIGHTPPEDAL;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_LEFTCPEDAL;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_RIGHTCPEDAL;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_INNER_WHEEL_ABS_START_SIGNAL;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_INBOARD_SHUTOFF_VALVE_CURRENT;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_LEFT_INBOARD_BRAKE_CONTROL_VALVE_CURRENT;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_RIGHT_INBOARD_BRAKE_CONTROL_VALVE_CURRENT;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_LEFTWHEELSPEED;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_RIGHTWHEELSPEED;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_AUTO_BRAKE_SW_SIGNAL;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_OTHER_SW_SIGNAL;
    MCB_Data[add_num++].value=0;
    /*read Master 429*/
    MCB_Data[add_num].Addr=ADDR_HYDRAULIC_SYSTEM_PRESSURE;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_ACCUMULATOR_SYSTEM_PRESSURE;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_TLA_POSITION_LEFT;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_TLA_POSITION_RIGHT;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_MASTER_TIME;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_AIRSPEED;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_DATE_MASTER;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_LG_GEARDOWN_LOCKED;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_COMPUTED_WOW_DATA1;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_COMPUTED_WOW_DATA2;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_AIRCRAFTACCELERATION_LEFT;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_AIRCRAFTACCELERATION_RIGHT;
    MCB_Data[add_num++].value=0;
    /*read Slave coll*/
     MCB_Data[add_num].Addr=ADDR_INBD_LT_TEMP | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=ADDR_INBD_RT_TEMP | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=ADDR_INBD_LT_BPSI | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=ADDR_INBD_RT_BPSI | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=ADDR_LEFTPPEDAL | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=ADDR_RIGHTPPEDAL | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=ADDR_LEFTCPEDAL | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=ADDR_RIGHTCPEDAL | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=ADDR_INNER_WHEEL_ABS_START_SIGNAL | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=ADDR_INBOARD_SHUTOFF_VALVE_CURRENT | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=ADDR_LEFT_INBOARD_BRAKE_CONTROL_VALVE_CURRENT | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=ADDR_RIGHT_INBOARD_BRAKE_CONTROL_VALVE_CURRENT | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=ADDR_LEFTWHEELSPEED | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=ADDR_RIGHTWHEELSPEED | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=ADDR_AUTO_BRAKE_SW_SIGNAL | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=ADDR_OTHER_SW_SIGNAL | 0x800;
     MCB_Data[add_num++].value=0;
     /*read Slave 429*/
     MCB_Data[add_num].Addr=OTHER_ADDR_HYDRAULIC_SYSTEM_PRESSURE | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=OTHER_ADDR_ACCUMULATOR_SYSTEM_PRESSURE | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=OTHER_ADDR_TLA_POSITION_LEFT | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=OTHER_ADDR_TLA_POSITION_RIGHT | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=OTHER_ADDR_MASTER_TIME | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=OTHER_ADDR_AIRSPEED | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=OTHER_ADDR_DATE_MASTER | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=OTHER_ADDR_LG_GEARDOWN_LOCKED | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=OTHER_ADDR_COMPUTED_WOW_DATA1 | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=OTHER_ADDR_COMPUTED_WOW_DATA2 | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=OTHER_ADDR_AIRCRAFTACCELERATION_LEFT | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=OTHER_ADDR_AIRCRAFTACCELERATION_RIGHT | 0x800;
     MCB_Data[add_num++].value=0;
     /*write Slave coll*/
     MCB_Data[add_num].Addr=ADDR_TEST_DAC_BCV_LT | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=ADDR_TEST_DAC_BCV_RT | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=ADDR_TEST_DAC_CV_LT | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=ADDR_TEST_DAC_CV_RT | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=ADDR_TEST_DAC_WHELL_SPEED_LT | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=ADDR_TEST_DAC_WHELL_SPEED_RT | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=ADDR_TEST_DAC_SOV | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=ADDR_TEST_DAC_PLT_LT | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=ADDR_TEST_DAC_PLT_RT | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=ADDR_TEST_DAC_CPLT_LT | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=ADDR_TEST_DAC_CPLT_RT | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=ADDR_SPINUP_SIGNAL | 0x800;
     MCB_Data[add_num++].value=0;
     /*write Slave 429*/
     MCB_Data[add_num].Addr=OTHER_ADDR_LEFT_WHEEL_SPEED | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=OTHER_ADDR_RIGHT_WHEEL_SPEED | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=OTHER_ADDR_LEFT_BRAKE_CONTROL_VALVE_CURENT | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=OTHER_ADDR_RIGHT_BRAKE_CONTROL_VALVE_CURENT | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=OTHER_ADDR_LEFT_BRAKE_PRESSURE | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=OTHER_ADDR_RIGHT_BRAKE_PRESSURE | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=OTHER_ADDR_SHUTOFF_VALVE_CURRENT | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=OTHER_ADDR_LEFT_BRAKE_TEMPERATURE | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=OTHER_ADDR_RIGHT_BRAKE_TEMPERATURE | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=OTHER_ADDR_LEFT_BRAKE_PEDAL_POSITION_PILOT | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=OTHER_ADDR_RIGHT_BRAKE_PEDAL_POSITION_PILOT | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=OTHER_ADDR_LEFT_BRAKE_PEDAL_POSITION_COPILOT | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=OTHER_ADDR_RIGHT_BRAKE_PEDAL_POSITION_COPILOT | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=OTHER_ADDR_WARNING | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=OTHER_ADDR_STATUS_1 | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=OTHER_ADDR_STATUS_2 | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=OTHER_ADDR_BCU_AIRCRAFT_REFERENCE_SPEED | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=OTHER_ADDR_FAILURE_WORD_1 | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=OTHER_ADDR_FAILURE_WORD_2 | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=OTHER_ADDR_FAILURE_WORD_3 | 0x800;
     MCB_Data[add_num++].value=0;
     MCB_Data[add_num].Addr=OTHER_ADDR_FAILURE_WORD_4 | 0x800;
     MCB_Data[add_num++].value=0;
     SDRAM_READ_DATA_LEN=add_num;
     temp=add_num;
    /*write Master coll*/
    MCB_Data[add_num].Addr=ADDR_TEST_DAC_BCV_LT;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_TEST_DAC_BCV_RT;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_TEST_DAC_CV_LT;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_TEST_DAC_CV_RT;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_TEST_DAC_WHELL_SPEED_LT;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_TEST_DAC_WHELL_SPEED_RT;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_TEST_DAC_SOV;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_TEST_DAC_PLT_LT;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_TEST_DAC_PLT_RT;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_TEST_DAC_CPLT_LT;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_TEST_DAC_CPLT_RT;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_SPINUP_SIGNAL;
    MCB_Data[add_num++].value=0;
    /*write Master 429*/
    MCB_Data[add_num].Addr=ADDR_LEFT_WHEEL_SPEED;
    MCB_Data[add_num++].value=0x60000006 | (Board_ID<<8);
    MCB_Data[add_num].Addr=ADDR_RIGHT_WHEEL_SPEED;
    MCB_Data[add_num++].value=0x60000007 | (Board_ID<<8);
    MCB_Data[add_num].Addr=ADDR_LEFT_BRAKE_CONTROL_VALVE_CURENT;
    MCB_Data[add_num++].value=0x60000029 | (Board_ID<<8);
    MCB_Data[add_num].Addr=ADDR_RIGHT_BRAKE_CONTROL_VALVE_CURENT;
    MCB_Data[add_num++].value=0x6000002A | (Board_ID<<8);
    MCB_Data[add_num].Addr=ADDR_LEFT_BRAKE_PRESSURE;
    MCB_Data[add_num++].value=0x60000038 | (Board_ID<<8);
    MCB_Data[add_num].Addr=ADDR_RIGHT_BRAKE_PRESSURE;
    MCB_Data[add_num++].value=0x60000039 | (Board_ID<<8);
    MCB_Data[add_num].Addr=ADDR_SHUTOFF_VALVE_CURRENT;
    MCB_Data[add_num++].value=0x6000004B | (Board_ID<<8);
    MCB_Data[add_num].Addr=ADDR_LEFT_BRAKE_TEMPERATURE;
    MCB_Data[add_num++].value=0x6000004C | (Board_ID<<8);
    MCB_Data[add_num].Addr=ADDR_RIGHT_BRAKE_TEMPERATURE;
    MCB_Data[add_num++].value=0x6000004E | (Board_ID<<8);
    MCB_Data[add_num].Addr=ADDR_LEFT_BRAKE_PEDAL_POSITION_PILOT;
    MCB_Data[add_num++].value=0x60000079 | (Board_ID<<8);
    MCB_Data[add_num].Addr=ADDR_RIGHT_BRAKE_PEDAL_POSITION_PILOT;
    MCB_Data[add_num++].value=0x6000007A | (Board_ID<<8);
    MCB_Data[add_num].Addr=ADDR_LEFT_BRAKE_PEDAL_POSITION_COPILOT;
    MCB_Data[add_num++].value=0x6000007B | (Board_ID<<8);
    MCB_Data[add_num].Addr=ADDR_RIGHT_BRAKE_PEDAL_POSITION_COPILOT;
    MCB_Data[add_num++].value=0x6000007C | (Board_ID<<8);
    MCB_Data[add_num].Addr=ADDR_WARNING;
    MCB_Data[add_num++].value=0x000000B8 | (Board_ID<<8);
    MCB_Data[add_num].Addr=ADDR_STATUS_1;
    MCB_Data[add_num++].value=0x000000B9 | (Board_ID<<8);
    MCB_Data[add_num].Addr=ADDR_STATUS_2;
    MCB_Data[add_num++].value=0x000000BA | (Board_ID<<8);
    MCB_Data[add_num].Addr=ADDR_BCU_AIRCRAFT_REFERENCE_SPEED;
    MCB_Data[add_num++].value=0x600000C2 | (Board_ID<<8);
    MCB_Data[add_num].Addr=ADDR_FAILURE_WORD_1;
    MCB_Data[add_num++].value=0x000000E8 | (Board_ID<<8);
    MCB_Data[add_num].Addr=ADDR_FAILURE_WORD_2;
    MCB_Data[add_num++].value=0x000000E9 | (Board_ID<<8);
    MCB_Data[add_num].Addr=ADDR_FAILURE_WORD_3;
    MCB_Data[add_num++].value=0x000000EA | (Board_ID<<8);
    MCB_Data[add_num].Addr=ADDR_FAILURE_WORD_4;
    MCB_Data[add_num++].value=0x000000EB | (Board_ID<<8);
    SDRAM_WRITE_DATA_LEN=add_num-temp;
    /*MCB DSP 用于串口测试*/
    MCB_Data[add_num].Addr=ADDR_BrakePressureCommand_L;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_BrakePressureCommand_R;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_CoilEnergized;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_SOV_Open;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_SD_CARD;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_BEANCH_MODE;
    MCB_Data[add_num++].value=0;
    MCB_Data[add_num].Addr=ADDR_DSP_MODE;
    MCB_Data[add_num++].value=0;
    Device_DATA_LEN=add_num;
    Receive_Machine_Parameters.Right_Inboard_Brake_Control_Valve_Current=0;
    Receive_Machine_Parameters.Left_Inboard_Brake_Control_Valve_Current=0;
    Receive_Machine_Parameters.Inboard_Shutoff_Valve_Current=0;
    Receive_Machine_Parameters.Inner_wheel_ABS_Valve_Current=0;
    Receive_Machine_Parameters.INBD_RT_BPSI=0;
    Receive_Machine_Parameters.INBD_LT_BPSI=0;
    Receive_Machine_Parameters.INBD_RT_TEMP=0;
    Receive_Machine_Parameters.INBD_LT_TEMP=0;
    Receive_Machine_Parameters.LeftCptPedal.Valid=1;
    Receive_Machine_Parameters.LeftCptPedal.Value=0;
    Receive_Machine_Parameters.RightCptPedal.Valid=1;
    Receive_Machine_Parameters.RightCptPedal.Value=0;
    Receive_Machine_Parameters.LeftPPedal.Valid=1;
    Receive_Machine_Parameters.LeftPPedal.Value=0;
    Receive_Machine_Parameters.RightPPedal.Valid=1;
    Receive_Machine_Parameters.RightPPedal.Value=0;
    Receive_Machine_Parameters.MaxPedal=0;
    Receive_Machine_Parameters.Inner_wheel_ABS_start_signal=0;
    Receive_Machine_Parameters.Parking_brake_signal=0;
    Receive_Machine_Parameters.SpoilerStoredSignal_Left=0;
    Receive_Machine_Parameters.SpoilerStoredSignal_Right=0;
    Receive_Machine_Parameters.LeftWOW=0;
    Receive_Machine_Parameters.RightWOW=0;
    Receive_Machine_Parameters.zong.Valid=1;
    Receive_Machine_Parameters.zong.Value=0;
    Receive_Machine_Parameters.NLGWOW=0;
    Receive_Machine_Parameters.LeftThrottleIdle=0;
    Receive_Machine_Parameters.RightThrottleIdle=0;
    Receive_Machine_Parameters.ThrottleIdleAtLeastOne=0;
    Receive_Machine_Parameters.ThrottleIdleBoth=0;
    Receive_Machine_Parameters.LandingGearExtentionandRetractionCommand=0;
    Receive_Machine_Parameters.AutoBRK_OFF.Valid=1;
    Receive_Machine_Parameters.AutoBRK_OFF.Value=0;
    Receive_Machine_Parameters.AutoBRK_LOW.Valid=1;
    Receive_Machine_Parameters.AutoBRK_LOW.Value=0;
    Receive_Machine_Parameters.AutoBRK_MED.Valid=1;
    Receive_Machine_Parameters.AutoBRK_MED.Value=0;
    Receive_Machine_Parameters.AutoBRK_HI.Valid=1;
    Receive_Machine_Parameters.AutoBRK_HI.Value=0;
    Receive_Machine_Parameters.AutoBRK_RTO.Valid=1;
    Receive_Machine_Parameters.AutoBRK_RTO.Value=0;
    Receive_Machine_Parameters.LeftWheelSpeed.Valid=1;
    Receive_Machine_Parameters.LeftWheelSpeed.Value=0;
    Receive_Machine_Parameters.RightWheelSpeed.Valid=1;
    Receive_Machine_Parameters.RightWheelSpeed.Value=0;
    Receive_Machine_Parameters.WheelSpeed_av=0;
    Receive_Machine_Parameters.spare=0;
    Receive_Machine_Parameters.PIN_PGR_1=0;
    Receive_Machine_Parameters.PIN_PGR_2=0;
    Receive_Machine_Parameters.Work_mode=0;
    Receive_Machine_Parameters.Master_Salve=0;
    Receive_Machine_Parameters.Bench_mode=0;
    Receive_Machine_Parameters.RX_429_Communication.Hydraulic_System_Pressure_1_60=0;
    Receive_Machine_Parameters.RX_429_Communication.Accumulator_System_Pressure_1_126=0;
    Receive_Machine_Parameters.RX_429_Communication.TLA_Position_Left_133=0;
    Receive_Machine_Parameters.RX_429_Communication.TLA_Position_Right_133=0;
    Receive_Machine_Parameters.RX_429_Communication.Master_Time_150=0;
    Receive_Machine_Parameters.RX_429_Communication.Date_Master_260=0;
    Receive_Machine_Parameters.RX_429_Communication.Computed_WOW_Data_1_277=0;
    Receive_Machine_Parameters.RX_429_Communication.Computed_WOW_Data_2_277=0;
    Receive_Machine_Parameters.RX_429_Communication.Airspeed.Valid=1;
    Receive_Machine_Parameters.RX_429_Communication.Airspeed.Value=0;
    Receive_Machine_Parameters.RX_429_Communication.AircraftAcceleration_Left.Valid=1;
    Receive_Machine_Parameters.RX_429_Communication.AircraftAcceleration_Left.Value=0;
    Receive_Machine_Parameters.RX_429_Communication.AircraftAcceleration1_Right.Valid=1;
    Receive_Machine_Parameters.RX_429_Communication.AircraftAcceleration1_Right.Value=0;
    Receive_Machine_Parameters.RX_429_Communication.In_MLG_LtDownlocked=0;
    Receive_Machine_Parameters.RX_429_Communication.In_MLG_RtDownlocked=0;
    Transmit_Machine_Parameters.SOV_Open=0;
    Transmit_Machine_Parameters.BrakePressureCommand_L=0;
    Transmit_Machine_Parameters.BrakePressureCommand_R=0;
    Transmit_Machine_Parameters.CoilEnergized=0;
    Transmit_Machine_Parameters.ARM_SIG=0;
    Transmit_Machine_Parameters.INBD_LT_SPINUP=0;
    Transmit_Machine_Parameters.INBD_RT_SPINUP=0;
    Transmit_Machine_Parameters.TEST_DAC_BCV_LT=0;
    Transmit_Machine_Parameters.TEST_DAC_BCV_RT=0;
    Transmit_Machine_Parameters.TEST_DAC_CV_LT=0;
    Transmit_Machine_Parameters.TEST_DAC_CV_RT=0;
    Transmit_Machine_Parameters.TEST_DAC_WHELL_SPEED_LT=0;
    Transmit_Machine_Parameters.TEST_DAC_WHELL_SPEED_RT=0;
    Transmit_Machine_Parameters.TEST_DAC_SOV=0;
    Transmit_Machine_Parameters.TEST_DAC_PLT_LT=0;
    Transmit_Machine_Parameters.TEST_DAC_PLT_RT=0;
    Transmit_Machine_Parameters.TEST_DAC_CPLT_LT=0;
    Transmit_Machine_Parameters.TEST_DAC_CPLT_RT=0;
    Transmit_Machine_Parameters.TX_429_Communication.Left_Inboard_Wheel_Speed_006=0;
    Transmit_Machine_Parameters.TX_429_Communication.Right_Inboard_Wheel_Speed_007=0;
    Transmit_Machine_Parameters.TX_429_Communication.Right_Inboard_Brake_Control_Valve_Current_052=0;
    Transmit_Machine_Parameters.TX_429_Communication.Left_Inboard_Brake_Control_Valve_Current_051=0;
    Transmit_Machine_Parameters.TX_429_Communication.Left_Inboard_Brake_Pressure_070=0;
    Transmit_Machine_Parameters.TX_429_Communication.Right_Inboard_Brake_Pressure_071=0;
    Transmit_Machine_Parameters.TX_429_Communication.Inboard_Shutoff_Valve_Current_113=0;
    Transmit_Machine_Parameters.TX_429_Communication.Left_Inboard_Brake_Temperature_114=0;
    Transmit_Machine_Parameters.TX_429_Communication.Right_Inboard_Brake_Temperature_116=0;
    Transmit_Machine_Parameters.TX_429_Communication.Left_Brake_Pedal_Position_Pilot_171=0;
    Transmit_Machine_Parameters.TX_429_Communication.Right_Brake_Pedal_Position_Pilot_172=0;
    Transmit_Machine_Parameters.TX_429_Communication.Left_Brake_Pedal_Position_Copilot_173=0;
    Transmit_Machine_Parameters.TX_429_Communication.Right_Brake_Pedal_Position_Copilot_174=0;
    Transmit_Machine_Parameters.TX_429_Communication.Warning_270=0;
    Transmit_Machine_Parameters.TX_429_Communication.Status_Word_1__271=0;
    Transmit_Machine_Parameters.TX_429_Communication.Status_Word_2_272=0;
    Transmit_Machine_Parameters.TX_429_Communication.BCU_Aircraft_Reference_Speed_302=0;
    Transmit_Machine_Parameters.TX_429_Communication.Failure_Word_1_350=0;
    Transmit_Machine_Parameters.TX_429_Communication.Failure_Word_1_351=0;
    Transmit_Machine_Parameters.TX_429_Communication.Failure_Word_1_352=0;
    Transmit_Machine_Parameters.TX_429_Communication.Failure_Word_1_353=0;
    device_status.Work_Mode=NORMAL_MODE;
    device_status.ABSW_Status=0;
    device_status.SOV_Status=0;
    device_status.BrakePressureCommand_L=0;
    device_status.BrakePressureCommand_R=0;
}


/**********************
 * void SD_GPIO_Init(void)
 * SD卡的I/O口初始化
 * ********************/
void SD_GPIO_Init(void)
{
    spiREG3->PC0 &=(~0x00000fff);    /*mbspi cs ENA CLK SIMO SOMI as i/o  to SCLK CS MOSI MISO     0--GPIO*/
    spiREG3->PC1 |=0x00000806;       /*SD_CLK 、 SD_CMD  、SD_DATA3  output     0--INPUT  1--OUTPUT*/
    spiREG3->PC1 &=(~0x00000101);    /*SD_DAT0 、SD_CD              INtput     0--INPUT  1--OUTPUT*/
}

/**********************
 * void Other_GPIO_Init(void)
 * 其它I/O口的配置
 * ********************/
void Other_GPIO_Init(void)
{
    canREG1->TIOC &=(~0x00000008);   /*can1_tx as i/o  to DSP_CL_N_SOV */
    canREG1->TIOC |= 0x00000004;     /*can1_tx output */
    canREG3->TIOC &=(~0x00000008);   /*can3_tx as i/o  to DSP_CL_AB_SW_SOL */
    canREG3->TIOC |= 0x00000004;     /*can3_tx output */
    canREG3->RIOC &=(~0x0000000C);   /*can3_rx as i/o  to DSP_M_S_MODE  intput*/
    canREG1->RIOC &=(~0x0000000C);   /*can1_rx as i/o  to DSP_BENCH_MODE  intput*/
}
/**********************
 * void Get_Master_Slave(void)
 * 获取板卡主从状态
 * 板卡ID--Board_ID
 * 主板==Inboard ==A==0x001
 * 从板==Outboard==B==0x002
 * ********************/
void Get_Master_Slave(void)
{
    unsigned short i=0;
    if(((canREG3->RIOC) & 0x00000001)!=0) /*内轮板*/
    {
        Board_ID=0x001;
        Other_Board_ID=0x002;
        device_status.Master_Salve_mode=INBOARD;
    }
    else
    {
        Board_ID=0x002;
        Other_Board_ID=0x001;
        device_status.Master_Salve_mode=OUTBOARD;
    }
    for(i=(SDRAM_READ_DATA_LEN+SDRAM_WRITE_DATA_LEN);i<Device_DATA_LEN;i++)
    {
        if(MCB_Data[i].Addr==ADDR_DSP_MODE)
        {
            MCB_Data[i].value=device_status.Master_Salve_mode;
            break;
        }
    }
}

/**********************
 * void Get_Bench_Mode(void)
 * 获取BENCH状态
 * ********************/
void Get_Bench_Mode(void)
{
    unsigned short i=0;
    if(((canREG1->RIOC) & 0x00000001)!=0) /**/
    {
        device_status.Bench_Mode=INBOARD;
    }
    else
    {
        device_status.Bench_Mode=OUTBOARD;
    }
    for(i=(SDRAM_READ_DATA_LEN+SDRAM_WRITE_DATA_LEN);i<Device_DATA_LEN;i++)
    {
        if(MCB_Data[i].Addr==ADDR_BEANCH_MODE)
        {
            MCB_Data[i].value=device_status.Bench_Mode;
            break;
        }
    }
}
