/**
 *  $Id: $
 *  $Log:$
 */
#include "GsimKernel/GsimMaterial.h"
#include "GsimPersistency/GsimMessage.h"

GsimMaterial::GsimMaterial()

{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  ;
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}



GsimMaterial::~GsimMaterial()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  ;
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}
