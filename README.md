# TOF t0 correction / recalibration package

Version: 1.1

Author: Yige Huang

Date: 21.09.2023

## Usage

(in your code)

```
    TofT0Correction* corr = new TofT0Correction();
    corr->SetT0(0.2); // for example
    /*
        in your event and track loop
    */
        double mass2 = corr->GetBTofMass2(mPicoDst, mPicoEvent, mPicoTrack);
        double beta = corr->GetBTofBeta(mPicoDst, mPicoEvent, mPicoTrack);
        hMass2->Fill(mass2);
        h1OverBeta->Fill(1.0 / beta);
        ...
```

## Change Log
 
21.09.2023 by yghuang (v1.1):

> Fix a lot of bugs.

20.09.2023 by yghuang (v1.0):

> Fisrt version.
