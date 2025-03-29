/*********************************************************************************************************//**
 * @file    ht32f5xxxx_pwrcu.h
 * @version $Rev:: 2298         $
 * @date    $Date:: 2017-12-08 #$
 * @brief   The header file of the Power Control Unit library.
 *************************************************************************************************************
 * @attention
 *
 * Firmware Disclaimer Information
 *
 * 1. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, which is supplied by Holtek Semiconductor Inc., (hereinafter referred to as "HOLTEK") is the
 *    proprietary and confidential intellectual property of HOLTEK, and is protected by copyright law and
 *    other intellectual property laws.
 *
 * 2. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, is confidential information belonging to HOLTEK, and must not be disclosed to any third parties
 *    other than HOLTEK and the customer.
 *
 * 3. The program technical documentation, including the code, is provided "as is" and for customer reference
 *    only. After delivery by HOLTEK, the customer shall use the program technical documentation, including
 *    the code, at their own risk. HOLTEK disclaims any expressed, implied or statutory warranties, including
 *    the warranties of merchantability, satisfactory quality and fitness for a particular purpose.
 *
 * <h2><center>Copyright (C) Holtek Semiconductor Inc. All rights reserved</center></h2>
 ************************************************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __HT32F5XXXX_PWRCU_H
#define __HT32F5XXXX_PWRCU_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"

/** @addtogroup HT32F5xxxx_Peripheral_Driver HT32F5xxxx Peripheral Driver
  * @{
  */

/** @addtogroup PWRCU
  * @{
  */


/* Exported types ------------------------------------------------------------------------------------------*/
/** @defgroup PWRCU_Exported_Types PWRCU exported types
  * @{
  */

/**
 * @brief Status of Power control unit
 */

/**
 * @brief Wakeup pin selection
 */
typedef enum
{
  PWRCU_WAKEUP_PIN_0 = 0,
  #if (LIBCFG_PWRCU_WAKEUP1)
  PWRCU_WAKEUP_PIN_1
  #endif
} PWRCU_WUP_Enum;
/**
 * @brief Wakeup pin trigger type selection
 */
#if (LIBCFG_WAKEUP_V01)
typedef enum
{
  PWRCU_WUP_POSITIVE_EDGE = 0,        /*!< Wakeup pin positive_edge triggered                                   */
  PWRCU_WUP_NEGATIVE_EDGE,           /*!< Wakeup pin negative_edge triggered                                  */
  PWRCU_WUP_HIGH_LEVEL,             /*!< Wakeup pin high-level sensitive                                    */
  PWRCU_WUP_LOW_LEVEL,              /*!< Wakeup pin low-level sensitive                                     */
} PWRCU_WUPTYPE_Enum;
#endif
typedef enum
{
  PWRCU_OK = 0,                     /*!< Ready for access or VDD power domain power-on reset is released    */
  PWRCU_TIMEOUT,                    /*!< Time out                                                           */
  PWRCU_ERROR                       /*!< Error                                                              */
} PWRCU_Status;
/**
 * @brief DMOS status
 */
typedef enum
{
  PWRCU_DMOS_STS_ON = 0,            /*!< DMOS on                                                            */
  PWRCU_DMOS_STS_OFF,               /*!< DMOS off                                                           */
  PWRCU_DMOS_STS_OFF_BY_BODRESET    /*!< DMOS off caused by brow out reset                                  */
} PWRCU_DMOSStatus;
/**
 * @brief LVD level selection
 */
#if (LIBCFG_VDD_5V)
typedef enum
{
  PWRCU_LVDS_2V65 = 0x00000000,      /*!< LVD level is 2.65 V                                               */
  PWRCU_LVDS_2V85 = 0x00020000,      /*!< LVD level is 2.85 V                                               */
  PWRCU_LVDS_3V05 = 0x00040000,      /*!< LVD level is 3.05 V                                               */
  PWRCU_LVDS_3V25 = 0x00060000,      /*!< LVD level is 3.25 V                                               */
  PWRCU_LVDS_3V45 = 0x00400000,      /*!< LVD level is 3.45 V                                               */
  PWRCU_LVDS_4V25 = 0x00420000,      /*!< LVD level is 4.25 V                                               */
  PWRCU_LVDS_4V45 = 0x00440000,      /*!< LVD level is 4.45 V                                               */
  PWRCU_LVDS_4V65 = 0x00460000       /*!< LVD level is 4.65 V                                               */
} PWRCU_LVDS_Enum;
#else
typedef enum
{
  PWRCU_LVDS_2V25 = 0x00000000,      /*!< LVD level is 2.25 V                                               */
  PWRCU_LVDS_2V4  = 0x00020000,      /*!< LVD level is 2.40 V                                               */
  PWRCU_LVDS_2V55 = 0x00040000,      /*!< LVD level is 2.55 V                                               */
  PWRCU_LVDS_2V7  = 0x00060000,      /*!< LVD level is 2.70 V                                               */
  PWRCU_LVDS_2V85 = 0x00400000,      /*!< LVD level is 2.85 V                                               */
  PWRCU_LVDS_3V   = 0x00420000,      /*!< LVD level is 3.00 V                                               */
  PWRCU_LVDS_3V15 = 0x00440000,      /*!< LVD level is 3.15 V                                               */
  PWRCU_LVDS_3V3  = 0x00460000       /*!< LVD level is 3.30 V                                               */
} PWRCU_LVDS_Enum;
#endif
/**
 * @brief BOD reset or interrupt selection
 */
