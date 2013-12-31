/* Copyright (c) 2011, Code Aurora Forum. All rights reserved.
 * Copyright (c) 2013, Tomasz "Dorregaray" Rostanski
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/module.h>

#include "msm_sensor.h"
#define SENSOR_NAME "mt9m113"
#define PLATFORM_DRIVER_NAME "msm_camera_mt9m113"
#define mt9m113_obj mt9m113_##obj

DEFINE_MUTEX(mt9m113_mut);
static struct msm_sensor_ctrl_t mt9m113_s_ctrl;

static struct msm_camera_i2c_reg_conf mt9m113_settings[] = {
};

static struct msm_camera_i2c_reg_conf mt9m113_init_settings[] = {
	//[Default]
	//STATE = Sensor Reset, 1  // hard reset
	//DELAY=10
	//STATE = Sensor Reset, 0  // hard reset
	//DELAY=10	      
	{ 0x001C, 0x0001 }, 	// MCU_BOOT_MODE
#if 1
	//Enlarge MCU_MODE/PLL/MCU_DATA delay by 20 ms to optimize "black screen issue"--0126
	{ 0x001C, 0x0000, 30}, 	// MCU_BOOT_MODEDELAY=30
#endif
	{ 0x0016, 0x00FF }, 	// CLOCKS_CONTROL
	{ 0x0018, 0x0028 }, 	// STANDBY_CONTROL
	{ 0x0014, 0x2145 }, 	// PLL_CONTROL
	{ 0x0014, 0x2145 }, 	// PLL_CONTROL
	{ 0x0014, 0x2145 }, 	// PLL_CONTROL
	{ 0x0010, 0x0114 }, 	// PLL_DIVIDERSAF
	{ 0x0012, 0x00F1 }, 	// PLL_P_DIVIDERS
	{ 0x0014, 0x2545 }, 	// PLL_CONTROL
	{ 0x0014, 0x2547 }, 	// PLL_CONTROL
#if 1
	{ 0x0014, 0x3447, 20}, 	// PLL_CONTROL//DELAY = 20 // Allow PLL to lock
#endif
//	{ 0xffff, 0x0064 }, //delay

	{ 0x0014, 0x3047 }, 	// PLL_CONTROL
	{ 0x0014, 0x3046 }, 	// PLL_CONTROL
	{ 0x001A, 0x0218 }, 	// RESET_AND_MISC_CONTROL
	{ 0x0018, 0x002A }, 	// STANDBY_CONTROL
	{ 0x321C, 0x0003 }, 	// OFIFO_CONTROL_STATUS

	{ 0x098C, 0x2703 }, 	// MCU_ADDRESS [MODE_OUTPUT_WIDTH_A] = 640
	{ 0x0990, 0x0280 }, 	// MCU_DATA_0
	{ 0x098C, 0x2705 }, 	// MCU_ADDRESS [MODE_OUTPUT_HEIGHT_A] = 480
	{ 0x0990, 0x01E0 }, 	// MCU_DATA_0
	{ 0x098C, 0x2707 }, 	// MCU_ADDRESS [MODE_OUTPUT_WIDTH_B] = 1280
	{ 0x0990, 0x0500 }, 	// MCU_DATA_0
	{ 0x098C, 0x2709 }, 	// MCU_ADDRESS [MODE_OUTPUT_HEIGHT_B] = 1024
	{ 0x0990, 0x0400 }, 	// MCU_DATA_0
	{ 0x098C, 0x270D }, 	// MCU_ADDRESS [MODE_SENSOR_ROW_START_A] = 0
	{ 0x0990, 0x0000 }, 	// MCU_DATA_0
	{ 0x098C, 0x270F }, 	// MCU_ADDRESS [MODE_SENSOR_COL_START_A] = 0
	{ 0x0990, 0x0000 }, 	// MCU_DATA_0
	{ 0x098C, 0x2711 }, 	// MCU_ADDRESS [MODE_SENSOR_ROW_END_A] = 973
	{ 0x0990, 0x03CD }, 	// MCU_DATA_0
	{ 0x098C, 0x2713 }, 	// MCU_ADDRESS [MODE_SENSOR_COL_END_A] = 1293
	{ 0x0990, 0x050D }, 	// MCU_DATA_0
	{ 0x098C, 0x2715 }, 	// MCU_ADDRESS [MODE_SENSOR_ROW_SPEED_A] =  8465
	{ 0x0990, 0x2111 }, 	// MCU_DATA_0
	{ 0x098C, 0x2717 }, 	// MCU_ADDRESS [MODE_SENSOR_READ_MODE_A]
	{ 0x0990, 0x046C }, 	// MCU_DATA_0
	{ 0x098C, 0x2719 }, 	// MCU_ADDRESS [MODE_SENSOR_FINE_CORRECTION_A] = 172
	{ 0x0990, 0x00AC }, 	// MCU_DATA_0
	{ 0x098C, 0x271B }, 	// MCU_ADDRESS [MODE_SENSOR_FINE_IT_MIN_A] = 497
	{ 0x0990, 0x01F1 }, 	// MCU_DATA_0
	{ 0x098C, 0x271D }, 	// MCU_ADDRESS [MODE_SENSOR_FINE_IT_MAX_MARGIN_A] = 319
	{ 0x0990, 0x013F }, 	// MCU_DATA_0
	{ 0x098C, 0x271F }, 	// MCU_ADDRESS [MODE_SENSOR_FRAME_LENGTH_A] = 814
	{ 0x0990, 0x032E }, 	// MCU_DATA_0
	{ 0x098C, 0x2721 }, 	// MCU_ADDRESS [MODE_SENSOR_LINE_LENGTH_PCK_A] = 1228
	{ 0x0990, 0x04CC }, 	// MCU_DATA_0
	{ 0x098C, 0x2723 }, 	// MCU_ADDRESS [MODE_SENSOR_ROW_START_B]
	{ 0x0990, 0x0004 }, 	// MCU_DATA_0
	{ 0x098C, 0x2725 }, 	// MCU_ADDRESS [MODE_SENSOR_COL_START_B]
	{ 0x0990, 0x0004 }, 	// MCU_DATA_0
	{ 0x098C, 0x2727 }, 	// MCU_ADDRESS [MODE_SENSOR_ROW_END_B] =
	{ 0x0990, 0x040B }, 	// MCU_DATA_0
	{ 0x098C, 0x2729 }, 	// MCU_ADDRESS [MODE_SENSOR_COL_END_B]
	{ 0x0990, 0x050B }, 	// MCU_DATA_0
	{ 0x098C, 0x272B }, 	// MCU_ADDRESS [MODE_SENSOR_ROW_SPEED_B]
	{ 0x0990, 0x2111 }, 	// MCU_DATA_0
	{ 0x098C, 0x272D }, 	// MCU_ADDRESS [MODE_SENSOR_READ_MODE_B]
	{ 0x0990, 0x0024 }, 	// MCU_DATA_0
	{ 0x098C, 0x272F }, 	// MCU_ADDRESS [MODE_SENSOR_FINE_CORRECTION_B]
	{ 0x0990, 0x004C }, 	// MCU_DATA_0
	{ 0x098C, 0x2731 }, 	// MCU_ADDRESS [MODE_SENSOR_FINE_IT_MIN_B]
	{ 0x0990, 0x00F9 }, 	// MCU_DATA_0
	{ 0x098C, 0x2733 }, 	// MCU_ADDRESS [MODE_SENSOR_FINE_IT_MAX_MARGIN_B]
	{ 0x0990, 0x00A7 }, 	// MCU_DATA_0
	{ 0x098C, 0x2735 }, 	// MCU_ADDRESS [MODE_SENSOR_FRAME_LENGTH_B]
	{ 0x0990, 0x0559 }, 	// MCU_DATA_0
	{ 0x098C, 0x2737 }, 	// MCU_ADDRESS [MODE_SENSOR_LINE_LENGTH_PCK_B]
	{ 0x0990, 0x0722 }, 	// MCU_DATA_0
	{ 0x098C, 0x2739 }, 	// MCU_ADDRESS [MODE_CROP_X0_A]
	{ 0x0990, 0x0000 }, 	// MCU_DATA_0
	{ 0x098C, 0x273B }, 	// MCU_ADDRESS [MODE_CROP_X1_A]
	{ 0x0990, 0x027F }, 	// MCU_DATA_0
	{ 0x098C, 0x273D }, 	// MCU_ADDRESS [MODE_CROP_Y0_A]
	{ 0x0990, 0x0000 }, 	// MCU_DATA_0
	{ 0x098C, 0x273F }, 	// MCU_ADDRESS [MODE_CROP_Y1_A]
	{ 0x0990, 0x01DF }, 	// MCU_DATA_0
	{ 0x098C, 0x2747 }, 	// MCU_ADDRESS [MODE_CROP_X0_B]
	{ 0x0990, 0x0000 }, 	// MCU_DATA_0
	{ 0x098C, 0x2749 }, 	// MCU_ADDRESS [MODE_CROP_X1_B]
	{ 0x0990, 0x04FF }, 	// MCU_DATA_0
	{ 0x098C, 0x274B }, 	// MCU_ADDRESS [MODE_CROP_Y0_B]
	{ 0x0990, 0x0000 }, 	// MCU_DATA_0
	{ 0x098C, 0x274D }, 	// MCU_ADDRESS [MODE_CROP_Y1_B]
	{ 0x0990, 0x03FF }, 	// MCU_DATA_0
	{ 0x098C, 0x222D }, 	// MCU_ADDRESS [AE_R9_STEP ????]
	{ 0x0990, 0x00CC }, 	// MCU_DATA_0
	{ 0x098C, 0xA404 }, 	// MCU_ADDRESS [FD_MODE]
	{ 0x0990, 0x0010 }, 	// MCU_DATA_0
	{ 0x098C, 0xA408 }, 	// MCU_ADDRESS [FD_SEARCH_F1_50]
	{ 0x0990, 0x0032 }, 	// MCU_DATA_0
	{ 0x098C, 0xA409 }, 	// MCU_ADDRESS [FD_SEARCH_F2_50]
	{ 0x0990, 0x0034 }, 	// MCU_DATA_0
	{ 0x098C, 0xA40A }, 	// MCU_ADDRESS [FD_SEARCH_F1_60]
	{ 0x0990, 0x003C }, 	// MCU_DATA_0
	{ 0x098C, 0xA40B }, 	// MCU_ADDRESS [FD_SEARCH_F2_60]
	{ 0x0990, 0x003E }, 	// MCU_DATA_0
	{ 0x098C, 0x2411 }, 	// MCU_ADDRESS [FD_R9_STEP_F60_A]
	{ 0x0990, 0x00CC }, 	// MCU_DATA_0
	{ 0x098C, 0x2413 }, 	// MCU_ADDRESS [FD_R9_STEP_F50_A]
	{ 0x0990, 0x00F4 }, 	// MCU_DATA_0
	{ 0x098C, 0x2415 }, 	// MCU_ADDRESS [FD_R9_STEP_F60_B]
	{ 0x0990, 0x0089 }, 	// MCU_DATA_0
	{ 0x098C, 0x2417 }, 	// MCU_ADDRESS [FD_R9_STEP_F50_B]
	{ 0x0990, 0x00A4 }, 	// MCU_DATA_0
	{ 0x098C, 0xA40D }, 	// MCU_ADDRESS [FD_STAT_MIN]
	{ 0x0990, 0x0002 }, 	// MCU_DATA_0
	{ 0x098C, 0xA40E }, 	// MCU_ADDRESS [FD_STAT_MAX]
	{ 0x0990, 0x0003 }, 	// MCU_DATA_0
	{ 0x098C, 0xA410 }, 	// MCU_ADDRESS [FD_MIN_AMPLITUDE]
	{ 0x0990, 0x000A }, 	// MCU_DATA_0
	{ 0x3044, 0x0504 }, 	// RESERVED_CORE_3044
	{ 0x3086, 0x24F7 }, 	// RESERVED_CORE_3086
	{ 0x3088, 0xF059 }, 	// RESERVED_CORE_3088
	{ 0x3090, 0x0716 }, 	// RESERVED_CORE_3090
	{ 0x3092, 0xAB1F }, 	// RESERVED_CORE_3092
	{ 0x30D4, 0x9020 }, 	// RESERVED_CORE_30D4
	{ 0x30E2, 0x6645 }, 	// RESERVED_CORE_30E2
	{ 0x30E4, 0x7A66 }, 	// RESERVED_CORE_30E4
	{ 0x30E6, 0x6652 }, 	// RESERVED_CORE_30E6
	{ 0x30E8, 0x7766 }, 	// RESERVED_CORE_30E8
	{ 0x30EA, 0x2E03 }, 	// RESERVED_CORE_30EA
	{ 0x30EC, 0x452E }, 	// RESERVED_CORE_30EC
	{ 0x30EE, 0x2E17 }, 	// RESERVED_CORE_30EE
	{ 0x30F0, 0x452E }, 	// RESERVED_CORE_30F0
	{ 0x30F6, 0x0501 }, 	// RESERVED_CORE_30F6
	{ 0x30F8, 0x0501 }, 	// RESERVED_CORE_30F8
	{ 0x30FA, 0x0401 }, 	// RESERVED_CORE_30FA
	{ 0x30FC, 0x0401 }, 	// RESERVED_CORE_30FC
	{ 0x30FE, 0x5145 }, 	// RESERVED_CORE_30FE
	{ 0x3100, 0x4F45 }, 	// RESERVED_CORE_3100
	{ 0x3102, 0x652E }, 	// RESERVED_CORE_3102
	{ 0x3104, 0x7552 }, 	// RESERVED_CORE_3104
	{ 0x3106, 0x2D05 }, 	// RESERVED_CORE_3106
	{ 0x3108, 0x4405 }, 	// RESERVED_CORE_3108
	{ 0x311A, 0x5045 }, 	// RESERVED_CORE_311A
	{ 0x311E, 0x0601 }, 	// RESERVED_CORE_311E
	{ 0x3122, 0x0601 }, 	// RESERVED_CORE_3122
	{ 0x316C, 0x8406 }, 	// RESERVED_CORE_316C
	{ 0x098C, 0xAB2D }, 	// MCU_ADDRESS [HG_NR_START_G]
	{ 0x0990, 0x002A }, 	// MCU_DATA_0
	{ 0x098C, 0xAB31 }, 	// MCU_ADDRESS [HG_NR_STOP_G]
	{ 0x0990, 0x002E }, 	// MCU_DATA_0
	{ 0x098C, 0x2B28 }, 	// MCU_ADDRESS [HG_LL_BRIGHTNESSSTART]
	{ 0x0990, 0x1F40 }, 	// MCU_DATA_0
	{ 0x098C, 0x2B2A }, 	// MCU_ADDRESS [HG_LL_BRIGHTNESSSTOP]
	{ 0x0990, 0x3A98 }, 	// MCU_DATA_0
	{ 0x098C, 0x2B38 }, 	// MCU_ADDRESS [HG_GAMMASTARTMORPH]
	{ 0x0990, 0x1F40 }, 	// MCU_DATA_0
	{ 0x098C, 0x2B3A }, 	// MCU_ADDRESS [HG_GAMMASTOPMORPH]
	{ 0x0990, 0x3A98 }, 	// MCU_DATA_0
	{ 0x098C, 0x2257 }, 	// MCU_ADDRESS [RESERVED_AE_57]
	{ 0x0990, 0x2710 }, 	// MCU_DATA_0
	{ 0x098C, 0x2250 }, 	// MCU_ADDRESS [RESERVED_AE_50]
	{ 0x0990, 0x1B58 }, 	// MCU_DATA_0
	{ 0x098C, 0x2252 }, 	// MCU_ADDRESS [RESERVED_AE_52]
	{ 0x0990, 0x32C8 }, 	// MCU_DATA_0
	{ 0x098C, 0xA24B }, 	// MCU_ADDRESS [AE_TARGETMAX]
	{ 0x0990, 0x0082 }, 	// MCU_DATA_0
	{ 0x326C, 0x0C00 }, 	// APERTURE_PARAMETERS
	{ 0x098C, 0x2717 }, 	// MCU_ADDRESS [MODE_SENSOR_READ_MODE_A]
	{ 0x0990, 0x046C }, 	// MCU_DATA_0
	{ 0x098C, 0x2719 }, 	// MCU_ADDRESS [MODE_SENSOR_FINE_CORRECTION_A]
	{ 0x0990, 0x00AC }, 	// MCU_DATA_0
	{ 0x098C, 0x271B }, 	// MCU_ADDRESS [MODE_SENSOR_FINE_IT_MIN_A]
	{ 0x0990, 0x01F1 }, 	// MCU_DATA_0
	{ 0x098C, 0x271D }, 	// MCU_ADDRESS [MODE_SENSOR_FINE_IT_MAX_MARGIN_A]
	{ 0x0990, 0x013F }, 	// MCU_DATA_0
	{ 0x098C, 0x271F }, 	// MCU_ADDRESS [MODE_SENSOR_FRAME_LENGTH_A]
	{ 0x0990, 0x032E }, 	// MCU_DATA_0
	{ 0x098C, 0x2721 }, 	// MCU_ADDRESS [MODE_SENSOR_LINE_LENGTH_PCK_A]
	{ 0x0990, 0x04CC }, 	// MCU_DATA_0
	{ 0x098C, 0x275F }, 	// MCU_ADDRESS [RESERVED_MODE_5F]
	{ 0x0990, 0x0596 }, 	// MCU_DATA_0
	{ 0x098C, 0x2761 }, 	// MCU_ADDRESS [RESERVED_MODE_61]
	{ 0x0990, 0x0094 }, 	// MCU_DATA_0
	{ 0x364E, 0x07B0 }, 	// P_GR_P0Q0
	{ 0x3650, 0x7E0E }, 	// P_GR_P0Q1
	{ 0x3652, 0x3D31 }, 	// P_GR_P0Q2
	{ 0x3654, 0x80AE }, 	// P_GR_P0Q3
	{ 0x3656, 0xE131 }, 	// P_GR_P0Q4
	{ 0x3658, 0x01B0 }, 	// P_RD_P0Q0
	{ 0x365A, 0x878D }, 	// P_RD_P0Q1
	{ 0x365C, 0x2671 }, 	// P_RD_P0Q2
	{ 0x365E, 0x7D2D }, 	// P_RD_P0Q3
	{ 0x3660, 0xA5D1 }, 	// P_RD_P0Q4
	{ 0x3662, 0x03B0 }, 	// P_BL_P0Q0
	{ 0x3664, 0x5A0E }, 	// P_BL_P0Q1
	{ 0x3666, 0x0E71 }, 	// P_BL_P0Q2
	{ 0x3668, 0x99EE }, 	// P_BL_P0Q3
	{ 0x366A, 0xA671 }, 	// P_BL_P0Q4
	{ 0x366C, 0x0170 }, 	// P_GB_P0Q0
	{ 0x366E, 0xF44D }, 	// P_GB_P0Q1
	{ 0x3670, 0x2971 }, 	// P_GB_P0Q2
	{ 0x3672, 0x2D4A }, 	// P_GB_P0Q3
	{ 0x3674, 0xD671 }, 	// P_GB_P0Q4
	{ 0x3676, 0x674C }, 	// P_GR_P1Q0
	{ 0x3678, 0x748D }, 	// P_GR_P1Q1
	{ 0x367A, 0x3FEE }, 	// P_GR_P1Q2
	{ 0x367C, 0x89AE }, 	// P_GR_P1Q3
	{ 0x367E, 0xB410 }, 	// P_GR_P1Q4
	{ 0x3680, 0x168C }, 	// P_RD_P1Q0
	{ 0x3682, 0xC56D }, 	// P_RD_P1Q1
	{ 0x3684, 0x7CAC }, 	// P_RD_P1Q2
	{ 0x3686, 0x038F }, 	// P_RD_P1Q3
	{ 0x3688, 0xA86F }, 	// P_RD_P1Q4
	{ 0x368A, 0xDB6B }, 	// P_BL_P1Q0
	{ 0x368C, 0xA2AE }, 	// P_BL_P1Q1
	{ 0x368E, 0xFA8D }, 	// P_BL_P1Q2
	{ 0x3690, 0x5C8E }, 	// P_BL_P1Q3
	{ 0x3692, 0x740C }, 	// P_BL_P1Q4
	{ 0x3694, 0x9F4B }, 	// P_GB_P1Q0
	{ 0x3696, 0x1C4D }, 	// P_GB_P1Q1
	{ 0x3698, 0x978D }, 	// P_GB_P1Q2
	{ 0x369A, 0x21EC }, 	// P_GB_P1Q3
	{ 0x369C, 0xF5AD }, 	// P_GB_P1Q4
	{ 0x369E, 0x7D10 }, 	// P_GR_P2Q0
	{ 0x36A0, 0x3E2E }, 	// P_GR_P2Q1
	{ 0x36A2, 0x8953 }, 	// P_GR_P2Q2
	{ 0x36A4, 0xD910 }, 	// P_GR_P2Q3
	{ 0x36A6, 0x3033 }, 	// P_GR_P2Q4
	{ 0x36A8, 0x06D1 }, 	// P_RD_P2Q0
	{ 0x36AA, 0xAD4E }, 	// P_RD_P2Q1
	{ 0x36AC, 0xD2D2 }, 	// P_RD_P2Q2
	{ 0x36AE, 0x5CCE }, 	// P_RD_P2Q3
	{ 0x36B0, 0x3B93 }, 	// P_RD_P2Q4
	{ 0x36B2, 0x50D0 }, 	// P_BL_P2Q0
	{ 0x36B4, 0x79AD }, 	// P_BL_P2Q1
	{ 0x36B6, 0xDFF2 }, 	// P_BL_P2Q2
	{ 0x36B8, 0x88AF }, 	// P_BL_P2Q3
	{ 0x36BA, 0x2453 }, 	// P_BL_P2Q4
	{ 0x36BC, 0x0051 }, 	// P_GB_P2Q0
	{ 0x36BE, 0x81CF }, 	// P_GB_P2Q1
	{ 0x36C0, 0x8313 }, 	// P_GB_P2Q2
	{ 0x36C2, 0x2250 }, 	// P_GB_P2Q3
	{ 0x36C4, 0x4A53 }, 	// P_GB_P2Q4
	{ 0x36C6, 0x0C8D }, 	// P_GR_P3Q0
	{ 0x36C8, 0x362B }, 	// P_GR_P3Q1
	{ 0x36CA, 0xAD51 }, 	// P_GR_P3Q2
	{ 0x36CC, 0xA470 }, 	// P_GR_P3Q3
	{ 0x36CE, 0x3DD2 }, 	// P_GR_P3Q4
	{ 0x36D0, 0x174C }, 	// P_RD_P3Q0
	{ 0x36D2, 0x152F }, 	// P_RD_P3Q1
	{ 0x36D4, 0x82F1 }, 	// P_RD_P3Q2
	{ 0x36D6, 0xDED0 }, 	// P_RD_P3Q3
	{ 0x36D8, 0x6F12 }, 	// P_RD_P3Q4
	{ 0x36DA, 0xD36C }, 	// P_BL_P3Q0
	{ 0x36DC, 0x51AE }, 	// P_BL_P3Q1
	{ 0x36DE, 0xD0AE }, 	// P_BL_P3Q2
	{ 0x36E0, 0x274E }, 	// P_BL_P3Q3
	{ 0x36E2, 0x25F2 }, 	// P_BL_P3Q4
	{ 0x36E4, 0xDCCA }, 	// P_GB_P3Q0
	{ 0x36E6, 0x438E }, 	// P_GB_P3Q1
	{ 0x36E8, 0xD64E }, 	// P_GB_P3Q2
	{ 0x36EA, 0x8A71 }, 	// P_GB_P3Q3
	{ 0x36EC, 0x1492 }, 	// P_GB_P3Q4
	{ 0x36EE, 0xD5B1 }, 	// P_GR_P4Q0
	{ 0x36F0, 0xEBF0 }, 	// P_GR_P4Q1
	{ 0x36F2, 0x53F3 }, 	// P_GR_P4Q2
	{ 0x36F4, 0x3492 }, 	// P_GR_P4Q3
	{ 0x36F6, 0x9AF4 }, 	// P_GR_P4Q4
	{ 0x36F8, 0x8BF1 }, 	// P_RD_P4Q0
	{ 0x36FA, 0x204F }, 	// P_RD_P4Q1
	{ 0x36FC, 0x3A93 }, 	// P_RD_P4Q2
	{ 0x36FE, 0xB551 }, 	// P_RD_P4Q3
	{ 0x3700, 0xE214 }, 	// P_RD_P4Q4
	{ 0x3702, 0xF2B0 }, 	// P_BL_P4Q0
	{ 0x3704, 0x8C30 }, 	// P_BL_P4Q1
	{ 0x3706, 0x3053 }, 	// P_BL_P4Q2
	{ 0x3708, 0x64F0 }, 	// P_BL_P4Q3
	{ 0x370A, 0xFC73 }, 	// P_BL_P4Q4
	{ 0x370C, 0xD311 }, 	// P_GB_P4Q0
	{ 0x370E, 0x336F }, 	// P_GB_P4Q1
	{ 0x3710, 0x5AF3 }, 	// P_GB_P4Q2
	{ 0x3712, 0x4EAF }, 	// P_GB_P4Q3
	{ 0x3714, 0xDBD4 }, 	// P_GB_P4Q4
	{ 0x3644, 0x02A0 }, 	// POLY_ORIGIN_C
	{ 0x3642, 0x01FC }, 	// POLY_ORIGIN_R
	{ 0x3210, 0x01B8 }, 	// COLOR_PIPELINE_CONTROL
	{ 0x098C, 0x2306 }, 	// MCU_ADDRESS [AWB_CCM_L_0]
	{ 0x0990, 0x0233 }, 	// MCU_DATA_0
	{ 0x098C, 0x2308 }, 	// MCU_ADDRESS [AWB_CCM_L_1]
	{ 0x0990, 0xFF0B }, 	// MCU_DATA_0
	{ 0x098C, 0x230A }, 	// MCU_ADDRESS [AWB_CCM_L_2]
	{ 0x0990, 0x0024 }, 	// MCU_DATA_0
	{ 0x098C, 0x230C }, 	// MCU_ADDRESS [AWB_CCM_L_3]
	{ 0x0990, 0xFFC8 }, 	// MCU_DATA_0
	{ 0x098C, 0x230E }, 	// MCU_ADDRESS [AWB_CCM_L_4]
	{ 0x0990, 0x01DE }, 	// MCU_DATA_0
	{ 0x098C, 0x2310 }, 	// MCU_ADDRESS [AWB_CCM_L_5]
	{ 0x0990, 0xFFBD }, 	// MCU_DATA_0
	{ 0x098C, 0x2312 }, 	// MCU_ADDRESS [AWB_CCM_L_6]
	{ 0x0990, 0x0019 }, 	// MCU_DATA_0
	{ 0x098C, 0x2314 }, 	// MCU_ADDRESS [AWB_CCM_L_7]
	{ 0x0990, 0xFF2B }, 	// MCU_DATA_0
	{ 0x098C, 0x2316 }, 	// MCU_ADDRESS [AWB_CCM_L_8]
	{ 0x0990, 0x01E8 }, 	// MCU_DATA_0
	{ 0x098C, 0x2318 }, 	// MCU_ADDRESS [AWB_CCM_L_9]
	{ 0x0990, 0x0024 }, 	// MCU_DATA_0
	{ 0x098C, 0x231A }, 	// MCU_ADDRESS [AWB_CCM_L_10]
	{ 0x0990, 0x0030 }, 	// MCU_DATA_0
	{ 0x098C, 0x231C }, 	// MCU_ADDRESS [AWB_CCM_RL_0]
	{ 0x0990, 0xFF7D }, 	// MCU_DATA_0
	{ 0x098C, 0x231E }, 	// MCU_ADDRESS [AWB_CCM_RL_1]
	{ 0x0990, 0x002C }, 	// MCU_DATA_0
	{ 0x098C, 0x2320 }, 	// MCU_ADDRESS [AWB_CCM_RL_2]
	{ 0x0990, 0x002C }, 	// MCU_DATA_0
	{ 0x098C, 0x2322 }, 	// MCU_ADDRESS [AWB_CCM_RL_3]
	{ 0x0990, 0x0006 }, 	// MCU_DATA_0
	{ 0x098C, 0x2324 }, 	// MCU_ADDRESS [AWB_CCM_RL_4]
	{ 0x0990, 0x00A3 }, 	// MCU_DATA_0
	{ 0x098C, 0x2326 }, 	// MCU_ADDRESS [AWB_CCM_RL_5]
	{ 0x0990, 0xFF75 }, 	// MCU_DATA_0
	{ 0x098C, 0x2328 }, 	// MCU_ADDRESS [AWB_CCM_RL_6]
	{ 0x0990, 0xFFF4 }, 	// MCU_DATA_0
	{ 0x098C, 0x232A }, 	// MCU_ADDRESS [AWB_CCM_RL_7]
	{ 0x0990, 0x00AC }, 	// MCU_DATA_0
	{ 0x098C, 0x232C }, 	// MCU_ADDRESS [AWB_CCM_RL_8]
	{ 0x0990, 0xFF75 }, 	// MCU_DATA_0
	{ 0x098C, 0x232E }, 	// MCU_ADDRESS [AWB_CCM_RL_9]
	{ 0x0990, 0x0010 }, 	// MCU_DATA_0
	{ 0x098C, 0x2330 }, 	// MCU_ADDRESS [AWB_CCM_RL_10]
	{ 0x0990, 0xFFF4 }, 	// MCU_DATA_0
	{ 0x098C, 0xA348 }, 	// MCU_ADDRESS [AWB_GAIN_BUFFER_SPEED]
	{ 0x0990, 0x0008 }, 	// MCU_DATA_0
	{ 0x098C, 0xA349 }, 	// MCU_ADDRESS [AWB_JUMP_DIVISOR]
	{ 0x0990, 0x0002 }, 	// MCU_DATA_0
	{ 0x098C, 0xA34A }, 	// MCU_ADDRESS [AWB_GAIN_MIN]
	{ 0x0990, 0x0059 }, 	// MCU_DATA_0
	{ 0x098C, 0xA34B }, 	// MCU_ADDRESS [AWB_GAIN_MAX]
	{ 0x0990, 0x00A6 }, 	// MCU_DATA_0
	{ 0x098C, 0xA351 }, 	// MCU_ADDRESS [AWB_CCM_POSITION_MIN]
	{ 0x0990, 0x0000 }, 	// MCU_DATA_0
	{ 0x098C, 0xA352 }, 	// MCU_ADDRESS [AWB_CCM_POSITION_MAX]
	{ 0x0990, 0x007F }, 	// MCU_DATA_0
	{ 0x098C, 0xA35D }, 	// MCU_ADDRESS [AWB_STEADY_BGAIN_OUT_MIN]
	{ 0x0990, 0x0078 }, 	// MCU_DATA_0
	{ 0x098C, 0xA35E }, 	// MCU_ADDRESS [AWB_STEADY_BGAIN_OUT_MAX]
	{ 0x0990, 0x0086 }, 	// MCU_DATA_0
	{ 0x098C, 0xA35F }, 	// MCU_ADDRESS [AWB_STEADY_BGAIN_IN_MIN]
	{ 0x0990, 0x007E }, 	// MCU_DATA_0
	{ 0x098C, 0xA360 }, 	// MCU_ADDRESS [AWB_STEADY_BGAIN_IN_MAX]
	{ 0x0990, 0x0082 }, 	// MCU_DATA_0// new 20110121 add for cold color
	{ 0x098C, 0xA369 }, 	// MCU_ADDRESS [AWB_KR_R]
	{ 0x0990, 0x0097 }, 	// MCU_DATA_0
	{ 0x098C, 0xA36A }, 	// MCU_ADDRESS [AWB_KG_R]
	{ 0x0990, 0x008C }, 	// MCU_DATA_0
	{ 0x098C, 0xA36B }, 	// MCU_ADDRESS [AWB_KB_R]
	{ 0x0990, 0x0080 }, 	// MCU_DATA_0
	{ 0x098C, 0xA302 }, 	// MCU_ADDRESS [AWB_WINDOW_POS]
	{ 0x0990, 0x0000 }, 	// MCU_DATA_0
	{ 0x098C, 0xA303 }, 	// MCU_ADDRESS [AWB_WINDOW_SIZE]
	{ 0x0990, 0x00FF }, 	// MCU_DATA_0
	{ 0x098C, 0xA11D }, 	// MCU_ADDRESS [SEQ_PREVIEW_1_AE]
	{ 0x0990, 0x0002 }, 	// MCU_DATA_0
	{ 0x098C, 0x271F }, 	// MCU_ADDRESS [MODE_SENSOR_FRAME_LENGTH_A]
	{ 0x0990, 0x032E }, 	// MCU_DATA_0
	{ 0x098C, 0x2721 }, 	// MCU_ADDRESS [MODE_SENSOR_LINE_LENGTH_PCK_A]
	{ 0x0990, 0x04CC }, 	// MCU_DATA_0
	{ 0x098C, 0xA216 }, 	// MCU_ADDRESS [AE_MAXGAIN23]
	{ 0x0990, 0x0060 }, 	// MCU_DATA_0
	{ 0x098C, 0xA215 }, 	// MCU_ADDRESS [AE_INDEX_TH23]
	{ 0x0990, 0x000A }, 	// MCU_DATA_0
	{ 0x098C, 0xA20C }, 	// MCU_ADDRESS [AE_MAX_INDEX]
	{ 0x0990, 0x0028 }, 	// MCU_DATA_0
	{ 0x098C, 0xA24F }, 	// MCU_ADDRESS [AE_BASETARGET]
	{ 0x0990, 0x0042 }, 	// MCU_DATA_0
	{ 0x098C, 0xA20E }, 	// MCU_ADDRESS [AE_MAX_VIRTGAIN]
	{ 0x0990, 0x0060 }, 	// MCU_DATA_0
	{ 0x098C, 0xA202 }, 	// MCU_ADDRESS [AE_WINDOW_POS]
	{ 0x0990, 0x0000 }, 	// MCU_DATA_0
	{ 0x098C, 0xA203 }, 	// MCU_ADDRESS [AE_WINDOW_SIZE]
	{ 0x0990, 0x00FF }, 	// MCU_DATA_0
	{ 0x098C, 0xA207 }, 	// MCU_ADDRESS [AE_GATE]
	{ 0x0990, 0x0004 }, 	// MCU_DATA_0
	{ 0x098C, 0xAB37 }, 	// MCU_ADDRESS [HG_GAMMA_MORPH_CTRL]
	{ 0x0990, 0x0003 }, 	// MCU_DATA_0
	{ 0x098C, 0x2B38 }, 	// MCU_ADDRESS [HG_GAMMASTARTMORPH]
	{ 0x0990, 0x3A98 }, 	// MCU_DATA_0
	{ 0x098C, 0x2B3A }, 	// MCU_ADDRESS [HG_GAMMASTOPMORPH]
	{ 0x0990, 0x5000 }, 	// MCU_DATA_0
	{ 0x098C, 0xAB20 }, 	// MCU_ADDRESS [HG_LL_SAT1]
	{ 0x0990, 0x0023 }, 	// MCU_DATA_0
	{ 0x098C, 0xAB24 }, 	// MCU_ADDRESS [HG_LL_SAT2]
	{ 0x0990, 0x0010 }, 	// MCU_DATA_0//gamma a
	//Commands to speed up AE speed. Begin--0308
	{ 0x098C, 0xA109 }, 	// MCU_ADDRESS [SEQ_AE_FASTBUFF]
	{ 0x0990, 0x0020 }, 	// MCU_DATA_0
	{ 0x098C, 0xA10A }, 	// MCU_ADDRESS [SEQ_AE_FASTSTEP]
	{ 0x0990, 0x0002 }, 	// MCU_DATA_0
	//Commands to speed up AE speed. End--0308
	{ 0x098C, 0xAB3C }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_0]
	{ 0x0990, 0x0000 }, 	// MCU_DATA_0
	{ 0x098C, 0xAB3D }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_1]
	{ 0x0990, 0x0006 }, 	// MCU_DATA_0
	{ 0x098C, 0xAB3E }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_2]
	{ 0x0990, 0x0014 }, 	// MCU_DATA_0
	{ 0x098C, 0xAB3F }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_3]
	{ 0x0990, 0x0038 }, 	// MCU_DATA_0
	{ 0x098C, 0xAB40 }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_4]
	{ 0x0990, 0x005F }, 	// MCU_DATA_0
	{ 0x098C, 0xAB41 }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_5]
	{ 0x0990, 0x0079 }, 	// MCU_DATA_0
	{ 0x098C, 0xAB42 }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_6]
	{ 0x0990, 0x008D }, 	// MCU_DATA_0
	{ 0x098C, 0xAB43 }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_7]
	{ 0x0990, 0x009E }, 	// MCU_DATA_0
	{ 0x098C, 0xAB44 }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_8]
	{ 0x0990, 0x00AC }, 	// MCU_DATA_0
	{ 0x098C, 0xAB45 }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_9]
	{ 0x0990, 0x00B8 }, 	// MCU_DATA_0
	{ 0x098C, 0xAB46 }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_10]
	{ 0x0990, 0x00C3 }, 	// MCU_DATA_0
	{ 0x098C, 0xAB47 }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_11]
	{ 0x0990, 0x00CD }, 	// MCU_DATA_0
	{ 0x098C, 0xAB48 }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_12]
	{ 0x0990, 0x00D5 }, 	// MCU_DATA_0
	{ 0x098C, 0xAB49 }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_13]
	{ 0x0990, 0x00DE }, 	// MCU_DATA_0
	{ 0x098C, 0xAB4A }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_14]
	{ 0x0990, 0x00E5 }, 	// MCU_DATA_0
	{ 0x098C, 0xAB4B }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_15]
	{ 0x0990, 0x00EC }, 	// MCU_DATA_0
	{ 0x098C, 0xAB4C }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_16]
	{ 0x0990, 0x00F3 }, 	// MCU_DATA_0
	{ 0x098C, 0xAB4D }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_17]
	{ 0x0990, 0x00F9 }, 	// MCU_DATA_0
	{ 0x098C, 0xAB4E }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_18]
	{ 0x0990, 0x00FF }, 	// MCU_DATA_0
	{ 0x098C, 0xAB2C }, 	// MCU_ADDRESS [HG_NR_START_R]
	{ 0x0990, 0x0010 }, 	// MCU_DATA_0
	{ 0x098C, 0xAB2D }, 	// MCU_ADDRESS [HG_NR_START_G]
	{ 0x0990, 0x002A }, 	// MCU_DATA_0
	{ 0x098C, 0xAB2E }, 	// MCU_ADDRESS [HG_NR_START_B]
	{ 0x0990, 0x0010 }, 	// MCU_DATA_0
	{ 0x098C, 0xAB2F }, 	// MCU_ADDRESS [HG_NR_START_OL]
	{ 0x0990, 0x0010 }, 	// MCU_DATA_0// new gamma B
	{ 0x098C, 0xAB4F }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_0]
	{ 0x0990, 0x0000 }, 	// MCU_DATA_0
	{ 0x098C, 0xAB50 }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_1]
	{ 0x0990, 0x0004 }, 	// MCU_DATA_0
	{ 0x098C, 0xAB51 }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_2]
	{ 0x0990, 0x000D }, 	// MCU_DATA_0
	{ 0x098C, 0xAB52 }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_3]
	{ 0x0990, 0x0028 }, 	// MCU_DATA_0
	{ 0x098C, 0xAB53 }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_4]
	{ 0x0990, 0x0053 }, 	// MCU_DATA_0
	{ 0x098C, 0xAB54 }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_5]
	{ 0x0990, 0x0075 }, 	// MCU_DATA_0
	{ 0x098C, 0xAB55 }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_6]
	{ 0x0990, 0x0092 }, 	// MCU_DATA_0
	{ 0x098C, 0xAB56 }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_7]
	{ 0x0990, 0x00A7 }, 	// MCU_DATA_0
	{ 0x098C, 0xAB57 }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_8]
	{ 0x0990, 0x00B7 }, 	// MCU_DATA_0
	{ 0x098C, 0xAB58 }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_9]
	{ 0x0990, 0x00C4 }, 	// MCU_DATA_0
	{ 0x098C, 0xAB59 }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_10]
	{ 0x0990, 0x00CF }, 	// MCU_DATA_0
	{ 0x098C, 0xAB5A }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_11]
	{ 0x0990, 0x00D8 }, 	// MCU_DATA_0
	{ 0x098C, 0xAB5B }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_12]
	{ 0x0990, 0x00DF }, 	// MCU_DATA_0
	{ 0x098C, 0xAB5C }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_13]
	{ 0x0990, 0x00E6 }, 	// MCU_DATA_0
	{ 0x098C, 0xAB5D }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_14]
	{ 0x0990, 0x00EC }, 	// MCU_DATA_0
	{ 0x098C, 0xAB5E }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_15]
	{ 0x0990, 0x00F2 }, 	// MCU_DATA_0
	{ 0x098C, 0xAB5F }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_16]
	{ 0x0990, 0x00F6 }, 	// MCU_DATA_0
	{ 0x098C, 0xAB60 }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_17]
	{ 0x0990, 0x00FB }, 	// MCU_DATA_0
	{ 0x098C, 0xAB61 }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_18]
	{ 0x0990, 0x00FF }, 	// MCU_DATA_0//
	//Normal data readout, no mirror no flip --Begin
	{ 0x098C, 0x2717 }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_18]
	{ 0x0990, 0x046C }, 	// MCU_DATA_0
	{ 0x098C, 0x272D }, 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_18]
	{ 0x0990, 0x0024 }, 	// MCU_DATA_0
	//Normal data readout, no mirror no flip --End
	//Move RESET command here to resolve abnormal preview color issue. --0126
	{ 0x001A, 0x021C }, 	// RESET_AND_MISC_CONTROL
	{ 0x098C, 0xA103 }, 	// MCU_ADDRESS
	//{ 0x0990, 0x0006, WORD_LEN, 120}, 	// MCU_DATA_0 //DELAY= 120
	//Here will use polling instead of delay to avoid "black screen issue"--0129
	{ 0x0990, 0x0006 }, 	// MCU_DATA_0 //DELAY= 120
	//Settings below will be applied in codes.-- 0129
	//{ 0x098C, 0xA103 }, 	// MCU_ADDRESS
	//{ 0x0990, 0x0005, WORD_LEN, 120}, 	// MCU_DATA_0 //DELAY= 120
	//{ 0x001A, 0x021C }, 	// RESET_AND_MISC_CONTROL
	//{ 0x3400, 0x7A08 }, 	// MIPI_CONTROL
};

static struct v4l2_subdev_info mt9m113_subdev_info[] = {
	{
	.code   = V4L2_MBUS_FMT_YUYV8_2X8,
	.colorspace = V4L2_COLORSPACE_JPEG,
	.fmt    = 1,
	.order    = 0,
	},
	/* more can be supported, to be added later */
};

