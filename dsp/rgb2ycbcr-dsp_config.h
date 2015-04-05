/** ============================================================================
 *  @file   rgb2ycbcr-dsp_config.h
 *
 *  @path   $(DSPLINK)/dsp/src/samples/rgb2ycbcr-dsp/
 *
 *  @desc   Header file containing configuration for the RGB2YCBCR_DSP app.
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


#if !defined (RGB2YCBCR_DSP_CONFIG_)
#define RGB2YCBCR_DSP_CONFIG_


#if defined (__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @const  SAMPLE_POOL_ID
 *
 *  @desc   Pool identifier for this application.
 *  ============================================================================
 */
#define SAMPLE_POOL_ID  0

/** ============================================================================
 *  @const  DSPLINK_DRVNAME
 *
 *  @desc   Name of the DSPLINK driver. This is different based on whether the
 *          sample application uses TSKs or SWIs for data transfer.
 *  ============================================================================
 */
#if defined (SWI_MODE)
#define DSPLINK_DRVNAME  "/dsplink"
#endif

#if defined (TSK_MODE)
#define DSPLINK_DRVNAME  "/dio_dsplink"
#endif

/** ============================================================================
 *  @const  INPUT_CHNL_NAME
 *
 *  @desc   Name of the input channel, generated based on the DSPLINK driver
 *          name specified.
 *  ============================================================================
 */
#define INPUT_CHNL_NAME(drvName)  drvName ## "0"

/** ============================================================================
 *  @const  OUTPUT_CHNL_NAME
 *
 *  @desc   Name of the output channel, generated based on the DSPLINK driver
 *          name specified.
 *  ============================================================================
 */
#define OUTPUT_CHNL_NAME(drvName)  drvName ## "1"

/** ============================================================================
 *  @const  INPUT_CHANNEL
 *
 *  @desc   Name of the input channel.
 *  ============================================================================
 */
#define INPUT_CHANNEL             INPUT_CHNL_NAME(DSPLINK_DRVNAME)

/** ============================================================================
 *  @const  OUTPUT_CHANNEL
 *
 *  @desc   Name of the output channel.
 *  ============================================================================
 */
#define OUTPUT_CHANNEL            OUTPUT_CHNL_NAME(DSPLINK_DRVNAME)

#if defined (CHNL_PCPY_LINK)
/** ============================================================================
 *  @name   NUM_BUF_SIZES
 *
 *  @desc   Number of buffer pools to be configured for the pool.
 *  ============================================================================
 */
#define NUM_BUF_SIZES       1

/** ============================================================================
 *  @name   NUM_BUFFERS
 *
 *  @desc   Number of buffers to be allocated.
 *  ============================================================================
 */
#define NUM_BUFFERS         2

/** ============================================================================
 *  @name   SAMPLEPOOL_open
 *
 *  @desc   Macro over the allocator for opening a pool.
 *
 *  @arg    poolId
 *              Pool id.
 *  @arg    params
 *              Parameters.
 *
 *  @see    SAMPLEPOOL_close
 *  ============================================================================
 */
#define SAMPLEPOOL_open(poolId, params)                        \
        (((POOL->allocators [poolId]).fxns->open)              \
                (&(POOL->allocators [poolId].object),          \
                 params))

/** ============================================================================
 *  @name   SAMPLEPOOL_close
 *
 *  @desc   Macro over the allocator for closing a pool.
 *
 *  @arg    poolId
 *              Pool id.
 *
 *  @see    SAMPLEPOOL_open
 *  ============================================================================
 */
#define SAMPLEPOOL_close(poolId)                         \
        (((POOL->allocators [poolId]).fxns->close)       \
                (POOL->allocators [poolId].object))

/** ============================================================================
 *  @name   POOL
 *
 *  @desc   Configuration variable defined by the POOL component.
 *  ============================================================================
 */
extern POOL_Config  * POOL ;

/** ============================================================================
 *  @name   RGB2YCBCR_DSP_Pools
 *
 *  @desc   Array of pools.
 *  ============================================================================
 */
extern POOL_Obj RGB2YCBCR_DSP_Pools [] ;
#endif /* if defined (CHNL_PCPY_LINK) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (RGB2YCBCR_DSP_CONFIG_) */
