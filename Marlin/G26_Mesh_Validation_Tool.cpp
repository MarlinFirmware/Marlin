/*
 * Marlin Firmware -- G26
 */

#include "Marlin.h"
#include "Configuration.h"
#include "G29_Unified_Bed_Leveling.h"

#ifdef UNIFIED_BED_LEVELING_FEATURE 

#define SIZE_OF_INTERSECTION_CIRCLES	5


// Roxy's G26 Mesh Validation Tool
//
// G26 Is a Mesh Validation Tool intended to provide support for the Marlin Unified Bed Leveling System.
// In order to fully utilize and benefit from the Marlin Unified Bed Leveling System an accurate Mesh must
// be defined.  G29 is designed to allow the user to quickly validate the correctness of her Mesh.  It will
// first heat the bed and nozzle. It will then print lines and circles along the Mesh Cell boundaries and
// the intersections of those lines (respectively).
//
// This action allows the user to immediately see where the Mesh is properly defined and where it needs to 
// be edited.   The command will generate the Mesh lines closest to the nozzle's starting position.  Alternatively
// the user can specify the X and Y position of interest with command parameters.  This allows the user to
// focus on a particular area of the Mesh where attention is needed.
//
// B #	Bed		Set the Bed Temperature.  If not specified, a default of 60 C. will be assumed.
//
// C    Current 	When searching for Mesh Intersection points to draw, use the current nozzle location
// 			as the base for any distance comparison.  
//
// D    Disable		Disable the Unified Bed Leveling System.  In the normal case the user is invoking this
// 			command to see how well a Mesh as been adjusted to match a print surface.  In order to do
// 			this the Unified Bed Leveling System is turned on by the G26 command.   The D parameter
// 			alters the command's normal behaviour and disables the Unified Bed Leveling System even if
// 			it is on. 
//
// H #	Hotend 		Set the Nozzle Temperature.  If not specified, a default of 205 C. will be assumed.
// 
// F #  Filament	Used to specify the diameter of the filament being used.  If not specified
// 			1.75mm filament is assumed.  If you are not getting acceptable results by using the
// 			'correct' numbers, you can scale this number up or down a little bit to change the amount
// 			of filament that is being extruded during the printing of the various lines on the bed.
//
// K    Keep-On		Keep the heaters turned on at the end of the command.
//
// L #  Layer		Layer height.  (Height of nozzle above bed)  If not specified .20mm will be used.
//
// N #  Nozzle		Used to control the size of nozzle diameter.  If not specified, a .4mm nozzle is assumed.
//
// O #  Ooooze		How much your nozzle will Ooooze filament while getting in position to print.  This
// 			is over kill, but using this parameter will let you get the very first 'cicle' perfect 
// 			so you have a trophy to peel off of the bed and hang up to show how perfectly you have your 
// 			Mesh calibrated.  If not specified, a filament length of .3mm is assumed.
//
// P #  Prime		Prime the nozzle with specified length of filament.  If this parameter is not 
// 			given, no prime action will take place.   If the parameter specifies an amount, that much
// 			will be purged before continuing.   If no amount is specified the command will start
// 			purging filament until the user provides an LCD Click and then it will continue with
// 			printing the Mesh.  You can carefully remove the spent filament with a needle nose 
// 			pliers while holding the LCD Click wheel in a depressed state. 
//
// R #  Random		Randomize the order that the circles are drawn on the bed.  The search for the closest 
// 			undrawn cicle is still done.  But the distance to the location for each circle has a 
// 			random number of the size specified added to it.  Specifying R50 will give an interesting
// 			deviation from the normal behaviour on a 10 x 10 Mesh.
//
// X #	X coordinate	Specify the starting location of the drawing activity.
//
// Y #	Y coordinate	Specify the starting location of the drawing activity.
//

