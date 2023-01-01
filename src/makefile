GPP=g++
PROJ_FILES_PATH=.
TESTS_DIR=./tests
O_FILES_DIR=$(TESTS_DIR)/OFiles
EXEC=worldCupTester
#TESTS_INCLUDED_FILE=worldcup23a1.h
OBJS=$(O_FILES_DIR)/UnitTests.o $(O_FILES_DIR)/Team.o $(O_FILES_DIR)/Player.o $(O_FILES_DIR)/UnionFind.o  #$(O_FILES_DIR)/worldcup23a1.o # UPDATE HERE ALL THE O FILES YOU CREATED BELOW
DEBUG_FLAG= -g # can add -g
COMP_FLAG=--std=c++11 -Wall -Werror -pedantic-errors $(DEBUG_FLAG)

$(EXEC) : $(OBJS)
	$(GPP) $(COMP_FLAG) $(OBJS) -o $@

$(O_FILES_DIR)/UnitTests.o : $(TESTS_DIR)/worldCup_tests.cpp
	@mkdir -p $(O_FILES_DIR)
	$(GPP) -c $(COMP_FLAG) $(TESTS_DIR)/worldCup_tests.cpp -o $@

 # UPDATE FROM HERE

$(O_FILES_DIR)/AVLRankTree.o : AVLRankTree.cpp AVLRankTree.h 
	@mkdir -p $(O_FILES_DIR)
	$(GPP) -c $(COMP_FLAG) AVLRankTree.cpp -o $@

$(O_FILES_DIR)/Team.o : Team.cpp Team.h Player.h wet2util.h
	@mkdir -p $(O_FILES_DIR)
	$(GPP) -c $(COMP_FLAG) Team.cpp -o $@

$(O_FILES_DIR)/Player.o : Player.cpp Player.h wet2util.h
	@mkdir -p $(O_FILES_DIR)
	$(GPP) -c $(COMP_FLAG) Player.cpp -o $@

$(O_FILES_DIR)/UnionFind.o : UnionFind.cpp Exception.h wet2util.h Team.h Player.h
	@mkdir -p $(O_FILES_DIR)
	$(GPP) -c $(COMP_FLAG) UnionFind.cpp -o $@
# $(O_FILES_DIR)/worldcup23a1.o : worldcup23a1.cpp worldcup23a1.h wet1util.h AVLRankTree.h Team.h Player.h
# 	@mkdir -p $(O_FILES_DIR)
# 	$(GPP) -c $(COMP_FLAG) worldcup23a1.cpp -o $@
	
 # UNTIL HERE
	

.PHONY: clean
clean:
	rm -f $(OBJS) $(EXEC)
