#ifndef SignalCorrector_h
#define SignalCorrector_h

//---------------------------------------------------------------------
class Region
{
public:
   Region(TString n, TString v, TCut c, TString w, int nm) :
   name(n),var(v),cut(c),weight(w),normMode(nm) {}
   void Init(TTree * t, int nbins, float *bins, float frac, float area=1.) {
      fraction = frac;
      cut*=weight;
      h = new TH1D(name,"",nbins,bins);
      cout << "  " << h->GetName() << " with fraction: " << fraction << " area: " << area << endl;
      float nSel = t->Project(h->GetName(),var,cut);
      cout << "  draw: " << var << " cut: " << TString(cut) << ": " << nSel << endl;
      hNorm = (TH1D*)h->Clone(Form("%sNorm",h->GetName()));
      if (normMode>0&&h->Integral()>0) hNorm->Scale(area/h->Integral());
      hScaled = (TH1D*)hNorm->Clone(Form("%sScaled",hNorm->GetName()));
      cout << "  " << hScaled->GetName() << " scale by: " << fraction << endl;
      hScaled->Scale(fraction);
      //for (int i=1; i<=hScaled->GetNbinsX()+1 ; ++i) cout << hScaled->GetBinLowEdge(i) << " (" << hScaled->GetBinContent(i) << ") ";
      //cout << endl;
      // check
      t->Draw("cBin>>"+name+"_cBin(40,0,40)",cut,"goff");
   }
   
   TH1D * h;
   TH1D * hNorm;
   TH1D * hScaled;
   TString name;
   TString var;
   TCut cut;
   TString weight;
   int normMode;
   float fraction;
};

//---------------------------------------------------------------------
class SignalCorrector
{
public:
   SignalCorrector(TTree * tree, TString n, TString var, TCut s, TString w="(1==1)", int nm=1) : 
   name(n),
   sel(s),
   rSigAll(n+"SignalAll",var,s,w,nm),
   rBkgDPhi(n+"BkgDPhi",var,s,w,nm),
   rBkgSShape(n+"BkgSShape",var,s,w,nm),
   rBkgSShapeDPhi(n+"BkgSShapeDPhi",var,s,w,nm),
   weight(w),
   normMode(nm), // 0=area is signal region count, 1=unit normalization, 2=per photon normalization
   subDPhiSide(true),
   subSShapeSide(true),
   subSShapeSideDPhiSide(true),
   cutBkgDPhi("jetEt>30&&acos(cos(photonPhi-jetPhi))>0.7 && acos(cos(photonPhi-jetPhi))<3.14159/2. && sigmaIetaIeta<0.01"),
   cutSShape("jetEt>30&&acos(cos(photonPhi-jetPhi))>2.0944 && sigmaIetaIeta>0.011 && sigmaIetaIeta<0.017"),
   cutSShapeDPhi("jetEt>30&&acos(cos(photonPhi-jetPhi))>0.7 && acos(cos(photonPhi-jetPhi))<3.14159/2. && sigmaIetaIeta>0.011 && sigmaIetaIeta<0.017"),   
   nSelPhoton(0),nSigAll(0),fracDPhiBkg(0),photonPurity(0),fracPhotonBkg(0),fracPhotonBkgDPhiBkg(0) {
      t = tree;
   }
   
   void SetPhotonIsolation(int isolScheme, int cBin)
   {
      //
      // Centrality bins: 0=0-4, 1=4-8, 2=8-12, 3=12-20, 4=20-40
      // 
      if (isolScheme==0) { //sum isol
//         nameIsol="Sum(Isol.)<5";
//         cutIsol = "sumIsol/0.9<5";
//         if (cBin==0) photonPurity=0.50;
//         if (cBin>=1&&cBin<=2) photonPurity=0.64;
//         if (cBin>=3&&cBin<=4) photonPurity=0.57;
         nameIsol="Sum(Isol.)<1GeV";
         cutIsol = "(cc4+cr4+ct4PtCut20)/0.9 <1";
         if (cBin==0) photonPurity=0.74;
         if (cBin>=1&&cBin<=2) photonPurity=0.78;
         if (cBin==3) photonPurity=0.76;
         if (cBin==4) photonPurity=0.82;
      } else if (isolScheme==1) { // cut isol
         nameIsol="3DCutIsol.";
         cutIsol = "cc4 < 6.9 && ct4PtCut20 < 3.00 && cr4<5";
         if (cBin==0) photonPurity=0.46;
         if (cBin>=1&&cBin<=2) photonPurity=0.62;
         if (cBin>=3&&cBin<=4) photonPurity=0.54;
      } else if (isolScheme==2) { // fisher isol
         nameIsol="Fisher Isol.";
         t->SetAlias("fisherIsol","(4.5536204845644690e-01 +cc5*-1.1621087258504197e-03 +cc4*-1.3139962130657250e-02 +cc3*9.8272534188056666e-03 +cc2*-7.9659880964355362e-02 +cc1*5.6661268034678275e-02 +cr5*-1.2763802967154852e-02 +cr4*-1.2594575465310987e-03 +cr3*-1.3333157740152167e-02 +cr2*-2.5518237583408113e-02 +cr1*-1.3706749407235775e-02 +ct4PtCut20*-7.9844325658248016e-03 +ct3PtCut20*-2.5276510400767658e-03 +ct2PtCut20*-2.0741636383420897e-02 +ct1PtCut20*7.1545293456054884e-04 +ct5PtCut20*7.8080659557798627e-03)");
         cutIsol = "fisherIsol>0.2";
         if (cBin==0) photonPurity=0.62;
         if (cBin>=1&&cBin<=2) photonPurity=0.66;
         if (cBin>=3&&cBin<=4) photonPurity=0.64;
      }
      fracPhotonBkg=1-photonPurity;
      // Isolation Cut
      sel = sel&&cutIsol;      
   }
   
