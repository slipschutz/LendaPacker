
#include "LendaPacker.hh"


LendaPacker::LendaPacker(){
  ///Use bad Defaults to ensure that the calling program set 
  //everything
  FL=-1; 
  FG=-1;
  d=-1;
  w=-1;
  lg=-1;
  sg=-1;
  lg2=-1;
  sg2=-1;
  traceDelay=-1;
  jentry=-1;
  Reset();
  theChannel=NULL;
}

void LendaPacker::SetFilter(Double_t _FL,Double_t _FG,Double_t _d,Double_t _w){
  FL=_FL;
  FG=_FG;
  d=_d;
  w=_w;
}

void LendaPacker::SetGates(Double_t _lg,Double_t _sg,Double_t _lg2,Double_t _sg2){

  lg=_lg;
  sg=_sg;
  lg2=_lg2;
  sg2=_sg2;
} 


LendaPacker::~LendaPacker(){


}

void LendaPacker::Reset(){

  thisEventsFF.clear();
  thisEventsCFD.clear();
  theChannel=NULL;

  thisEventsIntegral=0;
  thisEventsPulseHeight=0;
  longGate=0;
  shortGate=0;
  cubicCFD=0;
  softwareCFD=0;
  start=0;
  numZeroCrossings=0;
  

}
void LendaPacker::CalcTimeFilters(){
  
  
  theFilter.FastFilter(theChannel->trace,thisEventsFF,FL,FG); //run FF algorithim
  thisEventsCFD = theFilter.CFD(thisEventsFF,d,w); //run CFD algorithim

  softwareCFD=theFilter.GetZeroCrossing(thisEventsCFD,numZeroCrossings)-traceDelay; //find zeroCrossig of CFD

  cubicCFD = theFilter.GetZeroCubic(thisEventsCFD)-traceDelay;

 
}
void LendaPacker::CalcEnergyGates(){
  //theTrace = trace;//Stor the trace in packer for later 
  if (softwareCFD!=0)
    start = theFilter.getStartForPulseShape(softwareCFD,traceDelay);
  else{
    softwareCFD=theFilter.GetZeroCrossing(thisEventsCFD,numZeroCrossings)-traceDelay;
    start = theFilter.getStartForPulseShape(softwareCFD,traceDelay);
  }

  thisEventsPulseHeight=theFilter.getMaxPulseHeight(theChannel->trace);
    
  thisEventsIntegral = theFilter.getEnergy(theChannel->trace);
  if (theChannel->chanid ==9){
    longGate = theFilter.getGate(theChannel->trace,start,lg);
    shortGate = theFilter.getGate(theChannel->trace,start,sg);
  } else if (theChannel->chanid ==8 ){
    longGate = theFilter.getGate(theChannel->trace,start,lg2);
    shortGate = theFilter.getGate(theChannel->trace,start,sg2);
  }
  // if ((softwareCFD <0 || softwareCFD>3)&&theChannel->chanid!=0&&theChannel->chanid!=1){
  //   cout<<"jentry is "<<jentry<<endl;
  //   cout<<"softCFD "<<softwareCFD<<endl;
  //   cout<<"start is "<<start<<endl;
  //   cout<<"A Problem with "<<theChannel->chanid<<endl;
  // }
}

void LendaPacker::PackEvent(LendaEvent * Event){

  Event->pushTrace(theChannel->trace);//save the trace for later 
  
  Event->pushFilter(thisEventsFF); //save filter if it is there
  Event->pushCFD(thisEventsCFD); //save CFD if it is there
  
  //Push other thing into the event
  Event->pushLongGate(longGate); //longer integration window
  Event->pushShortGate(shortGate);//shorter integration window
  Event->pushChannel(theChannel->chanid);//the channel for this pulse
  Event->pushEnergy(thisEventsIntegral);;//push trace energy if there
  Event->pushInternEnergy(theChannel->energy);//push internal energy
  Event->pushTime(theChannel->time);
  Event->pushSoftTime(theChannel->timelow +theChannel->timehigh*4294967296.0+softwareCFD);
  Event->pushSoftwareCFD(softwareCFD);
  Event->pushCubicCFD(cubicCFD);
  Event->pushCubicTime(theChannel->timelow +theChannel->timehigh*4294967296.0+cubicCFD);
  Event->pushInternalCFD((theChannel->timecfd)/65536.0);
  Event->pushEntryNum(jentry);
  Event->pushNumZeroCrossings(numZeroCrossings);
  Event->pushPulseHeight(thisEventsPulseHeight);

  Reset();//Reset the Packers variables
}

void LendaPacker::CalcAll(){
   CalcTimeFilters();
   CalcEnergyGates();
}




void LendaPacker::RePackEvent(LendaEvent * Event){



}
