/* $********** SCADE Suite KCG 64-bit 6.5 (build i12) ***********
** Command: kcg65.exe -config S:/CR929/BrakeControlLow/SCADE_Mod/BCS_Model_V3/BCS_Brake/KCG/config.txt
** Generation date: 2022-08-11T09:49:10
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "OFFtoAutobrakeArmed.h"

/* OFFtoAutobrakeArmed */
kcg_bool OFFtoAutobrakeArmed(
  /* OFFtoAutobrakeArmed::AutoBrk_LO */ kcg_bool AutoBrk_LO,
  /* OFFtoAutobrakeArmed::AutoBrk_MED */ kcg_bool AutoBrk_MED,
  /* OFFtoAutobrakeArmed::AutoBrk_HI */ kcg_bool AutoBrk_HI,
  /* OFFtoAutobrakeArmed::BCSfaults */ kcg_bool BCSfaults,
  /* OFFtoAutobrakeArmed::WOW_BRK */ kcg_bool WOW_BRK)
{
  /* OFFtoAutobrakeArmed::LandingArmed */ kcg_bool LandingArmed;

  LandingArmed = (AutoBrk_LO | AutoBrk_MED | AutoBrk_HI) & !BCSfaults & !WOW_BRK;
  return LandingArmed;
}

/* $********** SCADE Suite KCG 64-bit 6.5 (build i12) ***********
** OFFtoAutobrakeArmed.c
** Generation date: 2022-08-11T09:49:10
*************************************************************$ */

