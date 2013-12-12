



#ifndef __LENDAPACKER_HH
#define __LENDAPACKER_HH

#include "TTree.h"
#include "Filter.hh"
#include <vector>
#include "LendaEvent.hh"
#include "DDASEvent.h"
//#include "ddaschannel.h"
class LendaPacker {

public:

  LendaPacker();
  ~LendaPacker();
  
  Filter theFilter;

  void Reset();

  void CalcTimeFilters();
  void CalcEnergyGates();
  void CalcAll();
  void SetFilter(Double_t,Double_t,Double_t,Double_t);
  void SetGates(Double_t,Double_t,Double_t,Double_t);
  inline void SetTraceDelay(Int_t x){traceDelay=x;}


  inline void SetDDASChannel(ddaschannel *d){theChannel=d;} 
  inline void SetJEntry(Long64_t n){jentry=n;}

  vector <Double_t> thisEventsFF;
  vector <Double_t> thisEventsCFD;


  Double_t thisEventsIntegral;
  Double_t longGate;
  Double_t shortGate;
  Double_t cubicCFD;
  Double_t softwareCFD;
  Double_t start;
  
  bool lean;


  void PackEvent(LendaEvent* Event);
  void RePackEvent(LendaEvent* Event);

private:
  ddaschannel * theChannel;
  Double_t FL,FG,d,w;
  Double_t lg,sg,lg2,sg2;
  Int_t traceDelay;
  Long64_t jentry;

};

#endif