extern int UBL_has_control_of_LCD_Panel;
extern float feedrate;
extern bool relative_mode;
extern Planner planner;
void set_current_to_destination();
void set_destination_to_current();
bool prepare_move_to_destination_cartesian();
void line_to_destination();
void line_to_destination(float );
void gcode_G28();
void sync_plan_position_e();
void un_retract_filament();
void retract_filament();
bool direction_is_on_bed( float , float , int );
void look_for_lines_to_connect();
void bit_clear( unsigned int bits[16], int , int );
void bit_set( unsigned int bits[16], int , int );
bool is_bit_set( unsigned int bits[16], int , int );
bool parse_G26_parameters();
void move_to( float, float, float, float);
void print_line_from_here_to_there( float sx, float sy, float sz, float ex, float ey, float ez );
bool turn_on_heaters();
void prime_nozzle();

static unsigned circle_flags[16], horizontal_mesh_line_flags[16], vertical_mesh_line_flags[16], Continue_with_closest=0;
static float G26_E_AXIS_feedrate = 0.035;
static float Random_Deviation = 0.0, Layer_Height=.20;

#if ENABLED(ULTRA_LCD)
void lcd_setstatus(const char* message, bool persist);
#endif

float valid_trig_angle( float );
struct mesh_index_pair find_closest_circle_to_print( float, float );	
void mesh_buffer_line(float, float, float, const float, float, const uint8_t& ,// uint16_t , uint16_t );
		uint16_t x_splits = 0xffff, uint16_t y_splits = 0xffff);
static float E_Pos_Delta, Filament_Factor=1.0;
static float Nozzle=0.4, Filament=1.75, Prime_Length=10.0;
static float X_Pos, Y_Pos, bed_temp=60.0, hotend_temp=205.0, Ooooze_Amount=0.3;
static int Prime_Flag=0, Keep_Heaters_On=0; 

void gcode_G26() {
float circle_x, circle_y, x, y, dx, dy, xe, ye, tmp;
float E_Pos=0.0;
int   i, j, k, xi, yi; 
struct mesh_index_pair location;

  if ( axis_unhomed_error(true, true, true) )	 // Don't allow Mesh Validation without homing first
    gcode_G28();

  if ( parse_G26_parameters() )			// If the paramter parsing did not go OK, we abort the command
	  return;

  if ( current_position[Z_AXIS] < Z_RAISE_BETWEEN_PROBINGS )  {
  	do_blocking_move_to_z( Z_RAISE_BETWEEN_PROBINGS);
	stepper.synchronize();
	set_current_to_destination();
  }

  if ( turn_on_heaters() )			// Turn on the heaters, leave the command if anything
	  goto LEAVE;				// has gone wrong.

  axis_relative_modes[E_AXIS] = false;		// Get things setup so we can take control of the
  relative_mode = false;			// planner and stepper motors!
  current_position[E_AXIS] = 0.0;
  sync_plan_position_e();

  if (Prime_Flag)  
	prime_nozzle();
  

//
//			Bed is preheated
//
// 			Nozzle is at temperature
//
// 			Filament is primed!   
//
// 			It's  "Show Time" !!!
//


//
// Clear all of the flags we need
//
  for(i=0; i<16; i++) {		
	circle_flags[i]			= 0;	
	horizontal_mesh_line_flags[i]	= 0;
	vertical_mesh_line_flags[i]	= 0;
  }

//
// Move nozzle to the specified height for the first layer
//
  set_destination_to_current();
  destination[Z_AXIS] = Layer_Height;
  move_to( destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], 0.0 );
  move_to( destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], Ooooze_Amount );

