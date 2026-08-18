#pragma once
#include "frame_buffer_config.hpp"
struct PixelColor{
    uint32_t r,g,b;
};
class PixelWriter{
    public:
        PixelWriter (const FrameBufferConfig& config): config_{config}{}
        virtual ~PixelWriter() = default;
        virtual void Write(int x,int y ,const PixelColor &c)=0;
    
    protected:
        uint8_t* PixelAt(int x,int y){
            return config_.frame_buffer +4*(config_.pixels_per_scan_line*y+x);
        }
    private:
        const FrameBufferConfig& config_;
    };
    class RGBResv8bitPerColorPixelWriter : public PixelWriter{
        public:
            using PixelWriter::PixelWriter;
    
            virtual void Write(int x,int y,const PixelColor& c) override;
    };
    class BGRResv8bitPerColorPixelWriter : public PixelWriter{
    public:
        using PixelWriter::PixelWriter;
    
        virtual void Write(int x,int y,const PixelColor& c) override;
    };
    extern char pixel_writer_buff[sizeof(RGBResv8bitPerColorPixelWriter)];
    extern PixelWriter* pixel_writer;
    void WriteAscii(PixelWriter& writer,int x,int y,char c,const PixelColor& color);

extern void WriteString(PixelWriter&writer,int x,int y,const char *s,const PixelColor& color);