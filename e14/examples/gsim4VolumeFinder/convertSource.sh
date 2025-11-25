grep GsimE14 ../gsim4test/gsim4test.cc  | grep include | sed "/\/\/.*/d" > GsimE14add.h
grep GsimE14 ../gsim4test/gsim4test.cc  | grep new | sed "/\/\/.*/d"     > GsimE14add.icc
grep GsimPhotoNuclearCounter ../gsim4test/gsim4test.cc  | grep include | sed "/\/\/.*/d" >> GsimE14add.h
grep GsimPhotoNuclearCounter ../gsim4test/gsim4test.cc  | grep new | sed "/\/\/.*/d"     >> GsimE14add.icc