// We are by-passing the G1 command where a check is made to see if the Z-Axis is changing.
// For efficiency, that is where we set the fade_scaling_factor_for_current_height variable. 
// We need to duplicate that calculation here to keep everything consistent.

 if (destination[Z_AXIS] > bed_leveling_mesh.state.G29_Correction_Fade_Height )  {
 	fade_scaling_factor_for_current_height = 0.0;
 } else {
 	fade_scaling_factor_for_current_height = 1.0 - (destination[Z_AXIS] / bed_leveling_mesh.state.G29_Correction_Fade_Height);
 }
	   

  do {
	if ( G29_lcd_clicked() ) {					// Check if the user wants to stop the Mesh Validation
		strcpy( lcd_status_message, "Mesh Validation Stopped.");// We can't do lcd_setstatus() without having it continue;
		while ( G29_lcd_clicked() )				// Debounce the switch click
	       		idle();	
	        #if ENABLED(ULTRA_LCD)
                  lcd_setstatus( "Mesh Validation Stopped.", true);
	          lcd_quick_feedback();
                #endif
		goto LEAVE;
	}
	
	if ( Continue_with_closest ) {
	   location = find_closest_circle_to_print( current_position[X_AXIS], current_position[Y_AXIS]);
	} else
	   location = find_closest_circle_to_print( X_Pos, Y_Pos );	// Find the closest Mesh Intersection to where
									// we are now.
    	if (location.x_index>=0 && location.y_index>=0 ) {
		circle_x = bed_leveling_mesh.map_x_index_to_bed_location(location.x_index); 
		circle_y = bed_leveling_mesh.map_y_index_to_bed_location(location.y_index);

// Let's do a couple of quick sanity checks.  We can pull this code out later if we never see it catch a problem

#ifdef DELTA
		if ( sqrt( circle_x*circle_x + circle_y*circle_y) > DELTA_PRINTABLE_RADIUS) {
    			SERIAL_PROTOCOLLNPGM("?Error: Attempt to print outside of DELTA_PRINTABLE_RADIUS.");
			goto LEAVE;
		}
#endif
		if ( circle_x<X_MIN_POS || circle_x>X_MAX_POS || circle_y<Y_MIN_POS || circle_y>Y_MAX_POS)  {
    			SERIAL_PROTOCOLLNPGM("?Error: Attempt to print off the bed.");
			goto LEAVE;
		}

		xi = location.x_index; 	// Just to shrink the next few lines and make them easier to understand	
		yi = location.y_index; 

		if ( xi>0 && xi<MESH_NUM_X_POINTS-1 && yi>0 && yi<MESH_NUM_Y_POINTS-1) {  // Do the full circle!
			k = 0;
			j = 9;
		} else {
			k = 0;
			while ( !direction_is_on_bed(circle_x, circle_y, k)) 	// Make sure k's angle index is on the 
				k++;						// bed.  If it isn't, scan forward
										// for a place it is on the bed.

			while ( direction_is_on_bed(circle_x, circle_y, k) )	// Find where k's angle index goes off the bed.
				k++;
			j = k--;						// j will be the ending angle index

			while (direction_is_on_bed(circle_x, circle_y, k) )	// Now scan backwards to find the max arc 
				k--;						// length that is on the bed
			k++;							// K is the starting angle index

			if ( xi==MESH_NUM_X_POINTS-1 || yi==MESH_NUM_Y_POINTS-1)// The algorythm up above will over shoot
				j -= 1;						// by one if we are on the max side.
		}

		for( i=k; i<j; i++) {
			x = circle_x + SIZE_OF_INTERSECTION_CIRCLES * cos( RADIANS( valid_trig_angle( ((float) i) * 360.0/8.0 ) ) );
			y = circle_y + SIZE_OF_INTERSECTION_CIRCLES * sin( RADIANS( valid_trig_angle( ((float) i) * 360.0/8.0 ) ) );
			xe = circle_x + SIZE_OF_INTERSECTION_CIRCLES * cos( RADIANS( valid_trig_angle( ((float) i+1) * 360.0/8.0 ) ) );
			ye = circle_y + SIZE_OF_INTERSECTION_CIRCLES * sin( RADIANS( valid_trig_angle( ((float) i+1) * 360.0/8.0 ) ) );
#ifdef DELTA
			if ( sqrt( x*x + y*y) > DELTA_PRINTABLE_RADIUS) 	// Check to make sure this part of
				continue;					// the 'circle' is on the bed.  If
#else										// not, we need to skip

			x = constrain( x, X_MIN_POS+1, X_MAX_POS-1); 	// This keeps us from bumping the endstops	
			y = constrain( y, Y_MIN_POS+1, Y_MAX_POS-1); 			
			xe= constrain( xe, X_MIN_POS+1, X_MAX_POS-1); 			
			ye= constrain( ye, Y_MIN_POS+1, Y_MAX_POS-1); 			
#endif
			print_line_from_here_to_there( x, y, Layer_Height, xe, ye, Layer_Height);
		}
		look_for_lines_to_connect();
	}

  } while (location.x_index>=0 && location.y_index>=0 );

