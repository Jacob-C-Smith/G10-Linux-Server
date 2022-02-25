clear

# Build it and link it...
gcc main.c G10.c GXServer.c GXPhysics.c GXClient.c GXTerminal.c GXJSON.c GXCommand.c -o server -I include -I G10\ source/include/G10\ client/ -g -lpthread

ss -K dst  172.27.162.87  dport = 8877
# ...then run gdb
gdb