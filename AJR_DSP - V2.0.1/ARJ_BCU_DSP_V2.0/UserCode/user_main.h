/*
  ******************************************************************************
  * @file    user_Main.h
  * @author  JYS
  * @version V1.0.0
  * @date    2019-11-20
  * @brief   Init
  */

#ifndef __USER_MAIN_H
#define __USER_MAIN_H
/*=========================== sys_H ================================*/
#include "het.h"
#include "gio.h"
#include "spi.h"
#include "sci.h"
#include "het.h"
#include "rti.h"
#include "pinmux.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "reg_rti.h"
#include "reg_can.h"
#include "reg_mibspi.h"
#include "reg_spi.h"
#include "mibspi.h"
#include "sys_core.h"
/*============================ user_H ==============================*/
#include "sdcard.h"
#include "ff.h"
#include "ffconf.h"
#include "diskio.h"
#include "sdram.h"
#include "dac5689.h"
#include "delay.h"
#include "Brake.h"
#include "init.h"
#include "Device_control.h"
#include "test.h"
#include "rs232.h"
#include "SD_card.h"
#include "sdram.h"
#define MASTER 1
#define SLAVE 0
#define Master_Slave  MASTER
typedef  struct _429_DATA_RX
{
    float    Hydraulic_System_Pressure_1_60;    /*油源压力1*/
    float    Accumulator_System_Pressure_1_126; /*蓄压器压力1*/
    float    TLA_Position_Left_133;             /*左油门杆位置*/
    float    TLA_Position_Right_133;            /*右油门杆位置*/
    unsigned int    Master_Time_150;                   /*主机时间*/
    unsigned int    Date_Master_260;                   /*主机日期*/
    unsigned int    Computed_WOW_Data_1_277;           /*计算轮载数据1*/
    unsigned int    Computed_WOW_Data_2_277;           /*计算轮载数据2*/
    tValidityF32 Airspeed;                      /*Computed_Air_Speed_206--计算空速*/
    tValidityF32 AircraftAcceleration_Left;     /*Longitudinal_Acceleration_Body_Left_331--机体左侧纵向加速度*/
    tValidityF32 AircraftAcceleration1_Right;   /*Longitudinal_Acceleration_Body_Right_331--机体右侧纵向加速度*/
    kcg_bool In_MLG_RtDownlocked;                /*Gear_Position_1_272（Bit13）--起落架位置1*/
    kcg_bool In_MLG_LtDownlocked;                /*Gear_Position_1_272（Bit14）--起落架位置1*/

}RX_429_DATA;
typedef  struct _429_DATA_TX
{
    float    Left_Inboard_Wheel_Speed_006;                  /*--左轮轮速*/
    float    Right_Inboard_Wheel_Speed_007;                 /*--右轮轮速*/
    float    Right_Inboard_Brake_Control_Valve_Current_052; /*--右轮刹车控制阀电流*//*回采*/
    float    Left_Inboard_Brake_Control_Valve_Current_051;  /*--左轮刹车控制阀电流 *//*回采*/
    float    Left_Inboard_Brake_Pressure_070;               /*左轮刹车压力 */
    float    Right_Inboard_Brake_Pressure_071;              /*右轮刹车压力 */
    float    Inboard_Shutoff_Valve_Current_113;              /*切断阀电流检测 *//*回采*/
    float    Left_Inboard_Brake_Temperature_114;            /*左轮刹车温度 */
    float    Right_Inboard_Brake_Temperature_116;           /*右轮刹车温度 */
    float    Left_Brake_Pedal_Position_Pilot_171;           /*主驾左脚蹬*/
    float    Right_Brake_Pedal_Position_Pilot_172;          /*主驾右脚蹬*/
    float    Left_Brake_Pedal_Position_Copilot_173;         /*副驾左脚蹬*/
    float    Right_Brake_Pedal_Position_Copilot_174;        /*副驾右脚蹬*/
    unsigned int    Warning_270;                                   /*警告*/
    unsigned int    Status_Word_1__271;                            /*状态文字#1*/
    unsigned int    Status_Word_2_272;                             /*状态文字#2*/
    float    BCU_Aircraft_Reference_Speed_302;              /*BCU飞机参考速度*/
    unsigned int    Failure_Word_1_350;                            /*故障文字#1*/
    unsigned int    Failure_Word_1_351;                            /*故障文字#2*/
    unsigned int    Failure_Word_1_352;                            /*故障文字#3*/
    unsigned int    Failure_Word_1_353;                            /*故障文字#4*/
}TX_429_DATA;
typedef  struct RX_FPGA_DATA
{
        float    Right_Inboard_Brake_Control_Valve_Current; /*--右轮刹车控制阀电流*//*回采*/
        float    Left_Inboard_Brake_Control_Valve_Current;  /*--左轮刹车控制阀电流 *//*回采*/
        float    Inboard_Shutoff_Valve_Current;             /*切断阀电流检测 *//*回采*/
        float    Inner_wheel_ABS_Valve_Current;             /*自动刹车电流 *//*回采*/
        float    INBD_RT_BPSI;                              /*右内轮刹车压力 */
        float    INBD_LT_BPSI;                              /*左内轮刹车压力 */
        float    INBD_RT_TEMP;                              /*右内轮温度 */
        float    INBD_LT_TEMP;                              /*左内轮温度 */
        tValidityF32 LeftCptPedal;                          /*CH1_CPLT_LT_LVDT_Percent--副驾左脚蹬信号*/
        tValidityF32 RightCptPedal;                         /*CH1_CPLT_LT_LVDT_Percent--副驾右脚蹬信号*/
        tValidityF32 LeftPPedal;                            /*CH1_PLT_LT_LVDT_Percent--主驾左脚蹬信号*/
        tValidityF32 RightPPedal;                           /*CH1_PLT_LT_LVDT_Percent--主驾右脚蹬信号*/
        kcg_float32 MaxPedal;                               /*无*/
        bool Inner_wheel_ABS_start_signal   ;               /* 内轮ABS启动信号*/ /*回采*/
        bool Parking_brake_signal   ;                       /*停机刹车信号 */
        kcg_bool SpoilerStoredSignal_Left;                  /*INBD_IO_1（A22）--扰流板信号*/
        kcg_bool SpoilerStoredSignal_Right;                 /*INBD_IO_1（A22）--扰流板信号*/
        kcg_bool LeftWOW;                                   /*INBD_IO_1（A8）--轮载信号*/
        kcg_bool RightWOW;                                  /*INBD_IO_1（A8）--轮载信号*/
        tValidityBool zong;                                 /*INBD_IO_1（A8）--轮载信号*/
        kcg_bool NLGWOW;                                    /*无*/
        kcg_bool LeftThrottleIdle;                          /*INBD_IO_1（A20）--油门信号*/
        kcg_bool RightThrottleIdle;                         /*INBD_IO_1（A20）--油门信号*/
        kcg_bool ThrottleIdleAtLeastOne;                    /*INBD_IO_1（A20）--油门信号*/
        kcg_bool ThrottleIdleBoth;                          /*INBD_IO_1（A20）--油门信号*/
        kcg_bool LandingGearExtentionandRetractionCommand;  /*INBD_IO_1（A31）--手柄信号*/
        tValidityBool AutoBRK_OFF;                          /*INBD_ABS_SW(A16)--自动刹车开关的OFF挡*/
        tValidityBool AutoBRK_LOW;                          /*INBD_ABS_SW(A17)--自动刹车开关的LOW挡*/
        tValidityBool AutoBRK_MED;                          /*INBD_ABS_SW(A7)--自动刹车开关的MED挡*/
        tValidityBool AutoBRK_HI;                           /*INBD_ABS_SW(A6)--自动刹车开关的HI挡*/
        tValidityBool AutoBRK_RTO;                          /*INBD_ABS_SW(A15)--自动刹车开关的RTO挡*/
        tValidityF32 LeftWheelSpeed;                        /*INBD_LT_WST--左轮轮速*/
        tValidityF32 RightWheelSpeed;                       /*INBD_RT_WST--右轮轮速*/
        kcg_float32 WheelSpeed_av;                          /*(INBD_LT_WST+INBD_RT_WST）/2*/
        bool spare ;                                        /*备用信号 */
        bool PIN_PGR_1  ;                                   /*PIN PGR 1 */
        bool PIN_PGR_2  ;                                   /*PIN PGR 2 */
        bool Work_mode  ;                                   /*工作模式 */
        bool Bench_mode  ;                                  /*Bench模式 */
        bool Master_Salve  ;                                /*FPGA主从机模式 */
        RX_429_DATA RX_429_Communication;
} RX_FPGA_DATA;
typedef  struct TX_FPGA_DATA
{
        kcg_bool SOV_Open;                      /*INBD_SOV--内轮切断阀信号 */
        kcg_float32 BrakePressureCommand_L;     /*INBD_LT_BCV--左轮刹车控制阀 */
        kcg_float32 BrakePressureCommand_R;     /*INBD_RT_BCV-- 右轮刹车控制阀*/
        kcg_bool CoilEnergized;                 /*INBD_ABS_ARMED_EXC-- 自动刹车开关激励*/
        bool ARM_SIG;                           /*ARM_SIG */
        bool INBD_LT_SPINUP;                    /*左轮起转信号 */
        bool INBD_RT_SPINUP;                    /* 右轮起转信号*/
        float TEST_DAC_BCV_LT;                  /*测试信号左轮刹车控制阀 *//*输入范围0~0xffff对应0~5V*/
        float TEST_DAC_BCV_RT;                  /*测试信号右轮刹车控制阀 *//*输入范围0~0xffff对应0~5V*/
        float TEST_DAC_CV_LT;                   /*测试信号CV左 *//*输入范围0~0xffff对应0~5V*/
        float TEST_DAC_CV_RT;                   /*测试信号CV右 *//*输入范围0~0xffff对应0~5V*/
        float TEST_DAC_WHELL_SPEED_LT;          /*测试信号内轮轮速 LT *//*输入范围0~0xffff对应0~5V*/
        float TEST_DAC_WHELL_SPEED_RT;          /*测试信号内轮轮速 RT *//*输入范围0~0xffff对应0~5V*/
        float TEST_DAC_SOV;                     /*测试信号SOV *//*输入范围0~0xffff对应0~5V*/
        float TEST_DAC_PLT_LT;                  /*BRK PDL CH1 PLT LT  脚蹬信号 *//*输入范围0~0xffff对应0~5V*/
        float TEST_DAC_PLT_RT;                  /*BRK PDL CH1 PLT RT  脚蹬信号 *//*输入范围0~0xffff对应0~5V*/
        float TEST_DAC_CPLT_LT;                 /*BRK PDL CH1 CPLT LT  脚蹬信号 *//*输入范围0~0xffff对应0~5V*/
        float TEST_DAC_CPLT_RT;                 /*BRK PDL CH1 CPLT RT  脚蹬信号 *//*输入范围0~0xffff对应0~5V*/
        TX_429_DATA TX_429_Communication;
} TX_FPGA_DATA;

extern RX_FPGA_DATA Receive_Machine_Parameters; /*从FPGA接收的设备状态参数*/
extern TX_FPGA_DATA Transmit_Machine_Parameters;  /*发往FPGA控制设备状态的参数*/
extern uint8_t SD_Process_Flag;
extern uint16_t File_Name;
extern uint8_t SD_Process_Mode;
extern uint16_t get_bench_mode_time_count;

extern  float temp_value_right;   /*右轮温度值*/
extern  float temp_value_left;    /*左轮温度值*/
extern void user_main(void);
void SD_GPIO_Init(void);
extern uint8_t SDRAM_RxTx;

#endif /* __USER_MAIN_H */
