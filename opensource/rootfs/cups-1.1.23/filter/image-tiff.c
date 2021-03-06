/*
 * "$Id: image-tiff.c,v 1.1.1.1.12.1 2009/02/03 08:27:05 wiley Exp $"
 *
 *   TIFF file routines for the Common UNIX Printing System (CUPS).
 *
 *   Copyright 1993-2005 by Easy Software Products.
 *
 *   These coded instructions, statements, and computer programs are the
 *   property of Easy Software Products and are protected by Federal
 *   copyright law.  Distribution and use rights are outlined in the file
 *   "LICENSE.txt" which should have been included with this file.  If this
 *   file is missing or damaged please contact Easy Software Products
 *   at:
 *
 *       Attn: CUPS Licensing Information
 *       Easy Software Products
 *       44141 Airport View Drive, Suite 204
 *       Hollywood, Maryland 20636 USA
 *
 *       Voice: (301) 373-9600
 *       EMail: cups-info@cups.org
 *         WWW: http://www.cups.org
 *
 *   This file is subject to the Apple OS-Developed Software exception.
 *
 * Contents:
 *
 *   ImageReadTIFF() - Read a TIFF image file.
 */

/*
 * Include necessary headers...
 */

#include "image.h"

#ifdef HAVE_LIBTIFF
#  include <tiff.h>	/* TIFF image definitions */
#  include <tiffio.h>
#  include <unistd.h>


/*
 * 'ImageReadTIFF()' - Read a TIFF image file.
 */

