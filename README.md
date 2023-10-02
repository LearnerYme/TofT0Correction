# TOF t0 correction / recalibration package

Version: 2.1

Author: Yige Huang

Date: 02.10.2023

## Usage

* v2.0 and higher:

```
    TofT0Correction* corr = new TofT0Correction();
    corr->SetT0(0.2); // this is a default value
    /*
        in your event and track loop
    */
        corr->ReadBTofTrack(mPicoDst, mPicoEvent, mPicoTrack);
        double beta1 = corr->GetBeta(); // this beta will based on t0 = 0.2 (the default value) 
        double beta2 = corr->GetBeta(0.15); // this beta will based on t0 = 0.15 (the selected value) 
        double mass21 = corr->GetMass2(); // this mass2 is from beta2, (t0=0.15)
        double mass22 = corr->GetMass2(false); // this mass2 will use t0 = 0.2 (default value)
        double mass23 = corr->GetMass2(0.15, false); // this mass2 will use t0 = 0.15 and get the same result as mass21, but take more time
        h1OverBeta->Fill(1.0 / beta1);
        hMass2->Fill(mass2);
        ...
```

* before v2.0: 

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

02.10.2023 by yghuang (v2.1):

> The getter of mass square WAS calculating mass instead of mass square!
>
> This is a fatal bug, and is not fixed.

25.09.2023 by yghuang (v2.0):

> Now will save the time of flight (raw) to buffer, and one can calculate the corrected time with different t0.
>
> The goal is to save time when we want to process multi-t0.
 
21.09.2023 by yghuang (v1.1):

> Fix a lot of bugs.

20.09.2023 by yghuang (v1.0):

> Fisrt version.
