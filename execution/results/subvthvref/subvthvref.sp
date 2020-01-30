topckt subvthvref vref vcc vss

M99 (vbp net5 vb1 vss) nfet  l=10u w=10u m=1 nf=1 
M200 (net4 net4 vss vss) nfet  l=10u w=10u m=1 nf=1 
M100 (net5 net4 vss vss) nfet  l=10u w=10u m=1 nf=1 
M8 (vb1 vb1 vss vss) nfet  l=10u w=0.8u m=1 nf=1  
M400 (vcc net5 vcc vcc) pfet  l=10u w=10u m=1 nf=1 
M300 (net4 vbp vcc vcc) pfet  l=10u w=10u m=1 nf=1 
M9 (vb1 vbp vcc vcc) pfet  l=10u w=60u m=1 nf=6 
M7 (dm6m7 vbp vcc vcc) pfet  l=10u w=130u m=1 nf=13 
M1 (net3 vbp vcc vcc) pfet  l=10u w=20u m=1 nf=2 
M2 (vbp vbp vcc vcc) pfet  l=10u w=60u m=1 nf=6 
M10 (net2 vbp vcc vcc) pfet  l=10u w=10u m=8 nf=1 
M13 (vbody vbp vcc vcc) pfet  l=10u w=20u m=1 nf=2 
M14 (vbody vbody vss vss) nfet  l=10u w=6.7u m=1 nf=1 
M3 (net3 net3 vss vss) nfet  l=10u w=15u m=1 nf=1 
M4 (vbp net3 net1 vss) nfet  l=10u w=105u m=1 nf=7 
M12 (vref net2 vss vss) nfet  l=10u w=1u m=1 nf=1 
M11 (net2 net2 vref vbody) nfet  l=10u w=11.9u m=1 nf=1 
M5 (net1 vb1 vss dm6m7) nfet  l=10u w=0.22u m=1 nf=1 
M6 (dm6m7 vb1 vss dm6m7) nfet  l=10u w=0.22u m=1 nf=1 

ends subvthvref
