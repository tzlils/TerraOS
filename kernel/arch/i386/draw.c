#include <kernel/draw.h>
#include <kernel/vesa.h>
#include <kernel/math.h>
#include <stddef.h>
#include <stdarg.h>
void draw_rect(struct Vector2 pos, int w, int h) {
    for (size_t i = pos.y; i < pos.y+h; i++)
    {
        for (size_t j = pos.x; j < pos.x+w; j++) {
            draw_pixel_at(j, i, get_vesa_color());
        }
    }
}

void draw_line(struct Vector2 a, struct Vector2 b) {
    int w = b.x - a.x;
    int h = b.y - a.y;
    double m = h/(double)w;
    double j = a.y;
    for (size_t i = a.x; i <= b.x; i++)
    {
        draw_pixel_at(i, (int)j, get_vesa_color());
        j += m;
    }
}

void draw_circle(int x, int y, int r) {
    double slice = 2 * PI / r;
    for (size_t i = 0; i < r; i++)
    {
        double angle = slice * i;
        int px = x+r*sin(angle);
        int py = y+r+cos(angle);
        draw_line(*Vector2_new(x, y), *Vector2_new(px, py));
    }
}

void draw_polygon(int num, ...) {
    va_list vertices;
	va_start(vertices, num);
    Vector2 last_vec;
    
    for (size_t i = 0; i < num; i++)
    {
        if(i < 1) {
            draw_line(last_vec, va_arg(vertices, Vector2));
        }
    }
    

    va_end(vertices);
    return 0;
};

void draw_hello() {
    int x_offset = 500;
    int y_offset = 500;
    draw_rect(*Vector2_new(x_offset, y_offset), 30, 100);
    draw_rect(*Vector2_new(x_offset-50, y_offset), 30, 100);
    draw_rect(*Vector2_new(x_offset-50, y_offset+40), 50, 20);


    x_offset += 90;
    draw_rect(*Vector2_new(x_offset-50, y_offset), 30, 100);
    draw_rect(*Vector2_new(x_offset-50, y_offset+80), 50, 20);
    draw_rect(*Vector2_new(x_offset-50, y_offset+40), 50, 20);
    draw_rect(*Vector2_new(x_offset-50, y_offset), 50, 20);

    x_offset += 60;
    draw_rect(*Vector2_new(x_offset-50, y_offset), 30, 100);
    draw_rect(*Vector2_new(x_offset-50, y_offset+80), 50, 20);

    x_offset += 60;
    draw_rect(*Vector2_new(x_offset-50, y_offset), 30, 100);
    draw_rect(*Vector2_new(x_offset-50, y_offset+80), 50, 20);

    x_offset += 60;
    draw_rect(*Vector2_new(x_offset, y_offset), 30, 100);
    draw_rect(*Vector2_new(x_offset-50, y_offset), 30, 100);
    draw_rect(*Vector2_new(x_offset-50, y_offset+80), 50, 20);
    draw_rect(*Vector2_new(x_offset-50, y_offset), 50, 20);

    x_offset += 45;
    draw_rect(*Vector2_new(x_offset, y_offset), 20, 70);
    draw_rect(*Vector2_new(x_offset, y_offset+80), 20, 20);
}