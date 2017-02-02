#include "CxAODReader_VHbb/MVATree_VHbb.h"
#include "EventLoop/Worker.h"

MVATree_VHbb::MVATree_VHbb(bool persistent, bool readMVA, std::string analysisType, EL::Worker* wk, std::vector<std::string> variations, bool nominalOnly) :
        MVATree(persistent, readMVA, wk, variations, nominalOnly),
        m_analysisType(analysisType)
{
  SetBranches();
}


void MVATree_VHbb::AddBranch(TString name, Int_t* address)
{
  for (std::pair<std::string, TTree*> tree : m_treeMap) {
    tree.second -> Branch(name, address);
  }
  m_reader.AddVariable(name, address);
}

void MVATree_VHbb::AddBranch(TString name, Float_t* address)
{
  for (std::pair<std::string, TTree*> tree : m_treeMap) {
    tree.second -> Branch(name, address);
  }
  m_reader.AddVariable(name, address);
}

void MVATree_VHbb::AddBranch(TString name, ULong64_t* address)
{
  for (std::pair<std::string, TTree*> tree : m_treeMap) {
    tree.second -> Branch(name, address);
  }
  m_reader.AddVariable(name, address);
}

// void MVATree_VHbb::AddBranch(TString name, std::string* address)
// {
//   for (std::pair<std::string, TTree*> tree : m_treeMap) {
//     tree.second -> Branch(name, address);
//   }
// }

