/**
  ******************************************************************************
  * @file    fonts.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    18-February-2014
  * @brief   Header for fonts.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FONTS_H
#define __FONTS_H

/* Max size of bitmap will based on a font24 (17x24) */
#define MAX_HEIGHT_FONT         24
#define MAX_WIDTH_FONT          17
#define OFFSET_BITMAP           54

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

typedef struct _tFont	//�ֿ���Ϣ�ṹ��
{    
  const uint8_t *table;
  uint16_t Width;
  uint16_t Height;
	uint8_t	step;
  
} sFONT;
extern sFONT Font64;
extern sFONT FontCN16;
extern sFONT Font8X16;
extern sFONT FontCN24;

#ifdef __cplusplus
}
#endif

//�ֿ��
#define ��	0,
#define ��	1,
#define ��	2,
#define ��	3,
#define ��	4,
#define ��	5,
#define ��	6,
#define ��	7,
#define ��	8,
#define ϵ	9,
#define ͳ	10,
#define ��	11,
#define ��	12,
#define ��	13,
#define ��	14,
#define ��	15,
#define ��	16,
#define ѹ	17,
#define ƫ	18, 
#define ��	19, 
#define ��	20, 
#define ��	21, 
#define ��	22, 
#define ʾ	23, 
#define ��	24, 
#define ��	25, 
#define ��	26, 
#define Ƶ  27,
#define ��  28,
#define ��	29,
#define ��	30,
#define ��	31,
#define	ʱ	32,
#define	У  33,
#define ׼	34, 
//#define ��	35, 
#define ��	36, 
#define ��	37, 
#define ��	38,
  
#endif /* __FONTS_H */
 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
