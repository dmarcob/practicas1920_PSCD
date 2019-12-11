#!/bin/bash
# File:   ejecuta_p5_SERVER.bash
# Author: Diego Marco 755232
# Date:   diciembre 2019
# Coms:   Ejecutar: ./ejecuta_p5_SERVER.bash <puerto>

if [ $# -ne 1 ];then
  echo "FALTAN ARGUMENTOS"
  echo "./ejecuta_p5_SERVER.bash <puerto>"
  exit 1
fi
echo "Compilando..."
make -f Makefile_p5_e1
if [ $? -eq 0 ];then
  echo "Compilacion correcta"
else
  exit 1
fi
echo "Servidor levantdo en puerto $1"
./ServidorMulticliente $1
exit 0
