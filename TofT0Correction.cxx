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
    cout << "[LOG] - From TofT0Correction: Default time offset is " << t0 << endl;
    this->t0 = t0;
}

void TofT0Correction::ReSet() {
    c= 29.98;
    tof = -999.0;
    p = 0.0;
    beta = 1.0;
}

bool TofT0Correction::ReadBTofTrack(StPicoDst* pico, StPicoEvent* event, StPicoTrack* track) {
    ReSet();
    if (!event) { return false; }
    if (!track) { return false; }
    if (!track->isPrimary()) { return false; }
    if (!track->isTofTrack()) { return false; }

    StPicoBTofPidTraits* tofTraits = pico->btofPidTraits(track->bTofPidTraitsIndex());
    if (!tofTraits) { return false; }
    if (tofTraits->btofMatchFlag() <= 0) { return false; }

    // now it should be a legit tof pid
    auto vertex = event->primaryVertex();
    auto hit = tofTraits->btofHitPos();
    const StThreeVectorD vertexVec(vertex.X(), vertex.Y(), vertex.Z());
    const StThreeVectorD hitVec(hit.X(), hit.Y(), hit.Z());
    trackLength = tofPathLength(&vertexVec, &hitVec, track->helix(event->bField()).curvature());
    if (trackLength == -999) { return false; }
    p = track->pPtot();

    tof = tofTraits->btof();
    if (tof == 0.0) { return false; }
    return true; // has valid bTOF information
}

bool TofT0Correction::ReadETofTrack(StPicoDst* pico, StPicoEvent* event, StPicoTrack* track) {
    ReSet();
    if (!event) { return false; }
    if (!track) { return false; }
    if (!track->isPrimary()) { return false; }
    if (!track->isETofTrack()) { return false; }

    StPicoETofPidTraits* tofTraits = pico->etofPidTraits(track->eTofPidTraitsIndex());
    if (!tofTraits) { return false; }
    if (tofTraits->matchFlag() <= 0) { return false; }

    // now it should be a legit tof pid
    auto vertex = event->primaryVertex();
    auto hit = tofTraits->crossingPos();
    const StThreeVectorD vertexVec(vertex.X(), vertex.Y(), vertex.Z());
    const StThreeVectorD hitVec(hit.X(), hit.Y(), hit.Z());
    trackLength = tofPathLength(&vertexVec, &hitVec, track->helix(event->bField()).curvature());
    if (trackLength == -999) { return false; }
    p = track->pPtot();

    tof = tofTraits->tof();
    return true; // has valid eTOF information
}
// return trackLength / tof / c;

double TofT0Correction::GetBeta() {
    beta = tof - t0 == 0 ? -999.0 : trackLength / (tof - t0) / c;
    return beta;
}

double TofT0Correction::GetBeta(double t0) {
    beta = tof - t0 == 0 ? -999.0 : trackLength / (tof - t0) / c;
    return beta;
}

double TofT0Correction::GetMass2(bool read_buffer=true) {
    /*
        If you calculate and fill beta first, you can just read buffer.
        But if you did not get this beta before call GetMass2, please set read_buffer as false.
    */
    if (!read_buffer) {
        beta = GetBeta();
    }
    if (beta == 0.0 || beta < -990) { return -999.0; } 
    return p*p * (1.0 / beta / beta - 1.0);
}

double TofT0Correction::GetMass2(double t0, bool read_buffer=true) {
    if (!read_buffer) {
        beta = GetBeta(t0);
    }
    if (beta == 0.0 || beta < -990) { return -999.0; } 
    return p*p * (1.0 / beta / beta - 1.0);
}