﻿#include "base/dialog.h"
#include "base/canvas.h"
#include "base/widget.h"
#include "font_dummy.h"
#include "lcd_log.h"
#include "gtest/gtest.h"
#include <stdlib.h>

TEST(Dialog, basic) {
  value_t v1;
  value_t v2;
  widget_t* b = dialog_create(NULL, 10, 20, 30, 40);

  value_set_wstr(&v1, L"dialog");
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_TEXT, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_TEXT, &v2), RET_OK);
  ASSERT_EQ(wcscmp(v1.value.wstr, v2.value.wstr), 0);

  value_set_str(&v1, "center_scale");
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_ANIM_HINT, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_ANIM_HINT, &v2), RET_OK);
  ASSERT_EQ(strcmp(value_str(&v2), "center_scale"), 0);

  value_set_int(&v1, 5);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_MARGIN, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MARGIN, &v2), RET_OK);
  ASSERT_EQ(value_int(&v2), 5);

  widget_destroy(b);
}
