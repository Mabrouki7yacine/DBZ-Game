#ifndef BUTTON_H
#define BUTTON_H

#include "raylib.h"

Color ButtonBorder = { 20, 20, 20, 255 };      // Darker outline for contrast
Color ButtonFill   = { 200, 130, 30, 255 };    // Warm orange-gold (ties in with DBZ/Dragon Balls)
Color ButtonHover  = { 255, 170, 50, 255 };    // Bright hover state
Color TextColor    = { 10, 10, 10, 255 };      // Deep, visible black for text

// Draw a pixel-perfect rectangle with border
void DrawPixelRect(int x, int y, int width, int height, Color fill, Color border, int borderWidth = 2) {
    // Draw border
    DrawRectangle(x, y, width, height, border);
    // Draw inner fill
    DrawRectangle(x + borderWidth, y + borderWidth, width - borderWidth * 2, height - borderWidth * 2, fill);
}

// Draw pixel-style text with shadow
void DrawPixelText(const char* text, int x, int y, int fontSize, Color color, bool withShadow = true) {
    if (withShadow) {
        DrawText(text, x + 2, y + 2, fontSize, BLACK);
    }
    DrawText(text, x, y, fontSize, color);
}

bool DrawPixelButton(Rectangle bounds, const char* text, bool& isHovered) {
    Vector2 mouse = GetMousePosition();
    isHovered = CheckCollisionPointRec(mouse, bounds);
    bool clicked = isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    
    Color fillColor = isHovered ? ButtonHover : ButtonFill;
    
    // Draw button with pixel border
    DrawPixelRect(bounds.x, bounds.y, bounds.width, bounds.height, fillColor, ButtonBorder, 5);
    
    // Center text
    int textWidth = MeasureText(text, 20);
    int textX = bounds.x + (bounds.width - textWidth) / 2;
    int textY = bounds.y + (bounds.height - 20) / 2;
    
    DrawPixelText(text, textX, textY, 20, TextColor, true);
    
    return clicked;
}



#endif