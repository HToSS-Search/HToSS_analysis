import ROOT

getInfoStruct_code = '''
using FourVector = ROOT::Math::PxPyPzMVector;
struct Isolation
{
  int PairIdx;
  std::vector<int> defIdx(2);
  // std::vector<int> refitIdx(2);
  std::vector<FourVector> refittedTrks(2);
  std::vector<FourVector> defTrks(2);
  std::vector<float> pfiso(5);
  
};
'''
def gInterpreter_lifetime_reweight():
    getLifetimeReweight_code = '''
        using FourVectorPtEtaPhiEVector = ROOT::Math::PtEtaPhiEVector;
        using FourVectorPtEtaPhiMVector = ROOT::Math::PtEtaPhiMVector;
        Float_t getLifetimeReweight(float old_lt, float new_lt, float L_scalar1, float L_scalar2, FourVectorPtEtaPhiMVector& vec_scalar1, FourVectorPtEtaPhiMVector& vec_scalar2, float mass){
            /* MULTIPLIED BY 10 BECAUSE TYPICALLY UNITS OF VTX ARE IN CM */
            float beta1 = vec_scalar1.Beta();
            //float gamma1 = 1/(sqrt(1-beta1*beta1));
            float gamma1 = vec_scalar1.Gamma();
            float betagamma_1 = vec_scalar1.P()/mass;
            float t1 = 10*L_scalar1/(betagamma_1); 
            float beta2 = vec_scalar2.Beta();
            //float gamma2 = 1/(sqrt(1-beta2*beta2));
            float gamma2 = vec_scalar2.Gamma();
            float betagamma_2 = vec_scalar2.P()/mass;
            // std::cout<<"betagamma1,M,P,beta*gamma:"<<betagamma_1<<","<<vec_scalar1.M()<<","<<vec_scalar1.P()<<","<<(beta1*gamma1)<<std::endl;
            // std::cout<<"betagamma2,M,P,beta*gamma:"<<betagamma_2<<","<<vec_scalar2.M()<<","<<vec_scalar2.P()<<","<<(beta2*gamma2)<<std::endl;
            // std::cout<<"old_lt,new_lt:"<<old_lt<<","<<new_lt<<std::endl;
            float t2 = 10*L_scalar2/(betagamma_2);
            Float_t weight = pow((old_lt/new_lt),2)*exp((t1+t2)*((1/old_lt) - (1/new_lt)));
            // Float_t weight = old_lt/new_lt*exp((t2)*((1/old_lt) - (1/new_lt)));
            // std::cout<<"L_scalar1,beta1,gamma1,t1:"<<L_scalar1<<","<<beta1<<","<<gamma1<<","<<t1<<std::endl;
            // std::cout<<"L_scalar2,beta2,gamma2,t2:"<<L_scalar2<<","<<beta2<<","<<gamma2<<","<<t2<<std::endl;
            return weight;
        }
    '''
    getLifetime_code = '''
        float getLifetime(unsigned int slot, const ROOT::RDF::RSampleInfo &id) {
            
            //std::cout<<id.AsString()<<std::endl;
            //std::cout<<id.Contains("_ctauS1_")<<","<<id.Contains("_ctauS10_")<<std::endl;
            if (id.Contains("_ctauS0_")) return 0.1;
            else if (id.Contains("_ctauS100_")) return 100;
            else if (id.Contains("_ctauS10_")) return 10;
            else if (id.Contains("_ctauS1_")) return 1;
            else return -99;
        }
    '''
    ROOT.gInterpreter.Declare(getLifetimeReweight_code)
    ROOT.gInterpreter.Declare(getLifetime_code)

# getEvtWeight_code = '''
# Float_t getEvtWt(float cs, float sum_wts, float MCweight, bool isData=false) {
# 	float evt_wt = 1.;
# 	if (isData) {
# 		return 1.;
# 	}
# 	else {
# 		evt_wt = (cs/sum_wts)*MCweight;
# 		return evt_wt;
# 	}
# }
# '''
def gInterpreter_std_map():
    ROOT.gInterpreter.Declare("""
    auto &GetTheMap() {
        static std::unordered_map<float, float> theGlobalMap;
        return theGlobalMap;
    }
    """) # defines a std::map in C++ and returns it with address -> modify this std::map as theGlobalMap[x]=y and the change will
    # appear globally. Hence, next time you call GetTheMap, you will access std::map which has new values taken acc. to above

