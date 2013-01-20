//This is brl/bbas/volm/volm_query.h
#ifndef volm_query_h_
#define volm_query_h_
//:
// \file
// \brief  A class to represent a volumetric matching query through an image mark-up and estimated camera parameters
//              The container is numbers of points distributed on a unit spherecial surface
//              The camera parameters, heading, tilt, roll, right_fov, top_fov, have resolution one degree, and their
//              default values are chosen based on query image category (desert/coast)
//              Use top viewing angle to define the viewing volume
//
// \author Yi Dong
// \date October 23, 2012
// \verbatim
//   Modifications
//    Yi Dong   Jan-2013   added functions to generate object based query infomation for object based volm_matcher
//    Yi Dong   Jan-2013   added object orientation and object NLCD land classification
//    JLM       Jan 20, 2013 Added constructor from depth_map_scene
// \endverbatim
//

#include <vbl/vbl_ref_count.h>
#include <bpgl/depth_map/depth_map_scene_sptr.h>
#include <bpgl/depth_map/depth_map_scene.h>
#include <vgl/vgl_polygon.h>
#include <volm/volm_spherical_container_sptr.h>
#include <volm/volm_spherical_shell_container_sptr.h>
#include <volm/volm_spherical_shell_container.h>
#include <vpgl/vpgl_perspective_camera.h>
#include <vil/vil_image_view.h>
#include <vcl_set.h>

class volm_query : public vbl_ref_count
{
 public:
  //: default consturctor
  volm_query() {}
  //: constructor from files
  volm_query(vcl_string const& cam_kml_file,
             vcl_string const& label_xml_file,
             volm_spherical_container_sptr const& sph,
             volm_spherical_shell_container_sptr const& sph_shell,
             bool const& use_default = true);

  //: constructor from depth map scene
  volm_query(vcl_string const& depth_map_scene_file,
             vcl_string const& image_category,
             volm_spherical_container_sptr const& sph,
             volm_spherical_shell_container_sptr const& sph_shell,
             double altitude);


  // === accessors ===
  vcl_vector<vcl_vector<unsigned char> >& min_dist()            { return min_dist_;}
  vcl_vector<vcl_vector<unsigned char> >& max_dist()            { return max_dist_;}
  vcl_vector<vcl_vector<unsigned char> >& order()               { return order_; }
  vcl_vector<unsigned>& valid_ray_num()                         { return ray_count_; }
  vcl_set<unsigned>& order_set()                                { return order_set_; }
  vcl_vector<vcl_vector<vcl_vector<unsigned> > >& order_index() { return order_index_; }
  vcl_vector<vcl_vector<vcl_vector<unsigned> > >& dist_id()     { return dist_id_; }
  vcl_vector<unsigned char>& max_obj_dist()                     { return max_obj_dist_; }
  vcl_vector<unsigned char>& min_obj_dist()                     { return min_obj_dist_; }
  vcl_vector<unsigned char>& obj_orient()                       { return obj_orient_; }
  vcl_vector<unsigned char>& obj_nlcd()                         { return obj_nlcd_; }
  vcl_vector<unsigned char>& order_obj()                        { return order_obj_; }
  vcl_vector<vcl_vector<unsigned> >& ground_id()                { return ground_id_; }
  vcl_vector<vcl_vector<unsigned char> >& ground_dist()         { return ground_dist_; }
  vcl_vector<vcl_vector<unsigned char> >& ground_nlcd()         { return ground_nlcd_; }
  unsigned char ground_orient()                                 { return ground_orient_; }
  vcl_vector<vcl_vector<unsigned> >& sky_id()                   { return sky_id_; }
  vcl_vector<float>& obj_weight()                               { return weight_obj_; }
  float grd_weight() const                                      { return weight_grd_; }
  float sky_weight() const                                      { return weight_sky_; }
  depth_map_scene_sptr depth_scene() const                      { return dm_; }
  vcl_vector<depth_map_region_sptr>& depth_regions()            { return depth_regions_; }
  unsigned get_cam_num() const                                  { return (unsigned)cameras_.size(); }
  unsigned get_obj_order_num() const                            { return (unsigned)order_index_[0].size(); }
  unsigned get_query_size() const                               { return query_size_; }
  unsigned get_valid_ray_num(unsigned const& cam_idx) const     { return ray_count_[cam_idx]; }
  vcl_vector<double>& top_fov()                                 { return top_fov_; }
  vcl_vector<double>& headings()                                { return headings_; }
  vcl_vector<double>& tilts()                                   { return tilts_; }
  vcl_vector<double>& rolls()                                   { return rolls_; }
  //: return number of voxels having ground properties
  unsigned get_ground_id_size() const;
  //: return stored distance for all ground voxels
  unsigned get_ground_dist_size() const;
  //: return number of voxels having non-ground, non-sky properties
  unsigned get_dist_id_size() const;
  //: return number of voxels having sky properties
  unsigned get_sky_id_size() const;
  //: return number of voxels for all non-ground objects (order_index)
  unsigned get_order_size() const;
  //: return the total query size in byte(object based)
  unsigned obj_based_query_size_byte() const;
  