LEAVE:
  retract_filament();
  move_to( destination[X_AXIS], destination[Y_AXIS], Z_RAISE_BETWEEN_PROBINGS, 0);	// Raise the nozzle
  move_to( X_Pos, Y_Pos, Z_RAISE_BETWEEN_PROBINGS, 0);	// Move back to the starting position
  UBL_has_control_of_LCD_Panel = 0;			// Give back control of the LCD Panel!

  if ( !Keep_Heaters_On) {
  	thermalManager.setTargetBed( 0.0 );
	thermalManager.setTargetHotend( 0.0 , 0 );
  }
}

bool direction_is_on_bed( float x, float y, int i ) {
float x1, y1;
	
	x1 = x + cos( RADIANS( valid_trig_angle( ((float) i) * 360.0/8.0 ) ) );	// move off of the point in the specified direction
	y1 = y + sin( RADIANS( valid_trig_angle( ((float) i) * 360.0/8.0 ) ) );

	if ( x1<X_MIN_POS || x1>X_MAX_POS || y1<Y_MIN_POS || y1>Y_MAX_POS) 
		return false;
	
	return true;
}

float valid_trig_angle( float d ) {
	while ( d > 360.0 )
		d -= 360.0;

	while ( d < 0.0 )
		d += 360.0;
	return d;
}

struct mesh_index_pair find_closest_circle_to_print( float X, float Y )	
{
  int i, j;
  float f, mx, my, dx, dy, closest=99999.99;
  struct mesh_index_pair return_val;

	return_val.x_index = -1;
	return_val.y_index = -1;

	for(i=0; i<MESH_NUM_X_POINTS; i++) {
		for(j=0; j<MESH_NUM_Y_POINTS; j++) {
			if ( !is_bit_set( circle_flags, i, j) )  {
				mx = bed_leveling_mesh.map_x_index_to_bed_location(i);	// We found a circle that needs to be printed
				my = bed_leveling_mesh.map_y_index_to_bed_location(j);

				dx = X - mx;				// Get the distance to this intersection
				dy = Y - my;	
				f = sqrt(dx*dx+dy*dy);

				dx = X_Pos - mx; 			// It is possible that we are being called with the values 
				dy = Y_Pos - my; 			// to let us find the closest circle to the start position.
				f += sqrt(dx*dx+dy*dy) / 15.0;		// But if this is not the case, we are going to add in a small
									// weighting to the distance calculation to help it choose
									// a better place to continue.  

				if (Random_Deviation > 1.0)
					f +=random(0.0,Random_Deviation);// Add in the specified amount of Random Noise to our search

				if ( f < closest ) {	
					closest = f;			// We found a closer location that is still
					return_val.x_index = i;		// un-printed  --- save the data for it
					return_val.y_index = j;	
					return_val.distance= closest;	
				}
			}
		}
	}
	bit_set( circle_flags, return_val.x_index, return_val.y_index);		// Mark this location as done.
	return return_val;
}


// These support functions allow the use of large bit arrays of flags that take very 
// little RAM. Currently they are limited to being 16x16 in size.  Changing the declaration 
// to unsigned long will allow us to go to 32x32 if higher resolution Mesh's are needed 
// in the future.

void bit_clear( unsigned int bits[16], int x, int y)
{
unsigned mask;

	mask = 0x1 << x;
	bits[y] |= mask;	// set the bit and then
	bits[y] ^= mask;	// use XOR to invert the bit
}

void bit_set( unsigned int bits[16], int x, int y)
{
unsigned mask;

	mask = 0x1 << x;
	bits[y] |= mask;	// set the bit 
}

bool is_bit_set( unsigned int bits[16], int x, int y)
{
unsigned mask;

	mask = 0x1 << x;
	return (bool) (bits[y] & mask);	// return the specified bit's value
}



