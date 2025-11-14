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
	/usr/cs-local/432/bin/submit432 2 pager.cc sample.cc test3.cc test5.cc test9.cc test10.cc test12.cc test13.cc test14.cc test15.cc test16.cc test17.cc test18.cc test19.cc test20.cc 