static struct msm_camera_i2c_reg_conf mt9m113_config_change_settings[] = {
};

static void mt9m113_stop_stream(struct msm_sensor_ctrl_t *s_ctrl) {}

static struct msm_camera_i2c_conf_array mt9m113_init_conf[] = {
	{mt9m113_init_settings,
	ARRAY_SIZE(mt9m113_init_settings), 0, MSM_CAMERA_I2C_WORD_DATA},
	{mt9m113_config_change_settings,
	ARRAY_SIZE(mt9m113_config_change_settings),
	0, MSM_CAMERA_I2C_WORD_DATA},
};

static struct msm_camera_i2c_conf_array mt9m113_confs[] = {
	{mt9m113_settings,
	ARRAY_SIZE(mt9m113_settings), 0, MSM_CAMERA_I2C_WORD_DATA},
};

#if 0
static struct msm_camera_i2c_reg_conf mt9m113_saturation[][1] = {
};

static struct msm_camera_i2c_reg_conf mt9m113_refresh[] = {
	{ 0x098C, 0xA103, 0},
	{ 0x0990, 0x0005, 5},
	{ 0x098C, 0xA103, 0},
	{ 0x0990, 0x0006, 5}
};

static struct msm_camera_i2c_conf_array mt9m113_saturation_confs[][2] = {
	{ mt9m113_refresh,
		ARRAY_SIZE(mt9m113_refresh), 0, MSM_CAMERA_I2C_WORD_DATA} },
};

