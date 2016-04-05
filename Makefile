CC = gcc
RM = /bin/rm
STD = -std=c99 -Wall
OS = $(shell uname)

ifeq ($(OS), Darwin)
    SSL_INCLUDE_DIR = -I/Applications/XAMPP/xamppfiles/include
    SSL_LIB_DIR     = -L/Applications/XAMPP/xamppfiles/lib

    LUAC            = /usr/local/bin/luac
    LUA_INCLUDE_DIR = -I/usr/local/include
    LUA_LIB_DIR     = -L/usr/local/lib
else
    SSL_INCLUDE_DIR = -I/usr/local/openssl/include
    SSL_LIB_DIR     = -L/usr/local/openssl/lib

    LUAC            = /usr/local/bin/luac
    LUA_INCLUDE_DIR = -I/usr/local/include
    LUA_LIB_DIR     = -L/usr/local/lib
endif

SSL_LIBS  = -lssl -lcrypto
LUA_LIBS  = -llua
FUNC_LUA  = ./lua/func.lua
FUNC_LUAC = ./lua/func.luac

C_LIB_DIR = $(SSL_LIB_DIR) $(LUA_LIB_DIR)
C_LIBS    = $(SSL_LIBS) $(LUA_LIBS)

objects = main.o apns_conn.o config.o apns_type.o kmalloc.o

kpush : $(objects)
	$(CC) $(STD) -o kpush $(objects) $(C_LIB_DIR) $(C_LIBS)

main.o : main.c
	$(CC) $(STD) -c main.c $(SSL_INCLUDE_DIR) $(LUA_INCLUDE_DIR)

kmalloc.o : kmalloc.c
	$(CC) $(STD) -c kmalloc.c

apns_type.o : apns_type.c
	$(CC) $(STD) -c apns_type.c $(SSL_INCLUDE_DIR)

apns_conn.o : apns_conn.c
	$(CC) $(STD) -c apns_conn.c $(SSL_INCLUDE_DIR)

config.o : config.c $(FUNC_LUAC)
	$(CC) $(STD) -c config.c $(LUA_INCLUDE_DIR)

$(FUNC_LUAC) : $(FUNC_LUA)
	$(LUAC) -o $(FUNC_LUAC) $(FUNC_LUA)

.PHONY : clean clean_objects
clean :
	-$(RM) kpush $(objects) ./lua/func.luac
clean_objects :
	-$(RM) $(objects)
