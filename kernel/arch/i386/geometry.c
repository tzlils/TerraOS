#include <kernel/geometry.h>
#include <kernel/kheap.h>

Vector2* Vector2_new(float x, float y) {
    Vector2* v = kmalloc(sizeof(Vector2));
    v->x = x;
    v->y = y;
    return v;
};

bool Vector2_compare(Vector2 a, Vector2 b) {
    return a.x == b.x && a.y == b.y;
}