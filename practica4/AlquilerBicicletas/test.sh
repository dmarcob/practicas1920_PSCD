#!/bin/bash
echo "TEST"
echo "Compilando..."
make -f Makefile_p4
if [ $? != 0 ]; then
	exit 1
fi
echo "Iniciando test..."
for i in {1..35}; 
do 
	./main_p4
	if [ $? != 0 ]; then
   		echo "ERROR"
   		exit 1
	else
		echo "*******************"
		echo "* $i OK           *"
		echo "*******************"
	fi 
done
echo "Terminado"
exit 0
