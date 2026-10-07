#include <X11/Xutil.h>
#include <string.h>

void DrawCenteredText(Window window, Display* display, int screen, const char *text) {
    XWindowAttributes attrs;
    XGetWindowAttributes(display, window, &attrs);

    int win_width = attrs.width;
    int win_height = attrs.height;

    XFontStruct *fs;
    XGCValues values;
    XGetGCValues(display, DefaultGC(display, screen), GCFont, &values);

    fs = XQueryFont(display, values.font);
    if (!fs) return;

    int text_width  = XTextWidth(fs, text, strlen(text));
    int text_height = fs->ascent;

    int x = (win_width  - text_width) / 2;
    int y = (win_height + text_height) / 2;

    XDrawString(display, window, DefaultGC(display, screen), x, y, text, strlen(text));
    XFlush(display);

    XFreeFont(display, fs);
}

int main() {
    Display* display = XOpenDisplay(NULL);
    if (!display) return 1;

    int screen = DefaultScreen(display);

    int screen_width  = DisplayWidth(display, screen);
    int screen_height = DisplayHeight(display, screen);

    int win_width = 500;
    int win_height = 500;

    int x = (screen_width - win_width) / 2;
    int y = (screen_height - win_height) / 2;

    Window window = XCreateSimpleWindow(
        display,
        RootWindow(display, screen),
        x, y,
        win_width, win_height,
        2,
        BlackPixel(display, screen),
        WhitePixel(display, screen)
    );

    // --- IMPORTANT: Load font explicitly ---
    XFontStruct *font = XLoadQueryFont(display, "fixed");
    if (!font) font = XLoadQueryFont(display, "9x15");
    XSetFont(display, DefaultGC(display, screen), font->fid);

    XSelectInput(display, window, ExposureMask);
    XMapWindow(display, window);

    const char *text = "Paint It Black!";

    XEvent event;
    while (1) {
        XNextEvent(display, &event);

        // Draw only on final expose
        if (event.type == Expose && event.xexpose.count == 0) {
            DrawCenteredText(window, display, screen, text);
        }
    }

    return 0;
}

