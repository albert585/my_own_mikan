#include <stddef.h>
#include <stdio.h>
#include "font.hpp"
#include "frame_buffer_config.hpp"
#include "graphics.hpp"
void* operator new(size_t size, void* buf) noexcept { return buf; }
void* operator new[](size_t size, void* buf) noexcept { return buf; }
void operator delete(void*) noexcept {}
void operator delete[](void*) noexcept {}
void operator delete(void*, size_t) noexcept {}
void operator delete[](void*, size_t) noexcept {}


static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static void serial_init() {
    outb(0x3F8 + 1, 0x00);   // 关中断
    outb(0x3F8 + 3, 0x80);   // DLAB=1, 准备配波特率
    outb(0x3F8 + 0, 0x03);   // 115200 / 3 = 38400 低位
    outb(0x3F8 + 1, 0x00);   // 高位 = 0
    outb(0x3F8 + 3, 0x03);   // 8N1, DLAB=0
    outb(0x3F8 + 2, 0xC7);   // 开 FIFO, 清发送/接收
}

static void serial_putc(char c) {
    while (!(inb(0x3F8 + 5) & 0x20));  // 等发送器空
    outb(0x3F8, c);
    if (c == '\n') serial_putc('\r');
}

static void serial_write(const char* s) {
    while (*s) serial_putc(*s++);
}


extern "C" void KernelMain(
    const FrameBufferConfig* config,
    const void*            memory_map,
    const void*            acpi_table,
    void*                  volume_image,
    void*                  runtime_services)
{
    serial_init();
    serial_write("Hello from my kernel!\n");
    switch (config->pixel_format){
        case kPixelBGRResv8BitPerColor:
            pixel_writer = new(pixel_writer_buff)
                BGRResv8bitPerColorPixelWriter{*config};
            break;
        case kPixelRGBResv8BitPerColor:
            pixel_writer = new(pixel_writer_buff)
                RGBResv8bitPerColorPixelWriter{*config};
            break;
    }
    uint8_t* fb = config->frame_buffer;
    for (uint32_t y = 0; y < config->vertical_resolution; ++y) {
        for (uint32_t x = 0; x < config->horizontal_resolution; ++x) {
            pixel_writer->Write(x,y,{0,0,0});// Reserved
        }
    }
    int i=0;
    for(char c='!'; c <='~';++c,++i){WriteAscii(*pixel_writer,i*8,100,c,{255,255,255});}
    //WriteAscii(*pixel_writer,100,100,'A',{255,255,255});

    // for (uint32_t y = 200; y < config->vertical_resolution; ++y) {
    //     for (uint32_t x = 100; x < 200; ++x) {
    //         uint32_t offset = (y * config->pixels_per_scan_line + x) * 4;
    //         fb[offset + 0] = x ;   // B
    //         fb[offset + 1] = y;     // G
    //         fb[offset + 2] = 0;                                       // R
    //         fb[offset + 3] = 0;                                        // Reserved
    //     }
    // }
    char buf[128];
    sprintf(buf,"1+2=%d",1+2);
    WriteString(*pixel_writer,0,82,buf,{255,255,255});
    while (1) __asm__("hlt");
}
