﻿/**
 * File:   list_view.h
 * Author: AWTK Develop Team
 * Brief:  list_view
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
 * 2018-07-04 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/utils.h"
#include "base/layout.h"
#include "base/list_view.h"
#include "base/scroll_bar.h"
#include "base/scroll_view.h"

static ret_t list_view_on_add_child(widget_t* widget, widget_t* child);

static ret_t list_view_on_paint_self(widget_t* widget, canvas_t* c) {
  return widget_paint_helper(widget, c, NULL, NULL);
}

static ret_t list_view_get_prop(widget_t* widget, const char* name, value_t* v) {
  list_view_t* list_view = LIST_VIEW(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_ITEM_HEIGHT)) {
    value_set_int(v, list_view->item_height);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_DEFAULT_ITEM_HEIGHT)) {
    value_set_int(v, list_view->default_item_height);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t list_view_set_prop(widget_t* widget, const char* name, const value_t* v) {
  list_view_t* list_view = LIST_VIEW(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_ITEM_HEIGHT)) {
    list_view->item_height = value_int(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_DEFAULT_ITEM_HEIGHT)) {
    list_view->default_item_height = value_int(v);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static const widget_vtable_t s_list_view_vtable = {.type_name = WIDGET_TYPE_LIST_VIEW,
                                                   .set_prop = list_view_set_prop,
                                                   .get_prop = list_view_get_prop,
                                                   .on_add_child = list_view_on_add_child,
                                                   .on_paint_self = list_view_on_paint_self};

static int32_t scroll_bar_to_scroll_view(list_view_t* list_view, int32_t v) {
  scroll_view_t* scroll_view = SCROLL_VIEW(list_view->scroll_view);
  scroll_bar_t* scroll_bar = SCROLL_BAR(list_view->scroll_bar);
  int32_t range = scroll_bar->virtual_size;
  float_t percent = range > 0 ? (float_t)v / (float_t)(range) : 0;

  return percent * (scroll_view->virtual_h - list_view->scroll_view->h);
}

static ret_t list_view_on_scroll_bar_value_changed(void* ctx, event_t* e) {
  list_view_t* list_view = LIST_VIEW(ctx);
  scroll_bar_t* scroll_bar = SCROLL_BAR(list_view->scroll_bar);
  int32_t offset = scroll_bar_to_scroll_view(list_view, scroll_bar->value);

  scroll_view_set_offset(list_view->scroll_view, 0, offset);

  return RET_OK;
}

static int32_t scroll_view_to_scroll_bar(list_view_t* list_view, int32_t v) {
  scroll_view_t* scroll_view = SCROLL_VIEW(list_view->scroll_view);
  scroll_bar_t* scroll_bar = SCROLL_BAR(list_view->scroll_bar);
  int32_t range = scroll_view->virtual_h - list_view->scroll_view->h;
  float_t percent = range > 0 ? (float_t)v / (float_t)range : 0;

  return percent * scroll_bar->virtual_size;
}

static ret_t list_view_on_scroll_view_scroll(widget_t* widget, int32_t xoffset, int32_t yoffset) {
  list_view_t* list_view = LIST_VIEW(widget->parent);
  int32_t value = scroll_view_to_scroll_bar(list_view, yoffset);

  scroll_bar_set_value_only(list_view->scroll_bar, value);
  widget_set_opacity(list_view->scroll_bar, 0xff);
  widget_set_visible(list_view->scroll_bar, TRUE, FALSE);

  return RET_OK;
}

static ret_t list_view_on_scroll_view_scroll_to(widget_t* widget, int32_t xoffset_end,
                                                int32_t yoffset_end, int32_t duration) {
  list_view_t* list_view = LIST_VIEW(widget->parent);
  int32_t value = scroll_view_to_scroll_bar(list_view, yoffset_end);

  emitter_disable(list_view->scroll_bar->emitter);
  scroll_bar_scroll_to(list_view->scroll_bar, value, duration);
  emitter_enable(list_view->scroll_bar->emitter);

  (void)xoffset_end;

  return RET_OK;
}

static ret_t list_view_on_scroll_view_layout_children(widget_t* widget) {
  int32_t virtual_h = widget->h;
  list_view_t* list_view = LIST_VIEW(widget->parent);
  int32_t item_height = list_view->item_height;
  int32_t default_item_height = list_view->default_item_height;

  if (widget->children != NULL) {
    int32_t i = 0;
    int32_t n = 0;
    int32_t x = 0;
    int32_t y = 0;
    int32_t w = widget->w;
    int32_t h = item_height;
    widget_t** children = (widget_t**)(widget->children->elms);
    for (i = 0, n = widget->children->size; i < n; i++) {
      widget_t* iter = children[i];

      if (item_height <= 0) {
        h = iter->h;
      }

      if (h <= 0) {
        h = default_item_height;
      }

      widget_move_resize(iter, x, y, w, h);
      widget_layout(iter);

      y = iter->y + iter->h;
      if (y > virtual_h) {
        virtual_h = y;
      }
    }
  }

  scroll_view_set_virtual_h(list_view->scroll_view, virtual_h);
  item_height = tk_max(item_height, default_item_height);
  scroll_bar_set_params(list_view->scroll_bar, virtual_h, item_height);

  scroll_view_set_xslidable(list_view->scroll_view, FALSE);
  if (scroll_bar_is_mobile(list_view->scroll_bar)) {
    scroll_view_set_yslidable(list_view->scroll_view, TRUE);
    widget_set_visible(list_view->scroll_bar, FALSE, FALSE);
  }

  return RET_OK;
}

static ret_t list_view_on_add_child(widget_t* widget, widget_t* child) {
  list_view_t* list_view = LIST_VIEW(widget);

  if (child->type == WIDGET_SCROLL_VIEW) {
    scroll_view_t* scroll_view = SCROLL_VIEW(child);

    list_view->scroll_view = child;
    scroll_view->on_scroll = list_view_on_scroll_view_scroll;
    scroll_view->on_scroll_to = list_view_on_scroll_view_scroll_to;
    scroll_view->on_layout_children = list_view_on_scroll_view_layout_children;
  } else if (child->type == WIDGET_SCROLL_BAR) {
    list_view->scroll_bar = child;
    widget_on(child, EVT_VALUE_CHANGED, list_view_on_scroll_bar_value_changed, widget);
  }

  /*return RET_FAIL let widget_add_child do its job*/
  return RET_FAIL;
}

widget_t* list_view_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = NULL;
  list_view_t* list_view = TKMEM_ZALLOC(list_view_t);
  return_value_if_fail(list_view != NULL, NULL);

  widget = WIDGET(list_view);
  widget->vt = &s_list_view_vtable;
  widget_init(widget, parent, WIDGET_LIST_VIEW);
  widget_move_resize(widget, x, y, w, h);

  widget_set_state(widget, WIDGET_STATE_NORMAL);

  return widget;
}

ret_t list_view_set_item_height(widget_t* widget, int32_t item_height) {
  list_view_t* list_view = LIST_VIEW(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  list_view->item_height = item_height;

  return RET_OK;
}

ret_t list_view_set_default_item_height(widget_t* widget, int32_t default_item_height) {
  list_view_t* list_view = LIST_VIEW(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  list_view->default_item_height = default_item_height;

  return RET_OK;
}
