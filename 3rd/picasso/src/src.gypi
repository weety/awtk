# Picasso - a vector graphics library
# 
# Copyright (C) 2013 Zhang Ji Peng
# Contact: onecoolx@gmail.com

{
  'targets': [
    {
      'target_name': 'picasso2_sw',
      'type': 'shared_library',
      'dependencies': [
      ],
      'defines':[
        'EXPORT',
      ],
      'include_dirs': [
        '../include',
        '../build',
        './',
        './include',
        './simd',
        './gfx',
      ],
      'sources': [
        '../build/pconfig.h',
        '../include/picasso.h',
        'core/curve.cpp',
        'core/device.cpp',
        'core/graphic_path.cpp',
        'core/clipper.cpp',
        'gfx/gfx_blur.cpp',
        'gfx/gfx_blur.h',
        'gfx/gfx_device.cpp',
        'gfx/gfx_device.h',
        'gfx/gfx_font_adapter.h',
        'gfx/gfx_font_adapter_freetype2.cpp',
        'gfx/gfx_font_adapter_win32.cpp',
        'gfx/gfx_font_load_freetype2.cpp',
        'gfx/gfx_gamma_function.h',
        'gfx/gfx_gradient_adapter.cpp',
        'gfx/gfx_gradient_adapter.h',
        'gfx/gfx_mask_layer.h',
        'gfx/gfx_math.h',
        'gfx/gfx_painter.h',
        'gfx/gfx_painter_helper.h',
        'gfx/gfx_pixfmt_rgb.h',
        'gfx/gfx_pixfmt_rgb16.h',
        'gfx/gfx_pixfmt_rgba.h',
        'gfx/gfx_raster_adapter.cpp',
        'gfx/gfx_raster_adapter.h',
        'gfx/gfx_rasterizer_cell.h',
        'gfx/gfx_rasterizer_scanline.h',
        'gfx/gfx_renderer.h',
        'gfx/gfx_rendering_buffer.cpp',
        'gfx/gfx_rendering_buffer.h',
        'gfx/gfx_scanline.h',
        'gfx/gfx_scanline_renderer.h',
        'gfx/gfx_scanline_storage.h',
        'gfx/gfx_span_generator.h',
        'gfx/gfx_span_image_filters.h',
        'gfx/gfx_sqrt_tables.cpp',
        'gfx/gfx_trans_affine.h',
        'gfx/gfx_image_accessors.h',
        'gfx/gfx_image_filters.cpp',
        'gfx/gfx_image_filters.h',
        'gfx/gfx_line_generator.h',
        'gfx/gfx_pixfmt_rgba.h',
        'gfx/gfx_pixfmt_wrapper.h',
        'include/color_type.h',
        'include/common.h',
        'include/convert.h',
        'include/curve.h',
        'include/clipper.h',
        'include/data_vector.h',
        'include/device.h',
        'include/fastcopy.h',
        'include/geometry.h',
        'include/graphic_base.h',
        'include/graphic_helper.h',
        'include/graphic_path.h',
        'include/interfaces.h',
        'include/math_type.h',
        'include/memory_manager.h',
        'include/platform.h',
        'include/refptr.h',
        'include/shared.h',
        'include/vertex.h',
        'include/vertex_dist.h',
        'simd/fastcopy_sse.h',
        'picasso_api.cpp',
        'picasso_canvas.cpp',
        'picasso_font_api.cpp',
        'picasso_font.cpp',
        'picasso_font.h',
        'picasso_font_cache.h',
        'picasso_global.h',
        'picasso_gradient_api.cpp',
        'picasso_gradient.cpp',
        'picasso_gradient.h',
        'picasso_image.cpp',
        'picasso_mask_api.cpp',
        'picasso_mask.cpp',
        'picasso_mask.h',
        'picasso_matrix_api.cpp',
        'picasso_matrix.cpp',
        'picasso_matrix.h',
        'picasso_objects.h',
        'picasso_painter.cpp',
        'picasso_painter.h',
        'picasso_path.cpp',
        'picasso_pattern.cpp',
        'picasso_private.h',
        'picasso_raster_adapter.cpp',
        'picasso_raster_adapter.h',
        'picasso_rendering_buffer.cpp',
        'picasso_rendering_buffer.h',
      ],
      'conditions': [
        ['OS=="win"', {
          'sources': [
            'picasso.rc',
            'picasso.def',
            'resource.h',
          ],
        }],
        ['OS=="linux"', {
          'libraries': [
            '-lfreetype',
            '-lfontconfig',
          ],
        }],
      ],
      'includes':[
        '../build/configs.gypi',
        '../build/defines.gypi',
      ],
    },
  ],
}
