#---E14---
# you must set
#   E14_TOP_DIR



TARGET=`uname 2>/dev/null`
#isKEKCC=`hostname -f | sed -n "/cc.kek.jp/p"`

#---E14 library---
if [ x$E14_TOP_DIR = x ] ; then
    echo 'Please set E14_TOP_DIR.'
    return 1
fi
export E14_CONFIG_DIR=${E14_TOP_DIR}/config
export LD_LIBRARY_PATH=${E14_TOP_DIR}/lib/so:${LD_LIBRARY_PATH}
if [ $TARGET != Darwin ] ; then
    if [ x$MY_TOP_DIR != x ] ; then
	export LD_LIBRARY_PATH=${MY_TOP_DIR}/lib:${LD_LIBRARY_PATH}
    fi
fi
if [ $TARGET = Darwin ] ; then
    export DYLD_LIBRARY_PATH=${E14_TOP_DIR}/lib/so:${DYLD_LIBRARY_PATH}
    if [ x$MY_TOP_DIR != x ] ; then
	export DYLD_LIBRARY_PATH=${MY_TOP_DIR}/lib:${DYLD_LIBRARY_PATH}
    fi
fi
export PATH=${E14_TOP_DIR}/bin:${PATH}

#---ROOT---
if [ x$ROOTSYS = x ] ; then
    echo 'Please install ROOT and set ROOTSYS'
    echo ' add ${ROOTSYS}/bin to PATH'
    echo ' add ${ROOTSYS}/lib to LD_LIBRARY_PATH'
    echo '  also to DYLD_LIBRARY_PATH for OSX'
    echo 'At KEK CC, root is installed under /software/ROOT/'
    return 1
fi

isEXIST=`which root-config 2> /dev/null`
if [ x$isEXIST = x ] ; then
    echo ' add ${ROOTSYS}/bin to PATH'
    return 1
fi


if [ $TARGET != Darwin ] ; then
    isEXIST=`echo $LD_LIBRARY_PATH | sed "s/:/\n/g" | grep $ROOTSYS | head -n 1`
    if [ x$isEXIST = x ] ; then
	echo ' add ${ROOTSYS}/lib to LD_LIBRARY_PATH'
	return 1
    fi
fi

if [ $TARGET = Darwin ] ; then
    isEXIST=`echo $DYLD_LIBRARY_PATH | sed "s/:/\n/g" | grep $ROOTSYS | head -n 1`
    if [ x$isEXIST = x ] ; then
	echo ' add ${ROOTSYS}/lib to DYLD_LIBRARY_PATH'
	return 1
    fi
fi

#---Geant4/CLHEP---
isNewG4=`which geant4-config 2> /dev/null`

if [ x$isNewG4 != x ] ; then
    :
elif [ x$G4INSTALL != x ]; then
    :
else
    echo 'Please install Geant4>=8.3.'
    echo 'For standard G4 environment,'
    echo 'Set $G4INSTALL and source $G4INSTALL/env.sh'
    echo 'Add CLHEP bin to the PATH; export PATH=${CLHEP_BASE_DIR}/bin:$PATH'
    echo 'At KEK CC,'
    echo '    soruce /software/Geatn4/geant4.x.y/g4rc.sh'
    echo '    export G4LIB=$G4INSTALL/slib'
    echo '    export G4SYSTEM=Linux-g++'
    echo '    export G4LIB_BUILD_SHARED=1'
    echo '    export LD_LIBRARY_PATH=$CLHEP_BASE_DIR/lib:$LD_LIBRARY_PATH'
    echo '    export LD_LIBRARY_PATH=$G4LIB/$G4SYSTEM:$LD_LIBRARY_PATH'
    echo '    export G4LEVELGAMMADATA=$G4INSTALL/../data/....'
    echo '    also export for other crosssection data directories.'
    return 1
fi

if [ x$isNewG4 != x ] ; then
    if [ $TARGET != Darwin ] ; then
	isEXIST=$(echo $LD_LIBRARY_PATH | sed "s/:/\n/g" | grep $(cd $(geant4-config --libs | awk '{print $1}' | sed "s/-L//");pwd)  | head -n  1)
	if [ x$isEXIST = x ] ; then
	    echo 'Please add G4 lib directory to LD_LIBRARY_PATH using source geant4.sh'
	    return 1
	fi
    else
	isEXIST=$(echo $DYLD_LIBRARY_PATH | sed "s/:/\n/g" | grep $(cd $(geant4-config --libs | awk '{print $1}' | sed "s/-L//");pwd)  | head -n  1)
	if [ x$isEXIST = x ] ; then
	    echo 'Please add G4 lib directory to DYLD_LIBRARY_PATH using source geant4.sh'
	    return 1
	fi
    fi
fi


