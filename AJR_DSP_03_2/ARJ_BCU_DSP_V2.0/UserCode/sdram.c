/*
 * sdram.c
 *
 *  Created on: 2020年9月12日
 *      Author: liaot
 */

#include "user_main.h"
SDRAM_DATA MCB_Data[DATA_LEN];/* 保存所有通信用到的地址和其对应的数据*/
/**************************************
 * void Sdram_Init(void)
 * 与FPGA通信的初始化
 * ************************************/
void Sdram_Init(void)
{
    hetPORT1->DIR &= (~ 0x0555DC00);    /*INPUT--DATA:10-12 14-16 18 20 22 24 26*/
    hetPORT1->DIR |= 0x500003BF;        /*OUTPUT--ADDR:0-5  7-9 28 30*/
    gioPORTA->DIR &= (~ 0X000000E4);    /*INPUT--DATA:2 5 6 7*/
    canREG2->TIOC &=(~0x00000008);      /*can2_tx as i/o  to ADDR_A6*/
    canREG2->TIOC |= 0x00000004;        /*can2_tx output*/
    canREG2->RIOC &=(~0x0000000C);      /*can2_Rx as i/o  to DATA_A12   INPUT*/
    mibspiREG5->PC0 &=(~0x00000E01);    /*mbspi_cS as i/o  to cS;  mbspi_clkx as i/o  to clk; mbspi_SOMI as i/o  to DWE;mbspi_SIMO as i/o  to FWE;*/
    mibspiREG5->PC1 |=(0x00000A01);     /*CS、CLK、DWE output*/
    mibspiREG5->PC1 &=(~0x00000400);    /*FWE intput*/
}

/**************************************
 * void SDRAM_Set_IO_To_Read_Mode(void)
 * 将SDRAM的IO设置为读模式
 * ************************************/
void SDRAM_Set_IO_To_Read_Mode(void)
{
    /*置1输出。   将IO扣配置为读取来自PFGA数据的模式*/
    hetPORT1->DIR &= (~ 0x0555DC00);    /*INPUT--DATA:10-12 14-16 18 20 22 24 26*/
    gioPORTA->DIR &= (~ 0X000000E4);    /*INPUT--DATA:2 5 6 7*/
    canREG2->RIOC &=(~0x00000004);      /*can2_Rx as i/o  to DATA_A12   INPUT*/
}

/**************************************
 * uint8_t SDRAM_Read_Data( uint8_t Addr,uint16_t *Read_data)
 * 从FPGA指定地址读取数据
 * 成功返回0，失败返回1；
 * ************************************/
