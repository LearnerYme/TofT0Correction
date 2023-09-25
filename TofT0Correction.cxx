/*
    TOF t0 (mainly for eTOF) correction
    You need to calculate t0 by yourself, and use this module to get the corrected beta (and t).
*/
#include "TMath.h"

#include "StPicoEvent/StPicoDst.h"
#include "StPicoEvent/StPicoEvent.h"
#include "StPicoEvent/StPicoTrack.h"

#include "StPicoEvent/StPicoBTofPidTraits.h"
#include "StPicoEvent/StPicoETofPidTraits.h"

#include "StThreeVector.hh"
#include "StBTofUtil/tofPathLength.hh"

#include "TofT0Correction.h"
void TofT0Correction::SetT0(double t0) {
    cout << "[LOG] - From TofT0Correction: Time offset is " << t0 << endl;
    this->t0 = t0;
}

double TofT0Correction::GetBTofBeta(StPicoDst* pico, StPicoEvent* event, StPicoTrack* track, bool getRaw=false) {
    if (!event) { return -999.0; }
    if (!track) { return -999.0; }
    if (!track->isPrimary()) { return -999.0; }
    if (!track->isTofTrack()) { return -999.0; }

    StPicoBTofPidTraits* tofTraits = pico->btofPidTraits(track->bTofPidTraitsIndex());
    if (!tofTraits) { return -999.0; }
    if (tofTraits->btofMatchFlag() <= 0) { return -999.0; }

    // now it should be a legit tof pid
    auto vertex = event->primaryVertex();
    auto hit = tofTraits->btofHitPos();
    const StThreeVectorD vertexVec(vertex.X(), vertex.Y(), vertex.Z());
    const StThreeVectorD hitVec(hit.X(), hit.Y(), hit.Z());
    double trackLength = tofPathLength(&vertexVec, &hitVec, track->helix(event->bField()).curvature());
    if (trackLength == -999) { return -999.0; }

    double tof = tofTraits->btof();
    if (!getRaw) {
        tof -= t0;
    }
    if (tof == 0.0) { return -999.0; }
    return trackLength / tof / c;
}

double TofT0Correction::GetBTofMass2(StPicoDst* pico, StPicoEvent* event, StPicoTrack* track, bool getRaw=false) {
    double beta = GetBTofBeta(pico, event, track, getRaw);
    if (beta == 0.0 || beta < -990) { return -999.0; }
    double p = track->pPtot();
    return p*sqrt(1.0 / beta / beta - 1.0);
}

double TofT0Correction::GetETofBeta(StPicoDst* pico, StPicoEvent* event, StPicoTrack* track, bool getRaw=false) {
    if (!event) { return -999.0; }
    if (!track) { return -999.0; }
    if (!track->isPrimary()) { return -999.0; }
    if (!track->isETofTrack()) { return -999.0; }

    StPicoETofPidTraits* tofTraits = pico->etofPidTraits(track->eTofPidTraitsIndex());
    if (!tofTraits) { return -999.0; }
    if (tofTraits->matchFlag() <= 0) { return -999.0; }

    // now it should be a legit tof pid
    auto vertex = event->primaryVertex();
    auto hit = tofTraits->crossingPos();
    const StThreeVectorD vertexVec(vertex.X(), vertex.Y(), vertex.Z());
    const StThreeVectorD hitVec(hit.X(), hit.Y(), hit.Z());
    double trackLength = tofPathLength(&vertexVec, &hitVec, track->helix(event->bField()).curvature());
    if (trackLength == -999) { return -999.0; }

    double tof = tofTraits->tof();
    if (!getRaw) {
        tof -= t0;
    }
    if (tof == 0.0) { return -999.0; }
    return trackLength / tof / c;
}

double TofT0Correction::GetETofMass2(StPicoDst* pico, StPicoEvent* event, StPicoTrack* track, bool getRaw=false) {
    double beta = GetETofBeta(pico, event, track, getRaw);
    if (beta == 0.0 || beta < -990) { return -999.0; }
    double p = track->pPtot();
    return p*sqrt(1.0 / beta / beta - 1.0);
}
