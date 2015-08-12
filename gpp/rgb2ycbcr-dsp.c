/** ============================================================================
 *  @file   rgb2ycbcr-dsp.c
 *
 *  @path   $(DSPLINK)/gpp/src/samples/rgb2ycbcr-dsp/
 *
 *  @desc   This is a color space application which uses the dsp
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


/*  ----------------------------------- DSP/BIOS Link                   */
#include <dsplink.h>

/*  ----------------------------------- DSP/BIOS LINK API               */
#include <proc.h>
#include <chnl.h>
#include <pool.h>

/*  ----------------------------------- Application Header              */
#include <rgb2ycbcr-dsp.h>
#include <rgb2ycbcr-dsp_os.h>


#if defined (__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */

//#define DEBUG

/*  ============================================================================
 *  @name   NUM_ARGS
 *
 *  @desc   Number of arguments specified to the DSP application.
 *  ============================================================================
 */
#define NUM_ARGS 3

/*  ============================================================================
 *  @name   XFER_CHAR
 *
 *  @desc   The value used to initialize the output buffer and used for
 *          validation against the input buffer recieved.
 *  ============================================================================
 */
#define XFER_CHAR (Char8) 0xE7

/** ============================================================================
 *  @name   NUMBUFFERPOOLS
 *
 *  @desc   Number of buffer pools in this application.
 *  ============================================================================
 */
#define NUMBUFFERPOOLS 1

/** ============================================================================
 *  @name   NUMBUFS
 *
 *  @desc   Number of buffers in pool.
 *  ============================================================================
 */
#define NUMBUFS 4

/** ============================================================================
 *  @name   POOL_ID
 *
 *  @desc   Pool id for this application.
 *  ============================================================================
 */
#define POOL_ID 0

/*  ============================================================================
 *  @name   RGB2YCBCR_DSP_BufferSize
 *
 *  @desc   Size of buffer to be used for data transfer.
 *  ============================================================================
 */
STATIC Uint32 RGB2YCBCR_DSP_BufferSize;

/*  ============================================================================
 *  @name   RGB2YCBCR_DSP_NumIterations
 *
 *  @desc   Number of iterations of data transfer. Depends on data total size
 *  ============================================================================
 */
STATIC Uint32 RGB2YCBCR_DSP_NumIterations;

/** ============================================================================
 *  @name   RGB2YCBCR_DSP_Buffers
 *
 *  @desc   Array of buffers used by the rgb2ycbcr-dsp application.
 *          Length of array in this application is 1.
 *  ============================================================================
 */
STATIC Char8 * RGB2YCBCR_DSP_Buffers[1];

/** ============================================================================
 *  @name   RGB2YCBCR_DSP_IOReq
 *
 *  @desc   It gives information for adding or reclaiming a request.
 *  ============================================================================
 */
STATIC ChannelIOInfo RGB2YCBCR_DSP_IOReq;

/** ============================================================================
 *  @func   RGB2YCBCR_DSP_Create
 *
 *  @desc   This function allocates and initializes resources used by
 *          this application.
 *
 *  @modif  RGB2YCBCR_DSP_Buffers
 *  ============================================================================
 */
