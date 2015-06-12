/*
 * Copyright (c) 2015, NVIDIA CORPORATION.  All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License, version 2.1, as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <win_x11.h>

#if defined(WIN_X11_USE_GL)
#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#ifndef WIN_WIDTH
#define WIN_WIDTH  1920
#endif
#ifndef WIN_HEIGHT
#define WIN_HEIGHT 1080
#endif
#ifndef WIN_X_OFFSET
#define WIN_X_OFFSET 0
#endif
#ifndef WIN_Y_OFFSET
#define WIN_Y_OFFSET 0
#endif

Display *     x_display;
int           x_screen;
int           x_depth[WIN_X11_MAX_WIN_IDS];
Visual *      x_visual[WIN_X11_MAX_WIN_IDS];
Window        x_window[WIN_X11_MAX_WIN_IDS];
Pixmap        x_pixmap[WIN_X11_MAX_WIN_IDS];
GC            x_gc[WIN_X11_MAX_WIN_IDS];
int           x_window_width[WIN_X11_MAX_WIN_IDS];
int           x_window_height[WIN_X11_MAX_WIN_IDS];

#if defined(WIN_X11_USE_GL)
XVisualInfo * x_visual_info;
GLXContext    gl_context;
int           gl_double_buffered;
#endif

VdpDevice           vdp_device;
VdpGetProcAddress * vdp_get_proc_address;

VdpPresentationQueueTarget vdp_flip_target[WIN_X11_MAX_WIN_IDS];
VdpPresentationQueue       vdp_flip_queue[WIN_X11_MAX_WIN_IDS];

VdpGetErrorString * vdp_get_error_string;
VdpGetApiVersion * vdp_get_api_version;
VdpGetInformationString * vdp_get_information_string;
VdpDeviceDestroy * vdp_device_destroy;
VdpGenerateCSCMatrix * vdp_generate_csc_matrix;
VdpVideoSurfaceQueryCapabilities * vdp_video_surface_query_capabilities;
VdpVideoSurfaceQueryGetPutBitsYCbCrCapabilities * vdp_video_surface_query_get_put_bits_y_cb_cr_capabilities;
VdpVideoSurfaceCreate * vdp_video_surface_create;
VdpVideoSurfaceDestroy * vdp_video_surface_destroy;
VdpVideoSurfaceGetParameters * vdp_video_surface_get_parameters;
VdpVideoSurfaceGetBitsYCbCr * vdp_video_surface_get_bits_y_cb_cr;
VdpVideoSurfacePutBitsYCbCr * vdp_video_surface_put_bits_y_cb_cr;
VdpOutputSurfaceQueryCapabilities * vdp_output_surface_query_capabilities;
VdpOutputSurfaceQueryGetPutBitsNativeCapabilities * vdp_output_surface_query_get_put_bits_native_capabilities;
VdpOutputSurfaceQueryPutBitsIndexedCapabilities * vdp_output_surface_query_put_bits_indexed_capabilities;
VdpOutputSurfaceQueryPutBitsYCbCrCapabilities * vdp_output_surface_query_put_bits_y_cb_cr_capabilities;
VdpOutputSurfaceCreate * vdp_output_surface_create;
VdpOutputSurfaceDestroy * vdp_output_surface_destroy;
VdpOutputSurfaceGetParameters * vdp_output_surface_get_parameters;
VdpOutputSurfaceGetBitsNative * vdp_output_surface_get_bits_native;
VdpOutputSurfacePutBitsNative * vdp_output_surface_put_bits_native;
VdpOutputSurfacePutBitsIndexed * vdp_output_surface_put_bits_indexed;
VdpOutputSurfacePutBitsYCbCr * vdp_output_surface_put_bits_y_cb_cr;
VdpBitmapSurfaceQueryCapabilities * vdp_bitmap_surface_query_capabilities;
VdpBitmapSurfaceCreate * vdp_bitmap_surface_create;
VdpBitmapSurfaceDestroy * vdp_bitmap_surface_destroy;
VdpBitmapSurfaceGetParameters * vdp_bitmap_surface_get_parameters;
VdpBitmapSurfacePutBitsNative * vdp_bitmap_surface_put_bits_native;
VdpOutputSurfaceRenderOutputSurface * vdp_output_surface_render_output_surface;
VdpOutputSurfaceRenderBitmapSurface * vdp_output_surface_render_bitmap_surface;
VdpDecoderQueryCapabilities * vdp_decoder_query_capabilities;
VdpDecoderCreate * vdp_decoder_create;
VdpDecoderDestroy * vdp_decoder_destroy;
VdpDecoderGetParameters * vdp_decoder_get_parameters;
VdpDecoderRender * vdp_decoder_render;
VdpVideoMixerQueryFeatureSupport * vdp_video_mixer_query_feature_support;
VdpVideoMixerQueryParameterSupport * vdp_video_mixer_query_parameter_support;
VdpVideoMixerQueryAttributeSupport * vdp_video_mixer_query_attribute_support;
VdpVideoMixerQueryParameterValueRange * vdp_video_mixer_query_parameter_value_range;
VdpVideoMixerQueryAttributeValueRange * vdp_video_mixer_query_attribute_value_range;
VdpVideoMixerCreate * vdp_video_mixer_create;
VdpVideoMixerSetFeatureEnables * vdp_video_mixer_set_feature_enables;
VdpVideoMixerSetAttributeValues * vdp_video_mixer_set_attribute_values;
VdpVideoMixerGetFeatureSupport * vdp_video_mixer_get_feature_support;
VdpVideoMixerGetFeatureEnables * vdp_video_mixer_get_feature_enables;
VdpVideoMixerGetParameterValues * vdp_video_mixer_get_parameter_values;
VdpVideoMixerGetAttributeValues * vdp_video_mixer_get_attribute_values;
VdpVideoMixerDestroy * vdp_video_mixer_destroy;
VdpVideoMixerRender * vdp_video_mixer_render;
VdpPresentationQueueTargetDestroy * vdp_presentation_queue_target_destroy;
VdpPresentationQueueCreate * vdp_presentation_queue_create;
VdpPresentationQueueDestroy * vdp_presentation_queue_destroy;
VdpPresentationQueueSetBackgroundColor * vdp_presentation_queue_set_background_color;
VdpPresentationQueueGetBackgroundColor * vdp_presentation_queue_get_background_color;
VdpPresentationQueueGetTime * vdp_presentation_queue_get_time;
VdpPresentationQueueDisplay * vdp_presentation_queue_display;
VdpPresentationQueueBlockUntilSurfaceIdle * vdp_presentation_queue_block_until_surface_idle;
VdpPresentationQueueQuerySurfaceStatus * vdp_presentation_queue_query_surface_status;
VdpPreemptionCallbackRegister * vdp_preemption_callback_register;
VdpPresentationQueueTargetCreateX11 * vdp_presentation_queue_target_create_x11;

#if defined(WIN_X11_USE_GL)

/* attributes for a single buffered visual in RGBA format with at least
 * 8 bits per color */
