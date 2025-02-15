/* $********** SCADE Suite KCG 64-bit 6.5 (build i12) ***********
** Command: kcg65.exe -config S:/CR929/BrakeControlLow/SCADE_Mod/BCS_Model_V3/BCS_Brake/KCG/config.txt
** Generation date: 2022-08-11T09:49:10
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "Brake.h"

#ifndef KCG_USER_DEFINED_INIT
void Brake_init(outC_Brake *outC)
{
  outC->Output.SOV_Open = kcg_true;
  outC->Output.BrakePressureCommand_L = kcg_lit_float32(0.0);
  outC->Output.BrakePressureCommand_R = kcg_lit_float32(0.0);
  outC->Output.BrakeAutoBKOut.AutoBrakeActivate = kcg_true;
  outC->Output.BrakeAutoBKOut.Left_BrakePressureCommand = kcg_lit_float32(0.0);
  outC->Output.BrakeAutoBKOut.Right_BrakePressureCommand = kcg_lit_float32(0.0);
  outC->Output.BrakeAutoBKOut.ArmDisagree = kcg_true;
  outC->Output.BrakeAutoBKOut.AutoBrakeState.LandingArmedState = kcg_true;
  outC->Output.BrakeAutoBKOut.AutoBrakeState.LandingApplicationState = kcg_true;
  outC->Output.BrakeAutoBKOut.AutoBrakeState.LandingEnabledState = kcg_true;
  outC->Output.BrakeAutoBKOut.AutoBrakeState.LandingDisArmedState = kcg_true;
  outC->Output.BrakeAutoBKOut.AutoBrakeState.RTOArmedState = kcg_true;
  outC->Output.BrakeAutoBKOut.AutoBrakeState.RTOEnabledState = kcg_true;
  outC->Output.BrakeAutoBKOut.AutoBrakeState.RTODisarmedState = kcg_true;
  outC->Output.BrakeAutoBKOut.AutoBrakeState.RTOApplicationState = kcg_true;
  outC->Output.BrakeAutoBKOut.AutoBrakeState.OFFState = kcg_true;
  outC->Output.BrakeAutoBKOut.CoilEnergized = kcg_true;
  outC->Output.BrakeAutoBKOut.testData.AUTOBRKfault = kcg_true;
  outC->Output.BrakeAutoBKOut.testData.ARMdisagree = kcg_true;
  outC->Output.BrakeAutoBKOut.testData.latch = kcg_true;
  outC->Output.BrakeAutoBKOut.testData.unlatch = kcg_true;
  outC->Output.BrakeAutoBKOut.testData.maxpedal = kcg_lit_float32(0.0);
  outC->Output.BrakeIfbOut.IFB_BrakeActivate = kcg_true;
  outC->Output.BrakeIfbOut.LeftIFB_BrakePressureCommand = kcg_lit_float32(0.0);
  outC->Output.BrakeIfbOut.RightIFB_BrakePressureCommand = kcg_lit_float32(0.0);
  outC->Output.BrakeInhibitOut.SOV_Open = kcg_true;
  outC->Output.BrakeInhibitOut.LeftBrakePressureCommandAdj = kcg_lit_float32(0.0);
  outC->Output.BrakeInhibitOut.RightBrakePressureCommandAdj = kcg_lit_float32(
      0.0);
  outC->Output.BrakeInhibitOut.BrakeInhibits.TDP_BrakeInhibit = kcg_true;
  outC->Output.BrakeInhibitOut.BrakeInhibits.LWP_LeftBrakeInhibit = kcg_true;
  outC->Output.BrakeInhibitOut.BrakeInhibits.LWP_RightBrakeInhibit = kcg_true;
  outC->Output.BrakePdlOut.LeftPDL_BrakePressureCommand = kcg_lit_float32(0.0);
  outC->Output.BrakePdlOut.RightPDL_BrakePressureCommand = kcg_lit_float32(0.0);
  outC->Output.BrakePdlOut.PedalBrakeActivate = kcg_true;
  outC->Output.BrakeSelectOut.SOV_Open = kcg_true;
  outC->Output.BrakeSelectOut.LeftBrakePressureCommand = kcg_lit_float32(0.0);
  outC->Output.BrakeSelectOut.RightBrakePressureCommand = kcg_lit_float32(0.0);
  outC->Output.BrakeSkidOut.LeftSkidEnable = kcg_true;
  outC->Output.BrakeSkidOut.LeftBrakePressureCommandAdj = kcg_lit_float32(0.0);
  outC->Output.BrakeSkidOut.Ratio = kcg_lit_float32(0.0);
  outC->Output.BrakeSkidOut.RightSkidEnable = kcg_true;
  outC->Output.BrakeSkidOut.RightBrakePressureCommandAdj = kcg_lit_float32(0.0);
  outC->Output.WOWBK = kcg_true;
  outC->Output.WOW429 = kcg_true;
  outC->Output.speed.LeftWheelSpeed.Valid = kcg_true;
  outC->Output.speed.LeftWheelSpeed.Value = kcg_lit_float32(0.0);
  outC->Output.speed.RightWheelSpeed.Valid = kcg_true;
  outC->Output.speed.RightWheelSpeed.Value = kcg_lit_float32(0.0);
  outC->Output.speed.LeftWheelRefSpeed.Valid = kcg_true;
  outC->Output.speed.LeftWheelRefSpeed.Value = kcg_lit_float32(0.0);
  outC->Output.speed.RightWheelRefSpeed.Valid = kcg_true;
  outC->Output.speed.RightWheelRefSpeed.Value = kcg_lit_float32(0.0);
  outC->Output.speed.LWP_RefSpeed.Valid = kcg_true;
  outC->Output.speed.LWP_RefSpeed.Value = kcg_lit_float32(0.0);
  outC->Output.speed.WheelSpeed_av = kcg_lit_float32(0.0);
  /* 3 */ TDP_init(&outC->_2_Context_3);
  /* 4 */ Lockwheel_init(&outC->Context_4);
  /* 3 */ Brake_Skid_init(&outC->_1_Context_3);
  /* 3 */ Spindown_init(&outC->Context_3);
  /* 2 */ AutoBrake_init(&outC->Context_2);
  /* 1 */ IO_Processing_init(&outC->Context_1);
}
#endif /* KCG_USER_DEFINED_INIT */