NORMAL_API DSP_STATUS RGB2YCBCR_DSP_Create (
   IN Char8  * dspExecutable,
   IN Char8  * strBufferSize,
   IN Char8  * strNumIterations,
   IN Uint8    processorId)
{
   DSP_STATUS status                    = DSP_SOK;
   Uint32     numArgs                   = 0;
   Uint32     numBufs [NUMBUFFERPOOLS]  = {NUMBUFS};

   ChannelAttrs chnlAttrInput;
   ChannelAttrs chnlAttrOutput;
   Char8 *      args [NUM_ARGS];
   Uint32       size [NUMBUFFERPOOLS];
#if defined (ZCPY_LINK)
   SMAPOOL_Attrs poolAttrs;
#endif /* if defined (ZCPY_LINK) */

   RGB2YCBCR_DSP_0Print("Entered RGB2YCBCR_DSP_Create ()\n");

   /*
    *  Create and initialize the proc object.
    */
   status = PROC_setup (NULL);

   /*
    *  Attach the Dsp with which the transfers have to be done.
    */
   if (DSP_SUCCEEDED (status)) {
      status = PROC_attach (processorId, NULL);
      if (DSP_FAILED (status)) {
         RGB2YCBCR_DSP_1Print ("PROC_attach failed . Status = [0x%x]\n", status) ;
      }
   }
   else {
      RGB2YCBCR_DSP_1Print ("PROC_setup failed. Status =  [0x%x]\n", status);
   }

   /*
    * Open the pool.
    */
   if (DSP_SUCCEEDED (status)) {
      size [0]              = RGB2YCBCR_DSP_BufferSize;
      poolAttrs.bufSizes    = (Uint32 *) &size;
      poolAttrs.numBuffers  = (Uint32 *) &numBufs;
      poolAttrs.numBufPools = NUMBUFFERPOOLS;
#if defined (ZCPY_LINK)
      poolAttrs.exactMatchReq = TRUE;
#endif /* if defined (ZCPY_LINK) */

      status = POOL_open(POOL_makePoolId(processorId, POOL_ID), &poolAttrs);
      if (DSP_FAILED (status)) {
         RGB2YCBCR_DSP_1Print ("POOL_open () failed. Status = [0x%x]\n", status);
      }
   }

   /*
    *  Load the executable on the DSP.
    */
   if (DSP_SUCCEEDED (status)) {
      numArgs = NUM_ARGS;
      args[0] = strBufferSize;
      args[1] = strNumIterations;

      status = PROC_load (processorId, dspExecutable, numArgs, args);

      if (DSP_FAILED (status)) {
         RGB2YCBCR_DSP_1Print ("PROC_load failed. Status = [0x%x]\n", status);
      }
   }

   /*
    *  Create a channel to DSP
    */
   if (DSP_SUCCEEDED (status)) {
      chnlAttrOutput.mode      = ChannelMode_Output;
      chnlAttrOutput.endianism = Endianism_Default;
      chnlAttrOutput.size      = ChannelDataSize_16bits;

      status = CHNL_create (processorId, CHNL_ID_OUTPUT, &chnlAttrOutput);
      if (DSP_FAILED (status)) {
         RGB2YCBCR_DSP_1Print ("CHNL_create failed (output). Status = [0x%x]\n", status);
      }
   }

   /*
    *  Create a channel from DSP
    */
   if (DSP_SUCCEEDED (status)) {
      chnlAttrInput.mode      = ChannelMode_Input;
      chnlAttrInput.endianism = Endianism_Default;
      chnlAttrInput.size      = ChannelDataSize_16bits;

      status = CHNL_create (processorId, CHNL_ID_INPUT, &chnlAttrInput);
      if (DSP_FAILED (status)) {
         RGB2YCBCR_DSP_1Print ("CHNL_create failed (input). Status = [0x%x]\n", status);
      }
   }

   /*
    *  Allocate buffer(s) for data transfer to DSP.
    */
   if (DSP_SUCCEEDED (status)) {
      status = CHNL_allocateBuffer (processorId,
                                    CHNL_ID_OUTPUT,
                                    RGB2YCBCR_DSP_Buffers,
                                    RGB2YCBCR_DSP_BufferSize,
                                    1);
      if (DSP_FAILED (status)) {
         RGB2YCBCR_DSP_1Print ("CHNL_allocateBuffer failed (output)."
                               "Status = [0x%x]\n",
                               status);
      }
   }

   /*
    *  Initialize the buffer with valid data is now perform on execute phase.
    */

   RGB2YCBCR_DSP_0Print ("Leaving RGB2YCBCR_DSP_Create ()\n") ;

   return status ;
}


