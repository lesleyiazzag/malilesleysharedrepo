CXX=g++
CFLAGS+=-Wall -Werror -g -std=c++11
CXXFLAGS+=${CFLAGS}

TESTS = test3 test5 test9 test10 test12 test13 test14 test15 test16 test17 test18 test19 test20

TEST_SRCS = $(addsuffix .cc, $(TESTS))
TEST_BINS = $(TESTS)

all: pager sample $(TEST_BINS)

pager: pager.cc libvm_pager.a
	${CXX} ${CFLAGS} -o pager pager.cc libvm_pager.a -lcrypto

sample: sample.cc libvm_app.a
	${CXX} ${CFLAGS} -o sample sample.cc libvm_app.a

%: %.cc libvm_app.a
	$(CXX) $(CFLAGS) -o $@ $< libvm_app.a

stop:
	killall -9 pager

clean:
	rm *~ pager sample 

submit:
	/usr/cs-local/432/bin/submit432 2 pager.cc sample.cc test1.cc test2.cc test3.cc test4.cc