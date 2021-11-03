clear

# Build it and link it...
gcc main.c G10.c GXServer.c GXClient.c GXTerminal.c GXJSON.c GXCommand.c -o server -I include -g -lpthread

ss -K dst 172.24.100.230 dport = 8877
# ...then run gdb
gdb