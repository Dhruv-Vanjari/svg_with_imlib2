/*
 *  svg_with_imlib is just a demo program to render a svg image on a xdrawable
 *  Copyright (C) 2021 Dhruv Vanjari
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include<stdio.h>
#include<X11/Xlib.h>
#include<cairo.h>
#include<librsvg/rsvg.h>
#include<Imlib2.h>
#include<stdio.h>

#define DEBUG 1

#if DEBUG
#define LOG(x) printf(x)
#else
#define LOG(x)
#endif

int main() {

    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 640, 640);
    cairo_t *cr = cairo_create(surface);

    RsvgHandle *svg_handle = rsvg_handle_new_from_file("test.svg", 0);
    if (svg_handle == NULL) {
        printf("failed to open image\n");
        return 1;
    }

    LOG("setting dpi to 70\n");
    rsvg_handle_set_dpi(svg_handle, 70);      // this is just a hard coded value that works with the test image

    LOG("rendering image to cairo surface\n");
    rsvg_handle_render_cairo(svg_handle, cr);

    LOG("getting surface data from cairo surface\n");
    DATA32 *svg_buffer;
    svg_buffer = (DATA32 *) cairo_image_surface_get_data(surface);

    LOG("creating new imlib_image form cairo surface data\n");
    Imlib_Image img = imlib_create_image_using_data(640, 640, svg_buffer);
    if (img == NULL) {
        LOG("failed to create image\n");
        return 1;
    }
    LOG("imlib_image created successfully\n");

    LOG("initializing X11 stuff\n");
    Display *disp;
    Window win;
    Visual *vis;
    Colormap cm;
    int depth;

    disp = XOpenDisplay(NULL);
    vis = DefaultVisual(disp, DefaultScreen(disp));
    depth = DefaultDepth(disp, DefaultScreen(disp));
    cm = DefaultColormap(disp, DefaultScreen(disp));

    LOG("Creating a window\n");
    win = XCreateSimpleWindow(disp, DefaultRootWindow(disp), 0, 0, 300, 300, 0, 0, 0x000000);
    XMapWindow(disp, win);

    LOG("initializing imlib\n");
    imlib_set_color_usage(128);
    imlib_context_set_display(disp);
    imlib_context_set_visual(vis);
    imlib_context_set_colormap(cm);
    imlib_context_set_drawable(win);
    imlib_set_cache_size(1024 * 4096);

    LOG("Rendering image\n");
    imlib_context_set_image(img);
    imlib_render_image_on_drawable(0, 0);

    LOG("Destroying objects\n");
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
    imlib_free_image();
    g_object_unref(svg_handle);

    printf("press enter to exit\n");
    scanf("q");

    return 0;
}
