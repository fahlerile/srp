#include "Renderer.h"
#include "Vector/Vector2.h"
#include "utils.h"
#include "Context.h"
#include "Line.h"

// See https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
void drawLineBresenham(Vector2i p1, Vector2i p2)
{
    Color color = {255, 255, 255, 255};

    if (p1.x > p2.x)
        SWAP(p1, p2);

    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;

    bool steep = abs(dy) > abs(dx);
    if (steep)
    {
        SWAP(dx, dy);
        SWAP(p1.x, p1.y);
        SWAP(p2.x, p2.y);
    }

    if (p1.x > p2.x)
    {
        SWAP(p1, p2);
        dx = -dx;
        dy = -dy;
    }

    int D = 2*dy - dx;
    int y = p1.y;
    int yIncrement = 1;
    if (dy < 0 || (dx < 0 && steep))
    {
        yIncrement = -1;
        dy = -dy;
    }

    for (int x = p1.x; x <= p2.x; x++)
    {
        rendererDrawPixel(
            context.renderer,
            (steep) ? (Vector2i) {y, x} : (Vector2i) {x, y},
            color
        );
        if (D > 0)
        {
            y += yIncrement;
            D -= 2*dx;
        }
        D += 2*dy;
    }
}

