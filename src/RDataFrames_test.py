import numpy as np
import os,argparse
import re
import yaml
import ROOT
import time

def maxfilenumber(path):
	list_of_files = os.listdir(path)
	n = [int(re.findall("\d+", str(file))[0]) for file in list_of_files]
	return max(n)

start_time = time.time()
parser = argparse.ArgumentParser(description='Plot stacked histogram')
parser.add_argument("-c", "--config", dest="config",   help="Enter config file to process", type=str)
parser.add_argument("-a", "--cuts", dest="cuts",   help="Enter cuts file to process", type=str)
parser.add_argument("-o","--output", dest="output", help="Destination directory", type=str)
parser.add_argument("-y","--year", dest="year", help="Year for processing", type=int)
args = parser.parse_args()

fin = open(args.config,'r')
# f = open("params.txt",'w')
conf_pars = yaml.safe_load(fin)
data_loc = conf_pars['locations'][0]
data_name = conf_pars['name']
# list_of_files = os.listdir(data_loc)
treeName = "makeTopologyNtupleMiniAOD/tree"
rdf = ROOT.RDataFrame(treeName,data_loc+"*.root")
# print(data_loc)
entries_total = rdf.Count()
# auto myHist2 = myDf.Histo1D<float>({"histName", "histTitle", 64u, 0., 128.}, "myColumn");
print(entries_total.GetValue())
index_list_code = '''
auto idx_loop = [](int n){std::vector<int> muon_idx; for i in range(n) {muon_idx.push_back(i);} return;};
idx_loop()
'''
# .Define('mu_idx','ROOT.VecOps.AsRVec(np.arange(0,numMuonPF2PAT,1,int))')\
# .Define('mu_idx','[](int &numMuonPF2PAT){std::vector<int> muon_idx; for (int i=0; i < numMuonPF2PAT;i++) {muon_idx.push_back(i);} return muon_idx;};')\
# .Define('mu_idx','')\
# .Define('mu_sel','mu_idx_sel.size()>=2 && mu_pt_sel')

# attempt to make LorentzVector for tracks collection