  //: write vrml for spherical container and camera hypothesis
  void draw_template(vcl_string const& vrml_fname);
  //: write query image showing the depth map geometry and the penetrating ray
  void draw_query_images(vcl_string const& out_dir);
  void draw_query_image(unsigned i, vcl_string const& out_name);
  //: get camera string
  vcl_string get_cam_string(unsigned i) const { return camera_strings_[i]; }
  //: get the number of camera having the input top_fov value
  unsigned get_num_top_fov(double const& top_fov) const;
  //: extract the top_fov value from cam_id
  double get_top_fov(unsigned const& i) const;
  //: return valid top_fov from camera vector
  vcl_vector<double> get_valid_top_fov() const;
  //: visualized the query camera using the spherical shell geometry
  void visualize_query(vcl_string const& prefix);
  //: generate rgb depth image for given camera id and depth value
  void depth_rgb_image(vcl_vector<unsigned char> const& values, unsigned const& cam_id, vil_image_view<vil_rgb<vxl_byte> >& out_img);
  //: draw the polygons of regions on top of an rgb image
  void draw_depth_map_regions(vil_image_view<vil_rgb<vxl_byte> >& out_img);
  void draw_query_regions(vcl_string const& out_name);

  static void draw_polygon(vil_image_view<vil_rgb<vxl_byte> >& img, vgl_polygon<double> const& poly, unsigned char const& depth);

  //: initial camera parameters read from camera kml
  double init_focal_;
  double head_, head_d_, head_inc_;
  double tilt_, tilt_d_, tilt_inc_;
  double roll_, roll_d_, roll_inc_;
  double tfov_, tfov_d_, tfov_inc_;
  double altitude_;

 protected:
  //: a check whether use the viewing volume values provided by camera kml
  bool use_default_;
  //: image size
  unsigned ni_, nj_;
  unsigned log_downsample_ratio_;  // 0,1,2 or 3 (ni-->ni/2^ratio_), to generate downsampled depth maps for ground regions
  //: image category (e.g., desert, coast)
  vcl_string                       img_category_;
  //: depth map scene
  depth_map_scene_sptr                       dm_;
  //: voxel array used to get voxel index
  volm_spherical_container_sptr       sph_depth_;
  //: a unit sphere 
  volm_spherical_shell_container_sptr       sph_;
  //: upper bound on depth
  double d_threshold_;
  //: vector of depth_map_region sorted by depth order
  vcl_vector<depth_map_region_sptr> depth_regions_;
  //: depth region polygons (maybe not used)
  vcl_vector<vgl_polygon<double> >        dm_poly_;
  //: number of valid rays for a given camera
  vcl_vector<unsigned>                  ray_count_;
  //: camera parameters --- use even number later to ensure the init_value and init_value +/- conf_value is covered
  //: vectors store the space of camera hypotheses
  vcl_vector<double>  top_fov_;
  vcl_vector<double> headings_;
  vcl_vector<double>    tilts_;
  vcl_vector<double>    rolls_;
  vcl_vector<vpgl_perspective_camera<double> > cameras_;
  vcl_vector<vcl_string> camera_strings_;
  //: ingested query information
  vcl_vector<vcl_vector<unsigned char> > min_dist_;
  vcl_vector<vcl_vector<unsigned char> > max_dist_;
  vcl_vector<vcl_vector<unsigned char> >    order_;
  //: number of spherical shell rays
  unsigned query_size_;
  //: the order index assigned to sky
  unsigned order_sky_;
  //: the set of Cartesian points on the unit sphere
  vcl_vector<vgl_point_3d<double> > query_points_;
  //: order vector to store the index id associated with object order
  vcl_set<unsigned> order_set_;  // store the non-ground order, using set to ensure objects having same order are put together
  vcl_vector<vcl_vector<vcl_vector<unsigned> > > order_index_;
  //: ground plane distance, id, and NLCD classification
  vcl_vector<vcl_vector<unsigned> >          ground_id_;
  vcl_vector<vcl_vector<unsigned char> >   ground_dist_;
  vcl_vector<vcl_vector<unsigned char> >   ground_nlcd_;
  unsigned char                          ground_orient_;  // always horizontal
  //: sky distance
  vcl_vector<vcl_vector<unsigned> > sky_id_;
  //: object id based on min_dist (since objects may have different min_dist but same order)
  vcl_vector<vcl_vector<vcl_vector<unsigned> > > dist_id_;
  //: min and max distance, object orders, orientation and land clarifications for different objects, based on object orders
  vcl_vector<unsigned char> min_obj_dist_;
  vcl_vector<unsigned char> max_obj_dist_;
  vcl_vector<unsigned char>    order_obj_;
  vcl_vector<unsigned char>   obj_orient_;
  vcl_vector<unsigned char>     obj_nlcd_;
  //: weight parameters
  vcl_vector<float> weight_obj_;
  float             weight_grd_;
  float             weight_sky_;
  
  //: functions
  bool query_ingest();
  bool order_ingest();
  bool weight_ingest();
  unsigned char fetch_depth(double const& u,
                            double const& v,
                            unsigned char& order,
                            unsigned char& max_dist,
                            unsigned& object_id,
                            unsigned char& grd_nlcd,
                            bool& is_ground,
                            bool& is_sky,
                            bool& is_object,
                            vil_image_view<float> const& depth_img);
  void create_cameras();
  void generate_regions();
  void draw_viewing_volume(vcl_string const& fname,
                           vpgl_perspective_camera<double> cam,
                           float r,
                           float g,
                           float b);
  void draw_rays(vcl_string const& fname);

  void draw_dot(vil_image_view<vil_rgb<vxl_byte> >& img, 
                vgl_point_3d<double> const& world_point,
                unsigned char const& depth, 
                vpgl_perspective_camera<double> const& cam);
};

#endif  // volm_query_h_
