# Projeto 1 (Proj1) – Processamento de Imagens

**Universidade Presbiteriana Mackenzie | Faculdade de Computação e Informática**

**Disciplina:** Computação Visual

**Professor:** Prof. André Kishimoto

**Turma:** 07G 2026.1

---

## 👥 Grupo

* **Matheus Santiago de Brito** - 10408953
* **Fernando Pegoraro Bilia** - 10402097
* **Carlos Eduardo Rosendo Basseto** - 10409941
* **Joao Pedro Gianfaldoni** - 10409524

---

## 📝 Descrição Geral do Projeto

Este projeto consiste no desenvolvimento de um software de processamento de imagens em linguagem C. O programa utiliza a biblioteca gráfica SDL (Simple DirectMedia Layer) em sua versão 3, juntamente com `SDL_image` e `SDL_ttf`, para manipulação e exibição de imagens e textos.

O objetivo central é carregar uma imagem, convertê-la para escala de cinza (caso seja colorida) e fornecer uma interface gráfica interativa com duas janelas que permita ao usuário equalizar o histograma da imagem e analisar suas estatísticas de brilho e contraste.

---

## ⚙️ Funcionamento do Projeto

### 1. Entrada do Programa
O programa opera via interface de linha de comando, recebendo obrigatoriamente o caminho do arquivo de imagem como argumento na execução. (Ex: `programa img.png`).

### 2. Janelas de Exibição
O software constrói uma Interface Gráfica de Usuário (GUI) composta por duas janelas sincronizadas:
* **Janela Principal:** Exibe a imagem que está sendo processada. Inicia centralizada no monitor e adapta seu tamanho dinamicamente às dimensões da imagem carregada.
* **Janela Secundária:** Atua como janela filha, exibindo o histograma, um botão de operação e as características estatísticas da imagem.

### 3. Funcionalidades de Processamento e Interação
* **Conversão para Escala de Cinza:** Caso a imagem original seja colorida, é aplicada a conversão automática utilizando a fórmula matemática $Y=0.2125*R+0.7154*G+0.0721*B$.
* **Análise de Histograma:** O programa avalia a média de intensidade para classificar a imagem (Clara, Média ou Escura) e o desvio padrão para classificar o contraste (Alto, Médio ou Baixo).
* **Equalização:** Um botão interativo desenhado com primitivas da SDL permite equalizar o histograma para melhorar o contraste, ou reverter para a versão original.
* **Exportação:** Pressionar a tecla `S` salva a imagem atualmente processada na janela principal como um arquivo `output_image.png`.

---

## 🤝 Contribuições Individuais 

* **Carlos Eduardo Rosendo Basseto:** Responsável pela arquitetura do projeto. Estruturou as declarações de bibliotecas, variáveis globais e `structs`. Gerenciou as funções base de memória e inicialização da SDL, além de desenvolver a lógica de posicionamento dinâmico da janela filha ao lado da janela principal.
* **Joao Pedro Gianfaldoni:** Implementou o motor de atualização com a lógica de eventos, loop principal e inputs do mouse. Desenvolveu a detecção booleana de escala de cinza e construiu toda a lógica matemática por trás da equalização do histograma usando Tabelas de Transformação (LUT) e Função de Distribuição Acumulada (CDF).
* **Fernando Pegoraro Bilia:** Responsável pelo suporte de arquivos e renderização. Codificou a lógica de carregamento de imagens (`IMG_Load`), conversões de `Surface` para Texturas no formato `RGBA32`, além de programar a lógica de análise de dados (média e variância) da imagem para exibir as estatísticas na tela.
* **Matheus Santiago de Brito:** Encarregado da interação visual e organização geral. Desenvolveu as funções que criam e atualizam a UI interativa do botão e a matemática e renderização gráfica em barras do histograma. Estruturou a chamada de todas as funções através da função `main()`, além de elaborar toda a documentação deste repositório.

---

## 🚀 Compilação e Execução 

*Requisitos: O projeto deve ser compilado com o compilador `gcc`*

### Windows (MinGW)
Certifique-se de que os arquivos `.dll` (versão Runtime) da SDL3, SDL3_image e SDL3_ttf estejam na mesma pasta que o `main.c`.
1. **Compilar:** Abra o terminal na pasta do projeto e execute:
   ```bash
   gcc main.c -I./include -L./lib -lSDL3 -lSDL3_image -lSDL3_ttf -o programa.exe
