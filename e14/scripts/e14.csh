#---E14---
# you must set
#   E14_TOP_DIR
if ( ! ${?LD_LIBRARY_PATH} ) setenv LD_LIBRARY_PATH
if ( ! ${?MANPATH} ) setenv MANPATH
if ( ! ${?PATH} )    setenv PATH
set TARGET=`uname`
#set isKEKCC=`hostname -f | sed -n "/cc.kek.jp/p"`

#---E14 library---
if (! $?E14_TOP_DIR ) then
    echo 'Please set E14_TOP_DIR.'
    exit 1
endif
setenv E14_CONFIG_DIR ${E14_TOP_DIR}/config
setenv LD_LIBRARY_PATH ${E14_TOP_DIR}/lib/so:${LD_LIBRARY_PATH}
if ( $TARGET != Darwin ) then
    if ( $?MY_TOP_DIR ) then
	setenv LD_LIBRARY_PATH ${MY_TOP_DIR}/lib:${LD_LIBRARY_PATH}
    endif
endif
if ( $TARGET == Darwin ) then
    setenv DYLD_LIBRARY_PATH=${E14_TOP_DIR}/lib/so:${DYLD_LIBRARY_PATH}
    if ( $?MY_TOP_DIR ) then
	setenv DYLD_LIBRARY_PATH ${MY_TOP_DIR}/lib:${DYLD_LIBRARY_PATH}
    endif
endif
setenv PATH ${E14_TOP_DIR}/bin:${PATH}

#---ROOT---
if (! $?ROOTSYS ) then
    echo 'Please install ROOT and set ROOTSYS'
    echo ' add ${ROOTSYS}/bin to PATH'
    echo ' add ${ROOTSYS}/lib to LD_LIBRARY_PATH'
    echo '  also to DYLD_LIBRARY_PATH for OSX'
    echo 'At KEK CC, root is installed under /software/ROOT/'
    exit 1
endif

set isEXIST="yes"
root-config --prefix >& /dev/null || set isEXIST=""
if ( x$isEXIST == x ) then
    echo ' add ${ROOTSYS}/bin to PATH'
    exit 1
endif

if ( $TARGET != Darwin ) then
    set isEXIST=`echo $LD_LIBRARY_PATH | sed "s/:/\n/g" | grep $ROOTSYS | head -n 1`
    if ( x$isEXIST == x ) then
	echo ' add ${ROOTSYS}/lib to LD_LIBRARY_PATH'
	exit 1
    endif
endif

if ( $TARGET == Darwin ) then
    set isEXIST=`echo $DYLD_LIBRARY_PATH | sed "s/:/\n/g" | grep $ROOTSYS | head -n 1`
    if ( x$isEXIST == x ) then
	echo ' add ${ROOTSYS}/lib to DYLD_LIBRARY_PATH'
	exit 1
    endif
endif

#---Geant4/CLHEP---
set isNewG4="yes"
geant4-config --prefix >& /dev/null || set isNewG4=""


if ( x$isNewG4 == x ) then
    if ( ! ${?G4INSTALL} ) then
        echo 'Please install Geant4>=8.3.'
	echo 'For standard G4 environment,'
	echo 'Set $G4INSTALL and source $G4INSTALL/env.sh'
	echo 'Add CLHEP bin to the PATH; export PATH=${CLHEP_BASE_DIR}/bin:$PATH'
	echo 'At KEK CC,'
	echo '    soruce /software/Geatn4/geant4.x.y/g4rc.csh'
	echo '    setenv G4LIB $G4INSTALL/slib'
	echo '    setenv G4SYSTEM Linux-g++'
	echo '    setenv G4LIB_BUILD_SHARED 1'
	echo '    setenv LD_LIBRARY_PATH $CLHEP_BASE_DIR/lib:$LD_LIBRARY_PATH'
	echo '    setenv LD_LIBRARY_PATH $G4LIB/$G4SYSTEM:$LD_LIBRARY_PATH'
	echo '    setenv G4LEVELGAMMADATA $G4INSTALL/../data/....'
	echo '    also setenv for other crosssection data directories.'
	exit 1
    endif
endif


if ( x$isNewG4 != x ) then
    if ( $TARGET != Darwin ) then
	set temp=`geant4-config --libs | awk '{print $1}' | sed "s/-L//"`
	set temp2=`cd ${temp};pwd`
	set isEXIST=`echo $LD_LIBRARY_PATH | sed "s/:/\n/g" | grep ${temp2}  | head -n  1`
	if ( x$isEXIST == x ) then
	    echo 'Please add G4 lib directory to LD_LIBRARY_PATH using source geant4.sh'
	    exit 1
	endif
    else
	set temp=`geant4-config --libs | awk '{print $1}' | sed "s/-L//"`
	set temp2=`cd ${temp};pwd`
	isEXIST=`echo $DYLD_LIBRARY_PATH | sed "s/:/\n/g" | grep ${temp2}  | head -n  1`
	if ( x$isEXIST == x ) then
	    echo 'Please add G4 lib directory to DYLD_LIBRARY_PATH using source geant4.sh'
	    exit 1
	endif
    endif
