from ROOT import TFile, gDirectory
import numpy as np
import histmaker
from histmaker import histContainer

#mychain = gDirectory.Get('bTag_AntiKt4EMTopoJets')

# Definitions to aid output

# Truth flav lookup dict
truthflav_lookup = {0 : 'light', 4 : 'charm',
                    5 : 'bottom', 15: 'tau'}

# Root file has pt measurements in MeV
pt_bands = {20000 : '20-40GeV', 40000 : '40-80GeV', 80000 : '80-140GeV',
            140000 : '140GeV+'}




def GetJetProperties(tree_name):
    assert type(tree_name) == str

    mychain = gDirectory.Get('bTag_AntiKt4EMTopoJets')
    entries = mychain.GetEntriesFast()

    jets_array = []
    for jentry in xrange(entries):

        nb= mychain.GetEntry(jentry)
        if nb <= 0: continue #Breaks this pass and moves on to next.
        if not np.array(mychain.jet_pt).size: continue  # Checks if entry is empty

        i = 0

        for jet in xrange(np.array(mychain.jet_pt).size):
            jet_properties = dict()

            # Extract properties for jet
            jet_properties['flav'] = truthflav_lookup[mychain.jet_truthflav[i]]
            jet_properties['pt'] = mychain.jet_pt[i]
            for key in sorted(pt_bands.keys(), key=int):
                if jet_properties['pt'] >= key:
                    jet_properties['pt_band'] = pt_bands[key]
                else: break
            jet_properties['sv0_exists'] = True if (mychain.jet_sv0_sig3d[i] > -50) else False
            if jet_properties['sv0_exists']:
                jet_properties['sv0_mass'] = mychain.jet_sv0_m[i]
                jet_properties['sv0_ntracks_v'] = mychain.jet_sv0_ntrkv[i]
                jet_properties['sv0_normdist'] = mychain.jet_sv0_normdist[i]

            jets_array.append(jet_properties)
            i+=1

    return jets_array


def BandPlot(jets_array):
    hists = {}

    # Create hists for all flavour, pt band and sv0 stat combos
    for flavour in truthflav_lookup.values():
        for band in pt_bands.values():
            for stat in histmaker.sv0_stats.keys():
                name = "%s %s %s" % (flavour, band, stat)
                hists[name] = histmaker.HistMaker(flavour, name, stat)

    print hists.keys()

    for jet in jets_array:
        if not jet['sv0_exists']: continue

        for key in jet.keys():
            try_name = "%s %s %s" % (jet['flav'], jet['pt_band'], key)
            try:
                hists[try_name].Fill(jet[key])
            except LookupError:
                pass


    histcontainers = []
    for hist in hists.values():
        histcontainers.append(histContainer(hist))


    return histcontainers