#ifndef DEF_RAVE
#define DEF_RAVE

#include <iostream>
#include <string>
#include <vector>


class gRAVE_f {
// gRAVE score of feature f
   public:

      //Builders & Destroyers
      gRAVE_f();
      ~gRAVE_f();

      //Attributes
      float av;   // gRAVE value
      int Nf;     // number of final feature set containing f
};



#endif