getCh_code = '''
ROOT::VecOps::RVec<ROOT::Math::PxPyPzMVector> makeLVs(ROOT::VecOps::RVec<Int_t>& packedCandsPdgId,ROOT::VecOps::RVec<Int_t>& packedCandsCharge,ROOT::VecOps::RVec<Int_t>& packedCandsHasTrackDetails,ROOT::VecOps::RVec<Float_t>& packedCandsPx,ROOT::VecOps::RVec<Float_t>& packedCandsPy,ROOT::VecOps::RVec<Float_t>& packedCandsPz, double chsMass_, bool flag = true) {
    ROOT::VecOps::RVec<ROOT::Math::PxPyPzMVector> lvs;
    // std::cout<<"check sizes:"<<packedCandsPdgId.size()<<","<<packedCandsCharge.size()<<","<<packedCandsHasTrackDetails.size()<<","<<packedCandsPx.size()<<","<<packedCandsPy.size()<<","<<packedCandsPz.size()<<std::endl;
    for (Int_t k = 0; k < packedCandsPx.size(); k++) {
        ROOT::Math::PxPyPzMVector lVec {packedCandsPx[k], packedCandsPy[k], packedCandsPz[k], chsMass_};
        lvs.push_back(lVec);
    }
    // std::cout<<"Gets out"<<std::endl;
    return lvs;
}
'''
getChAlt_code = '''
ROOT::VecOps::RVec<ROOT::Math::PxPyPzMVector> makeLVsAlt(ROOT::VecOps::RVec<Int_t>& packedCandsPdgId,ROOT::VecOps::RVec<Int_t>& packedCandsCharge,ROOT::VecOps::RVec<Int_t>& packedCandsHasTrackDetails,ROOT::VecOps::RVec<Float_t>& packedCandsPx,ROOT::VecOps::RVec<Float_t>& packedCandsPy,ROOT::VecOps::RVec<Float_t>& packedCandsPz, double chsMass_,  const Int_t run, const Float_t lumi, const Int_t event, bool flag = true) {
    ROOT::VecOps::RVec<ROOT::Math::PxPyPzMVector> lvs;
    for (Int_t k = 0; k < packedCandsPx.size(); k++) {
        ROOT::Math::PxPyPzMVector lVec {packedCandsPx[k], packedCandsPy[k], packedCandsPz[k], chsMass_};
        // if (((run==1)&&(lumi==9))&&((event==10536) || (event==10607) || (event==11483))) {
        //   if (std::abs(lVec.Eta())<=2.4) {
        //     std::cout<<"From the inside: run,lumi,event:"<<run<<","<<lumi<<","<<event<<std::endl;
        //     std::cout<<"Track "<<k<<" (Pt,Eta,Phi,M):"<<lVec.Pt()<<","<<lVec.Eta()<<","<<lVec.Phi()<<","<<lVec.M()<<std::endl;
        //   }
        // }
				if ((std::abs(lVec.Eta()) < 2.4) && (lVec.Pt() > 5)) lvs.push_back(lVec);
    }
    return lvs;
}
'''
getTest_code ='''
using FourVector = ROOT::Math::PxPyPzMVector;
using namespace ROOT::VecOps;
Int_t getSize(const RVec<Float_t> lv_trk_pt, const RVec<Float_t> lv_trk_eta, const RVec<Float_t> cand_globalidx_hadronsonly, const RVec<Float_t> ch_trk_pdgid)
{
   std::cout<<"Vector sizes:"<<lv_trk_pt.size()<<","<<lv_trk_eta.size()<<","<<cand_globalidx_hadronsonly.size()<<","<<ch_trk_pdgid.size()<<std::endl;
   return 0;
}
'''
getIndices_code = '''
using namespace ROOT::VecOps;

RVec<unsigned long> getIndices(const int num) {
	RVec<unsigned long> idx;
	for (int i = 0;i<num;i++) idx.emplace_back(i);
	return idx;
}
'''
getKinematics_code ='''
using FourVector = ROOT::Math::PxPyPzMVector;
using namespace ROOT::VecOps;
RVec<Float_t> getKinematics(const RVec<FourVector> &tracks, TString var = "pt")
{
   auto pt = [](const FourVector &v) { return v.Pt(); };
   auto eta = [](const FourVector &v) { return v.Eta(); };
   auto phi = [](const FourVector &v) { return v.Phi(); };
   if (var.Contains("eta")) return Map(tracks, eta);
   else if (var.Contains("phi")) return Map(tracks, phi);
   else return Map(tracks, pt);
}
'''
getCheck_code ='''
using FourVector = ROOT::Math::PxPyPzMVector;
using namespace ROOT::VecOps;
bool getEvent(const RVec<FourVector> &tracks, const RVec<FourVector> &tracks_alt, const Int_t run, const Float_t lumi, const Int_t event)
{
   if (tracks.size()!=tracks_alt.size()) {
    std::cout<<"run,lumi,event:"<<run<<","<<lumi<<","<<event<<std::endl;
    std::cout<<"tracks.size(),tracks_alt.size():"<<tracks.size()<<","<<tracks_alt.size()<<std::endl;
    return true;
  }
   else return false;
}
bool PrintInformation(const RVec<FourVector> &tracks, const RVec<FourVector> &tracks_alt, const Int_t run, const Float_t lumi, const Int_t event)
{
  if (((run==1)&&(lumi==9))&&((event==10536) || (event==10607) || (event==11483))) {
    std::cout<<"run,lumi,event:"<<run<<","<<lumi<<","<<event<<std::endl;
    std::cout<<"tracks.size(),tracks_alt.size():"<<tracks.size()<<","<<tracks_alt.size()<<std::endl;
    for (int i=0;i<tracks.size();i++) {
      std::cout<<"Track "<<i<<" (Pt,Eta,Phi,M):"<<tracks[i].Pt()<<","<<tracks[i].Eta()<<","<<tracks[i].Phi()<<","<<tracks[i].M()<<std::endl;
    }
    std::cout<<"Now the old"<<std::endl;
    for (int i=0;i<tracks_alt.size();i++) {
      std::cout<<"Track "<<i<<" (Pt,Eta,Phi,M):"<<tracks_alt[i].Pt()<<","<<tracks_alt[i].Eta()<<","<<tracks_alt[i].Phi()<<","<<tracks_alt[i].M()<<std::endl;
    }
    return true;
  }
  return false;
}
'''
getDileptonCand_code = '''
using FourVector = ROOT::Math::PxPyPzMVector;
using namespace ROOT::VecOps;
bool getMuonTrackPairIndex(int leadingidx, int subleadingidx, Float_t num, const RVec<Float_t>& mu_idx1, const RVec<Float_t>& mu_idx2,const RVec<Float_t>& mu1_px, const RVec<Float_t>& mu1_py, const RVec<Float_t>& mu1_pz, const RVec<Float_t>& mu2_px, const RVec<Float_t>& mu2_py, const RVec<Float_t>& mu2_pz,  double muonMass_, std::vector<ROOT::Math::PxPyPzMVector> &refit_trks) { 
    int TkPairIdx = -1;
    for (int i{0}; i < num; i++) {
        if ((mu_idx1[i] == leadingidx && mu_idx2[i] == subleadingidx) || (mu_idx1[i] == subleadingidx && mu_idx2[i] == leadingidx)) {
          TkPairIdx = i;
          break;
        }
    }
    int idx = TkPairIdx;
    if (idx < 0) return false;

    if ( std::isnan(mu1_px[idx])  || std::isnan(mu2_px[idx]) ) return false;
    if ( std::isnan(mu1_py[idx])  || std::isnan(mu2_py[idx]) ) return false;
    if ( std::isnan(mu1_pz[idx]) || std::isnan(mu2_pz[idx]) ) return false;

    ROOT::Math::PxPyPzMVector muTrk1{mu1_px[idx], mu1_py[idx], mu1_pz[idx], muonMass_};
    ROOT::Math::PxPyPzMVector muTrk2{mu2_px[idx], mu2_py[idx], mu2_pz[idx], muonMass_};
    refit_trks[0]  = muTrk1.Pt() > muTrk2.Pt() ? muTrk1 : muTrk2;
    refit_trks[1] = muTrk1.Pt() > muTrk2.Pt() ? muTrk2 : muTrk1;
    return true;
}
//RVec<RVec<int>> getDileptonCand(const RVec<unsigned long>& mu_pt_sorted_idx, const RVec<Int_t>& mu_ch, const RVec<Float_t>& mu_px, const RVec<Float_t>& mu_py,const RVec<Float_t>& mu_pz, const Float_t mass, Float_t diMuonPt_, Float_t num, const RVec<Float_t>& mu_idx1, const RVec<Float_t>& mu_idx2,const RVec<Float_t>& mu1_px, const RVec<Float_t>& mu1_py, const RVec<Float_t>& mu1_pz, const RVec<Float_t>& mu2_px, const RVec<Float_t>& mu2_py, const RVec<Float_t>& mu2_pz, bool flag) {
bool getDileptonCand(const RVec<unsigned long>& mu_pt_sorted_idx, const RVec<Int_t>& mu_ch, const RVec<Float_t>& mu_px, const RVec<Float_t>& mu_py,const RVec<Float_t>& mu_pz, const Float_t mass, Float_t diMuonPt_, Float_t num, const RVec<Float_t>& mu_idx1, const RVec<Float_t>& mu_idx2,const RVec<Float_t>& mu1_px, const RVec<Float_t>& mu1_py, const RVec<Float_t>& mu1_pz, const RVec<Float_t>& mu2_px, const RVec<Float_t>& mu2_py, const RVec<Float_t>& mu2_pz, bool flag) {
	RVec<RVec<int>> mu_pairs(2);
	float maxDileptonDeltaR_ = 0.4;
    for (int i=0;i<mu_pt_sorted_idx.size();i++) {
        for (int j=i+1;j<mu_pt_sorted_idx.size();j++) { //it is Pt sorted array
            if (mu_ch[mu_pt_sorted_idx[i]] * mu_ch[mu_pt_sorted_idx[j]] >= 0) continue;
            FourVector lepton1{mu_px[mu_pt_sorted_idx[i]], mu_py[mu_pt_sorted_idx[i]], mu_pz[mu_pt_sorted_idx[i]], mass};
            FourVector lepton2{mu_px[mu_pt_sorted_idx[j]], mu_py[mu_pt_sorted_idx[j]], mu_pz[mu_pt_sorted_idx[j]], mass};
            double delR = ROOT::Math::VectorUtil::DeltaR(lepton1,lepton2);
            if ( delR > maxDileptonDeltaR_ ) continue;
            std::vector<FourVector> refittedTrks(2);
            if (!getMuonTrackPairIndex(mu_pt_sorted_idx[i], mu_pt_sorted_idx[j], num, mu_idx1, mu_idx2,mu1_px, mu1_py,mu1_pz, mu2_px, mu2_py, mu2_pz,  mass, refittedTrks)) continue; //checking if refitted tracks are present -> necessary for SV construction
            FourVector lepton1_refit = refittedTrks[0].Pt() > refittedTrks[1].Pt() ? refittedTrks[0] : refittedTrks[1];
            FourVector lepton2_refit = refittedTrks[0].Pt() > refittedTrks[1].Pt() ? refittedTrks[1] : refittedTrks[0];
            double pT { (lepton1_refit+lepton2_refit).Pt() };
            //double pT { (lepton1+lepton2).Pt() };
            if (flag) {
              if (pT < diMuonPt_) continue;
            }
	    	//mu_pairs[0].emplace_back(i);
	    	//mu_pairs[1].emplace_back(j);
		    return true;
        }
    }
    return false;
    //return mu_pairs;
}
'''
getIsolation = '''
Isolation SharedFunctions::PFIsolation(TString ptype,int trk_ind, int gen_ind, int trk_exc,  const AnalysisEvent& event, double dr_max = 0.4) {
  float nh_iso {0.0};
entries_hadron_sel= cut_hadron_sel_alt.Count()
  float ph_iso {0.0};
  float ch_iso {0.0};
  float pu_iso {0.0};
  float dr_trk_packedCandVec {99.};
  int count = 0;
  ROOT::Math::PxPyPzMVector Trk;
  if (ptype.Contains("muon")) 
    Trk.SetCoordinates(event.muonPF2PATPX[trk_ind], event.muonPF2PATPY[trk_ind], event.muonPF2PATPZ[trk_ind], 0.1056583745); //NECESSARY TO USE THE SAME LORENTZ VECTOR TYPE AND GIVE SAME QUANTITIES AS IN DEFINITION
    // Trk.SetPxPyPzE;
  else
    Trk.SetCoordinates(event.packedCandsPx[trk_ind],event.packedCandsPy[trk_ind],event.packedCandsPz[trk_ind],chsMass_);
    // Trk.SetPx(event.packedCandsPx[trk_ind]);Trk.SetPy(event.packedCandsPy[trk_ind]);Trk.SetPz(event.packedCandsPz[trk_ind]);Trk.SetM(chsMass_);
  if (verbose_) {
      std::cout<<"Check the PFCandidates for isolation!"<<std::endl;
      std::cout<<"Reco primary trk (type,index,pt,eta,phi):"<<ptype<<","<<trk_ind<<","<<Trk.Pt()<<","<<Trk.Eta()<<","<<Trk.Phi()<<std::endl;
    }
  for (int k = 0; k < event.numPackedCands; k++) {
    
    if (ptype.Contains("hadron")) {
      if ( k == trk_ind || k == trk_exc ) continue;
    }
    

    // exclude tracks from HF above? 
    
    ROOT::Math::PxPyPzEVector packedCandVec {event.packedCandsPx[k], event.packedCandsPy[k], event.packedCandsPz[k], event.packedCandsE[k]};
    ROOT::Math::PxPyPzEVector packedCandTrkVec {event.packedCandsPseudoTrkPt[k], event.packedCandsPseudoTrkEta[k], event.packedCandsPseudoTrkPhi[k], event.packedCandsE[k]};
    // packedCandTrkVec.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[k], event.packedCandsPseudoTrkEta[k], event.packedCandsPseudoTrkPhi[k], event.packedCandsE[k]);
    if (packedCandVec.Pt()<0.5) continue;
    dr_trk_packedCandVec = ROOT::Math::VectorUtil::DeltaR(Trk, packedCandVec);
    if ((verbose_) && (dr_trk_packedCandVec < dr_max)) {
      std::cout<<"Nearby trk (index,pt,eta,phi,pdg):"<<k<<","<<packedCandVec.Pt()<<","<<packedCandVec.Eta()<<","<<packedCandVec.Phi()<<","<<event.packedCandsPdgId[k]<<std::endl;
    }
    if ( std::abs(event.packedCandsPdgId[k]) == 11 ||  std::abs(event.packedCandsPdgId[k]) == 13) continue; //rejecting muon/ele tracks
    //const TLorentzVector Trk_avoid {event.packedCandsPx[trk_exc], event.packedCandsPy[trk_exc], event.packedCandsPz[trk_exc], event.packedCandsE[trk_exc]};
    if ( event.packedCandsCharge[k] == 0 ) {
      if ( dr_trk_packedCandVec < dr_max ) { 
        if (event.packedCandsPdgId[k] == 22) ph_iso += packedCandVec.Et();
        else nh_iso += packedCandVec.Et();
      }
    }
    else {
      if ( event.packedCandsFromPV[k] >= 2 ) {
        if ( dr_trk_packedCandVec < dr_max )  ch_iso += packedCandVec.Pt();
      }
      else {
        if ( dr_trk_packedCandVec  < dr_max )  pu_iso += packedCandVec.Pt();
      }
    }
  }
  struct Isolation pfiso;
  float iso  = ch_iso  + std::max( float(0.0), nh_iso+ph_iso - float(0.5*pu_iso)  );
  float RelIso;
  if (ptype.Contains("gen"))
    RelIso = iso/(event.genParPt[gen_ind]+1.0e-06);
  else
    RelIso = iso/(Trk.Pt()+1.0e-06);
  if (verbose_) std::cout<<"All the iso values (reliso, iso, ch_iso, ph_iso, nh_iso, pu_iso):"<<RelIso<<","<<iso<<","<<ch_iso<<","<<ph_iso<<","<<nh_iso<<","<<pu_iso<<std::endl;
  pfiso.chiso_ = ch_iso;
  pfiso.phiso_ = ph_iso;
  pfiso.nhiso_ = nh_iso;
  pfiso.puiso_ = pu_iso;
  pfiso.reliso_ = RelIso;
  return pfiso;
}'''


