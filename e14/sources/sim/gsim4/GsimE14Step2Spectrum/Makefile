PACKAGE      = $(shell basename `pwd`)
MAINS        = 
INSTALL_LIBS = yes
INSTALL_BINS = no

include $(E14_CONFIG_DIR)/Makefile.g4

pre::
	@ [ ! -d $(E14_TOP_DIR)/share ] &&  mkdir $(E14_TOP_DIR)/share || true
	@ [ ! -d $(E14_TOP_DIR)/share/$(PACKAGE) ] && mkdir $(E14_TOP_DIR)/share/$(PACKAGE) || true
	@ [ ! -f $(E14_TOP_DIR)/share/$(PACKAGE)/dist2d_C3_kl.dat ] && ln -s $(shell pwd)/$(PACKAGE)/dist2d_C3_kl.dat $(E14_TOP_DIR)/share/$(PACKAGE) || true
	@ [ ! -f $(E14_TOP_DIR)/share/$(PACKAGE)/dist2d_C3_n.dat ] && ln -s $(shell pwd)/$(PACKAGE)/dist2d_C3_n.dat $(E14_TOP_DIR)/share/$(PACKAGE) || true
	@ [ ! -f $(E14_TOP_DIR)/share/$(PACKAGE)/dist2d_C3_g.dat ] && ln -s $(shell pwd)/$(PACKAGE)/dist2d_C3_g.dat $(E14_TOP_DIR)/share/$(PACKAGE)  || true
