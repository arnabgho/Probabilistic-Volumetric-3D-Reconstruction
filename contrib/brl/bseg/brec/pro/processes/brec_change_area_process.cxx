// This is brl/bseg/brec/pro/processes/brec_change_area_process.cxx
#include <bprb/bprb_func_process.h>
//:
// \file
// \brief A class to find expected area over the whole image for a given prob map
//
// \author Ozge Can Ozcanli
// \date 10/01/08
//
// \verbatim
//  Modifications
//   Ozge C. Ozcanli - 02/03/09 - converted process-class to functions which is the new design for bprb processes.
// \endverbatim

#include <bprb/bprb_parameters.h>
#include <vcl_iostream.h>
#include <vil/vil_image_view.h>
#include <vil/vil_convert.h>
#include <brip/brip_vil_float_ops.h>
#include <core/vidl2_pro/vidl2_pro_utils.h>

//: Constructor
bool brec_change_area_process_cons(bprb_func_process& pro)
{
  bool ok=false;
  vcl_vector<vcl_string> input_types;
  input_types.push_back("vil_image_view_base_sptr"); //input probability frame
  input_types.push_back("vil_image_view_base_sptr"); //input probability frame's mask
  ok = pro.set_input_types(input_types);
  if (!ok) return ok;

  vcl_vector<vcl_string> output_types;
  output_types.push_back("float");  // expected area
  output_types.push_back("float");  // expected area as a percentage of the total number of pixels
  ok = pro.set_output_types(output_types);
  return ok;
}

//: Execute the process
bool brec_change_area_process(bprb_func_process& pro)
{
  // Sanity check
  if (pro.n_inputs()< 2){
    vcl_cerr << "In brec_change_area_process - invalid inputs\n";
    return false;
  }

  // get the inputs
  unsigned i = 0;
  vil_image_view_base_sptr temp = pro.get_input<vil_image_view_base_sptr>(i++);
  vil_image_view<float> map = *vil_convert_cast(float(), temp);
  if (temp->pixel_format() != VIL_PIXEL_FORMAT_FLOAT)
    return false;
  unsigned ni = map.ni(), nj = map.nj();

  temp = pro.get_input<vil_image_view_base_sptr>(i++);
  vil_image_view<bool> input_mask = *vil_convert_cast(bool(), temp);

  vil_image_view<float> out(ni, nj, 1);
  out.fill(0);

  float sum = 0.0f;
  float count = 0.0f;
  for (unsigned j = 0; j<nj; ++j)
    for (unsigned i = 0; i<ni; ++i)
    {
      if (input_mask(i,j)) {
        sum += map(i,j);
        count += 1.0f;
      }
    }

  pro.set_output_val<float>(0, sum);
  pro.set_output_val<float>(1, (sum/count)*100.0f);
  
  return true;
}

