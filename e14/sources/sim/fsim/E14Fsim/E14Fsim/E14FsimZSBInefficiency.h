#ifndef E14FsimZSBInefficiency_h
#define E14FsimZSBInefficiency_h

#include "E14Fsim/E14FsimInefficiency.h"

class E14FsimZSBInefficiency: public E14FsimInefficiency

{

 public:
  E14FsimZSBInefficiency();
  virtual ~E14FsimZSBInefficiency();

  void selectVersion(int iVersion);
 protected:
  int m_version;
};
#endif
