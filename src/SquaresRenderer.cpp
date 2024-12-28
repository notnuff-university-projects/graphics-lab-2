#include "SquaresRenderer.h"

SquaresRenderer::SquaresRenderer() {
    lines_renderer_.Init();
}

void SquaresRenderer::DrawSquare(const Point &p1, const Point &p2) {
    lines_renderer_.DrawLine(p1.x, p1.y, p2.x, p1.y);
    lines_renderer_.DrawLine(p2.x, p1.y, p2.x, p2.y);
    lines_renderer_.DrawLine(p2.x, p2.y, p2.x, p1.y);
    lines_renderer_.DrawLine(p2.x, p1.y, p1.x, p1.y);
}

void SquaresRenderer::DrawSquare(const Square &sq) {
    for(int i = 0; i < 4; ++i) {
        auto iNext = (i + 1) % 4;
        lines_renderer_.DrawLine(sq.points[i].x, sq.points[i].y, sq.points[iNext].x, sq.points[iNext].y);
    }
}

void SquaresRenderer::DrawTriangle(const Triangle &sq) {
    for(int i = 0; i < 3; ++i) {
        auto iNext = (i + 1) % 3;
        lines_renderer_.DrawLine(sq.points[i].x, sq.points[i].y, sq.points[iNext].x, sq.points[iNext].y);
    }
}


