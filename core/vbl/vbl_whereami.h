#ifndef vbl_whereami_h_
#define vbl_whereami_h_
#ifdef __GNUC__
#pragma interface
#endif
//
// .NAME vbl_whereami
// .INCLUDE vbl/vbl_whereami.h
// .SECTION Author
//  fsm@robots.ox.ac.uk
//

#include <vcl_iostream.h>

#define vbl_whereami (vcl_cerr << __FILE__ ", line " << __LINE__ << " : ")

#endif // vbl_whereami_h_