static int attrListSgl[] = {
    GLX_RGBA,
    GLX_RED_SIZE, 8,
    GLX_GREEN_SIZE, 8,
    GLX_BLUE_SIZE, 8,
    None
};

/* attributes for a double buffered visual in RGBA format with at least
 * 8 bits per color */
static int attrListDbl[] = {
    GLX_RGBA,
    GLX_DOUBLEBUFFER,
    GLX_RED_SIZE, 8,
    GLX_GREEN_SIZE, 8,
    GLX_BLUE_SIZE, 8,
    None
};

static void win_x11_get_visual(int win_id)
{
    x_visual_info[win_id] = glXChooseVisual(x_display, x_screen, attrListDbl);
    if (x_visual_info[win_id]) {
        x_depth[win_id] = x_visual_info[win_id]->depth;
        x_visual[win_id] = x_visual_info[win_id]->visual;
        gl_double_buffered = 1;
        return;
    }

    x_visual_info[win_id] = glXChooseVisual(x_display, x_screen, attrListSgl);
    if (x_visual_info[win_id]) {
        x_depth[win_id] = x_visual_info[win_id]->depth;
        x_visual[win_id] = x_visual_info[win_id]->visual;
        gl_double_buffered = 0;
        return;
    }

    fprintf(stderr, "GL visual lookup failed; bailing\n");
    exit(1);
}

#else

static void win_x11_get_visual(int win_id)
{
    x_depth[win_id] = CopyFromParent;
    x_visual[win_id] = CopyFromParent;
}

#endif

void win_x11_init_x11(void)
{
    x_display = XOpenDisplay(NULL);
    x_screen = DefaultScreen(x_display);
}

void win_x11_fini_x11(void)
{
    XCloseDisplay(x_display);
}

