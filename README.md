# Servidor DNS Simples em C++

Este é um servidor DNS simples implementado em C++. Ele é projetado para responder a consultas DNS básicas e manter um cache local para melhorar o desempenho.

## Configuração

Certifique-se de ter as seguintes dependências instaladas:

- CMake
- Um compilador C++ compatível (como g++)

## Compilação

Siga estas etapas para compilar o servidor:

1. Dê permissao para o arquivo build.sh executar com o comando
   ```bash
     chmod +x build.sh
   ```
2. Execute o arquivo build.sh com o comando
    ```bash
      ./build.sh
    ```
    
O executável será gerado no diretório build.

## Uso

Você pode executar o servidor com o seguinte comando:

```bash
./simple-dns
```

## Configuração de Registros DNS

Os registros DNS são configurados em um arquivo de texto. Certifique-se de que o arquivo de configuração esteja no formato correto, com pares de domínio e IP.

Exemplo de arquivo de configuração DNS em formato de texto:
```txt
exemplo.com 192.168.1.1
outroexemplo.com 192.168.1.2
