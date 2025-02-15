/* $********** SCADE Suite KCG 64-bit 6.5 (build i12) ***********
** Command: kcg65.exe -config S:/CR929/BrakeControlLow/SCADE_Mod/BCS_Model_V3/BCS_Brake/KCG/config.txt
** Generation date: 2022-08-11T09:49:10
*************************************************************$ */
#ifndef _AutoBrake_H_
#define _AutoBrake_H_

#include "kcg_types.h"
#include "Autobrake_Application_Disarmed.h"
#include "OFFtoAutobrakeArmed.h"
#include "OFFtoRTOArmed.h"
#include "RTOApplicationDisarmed_.h"
#include "RTOArmedtoEnabled.h"
#include "RTOEnabledtoApplication.h"
#include "Rate_select.h"
#include "PI_Controller.h"
#include "Autobrake_EnaletoApplication.h"
#include "stm.h"
#include "Autobrake_ARMtoEnable.h"

/* =====================  no input structure  ====================== */

/* =====================  no output structure  ====================== */

/* ========================  context type  ========================= */
typedef struct {
  /* ---------------------------  outputs  --------------------------- */
  tBrakeAutoBKOut /* AutoBrake::BRAKEAuto */ BRAKEAuto;
  /* -----------------------  no local probes  ----------------------- */
  /* -----------------  no initialization variables  ----------------- */
  /* ----------------------- local memories  ------------------------- */
  SSM_ST_SM1 /* AutoBrake::SM1 */ SM1_state_nxt;
  kcg_bool /* AutoBrake::ArmDisagree */ ArmDisagree;
  kcg_bool /* AutoBrake::CoilEnergized */ CoilEnergized;
  /* ---------------------  sub nodes' contexts  --------------------- */
  outC_PI_Controller /* 1 */ _2_Context_1;
  outC_stm /* 1 */ _1_Context_1;
  outC_Autobrake_ARMtoEnable /* 1 */ Context_1;
  outC_Autobrake_EnaletoApplication /* 4 */ Context_4;
  /* ----------------- no clocks of observable data ------------------ */
} outC_AutoBrake;

/* ===========  node initialization and cycle functions  =========== */
/* AutoBrake */
extern void AutoBrake(
  /* AutoBrake::APPData */ tINPUT_Data *APPData,
  outC_AutoBrake *outC);

#ifndef KCG_NO_EXTERN_CALL_TO_RESET
extern void AutoBrake_reset(outC_AutoBrake *outC);
#endif /* KCG_NO_EXTERN_CALL_TO_RESET */

#ifndef KCG_USER_DEFINED_INIT
extern void AutoBrake_init(outC_AutoBrake *outC);
#endif /* KCG_USER_DEFINED_INIT */

#endif /* _AutoBrake_H_ */
/* $********** SCADE Suite KCG 64-bit 6.5 (build i12) ***********
** AutoBrake.h
** Generation date: 2022-08-11T09:49:10
*************************************************************$ */

