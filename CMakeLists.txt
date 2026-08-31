cmake_minimum_required(VERSION 3.20)
project(processador_de_imagem VERSION 1.0 LANGUAGES CXX)

# Gera o arquivo compile_commands.json para a inteligência da IDEL
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Procura o OpenCV instalado no sistema operacional (Em maiúsculo)
find_package(OpenCV REQUIRED)
include_directories(include)
# target_include_directories(include)

# 2. Define o seu executável baseado no arquivo teste.cpp
add_executable(tratamento_img 
  src/teste.cpp 
  include/processadorImagem.hpp
  src/processadorImagem.cpp
)

# 3. Diz ao compilador onde encontrar as pastas dos headers (Essencial no Linux)
target_include_directories(tratamento_img PRIVATE ${OpenCV_INCLUDE_DIRS})

# 4. CORREÇÃO DO ERRO DO LINKER: Em vez de usar ${OpenCV_LIBS} (que puxa o HDF quebrado),
# vinculamos estritamente os módulos que você usa para abrir e exibir imagens.
target_link_libraries(tratamento_img PRIVATE
    opencv_core
    opencv_imgcodecs
    opencv_highgui
)