def gInterpreter_Indices():
    getIndices_code = '''
        using namespace ROOT::VecOps;
        RVec<unsigned long> getIndices(const int num) {
            RVec<unsigned long> idx;
            for (int i = 0;i<num;i++) idx.emplace_back(i);
            return idx;
        }
    '''
    ROOT.gInterpreter.Declare(getIndices_code)
def gInterpreter_Kinematics():
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
    ROOT.gInterpreter.Declare(getKinematics_code)
def gInterpreter_ChTrkSelection():
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
    ROOT.gInterpreter.Declare(getCh_code)
def gInterpreter_PFIsolation():
    getPFIsolation_code = '''
        using FourVectorPxPyPzE = ROOT::Math::PxPyPzEVector;
        using namespace ROOT::VecOps;

        RVec<Float_t> PFIsolation(TString ptype, FourVector &Trk, int trk_ind, int trk_exc, 
        const RVec<Float_t> packedCandsPx, const RVec<Float_t> packedCandsPy, const RVec<Float_t> packedCandsPz, const RVec<Float_t> packedCandsE, 
        const RVec<Int_t> packedCandsCharge, const RVec<Int_t> packedCandsPdgId, const RVec<Int_t> packedCandsFromPV, Int_t numPackedCands, double dr_max = 0.4) {
        float nh_iso {0.0};
        float ph_iso {0.0};
        float ch_iso {0.0};
        float pu_iso {0.0};
        float dr_trk_packedCandVec {99.};
        RVec<Float_t> pfiso(5);
        int count = 0;
        bool verbose_=false;
        if (verbose_) {
            std::cout<<"Check the PFCandidates for isolation!"<<std::endl;
            std::cout<<"Reco primary trk (type,index,pt,eta,phi):"<<ptype<<","<<trk_ind<<","<<Trk.Pt()<<","<<Trk.Eta()<<","<<Trk.Phi()<<std::endl;
            }
        for (int k = 0; k < numPackedCands; k++) {
            if (ptype.Contains("hadron")) {
            if ( k == trk_ind || k == trk_exc ) continue;
            }

            FourVectorPxPyPzE packedCandVec {packedCandsPx[k], packedCandsPy[k], packedCandsPz[k], packedCandsE[k]};
            if (packedCandVec.Pt()<0.5) continue;
            dr_trk_packedCandVec = ROOT::Math::VectorUtil::DeltaR(Trk, packedCandVec);
            if ((verbose_) && (dr_trk_packedCandVec < dr_max)) {
            std::cout<<"Nearby trk (index,pt,eta,phi,pdg):"<<k<<","<<packedCandVec.Pt()<<","<<packedCandVec.Eta()<<","<<packedCandVec.Phi()<<","<<packedCandsPdgId[k]<<std::endl;
            }
            if ( std::abs(packedCandsPdgId[k]) == 11 ||  std::abs(packedCandsPdgId[k]) == 13) continue; //rejecting muon/ele tracks
            if ( packedCandsCharge[k] == 0 ) {
            if ( dr_trk_packedCandVec < dr_max ) { 
                if (packedCandsPdgId[k] == 22) ph_iso += packedCandVec.Et();
                else nh_iso += packedCandVec.Et();
            }
            }
            else {
            if ( packedCandsFromPV[k] >= 2 ) {
                if ( dr_trk_packedCandVec < dr_max )  ch_iso += packedCandVec.Pt();
            }
            else {
                if ( dr_trk_packedCandVec  < dr_max )  pu_iso += packedCandVec.Pt();
            }
            }
        }
        float iso  = ch_iso  + std::max( float(0.0), nh_iso+ph_iso - float(0.5*pu_iso)  );
        float RelIso;
        RelIso = iso/(Trk.Pt()+1.0e-06);
        if (verbose_) std::cout<<"All the iso values (reliso, iso, ch_iso, ph_iso, nh_iso, pu_iso):"<<RelIso<<","<<iso<<","<<ch_iso<<","<<ph_iso<<","<<nh_iso<<","<<pu_iso<<std::endl;
        pfiso[0]=RelIso;
        pfiso[1]=ch_iso;
        pfiso[2]=nh_iso;
        pfiso[3]=ph_iso;
        pfiso[4]=pu_iso;
        return pfiso;
        }
    '''
    ROOT.gInterpreter.Declare(getPFIsolation_code)
