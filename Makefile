####################################################
###################### MAKE ########################
####################################################

EXECUTAVEL = mptscfl
PATHEXEC = ./bin
PATHSRC= ./
PATHTEMP = ./.temp

all:
	mkdir -p $(PATHEXEC)
	mkdir -p $(PATHTEMP)
	make $(EXECUTAVEL)

#Juntando todos os objetos e gerando o programa
$(EXECUTAVEL):    $(PATHTEMP)/ExactMPTSCFL.o $(PATHTEMP)/graphcost_ts.o $(PATHTEMP)/SampleDecoder.o  $(PATHTEMP)/heuristic.o $(PATHTEMP)/localsearch.o  $(PATHTEMP)/checkfeas.o $(PATHTEMP)/instance.o $(PATHTEMP)/main.o
	$(CPP) $(CCFLAGS)   $(PATHTEMP)/ExactMPTSCFL.o $(PATHTEMP)/graphcost_ts.o $(PATHTEMP)/SampleDecoder.o  $(PATHTEMP)/heuristic.o $(PATHTEMP)/localsearch.o $(PATHTEMP)/checkfeas.o $(PATHTEMP)/instance.o $(PATHTEMP)/main.o $(CCLNFLAGS) -o $(PATHEXEC)/$(EXECUTAVEL)


$(PATHTEMP)/main.o: $(PATHSRC)/main.cpp
	$(CPP) $(CCFLAGS) -c $(PATHSRC)/main.cpp -o $(PATHTEMP)/main.o

$(PATHTEMP)/instance.o: $(PATHSRC)/instance.cpp
	$(CPP) $(CCFLAGS) -c $(PATHSRC)/instance.cpp -o $(PATHTEMP)/instance.o

$(PATHTEMP)/heuristic.o: $(PATHSRC)/heuristic.cpp
	$(CPP) $(CCFLAGS) -c $(PATHSRC)/heuristic.cpp -o $(PATHTEMP)/heuristic.o

$(PATHTEMP)/graphcost_ts.o: $(PATHSRC)/graphcost_ts.cpp
	$(CPP) $(CCFLAGS) -c $(PATHSRC)/graphcost_ts.cpp -o $(PATHTEMP)/graphcost_ts.o

$(PATHTEMP)/localsearch.o: $(PATHSRC)/localsearch.cpp
	$(CPP) $(CCFLAGS) -c $(PATHSRC)/localsearch.cpp -o $(PATHTEMP)/localsearch.o

$(PATHTEMP)/SampleDecoder.o: $(PATHSRC)/BRKGA/SampleDecoder.cpp
	$(CPP) $(CCFLAGS) -c $(PATHSRC)/BRKGA/SampleDecoder.cpp -o $(PATHTEMP)/SampleDecoder.o

$(PATHTEMP)/checkfeas.o: $(PATHSRC)/checkfeas.cpp
	$(CPP) $(CCFLAGS) -c $(PATHSRC)/checkfeas.cpp -o $(PATHTEMP)/checkfeas.o

$(PATHTEMP)/ExactMPTSCFL.o: $(PATHSRC)/ExactMPTSCFL.cpp
	$(CPP) $(CCFLAGS) -c $(PATHSRC)/ExactMPTSCFL.cpp -o $(PATHTEMP)/ExactMPTSCFL.o

####################################################
###################### CLEAN #######################
####################################################
clean:
	#rm -rf $(PATHEXEC)
	rm -rf $(PATHTEMP)
	rm *.tmp
	rm *.lp
	rm solution.txt
	rm *.jpg

####################################################
####################### CPLEX ######################
####################################################

SYSTEM     = x86-64_linux
LIBFORMAT  = static_pic

#------------------------------------------------------------
#
# When you adapt this makefile to compile your CPLEX programs
# please copy this makefile and set CPLEXDIR and CONCERTDIR to
# the directories where CPLEX and CONCERT are installed.
#
#------------------------------------------------------------

CPLEXDIR      = /opt/ibm/ILOG/CPLEX_Studio1210/cplex

##### CPLEX CONFIGURATION's
# System architecture
SYSTEM = x86-64_linux
# Static library format for Cplex
LIBFORMAT = static_pic
# Cplex directory
ILCPLEXDIR = /opt/ibm/ILOG/CPLEX_Studio1210/cplex
# Concert directory
ILCONCERTDIR = /opt/ibm/ILOG/CPLEX_Studio1210/concert

##### CPLEX DIRECTIVE's
# Cplex static libraries directory
ILCPLEXLIBDIR = $(ILCPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
# Concert static libraries directory
ILCONCERTLIBDIR = $(ILCONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
# Cplex header's directory
ILCPLEXINCDIR = $(ILCPLEXDIR)/include
# Concert header's directory
ILCONCERTINCDIR = $(ILCONCERTDIR)/include

# ---------------------------------------------------------------------
# Compiler selection 
# ---------------------------------------------------------------------
 
CPP = g++ 
# ---------------------------------------------------------------------
# Compiler options 
# ---------------------------------------------------------------------


COPT  = -O3 -g -m64 -fPIC -w -fno-strict-aliasing -DIL_STD

# ---------------------------------------------------------------------
# Link options and libraries
# ---------------------------------------------------------------------

CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)

CPLEXLIB  = cplex

CLNDIRS   = -L$(CPLEXLIBDIR)
CCLNFLAGS  =  -L$(CPLEXLIBDIR) -lilocplex -lcplex -L$(ILCONCERTLIBDIR) -lconcert -lm -lpthread -ldl

CPLEXINCDIR   = $(CPLEXDIR)/include/ilcplex/

CCFLAGS  = $(COPT) -I$(ILCPLEXINCDIR) -I$(ILCONCERTINCDIR)  -I$(CPLEXINCDIR) 


