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

extern int width, height;
extern png_byte color_type;
extern png_byte bit_depth;

extern png_structp png_ptr;
extern png_infop info_ptr;
extern int number_of_passes;

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
   printf (str);
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
   printf (str, arg);
   fflush (stdout);
}

/** ============================================================================
 *  @func   RGB2YCBCR_DSP_Load_Image
 *
 *  @desc   Load a PNG Image.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API Void RGB2YCBCR_DSP_Load_Image(
   Char8 * file_name;
   Char8 * buff_ptr);
{
   char        header[8]; // 8 is the maximum size that can be checked

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

   // Now we going to fill data in buff
   if (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGBA)
      abort_("[process_file] color_type is RGBA but need to be RGB");

   if (png_get_color_type(png_ptr, info_ptr) != PNG_COLOR_TYPE_RGB)
      abort_("[process_file] input file is not PNG_COLOR_TYPE_RGB");

   for (y=0; y<height; y++) {
      png_byte* row = row_pointers[y];

      for (x=0; x<width; x++) {
         png_byte* ptr = &(row[x*3]);
         /* Assign R G B */
         *buff_ptr++ = ptr[0];
         *buff_ptr++ = ptr[1];
         *buff_ptr++ = ptr[2];
         
      }
   }
}

/** ============================================================================
 *  @func   RGB2YCBCR_DSP_Write_Image
 *
 *  @desc   Write a PNG Image.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API Void RGB2YCBCR_DSP_Write_Image(
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