uint8_t SDRAM_Read_Data( uint16_t Addr,uint32_t *Read_data)
{
    uint16_t time_out=100;
    *Read_data=0;
    if((Addr & 0x400) !=0)/*429数据*/
    {
        /*SET-ADDR*/
        gioSetBit(hetPORT1,PIN_HET_0,GetBit(Addr,0));   /*[A0]*/
        gioSetBit(hetPORT1,PIN_HET_1,GetBit(Addr,1));   /*[A1]*/
        gioSetBit(hetPORT1,PIN_HET_2,GetBit(Addr,2));   /*[A2]*/
        gioSetBit(hetPORT1,PIN_HET_3,GetBit(Addr,3));   /*[A3]*/
        gioSetBit(hetPORT1,PIN_HET_4,GetBit(Addr,4));   /*[A4]*/
        gioSetBit(hetPORT1,PIN_HET_5,GetBit(Addr,5));   /*[A5]*/
        can2_tx_SetBit(GetBit(Addr,6));                 /*[A6]*/
        gioSetBit(hetPORT1,PIN_HET_7,GetBit(Addr,7));   /*[A7]*/
        gioSetBit(hetPORT1,PIN_HET_8,GetBit(Addr,8));   /*[A8]*/
        gioSetBit(hetPORT1,PIN_HET_9,GetBit(Addr,9));   /*[A9]*/
        gioSetBit(hetPORT1,PIN_HET_28,GetBit(Addr,10)); /*[A10]*/
        gioSetBit(hetPORT1,PIN_HET_30,GetBit(Addr,11)); /*[A11]*/
        set_sdram_clk(HIGH);  /*读取低字节数据*/
        set_sdram_cs(HIGH);  /*工作模式为读取数据*/
        set_sdram_DWE(HIGH); /*DSP数据准备完成，告知FPGA读取数据*/
        time_out=SDRAM_TIME_OUT;
        while(Get_sdram_FWE()==LOW && time_out) /*等待DSP数据准备完成*/
        {
            time_out--;
        }
        /*GET-DATA*/
        *Read_data |=((gioGetBit(gioPORTA, 2)>0)  ? 0x00010000:0);
        *Read_data |=((gioGetBit(gioPORTA, 5)>0)  ? 0x00020000:0);
        *Read_data |=((gioGetBit(gioPORTA, 6)>0)  ? 0x00040000:0);
        *Read_data |=((gioGetBit(gioPORTA, 7)>0)  ? 0x00080000:0);
        *Read_data |=((gioGetBit(hetPORT1, 26)>0) ? 0x00100000:0);
        *Read_data |=((gioGetBit(hetPORT1, 24)>0) ? 0x00200000:0);
        *Read_data |=((gioGetBit(hetPORT1, 22)>0) ? 0x00400000:0);
        *Read_data |=((gioGetBit(hetPORT1, 20)>0) ? 0x00800000:0);
        *Read_data |=((gioGetBit(hetPORT1, 18)>0) ? 0x01000000:0);
        *Read_data |=((gioGetBit(hetPORT1, 16)>0) ? 0x02000000:0);
        *Read_data |=((gioGetBit(hetPORT1, 15)>0) ? 0x04000000:0);
        *Read_data |=((gioGetBit(hetPORT1, 14)>0) ? 0x08000000:0);
        *Read_data |=((get_sdram_DATA_A12()>0)    ? 0x10000000:0);
        *Read_data |=((gioGetBit(hetPORT1, 12)>0) ? 0x20000000:0);
        *Read_data |=((gioGetBit(hetPORT1, 11)>0) ? 0x40000000:0);
        *Read_data |=((gioGetBit(hetPORT1, 10)>0) ? 0x80000000:0);
        set_sdram_DWE(LOW); /*DSP读取数据完成，告知FPGA读取数据完成*/
    }
    /*SET-ADDR*/
    gioSetBit(hetPORT1,PIN_HET_0,GetBit(Addr,0));   /*[A0]*/
    gioSetBit(hetPORT1,PIN_HET_1,GetBit(Addr,1));   /*[A1]*/
    gioSetBit(hetPORT1,PIN_HET_2,GetBit(Addr,2));   /*[A2]*/
    gioSetBit(hetPORT1,PIN_HET_3,GetBit(Addr,3));   /*[A3]*/
    gioSetBit(hetPORT1,PIN_HET_4,GetBit(Addr,4));   /*[A4]*/
    gioSetBit(hetPORT1,PIN_HET_5,GetBit(Addr,5));   /*[A5]*/
    can2_tx_SetBit(GetBit(Addr,6));                 /*[A6]*/
    gioSetBit(hetPORT1,PIN_HET_7,GetBit(Addr,7));   /*[A7]*/
    gioSetBit(hetPORT1,PIN_HET_8,GetBit(Addr,8));   /*[A8]*/
    gioSetBit(hetPORT1,PIN_HET_9,GetBit(Addr,9));   /*[A9]*/
    gioSetBit(hetPORT1,PIN_HET_28,GetBit(Addr,10)); /*[A10]*/
    gioSetBit(hetPORT1,PIN_HET_30,GetBit(Addr,11)); /*[A11]*/
    set_sdram_clk(LOW);  /*读取低字节数据*/
    set_sdram_cs(HIGH);  /*工作模式为读取数据*/
    set_sdram_DWE(HIGH); /*DSP数据准备完成，告知FPGA读取数据*/
    time_out=SDRAM_TIME_OUT;
    while(Get_sdram_FWE()==LOW && time_out) /*等待DSP数据准备完成*/
    {
        time_out--;
    }
    /*GET-DATA*/
    *Read_data |=((gioGetBit(gioPORTA, 2)>>0)  ? 0x00000001:0);
    *Read_data |=((gioGetBit(gioPORTA, 5)>>0)  ? 0x00000002:0);
    *Read_data |=((gioGetBit(gioPORTA, 6)>>0)  ? 0x00000004:0);
    *Read_data |=((gioGetBit(gioPORTA, 7)>>0)  ? 0x00000008:0);
    *Read_data |=((gioGetBit(hetPORT1, 26)>>0) ? 0x00000010:0);
    *Read_data |=((gioGetBit(hetPORT1, 24)>>0) ? 0x00000020:0);
    *Read_data |=((gioGetBit(hetPORT1, 22)>>0) ? 0x00000040:0);
    *Read_data |=((gioGetBit(hetPORT1, 20)>>0) ? 0x00000080:0);
    *Read_data |=((gioGetBit(hetPORT1, 18)>>0) ? 0x00000100:0);
    *Read_data |=((gioGetBit(hetPORT1, 16)>>0) ? 0x00000200:0);
    *Read_data |=((gioGetBit(hetPORT1, 15)>>0) ? 0x00000400:0);
    *Read_data |=((gioGetBit(hetPORT1, 14)>>0) ? 0x00000800:0);
    *Read_data |=((get_sdram_DATA_A12()>>0)    ? 0x00001000:0);
    *Read_data |=((gioGetBit(hetPORT1, 12)>>0) ? 0x00002000:0);
    *Read_data |=((gioGetBit(hetPORT1, 11)>>0) ? 0x00004000:0);
    *Read_data |=((gioGetBit(hetPORT1, 10)>>0) ? 0x00008000:0);
    set_sdram_DWE(LOW); /*DSP读取数据完成，告知FPGA读取数据完成*/
    return 1;
}

/**************************************
 * void SDRAM_Read(void)
 * 从FPGA读取所有数据
 * ************************************/