void win_x11_init_window(int win_id)
{
    unsigned long windowMask;
    XSetWindowAttributes winAttrib;

    x_window_width[win_id] = WIN_WIDTH;
    x_window_height[win_id] = WIN_HEIGHT;

    win_x11_get_visual(win_id);

    windowMask = CWBorderPixel;

    winAttrib.border_pixel = BlackPixel(x_display, x_screen);
    winAttrib.override_redirect = 0;

#if defined(WIN_X11_USE_GL)
    windowMask |= CWColormap;

    winAttrib.colormap = XCreateColormap(
        x_display,
        RootWindow(x_display, x_screen),
        x_visual,
        AllocNone
    );
#endif

    x_window[win_id] = XCreateWindow(
        x_display,
        RootWindow(x_display, x_screen),
        WIN_X_OFFSET,
        WIN_Y_OFFSET,
        WIN_WIDTH,
        WIN_HEIGHT,
        0,
        x_depth[win_id],
        InputOutput,
        x_visual[win_id],
        windowMask,
        &winAttrib
    );

    XSelectInput(x_display, x_window[win_id], StructureNotifyMask);

    XMapRaised(x_display, x_window[win_id]);

    XFlush(x_display);
}

void win_x11_fini_window(int win_id)
{
    XDestroyWindow(x_display, x_window[win_id]);
}

void win_x11_init_pixmap(int win_id)
{
    XGCValues gc_values;
    gc_values.foreground = WhitePixel(x_display, x_screen);
    gc_values.background = BlackPixel(x_display, x_screen);

    x_gc[win_id] = XCreateGC(
        x_display,
        x_window[win_id],
        GCForeground | GCBackground,
        &gc_values
    );

    x_pixmap[win_id] = XCreatePixmap(
        x_display,
        RootWindow(x_display, x_screen),
        WIN_WIDTH,
        WIN_HEIGHT,
        24
    );
}

void win_x11_fini_pixmap(int win_id)
{
    XFreePixmap(x_display, x_pixmap[win_id]);
}

#define CHECK_ST \
  if (vdp_st != VDP_STATUS_OK) { \
      printf("Error at %s:%d\n", __FILE__, __LINE__); \
      exit(1); \
  }

