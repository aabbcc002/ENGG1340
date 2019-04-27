census.o:table_management.cpp
  g++ -c table_management.cpp

census:census.o
  g++ census.o -o census

run:
  ./census > output.txt
  cat output.txt