static int mt9m113_saturation_enum_map[] = {
	MSM_V4L2_SATURATION_L0,
	MSM_V4L2_SATURATION_L1,
	MSM_V4L2_SATURATION_L2,
	MSM_V4L2_SATURATION_L3,
	MSM_V4L2_SATURATION_L4,
	MSM_V4L2_SATURATION_L5,
	MSM_V4L2_SATURATION_L6,
	MSM_V4L2_SATURATION_L7,
	MSM_V4L2_SATURATION_L8,
	MSM_V4L2_SATURATION_L9,
	MSM_V4L2_SATURATION_L10,
};

static struct msm_camera_i2c_enum_conf_array mt9m113_saturation_enum_confs = {
	.conf = &mt9m113_saturation_confs[0][0],
	.conf_enum = mt9m113_saturation_enum_map,
	.num_enum = ARRAY_SIZE(mt9m113_saturation_enum_map),
	.num_index = ARRAY_SIZE(mt9m113_saturation_confs),
	.num_conf = ARRAY_SIZE(mt9m113_saturation_confs[0]),
	.data_type = MSM_CAMERA_I2C_WORD_DATA,
};

struct msm_sensor_v4l2_ctrl_info_t mt9m113_v4l2_ctrl_info[] = {
	{
		.ctrl_id = V4L2_CID_SATURATION,
		.min = MSM_V4L2_SATURATION_L0,
		.max = MSM_V4L2_SATURATION_L10,
		.step = 1,
		.enum_cfg_settings = &mt9m113_saturation_enum_confs,
		.s_v4l2_ctrl = msm_sensor_s_ctrl_by_enum,
	},
};
#endif