void SDRAM_Read(void)
{
    unsigned char i=0;
    signed short temp=0;
    signed short pressure=0;
    SDRAM_Set_IO_To_Read_Mode();
    /*读取数据,FGPA采集的数据和429数据以及对板数据*/
    for(i=0;i<SDRAM_READ_DATA_LEN;i++)
    {
         SDRAM_Read_Data(MCB_Data[i].Addr,&MCB_Data[i].value);
    }
    /*以下将从FPGA读取到的数据赋值给设备的状态参数*/
    for(i=0;i<SDRAM_READ_DATA_LEN;i++)
    {
        temp=MCB_Data[i].value&0xffff;
        switch(MCB_Data[i].Addr)
        {
            case ADDR_INBD_LT_TEMP:
            {
//                Receive_Machine_Parameters.INBD_LT_TEMP=(float)(temp*5000.0/32768/10)*2;
//                if(Receive_Machine_Parameters.INBD_LT_TEMP>10)
//                {
//                    Receive_Machine_Parameters.INBD_LT_TEMP-20;
//                }
                Receive_Machine_Parameters.INBD_LT_TEMP=(float)temp_value_left;
            }
            break;
            case ADDR_INBD_RT_TEMP:
            {
//                Receive_Machine_Parameters.INBD_RT_TEMP=(float)(temp*5000.0/32768/10)*2;
//                if(Receive_Machine_Parameters.INBD_RT_TEMP>10)
//                {
//                    Receive_Machine_Parameters.INBD_RT_TEMP-20;
//                }
                Receive_Machine_Parameters.INBD_RT_TEMP=(float)temp_value_right;
            }
            break;
            case ADDR_INBD_LT_BPSI:
            {
            //    pressure=((float)(temp*5.0/32768))>=0.48?((float)(temp*5.0/32768))*8:0;
            //    Receive_Machine_Parameters.INBD_LT_BPSI=pressure;
                Receive_Machine_Parameters.INBD_LT_BPSI=((float)(temp*5.0/32768));
            }
            break;
            case ADDR_INBD_RT_BPSI:
            {
              //    pressure=((float)(temp*5.0/32768))>=0.48?((float)(temp*5.0/32768))*8:0;
              //    Receive_Machine_Parameters.INBD_RT_BPSI=pressure;
                Receive_Machine_Parameters.INBD_RT_BPSI=(float)(temp*5.0/32768);
            }
            break;
            case ADDR_LEFTPPEDAL:
            {
                pressure=(float)(temp*5.0/32768)>=0.28 ?((float)(temp*5.0/32768)-0.28)*100/2.76:0;
                Receive_Machine_Parameters.LeftPPedal.Value=pressure;
           //     Receive_Machine_Parameters.LeftPPedal.Value=(float)(temp*5.0/32768);
            }
            break;
            case ADDR_RIGHTPPEDAL:
            {
                pressure=(float)(temp*5.0/32768)>=0.28 ?((float)(temp*5.0/32768)-0.28)*100/2.76:0;
                Receive_Machine_Parameters.RightPPedal.Value=pressure;
            //    Receive_Machine_Parameters.RightPPedal.Value=(float)(temp*5.0/32768);
            }
            break;
            case ADDR_LEFTCPEDAL:
            {
                pressure=(float)(temp*5.0/32768)>=0.28 ?((float)(temp*5.0/32768)-0.28)*100/2.76:0;
                Receive_Machine_Parameters.LeftCptPedal.Value=pressure;
           //     Receive_Machine_Parameters.LeftCptPedal.Value=(float)(temp*5.0/32768);
            }
            break;
            case ADDR_RIGHTCPEDAL:
            {
               pressure=(float)(temp*5.0/32768)>=0.28 ?((float)(temp*5.0/32768)-0.28)*100/2.76:0;
               Receive_Machine_Parameters.RightCptPedal.Value=pressure;
          //     Receive_Machine_Parameters.RightCptPedal.Value=(float)(temp*5.0/32768);
            }
            break;
            case ADDR_INNER_WHEEL_ABS_START_SIGNAL:
            {
               Receive_Machine_Parameters.Inner_wheel_ABS_Valve_Current=(float)(temp*5.0/32768);
            }
            break;
            case ADDR_INBOARD_SHUTOFF_VALVE_CURRENT:
            {
               Receive_Machine_Parameters.Inboard_Shutoff_Valve_Current=(float)(temp*5.0/32768);
            }
            break;
            case ADDR_LEFT_INBOARD_BRAKE_CONTROL_VALVE_CURRENT:
            {
             //   Receive_Machine_Parameters.Left_Inboard_Brake_Control_Valve_Current=(float)(temp*33.0/32768);
               Receive_Machine_Parameters.Left_Inboard_Brake_Control_Valve_Current=(float)(temp*5.0/32768);
            }
            break;
            case ADDR_RIGHT_INBOARD_BRAKE_CONTROL_VALVE_CURRENT:
            {
            //    Receive_Machine_Parameters.Right_Inboard_Brake_Control_Valve_Current=(float)(temp*33.0/32768);
               Receive_Machine_Parameters.Right_Inboard_Brake_Control_Valve_Current=(float)(temp*5.0/32768);
            }
            break;
            case ADDR_LEFTWHEELSPEED:
            {
               Receive_Machine_Parameters.LeftWheelSpeed.Value=MCB_Data[i].value;
               Receive_Machine_Parameters.WheelSpeed_av=(Receive_Machine_Parameters.RightWheelSpeed.Value+Receive_Machine_Parameters.LeftWheelSpeed.Value)/2;
            }
            break;
            case ADDR_RIGHTWHEELSPEED:
            {
               Receive_Machine_Parameters.RightWheelSpeed.Value=MCB_Data[i].value;
               Receive_Machine_Parameters.WheelSpeed_av=(Receive_Machine_Parameters.RightWheelSpeed.Value+Receive_Machine_Parameters.LeftWheelSpeed.Value)/2;
            }
            break;
            case ADDR_AUTO_BRAKE_SW_SIGNAL:
            {
              Receive_Machine_Parameters.AutoBRK_OFF.Value=(MCB_Data[i].value >> 0) &0x01;
              Receive_Machine_Parameters.AutoBRK_LOW.Value=(MCB_Data[i].value >> 1) &0x01;
              Receive_Machine_Parameters.AutoBRK_MED.Value=(MCB_Data[i].value >> 2) &0x01;
              Receive_Machine_Parameters.AutoBRK_HI.Value=(MCB_Data[i].value >> 3) &0x01;
              Receive_Machine_Parameters.AutoBRK_RTO.Value=(MCB_Data[i].value >> 4) &0x01;
            }
            break;
            case ADDR_OTHER_SW_SIGNAL:
           {
               Receive_Machine_Parameters.RightWOW=Receive_Machine_Parameters.LeftWOW=(MCB_Data[i].value >> 0) &0x01;
               Receive_Machine_Parameters.SpoilerStoredSignal_Right=Receive_Machine_Parameters.SpoilerStoredSignal_Left=(MCB_Data[i].value >> 1) &0x01;
               Receive_Machine_Parameters.LeftThrottleIdle=Receive_Machine_Parameters.RightThrottleIdle=(MCB_Data[i].value >> 2) &0x01;
               Receive_Machine_Parameters.Inner_wheel_ABS_start_signal=(MCB_Data[i].value >> 3) &0x01;
               Receive_Machine_Parameters.Bench_mode=(MCB_Data[i].value >> 4) &0x01;
               Receive_Machine_Parameters.spare=(MCB_Data[i].value >> 5) &0x01;
               Receive_Machine_Parameters.LandingGearExtentionandRetractionCommand=(MCB_Data[i].value >> 6) &0x01;
               Receive_Machine_Parameters.PIN_PGR_1=(MCB_Data[i].value >> 7) &0x01;
               Receive_Machine_Parameters.PIN_PGR_2=(MCB_Data[i].value >> 8) &0x01;
               Receive_Machine_Parameters.Parking_brake_signal=(MCB_Data[i].value >> 9) &0x01;
               Receive_Machine_Parameters.Master_Salve=(MCB_Data[i].value >> 10) &0x01;
           }
           break;
           default:
           {
               if(Get_Odd_value(MCB_Data[i].value)==((MCB_Data[i].value>>31)&0x01))
               {
                     if(MCB_Data[i].Addr==ADDR_HYDRAULIC_SYSTEM_PRESSURE)
                     {
                         Receive_Machine_Parameters.RX_429_Communication.Hydraulic_System_Pressure_1_60=(MCB_Data[i].value>>16)&0xfff;
                     }
                     else if(MCB_Data[i].Addr==ADDR_ACCUMULATOR_SYSTEM_PRESSURE)
                     {
                         Receive_Machine_Parameters.RX_429_Communication.Accumulator_System_Pressure_1_126=(MCB_Data[i].value>>16)&0xfff;
                     }
                     else if(MCB_Data[i].Addr==ADDR_TLA_POSITION_LEFT)
                     {
                         Receive_Machine_Parameters.RX_429_Communication.TLA_Position_Left_133=((MCB_Data[i].value>>16)&0xfff)*0.03125;
                         if(((MCB_Data[i].value>>28)&0x01)==0x01)
                         {
                              Receive_Machine_Parameters.RX_429_Communication.TLA_Position_Left_133=0- Receive_Machine_Parameters.RX_429_Communication.TLA_Position_Left_133;
                         }
                     }
                     else if(MCB_Data[i].Addr==ADDR_TLA_POSITION_RIGHT)
                     {
                         Receive_Machine_Parameters.RX_429_Communication.TLA_Position_Right_133=((MCB_Data[i].value>>16)&0xfff)*0.03125;
                         if(((MCB_Data[i].value>>28)&0x01)==0x01)
                         {
                               Receive_Machine_Parameters.RX_429_Communication.TLA_Position_Right_133=0- Receive_Machine_Parameters.RX_429_Communication.TLA_Position_Right_133;
                         }
                     }
                     else if(MCB_Data[i].Addr==ADDR_MASTER_TIME)
                     {
                         Receive_Machine_Parameters.RX_429_Communication.Master_Time_150=(MCB_Data[i].value>>10)&0x7ffff;
                     }
                     else if(MCB_Data[i].Addr==ADDR_AIRSPEED)
                     {
                         Receive_Machine_Parameters.RX_429_Communication.Airspeed.Value=((MCB_Data[i].value>>14)&0x7fff)*0.0625;
                     }
                     else if(MCB_Data[i].Addr==ADDR_DATE_MASTER)
                     {
                         Receive_Machine_Parameters.RX_429_Communication.Date_Master_260=(MCB_Data[i].value>>10)&0x7ffff;
                     }
                     else if(MCB_Data[i].Addr==ADDR_LG_GEARDOWN_LOCKED)
                     {
                         Receive_Machine_Parameters.RX_429_Communication.In_MLG_RtDownlocked=(MCB_Data[i].value>>12)&0x01;
                         Receive_Machine_Parameters.RX_429_Communication.In_MLG_LtDownlocked=(MCB_Data[i].value>>13)&0x01;
                     }
                     else if(MCB_Data[i].Addr==ADDR_COMPUTED_WOW_DATA1)
                     {
                         Receive_Machine_Parameters.RX_429_Communication.Computed_WOW_Data_1_277=(MCB_Data[i].value>>10)&0xf;
                     }
                     else if(MCB_Data[i].Addr==ADDR_COMPUTED_WOW_DATA2)
                     {
                         Receive_Machine_Parameters.RX_429_Communication.Computed_WOW_Data_2_277=(MCB_Data[i].value>>10)&0xf;
                     }
                     else if(MCB_Data[i].Addr==ADDR_AIRCRAFTACCELERATION_LEFT)
                     {
                         Receive_Machine_Parameters.RX_429_Communication.AircraftAcceleration_Left.Value=((MCB_Data[i].value>>13)&0xffff)*0.00012;
                         if(((MCB_Data[i].value>>28)&0x01)==1)
                         {
                             Receive_Machine_Parameters.RX_429_Communication.AircraftAcceleration_Left.Value=0-Receive_Machine_Parameters.RX_429_Communication.AircraftAcceleration_Left.Value;
                         }
                     }
                     else if(MCB_Data[i].Addr==ADDR_AIRCRAFTACCELERATION_RIGHT)
                     {
                         Receive_Machine_Parameters.RX_429_Communication.AircraftAcceleration1_Right.Value=((MCB_Data[i].value>>13)&0xffff)*0.00012;
                         if(((MCB_Data[i].value>>28)&0x01)==1)
                         {
                               Receive_Machine_Parameters.RX_429_Communication.AircraftAcceleration1_Right.Value=0-Receive_Machine_Parameters.RX_429_Communication.AircraftAcceleration1_Right.Value;
                         }
                     }
                     else if(MCB_Data[i].Addr==(OTHER_ADDR_STATUS_1 | 1<<11))
                     {
                          Receive_Machine_Parameters.RX_429_Communication.LandingArmedState=((MCB_Data[i].value>>10)&0x01);
                          Receive_Machine_Parameters.RX_429_Communication.LandingApplicationState=((MCB_Data[i].value>>11)&0x01);
                          Receive_Machine_Parameters.RX_429_Communication.LandingEnabledState=((MCB_Data[i].value>>12)&0x01);
                          Receive_Machine_Parameters.RX_429_Communication.LandingDisArmedState=((MCB_Data[i].value>>13)&0x01);
                          Receive_Machine_Parameters.RX_429_Communication.RTOArmedState=((MCB_Data[i].value>>14)&0x01);
                          Receive_Machine_Parameters.RX_429_Communication.RTOEnabledState=((MCB_Data[i].value>>15)&0x01);
                          Receive_Machine_Parameters.RX_429_Communication.RTODisarmedState=((MCB_Data[i].value>>16)&0x01);
                          Receive_Machine_Parameters.RX_429_Communication.RTOApplicationState=((MCB_Data[i].value>>17)&0x01);
                          Receive_Machine_Parameters.RX_429_Communication.OFFState=((MCB_Data[i].value>>18)&0x01);
                     }
               }
           }
           break;
           }
    }
}
/**************************************
 * void SDRAM_Set_IO_To_Write_Mode(void)
 * 将SDRAM的IO设置为写模式
 * ************************************/
