/* Copyright Statement:
 *
 * This software/firmware and related documentation ("AutoChips Software") are
 * protected under relevant copyright laws. The information contained herein is
 * confidential and proprietary to AutoChips Inc. and/or its licensors. Without
 * the prior written permission of AutoChips inc. and/or its licensors, any
 * reproduction, modification, use or disclosure of AutoChips Software, and
 * information contained herein, in whole or in part, shall be strictly
 * prohibited.
 * 
 * AutoChips Inc. (C) 2016. All rights reserved.
 * 
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("AUTOCHIPS SOFTWARE")
 * RECEIVED FROM AUTOCHIPS AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER
 * ON AN "AS-IS" BASIS ONLY. AUTOCHIPS EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NONINFRINGEMENT. NEITHER DOES AUTOCHIPS PROVIDE ANY WARRANTY WHATSOEVER WITH
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 * INCORPORATED IN, OR SUPPLIED WITH THE AUTOCHIPS SOFTWARE, AND RECEIVER AGREES
 * TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO
 * OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN AUTOCHIPS
 * SOFTWARE. AUTOCHIPS SHALL ALSO NOT BE RESPONSIBLE FOR ANY AUTOCHIPS SOFTWARE
 * RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND AUTOCHIPS'S
 * ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE AUTOCHIPS SOFTWARE
 * RELEASED HEREUNDER WILL BE, AT AUTOCHIPS'S OPTION, TO REVISE OR REPLACE THE
 * AUTOCHIPS SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE
 * CHARGE PAID BY RECEIVER TO AUTOCHIPS FOR SUCH AUTOCHIPS SOFTWARE AT ISSUE.
 */

#ifndef _SPM_AC78XX_REGS_H
#define _SPM_AC78XX_REGS_H

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif



#define SPM_BASE 0x40008000

#define REG_PWR_MGR_CFG0       (SPM_BASE + 0x000)
#define REG_PWR_EN_POS               0
#define REG_EN_FAST_BOOT_POS         1
#define REG_EN_PVD_POS           2
#define REG_EN_LVD_POS            4
#define REG_EN_CAN1_FILTER_POS       5
#define REG_EN_CAN2_FILTER_POS       6
#define REG_EN_IO_SUS_STOP_MODE      7

#define REG_SLEEP_MODE_POS           8
#define REG_SLEEP_MODE_BITWIDTH      2
	
#define REG_PWR_MGR_CFG1       (SPM_BASE + 0x004)
#define RG_MCU_PORLPVD_PWRLVD_CAL_POS      0
#define RG_MCU_PORLPVD_PWRLVD_CAL_BITWIDTH 4
#define REG_SYSPLL_ON_POS                  27
#define REG_XOSC_HSEBYP_POS                28
#define REG_XOSC_HSEON_POS                 29
#define SYSPLL_RDY_POS                     30
#define XOSC_RDY_POS                       31
#define REG_LVD_TRIGGER_LEVEL_POS    3
#define REG_PVD_TRIGGER_LEVEL_POS    0
#define REG_PVD_TRIGGER_LEVEL_BITWIDTH 2
	


#define REG_PWR_MGR_PG_PU_MISC4 (SPM_BASE + 0x060)
#define REG_PWR_MGR_PG_PU_MISC5 (SPM_BASE + 0x064)

#define REG_EN_PERIPH_SLEEP_ACK (SPM_BASE + 0x010)
#define REG_EN_PERIPH_WAKEUP    (SPM_BASE + 0x014)
#define REG_EN_TRIGGER_IRQ      (SPM_BASE + 0x018)
#define REG_SPM_WAKEUP_IRQ      (SPM_BASE + 0x01c)

#define REG_PWR_MGR_PG_MISC0    (SPM_BASE + 0x02c)

#ifdef __cplusplus
}
#endif


#endif
