// Created 18-04-2016 by N. Smith <nick.smith@cern.ch>
// This is to be a catch-all for L1 Trigger subsystems
// who do not have their own render plugin.
#include "DQM/DQMRenderPlugin.h"
#include "QualityTestStatusRenderPlugin.h"
#include "TBox.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TFrame.h"
#include "TGaxis.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TText.h"
#include <cassert>
#include <iostream>

class L1TStage2RenderPlugin : public DQMRenderPlugin
{

public:
  virtual void initialise(int, char **)
    {

    }

  virtual bool applies( const VisDQMObject &o, const VisDQMImgInfo & )
    {
      if( o.name.find( "L1T" ) != std::string::npos || o.name.find( "L1TEMU" ) != std::string::npos )
      {
        // Put here the Stage2 subsystems who provide their own plugin:
        if( o.name.find( "L1TStage2CaloLayer1" ) != std::string::npos )
          return false;
        if( o.name.find( "L1TdeStage2CaloLayer1" ) != std::string::npos )
          return false;
        if( o.name.find( "L1TStage2EMTF" ) != std::string::npos )
          return false;
        if( o.name.find( "L1TStage2CaloLayer2" ) != std::string::npos )
          return false;

        return true;
      }

      return false;
    }

  virtual void preDraw( TCanvas *c, const VisDQMObject &o, const VisDQMImgInfo &i, VisDQMRenderInfo &r )
    {
      c->cd();

      if( dynamic_cast<TH2F*>( o.object ) )
      {
        preDrawTH2F( c, o, r );
      }
      else if( dynamic_cast<TH1F*>( o.object ) )
      {
        preDrawTH1F( c, o, i, r );
      }
    }

  virtual void postDraw( TCanvas *c, const VisDQMObject &o, const VisDQMImgInfo &i )
    {
      c->cd();

      if( dynamic_cast<TH2F*>( o.object ) )
      {
        postDrawTH2F( c, o );
      }
      else if( dynamic_cast<TH1F*>( o.object ) )
      {
        postDrawTH1F( c, o, i);
      }
    }

private:

  void preDrawTH2F( TCanvas *, const VisDQMObject &o, VisDQMRenderInfo & )
    {
      TH2F* obj = dynamic_cast<TH2F*>( o.object );
      assert( obj );

      if (o.name.find("reportSummaryMap") != std::string::npos) {

          obj->SetStats(kFALSE);
          dqm::QualityTestStatusRenderPlugin::reportSummaryMapPalette(obj);

          obj->GetXaxis()->SetLabelSize(0.1);

          obj->GetXaxis()->CenterLabels();
          obj->GetYaxis()->CenterLabels();

          return;
      }

    }

  void preDrawTH1F( TCanvas *, const VisDQMObject &o, const VisDQMImgInfo &, VisDQMRenderInfo & )
    {
      TH1F* obj = dynamic_cast<TH1F*>( o.object );
      assert( obj );

      std::string name = o.name.substr(o.name.rfind("/")+1);

      // for object comparison histograms (muons)
      if (name.find("muColl1hwPt") != std::string::npos) {
        gPad->SetLogy(1);
      }

      if (name.find("muColl2hwPt") != std::string::npos) {
        gPad->SetLogy(1);
      }

      if (name.find("errorSummary") == 0) {
        obj->SetOption("texthist");
      }

      if (name.find("mismatchRatio") == 0) {
        obj->SetOption("texthist");
      }

      if (name.find("summary") == 0) {
        obj->SetOption("texthist");
      }

      // MP7 zero suppression validation
      if (name.find("zeroSuppVal") == 0) {
        obj->SetOption("texthist");
      }

    }

  void postDrawTH2F( TCanvas *, const VisDQMObject &o )
    {
      TH2F* obj = dynamic_cast<TH2F*>( o.object );
      assert( obj );

      gStyle->SetOptStat(11);

      std::string name = o.name.substr(o.name.rfind("/")+1);

      if ( name.find("reportSummaryMap") != std::string::npos )
      {
	TLine l_line;
	TText t_text;

            t_text.DrawText(2.25, 14.3, "Mu");
            t_text.DrawText(2.25, 13.3, "NonIsoEG");
            t_text.DrawText(2.25, 12.3, "IsoEG");
            t_text.DrawText(2.25, 11.3, "CenJet");
            t_text.DrawText(2.25, 10.3, "ForJet");
            t_text.DrawText(2.25, 9.3,  "NonIsoTau");
            t_text.DrawText(2.25, 8.3,  "IsoTau");
            t_text.DrawText(2.25, 7.3,  "ETT");
            t_text.DrawText(2.25, 6.3,  "ETM");
            t_text.DrawText(2.25, 5.3,  "HTT");
            t_text.DrawText(2.25, 4.3,  "HTM");
            t_text.DrawText(2.25, 3.3,  "minBias");
            t_text.DrawText(2.25, 2.3,  "-");
            t_text.DrawText(2.25, 1.3,  "GtExternal");

            t_text.DrawText(1.25, 11.3, "");
            t_text.DrawText(1.25, 10.3, "");
            t_text.DrawText(1.25, 9.3, "uGT");
            t_text.DrawText(1.25, 8.3, "uGMT");
            t_text.DrawText(1.25, 7.3, "EMTF");
            t_text.DrawText(1.25, 6.3, "OMTF");
            t_text.DrawText(1.25, 5.3, "BMTF");
            t_text.DrawText(1.25, 4.3, "Calo Layer2");
            t_text.DrawText(1.25, 3.3, "Calo Layer1");
            t_text.DrawText(1.25, 2.3, "HCAL TPG");
            t_text.DrawText(1.25, 1.3, "ECAL TPG");

            l_line.SetLineWidth(2);

            // vertical line

            l_line.DrawLine(2, 1, 2, 15);

            // horizontal lines

            l_line.DrawLine(1, 1, 3, 1);
            l_line.DrawLine(1, 2, 3, 2);
            l_line.DrawLine(1, 3, 3, 3);
            l_line.DrawLine(1, 4, 3, 4);
            l_line.DrawLine(1, 5, 3, 5);
            l_line.DrawLine(1, 6, 3, 6);
            l_line.DrawLine(1, 7, 3, 7);
            l_line.DrawLine(1, 8, 3, 8);
            l_line.DrawLine(1, 9, 3, 9);
            l_line.DrawLine(1, 10, 3, 10);
            l_line.DrawLine(1, 11, 3, 11);
            l_line.DrawLine(1, 12, 3, 12);
            l_line.DrawLine(2, 13, 3, 13);
            l_line.DrawLine(2, 14, 3, 14);

            return;
        }

    // muon TF track addresses
    if (name.find("muColl1TrkAddr") != std::string::npos || name.find("muColl2TrkAddr") != std::string::npos) {
      obj->SetOption("text colz");
    }

    }

  void postDrawTH1F( TCanvas *, const VisDQMObject &o, const VisDQMImgInfo & )
    {
      TH1F* obj = dynamic_cast<TH1F*>( o.object );
      assert( obj );

      std::string name = o.name.substr(o.name.rfind("/")+1);

    }
};

static L1TStage2RenderPlugin instance;