void SDRAM_Set_IO_To_Write_Mode(void)
{
      /*置1输出。将IO扣配置为发送数据到PFGA的模式*/
      hetPORT1->DIR |=  0x0555DC00;      /*0NPUT--DATA:10-12 14-16 18 20 22 24 26*/
      gioPORTA->DIR |=  0X000000E4;      /*0NPUT--DATA:2 5 6 7*/
      canREG2->RIOC |=  0x00000004;      /*can2_Rx  0NPUT*/
}

/**************************************
 * void SDRAM_Write_Data(uint8_t Addr, uint16_t Data)
 * 发送数据到FPGA指定地址
 *  成功返回0，失败返回1；
 * ************************************/
uint8_t SDRAM_Write_Data(uint16_t Addr, uint32_t Data)
{
    uint16_t time_out=100;
    if((Addr & 0x400) !=0)  /*429数据*/
    {
        /*SET-ADDR*/
        gioSetBit(hetPORT1,PIN_HET_0,GetBit(Addr,0));   /*[A0]*/
        gioSetBit(hetPORT1,PIN_HET_1,GetBit(Addr,1));   /*[A1]*/
        gioSetBit(hetPORT1,PIN_HET_2,GetBit(Addr,2));   /*[A2]*/
        gioSetBit(hetPORT1,PIN_HET_3,GetBit(Addr,3));   /*[A3]*/
        gioSetBit(hetPORT1,PIN_HET_4,GetBit(Addr,4));   /*[A4]*/
        gioSetBit(hetPORT1,PIN_HET_5,GetBit(Addr,5));   /*[A5]*/
        can2_tx_SetBit(GetBit(Addr,6));                 /*[A6]*/
        gioSetBit(hetPORT1,PIN_HET_7,GetBit(Addr,7));   /*[A7]*/
        gioSetBit(hetPORT1,PIN_HET_8,GetBit(Addr,8));   /*[A8]*/
        gioSetBit(hetPORT1,PIN_HET_9,GetBit(Addr,9));   /*[A9]*/
        gioSetBit(hetPORT1,PIN_HET_28,GetBit(Addr,10)); /*[A10]*/
        gioSetBit(hetPORT1,PIN_HET_30,GetBit(Addr,11)); /*[A11]*/
        /*SET-DATA*/
        /*DSP往数据线上写数据*/
        gioSetBit(gioPORTA,2,GetBit(Data,16));           /*[D0]*/
        gioSetBit(gioPORTA,5,GetBit(Data,17));           /*[D1]*/
        gioSetBit(gioPORTA,6,GetBit(Data,18));           /*[D2]*/
        gioSetBit(gioPORTA,7,GetBit(Data,19));           /*[D3]*/
        gioSetBit(hetPORT1,PIN_HET_26,GetBit(Data,20));  /*[D4]*/
        gioSetBit(hetPORT1,PIN_HET_24,GetBit(Data,21));  /*[D5]*/
        gioSetBit(hetPORT1,PIN_HET_22,GetBit(Data,22));  /*[D6]*/
        gioSetBit(hetPORT1,PIN_HET_20,GetBit(Data,23));  /*[D7]*/
        gioSetBit(hetPORT1,PIN_HET_18,GetBit(Data,24));  /*[D8]*/
        gioSetBit(hetPORT1,PIN_HET_16,GetBit(Data,25));  /*[D9]*/
        gioSetBit(hetPORT1,PIN_HET_15,GetBit(Data,26)); /*[D10]*/
        gioSetBit(hetPORT1,PIN_HET_14,GetBit(Data,27)); /*[D11]*/
        set_sdram_DATA_A12(GetBit(Data,28));            /*[D12]*/
        gioSetBit(hetPORT1,PIN_HET_12,GetBit(Data,29)); /*[D13]*/
        gioSetBit(hetPORT1,PIN_HET_11,GetBit(Data,30)); /*[D14]*/
        gioSetBit(hetPORT1,PIN_HET_10,GetBit(Data,31)); /*[D15]*/
        set_sdram_clk(HIGH);  /*读取低字节数据*/
        set_sdram_cs(LOW);  /*工作模式为读取数据*/
        set_sdram_DWE(HIGH); /*DSP数据准备完成，告知FPGA读取数据*/
        time_out=SDRAM_TIME_OUT;
        while(Get_sdram_FWE()==LOW && time_out) /*等待DSP数据准备完成*/
        {
            time_out--;
        }
        set_sdram_DWE(LOW); /*DSP读取数据完成，告知FPGA读取数据完成*/
    }
    /*SET-ADDR*/
    gioSetBit(hetPORT1,PIN_HET_0,GetBit(Addr,0));   /*[A0]*/
    gioSetBit(hetPORT1,PIN_HET_1,GetBit(Addr,1));   /*[A1]*/
    gioSetBit(hetPORT1,PIN_HET_2,GetBit(Addr,2));   /*[A2]*/
    gioSetBit(hetPORT1,PIN_HET_3,GetBit(Addr,3));   /*[A3]*/
    gioSetBit(hetPORT1,PIN_HET_4,GetBit(Addr,4));   /*[A4]*/
    gioSetBit(hetPORT1,PIN_HET_5,GetBit(Addr,5));   /*[A5]*/
    can2_tx_SetBit(GetBit(Addr,6));                 /*[A6]*/
    gioSetBit(hetPORT1,PIN_HET_7,GetBit(Addr,7));   /*[A7]*/
    gioSetBit(hetPORT1,PIN_HET_8,GetBit(Addr,8));   /*[A8]*/
    gioSetBit(hetPORT1,PIN_HET_9,GetBit(Addr,9));   /*[A9]*/
    gioSetBit(hetPORT1,PIN_HET_28,GetBit(Addr,10)); /*[A10]*/
    gioSetBit(hetPORT1,PIN_HET_30,GetBit(Addr,11)); /*[A11]*/
    /*SET-DATA*/
    /*DSP往数据线上写数据*/
    gioSetBit(gioPORTA,2,GetBit(Data,0));           /*[D0]*/
    gioSetBit(gioPORTA,5,GetBit(Data,1));           /*[D1]*/
    gioSetBit(gioPORTA,6,GetBit(Data,2));           /*[D2]*/
    gioSetBit(gioPORTA,7,GetBit(Data,3));           /*[D3]*/
    gioSetBit(hetPORT1,PIN_HET_26,GetBit(Data,4));  /*[D4]*/
    gioSetBit(hetPORT1,PIN_HET_24,GetBit(Data,5));  /*[D5]*/
    gioSetBit(hetPORT1,PIN_HET_22,GetBit(Data,6));  /*[D6]*/
    gioSetBit(hetPORT1,PIN_HET_20,GetBit(Data,7));  /*[D7]*/
    gioSetBit(hetPORT1,PIN_HET_18,GetBit(Data,8));  /*[D8]*/
    gioSetBit(hetPORT1,PIN_HET_16,GetBit(Data,9));  /*[D9]*/
    gioSetBit(hetPORT1,PIN_HET_15,GetBit(Data,10)); /*[D10]*/
    gioSetBit(hetPORT1,PIN_HET_14,GetBit(Data,11)); /*[D11]*/
    set_sdram_DATA_A12(GetBit(Data,12));            /*[D12]*/
    gioSetBit(hetPORT1,PIN_HET_12,GetBit(Data,13)); /*[D13]*/
    gioSetBit(hetPORT1,PIN_HET_11,GetBit(Data,14)); /*[D14]*/
    gioSetBit(hetPORT1,PIN_HET_10,GetBit(Data,15)); /*[D15]*/
    set_sdram_clk(LOW);  /*读取低字节数据*/
    set_sdram_cs(LOW);  /*工作模式为读取数据*/
    set_sdram_DWE(HIGH); /*DSP数据准备完成，告知FPGA读取数据*/
    time_out=SDRAM_TIME_OUT;
    while(Get_sdram_FWE()==LOW && time_out) /*等待DSP数据准备完成*/
    {
        time_out--;
    }
    if(time_out==0)
    {
        set_sdram_DWE(LOW); /*DSP读取数据完成，告知FPGA读取数据完成*/
        return 0;
    }
    set_sdram_DWE(LOW); /*DSP读取数据完成，告知FPGA读取数据完成*/
    return 1;
}

