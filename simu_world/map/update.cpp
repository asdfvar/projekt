#include "map.h"
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

   // write all queued chunks to disk
   queue->write_all();

   // for each queued chunk to read, read in chunk data
   // if chunk file doesn't exist, create the chunk
   // this logic currently exists in shift.cpp.

}