ROOT.gInterpreter.Declare(getIndices_code)
ROOT.gInterpreter.Declare(getKinematics_code)
ROOT.gInterpreter.Declare(getDileptonCand_code)
ROOT.gInterpreter.Declare(getCh_code)
ROOT.gInterpreter.Declare(getChAlt_code)
ROOT.gInterpreter.Declare(getTest_code)
ROOT.gInterpreter.Declare(getCheck_code)

muon_cuts = 'abs(muonPF2PATEta) <2.4 && muonPF2PATIsPFMuon && muonPF2PATLooseCutId && muonPF2PATPt > 5'
leadingmu_cut = 'Max(mu_pt)>30' #ordered collection
ch_trk_cuts_part1 = 'abs(packedCandsPdgId)==211 && packedCandsCharge!=0 && packedCandsHasTrackDetails==1'
ch_trk_cuts_part2 = 'lv_trk_pt>5 && abs(lv_trk_eta)<2.4'

chsMass_ = 0.493677 # 0.13957061;//pion mass
# trk_cuts = 'abs()'

trigger_definitions = rdf.Define('mu_trig','HLT_IsoMu27_v8 > 0 || HLT_IsoMu27_v9 > 0 || HLT_IsoMu27_v10 > 0 || HLT_IsoMu27_v11 > 0 || HLT_IsoMu27_v12 > 0 || HLT_IsoMu27_v13 > 0 || HLT_IsoMu27_v14 > 0')\
				.Define('met_filters', 'Flag_goodVertices > 0 && Flag_globalTightHalo2016Filter > 0 && Flag_HBHENoiseFilter > 0 && Flag_HBHENoiseIsoFilter > 0 && Flag_EcalDeadCellTriggerPrimitiveFilter > 0 && Flag_BadPFMuonFilter > 0 && Flag_ecalBadCalibFilter > 0')