static struct msm_sensor_output_info_t mt9m113_dimensions[] = {
	{
		.x_output = 0x500,
		.y_output = 0x400,
		.line_length_pclk = 0x500,
		.frame_length_lines = 0x400,
		.vt_pixel_clk = 48000000,
		.op_pixel_clk = 128000000,
		.binning_factor = 1,
	},
};

static struct msm_camera_csid_vc_cfg mt9m113_cid_cfg[] = {
	{0, CSI_YUV422_8, CSI_DECODE_8BIT},
	{1, CSI_EMBED_DATA, CSI_DECODE_8BIT},
};

static struct msm_camera_csi2_params mt9m113_csi_params = {
	.csid_params = {
		.lane_assign = 0xe4,
		.lane_cnt = 1,
		.lut_params = {
			.num_cid = 2,
			.vc_cfg = mt9m113_cid_cfg,
		},
	},
	.csiphy_params = {
		.lane_cnt = 1,
		.settle_cnt = 0x14,
	},
};

static struct msm_camera_csi2_params *mt9m113_csi_params_array[] = {
	&mt9m113_csi_params,
	&mt9m113_csi_params,
};

static struct msm_sensor_output_reg_addr_t mt9m113_reg_addr = {
	.x_output = 0x2707,
	.y_output = 0x2709,
	.line_length_pclk = 0x2707,
	.frame_length_lines = 0x2709,
};