/**************************************
 * void SDRAM_Write(void)
 * 发送所有数据到FPGA
 * ************************************/
void SDRAM_Write(void)
{
    unsigned short i=0;
    SDRAM_Set_IO_To_Write_Mode();
    /*将控制设备的参数赋给发送到FPGA的数据*/
    for(i=SDRAM_READ_DATA_LEN;i<(SDRAM_WRITE_DATA_LEN+SDRAM_READ_DATA_LEN);i++)
    {
         switch(MCB_Data[i].Addr)
         {
             case ADDR_TEST_DAC_BCV_LT:
             {
                 MCB_Data[i].value=Transmit_Machine_Parameters.TEST_DAC_BCV_LT;
             }
             break;
             case ADDR_TEST_DAC_BCV_RT:
              {
                  MCB_Data[i].value=Transmit_Machine_Parameters.TEST_DAC_BCV_RT;
              }
              break;
             case ADDR_TEST_DAC_CV_LT:
              {
                  MCB_Data[i].value=Transmit_Machine_Parameters.TEST_DAC_CV_LT;
              }
              break;
             case ADDR_TEST_DAC_CV_RT:
              {
                  MCB_Data[i].value=Transmit_Machine_Parameters.TEST_DAC_CV_RT;
              }
              break;
             case ADDR_TEST_DAC_WHELL_SPEED_LT:
              {
                  MCB_Data[i].value=Transmit_Machine_Parameters.TEST_DAC_WHELL_SPEED_LT;
              }
              break;
             case ADDR_TEST_DAC_WHELL_SPEED_RT:
              {
                  MCB_Data[i].value=Transmit_Machine_Parameters.TEST_DAC_WHELL_SPEED_RT;
              }
              break;
             case ADDR_TEST_DAC_SOV:
              {
                  MCB_Data[i].value=Transmit_Machine_Parameters.TEST_DAC_SOV;
              }
              break;
             case ADDR_TEST_DAC_PLT_LT:
              {
                  MCB_Data[i].value=Transmit_Machine_Parameters.TEST_DAC_PLT_LT;
              }
              break;
             case ADDR_TEST_DAC_PLT_RT:
              {
                  MCB_Data[i].value=Transmit_Machine_Parameters.TEST_DAC_PLT_RT;
              }
              break;
             case ADDR_TEST_DAC_CPLT_LT:
              {
                  MCB_Data[i].value=Transmit_Machine_Parameters.TEST_DAC_CPLT_LT;
              }
              break;
             case ADDR_TEST_DAC_CPLT_RT:
              {
                  MCB_Data[i].value=Transmit_Machine_Parameters.TEST_DAC_CPLT_RT;
              }
              break;
             case ADDR_SPINUP_SIGNAL:
             {
                    MCB_Data[i].value=(Transmit_Machine_Parameters.INBD_LT_SPINUP>0 ? 0x01:0x00)| (Transmit_Machine_Parameters.INBD_RT_SPINUP>0 ? 0x02:0x00) | (Transmit_Machine_Parameters.ARM_SIG>0 ? 0x04:0x00);
             }
             break;
             default:
             {
                 MCB_Data[i].value &=0x600003ff;
                 if( MCB_Data[i].Addr==ADDR_LEFT_WHEEL_SPEED)
                 {
                     MCB_Data[i].value |=((unsigned int)(Transmit_Machine_Parameters.TX_429_Communication.Left_Inboard_Wheel_Speed_006/0.125)&0xfff)<<16;
                 }
                 else if( MCB_Data[i].Addr==ADDR_RIGHT_WHEEL_SPEED)
                 {
                     MCB_Data[i].value |=((unsigned int)(Transmit_Machine_Parameters.TX_429_Communication.Right_Inboard_Wheel_Speed_007/0.125)&0xfff)<<16;
                 }
                 else if( MCB_Data[i].Addr==ADDR_LEFT_BRAKE_CONTROL_VALVE_CURENT)
                 {
                     MCB_Data[i].value |=((unsigned int)(Transmit_Machine_Parameters.TX_429_Communication.Left_Inboard_Brake_Control_Valve_Current_051/0.1)&0xfff)<<16;
                 }
                 else if( MCB_Data[i].Addr==ADDR_RIGHT_BRAKE_CONTROL_VALVE_CURENT)
                 {
                     MCB_Data[i].value |=((unsigned int)(Transmit_Machine_Parameters.TX_429_Communication.Right_Inboard_Brake_Control_Valve_Current_052/0.1)&0xfff)<<16;
                 }
                 else if( MCB_Data[i].Addr==ADDR_LEFT_BRAKE_PRESSURE)
                 {
                     MCB_Data[i].value |=((unsigned int)(Transmit_Machine_Parameters.TX_429_Communication.Left_Inboard_Brake_Pressure_070)&0xfff)<<16;
                 }
                 else if( MCB_Data[i].Addr==ADDR_RIGHT_BRAKE_PRESSURE)
                 {
                     MCB_Data[i].value |=((unsigned int)(Transmit_Machine_Parameters.TX_429_Communication.Right_Inboard_Brake_Pressure_071)&0xfff)<<16;
                 }
                 else if( MCB_Data[i].Addr==ADDR_SHUTOFF_VALVE_CURRENT)
                 {
                     MCB_Data[i].value |=((unsigned int)(Transmit_Machine_Parameters.TX_429_Communication.Inboard_Shutoff_Valve_Current_113)&0xfff)<<16;
                 }
                 else if( MCB_Data[i].Addr==ADDR_LEFT_BRAKE_TEMPERATURE)
                 {
                     MCB_Data[i].value |=((unsigned int)(Transmit_Machine_Parameters.TX_429_Communication.Left_Inboard_Brake_Temperature_114)&0xfff)<<16;
                 }
                 else if( MCB_Data[i].Addr==ADDR_RIGHT_BRAKE_TEMPERATURE)
                 {
                     MCB_Data[i].value |=((unsigned int)(Transmit_Machine_Parameters.TX_429_Communication.Right_Inboard_Brake_Temperature_116)&0xfff)<<16;
                 }
                 else if( MCB_Data[i].Addr==ADDR_LEFT_BRAKE_PEDAL_POSITION_PILOT)
                 {
                     MCB_Data[i].value |=((unsigned int)(Transmit_Machine_Parameters.TX_429_Communication.Left_Brake_Pedal_Position_Pilot_171/0.01)&0xff)<<20;
                 }
                 else if( MCB_Data[i].Addr==ADDR_RIGHT_BRAKE_PEDAL_POSITION_PILOT)
                 {
                     MCB_Data[i].value |=((unsigned int)(Transmit_Machine_Parameters.TX_429_Communication.Right_Brake_Pedal_Position_Pilot_172/0.01)&0xff)<<20;
                 }
                 else if( MCB_Data[i].Addr==ADDR_LEFT_BRAKE_PEDAL_POSITION_COPILOT)
                 {
                     MCB_Data[i].value |=((unsigned int)(Transmit_Machine_Parameters.TX_429_Communication.Left_Brake_Pedal_Position_Copilot_173/0.01)&0xff)<<20;
                 }
                 else if( MCB_Data[i].Addr==ADDR_RIGHT_BRAKE_PEDAL_POSITION_COPILOT)
                 {
                     MCB_Data[i].value |=((unsigned int)(Transmit_Machine_Parameters.TX_429_Communication.Right_Brake_Pedal_Position_Copilot_174/0.01)&0xff)<<20;
                 }
                 else if( MCB_Data[i].Addr==ADDR_WARNING)
                 {
                     MCB_Data[i].value |=(Transmit_Machine_Parameters.TX_429_Communication.Warning_270&0x7ffff)<<10;
                 }
                 else if( MCB_Data[i].Addr==ADDR_STATUS_1)
                 {
                     MCB_Data[i].value |=(Transmit_Machine_Parameters.TX_429_Communication.Status_Word_1__271&0x7ffff)<<10;
                 }
                 else if( MCB_Data[i].Addr==ADDR_STATUS_2)
                 {
                     MCB_Data[i].value |=(Transmit_Machine_Parameters.TX_429_Communication.Status_Word_2_272&0x7ffff)<<10;
                 }
                 else if( MCB_Data[i].Addr==ADDR_BCU_AIRCRAFT_REFERENCE_SPEED)
                 {
                     MCB_Data[i].value |=((unsigned int)(Transmit_Machine_Parameters.TX_429_Communication.BCU_Aircraft_Reference_Speed_302/0.125)&0xfff)<<16;
                 }
                 else if( MCB_Data[i].Addr==ADDR_FAILURE_WORD_1)
                 {
                     MCB_Data[i].value |=(Transmit_Machine_Parameters.TX_429_Communication.Failure_Word_1_350&0x7ffff)<<10;
                 }
                 else if( MCB_Data[i].Addr==ADDR_FAILURE_WORD_2)
                 {
                     MCB_Data[i].value |=(Transmit_Machine_Parameters.TX_429_Communication.Failure_Word_1_351&0x7ffff)<<10;
                 }
                 else if( MCB_Data[i].Addr==ADDR_FAILURE_WORD_3)
                 {
                     MCB_Data[i].value |=(Transmit_Machine_Parameters.TX_429_Communication.Failure_Word_1_352&0x7ffff)<<10;
                 }
                 else if( MCB_Data[i].Addr==ADDR_FAILURE_WORD_4)
                 {
                     MCB_Data[i].value |=(Transmit_Machine_Parameters.TX_429_Communication.Failure_Word_1_353&0x7ffff)<<10;
                 }
                 MCB_Data[i].value |=(Get_Odd_value(MCB_Data[i].value)<<31);
            }
            break;

         }
    }
      /*发送地址0x40--0x60的数据*/
    for(i=SDRAM_READ_DATA_LEN;i<(SDRAM_WRITE_DATA_LEN+SDRAM_READ_DATA_LEN);i++)
     {
        SDRAM_Write_Data(MCB_Data[i].Addr,MCB_Data[i].value);
     }
}