void look_for_lines_to_connect()
{
float sx, sy, ex, ey, dx, dy;
int i, j, k;

	for(i=0; i<MESH_NUM_X_POINTS; i++ ) {
		for(j=0; j<MESH_NUM_Y_POINTS; j++ ) {
			
			if ( i < MESH_NUM_X_POINTS)	// We can't connect to anything to the right than MESH_NUM_X_POINTS.  
							// This is already a half circle because we are at the edge of the bed.
							
			if ( is_bit_set( circle_flags, i, j) && is_bit_set( circle_flags, i+1, j) ) {	// check if we can do a line to the left
				if ( !is_bit_set( horizontal_mesh_line_flags, i, j) )  {

//
// We found two circles that need a horizontal line to connect them
// Print it!
//
					sx = bed_leveling_mesh.map_x_index_to_bed_location(i);	
					sx = sx + SIZE_OF_INTERSECTION_CIRCLES;			// get the right edge of the circle
					sy = bed_leveling_mesh.map_y_index_to_bed_location(j);

					ex = bed_leveling_mesh.map_x_index_to_bed_location(i+1);	
					ex = ex - SIZE_OF_INTERSECTION_CIRCLES;			// get the left edge of the circle
					ey = sy;

					sx= constrain( sx, X_MIN_POS+1, X_MAX_POS-1); 	// This keeps us from bumping the endstops	
					sy= constrain( sy, Y_MIN_POS+1, Y_MAX_POS-1); 			
					ex= constrain( ex, X_MIN_POS+1, X_MAX_POS-1); 			
					ey= constrain( ey, Y_MIN_POS+1, Y_MAX_POS-1); 			

					print_line_from_here_to_there( sx, sy, Layer_Height, ex, ey, Layer_Height );
					bit_set( horizontal_mesh_line_flags, i, j);		// Mark it as done so we don't do it again
				}
			}


			if ( j < MESH_NUM_Y_POINTS)	// We can't connect to anything further back than MESH_NUM_Y_POINTS.  
							// This is already a half circle because we are at the edge  of the bed.
							
			if ( is_bit_set( circle_flags, i, j) && is_bit_set( circle_flags, i, j+1) ) {	// check if we can do a line straight down
				if ( !is_bit_set( vertical_mesh_line_flags, i, j) )  {
//
// We found two circles that need a vertical line to connect them
// Print it!
					sx = bed_leveling_mesh.map_x_index_to_bed_location(i);	
					sy = bed_leveling_mesh.map_y_index_to_bed_location(j);
					sy = sy + SIZE_OF_INTERSECTION_CIRCLES;			// get the top edge of the circle

					ex = sx;
					ey = bed_leveling_mesh.map_y_index_to_bed_location(j+1); 
					ey = ey - SIZE_OF_INTERSECTION_CIRCLES;			// get the bottom edge of the circle

					sx= constrain( sx, X_MIN_POS+1, X_MAX_POS-1); 	// This keeps us from bumping the endstops	
					sy= constrain( sy, Y_MIN_POS+1, Y_MAX_POS-1); 			
					ex= constrain( ex, X_MIN_POS+1, X_MAX_POS-1); 			
					ey= constrain( ey, Y_MIN_POS+1, Y_MAX_POS-1); 			

					print_line_from_here_to_there( sx, sy, Layer_Height, ex, ey, Layer_Height );
					bit_set( vertical_mesh_line_flags, i, j);		// Mark it as done so we don't do it again
				}
			}
		}
	}
}

void debug_current_and_destination(char *title) {
SERIAL_ECHO("    current=( ");
SERIAL_ECHO_F( current_position[X_AXIS], 6 );
SERIAL_ECHO(" , ");
SERIAL_ECHO_F( current_position[Y_AXIS], 6 );
SERIAL_ECHO(" , ");
SERIAL_ECHO_F( current_position[Z_AXIS], 6 );
SERIAL_ECHO(" , ");
SERIAL_ECHO_F( current_position[E_AXIS], 6 );
SERIAL_ECHO(" )   destination=( ");
SERIAL_ECHO_F( destination[X_AXIS], 6 );
SERIAL_ECHO(" , ");
SERIAL_ECHO_F( destination[Y_AXIS], 6 );
SERIAL_ECHO(" , ");
SERIAL_ECHO_F( destination[Z_AXIS], 6 );
SERIAL_ECHO(" , ");
SERIAL_ECHO_F( destination[E_AXIS], 6 );
SERIAL_ECHO(" )   ");
SERIAL_ECHO( title );
SERIAL_ECHO("  \n");
}