VdpStatus win_x11_init_vdpau_procs(void)
{
    VdpStatus vdp_st;

    // Create Device

    vdp_st = vdp_device_create_x11(
        x_display,
        x_screen,
        &vdp_device,
        &vdp_get_proc_address
    );
    CHECK_ST

    // Entry point retrieval

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_PREEMPTION_CALLBACK_REGISTER,
        (void **)&vdp_preemption_callback_register
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_GET_ERROR_STRING,
        (void **)&vdp_get_error_string
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_GET_API_VERSION,
        (void **)&vdp_get_api_version
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_GET_INFORMATION_STRING,
        (void **)&vdp_get_information_string
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_DEVICE_DESTROY,
        (void **)&vdp_device_destroy
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_GENERATE_CSC_MATRIX,
        (void **)&vdp_generate_csc_matrix
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_VIDEO_SURFACE_QUERY_CAPABILITIES,
        (void **)&vdp_video_surface_query_capabilities
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_VIDEO_SURFACE_QUERY_GET_PUT_BITS_Y_CB_CR_CAPABILITIES,
        (void **)&vdp_video_surface_query_get_put_bits_y_cb_cr_capabilities
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_VIDEO_SURFACE_CREATE,
        (void **)&vdp_video_surface_create
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_VIDEO_SURFACE_DESTROY,
        (void **)&vdp_video_surface_destroy
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_VIDEO_SURFACE_GET_PARAMETERS,
        (void **)&vdp_video_surface_get_parameters
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_VIDEO_SURFACE_GET_BITS_Y_CB_CR,
        (void **)&vdp_video_surface_get_bits_y_cb_cr
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_VIDEO_SURFACE_PUT_BITS_Y_CB_CR,
        (void **)&vdp_video_surface_put_bits_y_cb_cr
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_OUTPUT_SURFACE_QUERY_CAPABILITIES,
        (void **)&vdp_output_surface_query_capabilities
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_OUTPUT_SURFACE_QUERY_GET_PUT_BITS_NATIVE_CAPABILITIES,
        (void **)&vdp_output_surface_query_get_put_bits_native_capabilities
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_OUTPUT_SURFACE_QUERY_PUT_BITS_INDEXED_CAPABILITIES,
        (void **)&vdp_output_surface_query_put_bits_indexed_capabilities
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_OUTPUT_SURFACE_QUERY_PUT_BITS_Y_CB_CR_CAPABILITIES,
        (void **)&vdp_output_surface_query_put_bits_y_cb_cr_capabilities
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_OUTPUT_SURFACE_CREATE,
        (void **)&vdp_output_surface_create
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_OUTPUT_SURFACE_DESTROY,
        (void **)&vdp_output_surface_destroy
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_OUTPUT_SURFACE_GET_PARAMETERS,
        (void **)&vdp_output_surface_get_parameters
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_OUTPUT_SURFACE_GET_BITS_NATIVE,
        (void **)&vdp_output_surface_get_bits_native
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_OUTPUT_SURFACE_PUT_BITS_NATIVE,
        (void **)&vdp_output_surface_put_bits_native
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_OUTPUT_SURFACE_PUT_BITS_INDEXED,
        (void **)&vdp_output_surface_put_bits_indexed
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_OUTPUT_SURFACE_PUT_BITS_Y_CB_CR,
        (void **)&vdp_output_surface_put_bits_y_cb_cr
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_BITMAP_SURFACE_QUERY_CAPABILITIES,
        (void **)&vdp_bitmap_surface_query_capabilities
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_BITMAP_SURFACE_CREATE,
        (void **)&vdp_bitmap_surface_create
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_BITMAP_SURFACE_DESTROY,
        (void **)&vdp_bitmap_surface_destroy
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_BITMAP_SURFACE_GET_PARAMETERS,
        (void **)&vdp_bitmap_surface_get_parameters
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_BITMAP_SURFACE_PUT_BITS_NATIVE,
        (void **)&vdp_bitmap_surface_put_bits_native
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_OUTPUT_SURFACE_RENDER_OUTPUT_SURFACE,
        (void **)&vdp_output_surface_render_output_surface
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_OUTPUT_SURFACE_RENDER_BITMAP_SURFACE,
        (void **)&vdp_output_surface_render_bitmap_surface
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_DECODER_QUERY_CAPABILITIES,
        (void **)&vdp_decoder_query_capabilities
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_DECODER_CREATE,
        (void **)&vdp_decoder_create
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_DECODER_DESTROY,
        (void **)&vdp_decoder_destroy
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_DECODER_GET_PARAMETERS,
        (void **)&vdp_decoder_get_parameters
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_DECODER_RENDER,
        (void **)&vdp_decoder_render
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_VIDEO_MIXER_QUERY_FEATURE_SUPPORT,
        (void **)&vdp_video_mixer_query_feature_support
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_VIDEO_MIXER_QUERY_PARAMETER_SUPPORT,
        (void **)&vdp_video_mixer_query_parameter_support
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_VIDEO_MIXER_QUERY_ATTRIBUTE_SUPPORT,
        (void **)&vdp_video_mixer_query_attribute_support
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_VIDEO_MIXER_QUERY_PARAMETER_VALUE_RANGE,
        (void **)&vdp_video_mixer_query_parameter_value_range
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_VIDEO_MIXER_QUERY_ATTRIBUTE_VALUE_RANGE,
        (void **)&vdp_video_mixer_query_attribute_value_range
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_VIDEO_MIXER_CREATE,
        (void **)&vdp_video_mixer_create
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_VIDEO_MIXER_SET_FEATURE_ENABLES,
        (void **)&vdp_video_mixer_set_feature_enables
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_VIDEO_MIXER_SET_ATTRIBUTE_VALUES,
        (void **)&vdp_video_mixer_set_attribute_values
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_VIDEO_MIXER_GET_FEATURE_SUPPORT,
        (void **)&vdp_video_mixer_get_feature_support
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_VIDEO_MIXER_GET_FEATURE_ENABLES,
        (void **)&vdp_video_mixer_get_feature_enables
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_VIDEO_MIXER_GET_PARAMETER_VALUES,
        (void **)&vdp_video_mixer_get_parameter_values
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_VIDEO_MIXER_GET_ATTRIBUTE_VALUES,
        (void **)&vdp_video_mixer_get_attribute_values
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_VIDEO_MIXER_DESTROY,
        (void **)&vdp_video_mixer_destroy
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_VIDEO_MIXER_RENDER,
        (void **)&vdp_video_mixer_render
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_PRESENTATION_QUEUE_TARGET_DESTROY,
        (void **)&vdp_presentation_queue_target_destroy
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_PRESENTATION_QUEUE_CREATE,
        (void **)&vdp_presentation_queue_create
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_PRESENTATION_QUEUE_DESTROY,
        (void **)&vdp_presentation_queue_destroy
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_PRESENTATION_QUEUE_SET_BACKGROUND_COLOR,
        (void **)&vdp_presentation_queue_set_background_color
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_PRESENTATION_QUEUE_GET_BACKGROUND_COLOR,
        (void **)&vdp_presentation_queue_get_background_color
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_PRESENTATION_QUEUE_GET_TIME,
        (void **)&vdp_presentation_queue_get_time
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_PRESENTATION_QUEUE_DISPLAY,
        (void **)&vdp_presentation_queue_display
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_PRESENTATION_QUEUE_BLOCK_UNTIL_SURFACE_IDLE,
        (void **)&vdp_presentation_queue_block_until_surface_idle
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_PRESENTATION_QUEUE_QUERY_SURFACE_STATUS,
        (void **)&vdp_presentation_queue_query_surface_status
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_PREEMPTION_CALLBACK_REGISTER,
        (void **)&vdp_preemption_callback_register
    );
    CHECK_ST

    vdp_st = vdp_get_proc_address(
        vdp_device,
        VDP_FUNC_ID_PRESENTATION_QUEUE_TARGET_CREATE_X11,
        (void **)&vdp_presentation_queue_target_create_x11
    );
    CHECK_ST

    return VDP_STATUS_OK;
}