cut_trig = trigger_definitions.Filter('mu_trig')
cut_met = cut_trig.Filter('met_filters')
muon_definitions = cut_met.Define('mu_pt',f'muonPF2PATPt[{muon_cuts}]')\
				.Define('mu_sel','mu_pt.size()>=2 && Max(mu_pt)>30')
cut_mu_sel = muon_definitions.Filter('mu_sel')
dimuon_definitions = cut_mu_sel.Define('mu_globalidx',f'getIndices(numMuonPF2PAT)[{muon_cuts}]')\
			.Define('mu_pt_sorted_globalidx_nocuts','ROOT::VecOps::Reverse(ROOT::VecOps::Argsort(muonPF2PATPt))')\
			.Define('mu_pt_sorted_globalidx','ROOT::VecOps::Intersect(mu_pt_sorted_globalidx_nocuts,mu_globalidx,false)')\
			.Define('mu_pt_sorted_idx','ROOT::VecOps::Reverse(ROOT::VecOps::Argsort(mu_pt))')\
			.Define('mu_charge',f'muonPF2PATCharge[{muon_cuts}]')\
			.Define('mu_pair_check','getDileptonCand(mu_pt_sorted_globalidx, muonPF2PATCharge, muonPF2PATPX, muonPF2PATPY, muonPF2PATPZ, 0.1056583745, 0, numMuonTrackPairsPF2PAT,muonTkPairPF2PATIndex1,muonTkPairPF2PATIndex2,muonTkPairPF2PATTk1Px,muonTkPairPF2PATTk1Py,muonTkPairPF2PATTk1Pz,muonTkPairPF2PATTk2Px,muonTkPairPF2PATTk2Py,muonTkPairPF2PATTk2Pz,true)')
