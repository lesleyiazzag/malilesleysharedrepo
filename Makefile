CXX=g++
CFLAGS+=-Wall -Werror -g -std=c++11
CXXFLAGS+=${CFLAGS}

all: pager sample

pager: pager.cc libvm_pager.a
	${CXX} ${CFLAGS} -o pager pager.cc libvm_pager.a -lcrypto

sample: sample.cc libvm_app.a
	${CXX} ${CFLAGS} -o sample sample.cc libvm_app.a

stop:
	killall -9 pager

clean:
	rm *~ pager sample 

submit:
	/usr/cs-local/432/bin/submit432 2 pager.cc sample.cc test2.cc test3.cc test4.cc test5.cc test6.cc test7.cc test8.cc test9.cc test10.cc test11.cc