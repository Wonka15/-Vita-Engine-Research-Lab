/*
 * ============================================================================
 * VITA ENGINE RESEARCH LAB
 * FIRST LIGHT — Native PlayStation Vita Graphics Prototype
 * ============================================================================
 *
 * PURPOSE
 *   Draw a rotating, six-colour cube and show live performance information.
 *
 * GRAPHICS BACKEND
 *   vitaGL (an OpenGL-style API translated to the Vita's SceGxm GPU).
 *   This is NOT yet a direct SceGxm renderer.
 *
 * BEGINNER'S GUIDE
 *   [CONFIG]   Screen size and fixed settings.
 *   [FONT]     Tiny built-in bitmap font used by the HUD.
 *   [SCENE]    Cube geometry and drawing.
 *   [HUD]      Rounded benchmark badge and speedometer icon.
 *   [STARTUP]  Graphics setup and camera.
 *   [LOOP]     Input, timing, animation and drawing.
 *
 * The comments explain the intention of each section. C syntax highlighting
 * in an editor such as VS Code provides the actual code colour-coding.
 * ============================================================================
 */

/* --------------------------------------------------------------------------
 * 1. VITA SDK AND STANDARD LIBRARY INCLUDES
 * -------------------------------------------------------------------------- */

#include <vitaGL.h>
#include <psp2/ctrl.h>
#include <psp2/kernel/processmgr.h>

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* --------------------------------------------------------------------------
 * 2. CONFIGURATION
 * -------------------------------------------------------------------------- */

/* The Vita's native screen resolution in landscape orientation. */
#define SCREEN_WIDTH  960
#define SCREEN_HEIGHT 544

/* --------------------------------------------------------------------------
 * 3. HUD FONT — SIMPLE 5 x 7 BITMAP CHARACTERS
 * -------------------------------------------------------------------------- */

/*
 * Each number below is one row of five pixels.
 * A set bit (1) means "draw a pixel"; a clear bit (0) means "leave it empty".
 * This keeps the first prototype independent of external font files.
 */
static const uint8_t *get_glyph(char character)
{
    static const uint8_t blank[7] = {
        0, 0, 0, 0, 0, 0, 0
    };

    static const uint8_t digits[10][7] = {
        {14, 17, 19, 21, 25, 17, 14},
        { 4, 12,  4,  4,  4,  4, 14},
        {14, 17,  1,  2,  4,  8, 31},
        {30,  1,  1, 14,  1,  1, 30},
        { 2,  6, 10, 18, 31,  2,  2},
        {31, 16, 16, 30,  1,  1, 30},
        { 6,  8, 16, 30, 17, 17, 14},
        {31,  1,  2,  4,  8,  8,  8},
        {14, 17, 17, 14, 17, 17, 14},
        {14, 17, 17, 15,  1,  2, 12}
    };

    static const uint8_t letter_f[7] = {
        31, 16, 16, 30, 16, 16, 16
    };

    static const uint8_t letter_p[7] = {
        30, 17, 17, 30, 16, 16, 16
    };

    static const uint8_t letter_s[7] = {
        15, 16, 16, 14, 1, 1, 30
    };

    static const uint8_t letter_m[7] = {
        17, 27, 21, 21, 17, 17, 17
    };

    static const uint8_t colon[7] = {
        0, 4, 4, 0, 4, 4, 0
    };

    static const uint8_t decimal_point[7] = {
        0, 0, 0, 0, 0, 12, 12
    };

    if (character >= '0' && character <= '9') {
        return digits[character - '0'];
    }

    switch (character) {
        case 'F': return letter_f;
        case 'P': return letter_p;
        case 'S': return letter_s;
        case 'M': return letter_m;
        case ':': return colon;
        case '.': return decimal_point;
        default:  return blank;
    }
}

/*
 * Draw a short line of text using small rectangles.
 *
 * x, y   = top-left position on the screen.
 * scale  = size multiplier for each font pixel.
 */
static void draw_text(float x, float y, const char *text, float scale)
{
    glBegin(GL_QUADS);

    for (size_t character_index = 0;
         character_index < strlen(text);
         ++character_index) {

        const uint8_t *rows = get_glyph(text[character_index]);

        for (int row = 0; row < 7; ++row) {
            for (int column = 0; column < 5; ++column) {

                /* Check whether this font pixel should be visible. */
                const uint8_t pixel_mask = (uint8_t)(1u << (4 - column));

                if (rows[row] & pixel_mask) {
                    const float pixel_x = x + (float)column * scale;
                    const float pixel_y = y + (float)row * scale;
                    const float pixel_size = scale - 1.0f;

                    glVertex2f(pixel_x,              pixel_y);
                    glVertex2f(pixel_x + pixel_size, pixel_y);
                    glVertex2f(pixel_x + pixel_size, pixel_y + pixel_size);
                    glVertex2f(pixel_x,              pixel_y + pixel_size);
                }
            }
        }

        /* Move to the next character, leaving one column of spacing. */
        x += 6.0f * scale;
    }

    glEnd();
}

