void mat4_ortho(float* mat4, float left, float right, float bottom, float top, float near, float far) {
    float lr = 1 / (left - right), bt = 1 / (bottom - top), nf = 1 / (near - far);

    mat4[0] = -2 * lr;
    mat4[1] = 0;
    mat4[2] = 0;
    mat4[3] = 0;
    mat4[4] = 0;
    mat4[5] = -2 * bt;
    mat4[6] = 0;
    mat4[7] = 0;
    mat4[8] = 0;
    mat4[9] = 0;
    mat4[10] = 2 * nf;
    mat4[11] = 0;
    mat4[12] = (left + right) * lr;
    mat4[13] = (top + bottom) * bt;
    mat4[14] = (far + near) * nf;
    mat4[15] = 1;
    
    return mat4;
};