void MVATree_VHbb::SetBranches()
{
  // prepare MVA reader
  m_reader.SetSplitVar(&EventNumber);
  m_reader.AddReader("reader", 2);
  if(m_analysisType == "0lep"){
     m_reader.AddReader("0lep_2jet", 2);
     m_reader.AddReader("0lep_3jet", 2);
  }
 if(m_analysisType == "1lep"){
     m_reader.AddReader("1lep_2jet", 2);
     m_reader.AddReader("1lep_3jet", 2);
     m_reader.AddReader("1lep_2jet_v2", 2);
     m_reader.AddReader("1lep_2jet_v3", 2);
     m_reader.AddReader("1lep_3jet_v2", 2);
     m_reader.AddReader("1lep_3jet_v3", 2);
     m_reader.AddReader("1lep_2jet_WZ", 2);
     m_reader.AddReader("1lep_3jet_WZ", 2);
  }

  AddBranch("sample", &sample);

  AddBranch("EventWeight", &EventWeight);
  AddBranch("EventNumber", &EventNumber);

  // variables to use Paul's MVA code
  AddBranch("isOdd", &isOdd);
  AddBranch("weight", &weight);

  AddBranch("nJ", &nJ);
  AddBranch("nSigJet", &nSigJet);
  AddBranch("nForwardJet", &nForwardJet);
  AddBranch("mBB", &mBB);
  AddBranch("dRBB", &dRBB);
  AddBranch("dPhiBB", &dPhiBB);
  AddBranch("dEtaBB", &dEtaBB);
  if (m_analysisType == "2lep") {
    AddBranch("dEtaVBB", &dEtaVBB);
  }
  AddBranch("dPhiVBB", &dPhiVBB);
  AddBranch("dPhiMETMPT", &dPhiMETMPT);
  AddBranch("dPhiMETdijet", &dPhiMETdijet);
  AddBranch("mindPhi", &mindPhi);
  if (m_analysisType == "1lep") {
    AddBranch("dPhiLBmin", &dPhiLBmin);
    AddBranch("Mtop", &Mtop);
    AddBranch("dEtaWH", &dEtaWH);
    AddBranch("dPhiLMET", &dPhiLMET);
    AddBranch("mBB_corr", &mBB_corr);
    AddBranch("MtopFullMETShift",&MtopFullMETShift);
    AddBranch("dYWH", &dYWH);
    AddBranch("QCDWeight", &QCDWeight);
  }
  if (m_analysisType == "1lep" || m_analysisType == "2lep") {
    AddBranch("pTV", &pTV);
  }
  if (m_analysisType == "0lep") {
    AddBranch("MEff", &MEff);
    AddBranch("MEff3", &MEff3);
  }
  AddBranch("sumPt", &sumPt);
  AddBranch("pTB1", &pTB1);
  AddBranch("pTB2", &pTB2);
  AddBranch("pTBB", &pTBB);
  AddBranch("pTBBoverMET", &pTBBoverMET);
  AddBranch("etaB1", &etaB1);
  AddBranch("etaB2", &etaB2);
  if (m_analysisType == "1lep") {
    AddBranch("pTL", &pTL);
    AddBranch("etaL", &etaL);
    AddBranch("mTW", &mTW);
  }
  AddBranch("MET", &MET);
  AddBranch("MPT", &MPT);
  AddBranch("HT", &HT);
  AddBranch("METHT", &METHT);
  if (m_analysisType == "2lep") {
    AddBranch("mLL", &mLL);
  }
  AddBranch("MV1cB1", &MV1cB1);
  AddBranch("MV1cB2", &MV1cB2);
  AddBranch("MV1cJ3", &MV1cJ3);
  AddBranch("pTJ3", &pTJ3);
  AddBranch("etaJ3", &etaJ3);
  AddBranch("dRB1J3", &dRB1J3);
  AddBranch("dRB2J3", &dRB2J3);
  AddBranch("mBBJ", &mBBJ);
  AddBranch("nTag", &nTag);
  AddBranch("BDT", &BDT);
  AddBranch("BDT_WZ", &BDT_WZ);
  AddBranch("btagWeight", &btagWeight);
  AddBranch("triggerSF", &triggerSF);
  AddBranch("leptonSF", &leptonSF);
  AddBranch("PUWeight", &PUWeight);
  AddBranch("MCWeight", &MCWeight);
  //AddBranch("CSWeight", &CSWeight);


  // book MVA reader
  TString xmlFile = getenv("ROOTCOREBIN");
  if (m_analysisType == "2lep" && m_readMVA) {
    // Run 1 files can be found in /eos/atlas/atlascerngroupdisk/phys-higgs/HSG5/Run1Paper/MVATrainingFiles/
    xmlFile += "/data/CxAODReader_VHbb/TMVAClassification_BDT_LiverpoolBmham_8TeV_llbb_2tag2jet_ptv0_120_ZllH125_0of2_root5.34.05_v3.0.xml";
    m_reader.BookReader("reader", xmlFile);
  }else if(m_analysisType == "0lep" && m_readMVA){
     //TString xmlFile_2jet = xmlFile+"/data/CxAODReader_VHbb/TMVAClassification_BDT_AcSinica_8TeV_vvbb_2tag2jet_ptv120_ZvvHandWlvHandZllH125_qq_and_gg_0of2_root5.34.11_v7.xml";
     TString xmlFile_2jet = xmlFile+"/data/CxAODReader_VHbb/TMVAClassification_BDT_0lep2tag2jet.weights.xml";
     m_reader.BookReader("0lep_2jet", xmlFile_2jet);

     //TString xmlFile_3jet = xmlFile+"/data/CxAODReader_VHbb/TMVAClassification_BDT_AcSinica_8TeV_vvbb_2tag3jet_ptv120_ZvvHandWlvHandZllH125_qq_and_gg_0of2_root5.34.11_v7.xml";
     TString xmlFile_3jet = xmlFile+"/data/CxAODReader_VHbb/TMVAClassification_BDT_0lep2tag3jet.weights.xml";
     m_reader.BookReader("0lep_3jet", xmlFile_3jet);
  }else if (m_analysisType == "1lep" && m_readMVA) {
    // TString xmlFile_2jet = xmlFile+"/data/CxAODReader_VHbb/TMVAClassification_1lep2jet_0of2_BDT_AdaBoost.weights.xml";
    // m_reader.BookReader("1lep_2jet", xmlFile_2jet);
    // TString xmlFile_3jet = xmlFile+"/data/CxAODReader_VHbb/TMVAClassification_1lep3jet_0of2_BDT_AdaBoost.weights.xml";
    // m_reader.BookReader("1lep_3jet", xmlFile_3jet);
    TString xmlFile_2jet = xmlFile+"/data/CxAODReader_VHbb/TMVAClassification_1lep2jet_0of2_BDT_AdaBoost.weights.xml";
    m_reader.BookReader("1lep_2jet", xmlFile_2jet);
    TString xmlFile_3jet = xmlFile+"/data/CxAODReader_VHbb/TMVAClassification_1lep3jet_0of2_BDT_AdaBoost.weights.xml";
    m_reader.BookReader("1lep_3jet", xmlFile_3jet);
   TString xmlFile_2jet_WZ = xmlFile+"/data/CxAODReader_VHbb/TMVAClassification_1lep2jet_WZ_0of2_BDT_AdaBoost.weights.xml";
    m_reader.BookReader("1lep_2jet_WZ", xmlFile_2jet_WZ);
    TString xmlFile_3jet_WZ = xmlFile+"/data/CxAODReader_VHbb/TMVAClassification_1lep3jet_WZ_0of2_BDT_AdaBoost.weights.xml";
    m_reader.BookReader("1lep_3jet_WZ", xmlFile_3jet_WZ);

    TString xmlFile_2jet_v2 = xmlFile+"/data/CxAODReader_VHbb/TMVAClassification_1lep2jet_0of2_BDT_AdaBoost.weights.xml";
    m_reader.BookReader("1lep_2jet_v2", xmlFile_2jet_v2);
    TString xmlFile_3jet_v2 = xmlFile+"/data/CxAODReader_VHbb/TMVAClassification_1lep3jet_0of2_BDT_AdaBoost.weights.xml";
    m_reader.BookReader("1lep_3jet_v2", xmlFile_3jet_v2);

    TString xmlFile_2jet_v3 = xmlFile+"/data/CxAODReader_VHbb/TMVAClassification_1lep2jet_Sherpa221_TruthAll_mBBCut_0of2_BDT_AdaBoost_v6.weights.xml";
    m_reader.BookReader("1lep_2jet_v3", xmlFile_2jet_v3);
    TString xmlFile_3jet_v3 = xmlFile+"/data/CxAODReader_VHbb/TMVAClassification_1lep3jet_Sherpa221_TruthAll_mBBCut_0of2_BDT_AdaBoost_v6.weights.xml";
    m_reader.BookReader("1lep_3jet_v3", xmlFile_3jet_v3);    


  }

}