typedef enum
{
  PWRCU_BODRIS_RESET = 0,  /*!< Reset the whole chip                                                        */
  PWRCU_BODRIS_INT   = 1,  /*!< Assert interrupt                                                            */
} PWRCU_BODRIS_Enum;
/**
 * @brief Sleep entry instruction selection
 */
typedef enum
{
  PWRCU_SLEEP_ENTRY_WFE = 0,          /*!< Sleep then wait for event                                        */
  PWRCU_SLEEP_ENTRY_WFI               /*!< Sleep then wait for interrupt                                    */
} PWRCU_SLEEP_ENTRY_Enum;
#if defined(LIBCFG_BAKREG)
/**
 * @brief Backup register selection
 */
typedef enum
{
  PWRCU_BAKREG_0 = 0,
  PWRCU_BAKREG_1,
  PWRCU_BAKREG_2,
  PWRCU_BAKREG_3,
  PWRCU_BAKREG_4,
  PWRCU_BAKREG_5,
  PWRCU_BAKREG_6,
  PWRCU_BAKREG_7,
  PWRCU_BAKREG_8,
  PWRCU_BAKREG_9
} PWRCU_BAKREG_Enum;
#endif
/**
 * @brief Vdd15 power good source selection
 */
typedef enum
{
  PWRCU_V15RDYSC_V33ISO = 0, /*!< Vdd15 power good source come from BK_ISO bit in CKCU unit                 */
  PWRCU_V15RDYSC_V15POR      /*!< Vdd15 power good source come from Vdd15 power on reset                    */
} PWRCU_V15RDYSC_Enum;
/**
 * @brief LDO operation mode selection
 */
typedef enum
{
  PWRCU_LDO_NORMAL = 0,     /*!< The LDO is operated in normal current mode                                 */
  PWRCU_LDO_LOWCURRENT      /*!< The LDO is operated in low current mode                                    */
} PWRCU_LDOMODE_Enum;
#if defined(USE_HT32F52342_52) || defined(USE_HT32F5826)
/**
 * @brief HSI ready counter bit length selection
 */
typedef enum
{
  PWRCU_HSIRCBL_4 = 0,  /*!< 4 bits                                                                         */
  PWRCU_HSIRCBL_5,      /*!< 5 bits                                                                         */
  PWRCU_HSIRCBL_6,      /*!< 5 bits                                                                         */
  PWRCU_HSIRCBL_7       /*!< 7 bits (Default)                                                               */
} PWRCU_HSIRCBL_Enum;
#endif
/**
  * @}
  */

/* Exported constants --------------------------------------------------------------------------------------*/
/** @defgroup PWRCU_Exported_Constants PWRCU exported constants
  * @{
  */

/* Definitions of PWRCU_FLAG                                                                                */
#if (!LIBCFG_NO_PWRCU_PORF)
#define PWRCU_FLAG_PWRPOR       0x0001  /*!< VDD power domain power-on reset flag                           */
#endif
#if (!LIBCFG_NO_PWRCU_PDF)
#define PWRCU_FLAG_PD           0x0002  /*!< Power-Down flag                                                */
#endif
#if (LIBCFG_PWRCU_PORF)
#define PWRCU_FLAG_POR          0x0010  /*!< Power-on reset flag                                            */
#endif
#define PWRCU_FLAG_WUP          0x0100  /*!< External WAKEUP pin flag                                       */
#define PWRCU_FLAG_WUP0         0x0100  /*!< External WAKEUP0 pin flag                                      */
#if (LIBCFG_PWRCU_WAKEUP1)
#define PWRCU_FLAG_WUP1         0x0200  /*!< External WAKEUP1 pin flag                                      */
#endif

/* check PWRCU_LVDS parameter                                                                               */
#if (LIBCFG_VDD_5V)
#define IS_PWRCU_LVDS(x)        ((x == PWRCU_LVDS_2V65) || (x == PWRCU_LVDS_2V85) || \
                                 (x == PWRCU_LVDS_3V05) || (x == PWRCU_LVDS_3V25) || \
                                 (x == PWRCU_LVDS_3V45) || (x == PWRCU_LVDS_4V25)  || \
                                 (x == PWRCU_LVDS_4V45) || (x == PWRCU_LVDS_4V65))
