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
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/*  ----------------------------------- DSP/BIOS Link                 */
#include <dsplink.h>

/*  ----------------------------------- Application Header            */
#include <rgb2ycbcr-dsp_os.h>
#include <rgb2ycbcr-dsp.h>

/* PNG header to manipulate the image */
#include <png.h>

/* Defines */
#define NUMBER_OF_CHANNELS 3

/* Global data */
int x, y;
int width, height;
png_byte color_type;
png_byte bit_depth;
png_structp png_ptr;
png_infop info_ptr;
int number_of_passes;
png_bytep * row_pointers;

#if defined (__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */

/* FUNCTION PROTOTYPES */
void image_load(
   Char8 * file_name);

void image_store(
   Char8 * file_name);

void abort_(
   const char * s, 
   ...);


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
   Char8 * dspExecutable    = NULL;
   Char8 * strImageInput    = NULL;
   Char8 * strImageOutput   = NULL;
   Char8 * strBufferSize    = NULL;
   Char8 * strProcessorId   = NULL;
   Uint8   processorId      = 0;
   Uint32  dataSize         = 0;
   Uint32  i                = 0;
   Char8 * imageData        = NULL;
   Uint32  numIterations    = 0;
   Char8   strNumIterations[10];
   Char8 * strNumIterationsPtr = NULL;

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

      /* Read input image */
      image_load(strImageInput);

      /* Calculate data size to process. Represent the size of the array in bytes */
      dataSize = width * height * NUMBER_OF_CHANNELS;

      /* Create array of data to be processed by the DSP this is a tmp variable to hold data */
      imageData = malloc(sizeof(Char8) * dataSize);

      /* According to the Data Size and the buffer size we calculate how many iterations are needed */
      numIterations = (Uint32) (dataSize / atoi(strBufferSize));
      if (0 != (dataSize % atoi(strBufferSize))) {
         numIterations += 1;
      }
      snprintf(strNumIterations, 10, "%u", (unsigned int) numIterations);
      strNumIterationsPtr = &strNumIterations[0];

#ifdef DEBUG
      printf("Data Size: %d\nStrBuffSize: %s\nstrNumIteration: %s", dataSize, strBufferSize, &strNumIterations);
#endif

      /* Assign data to new array.
         We have no better way at the moment to use a tmp array with the data */
      for (y = 0; y < height; y++) {
         png_byte* row = row_pointers[y];

         for (x = 0; x < width; x++) {
            png_byte* ptr = &(row[x*3]);
            /* Assign each channel data in R,G,B order */
            if (dataSize < i) {
               printf("ERROR: i greater than array size\n");
               return -1;
            }
            imageData[i]   = ptr[0];
            imageData[i+1] = ptr[1];
            imageData[i+2] = ptr[2];
            i += 3;
         }
      }

      /* Do image processing 
         This is the process we are evaluating. Is in charge of 
         send data to the DSP, apply transformation and get it back */
      if (processorId < MAX_PROCESSORS) {
         RGB2YCBCR_DSP_Main (dspExecutable,
                             imageData,
                             dataSize,
                             strBufferSize,
                             strNumIterationsPtr,
                             strProcessorId);
      }

      /* Transfer analized data into the image data we want to store */
      i = 0;
      for (y = 0; y < height; y++) {
         png_byte* row = row_pointers[y];

         for (x = 0; x < width; x++) {
            png_byte* ptr = &(row[x*3]);
            /* Assign each channel data in R,G,B order */
            if (dataSize < i) {
               printf("ERROR: i greater than array size\n");
               return -1;
            }
            ptr[0] = imageData[i];
            ptr[1] = imageData[i+1];
            ptr[2] = imageData[i+2];
            i += 3;
         }
      }
      /* Store processed image */
      image_store(strImageOutput);

      /* Free image tmp data */
      free(imageData);
   }
   /* We are done */
   return 0 ;
}

/* FUNCTIONS */
void abort_(
   const char * s, 
   ...)
{
   va_list args;
   va_start(args, s);
   vfprintf(stderr, s, args);
   fprintf(stderr, "\n");
   va_end(args);
   abort();
}

void image_load(
   Char8 * file_name)
{
   char header[8];    // 8 is the maximum size that can be checked

   /* open file and test for it being a png */
   FILE *fp = fopen(file_name, "rb");
   if (!fp)
      abort_("[read_png_file] File %s could not be opened for reading", file_name);

   fread(header, 1, 8, fp);

   if (png_sig_cmp(header, 0, 8))
      abort_("[read_png_file] File %s is not recognized as a PNG file", file_name);


   /* initialize stuff */
   png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

   if (!png_ptr)
      abort_("[read_png_file] png_create_read_struct failed");

   info_ptr = png_create_info_struct(png_ptr);
   if (!info_ptr)
      abort_("[read_png_file] png_create_info_struct failed");

   if (setjmp(png_jmpbuf(png_ptr)))
      abort_("[read_png_file] Error during init_io");

   png_init_io(png_ptr, fp);
   png_set_sig_bytes(png_ptr, 8);

   png_read_info(png_ptr, info_ptr);

   width = png_get_image_width(png_ptr, info_ptr);
   height = png_get_image_height(png_ptr, info_ptr);
   color_type = png_get_color_type(png_ptr, info_ptr);
   bit_depth = png_get_bit_depth(png_ptr, info_ptr);

   number_of_passes = png_set_interlace_handling(png_ptr);
   png_read_update_info(png_ptr, info_ptr);


   /* read file */
   if (setjmp(png_jmpbuf(png_ptr)))
      abort_("[read_png_file] Error during read_image");

   row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
   for (y=0; y<height; y++)
      row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));

   png_read_image(png_ptr, row_pointers);

   fclose(fp);
}

void image_store(
   Char8 * file_name)
{
   /* create file */
   FILE *fp = fopen(file_name, "wb");
   if (!fp)
      abort_("[write_png_file] File %s could not be opened for writing", file_name);


   /* initialize stuff */
   png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

   if (!png_ptr)
      abort_("[write_png_file] png_create_write_struct failed");

   info_ptr = png_create_info_struct(png_ptr);
   if (!info_ptr)
      abort_("[write_png_file] png_create_info_struct failed");

   if (setjmp(png_jmpbuf(png_ptr)))
      abort_("[write_png_file] Error during init_io");

   png_init_io(png_ptr, fp);


   /* write header */
   if (setjmp(png_jmpbuf(png_ptr)))
      abort_("[write_png_file] Error during writing header");

   png_set_IHDR(png_ptr, info_ptr, width, height,
                bit_depth, color_type, PNG_INTERLACE_NONE,
                PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

   png_write_info(png_ptr, info_ptr);


   /* write bytes */
   if (setjmp(png_jmpbuf(png_ptr)))
      abort_("[write_png_file] Error during writing bytes");

   png_write_image(png_ptr, row_pointers);


   /* end write */
   if (setjmp(png_jmpbuf(png_ptr)))
      abort_("[write_png_file] Error during end of write");

   png_write_end(png_ptr, NULL);

   /* cleanup heap allocation */
   for (y=0; y<height; y++)
      free(row_pointers[y]);

   free(row_pointers);
   fclose(fp);
}

#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