cut_dimuon = dimuon_definitions.Filter('mu_pair_check')
########### all good above ###################
#
			# .Define('cand_trk_px','ROOT::VecOps::Take(packedCandsPx,cand_globalidx_hadronsonly_alt)')\
			# .Define('cand_trk_py','ROOT::VecOps::Take(packedCandsPy,cand_globalidx_hadronsonly_alt)')\
			# .Define('cand_trk_pz','ROOT::VecOps::Take(packedCandsPz,cand_globalidx_hadronsonly_alt)')\
			# .Define('ch_trk_pt',f'lv_trk_pt[{ch_trk_cuts_part2}]')\

hadron_definitions = cut_dimuon.Define('packedCandsPt','sqrt(packedCandsPx*packedCandsPx + packedCandsPy*packedCandsPy)')\
			.Define('cand_globalidx','getIndices(numPackedCands)')\
			.Define('cand_globalidx_hadronsonly_alt',f'cand_globalidx[{ch_trk_cuts_part1}]')\
			.Define('cand_trk_px',f'packedCandsPx[{ch_trk_cuts_part1}]')\
			.Define('cand_trk_py',f'packedCandsPy[{ch_trk_cuts_part1}]')\
			.Define('cand_trk_pz',f'packedCandsPz[{ch_trk_cuts_part1}]')\
			.Define('ch_trk_pdgid',f'packedCandsPdgId[{ch_trk_cuts_part1}]')\
			.Define('ch_trk_charge',f'packedCandsCharge[{ch_trk_cuts_part1}]')\
			.Define('ch_trk_details',f'packedCandsHasTrackDetails[{ch_trk_cuts_part1}]')\
			.Define('ch_trk_mass',f'{chsMass_}')\
			.Define('ch_trk_sel_part1',ch_trk_cuts_part1)\
			.Define('LVs',f'makeLVs(ch_trk_pdgid, ch_trk_charge, ch_trk_details, cand_trk_px, cand_trk_py, cand_trk_pz, {chsMass_}, true)')\
			.Define('LVsAlt',f'makeLVsAlt(ch_trk_pdgid, ch_trk_charge, ch_trk_details, cand_trk_px, cand_trk_py, cand_trk_pz, {chsMass_}, eventRun,eventLumiblock,eventNum, true)')\
			.Define('lv_trk_pt','getKinematics(LVs, "pt")')\
			.Define('lv_trk_eta','getKinematics(LVs, "eta")')\
      .Define('lv_trk_pt_alt','getKinematics(LVsAlt, "pt")')\
			.Define('lv_trk_eta_alt','getKinematics(LVsAlt, "eta")')\
			.Define('ch_trk_pt',f'lv_trk_pt[{ch_trk_cuts_part2}]')\
			.Define('ch_trk_eta',f'lv_trk_eta[{ch_trk_cuts_part2}]')\
			.Define('LVs_sel',f'LVs[{ch_trk_cuts_part2}]')\
			.Define('ch_trk_sel','LVs_sel.size()>=2')\
			.Define('ch_trk_sel_alt','LVsAlt.size()>=2')\
			.Define('check','PrintInformation(LVs_sel,LVsAlt,eventRun,eventLumiblock,eventNum)')

			# .Define('check','getEvent(LVs_sel,LVsAlt,eventRun,eventLumiblock,eventNum)')

      ##### somehow garbage values are filled up above #########



			# .Define('ch_trk_sel_alt',f'makeLVs(packedCandsPdgId, packedCandsCharge, packedCandsHasTrackDetails, packedCandsPx, packedCandsPy, packedCandsPz, {chsMass_}, true)')\
			# .Define('ch_trk_sel_alt2',f'makeLVs(ch_trk_pdgid, ch_trk_charge, ch_trk_details, cand_trk_px, cand_trk_py, cand_trk_pz, {chsMass_}, true)')
