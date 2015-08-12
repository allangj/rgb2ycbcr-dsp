/** ============================================================================
 *  @file   swirgb2ycbcr-dsp.c
 *
 *  @path   $(DSPLINK)/dsp/src/samples/rgb2ycbcr-dsp/
 *
 *  @desc   This is simple LINK SWI application that does data transfer
 *          in rgb2ycbcr mode using GIO interface.
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


/*  ----------------------------------- DSP/BIOS Headers            */
#include <std.h>
#include <log.h>
#include <swi.h>
#include <sys.h>
#include <sio.h>
#include <tsk.h>
#include <iom.h>
#include <gio.h>
#if defined (DSP_BOOTMODE_NOBOOT)
#include <dio.h>
#include <iom.h>
#endif

/*  ----------------------------------- DSP/BIOS LINK Headers       */
#include <failure.h>
#include <platform.h>
#include <dsplink.h>
#if defined (DSP_BOOTMODE_NOBOOT)
#include <sma_pool.h>
#include <zcpy_data.h>
#endif

/*  ----------------------------------- Sample Headers              */
#include <rgb2ycbcr-dsp_config.h>
#include <swirgb2ycbcr-dsp.h>

/** ============================================================================
 *  @const  DXY or CZ
 *
 *  @desc   Values for the D and C matrix used for color space transformation
 *  D = [0.257   0.502  0.098;
 *      -.148  -0.289  0.438;
 *     0.438  -0.366 -0.071];
 *  C = [16; 128; 128];
 *  ============================================================================
 */
#define D11 257
#define D12 502
#define D13 98
#define D21 -148
#define D22 -289
#define D23 438
#define D31 438
#define D32 -366
#define D33 -71
#define C1 16
#define C2 128
#define C3 128

/** ============================================================================
 *  @const  FILEID
 *
 *  @desc   FILEID needed for SET_FAILURE_REASON macro.
 *  ============================================================================
 */
#define FILEID  FID_APP_C

/** ============================================================================
 *  @const  READ_MAILBOX_MASK
 *
 *  @desc   Read mailbox mask.
 *  ============================================================================
 */
#define READ_MAILBOX_MASK   0x1

/** ============================================================================
 *  @const  WRITE_MAILBOX_MASK
 *
 *  @desc   Write mailbox mask.
 *  ============================================================================
 */
#define WRITE_MAILBOX_MASK  0x2

/** ============================================================================
 *  @const  WRITE_MAILBOX_MASK
 *
 *  @desc   Initialial mailbox value.
 *  ============================================================================
 */
#define INITIAL_MAILBOX_VAL (READ_MAILBOX_MASK | WRITE_MAILBOX_MASK)

/** ============================================================================
 *  @name   xferBufSize
 *
 *  @desc   Size of the buffer size for SWI.
 *  ============================================================================
 */
extern Uint32 xferBufSize ;

#if defined (DSP_BOOTMODE_NOBOOT)

/** ============================================================================
 *  @name   DSPLINK_initFlag
 *
 *  @desc   Flag to indicate to the DSPLINK_init.
 *  ============================================================================
 */
extern volatile Uint32 DSPLINK_initFlag  ;

/** ============================================================================
 *  @name   smaPoolObj
 *
 *  @desc   Global object for pool parameters for the dynamic POOL_open call
 *  ============================================================================
 */
extern SMAPOOL_Params            smaPoolObj ;

extern IOM_Fxns ZCPYDATA_FXNS ;

extern Void ZCPYDATA_init (Void) ;

extern DIO_Params dioAttrs ;

extern DEV_Attrs devAttrs ;

extern DEV_Attrs dioDevAttrs ;
#endif