VdpStatus win_x11_fini_vdpau_procs(void)
{
    VdpStatus vdp_st;

    vdp_st = vdp_device_destroy(
        vdp_device
    );
    CHECK_ST

    return VDP_STATUS_OK;
}

VdpStatus win_x11_init_vdpau_flip_queue(int win_id, int is_pixmap)
{
    VdpStatus vdp_st;

    vdp_st = vdp_presentation_queue_target_create_x11(
        vdp_device,
        is_pixmap ? x_pixmap[win_id] : x_window[win_id],
        &vdp_flip_target[win_id]
    );
    CHECK_ST

    vdp_st = vdp_presentation_queue_create(
        vdp_device,
        vdp_flip_target[win_id],
        &vdp_flip_queue[win_id]
    );
    CHECK_ST

    return VDP_STATUS_OK;
}

VdpStatus win_x11_fini_vdpau_flip_queue(int win_id)
{
    VdpStatus vdp_st;


    vdp_st = vdp_presentation_queue_destroy(
        vdp_flip_queue[win_id]
    );
    CHECK_ST

    vdp_st = vdp_presentation_queue_target_destroy(
        vdp_flip_target[win_id]
    );
    CHECK_ST

    return VDP_STATUS_OK;
}

Bool _return_true(Display *display, XEvent *event, XPointer arg)
{
    return True;
}

void win_x11_poll_events()
{
    int win_id;

    for (;;) {
        XEvent event;
        Bool is_event = XCheckIfEvent(x_display, &event, _return_true, NULL);
        if (!is_event) {
            return;
        }
        switch (event.type) {
        case ConfigureNotify:
            for (win_id = 0; win_id < WIN_X11_MAX_WIN_IDS; ++win_id) {
                if (x_window[win_id] == event.xconfigure.window) {
                    x_window_width[win_id] = event.xconfigure.width;
                    x_window_height[win_id] = event.xconfigure.height;
                }
            }
            break;
        default:
            break;
        }
    }
}

uint32_t win_x11_get_width(int win_id)
{
    return x_window_width[win_id];
}

uint32_t win_x11_get_height(int win_id)
{
    return x_window_height[win_id];
}

#if defined(WIN_X11_USE_GL)
void win_x11_gl_draw_in_window(int win_id)
{
    Window winDummy;
    int x, y;
    unsigned int width, height;
    unsigned int borderDummy;
    unsigned int depth;

    gl_context = glXCreateContext(x_display, x_visual_info[win_id], 0, GL_TRUE);

    glXMakeCurrent(x_display, x_window[win_id], gl_context);

    XGetGeometry(
        x_display,
        x_window[win_id],
        &winDummy,
        &x,
        &y,
        &width,
        &height,
        &borderDummy,
        &depth
    );

    if (height < 1) {
        height = 1;
    }
    if (width < 1) {
        width = 1;
    }

    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glFlush();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(-1.5f, 0.0f, -6.0f);
    glBegin(GL_TRIANGLES);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 0.0f);
    glEnd();
    glTranslatef(3.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex3f(-1.0f, 1.0f, 0.0f);
        glVertex3f(1.0f, 1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 0.0f);
    glEnd();
    if (gl_double_buffered) {
        glXSwapBuffers(x_display, x_window);
    }
    glFinish();
}
#endif

void win_x11_copy_pixmap_to_win(int win_id)
{
    XCopyArea(
        x_display,
        x_pixmap[win_id],
        x_window[win_id],
        x_gc[win_id],
        0,
        0,
        x_window_width[win_id],
        x_window_height[win_id],
        0,
        0
    );

    XFlush(x_display);
}

