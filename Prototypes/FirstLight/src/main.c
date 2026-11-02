/*
 * Vita Engine Research Lab — First Light
 * Original project code. Native Vita homebrew using vitaGL's GXM-backed
 * OpenGL subset for the first hardware-rendering milestone.
 *
 * This is a deliberately small proof of the display/render loop, not yet
 * the engine renderer and not a direct-SceGxm backend.
 */
#include <vitaGL.h>
#include <psp2/ctrl.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/threadmgr.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define SCREEN_W 960
#define SCREEN_H 544

static const uint8_t *glyph(char c) {
    static const uint8_t blank[7] = {0,0,0,0,0,0,0};
    static const uint8_t digits[10][7] = {
        {14,17,19,21,25,17,14}, {4,12,4,4,4,4,14},
        {14,17,1,2,4,8,31}, {30,1,1,14,1,1,30},
        {2,6,10,18,31,2,2}, {31,16,16,30,1,1,30},
        {6,8,16,30,17,17,14}, {31,1,2,4,8,8,8},
        {14,17,17,14,17,17,14}, {14,17,17,15,1,2,12}
    };
    static const uint8_t F[7] = {31,16,16,30,16,16,16};
    static const uint8_t P[7] = {30,17,17,30,16,16,16};
    static const uint8_t S[7] = {15,16,16,14,1,1,30};
    static const uint8_t M[7] = {17,27,21,21,17,17,17};
    static const uint8_t colon[7] = {0,4,4,0,4,4,0};
    static const uint8_t dot[7] = {0,0,0,0,0,12,12};

    if (c >= '0' && c <= '9') return digits[c - '0'];
    switch (c) {
        case 'F': return F;
        case 'P': return P;
        case 'S': return S;
        case 'M': return M;
        case ':': return colon;
        case '.': return dot;
        default: return blank;
    }
}

static void draw_text(float x, float y, const char *text, float scale) {
    glBegin(GL_QUADS);
    for (size_t n = 0; n < strlen(text); ++n) {
        const uint8_t *rows = glyph(text[n]);
        for (int row = 0; row < 7; ++row) {
            for (int col = 0; col < 5; ++col) {
                if (rows[row] & (1u << (4 - col))) {
                    float px = x + (float)col * scale;
                    float py = y + (float)row * scale;
                    glVertex2f(px, py);
                    glVertex2f(px + scale - 1.0f, py);
                    glVertex2f(px + scale - 1.0f, py + scale - 1.0f);
                    glVertex2f(px, py + scale - 1.0f);
                }
            }
        }
        x += 6.0f * scale;
    }
    glEnd();
}

static void draw_cube(void) {
    /* Six coloured faces, two triangles per face. */
    static const float face_colours[6][3] = {
        {0.20f, 0.78f, 1.00f}, {0.12f, 0.42f, 0.90f},
        {0.20f, 0.92f, 0.72f}, {0.78f, 0.42f, 1.00f},
        {1.00f, 0.68f, 0.24f}, {0.96f, 0.34f, 0.46f}
    };
    static const float faces[6][6][3] = {
        {{-1,-1, 1},{ 1,-1, 1},{ 1, 1, 1},{-1,-1, 1},{ 1, 1, 1},{-1, 1, 1}},
        {{ 1,-1,-1},{-1,-1,-1},{-1, 1,-1},{ 1,-1,-1},{-1, 1,-1},{ 1, 1,-1}},
        {{-1,-1,-1},{-1,-1, 1},{-1, 1, 1},{-1,-1,-1},{-1, 1, 1},{-1, 1,-1}},
        {{ 1,-1, 1},{ 1,-1,-1},{ 1, 1,-1},{ 1,-1, 1},{ 1, 1,-1},{ 1, 1, 1}},
        {{-1, 1, 1},{ 1, 1, 1},{ 1, 1,-1},{-1, 1, 1},{ 1, 1,-1},{-1, 1,-1}},
        {{-1,-1,-1},{ 1,-1,-1},{ 1,-1, 1},{-1,-1,-1},{ 1,-1, 1},{-1,-1, 1}}
    };

    glBegin(GL_TRIANGLES);
    for (int face = 0; face < 6; ++face) {
        glColor3f(face_colours[face][0], face_colours[face][1], face_colours[face][2]);
        for (int vertex = 0; vertex < 6; ++vertex) {
            glVertex3f(faces[face][vertex][0], faces[face][vertex][1],
                       faces[face][vertex][2]);
        }
    }
    glEnd();
}