/** ----------------------------------------------------------------------------
 *  @func   readFinishCb/writeFinishCb
 *
 *  @desc   Declaration of callback functions to be called by GIO when
 *          IOM_READ/IOM_WRITE request is over.
 *
 *  @arg    arg
 *              Argument given while doing GIO_submit.
 *          status
 *              Status of IO.
 *          bufp
 *              Buffer on which IO has completed.
 *          size
 *              Size of IO done.
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
static Void readFinishCb  (Ptr arg, Int status, Ptr bufp, Uns size) ;
static Void writeFinishCb (Ptr arg, Int status, Ptr bufp, Uns size) ;

/** ----------------------------------------------------------------------------
 *  @func   rgb2ycbcr_dspSWI
 *
 *  @desc   Entry point for SWI that does data processing.
 *
 *  @arg    arg
 *              Argument given while doing GIO_submit.
 *          status
 *              Status of IO.
 *          bufp
 *              Buffer on which IO has completed.
 *          size
 *              Size of IO done.
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
static Void rgb2ycbcr_dspSWI (Arg arg0, Arg arg1) ;


/** ============================================================================
 *  @func   SWIRGB2YCBCR_DSP_create
 *
 *  @desc   Create phase of SWISWIRGB2YCBCR_DSP application. It allocates
 *          SWISWIRGB2YCBCR_DSP_TransferInfo structure and intializes it with configured
 *          values.
 *
 *  @modif  None.
 *  ============================================================================
 */
Int SWIRGB2YCBCR_DSP_create (SWIRGB2YCBCR_DSP_TransferInfo ** infoPtr)
{
    Int                     status      = SYS_OK ;
    SWI_Attrs               swiAttrs    = SWI_ATTRS ;
    SWIRGB2YCBCR_DSP_TransferInfo *  info;
#if  defined (DSP_BOOTMODE_NOBOOT)
    POOL_Obj                poolObj ;

    {
        while (DSPLINK_initFlag != 0xC0C0BABA) ;
    }
    /* Initialize DSP/BIOS LINK. */
    DSPLINK_init () ;
    smaPoolObj.poolId        = 0;
    smaPoolObj.exactMatchReq = TRUE ;
    poolObj.initFxn          = SMAPOOL_init ;
    poolObj.fxns             = (POOL_Fxns *) &SMAPOOL_FXNS ;
    poolObj.params           = &(smaPoolObj) ;
    poolObj.object           = NULL ;

    status = POOL_open (0, &poolObj) ;

    /* Create IOM driver dynamically */
    status = DEV_createDevice("/dsplink", &ZCPYDATA_FXNS, (Fxn) &ZCPYDATA_init, &devAttrs) ;

    /* Create DIO adapter dynamically */
    status = DEV_createDevice("/dio_dsplink", &DIO_tskDynamicFxns, NULL, &dioDevAttrs);
#endif

    /* Allocate SWIRGB2YCBCR_DSP_TransferInfo structure */
    *infoPtr = MEM_calloc (DSPLINK_SEGID,
                           sizeof (SWIRGB2YCBCR_DSP_TransferInfo),
                           DSPLINK_BUF_ALIGN) ;
    if (*infoPtr == NULL) {
        status = SYS_EALLOC ;
        SET_FAILURE_REASON (status) ;
    }
    else {
        info = *infoPtr ;
    }

    /* Initialize SWIRGB2YCBCR_DSP_TransferInfo structure */
    if (status == SYS_OK) {
        info->bufferSize = xferBufSize ;
        (info->appReadCb).fxn  = readFinishCb ;
        (info->appReadCb).arg  = (Ptr) info ;
        (info->appWriteCb).fxn = writeFinishCb ;
        (info->appWriteCb).arg = (Ptr) info ;
    }

    /* Create channel handles */
    if (status == SYS_OK) {
        GIO_Attrs gioAttrs  = GIO_ATTRS ;
        info->gioInputChan  = GIO_create (INPUT_CHANNEL,
                                          IOM_INPUT,
                                          NULL,
                                          NULL,
                                          &gioAttrs) ;
        info->gioOutputChan = GIO_create (OUTPUT_CHANNEL,
                                          IOM_OUTPUT,
                                          NULL,
                                          NULL,
                                          &gioAttrs) ;
        if (   (info->gioInputChan  == NULL)
            || (info->gioOutputChan == NULL)) {
            status = SYS_EALLOC ;
            SET_FAILURE_REASON (status) ;
        }
    }

    /* Create SWI for sending and receiving data */
    if (status == SYS_OK) {
        swiAttrs.fxn     = rgb2ycbcr_dspSWI ;
        swiAttrs.arg0    = (Arg) info ;
        swiAttrs.mailbox = INITIAL_MAILBOX_VAL ;
        info->swi = SWI_create (&swiAttrs) ;
        if (info->swi == NULL) {
            status = SYS_EALLOC ;
            SET_FAILURE_REASON (status) ;
        }
    }

    /* Allocate input and output buffers */
    if (status == SYS_OK) {
        status = POOL_alloc (SAMPLE_POOL_ID,
                             (Ptr *) &(info->inputBuffer),
                             info->bufferSize) ;
        if (status == SYS_OK) {
            status = POOL_alloc (SAMPLE_POOL_ID,
                                 (Ptr *) &(info->outputBuffer),
                                 info->bufferSize) ;
            if (status != SYS_OK) {
                SET_FAILURE_REASON (status) ;
            }
        }
        else {
            SET_FAILURE_REASON (status) ;
        }
    }

    return status ;
}


