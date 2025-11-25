/**
 *  @file
 *  @brief  GsimDetectorEventData
 *  @author Hajime Nanjo <nanjo@scphys.kyoto-u.ac.jp>
 *  @date   2005.10.5
 *  $Id$
 *  $Log$
 */
#ifndef GsimDetectorEventData_h
#define GsimDetectorEventData_h

//includes
#include "GsimData/GsimDetectorHitData.h"
#include "GsimData/GsimDigiData.h"
#include "GsimData/GsimTimeData.h"

#include "TObject.h"
#include "TClonesArray.h"

//forward declaration


/**
 *  @class GsimDetectorEventData
 *  @brief This class stores detector event data for output to a ROOT file.
 *
 *  The number of hits, the total energy deposited, the time of the first
 *  hit, the time of the last hit, and an array of the hits are stored.
 */
class GsimDetectorEventData : public TObject
{
 public:
  /**
   *  @function GsimDetectorEventData
   *  @brief Constructor for GsimDetectorEventData.
   */
  GsimDetectorEventData();
  /**
   *  @function ~GsimDetectorEventData
   *  @brief Destructor for GsimDetectorEventData.
   */
  virtual ~GsimDetectorEventData();


  virtual void   Clear(Option_t* opt="");
  
  /**
   *  @function initializeDataValues
   *  @brief Initializes the detector event data.
   *
   *  The number of hits, total energy deposited,
   *  and times are set to default values. The hits
   *  array is cleared.
   */
  void initializeDataValues();
  /**
   *  @function clearHitData
   *  @brief Clears the hit data array.
   */
  void clearHitData();
  /**
   *  @function clearDigiData
   *  @brief Clears the digi data array.
   */
  void clearDigiData();
  /**
   *  @function clearMtimeData
   *  @brief Clears the mtime data array.
   */
  void clearMtimeData();
  /**
   *  @function getClassName
   *  @brief Returns the name of the class - "GsimDetectorEventData".
   */
  std::string getClassName();
  /**
   *  @function getArraySize
   *  @brief Returns the size of the hits and digi arrays.
   *  @return 
   */
  static int getArraySize();

  void dumpHits();
  void dumpDigi();

  /**
   *  @brief Detector (trigger) status.
   */
  UShort_t           status;
  
  /**
   *  @brief The number of hits.
   */
  UInt_t           nHit;
  /**
   *  @brief The number of digi.
   */
  UShort_t           nDigi;
  /**
   *  @brief The number of mtime.
   */
  UInt_t           nMtime;
  /**
   *  @brief The number of trigger channel.
   */
  UShort_t           nTrig;
  /**
   *  @brief The total energy deposited.
   */
  Float_t        totalEnergy;
  /**
   *  @brief The time of the first hit.
   */
  Float_t        firstHitTime;
  /**
   *  @brief The time of the last hit.
   */
  Float_t        lastHitTime;

  
  /**
   *  @brief An array of the hits.
   */
  TClonesArray* hits;//->
  /**
   *  @brief An array of the digitizers.
   */
  TClonesArray* digi;//->

  /**
   *  @brief An array of time.
   */
  TClonesArray* mtime;//->

  /**
   *  @brief An array of time.
   */
  TClonesArray* trig;//->
  
 protected:
  /**
   *  @brief The name of the detector event.
   *
   *  Currently (release 0-5), this is left blank - "".
   */
  std::string name;//!
  /**
   *  @brief The name of the class.
   *
   *  This is hardcoded to "GsimDetectorEventData".
   */
  std::string className;//!
  Float_t t1;//!
  Float_t t2;//!
  /**
   *  @brief The size of the hits and digi arrays.
   *
   *  As of release-0-5, this is hardcoded to 50,000.
   */
  static const int s_arraySize;//!
  
  ClassDef(GsimDetectorEventData,2)
};

inline std::string GsimDetectorEventData::getClassName() {
  return className;
}

inline int GsimDetectorEventData::getArraySize() {
  return s_arraySize;
}
     

#endif // GsimDetectorEventData_h