cut_hadron_sel = hadron_definitions.Filter('ch_trk_sel') 
cut_hadron_sel_alt = hadron_definitions.Filter('ch_trk_sel_alt') 
cut_hadron_check = hadron_definitions.Filter('check') 
# cut_hadron_sel_alt = hadron_definitions.Filter('ch_trk_sel_alt')
# cut_hadron_sel_alt2 = hadron_definitions.Filter('ch_trk_sel_alt2')
############## Use Map to check if elements match between two vectors ###############
dihadron_definitions = cut_hadron_sel.Define('cand_globalidx_hadronsonly',f'getIndices(numPackedCands)[{ch_trk_cuts_part1}]')\
						.Define('ch_globalidx',f'cand_globalidx_hadronsonly[{ch_trk_cuts_part2}]')\
						.Define('testing','getSize(ch_trk_pt,ch_trk_eta,cand_globalidx_hadronsonly,ch_trk_pdgid)')\
						.Define('ch_pair_check',f'getDileptonCand(ch_globalidx, packedCandsCharge, packedCandsPx, packedCandsPy, packedCandsPz, {chsMass_}, 20, numChsTrackPairs,chsTkPairIndex1,chsTkPairIndex2,chsTkPairTk1Px,chsTkPairTk1Py,chsTkPairTk1Pz,chsTkPairTk2Px,chsTkPairTk2Py,chsTkPairTk2Pz,true)')

						# .Define('ch_pt_sorted_globalidx_nocuts','ROOT::VecOps::Reverse(ROOT::VecOps::Argsort(packedCandsPt))')\
						# .Define('ch_pt_sorted_globalidx','ROOT::VecOps::Intersect(ch_pt_sorted_globalidx_nocuts,ch_globalidx,false)')\
						# .Define('ch_pair_check',f'getDileptonCand(ch_pt_sorted_globalidx, packedCandsCharge, packedCandsPx, packedCandsPy, packedCandsPz, {chsMass_}, 20, numChsTrackPairs,chsTkPairIndex1,chsTkPairIndex2,chsTkPairTk1Px,chsTkPairTk1Py,chsTkPairTk1Pz,chsTkPairTk2Px,chsTkPairTk2Py,chsTkPairTk2Pz,true)')
			# .Define('mu_px',f'muonPF2PATPX[{muon_cuts}]')\
			# .Define('mu_py',f'muonPF2PATPY[{muon_cuts}]')\
			# .Define('mu_pz',f'muonPF2PATPZ[{muon_cuts}]')\
			# .Define('mu_LVs','makeLVs(mu_px,mu_py,mu_pz,0.1056583745)')\
			# .Define('mu_eta','getKinematics(mu_LVs, "pt")')\
			# .Define('mu_phi','getKinematics(mu_LVs, "phi")')