void move_to( float x, float y, float z, float e_delta) {
float feed_value;
static float last_z =-9999.99;

   if ( z != last_z )  {
	last_z = z;
        fade_scaling_factor_for_current_height = 1.0 - (z / bed_leveling_mesh.state.G29_Correction_Fade_Height);
	feed_value = planner.max_feedrate[Z_AXIS]/(3.0);	// Base the feed rate off of the configured Z_AXIS feed rate

	destination[Z_AXIS] = z;
	
	mesh_buffer_line( destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feed_value, 0, 0xffff, 0xffff );
	stepper.synchronize();
  	set_destination_to_current();
   }

  if ( x!=destination[X_AXIS] || y!=destination[Y_AXIS])  {	// Check if X or Y is involved in the movement.
	feed_value =PLANNER_XY_FEEDRATE()/(10.0);		// Yes!  It is a 'normal' movement
  } else  {
	feed_value = planner.max_feedrate[E_AXIS]/(1.5);	// it is just a retract() or un_retract()
  }

  destination[X_AXIS] = x;
  destination[Y_AXIS] = y;
  destination[E_AXIS] += e_delta;

  mesh_buffer_line( destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feed_value, 0, 0xffff, 0xffff );

  stepper.synchronize();
  set_destination_to_current();
}

static bool retracted=false;	// We keep track of the state of the nozzle to know if it
				// is currently retracted or not.  This allows us to be
				// less careful because mis-matched retractions and un-retractions
				// won't leave us in a bad state.

void retract_filament()
{
  if ( !retracted ) {		// Only retract if we are not already retracted!
	retracted = true;
	move_to( destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], -1.0 );
  }
}

void un_retract_filament()
{
  if ( retracted ) {		// Only un-retract if we are retracted.
	move_to( destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], +1.2 );
	retracted = false;
  }
}


// print_line_from_here_to_there() takes two cartesian coordinates and draws a line from one
// to the other.  But there are really three sets of coordinates involved.  The first coordinate
// is the present location of the nozzle.   We don't necessarily want to print from this location.
// We first need to move the nozzle to the start of line segment where we want to print.  Once
// there, we can use the two coordinates supplied to draw the line.
//
// Note:  Although we assume the first set of coordinates is the start of the line and the second
// set of coordinates is the end of the line, it does not always work out that way.  This function
// optimizes the movement to minimize the travel distance before it can start printing.  This saves
// a lot of time and eleminates a lot of non-sensical movement of the nozzle.   However, it does 
// cause a lot of very little short retracement of th nozzle when it draws the very first line 
// segment of a 'circle'.   The time this requires is very short and is easily saved by the other
// cases where the optimization comes into play.   


void print_line_from_here_to_there( float sx, float sy, float sz, float ex, float ey, float ez ) {
     float dx, dy, dx_s, dy_s, dx_e, dy_e, dist_start, dist_end, E_Pos_Deltai, Line_Length;

	dx_s = destination[X_AXIS] - sx;		// find our distance from the start of the actual line segment
	dy_s = destination[Y_AXIS] - sy;
	dist_start = sqrt( dx_s*dx_s + dy_s*dy_s);

	dx_e = destination[X_AXIS] - ex;		// find our distance from the end of the actual line segment
	dy_e = destination[Y_AXIS] - ey;
	dist_end = sqrt( dx_e*dx_e + dy_e*dy_e);

     	dx = ex - sx;
	dy = ey - sy;
	Line_Length = sqrt(dx*dx+dy*dy);
				

	// If the end point of the line is closer to the nozzle, we are going to
	// flip the direction of this line.   We will print it from the end to the start.
	// On very small lines we don't do the optimization because it just isn't worth it.
	//
	if ( (dist_end < dist_start) && ( SIZE_OF_INTERSECTION_CIRCLES < abs( Line_Length )) )	{	
		print_line_from_here_to_there( ex, ey, ez, sx, sy, sz );
		return;
	}

	// Now decide if we should retract.

	if ( dist_start > 2.0 )  {
		retract_filament();
	}

	move_to( sx, sy, sz, 0.0 );	// Get to the starting point with no extrusion

     	dx = ex - sx;
	dy = ey - sy;

	E_Pos_Delta = Line_Length * G26_E_AXIS_feedrate * Filament_Factor;

	un_retract_filament();
	move_to( ex, ey, ez, E_Pos_Delta);	// Get to the ending point with an appropriate amount of extrusion
}