#ifndef KCG_NO_EXTERN_CALL_TO_RESET
void Brake_reset(outC_Brake *outC)
{
  /* 3 */ TDP_reset(&outC->_2_Context_3);
  /* 4 */ Lockwheel_reset(&outC->Context_4);
  /* 3 */ Brake_Skid_reset(&outC->_1_Context_3);
  /* 3 */ Spindown_reset(&outC->Context_3);
  /* 2 */ AutoBrake_reset(&outC->Context_2);
  /* 1 */ IO_Processing_reset(&outC->Context_1);
}
#endif /* KCG_NO_EXTERN_CALL_TO_RESET */

/* Brake */
void Brake(inC_Brake *inC, outC_Brake *outC)
{
  /* Brake::BrakeSelectOut */ tBrakeSelectOut BrakeSelectOut;
  /* Brake::BrakeSkidOut */ tBrakeSkidOut BrakeSkidOut;
  /* Brake::BrakeInhibitOut */ tBrakeInhibitOut BrakeInhibitOut;
  /* Brake::BrakePdlOut */ tBrakePdlOut BrakePdlOut;
  /* Brake::BrakeAutoBKOut */ tBrakeAutoBKOut BrakeAutoBKOut;
  /* Brake::BrakeIfbOut */ tBrakeIfbOut BrakeIfbOut;
  /* Brake::App */ tINPUT_Data App;
  /* Brake::_L62 */ kcg_bool _L62;
  /* Brake::_L61 */ kcg_bool _L61;

  /* 1 */ IO_Processing(&inC->Input, &outC->Context_1);
  kcg_copy_tINPUT_Data(&App, &outC->Context_1.APPData);
  outC->Output.WOWBK = App.WOWData.WOWBK;
  outC->Output.WOW429 = App.WOWData.zong.Value;
  kcg_copy_tIOR_ProcessSpeedData(&outC->Output.speed, &App.WheelSpeedData);
  /* 2 */ AutoBrake(&App, &outC->Context_2);
  kcg_copy_tBrakeAutoBKOut(&BrakeAutoBKOut, &outC->Context_2.BRAKEAuto);
  kcg_copy_tBrakeAutoBKOut(&outC->Output.BrakeAutoBKOut, &BrakeAutoBKOut);
  /* 3 */ BrakePress(&App, &BrakePdlOut);
  kcg_copy_tBrakePdlOut(&outC->Output.BrakePdlOut, &BrakePdlOut);
  /* 3 */ Spindown(&App, &outC->Context_3);
  kcg_copy_tBrakeIfbOut(&BrakeIfbOut, &outC->Context_3.BrakeIfbOut);
  kcg_copy_tBrakeIfbOut(&outC->Output.BrakeIfbOut, &BrakeIfbOut);
  /* 1 */
  BrakeSelect(&BrakePdlOut, &BrakeIfbOut, &BrakeAutoBKOut, &BrakeSelectOut);
  kcg_copy_tBrakeSelectOut(&outC->Output.BrakeSelectOut, &BrakeSelectOut);
  /* 3 */ Brake_Skid(&App, &BrakeSelectOut, &outC->_1_Context_3);
  kcg_copy_tBrakeSkidOut(&BrakeSkidOut, &outC->_1_Context_3.BrakeSkid);
  kcg_copy_tBrakeSkidOut(&outC->Output.BrakeSkidOut, &BrakeSkidOut);
  /* 4 */ Lockwheel(&App, &outC->Context_4);
  _L61 = outC->Context_4.LDPActive_Left;
  _L62 = outC->Context_4.LDPActive_Rightt;
  /* 3 */ TDP(&App, &BrakeIfbOut, &outC->_2_Context_3);
  /* 1 */
  BrakeInhibit(
    outC->_2_Context_3.TDP,
    _L61,
    _L62,
    &BrakeSelectOut,
    &BrakeSkidOut,
    &BrakeInhibitOut);
  outC->Output.SOV_Open = BrakeInhibitOut.SOV_Open;
  outC->Output.BrakePressureCommand_L =
    BrakeInhibitOut.LeftBrakePressureCommandAdj;
  outC->Output.BrakePressureCommand_R =
    BrakeInhibitOut.RightBrakePressureCommandAdj;
  kcg_copy_tBrakeInhibitOut(&outC->Output.BrakeInhibitOut, &BrakeInhibitOut);
}

/* $********** SCADE Suite KCG 64-bit 6.5 (build i12) ***********
** Brake.c
** Generation date: 2022-08-11T09:49:10
*************************************************************$ */

