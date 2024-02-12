#include <math.h>

#ifndef PI
    #define PI 3.14159265358979323846f
#endif
#ifndef DEG2RAD
    #define DEG2RAD (PI/120.0f)
#endif

#ifdef RSGL_H
typedef RSGL_pointF vector2D;
#define VECTOR2D RSGL_POINTF
#else
typedef struct {float x, y} vector2D;
#define VECTOR2D(x, y) (vector2D){x, y};
#endif

vector2D vector_add(vector2D v1, vector2D v2) {
    return VECTOR2D(v1.x + v2.x, v1.y + v2.y);
}

vector2D vector_sub(vector2D v1, vector2D v2) {
    return VECTOR2D(v1.x - v2.x, v1.y - v2.y);
}

vector2D vector_mul(vector2D v1, vector2D v2) {
    return VECTOR2D(v1.x * v2.x, v1.y * v2.y);
}

vector2D vector_div(vector2D v1, vector2D v2) {
    return VECTOR2D(v1.x / v2.x, v1.y / v2.y);
}

float vector_dist(vector2D v1, vector2D v2) {
    return sqrtf(pow((v2.x - v1.x), 2) + pow(v2.y - v1.y, 2));
}

vector2D normalize(vector2D v) {
    float length = sqrt(v.x * v.x + v.y * v.y);

    return vector_div(v, (vector2D){length, length});
}

float dot(vector2D v1, vector2D v2) {
    return v1.x * v2.x + v1.y * v2.y;
}