static struct msm_sensor_id_info_t mt9m113_id_info = {
	.sensor_id_reg_addr = 0x0,
	.sensor_id = 0x2480,
};

static const struct i2c_device_id mt9m113_i2c_id[] = {
	{SENSOR_NAME, (kernel_ulong_t)&mt9m113_s_ctrl},
	{ }
};

static struct i2c_driver mt9m113_i2c_driver = {
	.id_table = mt9m113_i2c_id,
	.probe  = msm_sensor_i2c_probe,
	.driver = {
		.name = SENSOR_NAME,
	},
};

static struct msm_camera_i2c_client mt9m113_sensor_i2c_client = {
	.addr_type = MSM_CAMERA_I2C_WORD_ADDR,
};

static int __init msm_sensor_init_module(void)
{
	return i2c_add_driver(&mt9m113_i2c_driver);
}

static struct v4l2_subdev_core_ops mt9m113_subdev_core_ops = {
	.s_ctrl = msm_sensor_v4l2_s_ctrl,
	.queryctrl = msm_sensor_v4l2_query_ctrl,
	.ioctl = msm_sensor_subdev_ioctl,
	.s_power = msm_sensor_power,
};

static struct v4l2_subdev_video_ops mt9m113_subdev_video_ops = {
	.enum_mbus_fmt = msm_sensor_v4l2_enum_fmt,
};