/** ============================================================================
 *  @func   RGB2YCBCR_DSP_Execute
 *
 *  @desc   This function implements the execute phase for this application.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API DSP_STATUS RGB2YCBCR_DSP_Execute(
   IN Uint32 numIterations,
   IN Char8  * dataStream,
   IN Uint32 bufferSize,
   Uint8     processorId)
{
   DSP_STATUS status = DSP_SOK;
   Uint32     i;
   Uint32     j;
   Uint32     offsetData;
   Char8 *    temp = NULL;

   RGB2YCBCR_DSP_0Print ("Entered RGB2YCBCR_DSP_Execute ()\n") ;

   /*
    *  Start execution on DSP.
    */
   status = PROC_start(processorId);

#ifdef DEBUG
   RGB2YCBCR_DSP_0Print("Execute proc, PROC started\n");
#endif

   /*
    *  Fill the IO Request structure
    *  It gives Information for adding or reclaiming an input request.
    */
   if (DSP_SUCCEEDED (status)) {
      RGB2YCBCR_DSP_IOReq.buffer = RGB2YCBCR_DSP_Buffers [0];
      RGB2YCBCR_DSP_IOReq.size   = RGB2YCBCR_DSP_BufferSize;
   }
   else {
      RGB2YCBCR_DSP_1Print("PROC_start failed. Status = [0x%x]\n", status);
   }

#ifdef DEBUG
   RGB2YCBCR_DSP_1Print("Execute proc, starting iterations: %d\n", RGB2YCBCR_DSP_NumIterations);
#endif

   for (i = 1;
        ((RGB2YCBCR_DSP_NumIterations == 0) || (i <= RGB2YCBCR_DSP_NumIterations))
         && (DSP_SUCCEEDED (status)); i++) {

#ifdef DEBUG
      RGB2YCBCR_DSP_1Print("Execute proc, iteration: %d\n", i);
#endif

      /* Calculate offset */
      offsetData = bufferSize * (i - 1);

      /* Pointer to the Buffer beggining */
      temp = RGB2YCBCR_DSP_Buffers[0];

      /* Fill buffer with valid data */
      for (j = 0 ; j < bufferSize ; j++) {
         *temp++ = (dataStream[j + offsetData]);
      }

#ifdef DEBUG
      RGB2YCBCR_DSP_1Print("Execute proc, offsetData: %d\n", offsetData);
#endif

      /* Start counting time from here */
      /*
       *  Send data to DSP.
       *  Issue 'filled' buffer to the channel.
       */
      status = CHNL_issue(processorId, CHNL_ID_OUTPUT, &RGB2YCBCR_DSP_IOReq);
      if (DSP_FAILED (status)) {
         RGB2YCBCR_DSP_1Print("CHNL_issue failed (output). Status = [0x%x]\n", status);
      }

#ifdef DEBUG
      RGB2YCBCR_DSP_0Print("Execute proc, data send\n");
#endif

      /*
       *  Reclaim 'empty' buffer from the channel
       */
      if (DSP_SUCCEEDED (status)) {
         status = CHNL_reclaim (processorId, CHNL_ID_OUTPUT, WAIT_FOREVER, &RGB2YCBCR_DSP_IOReq);

         if (DSP_FAILED (status)) {
            RGB2YCBCR_DSP_1Print ("CHNL_reclaim failed (output). Status = [0x%x]\n", status);
         }
      }

#ifdef DEBUG
      RGB2YCBCR_DSP_0Print("Execute proc, empty buffer reclaimed\n");
#endif

      /*
       *  Receive data from DSP
       *  Issue 'empty' buffer to the channel.
       */
      if (DSP_SUCCEEDED (status)) {
         status = CHNL_issue (processorId, CHNL_ID_INPUT, &RGB2YCBCR_DSP_IOReq) ;
         if (DSP_FAILED (status)) {
            RGB2YCBCR_DSP_1Print ("CHNL_issue failed (input). Status = [0x%x]\n", status);
         }
      }

#ifdef DEBUG
      RGB2YCBCR_DSP_0Print("Execute proc, data buffer reclaimed\n");
#endif
      /*
       *  Reclaim 'filled' buffer from the channel
       */
      if (DSP_SUCCEEDED (status)) {
         status = CHNL_reclaim (processorId, CHNL_ID_INPUT, WAIT_FOREVER, &RGB2YCBCR_DSP_IOReq);
         if (DSP_FAILED (status)) {
            RGB2YCBCR_DSP_1Print ("CHNL_reclaim failed (input). Status = [0x%x]\n", status) ;
         }
      }

#ifdef DEBUG
      RGB2YCBCR_DSP_0Print("Execute proc, buffer filled\n");
#endif
      /* Stop counting time */

      /* We have transfered all data and got it back */
      if (DSP_SUCCEEDED (status)) {
         // Print execution time
      }
      else {
         // We fail
         RGB2YCBCR_DSP_0Print("Execute proc, ERROR\n");
         break;
      }

      /* Pointer to the Buffer beggining */
      temp = RGB2YCBCR_DSP_Buffers[0];
      /* Put proccesed data back on the data stream */
      offsetData = bufferSize * (i - 1);
      for (j = 0 ; j < bufferSize ; j++) {
         (dataStream[j + offsetData]) = *temp++;
      }
   }

   RGB2YCBCR_DSP_0Print ("Leaving RGB2YCBCR_DSP_Execute ()\n") ;

   return status ;
}