/*********************************
 * uint8_t GetBit(int data, int index)
 * 得到数据第n位的值
 * ********************************/
uint8_t GetBit(unsigned int data, unsigned int index)
{
    return ((data & (1 << index)) > 0) ? 1: 0;
}

/***************************
 *void set_sdram_clk(uint8_t statue)
 * 设置Clk信号(F2-HL)（1--高字节；0--低字节）
 * statue：信号高低状态
 * **************************/
void set_sdram_clk(uint8_t statue)
{
    if(statue==1)
    {
        mibspiREG5->PC3 |=0x00000200;
    }
    else
    {
        mibspiREG5->PC3 &=(~0x00000200);
    }
}

/***************************
 * uint8_t Get_sdram_FWE()
 * 获取FWE信号(FPGA_DR)
 * statue：信号高低状态
 * **************************/
uint8_t Get_sdram_FWE(void)
{
    uint16_t recv=0;
    recv=(uint16_t)((mibspiREG5->PC2) &0x00000400)>0?1:0;
    return recv;
}

/***************************
 * uint16_t set_sdram_cs( void)
 * 设置CS信号(RW)（1--读；0--写）
 * 返回：信号高低状态
 * **************************/
void set_sdram_cs(uint8_t statue)
{
    if(statue==1)
    {
        mibspiREG5->PC3 |=0x00000001;
    }
    else
    {
        mibspiREG5->PC3 &=(~0x00000001);
    }
}

