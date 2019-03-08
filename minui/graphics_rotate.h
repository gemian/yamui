/*
 * Copyright (C) 2014 The Android Open Source Project
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

#ifndef _GRAPHICS_ROTATE_H_
#define _GRAPHICS_ROTATE_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "minui.h"

void gr_rotate_exit(void);
void gr_rotate_init(gr_surface gr_draw);
void gr_rotate_update_surface(gr_surface from, gr_surface to);
gr_surface gr_rotate_surface_get(gr_surface gr_draw);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _GRAPHICS_ROTATE_H_ */