# cut_ch_trk_sel = ch_trk_definitions.Filter('ch_trk_sel')
#.Define('mu_pt_sorted_globalidx',f'ROOT::VecOps::Reverse(ROOT::VecOps::Argsort(muonPF2PATPt))')\

# below is for a different version where all combos are returned
# dimuon_definitions = cut_ch_trk_sel.Define('mu_charge',f'muonPF2PATCharge[{muon_cuts}]')\
# 					.Define('mu_pt_test','ROOT::VecOps::Take(muonPF2PATPt,mu_pt_sorted_globalidx)')\
# 					.Define('mu_pairs_idx','getDileptonCand(mu_pt_sorted_globalidx, muonPF2PATCharge, muonPF2PATPX, muonPF2PATPY, muonPF2PATPZ, 0.1056583745, 0, true)')\
# 										.Define('mu_leading_idx','mu_pairs_idx[0]')\
# 					.Define('mu_subleading_idx','mu_pairs_idx[1]')\
# 					.Define('mu_leading_pt','ROOT::VecOps::Take(muonPF2PATPt,mu_pairs_idx[0])')\
# 					.Define('mu_subleading_pt','ROOT::VecOps::Take(muonPF2PATPt,mu_pairs_idx[1])')\
# 					.Define('mu_leading_idx','mu_pairs_idx[0]')\
# 					.Define('mu_subleading_idx','mu_pairs_idx[1]')\
# 					.Define('mu_leading_pt','ROOT::VecOps::Take(muonPF2PATPt,mu_pairs_idx[0])')\
# 					.Define('mu_subleading_pt','ROOT::VecOps::Take(muonPF2PATPt,mu_pairs_idx[1])')\
# 					.Define('mu_leading_refit_idx1','ROOT::VecOps::Intersect(muonTkPairPF2PATIndex1,mu_leading_idx)')\
# 					.Define('mu_leading_refit_idx1','ROOT::VecOps::Intersect(muonTkPairPF2PATIndex2,mu_leading_idx)')\
# 					.Define('mu_subleading_refit_idx','ROOT::VecOps::Intersect(muonTkPairPF2PATIndex2,mu_subleading_idx)')\
# 					.Define('mu_subleading_refit_idx','ROOT::VecOps::Intersect(muonTkPairPF2PATIndex2,mu_subleading_idx)')\