if [ x$G4INSTALL != x ]; then
    if [ x$G4LIB = x ]; then
	echo 'Please set G4LIB.'
	echo 'For example,'
	echo 'export G4LIB=$G4INSTALL/lib'
	return 1
    fi

    if [ x$G4SYSTEM = x ]; then
	echo 'Please set G4SYSTEM.'
	echo 'For example,'
	echo 'export G4SYSTEM=Linux-g++'
	return 1
    fi

    if [ x$G4LIB_BUILD_SHARED = x ]; then
	echo 'Please set as export G4LIB_BUILD_SHARED=1'
	return 1
    fi
    if [ $TARGET != Darwin ] ; then
	isEXIST=`echo $LD_LIBRARY_PATH | sed "s/:/\n/g" | grep $G4LIB | head -n 1`
	if [ x$isEXIST = x ] ; then
	    echo 'Please add G4 lib directory to LD_LIBRARY_PATH.'
	    return 1
	fi
    else
	isEXIST=`echo $DYLD_LIBRARY_PATH | sed "s/:/\n/g" | grep $G4LIB | head -n 1`
	if [ x$isEXIST = x ] ; then
	    echo 'Please add G4 lib directory to DYLD_LIBRARY_PATH.'
	    return 1
	fi
    fi
    G4VERSION=
    if [ x$isNewG4 != x ] ; then
	G4VERSION=$(geant4-config --version | sed "s/\.//g")
    else
	G4VERSION=`grep G4VERSION_NUMBER $G4INSTALL/source/global/management/include/G4Version.hh | grep define  | awk '{print $3}'`
    fi
    G4OK=`awk -v v=$G4VERSION 'BEGIN{if(v>=830) print "ok";}'`

    if [ x$G4OK = x ];then
	echo 'Geant4 version<8.3 for G4INSTALL.'
	echo 'Please install/use Geant4>=8.3.'
	return 1
    fi
fi



if [ x$isNewG4 = x ] ; then
    isEXIST=`which clhep-config 2> /dev/null`
    if [ x$isEXIST = x ] ; then
	echo ' add CLHEP bin directory to PATH'
	return 1
    fi
    
    if [ $TARGET != Darwin ] ; then
	isEXIST=$(echo $LD_LIBRARY_PATH | sed "s/:/\n/g" | grep $(clhep-config --prefix |  sed "s/\"//g" ) | head -n 1)
	if [ x$isEXIST = x ] ; then
	    echo 'Please add CLHEP lib directory to LD_LIBRARY_PATH.'
	    return 1
	fi
    else
	isEXIST=$(echo $DYLD_LIBRARY_PATH | sed "s/:/\n/g" | grep $(clhep-config --prefix |  sed "s/\"//g" ) | head -n 1)
	if [ x$isEXIST = x ] ; then
	    echo 'Please add CLHEP lib directory to DYLD_LIBRARY_PATH.'
	    return 1
	fi
    fi
fi


if [ x$G4LEDATA = x ] || [ xG4LEVELGAMMADATA = x ] || [ xG4RADIOACTIVEDATA = x ]; then
    echo "Please set Geant4 Data variables, G4LEDATA, G4LEVELGAMMADATA, G4RADIOACTIVEDATA, etc."
    return 1
fi

G432=
if [ x$isNewG4 != x ] ; then
    G432=$(if file $(geant4-config --prefix)/"lib*"/libG4global.so | grep -q 32-bit;then echo 32bit;fi)
else
    G432=`if file $G4LIB/$G4SYSTEM/libG4global.so  | grep -q 32-bit;then echo 32bit;fi`
fi
RO32=`if file $ROOTSYS/bin/root | grep -q 32-bit;then echo 32bit;fi`

if [ x$G432 != x$RO32 ];then
    echo "Gean4 and ROOT architectures are different (32/64 bit)."
    echo $G432
    echo $RO32
    echo "Please fix."
    return 1
fi


echo '#include <stdlib.h>' > /tmp/rltest.c
echo '#include <stdio.h>' >> /tmp/rltest.c
echo '#include <readline/readline.h>' >> /tmp/rltest.c
echo 'int main() {return 0;}' >> /tmp/rltest.c
gcc -o /tmp/rltest /tmp/rltest.c -lreadline -lncurses 2> /dev/null
if [ ! -f /tmp/rltest ]; then
    rm -f /tmp/rltest.c
    echo "Please install readline libraray."
    return 1
else
    rm -f /tmp/rltest /tmp/rltest.c
fi




echo "Successfully configured!"

#---HPSS---
#if [ x${HPSS_ROOT} = x ];then
#    echo 'HPSS is not activated.'
#    echo 'If you want to turn on in KEK, do as follows.'
#    echo "export HPSS_ROOT=/hpss_src/32api"
#    echo "source /var/hpss/kek/hpss32.sh"
#    echo 'export HPSS_ROOT=/hpss_src/final'
#    echo 'export LD_LIBRARY_PATH=${HPSS_ROOT}/lib:${LD_LIBRARY_PATH}'
#fi
