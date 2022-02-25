#include <G10/GXPhysics.h>

void tick_thread ( GXServer_t *server, size_t ticks_per_sec )
{
    while(server->running)  
        if((clock() % server->tick_rate ) == 0)
            server->ticks++;
}