def gInterpreter_diObjectCandidate():
    getDileptonCand_code = '''
        using FourVector = ROOT::Math::PxPyPzMVector;
        using namespace ROOT::VecOps;
        int getMuonTrackPairIndex(int leadingidx, int subleadingidx, Float_t num, const RVec<Float_t>& mu1_idx, const RVec<Float_t>& mu2_idx,const RVec<Float_t>& mu1_px, const RVec<Float_t>& mu1_py, const RVec<Float_t>& mu1_pz, const RVec<Float_t>& mu2_px, const RVec<Float_t>& mu2_py, const RVec<Float_t>& mu2_pz,  double muonMass_, std::vector<ROOT::Math::PxPyPzMVector> &refit_trks) { 
            int TkPairIdx = -1;
            for (int i{0}; i < num; i++) {
                if ((mu1_idx[i] == leadingidx && mu2_idx[i] == subleadingidx) || (mu1_idx[i] == subleadingidx && mu2_idx[i] == leadingidx)) {
                TkPairIdx = i;
                break;
                }
            }
            int idx = TkPairIdx;
            if (idx < 0) return -1;

            if ( std::isnan(mu1_px[idx])  || std::isnan(mu2_px[idx]) ) return -1;
            if ( std::isnan(mu1_py[idx])  || std::isnan(mu2_py[idx]) ) return -1;
            if ( std::isnan(mu1_pz[idx]) || std::isnan(mu2_pz[idx]) ) return -1;

            ROOT::Math::PxPyPzMVector muTrk1{mu1_px[idx], mu1_py[idx], mu1_pz[idx], muonMass_};
            ROOT::Math::PxPyPzMVector muTrk2{mu2_px[idx], mu2_py[idx], mu2_pz[idx], muonMass_};
            if (mu1_idx[idx] == leadingidx && mu2_idx[idx] == subleadingidx) {
                refit_trks[0] = muTrk1;
                refit_trks[1] = muTrk2;
            }
            else {
                refit_trks[0]  = muTrk2;
                refit_trks[1]  = muTrk1;
            }
            return idx;
        }


        std::vector<Int_t> getDileptonCand(const RVec<unsigned long>& mu_pt_sorted_idx, const RVec<Int_t>& mu_ch, const RVec<Float_t>& mu_px, const RVec<Float_t>& mu_py,const RVec<Float_t>& mu_pz, const Float_t mass,  
        Float_t num, const RVec<Float_t>& mu1_idx, const RVec<Float_t>& mu2_idx, 
        const RVec<Float_t>& mu1_px, const RVec<Float_t>& mu1_py, const RVec<Float_t>& mu1_pz, 
        const RVec<Float_t>& mu2_px, const RVec<Float_t>& mu2_py, const RVec<Float_t>& mu2_pz,
        TString ptype, const RVec<Float_t>& packedCandsPx, const RVec<Float_t>& packedCandsPy, const RVec<Float_t>& packedCandsPz, const RVec<Float_t>& packedCandsE, 
        const RVec<Int_t>& packedCandsCharge, const RVec<Int_t>& packedCandsPdgId, const RVec<Int_t>& packedCandsFromPV, Int_t numPackedCands,
        ROOT::Math::PxPyPzMVector& leadinglv, ROOT::Math::PxPyPzMVector& subleadinglv, Float_t diMuonPt_=0, double dr_max = 0.4) {
            std::vector<Int_t> objidx(3);
            objidx[0]=-1;
            objidx[1]=-1;
            objidx[2]=-1;
            float maxDileptonDeltaR_ = 0.4;
            for (int i=0;i<mu_pt_sorted_idx.size();i++) {
                for (int j=i+1;j<mu_pt_sorted_idx.size();j++) { //it is Pt sorted array
                    if (mu_ch[mu_pt_sorted_idx[i]] * mu_ch[mu_pt_sorted_idx[j]] >= 0) continue;
                    FourVector lepton1{mu_px[mu_pt_sorted_idx[i]], mu_py[mu_pt_sorted_idx[i]], mu_pz[mu_pt_sorted_idx[i]], mass};
                    FourVector lepton2{mu_px[mu_pt_sorted_idx[j]], mu_py[mu_pt_sorted_idx[j]], mu_pz[mu_pt_sorted_idx[j]], mass};
                    double delR = ROOT::Math::VectorUtil::DeltaR(lepton1,lepton2);
                    if ( delR > maxDileptonDeltaR_ ) continue;
                    std::vector<FourVector> refittedTrks(2);
                    int TkPairIdx = getMuonTrackPairIndex(mu_pt_sorted_idx[i], mu_pt_sorted_idx[j], num, mu1_idx, mu2_idx,mu1_px, mu1_py,mu1_pz, mu2_px, mu2_py, mu2_pz,  mass, refittedTrks);
                    if (TkPairIdx < 0) continue; //checking if refitted tracks are present -> necessary for SV construction

                    int leadingidx = refittedTrks[0].Pt() > refittedTrks[1].Pt() ? mu_pt_sorted_idx[i] : mu_pt_sorted_idx[j];
                    int subleadingidx = refittedTrks[0].Pt() > refittedTrks[1].Pt() ? mu_pt_sorted_idx[j] : mu_pt_sorted_idx[i];
                    FourVector lepton1_refit = refittedTrks[0].Pt() > refittedTrks[1].Pt() ? refittedTrks[0] : refittedTrks[1];
                    FourVector lepton2_refit = refittedTrks[0].Pt() > refittedTrks[1].Pt() ? refittedTrks[1] : refittedTrks[0];

                    double pT { (lepton1_refit+lepton2_refit).Pt() };
                    if (pT < diMuonPt_) continue;
                    // RVec<Float_t> leadingMuonIso;
                    // RVec<Float_t> subleadingMuonIso;
                    // leadingMuonIso = PFIsolation(ptype, lepton1_refit, leadingidx, subleadingidx, packedCandsPx, packedCandsPy, packedCandsPz, packedCandsE, packedCandsCharge,packedCandsPdgId, packedCandsFromPV, numPackedCands, 0.4);
                    // subleadingMuonIso = PFIsolation(ptype, lepton2_refit, subleadingidx, leadingidx, packedCandsPx, packedCandsPy, packedCandsPz, packedCandsE, packedCandsCharge,packedCandsPdgId, packedCandsFromPV, numPackedCands, 0.4);
                    // if (flag) {
                        // std::cout<<"enters isolation check"<<std::endl;
                        // if ( leadingMuonIso[0] >  leadingRelIso ) continue;
                        // if ( subleadingMuonIso[0] > subleadingRelIso ) continue;
                        // std::cout<<"Isolation check:"<<ptype<<","<<flag<<","<<leadingMuonIso[0]<<","<<subleadingMuonIso[0]<<std::endl;
                    // }

                    objidx[0] = leadingidx;
                    objidx[1] = subleadingidx;
                    objidx[2] = TkPairIdx;
                    leadinglv = lepton1_refit;
                    subleadinglv = lepton2_refit;
                    return objidx;
                }
            }
            // std::cout<<"objidx:"<<objidx[0]<<","<<objidx[1]<<std::endl;
            return objidx;
        }
        '''
    ROOT.gInterpreter.Declare(getDileptonCand_code)
