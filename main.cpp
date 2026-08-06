#include <stdint.h>

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

enum PixelFormat {
  kPixelRGBResv8BitPerColor,
  kPixelBGRResv8BitPerColor,
};

struct FrameBufferConfig {
  uint8_t* frame_buffer;
  uint32_t pixels_per_scan_line;
  uint32_t horizontal_resolution;
  uint32_t vertical_resolution;
  enum PixelFormat pixel_format;
};

extern "C" void KernelMain(
    const FrameBufferConfig* config,
    const void*            memory_map,
    const void*            acpi_table,
    void*                  volume_image,
    void*                  runtime_services)
{
    serial_init();
    serial_write("Hello from my kernel!\n");

    uint8_t* fb = config->frame_buffer;
    for (uint32_t y = 0; y < config->vertical_resolution; ++y) {
        for (uint32_t x = 0; x < config->horizontal_resolution; ++x) {
            uint32_t offset = (y * config->pixels_per_scan_line + x) * 4;
            fb[offset + 0] = x * 255 / config->horizontal_resolution;   // B
            fb[offset + 1] = y * 255 / config->vertical_resolution;     // G
            fb[offset + 2] = 0;                                       // R
            fb[offset + 3] = 0;                                        // Reserved
        }
    }
    for (uint32_t y = 200; y < config->vertical_resolution; ++y) {
        for (uint32_t x = 100; x < 200; ++x) {
            uint32_t offset = (y * config->pixels_per_scan_line + x) * 4;
            fb[offset + 0] = x ;   // B
            fb[offset + 1] = y;     // G
            fb[offset + 2] = 0;                                       // R
            fb[offset + 3] = 0;                                        // Reserved
        }
    }
    while (1) __asm__("hlt");
}
