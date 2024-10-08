MEAS_DIR =  /home/user5/devil_pannu/monteCarlo/Project/hpctoolkit-barrier_option_pricing-measurements
GPUBIN_CFG = no
CPU_ANALYZE = 1
GPU_ANALYZE = 1
PAR_SIZE = 100000000
JOBS = 32
PTHREADS = 16
PROFTT = /home/apps/spack/opt/spack/linux-centos7-cascadelake/gcc-11.2.0/hpctoolkit-2021.10.15-xwzccbhlwuknxd3d2oiiok6dmnss3eiv/libexec/hpctoolkit/hpcproftt
STRUCT= /home/apps/spack/opt/spack/linux-centos7-cascadelake/gcc-11.2.0/hpctoolkit-2021.10.15-xwzccbhlwuknxd3d2oiiok6dmnss3eiv/bin/hpcstruct

#*******************************************************************************
# a helper template makefile used by hpcstruct at runtime
#
# if hpcstruct is passed the name of a measurements directory that contains
# a gpubins subdirectory, this makefile will be used to orchestrate parallel
# analysis of all gpubins within the subdirectory.
#
# to simplify things at execution time, this makefile will be incorporated
# into hpcstruct as a string and written to a temporary file if it is needed.
# this avoids the need for hpcstruct to know how to find a copy of this
# makefile at runtime in an hpctoolkit installation.
#*******************************************************************************

#-------------------------------------------------------------------------------
# set up subdirectory paths
#-------------------------------------------------------------------------------
GPUBIN_DIR  = $(MEAS_DIR)/gpubins
STRUCTS_DIR = $(MEAS_DIR)/structs

#*******************************************************************************
#*******************************************************************************

#*******************************************************************************
# enable analysis of GPU binaries
#*******************************************************************************
ifeq ($(GPU_ANALYZE),1)