def gInterpreter_diObjectMassCheck():
    getMassCompatibility_code = '''
        using namespace ROOT::VecOps;

        bool MassCompatibility(double dihadron_mass, double dimuon_mass, RVec<float> params_lower, RVec<float> params_higher) {
        // if (!flag) return true;
        TF1 func_lowerbound_("func_lowerbound","[1]*x+[0]",0.,10.);
        TF1 func_higherbound_("func_higherbound","[1]*x+[0]",0.,10.);
        func_lowerbound_.FixParameter(0,params_lower[0]);
        func_lowerbound_.FixParameter(1,params_lower[1]);
        func_higherbound_.FixParameter(0,params_higher[0]);
        func_higherbound_.FixParameter(1,params_higher[1]);
        double mass_upper = func_higherbound_.Eval(dimuon_mass);
        double mass_lower = func_lowerbound_.Eval(dimuon_mass);
        if ((dihadron_mass>=mass_lower) && (dihadron_mass<=mass_upper)) return true;
        else return false;
        }
        '''
    ROOT.gInterpreter.Declare(getMassCompatibility_code)
def gInterpreter_diObjectLxy():
    getLxy_code = '''
        using namespace ROOT::VecOps;
        RVec<double> getLxy(double pvCov00, double pvCov01,double pvCov02, double pvCov10, double pvCov11, double pvCov12, double pvCov20, double pvCov21, double pvCov22, double pvX, double pvY,
                            double svCov00, double svCov01,double svCov02, double svCov10, double svCov11, double svCov12, double svCov20, double svCov21, double svCov22, double svX, double svY) {
            double pvCov_arr[9] = {pvCov00, pvCov01, pvCov02, pvCov10, pvCov11, pvCov12, pvCov20, pvCov21, pvCov22};
            double svCov_arr[9] = {svCov00, svCov01, svCov02, svCov10, svCov11, svCov12, svCov20, svCov21, svCov22};
            ROOT::Math::SVector<double, 3>  distVecXY(svX - pvX, svY - pvY, 0.);
            ROOT::Math::SMatrix<double,3> svCov(svCov_arr,9); // Asymmetric!!! Esp. in 10 & 01
            ROOT::Math::SMatrix<double,3> pvCov(pvCov_arr,9);
            ROOT::Math::SMatrix<double,3> totalCov = pvCov + svCov;
            double distMagXY = ROOT::Math::Mag(distVecXY);
            double similarity_val_orig = ROOT::Math::Similarity(totalCov, distVecXY/distMagXY); //square root to be taken below
            double similarity_val = similarity_val_orig; //square root to be taken below
            double ij_elem, ji_elem;
            double dummy;
            int numberoftries = 6;
            int ctr=0;
            bool flag = true;
            bool use_ij = true;
            
            // std::cout<<"New event"<<std::endl;
            // checking if symmetric or not
            for (int i=0;i<3;i++) {
                for (int j=i+1;j<3;j++){
                    ij_elem = totalCov(i,j);
                    ji_elem = totalCov(j,i);
                    if (abs(ij_elem-ji_elem) > 1e-8) {
                        //std::cout<<"not symmetric(i,j):"<<i<<","<<j<<std::endl;
                        flag=false;
                        break;
                    }
                }
            if (!flag) break;
            }
            //if (!flag) std::cout<<"not symmetric"<<std::endl;
            // if ((flag)&&(similarity_val_orig<0)) std::cout<<"Comes from positive semi-definite"<<std::endl;
            // std::cout<<"Stuck in loop?"<<std::endl;
            
            ROOT::Math::SMatrix<double,3> totalCov_dummy, totalCov_dummy1, totalCov_dummy2;
            double similarity_val1, similarity_val2;
            totalCov_dummy = totalCov;
            while ((ctr < 3) && (!flag)) {
                totalCov_dummy1 = totalCov_dummy;
                totalCov_dummy2 = totalCov_dummy;
                // std::cout<<"Stuck in loop?"<<std::endl;

                for (int i=ctr;i<3;i++) {
                    for (int j=i+1;j<3;j++){
                        ij_elem = totalCov_dummy(i,j);
                        ji_elem = totalCov_dummy(j,i);
                        if (abs(ij_elem-ji_elem) > 1e-8) {
                            totalCov_dummy1(i,j) = totalCov_dummy(j,i);
                            totalCov_dummy2(j,i) = totalCov_dummy(i,j);
                            similarity_val1 = ROOT::Math::Similarity(totalCov_dummy1, distVecXY/distMagXY);
                            similarity_val2 = ROOT::Math::Similarity(totalCov_dummy2, distVecXY/distMagXY);
                            if (similarity_val1 > similarity_val2)
                                use_ij = true;
                            else
                                use_ij = false;
                        }
                    }
                    if (use_ij) totalCov_dummy = totalCov_dummy1;
                    else totalCov_dummy = totalCov_dummy2;
                }
                similarity_val = ROOT::Math::Similarity(totalCov_dummy, distVecXY/distMagXY);
                ctr++;
            }
            // flag=true;
            if (!flag) {
                for (int i=0;i<3;i++) {
                    for (int j=i+1;j<3;j++){
                        ij_elem = totalCov_dummy(i,j);
                        ji_elem = totalCov_dummy(j,i);
                        if (abs(ij_elem-ji_elem) > 1e-8) {
                            std::cout<<"After procedure, not symmetric(i,j):"<<i<<","<<j<<std::endl;
                            flag=false;
                            // break;
                        }
                    }
                // if (!flag) break;
                }
            }
            /*if (!flag){    
                std::cout<<"Before"<<std::endl;
                std::cout<<totalCov<<std::endl;
                std::cout<<pvCov<<std::endl;
                std::cout<<svCov<<std::endl;
                std::cout<<"similarity value original:"<<similarity_val_orig<<std::endl;
                std::cout<<"After"<<std::endl;
                std::cout<<totalCov_dummy<<std::endl;
                std::cout<<"similarity value:"<<similarity_val<<std::endl;
            }*/

            // if (flag) std::cout<<"Comes from positive semi-definite"<<std::endl;
            // std::cout<<"Stuck in loop?"<<std::endl;
            /*if (similarity_val<0) {
                // std::cout<<"Covariance Matrix PV:"<<std::endl;
                // std::cout<<pvCov<<std::endl;
                //std::cout<<"Covariance Matrix SV:"<<std::endl;
                //std::cout<<svCov<<std::endl;
                std::cout<<"Similarity value:"<<similarity_val_orig<<std::endl;
                std::cout<<"Covariance Matrix Original:"<<std::endl;
                std::cout<<totalCov<<std::endl;
                std::cout<<"Covariance Matrix New:"<<std::endl;
                std::cout<<totalCov_dummy<<std::endl;
                std::cout<<"Similarity value New:"<<similarity_val<<std::endl;
            }*/
            
            totalCov = totalCov_dummy;
            // all similarity value become zero (most) -> follow up on this later -> still weird events exist -> Either every element is negative 
            // Or all diagonals are negative
            similarity_val = ROOT::Math::Similarity(totalCov, distVecXY/distMagXY); //calculate again with modified matrix
            double sigmaDistMagXY = sqrt(similarity_val);
            double significance = distMagXY/sigmaDistMagXY;
            
            
            /*if (similarity_val < 0) {
                std::cout<<"Covariance Matrix:"<<std::endl;
                std::cout<<totalCov<<std::endl;
                //bool ret = totalCov.Det2(dummy) ;
                if (totalCov.Det2(dummy))
                    std::cout<<"Determinant:"<<dummy<<std::endl;
                else
                    std::cout<<"Determinant cannot be calculated"<<std::endl;
                std::cout<<"Displacement Vector(svX-pvX,svY-pvY,0):"<<distVecXY<<std::endl;
                std::cout<<"Lxy:"<< distMagXY<<std::endl;
                std::cout<<"distVecXY^T * totalCov * distVecXY (similarity value):"<< similarity_val<<std::endl;
            }*/

            RVec<double> lxy_vec(4);
            lxy_vec[0]=distMagXY;
            lxy_vec[1]=sigmaDistMagXY;
            lxy_vec[2]=significance;
            lxy_vec[3]=similarity_val;
            return lxy_vec;
        }
        '''
    ROOT.gInterpreter.Declare(getLxy_code)
