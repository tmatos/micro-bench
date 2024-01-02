#
#  Configurar para compilação cruzada:
#
#  source /opt/yocto/poky/2.2/environment-setup-core2-32-poky-linux
#  ou
#  source /opt/yocto/poky/1.3.2/environment-setup-core2-poky-linux
#

app: conv.c
	$(CC) -o conv_cpu conv.c -O5 -Wall -Wpedantic

clean:
	rm -rf *.o conv_cpu