/***************************
 * uint16_t set_sdram_DWE( void)
 * 设置DWE信号(DSP_DR)
 * 返回：信号高低状态
 * **************************/
void set_sdram_DWE(uint8_t statue)
{
    if(statue==1)
    {
        mibspiREG5->PC3 |=0x00000800;
    }
    else
    {
        mibspiREG5->PC3 &=(~0x00000800);
    }
}

/***************************
 * uint16_t get_sdram_DATA_A12( void)
 * 获取数据线 12信号状态
 * 返回：信号高低状态
 * **************************/
uint16_t get_sdram_DATA_A12( void)
{
   uint16_t recv=0;
   recv=(uint16_t)((canREG2->RIOC) &0x00000001);
   return recv;
}

/***************************
 * void set_sdram_DATA_A12(uint8_t statue)
 * 设置数据线 12信号
 * statue：信号高低状态
 * **************************/
void set_sdram_DATA_A12(uint8_t statue)
{
    if(statue==0)
    {
       canREG2->RIOC &=(~0x00000002);
    }
    else
    {
        canREG2->RIOC |=0x00000002;
    }
}

/***************************
 * void can2_tx_SetBit(uint8_t statue)
 * 设置CAN2 Tx信号
 * statue：信号高低状态
 * **************************/
void can2_tx_SetBit(uint8_t statue)
{
   if(statue==0)
    {
       canREG2->TIOC &=(~0x00000002);
    }
    else
    {
        canREG2->TIOC |=0x00000002;
    }
}

/***************************
 * unsigned char Get_Odd_value(unsigned int data)
 * 获取偶校验值。
 * data：数据
 * **************************/
unsigned char Get_Odd_value(unsigned int data)
{
    unsigned char i=0;
    unsigned char num=0;
    for(i=0;i<31;i++)
    {
        if(((data>>i)&0x0001)==0x0001)
        {
            num++;
        }
    }
    if(num%2==0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