/** ============================================================================
 *  @func   RGB2YCBCR_DSP_Delete
 *
 *  @desc   This function releases resources allocated earlier by call to
 *          RGB2YCBCR_DSP_Create ().
 *          During cleanup, the allocated resources are being freed
 *          unconditionally. Actual applications may require stricter check
 *          against return values for robustness.
 *
 *  @modif  RGB2YCBCR_DSP_Buffers
 *  ============================================================================
 */
NORMAL_API Void RGB2YCBCR_DSP_Delete(
   Uint8 processorId)
{
   DSP_STATUS status    = DSP_SOK;
   DSP_STATUS tmpStatus = DSP_SOK;

   RGB2YCBCR_DSP_0Print ("Entered RGB2YCBCR_DSP_Delete ()\n");

   /*
    *  Free the buffer(s) allocated for channel to DSP
    */
   tmpStatus = CHNL_freeBuffer (processorId, CHNL_ID_OUTPUT, RGB2YCBCR_DSP_Buffers, 1);
   if (DSP_SUCCEEDED (status) && DSP_FAILED (tmpStatus)) {
      RGB2YCBCR_DSP_1Print ("CHNL_freeBuffer () failed (output). Status = [0x%x]\n", tmpStatus);
   }

   /*
    *  Delete both input and output channels
    */
   tmpStatus = CHNL_delete (processorId, CHNL_ID_INPUT);
   if (DSP_SUCCEEDED (status) && DSP_FAILED (tmpStatus)) {
      RGB2YCBCR_DSP_1Print ("CHNL_delete () failed (input). Status = [0x%x]\n", tmpStatus) ;
   }

   tmpStatus = CHNL_delete (processorId, CHNL_ID_OUTPUT);
   if (DSP_SUCCEEDED (status) && DSP_FAILED (tmpStatus)) {
      RGB2YCBCR_DSP_1Print ("CHNL_delete () failed (output). Status = [0x%x]\n", tmpStatus);
   }

   /*
    *  Stop execution on DSP.
    */
   status = PROC_stop (processorId);

   /*
    *  Close the pool
    */
   tmpStatus = POOL_close (POOL_makePoolId(processorId, POOL_ID));
   if (DSP_SUCCEEDED (status) && DSP_FAILED (tmpStatus)) {
      RGB2YCBCR_DSP_1Print ("POOL_close () failed. Status = [0x%x]\n", tmpStatus);
   }

   /*
    *  Detach from the processor
    */
   tmpStatus = PROC_detach (processorId);
   if (DSP_SUCCEEDED (status) && DSP_FAILED (tmpStatus)) {
      RGB2YCBCR_DSP_1Print ("PROC_detach () failed. Status = [0x%x]\n", tmpStatus);
   }

   /*
    *  Destroy the PROC object.
    */
   tmpStatus = PROC_destroy ();
   if (DSP_SUCCEEDED (status) && DSP_FAILED (tmpStatus)) {
      RGB2YCBCR_DSP_1Print ("PROC_destroy () failed. Status = [0x%x]\n", tmpStatus);
   }

   RGB2YCBCR_DSP_0Print ("Leaving RGB2YCBCR_DSP_Delete ()\n") ;
}


