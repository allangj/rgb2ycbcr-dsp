/** ============================================================================
 *  @file   main.c
 *
 *  @path   $(DSPLINK)/gpp/src/samples/rgb2ycbcr-dsp/Linux/
 *
 *  @desc   Linux specific implementation of application's driver.
 *
 *  @ver    1.65.00.03
 *  ============================================================================
 *  Copyright (C) 2015, Allan Granados
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  
 *  *  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  
 *  *  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 *  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 *  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 *  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 *  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *  ============================================================================
 */


/*  ----------------------------------- OS Specific Headers           */
#include <stdio.h>
#include <stdlib.h>

/*  ----------------------------------- DSP/BIOS Link                 */
#include <dsplink.h>

/*  ----------------------------------- Application Header            */
#include <loop_os.h>
#include <loop.h>



#if defined (__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */

/** ============================================================================
 *  @func   main
 *
 *  @desc   Entry point for the application
 *
 *  @modif  None
 *  ============================================================================
 */
int main (int argc, char ** argv)
{
   Char8 * dspExecutable  = NULL;
   Char8 * strImageInput  = NULL;
   Char8 * strImageOutput = NULL;
   Uint8 * strBufferSize  = NULL;
   Char8 * strProcessorId = NULL;
   Uint8 processorId      = 0;

   if ((argc != 6) && (argc != 5)) {
      printf ("Usage : %s <absolute path of DSP executable> "
              "<Input image> <Output Image> <Buffer size> < DSP Processor Id >\n"
              "\n\t For single DSP configuration this is optional argument\n",
              argv [0]) ;
   }
   else {
      dspExecutable  = argv [1];
      strImageInput  = argv [2];
      strImageOutput = argv [3];
      strBufferSize  = argv [4];
      if(argc == 5 ) {
         strProcessorId = "0";
         processorId    = 0;
      }
      else {
         strProcessorId = argv[5];
         processorId    = atoi(argv [4]);
      }

      if (processorId < MAX_PROCESSORS) {
         RGB2YCBCR_DSP_Main (dspExecutable,
                             strImageInput,
                             strImageOutput,
                             strBufferSize,
                             strProcessorId);
      }

   }
   return 0 ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
