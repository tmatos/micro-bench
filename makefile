#
#  Configurar para compilação cruzada:
#
#  source /opt/yocto/poky/2.2/environment-setup-core2-32-poky-linux
#  ou
#  source /opt/yocto/poky/1.3.2/environment-setup-core2-poky-linux
#

TARGET = conv_cpu

all: conv.c
	$(CC) -o $(TARGET) $^ -O2 -Wall -Wpedantic

.PHONY: clean
clean:
	rm -rf *.o $(TARGET)