#else
#define IS_PWRCU_LVDS(x)        ((x == PWRCU_LVDS_2V25) || (x == PWRCU_LVDS_2V4) || \
                                 (x == PWRCU_LVDS_2V55) || (x == PWRCU_LVDS_2V7) || \
                                 (x == PWRCU_LVDS_2V85) || (x == PWRCU_LVDS_3V)  || \
                                 (x == PWRCU_LVDS_3V15) || (x == PWRCU_LVDS_3V3))
#endif

/* check PWRCU_BODRIS parameter                                                                             */
#define IS_PWRCU_BODRIS(x)      ((x == PWRCU_BODRIS_RESET) || (x == PWRCU_BODRIS_INT))

/* check PWRCU_HSIRCBL parameter                                                                            */
#define IS_PWRCU_HSIRCBL(x)     (x <= 3)

/* check PWRCU_SLEEP_ENTRY parameter                                                                        */
#define IS_PWRCU_SLEEP_ENTRY(x) ((x == PWRCU_SLEEP_ENTRY_WFI) || (x == PWRCU_SLEEP_ENTRY_WFE))

/* check PWRCU_BAKREG parameter                                                                             */
#define IS_PWRCU_BAKREG(x)      (x < 10)

/* check PWRCU_V15RDY_SRC parameter                                                                         */
#define IS_PWRCU_V15RDYSC(x)    ((x == PWRCU_V15RDYSC_V33ISO) || (x == PWRCU_V15RDYSC_V15POR))

/* check PWRCU_LDOMODE parameter                                                                            */
#define IS_PWRCU_LDOMODE(x)     ((x == PWRCU_LDO_NORMAL) || (x == PWRCU_LDO_LOWCURRENT))
/**
  * @}
  */

/* Exported functions --------------------------------------------------------------------------------------*/
/** @defgroup PWRCU_Exported_Functions PWRCU exported functions
  * @{
  */
void PWRCU_DeInit(void);
#if (!LIBCFG_NO_PWRCU_TEST_REG)
PWRCU_Status PWRCU_CheckReadyAccessed(void);
#endif
u16 PWRCU_GetFlagStatus(void);
#if defined(LIBCFG_BAKREG)
u32 PWRCU_ReadBackupRegister(PWRCU_BAKREG_Enum BAKREGx);
void PWRCU_WriteBackupRegister(PWRCU_BAKREG_Enum BAKREGx, u32 DATA);
#endif
void PWRCU_Sleep(PWRCU_SLEEP_ENTRY_Enum SleepEntry);
void PWRCU_DeepSleep1(PWRCU_SLEEP_ENTRY_Enum SleepEntry);
void PWRCU_DeepSleep2(PWRCU_SLEEP_ENTRY_Enum SleepEntry);
#if (!LIBCFG_NO_PWRCU_POWER_DOWN_MODE)
void PWRCU_PowerDown(void);
#endif
void PWRCU_SetLVDS(PWRCU_LVDS_Enum Level);
void PWRCU_LDOConfig(PWRCU_LDOMODE_Enum Sel);
void PWRCU_LVDCmd(ControlStatus NewState);
void PWRCU_BODCmd(ControlStatus NewState);
void PWRCU_BODRISConfig(PWRCU_BODRIS_Enum Selection);
FlagStatus PWRCU_GetLVDFlagStatus(void);
FlagStatus PWRCU_GetBODFlagStatus(void);
PWRCU_DMOSStatus PWRCU_GetDMOSStatus(void);
void PWRCU_DMOSCmd(ControlStatus NewState);
#if (!LIBCFG_NO_V15_READY_SOURCE)
void PWRCU_V15RDYSourceConfig(PWRCU_V15RDYSC_Enum Sel);
#endif
void PWRCU_LVDIntWakeupConfig(ControlStatus NewState);
void PWRCU_LVDEventWakeupConfig(ControlStatus NewState);
void PWRCU_WakeupPinCmd(ControlStatus NewState);
void PWRCU_WakeupPinIntConfig(ControlStatus NewState);
#if (LIBCFG_WAKEUP_V01)
void PWRCU_WakeupMultiPinCmd(PWRCU_WUP_Enum Pin, PWRCU_WUPTYPE_Enum Type, ControlStatus NewState);
void PWRCU_WakeupMultiPinIntConfig(PWRCU_WUP_Enum Pin, ControlStatus NewState);
#endif
#if defined(USE_HT32F52342_52) || defined(USE_HT32F5826)
void PWRCU_HSIReadyCounterBitLengthConfig(PWRCU_HSIRCBL_Enum BitLength);
#endif
/**
  * @}
  */


/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif
