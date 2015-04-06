/** ============================================================================
 *  @file   rgb2ycbcr-dsp.h
 *
 *  @path   $(DSPLINK)/gpp/src/samples/rgb2ycbcr-dsp/
 *
 *  @desc   Defines the configurable parameters for the rgb2ycbcr-dsp application
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


#if !defined (RGB2YCBCR_DSP_H)
#define RGB2YCBCR_DSP_H

/*  ----------------------------------- DSP/BIOS Link                 */
#include <dsplink.h>


#if defined (__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @const  CHNL_ID_OUTPUT
 *
 *  @desc   ID of channel used to send data to DSP.
 *  ============================================================================
 */
#define CHNL_ID_OUTPUT     0


/** ============================================================================
 *  @const  CHNL_ID_INPUT
 *
 *  @desc   ID of channel used to receive data from DSP.
 *  ============================================================================
 */
#define CHNL_ID_INPUT      1


/** ============================================================================
 *  @func   RGB2YCBCR_DSP_Create
 *
 *  @desc   This function allocates and initializes resources used by
 *          this application.
 *
 *  @arg    dspExecutable
 *              DSP executable name.
 *  @arg    strBufferSize
 *              Buffer size to be used for data transfer. Its a size in bytes.
 *
 *  @arg    processorId
 *             Id of the DSP Processor. 
 *
 *  @ret    DSP_SOK
 *              Operation Successfully completed.
 *          DSP_EFAIL
 *              Resource allocation failed.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    RGB2YCBCR_DSP_Delete
 *  ============================================================================
 */
NORMAL_API DSP_STATUS RGB2YCBCR_DSP_Create(
   IN Char8 * dspExecutable,
   IN Char8 * strImageInput,
   IN Uint8 * strBufferSize,
   IN Uint8   processorId);


/** ============================================================================
 *  @func   RGB2YCBCR_DSP_Execute
 *
 *  @desc   This function implements the execute phase for this application.
 *
 *  @ret    DSP_SOK
 *              Operation Successfully completed.
 *          DSP_EFAIL
 *              Execution failed.
 *
 *  @arg    processorId
 *             Id of the DSP Processor. 
 *
 *  @ret    DSP_SOK
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    RGB2YCBCR_DSP_Delete , RGB2YCBCR_DSP_Create
 *  ============================================================================
 */
NORMAL_API DSP_STATUS RGB2YCBCR_DSP_Execute(
   IN Uint8 processorId);


/** ============================================================================
 *  @func   RGB2YCBCR_DSP_Delete
 *
 *  @desc   This function releases resources allocated earlier by call to
 *          RGB2YCBCR_DSP_Create ().
 *          During cleanup, the allocated resources are being freed
 *          unconditionally. Actual applications may require stricter check
 *          against return values for robustness.
 *
 *  @arg    processorId
 *             Id of the DSP Processor. 
 *
 *  @ret    DSP_SOK
 *
 *  @ret    DSP_SOK
 *              Operation Successfully completed.
 *          DSP_EFAIL
 *              Resource deallocation failed.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    RGB2YCBCR_DSP_Create
 *  ============================================================================
 */
NORMAL_API Void RGB2YCBCR_DSP_Delete(
   Uint8 processorId);


/** ============================================================================
 *  @func   RGB2YCBCR_DSP_Main
 *
 *  @desc   The OS independent driver function for the rgb2ycbcr-dsp application.
 *
 *  @arg    dspExecutable
 *              Name of the DSP executable file.
 *  @arg    strImageInput
 *              Input image name in string format.
 *  @arg    strImageOutput
 *              Output image name in string format.
 *  @arg    strBufferSize
 *              Size of the Buffer to use.
 *
 *  @arg    strProcessorId
 *             Id of the DSP Processor in string format. 
 *
 *  @ret    DSP_SOK
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    RGB2YCBCR_DSP_Create, RGB2YCBCR_DSP_Execute, RGB2YCBCR_DSP_Delete
 *  ============================================================================
 */
NORMAL_API Void RGB2YCBCR_DSP_Main(
   IN Char8 * dspExecutable,
   IN Char8 * strImageInput,
   IN Char8 * strImageOutput,
   IN Char8 * strBufferSize,
   IN Char8 * strProcessorId);

/** ============================================================================
 *  @func   RGB2YCBCR_DSP_0Print
 *
 *  @desc   Print a message without any arguments.
 *          This is a OS specific function and is implemented in file:
 *              <GPPOS>\rgb2ycbcr-dsp_os.c
 *
 *  @arg    str
 *              String message to be printed.
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API Void RGB2YCBCR_DSP_0Print(
   Char8 * str);


/** ============================================================================
 *  @func   RGB2YCBCR_DSP_1Print
 *
 *  @desc   Print a message with one arguments.
 *          This is a OS specific function and is implemented in file:
 *              <GPPOS>\rgb2ycbcr-dsp_os.c
 *
 *  @arg    str
 *              String message to be printed.
 *  @arg    arg
 *              Argument to be printed.
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API Void RGB2YCBCR_DSP_1Print(
   Char8 * str, Uint32 arg);

/** ============================================================================
 *  @func   RGB2YCBCR_DSP_Load_Image
 *
 *  @desc   Load a PNG image file.
 *          This is a OS specific function and is implemented in file:
 *              <GPPOS>\rgb2ycbcr-dsp_os.c
 *
 *  @arg    file_name
 *              String path to image.
 *
 *  @arg    height
 *              Pointer to height of the image.
 *
 *  @arg    width
 *              Pointer to width of the image.
 *
 *  @arg    buff_ptr
 *              Pointer to buffer.
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API Void RGB2YCBCR_DSP_Load_Image(
   Char8 * file_name,
   Char8 * buff_ptr);

/** ============================================================================
 *  @func   RGB2YCBCR_DSP_Write_Image
 *
 *  @desc   Write a PNG image file.
 *          This is a OS specific function and is implemented in file:
 *              <GPPOS>\rgb2ycbcr-dsp_os.c
 *
 *  @arg    file_name
 *              String path to image.
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API Void RGB2YCBCR_DSP_Write_Image(
   Char8 * file_name);

#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (RGB2YCBCR_DSP_H) */
