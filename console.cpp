#include "console.hpp"
#include "graphics.hpp"
#include <string.h>

Console::Console(PixelWriter& writer, const PixelColor& fg_color, const PixelColor& bg_color)
    : writer_{writer}, fg_color_{fg_color}, bg_color_{bg_color},buffer_{}, cursor_row_{0}, cursor_col_{0} {}
void Console::PutString(const char* s) {
    for (int i = 0; s[i] != '\0'; ++i) {
        if (s[i] == '\n') {
            NewLine();
        } else if(cursor_col_<kCols-1){
            WriteAscii(writer_,8* cursor_col_,16*cursor_col_,*s,fg_color_);
            buffer_[cursor_row_][cursor_col_] = *s;
            ++cursor_col_;        
        }
        ++s;
    }
}
void Console::NewLine() {
    cursor_col_ = 0;
    if (cursor_row_ < kRows - 1) {
        ++cursor_row_;
    } else {
        for (int row = 1; row < kRows; ++row) {
            for (int col = 0; col < kCols; ++col){
                writer_.Write(row,col,bg_color_);
            }
        }
        for (int row=0;row<kRows-1;++row){
            memcpy(buffer_[row],buffer_[row+1],kCols+1);
            WriteString(writer_,0, 16*row, buffer_[row],fg_color_);
        }
        memset(buffer_[kRows-1],0,kCols+1);
    }
}