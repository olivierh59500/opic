/* init_helper.h ---
 *
 * Filename: init_helper.h
 * Description:
 * Author: Felix Chern
 * Maintainer:
 * Copyright: (c) 2016 Felix Chern
 * Created: Sun Mar  5 16:41:20 2017 (-0800)
 * Version:
 * Package-Requires: ()
 * Last-Updated:
 *           By:
 *     Update #: 0
 * URL:
 * Doc URL:
 * Keywords:
 * Compatibility:
 *
 */

/* Commentary:
 *
 *
 *
 */

/* Change Log:
 *
 *
 */

/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* Code: */

#ifndef OPIC_MALLOC_INIT_HELPER_H
#define OPIC_MALLOC_INIT_HELPER_H 1

#include "objdef.h"
#include "inline_aux.h"

OP_BEGIN_DECLS

bool OPHeapNew(OPHeap** heap_ref);
void OPHeapDestroy(OPHeap* heap);
void HPageEmptiedBMaps(HugePage* hpage, uint64_t* occupy_bmap,
                       uint64_t* header_bmap);
void USpanEmptiedBMap(UnarySpan* uspan, uint64_t* bmap);

OP_END_DECLS

#endif

/* init_helper.h ends here */
