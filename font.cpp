#include "font.hpp"
#include "graphics.hpp"

extern const uint8_t _binary_hankaku_bin_start;
extern const uint8_t _binary_hankaku_bin_size;

const uint8_t* GetFont(char c) {
    auto index = 16 * static_cast<unsigned int>(c);
    if (index >= reinterpret_cast<uintptr_t>(&_binary_hankaku_bin_size)) {
        return nullptr;
    }
    return &_binary_hankaku_bin_start + index;
}
void WriteString(PixelWriter&writer,int x,int y,const char *s,const PixelColor& color){
    for(int i=0;s[i]!='\0';++i){
        WriteAscii(writer,x+8*i,y,s[i],color);
    }
}