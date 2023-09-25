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
        double tof; // raw time of flight
        double trackLength;
        double p; // momentum
        double beta; // beta buffer

    public:
        TofT0Correction(){ ReSet(); }
        ~TofT0Correction(){}
        void SetT0(double t0);
        void ReSet();
        bool ReadBTofTrack(StPicoDst* pico, StPicoEvent* event, StPicoTrack* track);
        bool ReadETofTrack(StPicoDst* pico, StPicoEvent* event, StPicoTrack* track);
        double GetBeta();
        double GetBeta(double t0);
        double GetMass2(bool read_buffer);
        double GetMass2(double t0, bool read_buffer);
};


#endif