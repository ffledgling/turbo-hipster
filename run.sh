
g++ -c DBSystem.cpp -o DBSystem.o -g
g++ -c Tester.cpp -o Tester.o -g
g++ -g Tester.o DBSystem.o -o Exec -I .
./Exec > output
diff output outputfinal

