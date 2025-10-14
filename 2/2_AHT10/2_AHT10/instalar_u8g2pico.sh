#!/bin/bash

INSTALL_DIR=${1:-lib}

U8G2PICO_REPO_URL="https://github.com/georgines/u8g2pico.git"
U8G2LIB_REPO_URL="https://github.com/olikraus/u8g2.git"

U8G2PICO_NAME="u8g2pico"
U8G2LIB_NAME="u8g2"

detect_project_name() {
    local cmake_file="$1"
    
    if [ -f "$cmake_file" ]; then
        # Extrair o nome do projeto usando sed
        local project_name=$(grep "project(" "$cmake_file" | sed 's/.*project(\s*\([a-zA-Z0-9_-][a-zA-Z0-9_-]*\).*/\1/' | head -1)
        
        if [ -n "$project_name" ]; then
            echo "$project_name"
        fi
    fi
}

echo "=== Instalador da biblioteca u8g2pico ==="
echo "Pasta de destino: $INSTALL_DIR"

if [ ! -d "$INSTALL_DIR" ]; then
    echo "Pasta '$INSTALL_DIR' não existe. Criando..."
    mkdir -p "$INSTALL_DIR"
    if [ $? -eq 0 ]; then
        echo "Pasta '$INSTALL_DIR' criada com sucesso."
    else
        echo "Erro ao criar a pasta '$INSTALL_DIR'."
        exit 1
    fi
else
    echo "Pasta '$INSTALL_DIR' já existe."
fi

U8G2PICO_PATH="$INSTALL_DIR/$U8G2PICO_NAME"

if [ -d "$U8G2PICO_PATH" ]; then
    echo "Biblioteca '$U8G2PICO_NAME' já existe em '$U8G2PICO_PATH'."
    echo "Removendo versão anterior..."
    rm -rf "$U8G2PICO_PATH"
    if [ $? -eq 0 ]; then
        echo "Versão anterior removida com sucesso."
    else
        echo "Erro ao remover a versão anterior."
        exit 1
    fi
fi

echo "Baixando a biblioteca u8g2pico..."
git clone "$U8G2PICO_REPO_URL" "$U8G2PICO_PATH"

if [ $? -ne 0 ]; then
    echo "❌ Erro ao baixar a biblioteca u8g2pico."
    exit 1
fi

echo "✅ u8g2pico baixado com sucesso."

U8G2LIB_INSTALL_DIR="$U8G2PICO_PATH/lib"
U8G2LIB_PATH="$U8G2LIB_INSTALL_DIR/$U8G2LIB_NAME"

echo ""
echo "=== Instalando dependência u8g2lib ==="

if [ ! -d "$U8G2LIB_INSTALL_DIR" ]; then
    echo "Pasta '$U8G2LIB_INSTALL_DIR' não existe. Criando..."
    mkdir -p "$U8G2LIB_INSTALL_DIR"
    if [ $? -eq 0 ]; then
        echo "Pasta '$U8G2LIB_INSTALL_DIR' criada com sucesso."
    else
        echo "Erro ao criar a pasta '$U8G2LIB_INSTALL_DIR'."
        exit 1
    fi
else
    echo "Pasta '$U8G2LIB_INSTALL_DIR' já existe."
fi

if [ -d "$U8G2LIB_PATH" ]; then
    echo "Biblioteca '$U8G2LIB_NAME' já existe em '$U8G2LIB_PATH'."
    echo "Removendo versão anterior..."
    rm -rf "$U8G2LIB_PATH"
    if [ $? -eq 0 ]; then
        echo "Versão anterior removida com sucesso."
    else
        echo "Erro ao remover a versão anterior."
        exit 1
    fi
fi

echo "Baixando a biblioteca u8g2lib..."
git clone "$U8G2LIB_REPO_URL" "$U8G2LIB_PATH"

if [ $? -ne 0 ]; then
    echo "❌ Erro ao baixar a biblioteca u8g2lib."
    exit 1
fi

echo "✅ u8g2lib baixado com sucesso."

echo ""
echo "=== Instalação concluída ==="
echo "✅ Biblioteca u8g2pico instalada com sucesso em: $U8G2PICO_PATH"
echo "✅ Dependência u8g2lib instalada em: $U8G2LIB_PATH"

# Detectar o nome do projeto dinamicamente
PROJECT_NAME=$(detect_project_name "CMakeLists.txt")
if [ -z "$PROJECT_NAME" ]; then
    PROJECT_NAME="seu_projeto"
fi

echo ""
echo -e "\033[1;36m━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\033[0m"
echo -e "                            \033[1;33m⚙️  CONFIGURAÇÃO MANUAL\033[1;36m                           \033[0m"
echo -e "\033[1;36m━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\033[0m"
echo -e "\033[1;33m📝 Adicione manualmente estas linhas no seu CMakeLists.txt:\033[0m"
echo ""
echo -e "\033[1;32m1️⃣  Após a linha project(), adicione:\033[0m"
echo -e "    \033[1;37madd_subdirectory($U8G2PICO_PATH)\033[0m"
echo ""
echo -e "\033[1;32m2️⃣  Na seção target_link_libraries(), adicione u8g2pico:\033[0m"
echo -e "    \033[1;37mtarget_link_libraries($PROJECT_NAME\033[0m"
echo -e "    \033[1;37m    pico_stdlib\033[0m"
echo -e "    \033[1;37m    u8g2pico    \033[1;31m# <-- Adicione esta linha\033[0m"
echo -e "    \033[1;37m)\033[0m"
echo ""
echo -e "\033[1;36m━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\033[0m"
echo -e "\033[1;35m🎯 Pronto! A biblioteca u8g2pico está instalada e pronta para uso.\033[0m"
echo -e "\033[1;36m━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\033[0m"