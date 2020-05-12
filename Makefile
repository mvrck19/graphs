all: tsp tsp_threaded


tsp:
	g++ tsp.cpp -o tsp

tsp_threaded:
	g++ tsp_threaded.cpp -o tsp_threaded -pthread

