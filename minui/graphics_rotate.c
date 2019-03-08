/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "minui.h"
#include "graphics.h"

GRSurface _rotated_surface;

gr_surface rotated_surface = NULL;

/* ------------------------------------------------------------------------ */

void gr_rotate_init(gr_surface gr_draw)
{
    rotated_surface = &_rotated_surface;
    memcpy(rotated_surface, gr_draw, sizeof(GRSurface));

    rotated_surface->width = gr_draw->height;
    rotated_surface->height = gr_draw->width;
    rotated_surface->row_bytes = rotated_surface->width * rotated_surface->pixel_bytes;

    rotated_surface->data = (unsigned char *) malloc(rotated_surface->height * rotated_surface->row_bytes);
    if (rotated_surface->data == NULL) {
        perror("failed to allocate in-memory rotated surface");
        rotated_surface = NULL;
        return;
    }

    memset(rotated_surface->data, 0, rotated_surface->height * rotated_surface->row_bytes);

    printf("rotated to: %d x %d\n", rotated_surface->width, rotated_surface->height);
}

/* ------------------------------------------------------------------------ */

void gr_rotate_exit(void)
{
    if (rotated_surface) {
        if (rotated_surface->data)
            free(rotated_surface->data);
    }
    rotated_surface = NULL;
}

/* ------------------------------------------------------------------------ */

gr_surface gr_rotate_surface_get(gr_surface gr_draw)
{
    if (rotated_surface == NULL)
        gr_rotate_init(gr_draw);
    return rotated_surface;
}

/* ------------------------------------------------------------------------ */

void gr_rotate_update_surface(gr_surface from, gr_surface to)
{
    int to_row;
    int to_row_pos;
    int from_column;
    unsigned int *pixel_from;
    unsigned int *pixel_to;

    for (to_row = 0, from_column = from->width - 1; to_row < to->height; to_row++, from_column--) {
        for (to_row_pos = 0; to_row_pos < to->width; to_row_pos++) {
            pixel_from = (unsigned int *) (from->data + from->row_bytes * to_row_pos);
            pixel_to = (unsigned int *) (to->data + to->row_bytes * to_row);
            *(pixel_to + to_row_pos) = *(pixel_from + from_column);
        }
    }
}


