#ifndef USER_H
#define USER_H

class User
{

   public:

      User( void);

      void update( float dt);

      void move_forward( void );
      void move_backward( void );
      void move_upward( void );
      void move_downward( void );
      void move_right( void );
      void move_left( void );

      void stop_forward( void );
      void stop_right( void );
      void stop_left( void );
      void stop_up( void );
      void stop_down( void );

      void  get_direction( float *direction );
      void  get_position( float *position );
      float get_window_distance( void );
      float get_window_width( void );

      void set_direction( float *direction);

      void debug_info( void );

   private:

      float position[3];
      float direction[3];

      float forward_speed;
      float right_speed;
      float left_speed;
      float up_speed;
      float down_speed;

      float window_distance;
      float window_width;
      float window_height;
};

#endif
