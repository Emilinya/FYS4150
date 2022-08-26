p2: p2_cpp p2_py

.PHONY: p2_cpp
p2_cpp: problem2/.p2_cpp.stamp

problem2/.p2_cpp.stamp: ./problem2/p2
	./problem2/p2;
	touch $@
problem2/p2: problem2/p2.cpp
	g++ -o $@ $<

.PHONY: p2_py
p2_py: problem2/.p2_py.stamp

problem2/.p2_py.stamp: p2.png
	touch $@
p2.png: problem2/p2.py problem2/data.dat
	python3 problem2/p2.py