endif



if ( ${?G4INSTALL} ) then
    if ( x$G4LIB == x ) then
	echo 'Please set G4LIB.'
	echo 'For example,'
	echo 'setenv G4LIB $G4INSTALL/lib'
	exit 1
    endif

    if ( x$G4SYSTEM == x ) then
	echo 'Please set G4SYSTEM.'
	echo 'For example,'
	echo 'setenv G4SYSTEM Linux-g++'
	exit 1
    endif

    if ( x$G4LIB_BUILD_SHARED == x ) then
	echo 'Please set as setenv G4LIB_BUILD_SHARED 1'
	exit 1
    endif


    if ( $TARGET != Darwin ) then
	set isEXIST=`echo $LD_LIBRARY_PATH | sed "s/:/\n/g" | grep $G4LIB | head -n 1`
	if ( x$isEXIST == x ) then
	    echo 'Please add G4 lib directory to LD_LIBRARY_PATH.'
	    exit 1
	endif
    else
	set isEXIST=`echo $DYLD_LIBRARY_PATH | sed "s/:/\n/g" | grep $G4LIB | head -n 1`
	if ( x$isEXIST == x ) then
	    echo 'Please add G4 lib directory to DYLD_LIBRARY_PATH.'
	    exit 1
	endif
    endif
    
    set G4VERSION=`grep G4VERSION_NUMBER $G4INSTALL/source/global/management/include/G4Version.hh | grep define  | awk '{print $3}'`
    set G4OK=`awk -v v=$G4VERSION 'BEGIN{if(v>=830) print "ok";}'`

    if ( x$G4OK == x ) then
	echo 'Geant4 version<8.3 due to G4INSTALL.'
	echo 'Please install/use Geant4>=8.3.'
	exit 1
    endif
endif


set isEXIST="yes"
clhep-config --prefix >& /dev/null || set isEXIST=""
if ( x$isEXIST == x ) then
    echo ' add CLHEP bin directory to PATH'
    exit 1
endif

if ( $TARGET != Darwin ) then
    set temp=`clhep-config --prefix |  sed 's/\"//g' `
    set isEXIST=`echo $LD_LIBRARY_PATH | sed "s/:/\n/g" | grep ${temp} | head -n 1`
    if ( x$isEXIST == x ) then
	echo 'Please add CLHEP lib directory to LD_LIBRARY_PATH.'
	exit 1
    endif
else
    set temp=`clhep-config --prefix |  sed 's/\"//g' `
    set isEXIST=`echo $DYLD_LIBRARY_PATH | sed "s/:/\n/g" | grep ${temp} | head -n 1`
    if ( x$isEXIST == x ) then
	echo 'Please add CLHEP lib directory to DYLD_LIBRARY_PATH.'
	exit 1
    endif
endif

if ( ( x$G4LEDATA == x ) || ( xG4LEVELGAMMADATA == x ) || ( xG4RADIOACTIVEDATA == x ) ) then
    echo "Please set Geant4 Data variables, G4LEDATA, G4LEVELGAMMADATA, G4RADIOACTIVEDATA, etc."
    exit 1
endif

#
# Skip 32/64 bit check due to not working part.
#
# set G432=""
# if ( x$isNewG4 != x ) then
#     set temp=`geant4-config --prefix`
#     echo "DEBUG1"
#     set G432=`if ( { file ${temp}/lib*/libG4global.so | grep -q 32-bit } ) echo 32bit `
#     echo "DEBUG2"
# else
#     set G432=`if ( { file $G4LIB/$G4SYSTEM/libG4global.so  | grep -q 32-bit } ) echo 32bit `
# endif
# echo "DEBUG3"
# set RO32=`if ( { file $ROOTSYS/bin/root | grep -q 32-bit } ) echo 32bit `
# echo "DEBUG4"
# if ( x$G432 != x$RO32 ) then
#     echo "Gean4 and ROOT architectures are different (32/64 bit)."
#     echo "Please fix."
#     exit 1
# endif

echo '#include <stdlib.h>' > /tmp/rltest.c
echo '#include <stdio.h>' >> /tmp/rltest.c
echo '#include <readline/readline.h>' >> /tmp/rltest.c
echo 'int main() {return 0;}' >> /tmp/rltest.c
gcc -o /tmp/rltest /tmp/rltest.c -lreadline -lncurses >& /dev/null
if ( ! -f /tmp/rltest ) then
    rm -f /tmp/rltest.c
    echo "Please install readline libraray."
    exit 1
else
    rm -f /tmp/rltest /tmp/rltest.c
endif


echo "Successfully configured!"

#---HPSS---
#if (! $?HPSS_ROOT) then
#    echo 'HPSS is not activated.'
#    echo 'If you want to turn on in KEK, do as follows.'
#    echo "export HPSS_ROOT=/hpss_src/32api"
#    echo "source /var/hpss/kek/hpss32.csh"
#    echo 'setenv HPSS_ROOT /hpss_src/final'
#    echo 'setenv LD_LIBRARY_PATH ${HPSS_ROOT}/lib:${LD_LIBRARY_PATH}'
#endif