/* --------------------------------------------------------------------------
 * 4. FIRST LIGHT TEST SCENE — ORIGINAL ROTATING CUBE
 * -------------------------------------------------------------------------- */

/*
 * The cube has six faces.
 * Each face is made from two triangles, giving twelve triangles in total.
 * Coordinates are kept simple so this scene is easy to understand.
 */
static void draw_cube(void)
{
    /* One RGB colour for each face. Values range from 0.0 to 1.0. */
    static const float face_colours[6][3] = {
        {0.20f, 0.78f, 1.00f},
        {0.12f, 0.42f, 0.90f},
        {0.20f, 0.92f, 0.72f},
        {0.78f, 0.42f, 1.00f},
        {1.00f, 0.68f, 0.24f},
        {0.96f, 0.34f, 0.46f}
    };

    /* Six vertices per face: three for each of its two triangles. */
    static const float face_vertices[6][6][3] = {
        {
            {-1, -1,  1}, { 1, -1,  1}, { 1,  1,  1},
            {-1, -1,  1}, { 1,  1,  1}, {-1,  1,  1}
        },
        {
            { 1, -1, -1}, {-1, -1, -1}, {-1,  1, -1},
            { 1, -1, -1}, {-1,  1, -1}, { 1,  1, -1}
        },
        {
            {-1, -1, -1}, {-1, -1,  1}, {-1,  1,  1},
            {-1, -1, -1}, {-1,  1,  1}, {-1,  1, -1}
        },
        {
            { 1, -1,  1}, { 1, -1, -1}, { 1,  1, -1},
            { 1, -1,  1}, { 1,  1, -1}, { 1,  1,  1}
        },
        {
            {-1,  1,  1}, { 1,  1,  1}, { 1,  1, -1},
            {-1,  1,  1}, { 1,  1, -1}, {-1,  1, -1}
        },
        {
            {-1, -1, -1}, { 1, -1, -1}, { 1, -1,  1},
            {-1, -1, -1}, { 1, -1,  1}, {-1, -1,  1}
        }
    };

    glBegin(GL_TRIANGLES);

    for (int face = 0; face < 6; ++face) {
        glColor3f(
            face_colours[face][0],
            face_colours[face][1],
            face_colours[face][2]
        );

        for (int vertex = 0; vertex < 6; ++vertex) {
            glVertex3f(
                face_vertices[face][vertex][0],
                face_vertices[face][vertex][1],
                face_vertices[face][vertex][2]
            );
        }
    }

    glEnd();
}

/* --------------------------------------------------------------------------
 * 5. BENCHMARK HUD — ROUNDED PANEL AND SPEEDOMETER ICON
 * -------------------------------------------------------------------------- */

/*
 * Draw a rounded rectangle from four small corner arcs.
 * This is geometry, not an image file, so it scales cleanly with the HUD.
 */
static void draw_rounded_panel(
    float x,
    float y,
    float width,
    float height,
    float radius
) {
    const float half_pi = 1.57079632679f;
    const float pi = 3.14159265359f;

    const float corner_centres[4][2] = {
        {x + radius,          y + radius},
        {x + width - radius,  y + radius},
        {x + width - radius,  y + height - radius},
        {x + radius,          y + height - radius}
    };

    /* Each corner arc starts where the previous one finished. */
    const float arc_starts[4] = {
        pi,
        1.5f * pi,
        0.0f,
        0.5f * pi
    };

    glBegin(GL_TRIANGLE_FAN);

    /* The fan's centre fills the rounded panel. */
    glVertex2f(x + width * 0.5f, y + height * 0.5f);

    for (int corner = 0; corner < 4; ++corner) {
        for (int step = 0; step <= 6; ++step) {
            const float angle =
                arc_starts[corner] + half_pi * (float)step / 6.0f;

            glVertex2f(
                corner_centres[corner][0] + cosf(angle) * radius,
                corner_centres[corner][1] + sinf(angle) * radius
            );
        }
    }

    /* Close the outline back at the first corner. */
    glVertex2f(corner_centres[0][0] - radius, corner_centres[0][1]);

    glEnd();
}

/* Draw the small speedometer symbol beside the live performance numbers. */
static void draw_benchmark_icon(float centre_x, float centre_y)
{
    const float radius = 13.0f;
    const float pi = 3.14159265359f;

    /* Cyan arc: the outer edge of the gauge. */
    glColor4f(0.28f, 0.82f, 1.0f, 1.0f);

    glBegin(GL_LINE_STRIP);

    for (int point = 0; point <= 24; ++point) {
        const float angle = pi + pi * (float)point / 24.0f;

        glVertex2f(
            centre_x + cosf(angle) * radius,
            centre_y + sinf(angle) * radius
        );
    }

    glEnd();

    /* Five short tick marks make the arc read as a gauge. */
    glBegin(GL_LINES);

    for (int tick = 0; tick <= 4; ++tick) {
        const float angle = pi + pi * (float)tick / 4.0f;

        glVertex2f(
            centre_x + cosf(angle) * 9.0f,
            centre_y + sinf(angle) * 9.0f
        );

        glVertex2f(
            centre_x + cosf(angle) * 12.0f,
            centre_y + sinf(angle) * 12.0f
        );
    }

    /* Needle points towards the current reading. */
    glVertex2f(centre_x, centre_y);
    glVertex2f(centre_x + 8.0f, centre_y - 7.0f);

    glEnd();

    /* Small centre pin for the needle. */
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    glVertex2f(centre_x, centre_y);
    glEnd();
}

