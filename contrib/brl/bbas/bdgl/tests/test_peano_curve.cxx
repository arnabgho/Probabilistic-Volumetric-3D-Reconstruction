// This is brl/bbas/bdgl/tests/test_peano_curve.cxx
#include <iostream>
#include <vcl_compiler.h>
#include <bdgl/bdgl_peano_curve.h>
#include <testlib/testlib_test.h>

static void test_peano_curve()
{
  std::vector<vgl_point_2d<double> > gt;
  gt.push_back(vgl_point_2d<double>(-0.75,-0.75));
  gt.push_back(vgl_point_2d<double>(-0.25,-0.75));
  gt.push_back(vgl_point_2d<double>(-0.25,-0.25));
  gt.push_back(vgl_point_2d<double>(-0.75,-0.25));
  gt.push_back(vgl_point_2d<double>(-0.75,0.25));
  gt.push_back(vgl_point_2d<double>(-0.75,0.75));
  gt.push_back(vgl_point_2d<double>(-0.25,0.75));
  gt.push_back(vgl_point_2d<double>(-0.25,0.25));
  gt.push_back(vgl_point_2d<double>(0.25,0.25));
  gt.push_back(vgl_point_2d<double>(0.25,0.75));
  gt.push_back(vgl_point_2d<double>(0.75,0.75));
  gt.push_back(vgl_point_2d<double>(0.75,0.25));
  gt.push_back(vgl_point_2d<double>(0.75,-0.25));
  gt.push_back(vgl_point_2d<double>(0.25,-0.25));
  gt.push_back(vgl_point_2d<double>(0.25,-0.75));
  gt.push_back(vgl_point_2d<double>(0.75,-0.75));

  std::vector<vgl_point_2d<double> > p=recurse_peano_curve(2);

  bool flag=true;
  for (unsigned i=0;i<p.size();i++)
  {
    if (p[i]!=gt[i])
      flag=false;
  }
  TEST("Peano Curve for level 2", flag, true);
}

TESTMAIN(test_peano_curve);
