#include "graphics.hpp"
#include "font.hpp"

char pixel_writer_buff[sizeof(RGBResv8bitPerColorPixelWriter)];
PixelWriter* pixel_writer;

void RGBResv8bitPerColorPixelWriter::Write(int x,int y,const PixelColor& c){
    auto p = PixelAt(x,y);
    p[0]=c.r;
    p[1]=c.g;
    p[2]=c.b;

}

void BGRResv8bitPerColorPixelWriter::Write(int x,int y,const PixelColor& c){
    auto p = PixelAt(x,y);
    p[0]=c.b;
    p[1]=c.g;
    p[2]=c.r;
}
void WriteAscii(PixelWriter& writer,int x,int y,char c,const PixelColor& color){
    const uint8_t* font = GetFont(c);
    if (font  == nullptr){return;}
    for (int dy =0; dy<16;++dy){
        for(int dx=0;dx<8;++dx){
            if((font[dy]<<dx)& 0x80u){writer.Write(x+dx,y+dy,color);}
        }
    }
}
