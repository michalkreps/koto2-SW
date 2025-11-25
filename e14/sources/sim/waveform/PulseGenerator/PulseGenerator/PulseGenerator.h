/**
 * @file PulseGenerator.h
 * @brief CsI Pulse Generator from Iwai and Lee
 * @author H.Nanjo
 * @date  2013.8.28
 * $Id:$
 * $Log:$
 */

#ifndef PulseGenerator_h
#define PulseGenerator_h


class TF1;
class TGraph;

class PulseGenerator {
 public:
  ///Constructor.
  PulseGenerator();

  ///Destructor.
  virtual ~PulseGenerator();

  ///Reset waveform array and add noise and baseline
  void   resetWaveform();
  
  ///Add wavefrom to the waveform array
  void   addWaveform(double Energy, double Time);

  ///Generate wavefrom graph (TGraph must be deleted by user)
  //  option  0: Counts v.s. Time
  //          1: Coutns v.s. Clock
  void  generateWaveform(TGraph** gr,double Energy, double Time, int option=0);

  ///fill input array contents with ADC waveform output counts
  void fillADCCounts(int nSample,int* array);

  
  void setPEperMeV(double PEperMeV);
  void setCNTperMeV(double CNTperMeV);
  void setSigmaNoiseCount(double sigmaNoiseCount);
  void setBaselineCount(double baselineCount);

  void drawPhotoEmissionPDF();
  void drawSingleWaveformTemplate();

 private:


  ///sampling time
  int m_tSample ;//=8
  
  /// # of sample
  int m_nSample ;//= 64;

  /// further segmentation of time bin
  /**
   *  for binning template array of 1 p.e. waveform functions.
   *  10 means 0.8ns binning.
   */
  int   m_fSeg    ;//= 10;


  /// # of p.e. per MeV
  double m_PEperMeV;

  /// # of ADC cnt per MeV
  double m_CNTperMeV;

  /// ADC counts for sigma of gaussian noise
  double m_sigmaNoiseCount;

  /// ADC counts for baseline
  int  m_baselineCount;

  ////Poisson mean for fluctuation of 1 p.e. gain
  double m_lambdaPMT;


  

  /// un-doped CsI photo-emission timing pdf (decay)
  TF1* m_photoEmissionPDF;

  // single p.e. waveform pdf after Bessel filter
  TF1* m_singleWaveformPDF;

  /// 1 p.e. waveform template array
  double* m_singleWaveformTemplate;//[8*nSample*fSeg];

  /// output waveform;
  double *m_outputWaveform;

  /// output waveform ADC count
  int *m_outputWaveformCount;

  /// output waveform Time
  int *m_outputWaveformTime;

  /// output waveform Clock
  int *m_outputWaveformClock;

  void   makeTemplate();
  
  int    generateNumberOfPE(double Energy);
  double generatePhotonTiming(double Time);
  double generateGainFluctuation();
  void   addPhotonWaveform(double photonGain,double photonTime);

  static double agaus( double* x, double* par);
  static double asymmetric( double* x, double* par);

};


inline void PulseGenerator::setPEperMeV(double PEperMeV) {
  m_PEperMeV=PEperMeV;
}

inline void PulseGenerator::setCNTperMeV(double CNTperMeV)
{
  m_CNTperMeV=CNTperMeV;
}

inline void PulseGenerator::setSigmaNoiseCount(double sigmaNoiseCount)
{
  m_sigmaNoiseCount=sigmaNoiseCount;
}

inline void PulseGenerator::setBaselineCount(double baselineCount)
{
  m_baselineCount=baselineCount;
}

#endif
