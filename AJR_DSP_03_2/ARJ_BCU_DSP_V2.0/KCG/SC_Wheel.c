/* $********** SCADE Suite KCG 64-bit 6.5 (build i12) ***********
** Command: kcg65.exe -config S:/CR929/BrakeControlLow/SCADE_Mod/BCS_Model_V3/BCS_Brake/KCG/config.txt
** Generation date: 2022-08-11T09:49:10
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "SC_Wheel.h"

#ifndef KCG_USER_DEFINED_INIT
void SC_Wheel_init(outC_SC_Wheel *outC)
{
  outC->SkidEnable = kcg_true;
  outC->init = kcg_true;
  outC->BrakePressureCommandAdj = kcg_lit_float32(0.0);
  /* 1 */ SC_Function_init(&outC->Context_1);
}
#endif /* KCG_USER_DEFINED_INIT */


#ifndef KCG_NO_EXTERN_CALL_TO_RESET
void SC_Wheel_reset(outC_SC_Wheel *outC)
{
  outC->init = kcg_true;
  /* 1 */ SC_Function_reset(&outC->Context_1);
}
#endif /* KCG_NO_EXTERN_CALL_TO_RESET */

/** BCUSW-735 */
/** BCUSW-737 */
/** BCUSW-774 */
/** BCUSW-775 */
/** "Title_1" {Title = "LLR1 : Skid Control wheel function"} */
/** "Graphical_111" {Text = "Trace: SRD: LLR1 : BCUSW-735;BCUSW-739;BCUSW-774;BCUSW-775"} */
/* SC_Wheel */
void SC_Wheel(
  /* SC_Wheel::WOWBK */ kcg_bool WOWBK,
  /* SC_Wheel::WheelSpeed */ tValidityF32 *WheelSpeed,
  /* SC_Wheel::WheelRefSpeed */ kcg_float32 WheelRefSpeed,
  /* SC_Wheel::BrakePressureCommand */ kcg_float32 BrakePressureCommand,
  outC_SC_Wheel *outC)
{
  outC->SkidEnable = WOWBK & (*WheelSpeed).Valid;
  /* ck_SkidEnable */ if (outC->SkidEnable) {
    /* 1 */
    SC_Function(
      (*WheelSpeed).Value,
      WheelRefSpeed,
      (kcg_bool) (BrakePressureCommand > kcg_lit_float32(500.0)),
      BrakePressureCommand,
      &outC->Context_1);
    outC->BrakePressureCommandAdj = outC->Context_1.BrakePressureCommandAdj;
  }
  else if (outC->init) {
    outC->BrakePressureCommandAdj = BrakePressureCommand;
  }
  outC->init = kcg_false;
}

/* $********** SCADE Suite KCG 64-bit 6.5 (build i12) ***********
** SC_Wheel.c
** Generation date: 2022-08-11T09:49:10
*************************************************************$ */

