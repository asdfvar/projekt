#include "simu_world.h"
#include "diagnostics.h"

Text g_text;
bool screen_feedback;

int main( int argc, char** argv)
{

   screen_feedback = false;

   simu_world( argc, argv);

}