static struct v4l2_subdev_ops mt9m113_subdev_ops = {
	.core = &mt9m113_subdev_core_ops,
	.video  = &mt9m113_subdev_video_ops,
};

static struct msm_sensor_fn_t mt9m113_func_tbl = {
	.sensor_start_stream = msm_sensor_start_stream,
	.sensor_stop_stream = mt9m113_stop_stream,
	.sensor_setting = msm_sensor_setting,
	.sensor_set_sensor_mode = msm_sensor_set_sensor_mode,
	.sensor_mode_init = msm_sensor_mode_init,
	.sensor_get_output_info = msm_sensor_get_output_info,
	.sensor_config = msm_sensor_config,
	.sensor_power_up = msm_sensor_power_up,
	.sensor_power_down = msm_sensor_power_down,
};

static struct msm_sensor_reg_t mt9m113_regs = {
	.default_data_type = MSM_CAMERA_I2C_WORD_DATA,
	.start_stream_conf = mt9m113_config_change_settings,
	.start_stream_conf_size = ARRAY_SIZE(mt9m113_config_change_settings),
	.init_settings = &mt9m113_init_conf[0],
	.init_size = ARRAY_SIZE(mt9m113_init_conf),
	.mode_settings = &mt9m113_confs[0],
	.output_settings = &mt9m113_dimensions[0],
	.num_conf = ARRAY_SIZE(mt9m113_confs),
};

