#ifndef __YIGE_T0CORR_HEADER__
#define __YIGE_T0CORR_HEADER__

/*
    TOF t0 (mainly for eTOF) correction
    You need to calculate t0 by yourself, and use this module to get the corrected beta (and t).
*/

class StPicoDst;
class StPicoEvent;
class StPicoTrack;


class TofT0Correction {

    private:
        double t0;
        double c;

    public:
        TofT0Correction(){ c= 29.98; }
        ~TofT0Correction(){}
        void SetT0(double t0);
        double GetBTofBeta(StPicoDst* pico, StPicoEvent* event, StPicoTrack* track, bool getRaw);
        double GetETofBeta(StPicoDst* pico, StPicoEvent* event, StPicoTrack* track, bool getRaw);
        double GetBTofMass2(StPicoDst* pico, StPicoEvent* event, StPicoTrack* track, bool getRaw);
        double GetETofMass2(StPicoDst* pico, StPicoEvent* event, StPicoTrack* track, bool getRaw);
};


#endif