// The parse_G26_Parameters() function used to be inline code for the function.   But 
// there are so many parameters, it made sense to turn the paramter variables into
// static globals and get this code out of site from the main routine.    

bool parse_G26_parameters() {

   Filament_Factor=1.0;
   Nozzle=0.4; 
   Filament=1.75; 
   Prime_Length=10.0;
   bed_temp=60.0; 
   hotend_temp=205.0;
   Ooooze_Amount=0.3;
   Prime_Flag=0;
   Keep_Heaters_On=0; 
   X_Pos = current_position[X_AXIS];
   Y_Pos = current_position[Y_AXIS];
	
  if (code_seen('B')) {
	bed_temp = code_value_float();
	if ( bed_temp<15.0 || bed_temp>140.0 ) {
        	SERIAL_PROTOCOLPGM("?Specified bed temperature not plausable.\n");
		return true;
	}
  }

  if (code_seen('C')) 
	  Continue_with_closest++;

  if (code_seen('L')) {
	Layer_Height = code_value_float();
	if ( Layer_Height<0.0 || Layer_Height>2.0 ) {
        	SERIAL_PROTOCOLPGM("?Specified layer height not plausable.\n");
		return true;
	}
  }

  if (code_seen('N')) {
	Nozzle = code_value_float();
	if ( Nozzle<.1 || Nozzle>1.0 ) {
        	SERIAL_PROTOCOLPGM("?Specified nozzle size not plausable.\n");
		return true;
	}
  }

  if (code_seen('K')) 
	Keep_Heaters_On++;
	
  if (code_seen('O'))  {
	if ( code_has_value() ) 
		Ooooze_Amount = code_value_float();
  }

  if (code_seen('P')) {
	Prime_Flag++;
	if ( !code_has_value() ) 
		Prime_Flag = -1;
	else {
		Prime_Length = code_value_float();
		if ( Prime_Length<0.0 || Prime_Length>25.0 ) {
			SERIAL_PROTOCOLPGM("?Specified prime length not plausable.\n");
			return true;
		}
	}
  }

  if (code_seen('F')) {
	Filament = code_value_float();
	if ( Filament<1.0 || Filament>4.0 ) {
        	SERIAL_PROTOCOLPGM("?Specified filament size not plausable.\n");
		return true;
	}
  }
  Filament_Factor = 1.75*1.75 / (Filament*Filament);	// If we aren't using 1.75mm filament, we need to
  							// scale up or down the length needed to get the
							// same volume of filament

  if (code_seen('H')) {
	hotend_temp = code_value_float();
	if ( hotend_temp<165.0 || hotend_temp>280.0 ) {
        	SERIAL_PROTOCOLPGM("?Specified nozzle temperature not plausable.\n");
		return true;
	}
  }

  if (code_seen('R') ) {
  	randomSeed(millis());
	if ( code_has_value() )
		Random_Deviation = code_value_float();
	else
		Random_Deviation = 50.0;
  }

  X_Pos = current_position[X_AXIS];
  Y_Pos = current_position[Y_AXIS];

  if (code_seen('X')) {
	X_Pos = code_value_float();
	if ( X_Pos<X_MIN_POS || X_Pos>X_MAX_POS ) {
        	SERIAL_PROTOCOLPGM("?Specified X coordinate not plausable.\n");
		return true;
	}
  }

  if (code_seen('Y')) {
	Y_Pos = code_value_float();
	if ( Y_Pos<Y_MIN_POS || Y_Pos>Y_MAX_POS ) {
        	SERIAL_PROTOCOLPGM("?Specified Y coordinate not plausable.\n");
		return true;
	}
  }

//
// We save the question of what to do with the Unified Bed Leveling System's Activation until the very
// end.   The reason is, if one of the parameters specified up above is incorrect, we don't want to 
// alter the system's status.  We wait until we know everything is correct before altering the state
// of the system.
// 
  if (code_seen('D')) 
	  bed_leveling_mesh.state.active = 0;
  else
	  bed_leveling_mesh.state.active = 1;

  return false;
}