/** ============================================================================
 *  @func   SWIRGB2YCBCR_DSP_execute
 *
 *  @desc   Execute phase of SWIRGB2YCBCR_DSP application. It get the application
 *          started after that SWIs get posted automatically.
 *
 *  @modif  None.
 *  ============================================================================
 */
Int SWIRGB2YCBCR_DSP_execute (SWIRGB2YCBCR_DSP_TransferInfo * info)
{
    Int status = SYS_OK ;
    Int iomStatus ;

    /* Submit a read request */
    iomStatus = GIO_submit (info->gioInputChan,
                            IOM_READ,
                            info->inputBuffer,
                            (size_t *) &(info->bufferSize),
                            &(info->appReadCb)) ;

    if (iomStatus != IOM_PENDING) {
        status = SYS_EBADIO ;
        SET_FAILURE_REASON (status) ;
    }

    /* We don't write for the first time so clearing write bit
     * from SWI's mailbox */
    SWI_andn (info->swi, WRITE_MAILBOX_MASK) ;

    return status ;
}


/** ============================================================================
 *  @func   SWIRGB2YCBCR_DSP_delete
 *
 *  @desc   Delete phase of SWIRGB2YCBCR_DSP application. Deallocates all the
 *          resources that were allocated during create phase.
 *
 *  @modif  None.
 *  ============================================================================
 */
