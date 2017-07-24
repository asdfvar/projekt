#include "map.h"
#include "chunk.h"
#include "fileio.h"
#include "queue.h"
#include <sstream>

/*
** function name: update from: Map
** Update chunks from the IO thread
**
*/
void Map::update( void )
{

/*
** note to self: this may be disabled during
** development to mitigate excessive writes to the SSD
*/
//   queue->write_all();

   // read in chunk data
   // if file doesn't exist, create it

}
