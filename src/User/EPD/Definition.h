#include "port.h"

#include "E-INK-1.54.h"
#include "epdif.h"
#include "E-INK-Display.h"
#include "IMAGE.h"
#include <stdlib.h>


#define COLORED      0		//COLORED代表黑色 UNCOLORED代表白色
#define UNCOLORED    1


/******* Private function prototypes*******/

/*******System Clock Configuration*******/

#ifdef __GNUC__

  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */



void _Error_Handler(char * file, int line)
{

  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