Int SWIRGB2YCBCR_DSP_delete (SWIRGB2YCBCR_DSP_TransferInfo * info)
{
    Int status = SYS_OK ;
    Int iomStatus ;

    if (info != NULL) {
        /* Delete SWI */
        if (info->swi != NULL) {
            SWI_delete (info->swi) ;
        }

        /* Delete GIO input channel */
        if (info->gioInputChan != NULL) {
            iomStatus = GIO_delete (info->gioInputChan) ;
            if (iomStatus != IOM_COMPLETED) {
                status = SYS_EBADIO ;
                SET_FAILURE_REASON (status) ;
            }
        }

        /* Delete GIO output channel */
        if (info->gioOutputChan != NULL) {
            iomStatus = GIO_delete (info->gioOutputChan) ;
            if (iomStatus != IOM_COMPLETED) {
                status = SYS_EBADIO ;
                SET_FAILURE_REASON (status) ;
            }
        }

        /* Deallocate inputBuffer */
        if (info->inputBuffer != NULL) {
            POOL_free (SAMPLE_POOL_ID, info->inputBuffer, info->bufferSize) ;
        }

        /* Deallocate outputBuffer */
        if (info->outputBuffer != NULL) {
            POOL_free (SAMPLE_POOL_ID, info->outputBuffer, info->bufferSize) ;
        }

        /* Deallocate info structure */
        if (!MEM_free (DSPLINK_SEGID, info, sizeof (SWIRGB2YCBCR_DSP_TransferInfo))) {
            status = SYS_EALLOC ;
            SET_FAILURE_REASON (status) ;
        }
    }
    else {
        status = SYS_EALLOC ;
    }

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   readFinishCb
 *
 *  @desc   Function to be registered as callback funtion for IOM_READ command
 *          of GIO_submit.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static Void readFinishCb (Ptr arg, Int status, Ptr bufp, Uns size)
{
    SWIRGB2YCBCR_DSP_TransferInfo * info = (SWIRGB2YCBCR_DSP_TransferInfo *) arg ;

    /* Received full buffer from the GPP */
    info->inputBuffer = bufp ;

    info->readWords = size ;
    if (status != IOM_COMPLETED) {
        SET_FAILURE_REASON (status) ;
    }
    /* Give indication to SWI that READ is over */
    SWI_andn (info->swi, READ_MAILBOX_MASK) ;
}


/** ----------------------------------------------------------------------------
 *  @func   writeFinishCb
 *
 *  @desc   Function to registered as callback funtion for IOM_WRITE
 *          command of GIO_submit.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static Void writeFinishCb (Ptr arg, Int status, Ptr bufp, Uns size)
{
    SWIRGB2YCBCR_DSP_TransferInfo *info = (SWIRGB2YCBCR_DSP_TransferInfo *) arg ;

    (Void) size ; /* To remove compiler warning */

    /* Received empty buffer from the GPP */
    info->outputBuffer = bufp ;

    if (status != IOM_COMPLETED) {
        SET_FAILURE_REASON(status) ;
    }
    /* Give indication to SWI that WRITE is over */
    SWI_andn (info->swi, WRITE_MAILBOX_MASK) ;
}


/** ----------------------------------------------------------------------------
 *  @func   rgb2ycbcr_dspSWI
 *
 *  @desc   SWI entry point. This SWI runs every time both IOM_READ and
 *          IOM_WRITE request get completed. It puts does processing and put
 *          next IOM_READ and IOM_WRITE requests.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static Void rgb2ycbcr_dspSWI (Arg arg0, Arg arg1)
{
    Int                    status          = SYS_OK;
    SWIRGB2YCBCR_DSP_TransferInfo * info            = (SWIRGB2YCBCR_DSP_TransferInfo *) arg0;
    Uns                    numWordsToWrite = info->readWords;
    Int                    iomStatus;
    Uns i;
    Uint32                 y,cb,cr;

    (Void) arg1 ; /* To remove compiler warning */

    /* Do processing of data here. IMPORTANT: At the moment it seems the one executing is tsk mode */
    for (i = 0 ; (i+3) <= info->readWords ; i = i+3) {
       y = (((D11 * info->inputBuffer[i]) + (D12 * info->inputBuffer[i+1]) + (D13 * info->inputBuffer[i+2])) / 100) + C1;
       cb = (((D21 * info->inputBuffer[i]) + (D22 * info->inputBuffer[i+1]) + (D23 * info->inputBuffer[i+2])) / 100) + C2;
       cr = (((D31 * info->inputBuffer[i]) + (D32 * info->inputBuffer[i+1]) + (D33 * info->inputBuffer[i+2])) / 100) + C3;

       info->outputBuffer[i] = y;
       info->outputBuffer[i+1] = cb;
       info->outputBuffer[i+2] = cr;
    }

    /* Submit a Read data request */
    iomStatus = GIO_submit (info->gioInputChan,
                            IOM_READ,
                            info->inputBuffer,
                            (size_t *) &(info->bufferSize),
                            &(info->appReadCb)) ;

    if (iomStatus != IOM_PENDING) {
        status = SYS_EBADIO ;
        SET_FAILURE_REASON (status) ;
    }

    /* Submit a Write data request */
    if (status == SYS_OK) {
        iomStatus = GIO_submit (info->gioOutputChan,
                                IOM_WRITE,
                                info->outputBuffer,
                                &numWordsToWrite,
                                &(info->appWriteCb)) ;
    }
    if (iomStatus != IOM_PENDING) {
        status = SYS_EBADIO ;
        SET_FAILURE_REASON (status) ;
    }
}
