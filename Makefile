LUA= /usr
LUAINC= $(LUA)/include/lua5.1
LUALIB= $(LUA)/lib
LUABIN= $(LUA)/bin

CFLAGS= $(INCS) $(DEFS) $(WARN) -O2 -fPIC
WARN= -Wall
INCS= -I$(LUAINC)

OBJS= lua_date.o

SOS= lua-date.so

all: $(SOS)

$(SOS): $(OBJS)
	$(CC) -o $@ -shared $(OBJS) $(LIBS)

clean:
	rm -f $(OBJS) $(SOS)
