#!/bin/bash

mkdir -p build
cd build
cmake ..
make

if [ $? -eq 0 ]; then
    echo "Compilação bem-sucedida. Para executar o servidor DNS, use o comando:"
    echo "./dns_server"
else
    echo "Erro durante a compilação. Certifique-se de que está em um abiente UNIX."
fi
