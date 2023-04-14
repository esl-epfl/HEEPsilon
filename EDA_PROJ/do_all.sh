cd MGC_MSIM; vsim -c < BIN/run_sim.tcl; cd ..
eda snps vcd2saif -input MGC_MSIM/SA/dump.vcd -output SNPS_DC/SA/dump.saif
cd SNPS_DC; dc_shell -f BIN/run_syn.tcl; cd ..