   void MakeHistograms(TCut sigSel, int nbin, float xmin, float xmax) {
      float * bins = new float[nbin+1];
      float dx = (xmax-xmin)/nbin;
      for (int i=0; i<nbin+1; ++i) {
         bins[i] = xmin+i*dx;
      }
      MakeHistograms(sigSel,nbin,bins);
   }
   
   void MakeHistograms(TCut sigSel, int nbin, float * bins) {
      cout << endl << "Base Selection: " << sel << endl;
      rSigAll.cut = sel&&sigSel;
      rBkgDPhi.cut = sel&&cutBkgDPhi;
      rBkgSShape.cut = sel&&cutSShape;
      rBkgSShapeDPhi.cut = sel&&cutSShapeDPhi;
      
      // photon normalization
      nSelPhoton = t->GetEntries(sel&&"sigmaIetaIeta<0.01");
      // number of events in signal region
      nSigAll = t->GetEntries(rSigAll.cut);
      cout << " ** Number of selection photons: " << nSelPhoton << " gamma-jets: " << nSigAll << " ** " << endl;
      float area=1.;
      if (normMode==0) area=nSigAll;
      if (normMode==2) area=nSigAll/nSelPhoton;
      rSigAll.Init(t,nbin,bins,1.,area);
      if (subDPhiSide) {
         float nDPhiSide = t->GetEntries(rBkgDPhi.cut);
         float nDPhiBkg = nDPhiSide * (3.14159-2.0944)/(3.14159/2.-0.7);
         fracDPhiBkg = nDPhiBkg/nSigAll;
         rBkgDPhi.Init(t,nbin,bins,fracDPhiBkg,area);
         cout << "  |dhpi| sig all = " << nSigAll << "|dphi| side = " << nDPhiSide << " bck contamination: " << nDPhiBkg << " = " << fracDPhiBkg << endl;
      }
      if (subSShapeSide) {
         cout << "  fracPhotonBkg: " << fracPhotonBkg << endl;
         rBkgSShape.Init(t,nbin,bins,fracPhotonBkg,area);
         if (subSShapeSideDPhiSide) {
            float nDPhiSide = t->GetEntries(rBkgSShapeDPhi.cut);
            float nDPhiBkg = nDPhiSide * (3.14159-2.0944)/(3.14159/2.-0.7);
            fracPhotonBkgDPhiBkg = fracPhotonBkg*(nDPhiBkg/(nSigAll));
            //fracPhotonBkgDPhiBkg = 0;
            rBkgSShapeDPhi.Init(t,nbin,bins,fracPhotonBkgDPhiBkg,area);
         }
      }
      
      hSubtracted = (TH1D*)rSigAll.hScaled->Clone(name+"Subtracted");
      if (subDPhiSide) hSubtracted->Add(rBkgDPhi.hScaled,-1);
      if (subSShapeSide) {
         hSSSideDPhiSub = (TH1D*)rBkgSShape.hScaled->Clone(Form("%sDPhiSub",rBkgSShape.hScaled->GetName()));
         if (subSShapeSideDPhiSide) hSSSideDPhiSub->Add(rBkgSShapeDPhi.hScaled,-1);
         hSubtracted->Add(hSSSideDPhiSub,-1);
      }
      if (normMode>0) {
         // Rescale after subtraction
         if (subDPhiSide&&subSShapeSide) area*=(1-fracDPhiBkg-(fracPhotonBkg-fracPhotonBkgDPhiBkg))/(1-fracPhotonBkg);
         rSigAll.hScaled->Scale(area/rSigAll.hScaled->Integral());
         //if (subSShapeSide) rBkgSShape.hScaled->Scale(area/rBkgSShape.hScaled->Integral());
         hSubtracted->Scale(area/hSubtracted->Integral());
      }
   }
   TTree * t;
   TString name,nameIsol;
   TCut sel,cutIsol;
   Region rSigAll;
   Region rBkgDPhi;
   Region rBkgSShape;
   Region rBkgSShapeDPhi;
   TH1D * hSubtracted;
   TH1D * hSSSideDPhiSub;
   TString weight;
   int normMode;
   bool subDPhiSide;
   bool subSShapeSide;
   bool subSShapeSideDPhiSide;
   TCut cutBkgDPhi;
   TCut cutSShape;
   TCut cutSShapeDPhi;   
   float nSelPhoton;
   float nSigAll;
   float fracDPhiBkg;
   float photonPurity;
   float fracPhotonBkg;
   float fracPhotonBkgDPhiBkg;
};

#endif