#-------------------------------------------------------------------------------
# $(GB): gpubin files
#-------------------------------------------------------------------------------
GB := $(wildcard $(GPUBIN_DIR)/*.gpubin)

#-------------------------------------------------------------------------------
# $(GS): hpcstruct files for gpubins
#-------------------------------------------------------------------------------
GS := $(patsubst $(GPUBIN_DIR)/%,$(STRUCTS_DIR)/%.hpcstruct,$(GB))

#-------------------------------------------------------------------------------
# $(GW): warning files that may be generated during structure analysis of gpubins
#-------------------------------------------------------------------------------
GW := $(patsubst %.hpcstruct,%.warnings,$(GS))

endif

#*******************************************************************************
#*******************************************************************************

#*******************************************************************************
# enable analysis of CPU binaries
#*******************************************************************************
ifeq ($(CPU_ANALYZE),1)
CPUBIN_DIR  = $(MEAS_DIR)/cpubins
LM_DIR      = $(MEAS_DIR)/lm

#-------------------------------------------------------------------------------
# $(H): hpcrun file for thread 0 in each process
#-------------------------------------------------------------------------------
H := $(wildcard $(MEAS_DIR)/*.hpcrun)

#-------------------------------------------------------------------------------
# $(L): load modules for hpcrun files
#-------------------------------------------------------------------------------
L := $(patsubst $(MEAS_DIR)/%.hpcrun,$(LM_DIR)/%.lm,$(H))

#-------------------------------------------------------------------------------
# create $(LM_DIR)/all.lm: a list of all load modules involved in the execution
#-------------------------------------------------------------------------------
$(LM_DIR)/all.lm: $(L)
	cat $(L) | sort -u | grep -v gpubin | grep -v libhpcrun | grep -v libmonitor | grep -v libxed | grep -v libpfm | grep -v libcuda | grep -v libcupti > $(LM_DIR)/all.lm

#-------------------------------------------------------------------------------
# create cpubins directory containing links to all CPU binaries
#-------------------------------------------------------------------------------
$(CPUBIN_DIR): $(LM_DIR)/all.lm
	-@mkdir $(CPUBIN_DIR) >&- 2>&-
	-@cd $(CPUBIN_DIR) >&- 2>&-; for i in `cat $(LM_DIR)/all.lm`; do ln -s $$i; done >&- 2>&-

#-------------------------------------------------------------------------------
# $(CB): cpubin files
#-------------------------------------------------------------------------------
CB := $(wildcard $(CPUBIN_DIR)/*)

#-------------------------------------------------------------------------------
# $(CS): hpcstruct files for cpubins
#-------------------------------------------------------------------------------
CS := $(patsubst $(CPUBIN_DIR)/%,$(STRUCTS_DIR)/%.hpcstruct,$(CB))

#-------------------------------------------------------------------------------
# $(CW): warning files that may be generated during structure analysis of cpubins
#-------------------------------------------------------------------------------
CW := $(patsubst %.hpcstruct,%.warnings,$(CS))

endif
#-------------------------------------------------------------------------------
# execute the sequence of commands for each target in a single shell
#-------------------------------------------------------------------------------
.ONESHELL:
.SILENT:

.DEFAULT_GOAL := all

#-------------------------------------------------------------------------------
# rule for analyzing a cpu binary
# 1. analyze a cpu binary file in $(CPUBIN)
# 2. produce a hpcstruct file in $(STRUCTS_DIR)
# 3. leave a warnings file in $(STRUCTS_DIR) if trouble arises
# 4. announce when analysis of a cpu binary begins and ends
#-------------------------------------------------------------------------------
$(STRUCTS_DIR)/%.hpcstruct: $(CPUBIN_DIR)/%
	@cpubin_name=`basename -s x $<`
	struct_name=$@
	warn_name=$(STRUCTS_DIR)/$$cpubin_name.warnings
	nbytes=`du -b -L $< | tail -1 | awk '{ print $$1 }'`
	if test $$nbytes -gt $(CPAR_SIZE) ; then
		if test $(THREADS) -gt 1 ; then
			echo msg: begin parallel analysis of $$cpubin_name \(size = $$nbytes, using $(THREADS) threads\)
		else
			echo msg: begin concurrent analysis of $$cpubin_name \(size = $$nbytes, using 1 of $(JOBS) threads\)
		fi
		$(STRUCT) -j $(THREADS) -o $$struct_name $< > $$warn_name 2>&1
		if test -s $$warn_name ; then
			echo WARNING: incomplete analysis of $$cpubin_name';' see $$warn_name for details
			if test ! -s $$struct_name ; then
				rm -f $$struct_name
			fi
		else
			rm -f $$warn_name
		fi
		if test $(THREADS) -gt 1 ; then
			echo msg: end parallel analysis of $$cpubin_name
		else
			echo msg: end concurrent analysis of $$cpubin_name
		fi
	fi

#-------------------------------------------------------------------------------
# rule  for analyzing a gpubin
# 1. analyze a gpubin file in $(GPUBIN_DIR)
# 2. produce a hpcstruct file in $(STRUCTS_DIR)
# 3. leave a warnings file in $(STRUCTS_DIR) if trouble arises
# 4. announce when analysis of a gpubin begins and ends
#-------------------------------------------------------------------------------
$(STRUCTS_DIR)/%.hpcstruct: $(GPUBIN_DIR)/%
	@gpubin_name=`basename -s x $<`
	struct_name=$@
	warn_name=$(STRUCTS_DIR)/$$gpubin_name.warnings
	nbytes=`du -b -L $< | tail -1 | awk '{ print $$1 }'`
	if test $$nbytes -gt $(GPAR_SIZE) ; then
		if test $(THREADS) -gt 1 ; then
			echo msg: begin parallel analysis of $$gpubin_name \(size = $$nbytes, using $(THREADS) threads\)
		else
			echo msg: begin concurrent analysis of $$gpubin_name \(size = $$nbytes, using 1 of $(JOBS) threads\)
		fi
		$(STRUCT) -j $(THREADS) --gpucfg $(GPUBIN_CFG) -o $$struct_name $< > $$warn_name 2>&1
		if test -s $$warn_name ; then
			echo WARNING: incomplete analysis of $$gpubin_name';' see $$warn_name for details
			if test ! -s $$struct_name ; then
				rm -f $$struct_name
			fi
		else
			rm -f $$warn_name
		fi
		if test $(THREADS) -gt 1 ; then
			echo msg: end parallel analysis of $$gpubin_name
		else
			echo msg: end concurrent analysis of $$gpubin_name
		fi
	fi

#-------------------------------------------------------------------------------
# analyze files to create structure files
#-------------------------------------------------------------------------------
DOMAKE=1

ifeq ($(CPU_ANALYZE),1)
ifeq ($(L),)
all:
	echo ERROR: directory $(MEAS_DIR) does not contain any hpcrun measurement files
DOMAKE=0
endif
endif

ifeq ($(DOMAKE),1)
all: $(CPUBIN_DIR)
	$(MAKE) -j 1 JOBS=1 THREADS=$(PTHREADS)  GPAR_SIZE=$(PAR_SIZE) CPAR_SIZE=$(PAR_SIZE) analyze
	$(MAKE) -j $(JOBS) JOBS=$(JOBS) THREADS=1  GPAR_SIZE=0 CPAR_SIZE=0 analyze
endif

analyze: $(GS) $(CS) 

#-------------------------------------------------------------------------------
# rule for listing load modules in hpcrun files
#-------------------------------------------------------------------------------
$(LM_DIR)/%.lm: $(MEAS_DIR)/%.hpcrun
	-@mkdir $(LM_DIR) >&- 2>&-
	$(PROFTT) -l $< > $@

#-------------------------------------------------------------------------------
# remove all generated files
#-------------------------------------------------------------------------------
clean:
	@echo removing all hpcstruct files in $(STRUCTS_DIR)
	@rm -f $(GS)
	@rm -f $(CS)
	@echo removing all links to CPU binaries in $(CPUBIN_DIR)
	@rm -rf $(CPUBIN_DIR)
	@rm -rf $(LM_DIR)
	@echo removing all warnings in $(STRUCTS_DIR)
	@rm -f $(CW)
	@rm -f $(GW)

