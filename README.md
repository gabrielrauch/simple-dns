# Servidor DNS Simples em C++

Este é um simples servidor DNS que foi desenvolvido em C++. O mesmo é destinado a responder às consultas DNS simples, assim como efetua o cache local para melhorar o desempenho. 

## Configuração

Certifique-se de que você tem as seguintes dependências instaladas:

- CMake
- Um compilador C++ compatível, como g++

## Compilação

Execute os seguintes passos para compilar o Servidor:

1. Dê permissão ao arquivo build.sh para executar com o comando 
   ```bash
     chmod +x build.sh
   ```

2. Execute o arquivo build.sh com o comando 
    ```bash
      ./build.sh
    ```

O executável será gerado na pasta build.

## Uso

Você pode executar o servidor através do seguinte comando:

```bash
./simple-dns
```

## Configuração de Registros DNS

Os Registros DNS são configurados em um arquivo texto. Certifique-se que o arquivo de configuração está no formato correto (par domínio e IP). 

Exemplo de arquivo de configuração DNS  no formato texto: 
```txt
exemplo.com    192.168.1.1
outroexemplo.com    192.168.1.2