static struct msm_sensor_ctrl_t mt9m113_s_ctrl = {
	.msm_sensor_reg = &mt9m113_regs,
#if 0
	.msm_sensor_v4l2_ctrl_info = mt9m113_v4l2_ctrl_info,
	.num_v4l2_ctrl = ARRAY_SIZE(mt9m113_v4l2_ctrl_info),
#endif
	.sensor_i2c_client = &mt9m113_sensor_i2c_client,
	.sensor_i2c_addr = 0x90,
	.sensor_output_reg_addr = &mt9m113_reg_addr,
	.sensor_id_info = &mt9m113_id_info,
	.cam_mode = MSM_SENSOR_MODE_INVALID,
	.csi_params = &mt9m113_csi_params_array[0],
	.msm_sensor_mutex = &mt9m113_mut,
	.sensor_i2c_driver = &mt9m113_i2c_driver,
	.sensor_v4l2_subdev_info = mt9m113_subdev_info,
	.sensor_v4l2_subdev_info_size = ARRAY_SIZE(mt9m113_subdev_info),
	.sensor_v4l2_subdev_ops = &mt9m113_subdev_ops,
	.func_tbl = &mt9m113_func_tbl,
};

module_init(msm_sensor_init_module);
MODULE_DESCRIPTION("Aptina 1.3MP YUV sensor driver");
MODULE_LICENSE("GPL v2");
