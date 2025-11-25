#!/bin/csh
set DIR=""
set ARGS=($_)
if ("$ARGS" != "") then
   set DIR="`dirname ${ARGS[2]}`"
else
   # But $_ might not be set if the script is source non-interactively.
   # In [t]csh the sourced file is inserted 'in place' inside the
   # outer script, so we need an external source of information
   # either via the current directory or an extra parameter.
   if ( -e setup.csh ) then
      set DIR=${PWD}
   else if ( "$1" != "" ) then
      if ( -e ${1}/setup.csh ) then
         set DIR=${1}
      else 
         echo "setup.csh: ${1} does not contain a e14 directroy"
      endif 
   else
      echo 'Error: The call to "source where_e14lib_is/setup.csh" can not determine the location of the e14 library'
      echo "because it was embedded another script (this is an issue specific to csh)."
      echo "Use either:"
      echo "   cd where_e14_is; source setup.csh"
      echo "or"
      echo "   source where_e14_is/setup.csh where_e14_is" 
   endif
endif

if ($?DIR) then 
    setenv E14_TOP_DIR "`(cd ${DIR};pwd)`/e14"
    source ${E14_TOP_DIR}/scripts/e14.csh
endif





