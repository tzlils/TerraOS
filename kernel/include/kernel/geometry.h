#include <stdbool.h>
typedef struct Vector2 {
    float x;
    float y;
} Vector2;

Vector2* Vector2_new(float x, float y);
bool Vector2_compare(Vector2 a, Vector2 b);