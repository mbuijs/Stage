
// internal function declarations that are not part of the external
// interface to Stage

#include "stage.h"

#ifdef __cplusplus
extern "C" {
#endif 

void model_destroy_cb( gpointer mod );

  /** @addtogroup stg_gui */
  /** @{ */
  void gui_startup( int* argc, char** argv[] ); 
  void gui_poll( void );
  void gui_shutdown( void );
  gui_window_t* gui_world_create( stg_world_t* world );
  void gui_world_destroy( stg_world_t* world );
  void stg_world_save( stg_world_t* world );
  int gui_world_update( stg_world_t* world );
  void stg_world_add_model( stg_world_t* world, stg_model_t* mod  );
  /// render the geometry of all models
  void gui_world_geom( stg_world_t* world );

  void gui_model_create( stg_model_t* model );
  void gui_model_destroy( stg_model_t* model );
  void gui_model_render( stg_model_t* model );
  void gui_model_nose( stg_model_t* model );
  void gui_model_move( stg_model_t* mod );
  void gui_model_geom( stg_model_t* model );
  //void gui_model_lines( stg_model_t* model );
  void gui_model_polygons( stg_model_t* model );
  void gui_model_rangers( stg_model_t* mod );
  void gui_model_rangers_data( stg_model_t* mod );
  void gui_model_features( stg_model_t* mod );
  void gui_model_laser_data( stg_model_t* mod );
  void gui_model_laser( stg_model_t* mod );
  gui_model_t* gui_model_figs( stg_model_t* model );
  void gui_window_menus_create( gui_window_t* win );
  void gui_window_menus_destroy( gui_window_t* win );
  void gui_model_mouse(rtk_fig_t *fig, int event, int mode);
  void gui_model_display_pose( stg_model_t* mod, char* verb );
  /**@}*/

 
typedef struct
{
  double x, y, a;
  double cosa, sina;
  double range;
  double max_range;
  double big_incr;
  double small_incr;
  double med_incr;

  GPtrArray* models;
  int index;
  stg_matrix_t* matrix;

} itl_t;

  
  /// initialize the stage library - optionally pass in the arguments
  /// to main, so Stage or rtk or gtk or xlib can read the options.
  int stg_init( int argc, char** argv );

  /// get a string identifying the version of stage. The string is
  /// generated by autoconf.
  const char* stg_get_version_string( void );
  
typedef int(*stg_itl_test_func_t)(stg_model_t* finder, stg_model_t* found );

itl_t* itl_create( double x, double y, double a, double b, 
		   stg_matrix_t* matrix, itl_mode_t pmode );

void itl_destroy( itl_t* itl );
void itl_raytrace( itl_t* itl );
stg_model_t* itl_next( itl_t* itl );

stg_model_t* itl_first_matching( itl_t* itl, 
				stg_itl_test_func_t func, 
				stg_model_t* finder );

  

 /** @addtogroup stg_matrix */
  /** @{ */

  stg_matrix_t* stg_matrix_create( double ppm, double medppm, double bigppm );

  // frees all memory allocated by the matrix; first the cells, then the
  // cell array.
  void stg_matrix_destroy( stg_matrix_t* matrix );

  // removes all pointers from every cell in the matrix
  void stg_matrix_clear( stg_matrix_t* matrix );

  GPtrArray* stg_matrix_cell_get( stg_matrix_t* matrix, double x, double y);
  GPtrArray* stg_matrix_bigcell_get( stg_matrix_t* matrix, double x, double y);
  GPtrArray* stg_matrix_medcell_get( stg_matrix_t* matrix, double x, double y);

  // append the [object] to the pointer array at the cell
  void stg_matrix_cell_append(  stg_matrix_t* matrix, 
				double x, double y, void* object );

  // if [object] appears in the cell's array, remove it
  void stg_matrix_cell_remove(  stg_matrix_t* matrix,
				double x, double y, void* object );

  // these append to the [object] pointer to the cells on the edge of a
  // shape. shapes are described in meters about a center point. They
  // use the matrix.ppm value to scale from meters to matrix pixels.
  void stg_matrix_rectangle( stg_matrix_t* matrix,
			     double px, double py, double pth,
			     double dx, double dy, 
			     void* object, int add );

  void stg_matrix_line( stg_matrix_t* matrix, 
			double x1, double y1, 
			double x2, double y2,
			void* object, int add );

  void stg_matrix_lines( stg_matrix_t* matrix, 
			 stg_line_t* lines, int num_lines,
			 void* object, int add );
  
  /// render a polygon into the matrix
  void stg_matrix_polygon( stg_matrix_t* matrix,
			   double x, double y, double a,
			   stg_polygon_t* poly,
			   void* object, int add );
  
  /// render an array of polygons into the matrix
  void stg_matrix_polygons( stg_matrix_t* matrix,
			    double x, double y, double a,
			    stg_polygon_t* polys, int num_polys,
			    void* object, int add );
  
  
  /**@}*/

  // TOKEN -----------------------------------------------------------------------
  // token stuff for parsing worldfiles

#define CFG_OPEN '('
#define CFG_CLOSE ')'
#define STR_OPEN '\"'
#define STR_CLOSE '\"'
#define TPL_OPEN '['
#define TPL_CLOSE ']'

  typedef enum {
    STG_T_NUM = 0,
    STG_T_BOOLEAN,
    STG_T_MODELPROP,
    STG_T_WORLDPROP,
    STG_T_NAME,
    STG_T_STRING,
    STG_T_KEYWORD,
    STG_T_CFG_OPEN,
    STG_T_CFG_CLOSE,
    STG_T_TPL_OPEN,
    STG_T_TPL_CLOSE,
  } stg_token_type_t;


  typedef struct _stg_token {
    char* token; // the text of the token 
    stg_token_type_t type;
    unsigned int line; // the line on which the token appears 
  
    struct _stg_token* next; // linked list support
    struct _stg_token* child; // tree support
  
  } stg_token_t;

  stg_token_t* stg_token_next( stg_token_t* tokens );
  // print <token> formatted for a human reader, with a string <prefix>
  void stg_token_print( char* prefix,  stg_token_t* token );
  // print a token array suitable for human reader
  void stg_tokens_print( stg_token_t* tokens );
  void stg_tokens_free( stg_token_t* tokens );

  // create a new token structure from the arguments
  stg_token_t* stg_token_create( const char* token, stg_token_type_t type, int line );

  // add a token to a list
  stg_token_t* stg_token_append( stg_token_t* head, 
				 char* token, stg_token_type_t type, int line );

  const char* stg_token_type_string( stg_token_type_t type );

  const char* stg_model_type_string( stg_model_type_t type );


  // Bitmap loading -------------------------------------------------------

  // load <filename>, an image format understood by gdk-pixbuf, and
  // return a set of rectangles that approximate the image. Caller
  // must free the array of rectangles. If width and height are
  // non-null, they are filled in with the size of the image in pixels
  int stg_load_image( const char* filename, 
		      stg_rotrect_t** rects,
		      int* rect_count,
		      int* widthp, int* heightp );

  // functions for parsing worldfiles 
  //stg_token_t* stg_tokenize( FILE* wf );
  //stg_world_t* sc_load_worldblock( stg_client_t* cli, stg_token_t** tokensptr );
  //stg_model_t* sc_load_modelblock( stg_world_t* world, stg_model_t* parent, 
  //				stg_token_t** tokensptr );

  // ---------------------------------------------------------------------------


  // utility
  void stg_pose_sum( stg_pose_t* result, stg_pose_t* p1, stg_pose_t* p2 );


#ifdef __cplusplus
}
#endif 
