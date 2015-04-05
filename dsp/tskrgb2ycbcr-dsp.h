/** ============================================================================
 *  @file   tskrgb2ycbcr-dsp.h
 *
 *  @path   $(DSPLINK)/dsp/src/samples/rgb2ycbcr-dsp/
 *
 *  @desc   Header file for tskrgb2ycbcr-dsp application.
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


#if !defined (TSKRGB2YCBCR_DSP_)
#define TSKRGB2YCBCR_DSP_


#if defined (__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @const  MAX_BUFFERS
 *
 *  @desc   Size of buffers array.
 *  ============================================================================
 */
#define MAX_BUFFERS            100

/** ============================================================================
 *  @const  TSK_NUM_BUFFERS
 *
 *  @desc   Number of buffers to allocate and use for transfer.
 *  ============================================================================
 */
#define TSK_NUM_BUFFERS          1


/** ============================================================================
 *  @name   TSKRGB2YCBCR_DSP_TransferInfo
 *
 *  @desc   Structure used to keep various information needed by various phases
 *          of the application.
 *
 *  @field  inputStream
 *              Handle to input stream.
 *  @field  outputStream
 *              Handle to output stream.
 *  @field  numTransfers
 *              Number of transfers to be done.
 *  @field  bufferSize
 *              Buffer size information.
 *  @field  receivedSize
 *              Actual length of the buffer that was received.
 *  @field  numBuffers
 *              Number of valid buffers in "buffers" array.
 *  @field  buffers
 *              Array of buffer pointers.
 *  ============================================================================
 */
typedef struct TSKRGB2YCBCR_DSP_TransferInfo_tag {
    SIO_Handle inputStream;
    SIO_Handle outputStream;
    Uint16     numTransfers;
    Uint32     bufferSize;
    Uint16     receivedSize;
    Uint16     numBuffers;
    Char *     buffers [MAX_BUFFERS];
} TSKRGB2YCBCR_DSP_TransferInfo;


/** ============================================================================
 *  @func   TSKRGB2YCBCR_DSP_create
 *
 *  @desc   Create phase function of TSKRGB2YCBCR_DSP application.
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
Int TSKRGB2YCBCR_DSP_create (TSKRGB2YCBCR_DSP_TransferInfo ** transferInfo);

/** ============================================================================
 *  @func   TSKRGB2YCBCR_DSP_execute
 *
 *  @desc   Excecute phase function of TSKRGB2YCBCR_DSP application.
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
Int TSKRGB2YCBCR_DSP_execute (TSKRGB2YCBCR_DSP_TransferInfo * transferInfo);

/** ============================================================================
 *  @func   TSKRGB2YCBCR_DSP_delete
 *
 *  @desc   Delete phase function of TSKRGB2YCBCR_DSP application.
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
Int TSKRGB2YCBCR_DSP_delete (TSKRGB2YCBCR_DSP_TransferInfo * transferInfo);


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (TSKRGB2YCBCR_DSP_) */