float MVATree_VHbb::getBinnedMV1c(float MV1c) {
  const int nbins = 5;
  float xbins[nbins+1] = {0, 0.4050, 0.7028, 0.8353, 0.9237, 1.0};
  float xbins_mv2c20[nbins+1] = {-1, -0.5911, -0.0436, 0.4496, 0.7535, 1.0};

  if (m_analysisType == "0lep"){
     if (MV1c >= 1) MV1c = 1-1e-5;
     if (MV1c <= -1) MV1c = -1+1e-5;// lowset edge for MV2c20

     for(int i=0; i<nbins; i++) {
	if ( MV1c >= xbins_mv2c20[i] && MV1c < xbins_mv2c20[i+1] ) {
	   return (xbins[i] + xbins[i+1])/2;
	}
     }

     return 0;
  }

  if (MV1c >= 1) MV1c = 1-1e-5;
  if (MV1c <= 0) MV1c = 1e-5;

  for(int i=0; i<nbins; i++) {
    if ( MV1c >= xbins[i] && MV1c < xbins[i+1] ) {
      return (xbins[i] + xbins[i+1])/2;
    }
  }

  return 0;
}


void MVATree_VHbb::TransformVars() {
  MV1cB1 = getBinnedMV1c(MV1cB1);
  MV1cB2 = getBinnedMV1c(MV1cB2);
  MV1cJ3 = getBinnedMV1c(MV1cJ3);
  mBB = mBB / 1e3;
  MET = MET / 1e3;
  pTB1 = pTB1  / 1e3;
  pTB2 = pTB2  / 1e3;
  pTBB = pTBB  / 1e3;
  MEff = MEff / 1e3;
  MPT = MPT / 1e3;
  MEff3 = MEff3 / 1e3;
  pTJ3 = pTJ3 / 1e3;
  mBBJ = mBBJ / 1e3;
}

void MVATree_VHbb::Fill() {
  StaticSetBranches(m_analysisType, this);
  MVATree::Fill();
}

void MVATree_VHbb::Reset()
{

  sample = "Unknown";

  EventWeight = 0;
  EventNumber = -1;

  weight = 0;
  isOdd = -1;

  nJ     = -1;
  nSigJet    = -1;
  nForwardJet= -1;
  dRBB   = -1;
  dPhiBB = -1;
  dEtaBB = -1;
  dPhiVBB = -1;
  dPhiMETMPT = -1;
  dPhiMETdijet = -1;
  mindPhi = -1;
  dEtaVBB = -1;
  dPhiLBmin = -1;
  pTV    = -1;
  mBB    = -1;
  MEff   = -1;
  MEff3  = -1;
  sumPt   = -1;
  pTB1   = -1;
  pTB2   = -1;
  pTBB   = -1;
  pTBBoverMET   = -1;
  etaB1   = -1;
  etaB2   = -1;
  mTW    = -1;
  MPT    = -1;
  MET    = -1;
  mLL    = -1;
  MV1cB1 = -1;
  MV1cB2 = -1;
  MV1cJ3 = -1;
  pTJ3   = -1;
  etaJ3   = -1;
  dRB1J3   = -1;
  dRB2J3   = -1;
  mBBJ   = -1;
  pTL    = -1;
  etaL   = -1;
  Tau21  = -1;
  Tau32  = -1;
  mVH    = -1;
  pTVHnorm = -1;

  BDT    = -1;

  StaticReset();
}

void MVATree_VHbb::ReadMVA() {

  if (!m_readMVA) return;
  if (m_analysisType == "0lep"){
     //TransformVars();
     //if (nJ==2){
     //   BDT = m_reader.EvaluateMVA("0lep_2jet");
     //}else{
     //   BDT = m_reader.EvaluateMVA("0lep_3jet");
     //}
  }
  else if(m_analysisType == "1lep"){
    //TransformVars();x
    if (nJ==2){
      BDT = m_reader.EvaluateMVA("1lep_2jet");
      BDT_v2 = m_reader.EvaluateMVA("1lep_2jet_v2");
      BDT_v3 = m_reader.EvaluateMVA("1lep_2jet_v3");
      BDT_WZ = m_reader.EvaluateMVA("1lep_2jet_WZ");
    }else{
      BDT = m_reader.EvaluateMVA("1lep_3jet");
      BDT_v2 = m_reader.EvaluateMVA("1lep_3jet_v2");
      BDT_v3 = m_reader.EvaluateMVA("1lep_3jet_v3");
      BDT_WZ = m_reader.EvaluateMVA("1lep_3jet_WZ");
    }
  }else{
    BDT = m_reader.EvaluateMVA("reader");
  }

}