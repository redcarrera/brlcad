/*                           P P M . C
 * BRL-CAD
 *
 * Copyright (c) 2013-2025 United States Government as represented by
 * the U.S. Army Research Laboratory.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this file; see the file named COPYING for more
 * information.
 */
/** @file ppm.c
 *
 * This file contains reading and writing routines for ppm format.
 *
 */

#include "common.h"
#include <string.h>
#include "ppm.h"

#include "bu/log.h"
#include "bu/malloc.h"
#include "vmath.h"
#include "icv_private.h"

int
ppm_write(icv_image_t *bif, FILE *fp)
{
    if (UNLIKELY(!bif))
	return BRLCAD_ERROR;
    if (UNLIKELY(!fp))
	return BRLCAD_ERROR;

    if (bif->color_space == ICV_COLOR_SPACE_GRAY) {
	icv_gray2rgb(bif);
    } else if (bif->color_space != ICV_COLOR_SPACE_RGB) {
	bu_log("ppm_write : Color Space conflict");
	return BRLCAD_ERROR;
    }

    int rows = (int)bif->height;
    int cols = (int)bif->width;

    ppm_writeppminit(fp, cols, rows, (pixval)255, 0 );

    pixel *pixelrow = ppm_allocrow(cols);

    for (int p = 0; p < rows; p++) {
	for (int q = 0; q < cols; q++) {
	    int offset = ((rows - 1) * cols * 3) - (p * cols * 3);
	    pixelrow[q].r = lrint(bif->data[offset + q*3+0]*255.0);
	    pixelrow[q].g = lrint(bif->data[offset + q*3+1]*255.0);
	    pixelrow[q].b = lrint(bif->data[offset + q*3+2]*255.0);
	}
	ppm_writeppmrow(fp, pixelrow, cols, (pixval) 255, 0 );
    }

    ppm_freerow((void *)pixelrow);

    return 0;
}

icv_image_t*
ppm_read(FILE *fp)
{
    if (UNLIKELY(!fp))
	return NULL;

    int rows, cols, format;
    pixval maxval;
    ppm_readppminit(fp, &cols, &rows, &maxval, &format);

    /* For now, only handle PPM - should handle all variations */
    if (PPM_FORMAT_TYPE(format) != PPM_TYPE)
       	return NULL;

    pixel **pixels = ppm_allocarray(cols, rows);

    for (int row = 0; row < rows; row++)
	ppm_readppmrow(fp, pixels[row], cols, maxval, format);

    icv_image_t *bif;
    BU_ALLOC(bif, struct icv_image);
    ICV_IMAGE_INIT(bif);

    bif->width = (size_t)cols;
    bif->height = (size_t)rows;

    double *data = (double *)bu_malloc(bif->width * bif->height * 3 * sizeof(double), "image data");
    for (int p = 0; p < rows ; p++) {
	pixel *r = pixels[p];
	for (int q = 0; q < cols; q++) {
	    int offset = ((rows - 1) * cols * 3) - (p * cols * 3);
	    data[offset + q*3+0] = (double)r[q].r/(double)255.0;
	    data[offset + q*3+1] = (double)r[q].g/(double)255.0;
	    data[offset + q*3+2] = (double)r[q].b/(double)255.0;
	}
    }

    bif->data = data;
    ppm_freearray(pixels, rows);
    bif->magic = ICV_IMAGE_MAGIC;
    bif->channels = 3;
    bif->color_space = ICV_COLOR_SPACE_RGB;
    return bif;
}

/*
 * Local Variables:
 * tab-width: 8
 * mode: C
 * indent-tabs-mode: t
 * c-file-style: "stroustrup"
 * End:
 * ex: shiftwidth=4 tabstop=8
 */
