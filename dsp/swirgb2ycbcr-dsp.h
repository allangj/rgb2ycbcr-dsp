/** ============================================================================
 *  @file   swirgb2ycbcr-dsp.h
 *
 *  @path   $(DSPLINK)/dsp/src/samples/rgb2ycbcr-dsp/
 *
 *  @desc   Header file for SWI RGB2YCBCR-DSP application.
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


#if !defined (SWIRGB2YCBCR-DSP_)
#define SWIRGB2YCBCR-DSP_


#if defined (__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   SWIRGB2YCBCR-DSP_TransferInfo
 *
 *  @desc   Structure used to keep various information needed by various phases
 *          of the application.
 *
 *  @field  gioInputChan
 *              Handle input GIO channel.
 *  @field  gioOutputChan
 *              Handle output GIO channel.
 *  @field  appReadCb
 *              Callback structure for read requests.
 *  @field  appWriteCb
 *              Callback structure for write requests.
 *  @field  inputBuffer
 *              Buffer to receive data from GPP.
 *  @field  outputBuffer
 *              Buffer to send data to GPP.
 *  @field  readWords
 *              Number of words read from GPP.
 *  @field  swi
 *              SWI structure.
 *  @field  bufferSize
 *              Size of buffer.
 *  ============================================================================
 */
typedef struct SWIRGB2YCBCR-DSP_TransferInfo_tag {
    GIO_Handle      gioInputChan;
    GIO_Handle      gioOutputChan;
    GIO_AppCallback appReadCb;
    GIO_AppCallback appWriteCb;
    Char *          inputBuffer;
    Char *          outputBuffer;
    Uint16          readWords;
    SWI_Handle      swi;
    Uint16          bufferSize;
} SWIRGB2YCBCR-DSP_TransferInfo;


/** ============================================================================
 *  @func   SWIRGB2YCBCR-DSP_create
 *
 *  @desc   Create phase function of SWIRGB2YCBCR-DSP application.
 *
 *  @arg    transferInfo
 *              Information for transfer.
 *
 *  @ret    SYS_OK
 *              Successful operation.
 *          SYS_EBADIO
 *              Failure occured while doing IO.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
Int SWIRGB2YCBCR-DSP_create (SWIRGB2YCBCR-DSP_TransferInfo ** transferInfo);

/** ============================================================================
 *  @func   SWIRGB2YCBCR-DSP_execute
 *
 *  @desc   Excecute phase function of SWIRGB2YCBCR-DSP application.
 *
 *  @arg    transferInfo
 *              Information for transfer.
 *
 *  @ret    SYS_OK
 *              Successful operation.
 *          SYS_EBADIO
 *              Failure occured while doing IO.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
Int SWIRGB2YCBCR-DSP_execute (SWIRGB2YCBCR-DSP_TransferInfo * transferInfo);

/** ============================================================================
 *  @func   SWIRGB2YCBCR-DSP_delete
 *
 *  @desc   Delete phase function of SWIRGB2YCBCR-DSP application.
 *
 *  @arg    transferInfo
 *              Information for transfer.
 *
 *  @ret    SYS_OK
 *              Successful operation.
 *          SYS_EBADIO
 *              Failure occured while doing IO.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
Int SWIRGB2YCBCR-DSP_delete (SWIRGB2YCBCR-DSP_TransferInfo * transferInfo);


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (SWIRGB2YCBCR-DSP_) */
