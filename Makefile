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