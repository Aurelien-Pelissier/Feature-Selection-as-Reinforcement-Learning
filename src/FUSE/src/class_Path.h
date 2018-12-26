#ifndef DEF_PATH
#define DEF_PATH


class Path {
// gRAVE score of feature f
   public:

      //Builders & Destroyers
      Path(int fi, int T_adress);
      ~Path();

      //Attributes
      int Tree_adress;   //tree adress
      int Selected_feature;     //selected feature
};

#endif