//
//
// Turn on the bed and  nozzle heat.   Wait for them to get up to temperature
//
//

bool turn_on_heaters() {
#if ENABLED(ULTRA_LCD)
  lcd_setstatus( "G26 Heating Bed.  ", true);
  lcd_quick_feedback();
#endif
  UBL_has_control_of_LCD_Panel++;
  thermalManager.setTargetBed( bed_temp );
  while( abs(thermalManager.degBed()-bed_temp) > 3 ) {
  	if ( G29_lcd_clicked() ) {
		strcpy( lcd_status_message, "Leaving G26    "); // We can't do lcd_setstatus() without having it continue;
  		while ( G29_lcd_clicked() )			// Debounce the switch
			idle();
            	lcd_setstatus( "Leaving G26     ", true);	// Now we do it right.
		return 1;
	}
	idle();
  }

#if ENABLED(ULTRA_LCD)
  lcd_setstatus( "G26 Heating Nozzle. ", true);
  lcd_quick_feedback();
#endif
 
//
// Now turn on the hotend (nozzle) and wait for it to come up to 
// temperature.

  thermalManager.setTargetHotend( hotend_temp , 0 );
  while( abs(thermalManager.degHotend(0)-hotend_temp) > 3 ) {
  	if ( G29_lcd_clicked() ) {
		strcpy( lcd_status_message, "Leaving G26    "); // We can't do lcd_setstatus() without having it continue;
  		while ( G29_lcd_clicked() )			// Debounce the switch
			idle();
            	lcd_setstatus( "Leaving G26     ", true);	// Now we do it right.
		return 1;
	}
	idle();
  }

#if ENABLED(ULTRA_LCD)
  lcd_setstatus( "                  ", true);
  lcd_quick_feedback();
#endif
  return 0;
}

//
// This block of code primes the nozzle if needed.
//

void prime_nozzle() {
	if ( Prime_Flag == -1 )  {	// The user wants to control how much filament gets purged
		#if ENABLED(ULTRA_LCD)
		  lcd_setstatus( "User Controled Prime", true);
		  lcd_quick_feedback();
		#endif

		set_destination_to_current();

		un_retract_filament();		// Lets make sure the G26 command doesn't think the filament is
						// retracted().   We are here because we want to prime the nozzle.
						// So let's just unretract just to be sure.	
		while( !G29_lcd_clicked() ) {
			destination[E_AXIS] += 0.25;
			mesh_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], 
						planner.max_feedrate[E_AXIS]/(15.0), 0, 0xffff, 0xffff );

			stepper.synchronize();		// Without this synchronize, the purge is more consistent,
							// but because the planner has a buffer, we won't be able
							// to stop as quickly.  So we put up with the less smooth
							// action to give the user a more responsive 'Stop'.
			set_destination_to_current();
			idle();
		}

		strcpy( lcd_status_message, "Done Priming     ");	// We can't do lcd_setstatus() without having it continue;
										// So...  We cheate to get a message up.
		while ( G29_lcd_clicked() )	// Debounce the switch
				;
		#if ENABLED(ULTRA_LCD)
		  lcd_setstatus( "Done Priming     ", true);		// Now we do it right.
		  lcd_quick_feedback();
		#endif
	  } else {
		#if ENABLED(ULTRA_LCD)
                  lcd_setstatus( "Fixed Length Prime.", true);
	          lcd_quick_feedback();
                #endif
		set_destination_to_current();
		destination[E_AXIS] += Prime_Length;
		mesh_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], 
					planner.max_feedrate[E_AXIS]/(15.0), 0, 0xffff, 0xffff );
		stepper.synchronize();
		set_destination_to_current();
		retract_filament();
	  }
}

#endif



