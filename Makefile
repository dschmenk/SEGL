BIN=vctest sgles1 svg1 svg2 svg3 svg4
OBJS=segl.o 

CFLAGS+=-I/opt/vc/include/ -I/opt/vc/include/interface/vcos/pthreads/ \
-I/opt/vc/include/interface/vmcs_host/linux -I/opt/vc/include/interface/vmcs_host/linux \
-DHAVE_LIBBCM_HOST -DUSE_EXTERNAL_LIBBCM_HOST -DUSE_VCHIQ_ARM
LDFLAGS+=-L/opt/vc/lib/ -lGLESv2 -lEGL -lbcm_host -lvcos -lvchiq_arm segl.o

all: $(OBJS) $(BIN)

clean:
	rm $(BIN) $(OBJS)