# ROOT.gInterpreter.Declare(getEvtWeight_code)

def gInterpreter_MatchReco():
    getMatchReco_code='''
        using FourVectorPtEtaPhiM = ROOT::Math::PtEtaPhiMVector;
        using FourVectorPtEtaPhiE = ROOT::Math::PtEtaPhiEVector;
        using FourVectorPxPyPzM = ROOT::Math::PxPyPzMVector;
        using namespace ROOT::VecOps;

        Int_t AncestryCheck(Int_t k, const Int_t& parId, RVec<Int_t>& genParId, RVec<Int_t>& genParMotherId, RVec<Int_t>& genParMotherIndex) {
            Int_t NGENPARMAX=1000;
            Int_t pdgId        { abs(genParId[k]) };
            Int_t motherId     { abs(genParMotherId[k]) };
            Int_t motherIndex  { abs(genParMotherIndex[k]) };
            // if (verbose) std::cout << "Going up the ladder ... pdgId = " << pdgId << " : motherIndex = " << motherIndex << " : motherId = " << motherId << std::endl;
            if (motherId == 0 || motherIndex == -1) return -1; // if no parent, then mother Id is null and there's no index, quit search
            else if (motherId != parId) return motherId; // if parent is not particle excited, return motherId
            else if (motherIndex >= NGENPARMAX) return -1; // index exceeds stored genParticle range, return false for safety
            else {
                
        //        debugCounter++;
        //        std::cout << "debugCounter: " << debugCounter << std::endl;
                        
                    //if (verbose) std::cout << "Enters right condition ... pdgId = " << pdgId << " : motherIndex = " << motherIndex << " : motherId = " << motherId << std::endl;
                return AncestryCheck(motherIndex, parId, genParId, genParMotherId, genParMotherIndex); // otherwise check mother's mother ...
            }
        }
        Int_t AncestryCheckIdx(Int_t k, const Int_t parId, RVec<Int_t>& genParId, RVec<Int_t>& genParMotherId, RVec<Int_t>& genParMotherIndex) {
            Int_t NGENPARMAX=1000;
            Int_t pdgId        { abs(genParId[k]) };
            Int_t motherId     { abs(genParMotherId[k]) };
            Int_t motherIndex  { abs(genParMotherIndex[k]) };
            //std::cout << "Going up the ladder ... k = " << k << " : pdgId = " << pdgId << " : motherIndex = " << motherIndex << " : motherId = " << motherId << std::endl;
            if (motherId == 0 || motherIndex == -1) return -1; // if no parent, then mother Id is null and there's no index, quit search
            else if (motherId != parId) return motherIndex; // if parent is not particle excited, return motherId
            else if (motherIndex >= NGENPARMAX) return -1; // index exceeds stored genParticle range, return false for safety
            else {
                
        //        debugCounter++;
        //        std::cout << "debugCounter: " << debugCounter << std::endl;
                        
                //std::cout << "Enters right condition ... pdgId = " << pdgId << " : motherIndex = " << motherIndex << " : motherId = " << motherId << std::endl;
                return AncestryCheckIdx(motherIndex, parId, genParId, genParMotherId, genParMotherIndex); // otherwise check mother's mother ...
            }
        }
        Int_t MatchReco(FourVectorPxPyPzM &reco_lv,RVec<Float_t>& genParPt,RVec<Float_t>& genParEta,RVec<Float_t>& genParPhi,RVec<Float_t>& genParE,RVec<Int_t>& genParId,RVec<Int_t>& genParMotherId) {
            double minDR = 100;
            unsigned index = 0;
            double dr_max = 0.03;
            double delR=minDR;
            //std::cout<<"Stuck at index:"<<gen_ind<<std::endl;
            for (int i=0; i<genParPt.size(); i++) {
                FourVectorPtEtaPhiE genP {genParPt[i],genParEta[i],genParPhi[i],genParE[i]};
                delR = ROOT::Math::VectorUtil::DeltaR(reco_lv,genP);
                //std::cout<<"GenIndex, Pt, Charge, ID: "<<gen_ind<<", "<<event.genParPt[gen_ind]<<", "<<event.genParCharge[gen_ind]<<", "<<event.genParId[gen_ind]<<";"<<std::endl;
                //std::cout<<"PCdIndex, Pt, Charge, ID: "<<j<<", "<<packedCand.Pt()<<",  "<<event.packedCandsCharge[j]<<", "<<event.packedCandsPdgId[j]<<";"<<std::endl;
                //std::cout<<"Check the dR on this: "<<delR<<std::endl;
                //std::cout<<"GenIndex: "<<gen_ind<<", PackedCandIndex: "<<j<<std::endl;
                if (minDR < delR) continue;
                minDR = delR;
                index = i;
            }
            //std::cout<<"Index, PDGID, MPDGID: "<<index<<", "<<genParId[index]<<", "<<genParMotherId[index]<<";"<<std::endl;

            if (minDR < dr_max) 
                return index; //abs(genParId[index])
            else
                return 0;
        }
        Int_t MatchReco(FourVectorPxPyPzM &reco_lv1,FourVectorPxPyPzM &reco_lv2,RVec<Float_t>& genParPt,RVec<Float_t>& genParEta,RVec<Float_t>& genParPhi,RVec<Float_t>& genParE,RVec<Int_t>& genParId,RVec<Int_t>& genParStatus,RVec<Int_t>& genParMotherId,RVec<Int_t>& genParMotherIndex) {
            double minDR = 100;
            unsigned index = 0;
            double dr_max = 0.03;
            double delR=minDR;
            int idx1=-1;
            int idx2=-1;
            int pdgID_idx1=-1;
            int pdgID_idx2=-1;
            //std::cout<<"Stuck at index:"<<gen_ind<<std::endl;
            for (int i=0; i<genParPt.size(); i++) {
                if (genParStatus[i]!=1) continue;
                FourVectorPtEtaPhiE genP {genParPt[i],genParEta[i],genParPhi[i],genParE[i]};
                delR = ROOT::Math::VectorUtil::DeltaR(reco_lv1,genP);
                //std::cout<<"GenIndex, Pt, Charge, ID: "<<gen_ind<<", "<<event.genParPt[gen_ind]<<", "<<event.genParCharge[gen_ind]<<", "<<event.genParId[gen_ind]<<";"<<std::endl;
                //std::cout<<"PCdIndex, Pt, Charge, ID: "<<j<<", "<<packedCand.Pt()<<",  "<<event.packedCandsCharge[j]<<", "<<event.packedCandsPdgId[j]<<";"<<std::endl;
                //std::cout<<"Check the dR on this: "<<delR<<std::endl;
                //std::cout<<"GenIndex: "<<gen_ind<<", PackedCandIndex: "<<j<<std::endl;
                if (minDR < delR) continue;
                minDR = delR;
                index = i;
            }
            if (minDR < dr_max) {
                idx1=index;
                pdgID_idx1=abs(genParId[index]);
            }
            delR=minDR;
            minDR = 100;
            index=0;
            for (int i=0; i<genParPt.size(); i++) {
                if (genParStatus[i]!=1) continue;
                FourVectorPtEtaPhiE genP {genParPt[i],genParEta[i],genParPhi[i],genParE[i]};
                delR = ROOT::Math::VectorUtil::DeltaR(reco_lv2,genP);
                if (minDR < delR) continue;
                minDR = delR;
                index = i;
            }
            if (minDR < dr_max) {
                idx2=index;
                pdgID_idx2=abs(genParId[index]);
            }
            pdgID_idx1 = pdgID_idx1==9000006 ? 500 : pdgID_idx1;
            pdgID_idx2 = pdgID_idx2==9000006 ? 500 : pdgID_idx2;
            if ((idx1==-1) && (idx2==-1)) {
                return -399;
            }
            else if ((idx1==-1) || (idx2==-1)) {
                return pdgID_idx2*pdgID_idx1;
            }
            else if (idx1==idx2){
                //Int_t mu_motherIndex=AncestryCheckIdx(idx1,13,genParId,genParMotherId,genParMotherIndex);
                return -pdgID_idx1-400;
            }
            else {
                Int_t mu1_motherIndex=AncestryCheckIdx(idx1,13,genParId,genParMotherId,genParMotherIndex);
                Int_t mu2_motherIndex=AncestryCheckIdx(idx2,13,genParId,genParMotherId,genParMotherIndex);
                //Int_t mu1_motherIndex=-1;
                //Int_t mu2_motherIndex=-1;
                //std::cout<<"GenIndex1, GenIndex2, PDGID1, PDGID2, MPDGID1, MPDGID2: "<<idx1<<", "<<idx2<<", "<<pdgID_idx1<<", "<<pdgID_idx2<<", "<<genParMotherId[mu1_motherIndex]<<", "<<genParMotherId[mu2_motherIndex]<<";"<<std::endl;


                if ((mu1_motherIndex==mu2_motherIndex) && (mu1_motherIndex!=-1)) return abs(genParMotherId[mu1_motherIndex]);
                //if (genParMotherId[idx1]==genParMotherId[idx2]) return abs(genParMotherId[idx1]);
                else return 400;
            }
        }
    '''
    ROOT.gInterpreter.Declare(getMatchReco_code)