/** ============================================================================
 *  @func   RGB2YCBCR_DSP_Main
 *
 *  @desc   Entry point for the application
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API Void RGB2YCBCR_DSP_Main(
   IN Char8  * dspExecutable,
   IN Char8  * dataStream,
   IN Uint32   dataSize,
   IN Char8  * strBufferSize,
   IN Char8  * strNumIterations,
   IN Char8  * strProcessorId)
{
   DSP_STATUS status      = DSP_SOK;
   Uint8      processorId = 0;

   RGB2YCBCR_DSP_0Print("=============== tecSat Application : RGB2YCBCR_DSP ==========\n");

   if ((dspExecutable != NULL) && (dataStream != NULL) && (dataSize != 0) && 
       (strBufferSize != NULL) && (strNumIterations != NULL) && (strProcessorId != NULL)) {
      /*
       *  Validate the buffer size and number of iterations specified.
       */
      RGB2YCBCR_DSP_BufferSize = DSPLINK_ALIGN(RGB2YCBCR_DSP_Atoi(strBufferSize), DSPLINK_BUF_ALIGN);

      if (RGB2YCBCR_DSP_BufferSize == 0) {
         status = DSP_ESIZE;
      }

      /* Get the number of Iterations */
      RGB2YCBCR_DSP_NumIterations = RGB2YCBCR_DSP_Atoi(strNumIterations);
      if (0 == RGB2YCBCR_DSP_NumIterations)
      {
         RGB2YCBCR_DSP_1Print("==Erro: Invalid number of iterations %d ==\n", RGB2YCBCR_DSP_NumIterations);
         status = DSP_EFAIL;
      }

      /* Find out the processor id to work with */
      processorId = RGB2YCBCR_DSP_Atoi(strProcessorId);
      if (processorId >= MAX_DSPS) {
         RGB2YCBCR_DSP_1Print ("==Error: Invalid processor id  specified %d ==\n", processorId);
         status = DSP_EFAIL;
      }

      /*
       *  Specify the dsp executable file name and the buffer size for
       *  rgb2ycbcr-dsp creation phase.
       */
      if (DSP_SUCCEEDED(status)) {
         RGB2YCBCR_DSP_1Print ("==== Executing tecSat sample for DSP processor Id %d ====\n", processorId);

         status = RGB2YCBCR_DSP_Create(dspExecutable, strBufferSize, strNumIterations, processorId);

         /*
          *  Execute the data transfer rgb2ycbcr-dsp.
          */
         if (DSP_SUCCEEDED(status)) {
            status = RGB2YCBCR_DSP_Execute(RGB2YCBCR_DSP_NumIterations, dataStream, RGB2YCBCR_DSP_BufferSize, processorId);
         }

         /*
          *  Perform cleanup operation.
          */
         RGB2YCBCR_DSP_Delete(processorId);
      }
   }
   else {
      status = DSP_EINVALIDARG;
      RGB2YCBCR_DSP_0Print("ERROR! Invalid arguments specified for while executing "
                            "rgb2ycbcr-dsp application\n") ;
   }

   RGB2YCBCR_DSP_0Print ("====================================================\n") ;
}

#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
