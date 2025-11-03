#!/bin/bash
set -e  # Exit immediately if a command exits with a non-zero status

# Script auxiliar para executar a atividade de threads

# Cores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo "================================================="
echo "  Script de Execução - Atividade de Threads"
echo "================================================="
echo ""

# Função para verificar se um comando existe
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Opção 1: Compilar e executar localmente
executar_local() {
    echo -e "${YELLOW}Opção 1: Executando localmente...${NC}"
    echo ""
    
    if ! command_exists gcc; then
        echo -e "${RED}Erro: GCC não está instalado!${NC}"
        echo "Instale com: sudo apt install gcc (Ubuntu/Debian) ou sudo dnf install gcc (Fedora)"
        return 1
    fi
    
    if ! command_exists make; then
        echo -e "${RED}Erro: Make não está instalado!${NC}"
        echo "Instale com: sudo apt install make (Ubuntu/Debian) ou sudo dnf install make (Fedora)"
        return 1
    fi
    
    echo "Compilando o programa..."
    make clean 2>/dev/null
    if make; then
        echo -e "${GREEN}Compilação bem-sucedida!${NC}"
        echo ""
        echo "Executando o programa..."
        echo "================================================="
        ./atividade_threads
        echo "================================================="
        echo ""
        echo -e "${GREEN}Execução concluída com sucesso!${NC}"
    else
        echo -e "${RED}Erro na compilação!${NC}"
        return 1
    fi
}

# Opção 2: Executar com Docker
executar_docker() {
    echo -e "${YELLOW}Opção 2: Executando com Docker...${NC}"
    echo ""
    
    if ! command_exists docker; then
        echo -e "${RED}Erro: Docker não está instalado!${NC}"
        echo "Instale seguindo: https://docs.docker.com/get-docker/"
        return 1
    fi
    
    echo "Construindo a imagem Docker..."
    if docker build -t atividade-threads .; then
        echo -e "${GREEN}Imagem construída com sucesso!${NC}"
        echo ""
        echo "Executando o container..."
        echo "================================================="
        docker run --rm atividade-threads
        echo "================================================="
        echo ""
        echo -e "${GREEN}Execução concluída com sucesso!${NC}"
    else
        echo -e "${RED}Erro ao construir a imagem Docker!${NC}"
        echo ""
        echo "Dica: Se houver problemas de SSL/certificados, tente executar localmente (opção 1)"
        return 1
    fi
}

# Menu principal
echo "Escolha uma opção:"
echo "1) Executar localmente (requer GCC e Make)"
echo "2) Executar com Docker (requer Docker instalado)"
echo "3) Sair"
echo ""
read -p "Digite sua escolha [1-3]: " choice

case $choice in
    1)
        executar_local
        ;;
    2)
        executar_docker
        ;;
    3)
        echo "Saindo..."
        exit 0
        ;;
    *)
        echo -e "${RED}Opção inválida!${NC}"
        exit 1
        ;;
esac
