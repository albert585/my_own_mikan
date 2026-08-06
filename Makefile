CXX      := clang++
CXXFLAGS := -O2 -Wall -Wextra -g --target=x86_64-elf -ffreestanding \
            -mno-red-zone -fno-exceptions -fno-rtti -std=c++17
LD       := ld.lld
LDFLAGS  := --entry KernelMain -z norelro --image-base 0x100000 --static

TARGET := kernel.elf
OBJS   := main.o

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
