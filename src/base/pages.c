﻿/**
 * File:   pages.c
 * Author: AWTK Develop Team
 * Brief:  pages
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-06-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/utils.h"
#include "base/pages.h"
#include "base/widget_vtable.h"
#include "base/image_manager.h"

ret_t pages_set_active(widget_t* widget, uint32_t index) {
  pages_t* pages = PAGES(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (pages->active != index) {
    pages->active = index;
    widget_invalidate(widget, NULL);
  }

  return RET_OK;
}

ret_t pages_set_active_by_name(widget_t* widget, const char* name) {
  uint32_t i = 0;
  uint32_t nr = 0;
  widget_t** children = NULL;
  return_value_if_fail(widget != NULL && name != NULL, RET_BAD_PARAMS);

  if (widget->children && widget->children->elms) {
    nr = widget->children->size;
    children = (widget_t**)(widget->children->elms);

    for (i = 0; i < nr; i++) {
      widget_t* iter = children[i];
      if (iter->name.str && tk_str_eq(iter->name.str, name)) {
        return pages_set_active(widget, i);
      }
    }
  }

  return RET_NOT_FOUND;
}

static widget_t* pages_find_target(widget_t* widget, xy_t x, xy_t y) {
  pages_t* pages = PAGES(widget);
  return_value_if_fail(pages != NULL, NULL);

  return widget_get_child(widget, pages->active);
}

static ret_t pages_on_paint_children(widget_t* widget, canvas_t* c) {
  pages_t* pages = PAGES(widget);
  widget_t* active = widget_get_child(widget, pages->active);

  return_value_if_fail(active != NULL, RET_BAD_PARAMS);

  return widget_paint(active, c);
}

static ret_t pages_get_prop(widget_t* widget, const char* name, value_t* v) {
  pages_t* pages = PAGES(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE) || tk_str_eq(name, WIDGET_PROP_ACTIVE)) {
    value_set_uint32(v, pages->active);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t pages_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE) || tk_str_eq(name, WIDGET_PROP_ACTIVE)) {
    return pages_set_active(widget, value_int(v));
  }

  return RET_NOT_FOUND;
}

static const widget_vtable_t s_pages_vtable = {
    .type_name = WIDGET_TYPE_PAGES,
    .on_paint_self = widget_on_paint_background_null,
    .find_target = pages_find_target,
    .on_paint_children = pages_on_paint_children,
    .on_paint_background = widget_on_paint_background_null,
    .on_paint_done = widget_on_paint_done_null,
    .get_prop = pages_get_prop,
    .set_prop = pages_set_prop};

widget_t* pages_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = NULL;
  pages_t* pages = TKMEM_ZALLOC(pages_t);
  return_value_if_fail(pages != NULL, NULL);

  widget = WIDGET(pages);
  widget->vt = &s_pages_vtable;
  widget_init(widget, parent, WIDGET_PAGES);
  widget_move_resize(widget, x, y, w, h);

  return widget;
}