/*
 * Draw the live performance badge.
 *
 * FPS     = frames per second (how many frames are drawn each second).
 * frame_ms = duration of the most recently measured frame, in milliseconds.
 */
static void draw_benchmark_hud(float fps, float frame_ms)
{
    char performance_text[48];

    snprintf(
        performance_text,
        sizeof(performance_text),
        "FPS:%.1f MS:%.2f",
        fps,
        frame_ms
    );

    /*
     * The HUD uses screen coordinates:
     * (0, 0) is the top-left corner; positive Y moves down the screen.
     */
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(
        0.0,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        0.0,
        -1.0,
        1.0
    );

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    /* Dark glass-style background plate. */
    glColor4f(0.015f, 0.035f, 0.065f, 0.88f);

    draw_rounded_panel(
        18.0f,
        18.0f,
        452.0f,
        58.0f,
        12.0f
    );

    /* Cyan speedometer, followed by the readable live figures. */
    glColor3f(0.78f, 0.92f, 1.0f);

    draw_benchmark_icon(49.0f, 53.0f);
    draw_text(82.0f, 31.0f, performance_text, 4.0f);

    /* Restore the 3D camera matrices before the next frame is drawn. */
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
}

/* --------------------------------------------------------------------------
 * 6. APPLICATION STARTUP — DISPLAY, DEPTH BUFFER AND CAMERA
 * -------------------------------------------------------------------------- */

int main(void)
{
    /* Initialise vitaGL and reserve its command/temporary memory pool. */
    vglInit(0x800000);

    /* Match the viewport to the Vita's landscape display. */
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    /* A deep blue background makes the coloured cube easy to see. */
    glClearColor(0.025f, 0.045f, 0.085f, 1.0f);

    /* Depth testing makes nearer cube faces cover faces behind them. */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    /* Back-face culling avoids drawing the cube faces facing away. */
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    /* Set up a perspective camera, similar to how human vision feels. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(
        55.0f,
        (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
        0.1f,
        100.0f
    );

    glMatrixMode(GL_MODELVIEW);

    /* ----------------------------------------------------------------------
     * 7. FRAME TIMING AND ANIMATION VARIABLES
     * ---------------------------------------------------------------------- */

    /*
     * Vita time is measured in microseconds here.
     * 1,000 microseconds = 1 millisecond.
     * 1,000,000 microseconds = 1 second.
     */
    uint64_t previous_time_us = sceKernelGetProcessTimeWide();
    uint64_t sample_start_us = previous_time_us;

    unsigned frames_in_sample = 0;

    float frames_per_second = 0.0f;
    float last_frame_ms = 0.0f;
    float cube_rotation = 0.0f;

    SceCtrlData controller;

    /* ----------------------------------------------------------------------
     * 8. MAIN LOOP — INPUT, TIMING, DRAWING AND DISPLAY
     * ---------------------------------------------------------------------- */

    for (;;) {
        /* Read the Vita controls. START closes this prototype. */
        sceCtrlPeekBufferPositive(0, &controller, 1);

        if (controller.buttons & SCE_CTRL_START) {
            break;
        }

        /* Measure the time since the previous frame. */
        const uint64_t current_time_us = sceKernelGetProcessTimeWide();
        const uint64_t elapsed_us = current_time_us - previous_time_us;

        previous_time_us = current_time_us;

        /* Convert microseconds to milliseconds for the HUD. */
        last_frame_ms = (float)elapsed_us / 1000.0f;

        /* Rotate at a time-based speed, not a fixed amount per frame. */
        cube_rotation += (float)elapsed_us * 0.000045f;

        if (cube_rotation >= 360.0f) {
            cube_rotation -= 360.0f;
        }

        /* Count frames until the next one-second FPS sample is complete. */
        ++frames_in_sample;

        if (current_time_us - sample_start_us >= 1000000ULL) {
            const uint64_t sample_duration_us =
                current_time_us - sample_start_us;

            frames_per_second =
                (float)frames_in_sample * 1000000.0f /
                (float)sample_duration_us;

            frames_in_sample = 0;
            sample_start_us = current_time_us;
        }

        /* ------------------------- DRAW THIS FRAME ----------------------- */

        /* Clear the previous frame's colour and depth information. */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* Reset the camera, move the cube into view and rotate it. */
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -4.2f);
        glRotatef(cube_rotation, 0.8f, 1.0f, 0.35f);

        /* Draw the 3D test object, then the screen-space benchmark badge. */
        draw_cube();
        draw_benchmark_hud(frames_per_second, last_frame_ms);

        /* Present the completed frame on the Vita's display. */
        vglSwapBuffers(GL_FALSE);
    }

    /* Exit cleanly when the user presses START. */
    sceKernelExitProcess(0);

    return 0;
}
