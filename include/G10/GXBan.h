#include <time.h>

struct GXBan_s
{
    char           *name,
                   *reason;
    time_t          expiration;
    struct GXBan_s *next;
};
typedef struct GXBan_s GXBan_t;

// Allocators
GXBan_t *createBan     ( void );                   //

// Constructors
GXBan_t *loadBan       ( const char *path );  // 
GXBan_t *loadBanAsJSON ( char       *token ); //

// Finder
GXBan_t *findBan       ( char       *name );  //

// Remover
GXBan_t *removeBan     ( char       *name );  //

// Destructor
GXBan_t *destroyBan    ( GXBan_t    *ban);    // 