static void draw_rounded_panel(float x, float y, float w, float h, float radius) {
    const float pi = 3.14159265f;
    const float centers[4][2] = {
        {x + radius, y + radius},
        {x + w - radius, y + radius},
        {x + w - radius, y + h - radius},
        {x + radius, y + h - radius}
    };
    const float starts[4] = {pi, 1.5f * pi, 0.0f, 0.5f * pi};

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x + w * 0.5f, y + h * 0.5f);
    for (int corner = 0; corner < 4; ++corner) {
        for (int step = 0; step <= 6; ++step) {
            float angle = starts[corner] + (0.5f * pi * (float)step / 6.0f);
            glVertex2f(centers[corner][0] + cosf(angle) * radius,
                       centers[corner][1] + sinf(angle) * radius);
        }
    }
    glVertex2f(centers[0][0] - radius, centers[0][1]);
    glEnd();
}

static void draw_benchmark_icon(float cx, float cy) {
    /* Compact speedometer mark: a clear benchmark identity at HUD scale. */
    const float radius = 13.0f;
    glColor4f(0.28f, 0.82f, 1.0f, 1.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 24; ++i) {
        float angle = 3.14159265f + (3.14159265f * (float)i / 24.0f);
        glVertex2f(cx + cosf(angle) * radius, cy + sinf(angle) * radius);
    }
    glEnd();

    glBegin(GL_LINES);
    for (int i = 0; i <= 4; ++i) {
        float angle = 3.14159265f + (3.14159265f * (float)i / 4.0f);
        glVertex2f(cx + cosf(angle) * 9.0f, cy + sinf(angle) * 9.0f);
        glVertex2f(cx + cosf(angle) * 12.0f, cy + sinf(angle) * 12.0f);
    }
    glVertex2f(cx, cy);
    glVertex2f(cx + 8.0f, cy - 7.0f);
    glEnd();

    glPointSize(3.0f);
    glBegin(GL_POINTS);
    glVertex2f(cx, cy);
    glEnd();
}

static void draw_hud(float fps, float frame_ms) {
    char line[48];
    snprintf(line, sizeof(line), "FPS:%.1f MS:%.2f", fps, frame_ms);

    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, SCREEN_W, SCREEN_H, 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    /* Dark translucent HUD plate. */
    glColor4f(0.015f, 0.035f, 0.065f, 0.88f);
    draw_rounded_panel(18.0f, 18.0f, 452.0f, 58.0f, 12.0f);

    glColor3f(0.78f, 0.92f, 1.0f);
    draw_benchmark_icon(49.0f, 53.0f);
    draw_text(82, 31, line, 4.0f);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
}

int main(void) {
    /* Allocate the vitaGL command/temporary memory pool and initialize GXM. */
    vglInit(0x800000);

    glViewport(0, 0, SCREEN_W, SCREEN_H);
    glClearColor(0.025f, 0.045f, 0.085f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(55.0f, (float)SCREEN_W / (float)SCREEN_H, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);

    uint64_t previous_us = sceKernelGetProcessTimeWide();
    uint64_t sample_start_us = previous_us;
    unsigned frames_in_sample = 0;
    float fps = 0.0f;
    float frame_ms = 0.0f;
    float rotation = 0.0f;
    SceCtrlData pad;

    for (;;) {
        sceCtrlPeekBufferPositive(0, &pad, 1);
        if (pad.buttons & SCE_CTRL_START) break;

        uint64_t now_us = sceKernelGetProcessTimeWide();
        uint64_t delta_us = now_us - previous_us;
        previous_us = now_us;
        frame_ms = (float)delta_us / 1000.0f;
        rotation += (float)delta_us * 0.000045f;
        if (rotation >= 360.0f) rotation -= 360.0f;

        ++frames_in_sample;
        if (now_us - sample_start_us >= 1000000ULL) {
            fps = (float)frames_in_sample * 1000000.0f /
                  (float)(now_us - sample_start_us);
            frames_in_sample = 0;
            sample_start_us = now_us;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -4.2f);
        glRotatef(rotation, 0.8f, 1.0f, 0.35f);
        draw_cube();
        draw_hud(fps, frame_ms);
        vglSwapBuffers(GL_FALSE);
    }

    sceKernelExitProcess(0);
    return 0;
}