int					/* O - Read status */
ImageReadTIFF(image_t    *img,		/* IO - Image */
              FILE       *fp,		/* I - Image file */
              int        primary,	/* I - Primary choice for colorspace */
              int        secondary,	/* I - Secondary choice for colorspace */
              int        saturation,	/* I - Color saturation (%) */
              int        hue,		/* I - Color hue (degrees) */
	      const ib_t *lut)		/* I - Lookup table for gamma/brightness */
{
  TIFF		*tif;			/* TIFF file */
  uint32	width, height;		/* Size of image */
  uint16	photometric,		/* Colorspace */
		compression,		/* Type of compression */
		orientation,		/* Orientation */
		resunit,		/* Units for resolution */
		samples,		/* Number of samples/pixel */
		bits,			/* Number of bits/pixel */
		inkset,			/* Ink set for color separations */
		numinks;		/* Number of inks in set */
  float		xres,			/* Horizontal resolution */
		yres;			/* Vertical resolution */
  uint16	*redcmap,		/* Red colormap information */
		*greencmap,		/* Green colormap information */
		*bluecmap;		/* Blue colormap information */
  int		c,			/* Color index */
		num_colors,		/* Number of colors */
		bpp,			/* Bytes per pixel */
		x, y,			/* Current x & y */
		row,			/* Current row in image */
		xstart, ystart,		/* Starting x & y */
		xdir, ydir,		/* X & y direction */
		xcount, ycount,		/* X & Y counters */
		pstep,			/* Pixel step (= bpp or -2 * bpp) */
		scanwidth,		/* Width of scanline */
		r, g, b, k,		/* Red, green, blue, and black values */
		alpha;			/* Image includes alpha? */
  ib_t		*in,			/* Input buffer */
		*out,			/* Output buffer */
		*p,			/* Pointer into buffer */
		*scanline,		/* Scanline buffer */
		*scanptr,		/* Pointer into scanline buffer */
		bit,			/* Current bit */
		pixel,			/* Current pixel */
		zero,			/* Zero value (bitmaps) */
		one;			/* One value (bitmaps) */


 /*
  * Open the TIFF file and get the required parameters...
  */

  lseek(fileno(fp), 0, SEEK_SET); /* Work around "feature" in some stdio's */

  if ((tif = TIFFFdOpen(fileno(fp), "", "r")) == NULL)
  {
    fputs("ERROR: TIFFFdOpen() failed!\n", stderr);
    fclose(fp);
    return (-1);
  }

  if (!TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width))
  {
    fputs("ERROR: No image width tag in the file!\n", stderr);
    TIFFClose(tif);
    fclose(fp);
    return (-1);
  }

  if (!TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height))
  {
    fputs("ERROR: No image height tag in the file!\n", stderr);
    TIFFClose(tif);
    fclose(fp);
    return (-1);
  }

  if (!TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric))
  {
    fputs("ERROR: No photometric tag in the file!\n", stderr);
    TIFFClose(tif);
    fclose(fp);
    return (-1);
  }

  if (!TIFFGetField(tif, TIFFTAG_COMPRESSION, &compression))
  {
    fputs("ERROR: No compression tag in the file!\n", stderr);
    TIFFClose(tif);
    fclose(fp);
    return (-1);
  }

  if (!TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samples))
    samples = 1;

  if (!TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bits))
    bits = 1;

 /*
  * Get the image orientation...
  */

  if (!TIFFGetField(tif, TIFFTAG_ORIENTATION, &orientation))
    orientation = 0;

 /*
  * Get the image resolution...
  */

  if (TIFFGetField(tif, TIFFTAG_XRESOLUTION, &xres) &&
      TIFFGetField(tif, TIFFTAG_YRESOLUTION, &yres) &&
      TIFFGetField(tif, TIFFTAG_RESOLUTIONUNIT, &resunit))
  {
    if (resunit == RESUNIT_INCH)
    {
      img->xppi = xres;
      img->yppi = yres;
    }
    else if (resunit == RESUNIT_CENTIMETER)
    {
      img->xppi = xres * 2.54;
      img->yppi = yres * 2.54;
    }
    else
    {
      img->xppi = 128;
      img->yppi = 128;
    }

    if (img->xppi == 0 || img->yppi == 0)
    {
      fputs("ERROR: Bad TIFF resolution.\n", stderr);
      img->xppi = img->yppi = 128;
    }

    fprintf(stderr, "DEBUG: TIFF resolution = %fx%f, units=%d\n",
            xres, yres, resunit);
    fprintf(stderr, "DEBUG: Stored resolution = %dx%d PPI\n",
            img->xppi, img->yppi);
  }

 /*
  * See if the image has an alpha channel...
  */

  if (samples == 2 || (samples == 4 && photometric == PHOTOMETRIC_RGB))
    alpha = 1;
  else
    alpha = 0;

 /*
  * Check the size of the image...
  */

  if (width == 0 || width > IMAGE_MAX_WIDTH ||
      height == 0 || height > IMAGE_MAX_HEIGHT ||
      (bits != 1 && bits != 2 && bits != 4 && bits != 8) ||
      samples < 1 || samples > 4)
  {
    fprintf(stderr, "ERROR: Bad TIFF dimensions %ux%ux%ux%u!\n",
            (unsigned)width, (unsigned)height, (unsigned)bits,
	    (unsigned)samples);
    TIFFClose(tif);
    fclose(fp);
    return (1);
  }

 /*
  * Setup the image size and colorspace...
  */

  img->xsize = width;
  img->ysize = height;
  if (photometric == PHOTOMETRIC_MINISBLACK ||
      photometric == PHOTOMETRIC_MINISWHITE)
    img->colorspace = secondary;
  else if (photometric == PHOTOMETRIC_SEPARATED && primary == IMAGE_RGB_CMYK)
    img->colorspace = IMAGE_CMYK;
  else if (primary == IMAGE_RGB_CMYK)
    img->colorspace = IMAGE_RGB;
  else
    img->colorspace = primary;

  fprintf(stderr, "DEBUG: img->colorspace = %d\n", img->colorspace);

  bpp = ImageGetDepth(img);

  ImageSetMaxTiles(img, 0);

 /*
  * Set the X & Y start and direction according to the image orientation...
  */

  switch (orientation)
  {
    case ORIENTATION_TOPRIGHT :
        fputs("DEBUG: orientation = top-right\n", stderr);
        break;
    case ORIENTATION_RIGHTTOP :
        fputs("DEBUG: orientation = right-top\n", stderr);
        break;
    default :
    case ORIENTATION_TOPLEFT :
        fputs("DEBUG: orientation = top-left\n", stderr);
        break;
    case ORIENTATION_LEFTTOP :
        fputs("DEBUG: orientation = left-top\n", stderr);
        break;
    case ORIENTATION_BOTLEFT :
        fputs("DEBUG: orientation = bottom-left\n", stderr);
        break;
    case ORIENTATION_LEFTBOT :
        fputs("DEBUG: orientation = left-bottom\n", stderr);
        break;
    case ORIENTATION_BOTRIGHT :
        fputs("DEBUG: orientation = bottom-right\n", stderr);
        break;
    case ORIENTATION_RIGHTBOT :
        fputs("DEBUG: orientation = right-bottom\n", stderr);
        break;
  }

  switch (orientation)
  {
    case ORIENTATION_TOPRIGHT :
    case ORIENTATION_RIGHTTOP :
        xstart = img->xsize - 1;
        xdir   = -1;
        ystart = 0;
        ydir   = 1;
        break;
    default :
    case ORIENTATION_TOPLEFT :
    case ORIENTATION_LEFTTOP :
        xstart = 0;
        xdir   = 1;
        ystart = 0;
        ydir   = 1;
        break;
    case ORIENTATION_BOTLEFT :
    case ORIENTATION_LEFTBOT :
        xstart = 0;
        xdir   = 1;
        ystart = img->ysize - 1;
        ydir   = -1;
        break;
    case ORIENTATION_BOTRIGHT :
    case ORIENTATION_RIGHTBOT :
        xstart = img->xsize - 1;
        xdir   = -1;
        ystart = img->ysize - 1;
        ydir   = -1;
        break;
  }

 /*
  * Allocate a scanline buffer...
  */

  scanwidth = TIFFScanlineSize(tif);
  scanline  = _TIFFmalloc(scanwidth);

 /*
  * Allocate input and output buffers...
  */

  if (orientation < ORIENTATION_LEFTTOP)
  {
    if (samples > 1 || photometric == PHOTOMETRIC_PALETTE)
      pstep = xdir * 3;
    else
      pstep = xdir;

    in  = malloc(img->xsize * 3 + 3);
    out = malloc(img->xsize * bpp);
  }
  else
  {
    if (samples > 1 || photometric == PHOTOMETRIC_PALETTE)
      pstep = ydir * 3;
    else
      pstep = ydir;

    in  = malloc(img->ysize * 3 + 3);
    out = malloc(img->ysize * bpp);
  }

 /*
  * Read the image.  This is greatly complicated by the fact that TIFF
  * supports literally hundreds of different colorspaces and orientations,
  * each which must be handled separately...
  */

  fprintf(stderr, "DEBUG: photometric = %d\n", photometric);
  fprintf(stderr, "DEBUG: compression = %d\n", compression);

  switch (photometric)
  {
    case PHOTOMETRIC_MINISWHITE :
    case PHOTOMETRIC_MINISBLACK :
        if (photometric == PHOTOMETRIC_MINISWHITE)
        {
          zero = 255;
          one  = 0;
        }
        else
        {
          zero = 0;
          one  = 255;
        }

        if (orientation < ORIENTATION_LEFTTOP)
        {
         /*
          * Row major order...
          */

          for (y = ystart, ycount = img->ysize, row = 0;
               ycount > 0;
               ycount --, y += ydir, row ++)
          {
            if (bits == 1)
            {
              TIFFReadScanline(tif, scanline, row, 0);
              for (xcount = img->xsize, scanptr = scanline, p = in + xstart, bit = 128;
                   xcount > 0;
                   xcount --, p += pstep)
              {
        	if (*scanptr & bit)
                  *p = one;
                else
                  *p = zero;

        	if (bit > 1)
                  bit >>= 1;
        	else
        	{
                  bit = 128;
                  scanptr ++;
        	}
              }
            }
            else if (bits == 2)
            {
              TIFFReadScanline(tif, scanline, row, 0);
              for (xcount = img->xsize, scanptr = scanline, p = in + xstart, bit = 0xc0;
                   xcount > 0;
                   xcount --, p += pstep)
              {
                pixel = *scanptr & bit;
                while (pixel > 3)
                  pixel >>= 2;
                *p = (255 * pixel / 3) ^ zero;

        	if (bit > 3)
                  bit >>= 2;
        	else
        	{
                  bit = 0xc0;
                  scanptr ++;
        	}
              }
            }
            else if (bits == 4)
            {
              TIFFReadScanline(tif, scanline, row, 0);
              for (xcount = img->xsize, scanptr = scanline, p = in + xstart, bit = 0xf0;
                   xcount > 0;
                   xcount --, p += pstep)
              {
                if (bit == 0xf0)
                {
                  *p = (255 * ((*scanptr & 0xf0) >> 4) / 15) ^ zero;
                  bit = 0x0f;
                }
                else
        	{
                  *p = (255 * (*scanptr & 0x0f) / 15) ^ zero;
                  bit = 0xf0;
                  scanptr ++;
        	}
              }
            }
            else if (xdir < 0 || zero || alpha)
            {
              TIFFReadScanline(tif, scanline, row, 0);

              if (alpha)
	      {
        	if (zero)
        	{
                  for (xcount = img->xsize, p = in + xstart, scanptr = scanline;
                       xcount > 0;
                       xcount --, p += pstep, scanptr += 2)
                    *p = (scanptr[1] * (255 - scanptr[0]) +
		          (255 - scanptr[1]) * 255) / 255;
        	}
        	else
        	{
                  for (xcount = img->xsize, p = in + xstart, scanptr = scanline;
                       xcount > 0;
                       xcount --, p += pstep, scanptr += 2)
                    *p = (scanptr[1] * scanptr[0] +
		          (255 - scanptr[1]) * 255) / 255;
        	}
	      }
	      else
	      {
        	if (zero)
        	{
                  for (xcount = img->xsize, p = in + xstart, scanptr = scanline;
                       xcount > 0;
                       xcount --, p += pstep, scanptr ++)
                    *p = 255 - *scanptr;
        	}
        	else
        	{
                  for (xcount = img->xsize, p = in + xstart, scanptr = scanline;
                       xcount > 0;
                       xcount --, p += pstep, scanptr ++)
                    *p = *scanptr;
        	}
              }
            }
            else
              TIFFReadScanline(tif, in, row, 0);

            if (img->colorspace == IMAGE_WHITE)
	    {
	      if (lut)
	        ImageLut(in, img->xsize, lut);

              ImagePutRow(img, 0, y, img->xsize, in);
	    }
            else
            {
	      switch (img->colorspace)
	      {
		case IMAGE_RGB :
		    ImageWhiteToRGB(in, out, img->xsize);
		    break;
		case IMAGE_BLACK :
		    ImageWhiteToBlack(in, out, img->xsize);
		    break;
		case IMAGE_CMY :
		    ImageWhiteToCMY(in, out, img->xsize);
		    break;
		case IMAGE_CMYK :
		    ImageWhiteToCMYK(in, out, img->xsize);
		    break;
	      }

	      if (lut)
	        ImageLut(out, img->xsize * bpp, lut);

              ImagePutRow(img, 0, y, img->xsize, out);
	    }
          }
        }
        else
        {
         /*
          * Column major order...
          */

          for (x = xstart, xcount = img->xsize, row = 0;
               xcount > 0;
               xcount --, x += xdir, row ++)
          {
            if (bits == 1)
            {
              TIFFReadScanline(tif, scanline, row, 0);
              for (ycount = img->ysize, scanptr = scanline, p = in + ystart, bit = 128;
                   ycount > 0;
                   ycount --, p += ydir)
              {
        	if (*scanptr & bit)
                  *p = one;
                else
                  *p = zero;

        	if (bit > 1)
                  bit >>= 1;
        	else
        	{
                  bit = 128;
                  scanptr ++;
        	}
              }
            }
            else if (bits == 2)
            {
              TIFFReadScanline(tif, scanline, row, 0);
              for (ycount = img->ysize, scanptr = scanline, p = in + ystart, bit = 0xc0;
                   ycount > 0;
                   ycount --, p += ydir)
              {
                pixel = *scanptr & 0xc0;
                while (pixel > 3)
                  pixel >>= 2;

                *p = (255 * pixel / 3) ^ zero;

        	if (bit > 3)
                  bit >>= 2;
        	else
        	{
                  bit = 0xc0;
                  scanptr ++;
        	}
              }
            }
            else if (bits == 4)
            {
              TIFFReadScanline(tif, scanline, row, 0);
              for (ycount = img->ysize, scanptr = scanline, p = in + ystart, bit = 0xf0;
                   ycount > 0;
                   ycount --, p += ydir)
              {
        	if (bit == 0xf0)
        	{
                  *p = (255 * ((*scanptr & 0xf0) >> 4) / 15) ^ zero;
                  bit = 0x0f;
                }
        	else
        	{
                  *p = (255 * (*scanptr & 0x0f) / 15) ^ zero;
                  bit = 0xf0;
                  scanptr ++;
        	}
              }
            }
            else if (ydir < 0 || zero || alpha)
            {
              TIFFReadScanline(tif, scanline, row, 0);

              if (alpha)
	      {
		if (zero)
        	{
                  for (ycount = img->ysize, p = in + ystart, scanptr = scanline;
                       ycount > 0;
                       ycount --, p += ydir, scanptr += 2)
                    *p = (scanptr[1] * (255 - scanptr[0]) +
		          (255 - scanptr[1]) * 255) / 255;
        	}
        	else
        	{
                  for (ycount = img->ysize, p = in + ystart, scanptr = scanline;
                       ycount > 0;
                       ycount --, p += ydir, scanptr += 2)
                    *p = (scanptr[1] * scanptr[0] +
		          (255 - scanptr[1]) * 255) / 255;
        	}
              }
	      else
	      {
		if (zero)
        	{
                  for (ycount = img->ysize, p = in + ystart, scanptr = scanline;
                       ycount > 0;
                       ycount --, p += ydir, scanptr ++)
                    *p = 255 - *scanptr;
        	}
        	else
        	{
                  for (ycount = img->ysize, p = in + ystart, scanptr = scanline;
                       ycount > 0;
                       ycount --, p += ydir, scanptr ++)
                    *p = *scanptr;
        	}
	      }
            }
            else
              TIFFReadScanline(tif, in, row, 0);

            if (img->colorspace == IMAGE_WHITE)
	    {
	      if (lut)
	        ImageLut(in, img->ysize, lut);

              ImagePutCol(img, x, 0, img->ysize, in);
	    }
            else
            {
	      switch (img->colorspace)
	      {
		case IMAGE_RGB :
		    ImageWhiteToRGB(in, out, img->ysize);
		    break;
		case IMAGE_BLACK :
		    ImageWhiteToBlack(in, out, img->ysize);
		    break;
		case IMAGE_CMY :
		    ImageWhiteToCMY(in, out, img->ysize);
		    break;
		case IMAGE_CMYK :
		    ImageWhiteToCMYK(in, out, img->ysize);
		    break;
	      }

	      if (lut)
	        ImageLut(out, img->ysize * bpp, lut);

              ImagePutCol(img, x, 0, img->ysize, out);
	    }
          }
        }
        break;

    case PHOTOMETRIC_PALETTE :
	if (!TIFFGetField(tif, TIFFTAG_COLORMAP, &redcmap, &greencmap, &bluecmap))
	{
          fputs("ERROR: No colormap tag in the file!\n", stderr);
	  fclose(fp);
	  return (-1);
	}

        num_colors = 1 << bits;

        for (c = 0; c < num_colors; c ++)
	{
	  redcmap[c]   >>= 8;
	  greencmap[c] >>= 8;
	  bluecmap[c]  >>= 8;
	}

        if (orientation < ORIENTATION_LEFTTOP)
        {
         /*
          * Row major order...
          */

          for (y = ystart, ycount = img->ysize, row = 0;
               ycount > 0;
               ycount --, y += ydir, row ++)
          {
            if (bits == 1)
            {
              TIFFReadScanline(tif, scanline, row, 0);
              for (xcount = img->xsize, scanptr = scanline,
	               p = in + xstart * 3, bit = 128;
                   xcount > 0;
                   xcount --, p += pstep)
              {
        	if (*scanptr & bit)
		{
                  p[0] = redcmap[1];
                  p[1] = greencmap[1];
                  p[2] = bluecmap[1];
		}
                else
		{
                  p[0] = redcmap[0];
                  p[1] = greencmap[0];
                  p[2] = bluecmap[0];
		}

        	if (bit > 1)
                  bit >>= 1;
        	else
        	{
                  bit = 128;
                  scanptr ++;
        	}
              }
            }
            else if (bits == 2)
            {
              TIFFReadScanline(tif, scanline, row, 0);
              for (xcount = img->xsize, scanptr = scanline,
	               p = in + xstart * 3, bit = 0xc0;
                   xcount > 0;
                   xcount --, p += pstep)
              {
                pixel = *scanptr & bit;
                while (pixel > 3)
                  pixel >>= 2;

                p[0] = redcmap[pixel];
                p[1] = greencmap[pixel];
                p[2] = bluecmap[pixel];

        	if (bit > 3)
                  bit >>= 2;
        	else
        	{
                  bit = 0xc0;
                  scanptr ++;
        	}
              }
            }
            else if (bits == 4)
            {
              TIFFReadScanline(tif, scanline, row, 0);
              for (xcount = img->xsize, scanptr = scanline,
	               p = in + 3 * xstart, bit = 0xf0;
                   xcount > 0;
                   xcount --, p += pstep)
              {
                if (bit == 0xf0)
                {
		  pixel = (*scanptr & 0xf0) >> 4;
                  p[0]  = redcmap[pixel];
                  p[1]  = greencmap[pixel];
                  p[2]  = bluecmap[pixel];
                  bit   = 0x0f;
                }
                else
        	{
		  pixel = *scanptr++ & 0x0f;
                  p[0]  = redcmap[pixel];
                  p[1]  = greencmap[pixel];
                  p[2]  = bluecmap[pixel];
                  bit   = 0xf0;
        	}
              }
            }
            else
            {
              TIFFReadScanline(tif, scanline, row, 0);

              for (xcount = img->xsize, p = in + 3 * xstart, scanptr = scanline;
                   xcount > 0;
                   xcount --, p += pstep)
              {
	        p[0] = redcmap[*scanptr];
	        p[1] = greencmap[*scanptr];
	        p[2] = bluecmap[*scanptr++];
	      }
            }

            if (img->colorspace == IMAGE_RGB)
	    {
	      if (lut)
	        ImageLut(in, img->xsize * 3, lut);

              ImagePutRow(img, 0, y, img->xsize, in);
	    }
            else
            {
	      switch (img->colorspace)
	      {
		case IMAGE_WHITE :
		    ImageRGBToWhite(in, out, img->xsize);
		    break;
		case IMAGE_BLACK :
		    ImageRGBToBlack(in, out, img->xsize);
		    break;
		case IMAGE_CMY :
		    ImageRGBToCMY(in, out, img->xsize);
		    break;
		case IMAGE_CMYK :
		    ImageRGBToCMYK(in, out, img->xsize);
		    break;
	      }

	      if (lut)
	        ImageLut(out, img->xsize * bpp, lut);

              ImagePutRow(img, 0, y, img->xsize, out);
	    }
          }
        }
        else
        {
         /*
          * Column major order...
          */

          for (x = xstart, xcount = img->xsize, row = 0;
               xcount > 0;
               xcount --, x += xdir, row ++)
          {
            if (bits == 1)
            {
              TIFFReadScanline(tif, scanline, row, 0);
              for (ycount = img->ysize, scanptr = scanline,
	               p = in + 3 * ystart, bit = 128;
                   ycount > 0;
                   ycount --, p += ydir)
              {
        	if (*scanptr & bit)
		{
                  p[0] = redcmap[1];
                  p[1] = greencmap[1];
                  p[2] = bluecmap[1];
		}
                else
		{
                  p[0] = redcmap[0];
                  p[1] = greencmap[0];
                  p[2] = bluecmap[0];
		}

        	if (bit > 1)
                  bit >>= 1;
        	else
        	{
                  bit = 128;
                  scanptr ++;
        	}
              }
            }
            else if (bits == 2)
            {
              TIFFReadScanline(tif, scanline, row, 0);
              for (ycount = img->ysize, scanptr = scanline,
	               p = in + 3 * ystart, bit = 0xc0;
                   ycount > 0;
                   ycount --, p += ydir)
              {
                pixel = *scanptr & 0xc0;
                while (pixel > 3)
                  pixel >>= 2;

                p[0] = redcmap[pixel];
                p[1] = greencmap[pixel];
                p[2] = bluecmap[pixel];

        	if (bit > 3)
                  bit >>= 2;
        	else
        	{
                  bit = 0xc0;
                  scanptr ++;
        	}
              }
            }
            else if (bits == 4)
            {
              TIFFReadScanline(tif, scanline, row, 0);
              for (ycount = img->ysize, scanptr = scanline,
	               p = in + 3 * ystart, bit = 0xf0;
                   ycount > 0;
                   ycount --, p += ydir)
              {
                if (bit == 0xf0)
                {
		  pixel = (*scanptr & 0xf0) >> 4;
                  p[0]  = redcmap[pixel];
                  p[1]  = greencmap[pixel];
                  p[2]  = bluecmap[pixel];
                  bit   = 0x0f;
                }
                else
        	{
		  pixel = *scanptr++ & 0x0f;
                  p[0]  = redcmap[pixel];
                  p[1]  = greencmap[pixel];
                  p[2]  = bluecmap[pixel];
                  bit   = 0xf0;
        	}
              }
            }
            else
            {
              TIFFReadScanline(tif, scanline, row, 0);

              for (ycount = img->ysize, p = in + 3 * ystart, scanptr = scanline;
                   ycount > 0;
                   ycount --, p += ydir)
              {
	        p[0] = redcmap[*scanptr];
	        p[1] = greencmap[*scanptr];
	        p[2] = bluecmap[*scanptr++];
	      }
            }

            if (img->colorspace == IMAGE_RGB)
	    {
	      if (lut)
	        ImageLut(in, img->ysize * 3, lut);

              ImagePutCol(img, x, 0, img->ysize, in);
	    }
            else
            {
	      switch (img->colorspace)
	      {
		case IMAGE_WHITE :
		    ImageRGBToWhite(in, out, img->ysize);
		    break;
		case IMAGE_BLACK :
		    ImageRGBToBlack(in, out, img->ysize);
		    break;
		case IMAGE_CMY :
		    ImageRGBToCMY(in, out, img->ysize);
		    break;
		case IMAGE_CMYK :
		    ImageRGBToCMYK(in, out, img->ysize);
		    break;
	      }

	      if (lut)
	        ImageLut(out, img->ysize * bpp, lut);

              ImagePutCol(img, x, 0, img->ysize, out);
	    }
          }
        }
        break;

    case PHOTOMETRIC_RGB :
        if (orientation < ORIENTATION_LEFTTOP)
        {
         /*
          * Row major order...
          */

          for (y = ystart, ycount = img->ysize, row = 0;
               ycount > 0;
               ycount --, y += ydir, row ++)
          {
            if (bits == 1)
            {
              TIFFReadScanline(tif, scanline, row, 0);
              for (xcount = img->xsize, scanptr = scanline, p = in + xstart * 3, bit = 0xf0;
                   xcount > 0;
                   xcount --, p += pstep)
              {
        	if (*scanptr & bit & 0x88)
                  p[0] = 255;
                else
                  p[0] = 0;

        	if (*scanptr & bit & 0x44)
                  p[1] = 255;
                else
                  p[1] = 0;

        	if (*scanptr & bit & 0x22)
                  p[2] = 255;
                else
                  p[2] = 0;

        	if (bit == 0xf0)
                  bit = 0x0f;
        	else
        	{
                  bit = 0xf0;
                  scanptr ++;
        	}
              }
            }
            else if (bits == 2)
            {
              TIFFReadScanline(tif, scanline, row, 0);
              for (xcount = img->xsize, scanptr = scanline, p = in + xstart * 3;
                   xcount > 0;
                   xcount --, p += pstep, scanptr ++)
              {
                pixel = *scanptr >> 2;
                p[0] = 255 * (pixel & 3) / 3;
                pixel >>= 2;
                p[1] = 255 * (pixel & 3) / 3;
                pixel >>= 2;
                p[2] = 255 * (pixel & 3) / 3;
              }
            }
            else if (bits == 4)
            {
              TIFFReadScanline(tif, scanline, row, 0);
              for (xcount = img->xsize, scanptr = scanline, p = in + xstart * 3;
                   xcount > 0;
                   xcount -= 2, p += 2 * pstep, scanptr += 3)
              {
                pixel = scanptr[0];
                p[1] = 255 * (pixel & 15) / 15;
                pixel >>= 4;
                p[0] = 255 * (pixel & 15) / 15;
                pixel = scanptr[1];
                p[2] = 255 * ((pixel >> 4) & 15) / 15;

                if (xcount > 1)
                {
                  p[pstep + 0] = 255 * (pixel & 15) / 15;
                  pixel = scanptr[2];
                  p[pstep + 2] = 255 * (pixel & 15) / 15;
                  pixel >>= 4;
                  p[pstep + 1] = 255 * (pixel & 15) / 15;
                }
              }
            }
            else if (xdir < 0 || alpha)
            {
              TIFFReadScanline(tif, scanline, row, 0);

              if (alpha)
	      {
        	for (xcount = img->xsize, p = in + xstart * 3, scanptr = scanline;
                     xcount > 0;
                     xcount --, p += pstep, scanptr += 4)
        	{
                  p[0] = (scanptr[0] * scanptr[3] + 255 * (255 - scanptr[3])) / 255;
                  p[1] = (scanptr[1] * scanptr[3] + 255 * (255 - scanptr[3])) / 255;
                  p[2] = (scanptr[2] * scanptr[3] + 255 * (255 - scanptr[3])) / 255;
        	}
              }
	      else
              {
	      	for (xcount = img->xsize, p = in + xstart * 3, scanptr = scanline;
                     xcount > 0;
                     xcount --, p += pstep, scanptr += 3)
        	{
                  p[0] = scanptr[0];
                  p[1] = scanptr[1];
                  p[2] = scanptr[2];
        	}
	      }
            }
            else
              TIFFReadScanline(tif, in, row, 0);

            if ((saturation != 100 || hue != 0) && bpp > 1)
              ImageRGBAdjust(in, img->xsize, saturation, hue);

            if (img->colorspace == IMAGE_RGB)
	    {
	      if (lut)
	        ImageLut(in, img->xsize * 3, lut);

              ImagePutRow(img, 0, y, img->xsize, in);
	    }
            else
            {
	      switch (img->colorspace)
	      {
		case IMAGE_WHITE :
		    ImageRGBToWhite(in, out, img->xsize);
		    break;
		case IMAGE_BLACK :
		    ImageRGBToBlack(in, out, img->xsize);
		    break;
		case IMAGE_CMY :
		    ImageRGBToCMY(in, out, img->xsize);
		    break;
		case IMAGE_CMYK :
		    ImageRGBToCMYK(in, out, img->xsize);
		    break;
	      }

	      if (lut)
	        ImageLut(out, img->xsize * bpp, lut);

              ImagePutRow(img, 0, y, img->xsize, out);
	    }
          }
        }
        else
        {
         /*
          * Column major order...
          */

          for (x = xstart, xcount = img->xsize, row = 0;
               xcount > 0;
               xcount --, x += xdir, row ++)
          {
            if (bits == 1)
            {
              TIFFReadScanline(tif, scanline, row, 0);
              for (ycount = img->ysize, scanptr = scanline, p = in + ystart * 3, bit = 0xf0;
                   ycount > 0;
                   ycount --, p += pstep)
              {
        	if (*scanptr & bit & 0x88)
                  p[0] = 255;
                else
                  p[0] = 0;

        	if (*scanptr & bit & 0x44)
                  p[1] = 255;
                else
                  p[1] = 0;

        	if (*scanptr & bit & 0x22)
                  p[2] = 255;
                else
                  p[2] = 0;

        	if (bit == 0xf0)
                  bit = 0x0f;
        	else
        	{
                  bit = 0xf0;
                  scanptr ++;
        	}
              }
            }
            else if (bits == 2)
            {
              TIFFReadScanline(tif, scanline, row, 0);
              for (ycount = img->ysize, scanptr = scanline, p = in + ystart * 3;
                   ycount > 0;
                   ycount --, p += pstep, scanptr ++)
              {
                pixel = *scanptr >> 2;
                p[0] = 255 * (pixel & 3) / 3;
                pixel >>= 2;
                p[1] = 255 * (pixel & 3) / 3;
                pixel >>= 2;
                p[2] = 255 * (pixel & 3) / 3;
              }
            }
            else if (bits == 4)
            {
              TIFFReadScanline(tif, scanline, row, 0);
              for (ycount = img->ysize, scanptr = scanline, p = in + ystart * 3;
                   ycount > 0;
                   ycount -= 2, p += 2 * pstep, scanptr += 3)
              {
                pixel = scanptr[0];
                p[1] = 255 * (pixel & 15) / 15;
                pixel >>= 4;
                p[0] = 255 * (pixel & 15) / 15;
                pixel = scanptr[1];
                p[2] = 255 * ((pixel >> 4) & 15) / 15;

                if (ycount > 1)
                {
                  p[pstep + 0] = 255 * (pixel & 15) / 15;
                  pixel = scanptr[2];
                  p[pstep + 2] = 255 * (pixel & 15) / 15;
                  pixel >>= 4;
                  p[pstep + 1] = 255 * (pixel & 15) / 15;
                }
              }
            }
            else if (ydir < 0 || alpha)
            {
              TIFFReadScanline(tif, scanline, row, 0);

              if (alpha)
	      {
		for (ycount = img->ysize, p = in + ystart * 3, scanptr = scanline;
                     ycount > 0;
                     ycount --, p += pstep, scanptr += 4)
        	{
                  p[0] = (scanptr[0] * scanptr[3] + 255 * (255 - scanptr[3])) / 255;
                  p[1] = (scanptr[1] * scanptr[3] + 255 * (255 - scanptr[3])) / 255;
                  p[2] = (scanptr[2] * scanptr[3] + 255 * (255 - scanptr[3])) / 255;
        	}
              }
	      else
	      {
		for (ycount = img->ysize, p = in + ystart * 3, scanptr = scanline;
                     ycount > 0;
                     ycount --, p += pstep, scanptr += 3)
        	{
                  p[0] = scanptr[0];
                  p[1] = scanptr[1];
                  p[2] = scanptr[2];
        	}
	      }
            }
            else
              TIFFReadScanline(tif, in, row, 0);

            if ((saturation != 100 || hue != 0) && bpp > 1)
              ImageRGBAdjust(in, img->ysize, saturation, hue);

            if (img->colorspace == IMAGE_RGB)
	    {
	      if (lut)
	        ImageLut(in, img->ysize * 3, lut);

              ImagePutCol(img, x, 0, img->ysize, in);
            }
	    else
            {
	      switch (img->colorspace)
	      {
		case IMAGE_WHITE :
		    ImageRGBToWhite(in, out, img->ysize);
		    break;
		case IMAGE_BLACK :
		    ImageRGBToBlack(in, out, img->ysize);
		    break;
		case IMAGE_CMY :
		    ImageRGBToCMY(in, out, img->ysize);
		    break;
		case IMAGE_CMYK :
		    ImageRGBToCMYK(in, out, img->ysize);
		    break;
	      }

	      if (lut)
	        ImageLut(out, img->ysize * bpp, lut);

              ImagePutCol(img, x, 0, img->ysize, out);
	    }
          }
        }
        break;

    case PHOTOMETRIC_SEPARATED :
        inkset  = INKSET_CMYK;
        numinks = 4;

#ifdef TIFFTAG_NUMBEROFINKS
        if (!TIFFGetField(tif, TIFFTAG_INKSET, &inkset) &&
	    !TIFFGetField(tif, TIFFTAG_NUMBEROFINKS, &numinks))
#else
        if (!TIFFGetField(tif, TIFFTAG_INKSET, &inkset))
#endif /* TIFFTAG_NUMBEROFINKS */
	{
          fputs("WARNING: No inkset or number-of-inks tag in the file!\n", stderr);
	}

	if (inkset == INKSET_CMYK || numinks == 4)
	{
          if (orientation < ORIENTATION_LEFTTOP)
          {
           /*
            * Row major order...
            */

            for (y = ystart, ycount = img->ysize, row = 0;
        	 ycount > 0;
        	 ycount --, y += ydir, row ++)
            {
              if (bits == 1)
              {
        	TIFFReadScanline(tif, scanline, row, 0);
        	for (xcount = img->xsize, scanptr = scanline, p = in + xstart * 3, bit = 0xf0;
                     xcount > 0;
                     xcount --, p += pstep)
        	{
        	  if (*scanptr & bit & 0x11)
        	  {
                    p[0] = 0;
                    p[1] = 0;
                    p[2] = 0;
                  }
                  else
                  {
        	    if (*scanptr & bit & 0x88)
                      p[0] = 0;
                    else
                      p[0] = 255;

        	    if (*scanptr & bit & 0x44)
                      p[1] = 0;
                    else
                      p[1] = 255;

        	    if (*scanptr & bit & 0x22)
                      p[2] = 0;
                    else
                      p[2] = 255;
                  }

        	  if (bit == 0xf0)
                    bit = 0x0f;
        	  else
        	  {
                    bit = 0xf0;
                    scanptr ++;
        	  }
        	}
              }
              else if (bits == 2)
              {
        	TIFFReadScanline(tif, scanline, row, 0);
        	for (xcount = img->xsize, scanptr = scanline, p = in + xstart * 3;
                     xcount > 0;
                     xcount --, p += pstep, scanptr ++)
        	{
        	  pixel = *scanptr;
        	  k     = 255 * (pixel & 3) / 3;
        	  if (k == 255)
        	  {
        	    p[0] = 0;
        	    p[1] = 0;
        	    p[2] = 0;
        	  }
        	  else
        	  {
                    pixel >>= 2;
                    b = 255 - 255 * (pixel & 3) / 3 - k;
                    if (b < 0)
                      p[2] = 0;
                    else if (b < 256)
                      p[2] = b;
                    else
                      p[2] = 255;

                    pixel >>= 2;
                    g = 255 - 255 * (pixel & 3) / 3 - k;
                    if (g < 0)
                      p[1] = 0;
                    else if (g < 256)
                      p[1] = g;
                    else
                      p[1] = 255;

                    pixel >>= 2;
                    r = 255 - 255 * (pixel & 3) / 3 - k;
                    if (r < 0)
                      p[0] = 0;
                    else if (r < 256)
                      p[0] = r;
                    else
                      p[0] = 255;
                  }
        	}
              }
              else if (bits == 4)
              {
        	TIFFReadScanline(tif, scanline, row, 0);
        	for (xcount = img->xsize, scanptr = scanline, p = in + xstart * 3;
                     xcount > 0;
                     xcount --, p += pstep, scanptr += 2)
        	{
        	  pixel = scanptr[1];
        	  k     = 255 * (pixel & 15) / 15;
        	  if (k == 255)
        	  {
        	    p[0] = 0;
        	    p[1] = 0;
        	    p[2] = 0;
        	  }
        	  else
        	  {
                    pixel >>= 4;
                    b = 255 - 255 * (pixel & 15) / 15 - k;
                    if (b < 0)
                      p[2] = 0;
                    else if (b < 256)
                      p[2] = b;
                    else
                      p[2] = 255;

                    pixel = scanptr[0];
                    g = 255 - 255 * (pixel & 15) / 15 - k;
                    if (g < 0)
                      p[1] = 0;
                    else if (g < 256)
                      p[1] = g;
                    else
                      p[1] = 255;

                    pixel >>= 4;
                    r = 255 - 255 * (pixel & 15) / 15 - k;
                    if (r < 0)
                      p[0] = 0;
                    else if (r < 256)
                      p[0] = r;
                    else
                      p[0] = 255;
                  }
        	}
              }
              else if (img->colorspace == IMAGE_CMYK)
	      {
	        TIFFReadScanline(tif, scanline, row, 0);
		ImagePutRow(img, 0, y, img->xsize, scanline);
	      }
	      else
              {
        	TIFFReadScanline(tif, scanline, row, 0);

        	for (xcount = img->xsize, p = in + xstart * 3, scanptr = scanline;
                     xcount > 0;
                     xcount --, p += pstep, scanptr += 4)
        	{
        	  k = scanptr[3];
        	  if (k == 255)
        	  {
        	    p[0] = 0;
        	    p[1] = 0;
        	    p[2] = 0;
        	  }
        	  else
        	  {
                    r = 255 - scanptr[0] - k;
                    if (r < 0)
                      p[0] = 0;
                    else if (r < 256)
                      p[0] = r;
                    else
                      p[0] = 255;

                    g = 255 - scanptr[1] - k;
                    if (g < 0)
                      p[1] = 0;
                    else if (g < 256)
                      p[1] = g;
                    else
                      p[1] = 255;

                    b = 255 - scanptr[2] - k;
                    if (b < 0)
                      p[2] = 0;
                    else if (b < 256)
                      p[2] = b;
                    else
                      p[2] = 255;
        	  }
        	}
              }

              if ((saturation != 100 || hue != 0) && bpp > 1)
        	ImageRGBAdjust(in, img->xsize, saturation, hue);

              if (img->colorspace == IMAGE_RGB)
	      {
	        if (lut)
	          ImageLut(in, img->xsize * 3, lut);

        	ImagePutRow(img, 0, y, img->xsize, in);
	      }
              else if (img->colorspace == IMAGE_WHITE)
              {
		switch (img->colorspace)
		{
		  case IMAGE_WHITE :
		      ImageRGBToWhite(in, out, img->xsize);
		      break;
		  case IMAGE_BLACK :
		      ImageRGBToBlack(in, out, img->xsize);
		      break;
		  case IMAGE_CMY :
		      ImageRGBToCMY(in, out, img->xsize);
		      break;
		  case IMAGE_CMYK :
		      ImageRGBToCMYK(in, out, img->xsize);
		      break;
		}

		if (lut)
	          ImageLut(out, img->xsize * 3, lut);

        	ImagePutRow(img, 0, y, img->xsize, out);
	      }
            }
          }
          else
          {
           /*
            * Column major order...
            */

            for (x = xstart, xcount = img->xsize, row = 0;
        	 xcount > 0;
        	 xcount --, x += xdir, row ++)
            {
              if (bits == 1)
              {
        	TIFFReadScanline(tif, scanline, row, 0);
        	for (ycount = img->ysize, scanptr = scanline, p = in + xstart * 3, bit = 0xf0;
                     ycount > 0;
                     ycount --, p += pstep)
        	{
        	  if (*scanptr & bit & 0x11)
        	  {
                    p[0] = 0;
                    p[1] = 0;
                    p[2] = 0;
                  }
                  else
                  {
        	    if (*scanptr & bit & 0x88)
                      p[0] = 0;
                    else
                      p[0] = 255;

        	    if (*scanptr & bit & 0x44)
                      p[1] = 0;
                    else
                      p[1] = 255;

        	    if (*scanptr & bit & 0x22)
                      p[2] = 0;
                    else
                      p[2] = 255;
                  }

        	  if (bit == 0xf0)
                    bit = 0x0f;
        	  else
        	  {
                    bit = 0xf0;
                    scanptr ++;
        	  }
        	}
              }
              else if (bits == 2)
              {
        	TIFFReadScanline(tif, scanline, row, 0);
        	for (ycount = img->ysize, scanptr = scanline, p = in + xstart * 3;
                     ycount > 0;
                     ycount --, p += pstep, scanptr ++)
        	{
        	  pixel = *scanptr;
        	  k     = 255 * (pixel & 3) / 3;
        	  if (k == 255)
        	  {
        	    p[0] = 0;
        	    p[1] = 0;
        	    p[2] = 0;
        	  }
        	  else
        	  {
                    pixel >>= 2;
                    b = 255 - 255 * (pixel & 3) / 3 - k;
                    if (b < 0)
                      p[2] = 0;
                    else if (b < 256)
                      p[2] = b;
                    else
                      p[2] = 255;

                    pixel >>= 2;
                    g = 255 - 255 * (pixel & 3) / 3 - k;
                    if (g < 0)
                      p[1] = 0;
                    else if (g < 256)
                      p[1] = g;
                    else
                      p[1] = 255;

                    pixel >>= 2;
                    r = 255 - 255 * (pixel & 3) / 3 - k;
                    if (r < 0)
                      p[0] = 0;
                    else if (r < 256)
                      p[0] = r;
                    else
                      p[0] = 255;
                  }
        	}
              }
              else if (bits == 4)
              {
        	TIFFReadScanline(tif, scanline, row, 0);
        	for (ycount = img->ysize, scanptr = scanline, p = in + xstart * 3;
                     ycount > 0;
                     ycount --, p += pstep, scanptr += 2)
        	{
        	  pixel = scanptr[1];
        	  k     = 255 * (pixel & 15) / 15;
        	  if (k == 255)
        	  {
        	    p[0] = 0;
        	    p[1] = 0;
        	    p[2] = 0;
        	  }
        	  else
        	  {
                    pixel >>= 4;
                    b = 255 - 255 * (pixel & 15) / 15 - k;
                    if (b < 0)
                      p[2] = 0;
                    else if (b < 256)
                      p[2] = b;
                    else
                      p[2] = 255;

                    pixel = scanptr[0];
                    g = 255 - 255 * (pixel & 15) / 15 - k;
                    if (g < 0)
                      p[1] = 0;
                    else if (g < 256)
                      p[1] = g;
                    else
                      p[1] = 255;

                    pixel >>= 4;
                    r = 255 - 255 * (pixel & 15) / 15 - k;
                    if (r < 0)
                      p[0] = 0;
                    else if (r < 256)
                      p[0] = r;
                    else
                      p[0] = 255;
                  }
        	}
              }
              else if (img->colorspace == IMAGE_CMYK)
	      {
	        TIFFReadScanline(tif, scanline, row, 0);
		ImagePutCol(img, x, 0, img->ysize, scanline);
	      }
              else
              {
        	TIFFReadScanline(tif, scanline, row, 0);

        	for (ycount = img->ysize, p = in + xstart * 3, scanptr = scanline;
                     ycount > 0;
                     ycount --, p += pstep, scanptr += 4)
        	{
        	  k = scanptr[3];
        	  if (k == 255)
        	  {
        	    p[0] = 0;
        	    p[1] = 0;
        	    p[2] = 0;
        	  }
        	  else
        	  {
                    r = 255 - scanptr[0] - k;
                    if (r < 0)
                      p[0] = 0;
                    else if (r < 256)
                      p[0] = r;
                    else
                      p[0] = 255;

                    g = 255 - scanptr[1] - k;
                    if (g < 0)
                      p[1] = 0;
                    else if (g < 256)
                      p[1] = g;
                    else
                      p[1] = 255;

                    b = 255 - scanptr[2] - k;
                    if (b < 0)
                      p[2] = 0;
                    else if (b < 256)
                      p[2] = b;
                    else
                      p[2] = 255;
        	  }
        	}
              }

              if ((saturation != 100 || hue != 0) && bpp > 1)
        	ImageRGBAdjust(in, img->ysize, saturation, hue);

              if (img->colorspace == IMAGE_RGB)
	      {
		if (lut)
	          ImageLut(in, img->ysize * 3, lut);

        	ImagePutCol(img, x, 0, img->ysize, in);
              }
              else if (img->colorspace == IMAGE_WHITE)
              {
		switch (img->colorspace)
		{
		  case IMAGE_WHITE :
		      ImageRGBToWhite(in, out, img->ysize);
		      break;
		  case IMAGE_BLACK :
		      ImageRGBToBlack(in, out, img->ysize);
		      break;
		  case IMAGE_CMY :
		      ImageRGBToCMY(in, out, img->ysize);
		      break;
		  case IMAGE_CMYK :
		      ImageRGBToCMYK(in, out, img->ysize);
		      break;
		}

		if (lut)
	          ImageLut(out, img->ysize * bpp, lut);

        	ImagePutCol(img, x, 0, img->ysize, out);
	      }
            }
          }

          break;
	}

    default :
	_TIFFfree(scanline);
	free(in);
	free(out);

	TIFFClose(tif);
	fputs("ERROR: Unknown TIFF photometric value!\n", stderr);
	return (-1);
  }

 /*
  * Free temporary buffers, close the TIFF file, and return.
  */

  _TIFFfree(scanline);
  free(in);
  free(out);

  TIFFClose(tif);
  return (0);
}


#endif /* HAVE_LIBTIFF */


/*
 * End of "$Id: image-tiff.c,v 1.1.1.1.12.1 2009/02/03 08:27:05 wiley Exp $".
 */