# 					# .Define('mu_pair_dr','ROOT::VecOps::DeltaR(mu_eta[mu_charge>0],mu_eta[mu_charge<0],mu_phi[mu_charge>0],mu_phi[mu_charge<0])')
# # .Define('mu_idx','ROOT::VecOps::Where(,muonPF2PATPt,-1)')\
cut_dihadron = dihadron_definitions.Filter('ch_pair_check')
# cut_test = dihadron_definitions.Filter('testing')

b_list = open('BranchList2017MC.txt','r').read().split('\n')
# print(b_list)
cut_dihadron.Snapshot("makeTopologyNtupleMiniAOD/tree",args.output,b_list)
print("Time taken: --- %s seconds ---" % (time.time() - start_time))


# test and non-test match (distributions), check what happens while finding pairs
# histB = dimuon_definitions.Histo1D('mu_pt')
# histB.Draw()
# time.sleep(20)
# histC = dimuon_definitions.Histo1D('mu_pt_test')
# histC.Draw()
# time.sleep(30)
# histD = dimuon_definitions.Histo1D('mu_pt_sorted_globalidx')
# histD.Draw()
# time.sleep(10)
entries_trig = cut_trig.Count()
entries_met= cut_met.Count()
entries_mu_sel= cut_mu_sel.Count()
entries_dimuon= cut_dimuon.Count()
# entries_hadron_sel_part1= cut_hadron_sel_part1.Count()
# entries_hadron_sel_alt= cut_hadron_sel_alt.Count()
# entries_hadron_sel_alt2 = cut_hadron_sel_alt2.Count()
entries_hadron_sel = cut_hadron_sel.Count()
entries_dihadron= cut_dihadron.Count()
entries_test = cut_hadron_check.Count()
# entries_test= cut_test.Count()

print ('total:',entries_total.GetValue())
print('After trigger cut:',entries_trig.GetValue())
print('After MET filters:',entries_met.GetValue())
print('After muon selection:',entries_mu_sel.GetValue())
print('After dimuon cut:',entries_dimuon.GetValue())
# print('After hadron selection part 1:',entries_hadron_sel_part1.GetValue())
print('After hadron selection:',entries_hadron_sel.GetValue())
# print('After hadron selection alt:',entries_hadron_sel_alt.GetValue())
# print('After hadron selection alt2:',entries_hadron_sel_alt2.GetValue())
print('After dihadron cut:',entries_dihadron.GetValue())
# print ('after filter:',entries_filtered.GetValue())
# histA = rdf.Histo1D("numMuonPF2PAT")
# histA.Draw()
# outFile = ROOT.TFile(args.output, "RECREATE")
# outFile.cd()
# histA.Write()

