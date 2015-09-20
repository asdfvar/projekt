/*
** Class NAME: Light_source
*/
class Light_source {

   public:

      Light_source( float position[3],
                    float intensity[3]);

     ~Light_source( void );

      float get_position( int index );
      float get_intensity( int index );

   private:

      float position[3];
      float intensity[3];

};
