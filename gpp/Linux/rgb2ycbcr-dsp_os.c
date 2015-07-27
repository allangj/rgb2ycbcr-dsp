/** ============================================================================
 *  @file   rgb2ycbcr-dsp_os.c
 *
 *  @path   $(DSPLINK)/gpp/src/samples/rgb2ycbcr-dsp/Linux/
 *
 *  @desc   OS specific implementation of functions used by the rgb2ycbcr-dsp
 *          application.
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
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <png.h>

/*  ----------------------------------- DSP/BIOS Link                 */
#include <dsplink.h>

/*  ----------------------------------- Application Header            */
#include <rgb2ycbcr-dsp.h>


#if defined (__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */

/** ============================================================================
 *  @func   RGB2YCBCR_DSP_0Print
 *
 *  @desc   Print a message without any arguments.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API Void RGB2YCBCR_DSP_0Print (
   Char8 * str)
{
   printf ("%s", str);
   fflush (stdout);
}


/** ============================================================================
 *  @func   RGB2YCBCR_DSP_1Print
 *
 *  @desc   Print a message with one arguments.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API Void RGB2YCBCR_DSP_1Print (
   Char8 * str, 
   Uint32 arg)
{
   printf ("%s %d", str, arg);
   fflush (stdout);
}

/** ============================================================================
 *  @func   RGB2YCBCR_DSP_Atoll
 *
 *  @desc   Converts ascii to long int
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API Uint32 RGB2YCBCR_DSP_Atoll (Char8 * str)
{
   Uint32 val = 0 ;
   val = strtoll (str, NULL, 16) ;
   return val ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
