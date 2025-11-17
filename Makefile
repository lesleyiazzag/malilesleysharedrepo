CXX=g++
CFLAGS+=-Wall -Werror -g -std=c++11
CXXFLAGS+=${CFLAGS}
 
TESTS =  test2 test3 test4 test5 test6 test7 test8.2 test9.2 test10.2 test11.2

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
	/usr/cs-local/432/bin/submit432 2 pager.cc  test2.cc test3.cc test4.cc test5.cc test6.cc test7.cc test8.2.cc test9.2.cc test10.2.cc test11.2.cc
