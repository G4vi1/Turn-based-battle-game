# Relatório de migração de SDL3 para raylib

Projeto: Turn-Based Battle Game  
Data da análise: 28 de outubro de 2026  
Estado: planejamento técnico; nenhuma alteração no código do jogo foi realizada para produzir este relatório.

## 1. Decisão recomendada e escopo

Migrar a camada de apresentação e entrada para raylib, mantendo as regras de combate em C++. A primeira entrega deve reproduzir a batalha desktop existente. A segunda entrega pode disponibilizar o mesmo jogo no navegador com Emscripten, WebAssembly e WebGL 2.

Adotar raylib 6.2 como referência inicial: essa é a versão retornada pela página oficial de releases na consulta desta análise. Fixar a versão da dependência; não compilar a partir de `master`. A integração e os exemplos abaixo são propostas a validar na implementação, não resultados de uma compilação executada. [Release 6.2](https://github.com/raysan5/raylib/releases/tag/6.2)

O trabalho obrigatório envolve janela, desenho, fontes, entrada, relógio, tipos SDL, compilação, distribuição de recursos e documentação. Áudio, sprites, câmera, partículas e shaders são evoluções posteriores. WebGPU constitui outro projeto de renderização: não é necessário para migrar para raylib nem para gerar WebAssembly.

### Prioridades usadas no relatório

| Classificação | Significado |
|---|---|
| Obrigatório desktop | Necessário para substituir SDL3 e entregar a batalha existente |
| Obrigatório web | Necessário quando a entrega para navegador for executada |
| Recomendado | Reduz risco ou acoplamento durante a migração |
| Futuro | Nova funcionalidade ou melhoria que pode ser implementada separadamente |

## 2. Diagnóstico da stack atual

A inspeção encontrou um executável C++ montado por CMake, dependente de SDL3 e SDL3_ttf. A aplicação gráfica começa em `src/main.cpp`, que instancia `Renderer`, os dois personagens, `Battle` e `BattleScreen`. O loop de janela é implementado nesse arquivo.

`Game::run()` existe, mas inicia `Battle::start()`, o caminho de console. Não é o controlador utilizado pelo `main.cpp` gráfico. `Battle` combina métodos usados pela interface com métodos legados de console; `AttackAction` e `ItemAction` contêm leitura bloqueante por `cin`. Portanto, o domínio está independente do SDL, mas ainda não está totalmente independente da interface de console.

Características verificadas no código:

- Janela fixa de 1280 × 720 e layout com posições absolutas.
- Desenho de retângulos, bordas e texto encapsulado em `Renderer`.
- Fonte carregada do caminho `/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf`.
- Criação e destruição de superfície e textura a cada chamada de `drawText()`.
- Eventos de mouse SDL tratados por `BattleScreen` e `Button`.
- Leitura de teclado SDL em `BattleScreen::update()`.
- Efeitos com prazo de 700 ms e relógio `SDL_GetTicks()`.
- Simulação de alma, projéteis e padrões recebendo `deltaTime` em segundos.
- Arena e colisões implementadas com números e regras próprias, sem SDL.
- Ausência de assets de jogo e de suíte de testes entre os arquivos inventariados.
- CMake declara versão mínima 3.10; README informa C++17, mas o target não exige explicitamente esse padrão.
- Docker instala SDL3 e SDL3_ttf; `.dockerignore` exclui somente `build/`.

## 3. Stack de destino

| Camada | Atual | Destino | Intervenção |
|---|---|---|---|
| Linguagem e regras | C++ | C++17 explícito | Preservar regras; configurar padrão |
| Janela e contexto | SDL3 | raylib, backend desktop sem SDL | Substituir inicialização e encerramento |
| Renderização 2D | SDL_Renderer | raylib por meio de `Renderer` | Reimplementar o adaptador |
| Fontes | SDL3_ttf | Fontes da raylib | Substituir carga, desenho e liberação |
| Entrada | SDL_Event e teclado SDL | Snapshot de entrada preenchido pela raylib | Adaptar contratos da interface |
| Temporização | Ticks SDL | Tempo de quadro e relógio da aplicação | Padronizar em segundos |
| Combate e inventário | Classes próprias | Mesmas classes | Evitar dependência gráfica |
| Bullet hell | Simulação própria | Mesma simulação | Revisar apenas integração temporal |
| Build desktop | CMake + SDL instalado | CMake + raylib fixada | Alterar descoberta e linkagem |
| Recursos | Fonte do sistema | Pasta `assets/` distribuída | Adicionar arquivos e política de caminhos |
| Browser | Não configurado | Emscripten + Wasm + WebGL 2 | Segundo perfil de build |
| Áudio | Não implementado no fluxo observado | Módulo de áudio da raylib | Futuro |

Fluxo proposto:

```text
main / controlador da aplicação
    captura entrada e calcula tempo
                  |
                  v
             BattleScreen ----------> Renderer ----------> raylib
                  |
                  v
                Battle
           /      |       \
    Character  Inventory  Arena / Soul / Bullet / BulletPattern
```

Somente a camada de plataforma/apresentação precisa conhecer raylib. Não é necessário criar uma hierarquia de renderizadores virtuais: a classe `Renderer` existente já oferece um ponto de adaptação suficiente.

## 4. Inventário de arquivos e intervenções

Os caminhos desta seção são relativos à raiz do repositório.

| Arquivo(s) | Mudança prevista | Prioridade |
|---|---|---|
| `CMakeLists.txt` | Remover SDL, integrar raylib, declarar C++17, configurar assets e perfis | Obrigatório desktop |
| `src/main.cpp` | Substituir eventos/relógio SDL e coordenar o ciclo de quadros | Obrigatório desktop |
| `include/UI/Renderer.hpp` | Retirar tipos SDL/TTF; definir estado e propriedade dos recursos | Obrigatório desktop |
| `src/UI/Renderer.cpp` | Reimplementar janela, formas, texto e liberação | Obrigatório desktop |
| `include/UI/BattleScreen.hpp` | Trocar `handleEvent`, `Uint64` e include SDL | Obrigatório desktop |
| `src/UI/BattleScreen.cpp` | Adaptar mouse, teclado, efeitos e chamadas de desenho | Obrigatório desktop |
| `include/UI/Button.hpp`, `src/UI/Button.cpp` | Remover `SDL_Event` do contrato de clique | Obrigatório desktop |
| `include/UI/HealthBar.hpp`, `src/UI/HealthBar.cpp` | Substituir `SDL_Color` | Obrigatório desktop |
| `include/Battle.hpp` | Atualizar comentário que atribui o controle à camada SDL | Obrigatório desktop |
| `src/Battle.cpp` | Preservar regras; impedir entrada no fluxo bloqueante na aplicação gráfica | Recomendado; obrigatório no fluxo web |
| `include/Game.hpp`, `src/Game.cpp` | Manter legado isolado ou transformá-lo em controlador gráfico | Recomendado |
| `include/Actions/*`, `src/Actions/*` | Preservar nesta etapa; separar leitura de console se reutilizados pela GUI | Recomendado |
| `include/Character.hpp`, `src/Character.cpp` | Preservar regras e atributos | Sem troca de biblioteca |
| `include/Skill.hpp`, `src/Skill.cpp` | Preservar habilidades | Sem troca de biblioteca |
| `include/Item.hpp`, `src/Item.cpp` | Preservar efeitos dos itens | Sem troca de biblioteca |
| `include/Inventory.hpp`, `src/Inventory.cpp` | Preservar estoque e consumo | Sem troca de biblioteca |
| `include/GameData/Characters.hpp`, `src/GameData/Characters.cpp` | Preservar dados e configuração inicial | Sem troca de biblioteca |
| `include/BulletHell/*`, `src/BulletHell/*` | Preservar simulação, colisões e padrões | Sem troca de biblioteca |
| `Dockerfile` | Substituir dependências e isolar diretório de build | Obrigatório desktop |
| `.gitignore`, `.dockerignore` | Cobrir novos builds e artefatos gerados | Obrigatório para os novos perfis |
| `README.md` | Documentar stack, dependências, execução e limitações reais | Obrigatório desktop |
| `assets/fonts/` e avisos de licença | Distribuir a fonte escolhida | Obrigatório para texto equivalente |
| `include/UI/InputState.hpp` | Novo contrato de entrada sem SDL | Recomendado |
| `include/UI/Color.hpp` ou equivalente | Novo tipo RGBA próprio | Recomendado |
| `web/shell.html` | Página de carregamento, canvas e mensagens | Opcional para primeiro build web |
| `tests/` | Testes focados nas regras e nos riscos da migração | Recomendado |

## 5. Migração do Renderer

### 5.1 Janela e ciclo de vida

Substituir os ponteiros `SDL_Window*`, `SDL_Renderer*` e `TTF_Font*`. A raylib mantém o contexto de janela internamente; o adaptador passa a controlar flags de inicialização, cor corrente e uma fonte carregada.

Contrato recomendado:

1. Configurar flags de janela antes da abertura, se necessárias.
2. Abrir a janela de 1280 × 720.
3. Verificar a inicialização e retornar erro antes de construir a tela, se falhar.
4. Carregar a fonte e registrar explicitamente quem possui esse recurso.
5. Ao sair, liberar fonte própria e demais recursos GPU antes de fechar o contexto.
6. Tornar `shutdown()` seguro contra execução duplicada; proibir cópia do adaptador que possui recursos.

Se for usada a fonte padrão como fallback, distingui-la da fonte própria para não liberar o mesmo recurso duas vezes. Falhas devem aparecer no log com o caminho solicitado. O fallback é útil no diagnóstico, mas não prova equivalência visual com a fonte original.

### 5.2 Correspondência de operações

| Operação atual | Substituição proposta |
|---|---|
| `SDL_Init`, criação da janela e renderer | `InitWindow` e verificação de janela pronta |
| `SDL_RenderClear` | `BeginDrawing` seguido de `ClearBackground` no início do quadro |
| `SDL_RenderPresent` | `EndDrawing` no final do quadro |
| `SDL_SetRenderDrawColor` | Guardar uma cor no adaptador e passá-la ao desenho |
| `SDL_RenderFillRect` | `DrawRectangleRec` |
| `SDL_RenderRect` | `DrawRectangleLinesEx`, espessura definida |
| Texto com SDL_ttf | `LoadFontEx`, `DrawTextEx` e medição quando necessário |
| Destruição do contexto | Liberação dos recursos e `CloseWindow` |

Referência das operações: [API pública raylib 6.2](https://github.com/raysan5/raylib/blob/6.2/src/raylib.h).

Não colocar `BeginDrawing()` em cada chamada de desenho. Deve existir exatamente um par início/fim por quadro. Atualmente `BattleScreen::draw()` chama `clear()` e `present()`: a adaptação mínima pode manter essa responsabilidade. Se o loop principal assumir o par, retirar as duas chamadas da tela no mesmo patch. Misturar as alternativas gera quadros duplicados ou chamadas fora do contexto.

É preferível renomear `clear/present` para `beginFrame/endFrame`, tornando explícita a nova responsabilidade. A cor de limpeza deve ser definida independentemente da última cor usada no quadro anterior. Preservar posições em ponto flutuante e comparar o alinhamento das bordas de 1 pixel.

### 5.3 Texto, fonte e Unicode

O novo renderer deve carregar a fonte uma vez e reutilizar seu atlas. Deixar de criar superfícies e texturas temporárias por texto resolve uma fonte concreta de trabalho repetido no código atual; o ganho em FPS precisa ser medido.

Passos de migração:

1. Adicionar uma fonte redistribuível a `assets/fonts/` com a licença correspondente.
2. Definir como o executável encontra `assets/` ao ser iniciado de outro diretório.
3. Carregar glifos suficientes para os textos reais. Para português, incluir caracteres acentuados e cedilha; carregar apenas o conjunto padrão não deve ser considerado garantia de cobertura.
4. Preservar inicialmente a cor branca e os tamanhos usados pela interface.
5. Medir labels para validar botões de 300 pixels, logs e nomes longos.
6. Comparar legibilidade, espaçamento e altura: um tamanho numérico igual em SDL_ttf e raylib não garante a mesma aparência.
7. Definir tamanho base e filtragem da fonte; evitar redimensionar um atlas pequeno de forma que os textos fiquem borrados.

Critério de aceite: todos os textos atuais legíveis, sem cortes novos, e o jogo inicia em uma máquina que não possui a fonte no caminho Linux antigo.

## 6. Entrada e estados da interface

### 6.1 Novo contrato

Substituir `handleEvent(const SDL_Event&)` por uma operação como `handleInput(const InputState&)`. O snapshot é uma proposta de tipo novo, não uma API que já existe:

```cpp
struct InputState {
    float mouseX = 0.0f;
    float mouseY = 0.0f;
    bool primaryPressed = false;
    bool primaryDown = false;
    bool primaryReleased = false;
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
};
```

Capturar o snapshot uma vez por quadro na camada raylib. Cliques usam a transição de pressionamento; movimento usa tecla mantida. Reutilizar esse snapshot nos subpassos de simulação, mas nunca reaplicar o clique em cada subpasso.

`Button::isClicked()` passa a receber posição e flag de clique, ou o snapshot. A checagem geométrica própria pode ser mantida; não há necessidade de adicionar raylib à classe para fazer a colisão do mouse.

### 6.2 Semântica que exige atenção

- O código atual aceita qualquer botão do mouse em `SDL_EVENT_MOUSE_BUTTON_DOWN`. Recomenda-se restringir ao principal, registrando essa mudança de comportamento; preservar todos exigiria mapeamento explícito.
- Manter uma tecla pressionada deve mover a alma continuamente, mas manter o mouse pressionado não pode consumir vários itens ou turnos.
- Uma ação aceita consome o clique. Não permitir que o mesmo quadro abra SKILLS e também escolha uma habilidade situada sob o cursor.
- Recalcular hover após mudanças de tela, mesmo sem movimento do mouse.
- Limpar estado pressionado ao soltar, trocar de tela, perder foco e reiniciar.
- Desabilitar seleção quando não houver mana, estoque ou efeito útil de item.
- Revisar o comportamento de Escape: raylib possui uma tecla de saída padrão. Se a intenção for preservar o fechamento apenas pelo X/EXIT, desabilitar esse atalho ou defini-lo conscientemente.

### 6.3 Reconstrução dos botões

Hoje `draw()` chama `updateSkillButtons()` ou `updateItemButtons()`. Essas funções limpam os vetores e recriam os botões, descartando hover/pressed. Antes de implementar a nova captura:

1. Atualizar a estrutura dos botões na entrada do submenu ou quando habilidades/estoque mudarem.
2. Atualizar labels e habilitação quando HP, MP e quantidades mudarem.
3. Aplicar hover e pressed após a atualização estrutural.
4. Desenhar sem reconstruir os objetos.

Não alterar o vetor enquanto uma referência a um botão está sendo utilizada para executar uma ação. Consumir a ação por índice, terminar o processamento do clique e reconstruir depois, se necessário.

### 6.4 Estados a preservar

| Estado | Entrada aceita | Saída esperada |
|---|---|---|
| ACTIONS | Skill, Item, Defend | Submenu ou turno inimigo |
| SKILLS | Habilidade válida, Back | Turno inimigo/fim ou ACTIONS |
| ITEMS | Item utilizável, Back | Turno inimigo ou ACTIONS |
| ENEMY_TURN | Continue | Bullet hell, resultado inimigo ou fim |
| BULLET_HELL | WASD/setas | Resolução do dano, ACTIONS ou FINISHED |
| FINISHED | Restart, Exit | Estado inicial ou encerramento |

O bloqueio de entradas fora do estado correto continua sendo responsabilidade da tela, com validação das regras em `Battle`.

## 7. Tempo, loop e simulação

### 7.1 Unidades

O `deltaTime` atual já é convertido de milissegundos para segundos antes de chegar à simulação. Usar `GetFrameTime()` não exige multiplicar ou dividir novamente por 1000. Velocidades permanecem em unidades por segundo.

Para efeitos, uma substituição mínima seria `effectEndsAt = GetTime() + 0.7`, com prazo em `double`. A recomendação para desktop e web é um temporizador restante atualizado pelo tempo ativo da aplicação, permitindo pausar efeitos junto com a batalha. Escolher uma política única e documentar o comportamento ao perder foco.

Mover a expiração de `drawBattleEffect()` para `update()`. Como o `update()` atual retorna cedo fora de BULLET_HELL, atualizar o relógio dos efeitos antes desse retorno. Caso contrário, efeitos de cura e defesa podem nunca terminar.

### 7.2 Ordem do quadro

```text
capturar entrada
verificar saída/foco
atualizar controles da tela
processar no máximo uma ação de clique
avançar movimento e simulação
resolver eventual término do bullet hell
atualizar efeitos
iniciar desenho -> desenhar interface -> terminar desenho
```

O fechamento deve impedir atualizações adicionais depois da decisão de sair. Não desenhar após liberar a janela.

### 7.3 Passos de simulação

Na etapa de equivalência desktop, preservar inicialmente o modelo de tempo variável para reduzir mudanças simultâneas. Antes da versão web, recomenda-se um acumulador com passos pequenos fixos e limite de recuperação por quadro.

Um passo de 1/120 s e limite de 8 subpassos são valores iniciais para avaliação, não parâmetros já validados. Se houver atraso excessivo, decidir entre descartar atraso acumulado ou pausar; não acelerar vários segundos de batalha ao retornar de uma aba oculta. Cliques são processados uma vez por quadro; movimento, invencibilidade e projéteis usam cada subpasso.

Passos menores reduzem saltos, mas não garantem colisão contínua para qualquer velocidade. Se projéteis rápidos atravessarem a alma entre posições amostradas, considerar colisão varrida como melhoria separada. Limitar `deltaTime` também descarta tempo: isso precisa ser tratado como política de jogabilidade, não como correção sem efeitos.

### 7.4 Preservação do bullet hell

- Manter coordenadas locais da arena e aplicar o deslocamento apenas no desenho.
- Preservar origem no canto superior esquerdo e altura específica das lanças.
- Manter a cor de invencibilidade, duração configurada de proteção e dano acumulado.
- Preservar colisão AABB existente, inclusive sua regra de que somente tocar a borda não conta como interseção.
- Manter os limites da arena, remoção de projéteis e encerramento do padrão.
- Testar todas as variantes de `BulletPatternType`, inclusive perseguição e labirinto.
- Manter inicialmente o comportamento diagonal: hoje os eixos são aplicados separadamente e a diagonal pode ser mais rápida. Normalizar o vetor altera o equilíbrio e deve ser uma decisão separada.
- Para comparações reproduzíveis, permitir injetar uma semente de teste no gerador aleatório; não substituir aleatoriedade por funções raylib apenas por causa da migração.

## 8. Tipos, domínio e propriedade de objetos

Remover `Uint8`, `Uint64`, `SDL_Color` e `SDL_Event` dos headers de UI. Usar `std::uint8_t` para canais e um tipo próprio como `UiColor`, ou `Color` da raylib se for aceitável acoplar a UI à biblioteca. A recomendação é manter a conversão dentro do renderer.

`HealthBar` continua desenhando a proporção atual/máxima. Avaliar proteção quando o máximo for zero; isso é uma robustez adicional, não uma exigência causada pela raylib. Preservar inicialmente a conversão dos valores do domínio para inteiros exibidos, sem mudar precisão de dano no mesmo patch.

`Battle` mantém referências aos personagens; `BattleScreen` mantém referências a `Battle`, personagens e renderer. O controlador deve criá-los nessa ordem e destruí-los na ordem inversa. Não copiar `Battle` inadvertidamente: ele possui ponteiros de ações liberados no destrutor. Uma conversão futura para `unique_ptr` é útil, mas não é pré-requisito para a troca gráfica.

O caminho gráfico deve continuar usando os métodos não bloqueantes (`heroUseSkill`, `heroUseItem`, `heroDefend`, integração da arena), sem chamar `Battle::start()` nem executar menus com `cin`.

Há duas opções para `Game`: mantê-lo isolado como legado ou fazê-lo possuir o contexto gráfico. Se for adotada a segunda, substituir o corpo atual de `Game::run()`; não apenas chamá-lo no novo `main`, porque isso inicia a batalha de console.

## 9. Build, dependências e CMake

### 9.1 Configuração desktop

Usar o backend `Desktop` da raylib com GLFW para remover SDL da stack escolhida. A raylib também oferece uma plataforma SDL: selecioná-la contrariaria o objetivo de eliminar essa dependência.

A raylib 6.2 exige CMake 3.22 em sua compilação a partir do código-fonte. Atualizar o mínimo do projeto quando ela for integrada por `FetchContent`. [CMake da versão 6.2](https://github.com/raysan5/raylib/blob/6.2/CMakeLists.txt)

Trecho proposto, mantendo a lista explícita atual de fontes no local indicado:

```cmake
cmake_minimum_required(VERSION 3.22)
project(TurnBasedBattleGame LANGUAGES C CXX)

include(FetchContent)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 6.2
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(raylib)

# Manter aqui add_executable(game ...) com a lista de fontes do projeto.
target_compile_features(game PRIVATE cxx_std_17)
set_target_properties(game PROPERTIES CXX_EXTENSIONS OFF)
target_include_directories(game PRIVATE include)
target_link_libraries(game PRIVATE raylib)
```

Esse trecho não é um CMakeLists completo: depende do `add_executable` indicado. Remover os dois `find_package` SDL e os respectivos targets da linkagem. Para maior reprodutibilidade, registrar também o commit resolvido da tag ou usar arquivo de release com hash verificado; o relatório não inventa um hash.

`FetchContent` requer acesso à rede na obtenção inicial. Em ambiente offline, fornecer a mesma versão por checkout local e override da dependência. Não misturar headers de uma versão com binários de outra. `find_package(raylib CONFIG REQUIRED)` é alternativa quando a instalação for gerenciada externamente, com versão e plataforma verificadas.

Comandos propostos após implementar o CMake:

```bash
cmake -S . -B build-desktop -DCMAKE_BUILD_TYPE=Debug -DPLATFORM=Desktop
cmake --build build-desktop --parallel
./build-desktop/game
```

Usar um diretório novo evita reutilizar caches SDL. Não é necessário apagar `build/` para fazer a migração.

### 9.2 Assets e execução

Copiar `assets/` para junto do executável ou instalar os arquivos numa localização conhecida. Resolver o caminho a partir da localização da aplicação no desktop, com uma estratégia própria no web; não presumir que o diretório de trabalho é a raiz do repositório.

Se a cópia usar apenas `POST_BUILD`, mudanças isoladas em assets podem não disparar a cópia. Criar uma etapa que acompanhe os recursos ou documentar uma etapa explícita de empacotamento. Testar tanto execução pela IDE quanto por outro diretório.

### 9.3 Linux e Docker

Remover `libsdl3-dev` e `libsdl3-ttf-dev`. Instalar compiladores, CMake compatível, Git/certificados para obtenção da dependência e bibliotecas de desenvolvimento do backend escolhido. No perfil X11, verificar os pacotes de X11, Xrandr, Xinerama, Xcursor, Xi e OpenGL/Mesa; para Wayland, preparar o conjunto correspondente e habilitar a opção de build. A lista exata deve ser conferida na imagem Ubuntu adotada. [Guia Linux](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux)

Não presumir que a biblioteca elimina dependências do sistema gráfico. Fixar as opções X11/Wayland conforme o ambiente e validar a execução no display de destino. [Opções da raylib 6.2](https://github.com/raysan5/raylib/blob/6.2/CMakeOptions.txt)

No Docker, usar um diretório de build interno limpo, excluir `.git`, executáveis locais e `build-*` do contexto, e preservar os assets necessários. A compilação pode funcionar sem display; executar a janela requer acesso ao servidor gráfico e, quando aplicável, à GPU. Separar a validação de compilação da validação visual; não declarar o jogo funcional apenas porque a imagem foi construída.

## 10. Segunda entrega: WebAssembly e WebGL

### 10.1 Papel das tecnologias

WebAssembly executa o C++ compilado; WebGL desenha no canvas através do backend web da raylib. Emscripten produz o módulo e a integração JavaScript. A migração web recompila também a raylib para o destino; uma biblioteca nativa Linux não pode ser linkada no módulo Wasm. [Guia web da raylib](https://github.com/raysan5/raylib/wiki/Working-for-Web-%28HTML5%29)

### 10.2 Callback e duração dos objetos

Extrair a operação de um quadro para uma função reutilizada pelo loop desktop e pelo callback web. Usar um contexto de aplicação cuja duração cubra todos os callbacks. Não guardar ponteiros para objetos locais já destruídos após o retorno do `main`.

No navegador, registrar a função com a API de main loop do Emscripten. Ao solicitar saída, cancelar o loop e finalizar recursos de forma controlada ou mostrar uma tela de encerramento. O botão EXIT não deve prometer fechar a aba. O fechamento real da página não garante execução de toda a limpeza C++ e não deve ser o único momento de salvar progresso. [Modelo de execução Emscripten](https://emscripten.org/docs/porting/emscripten-runtime-environment.html)

Não adicionar Asyncify como primeira solução para manter o loop bloqueante. O fluxo gráfico atual já permite organizar atualização por quadro; leituras de console devem ficar fora dele.

### 10.3 Perfil de compilação

Instalar e fixar uma versão do SDK Emscripten depois de validar sua combinação com raylib 6.2. Não há versão do SDK validada neste relatório.

Comandos ilustrativos, após ativar o SDK e implementar o perfil:

```bash
emcmake cmake -S . -B build-web -DCMAKE_BUILD_TYPE=Release \
  -DPLATFORM=Web -DOPENGL_VERSION="ES 3.0"
cmake --build build-web --parallel
```

No ramo `if(EMSCRIPTEN)` do CMake, definir sufixo `.html` do executável e empacotar assets. Para exigir WebGL 2, manter consistência com ES 3.0. A raylib 6.2 propaga flags WebGL correspondentes em seu target; verificar o comando de link efetivo antes de duplicar opções. [Build web da biblioteca](https://github.com/raysan5/raylib/blob/6.2/src/CMakeLists.txt)

Após criar a pasta de recursos, um exemplo de opções adicionais é:

```cmake
if(EMSCRIPTEN)
    set_target_properties(game PROPERTIES SUFFIX ".html")
    target_link_options(game PRIVATE
        "SHELL:--preload-file '${CMAKE_CURRENT_SOURCE_DIR}/assets@/assets'"
    )
endif()
```

Verificar o quoting com a toolchain e com caminhos que contenham espaços. O código web deve procurar os recursos em `/assets`. O preload os disponibiliza no sistema de arquivos virtual; o filesystem real do computador do jogador não é exposto como no desktop. [Empacotamento Emscripten](https://emscripten.org/docs/porting/files/packaging_files.html)

### 10.4 Publicação e operação

- Publicar juntos HTML, JavaScript, Wasm e arquivo de dados, quando gerado.
- Testar por servidor HTTP local; não usar abertura por `file://` como validação.
- Configurar tipo MIME de Wasm e caminhos de assets corretamente.
- Versionar o conjunto publicado para evitar JavaScript antigo com Wasm novo no cache.
- Testar Chromium e Firefox e, se fizer parte do público-alvo, Safari; registrar versões e resultados reais.
- Mostrar erro de carregamento ou de contexto gráfico; não deixar apenas um canvas preto.
- Definir pausa ao perder foco/visibilidade para evitar dano instantâneo ao retornar.
- A primeira versão pode exigir teclado e mouse; suporte móvel precisa de controles de toque e layout próprios.
- Saves futuros precisam de persistência explícita, por exemplo IndexedDB; arquivos apenas em memória não garantem sobrevivência a recarregamento.
- Se áudio for adicionado, iniciar/retomar após interação do usuário conforme a política do navegador. [Áudio no Emscripten](https://emscripten.org/docs/porting/Audio.html)

### 10.5 Escala do canvas

Manter 1280 × 720 como resolução lógica inicial. Se a apresentação for escalada, usar uma transformação consistente para desenho e entrada. Para letterboxing, a conversão conceitual é:

```text
escala = min(larguraDisponivel / 1280, alturaDisponivel / 720)
offsetX = (larguraDisponivel - 1280 * escala) / 2
offsetY = (alturaDisponivel - 720 * escala) / 2
mouseLogico = (mouseNaArea - offset) / escala
```

Não aplicar essa transformação duas vezes se o backend já normaliza o mouse. Conferir canvas CSS, resolução interna e densidade de pixels. Cliques nas margens devem ser ignorados. Câmera do mundo, se adicionada, não deve deslocar o HUD.

## 11. WebGPU e melhorias futuras

WebGPU não consta como backend gráfico da raylib 6.2 nas opções consultadas. Sua adoção exige um backend adicional, outro renderizador ou uma integração específica. Wasm pode chamar WebGPU por bindings como Emdawnwebgpu, mas essa possibilidade não transforma automaticamente as chamadas raylib em WebGPU. [Plataformas raylib](https://github.com/raysan5/raylib/wiki/raylib-platforms-and-graphics) · [Emdawnwebgpu](https://dawn.googlesource.com/dawn/+/refs/heads/main/src/emdawnwebgpu/README.md)

Para este jogo, WebGPU fica fora da entrega de migração. Caso se torne requisito, avaliar separadamente pipelines, buffers, shaders, carregamento de texturas, renderização de fontes e compatibilidade dos navegadores. Não prometer ganho automático de FPS.

Depois da equivalência funcional, podem ser adicionados:

| Evolução | Trabalho adicional |
|---|---|
| Sprites e animação | Assets, recortes por quadro, estados e temporizadores |
| Áudio | Gerenciador de recursos, volume, efeitos e atualização de música em streaming |
| Câmera de exploração | Transformações de mundo, mapa e colisões do cenário |
| Shaders | Versões compatíveis com desktop/web e testes visuais |
| Partículas | Emissão, vida útil, limites de quantidade e medição de custo |
| Resolução adaptável | Transformação de input, escalonamento e legibilidade |

Esses itens não devem bloquear a remoção do SDL nem ser usados para medir a equivalência da primeira entrega.

## 12. Problemas existentes a registrar antes da comparação

| Achado | Efeito potencial | Tratamento |
|---|---|---|
| Texto Victory/Defeat invertido em concatenações de `battleMessage` | Mensagem diverge do log/indicador final | Corrigir em mudança identificável e testar ambos os resultados |
| Botões de habilidades/itens recriados no desenho | Hover e pressed descartados | Resolver junto à adaptação da entrada |
| Fonte absoluta do Linux | Falha fora da máquina configurada | Substituir por asset distribuído |
| Tipos SDL vazando pelos headers | Dependência transitiva em componentes de UI | Remover todos os contratos afetados |
| `Game::run()` ainda aponta para console | Possibilidade de iniciar caminho bloqueante | Isolar ou reescrever explicitamente |
| Efeitos expiram dentro do desenho | Desenho altera o estado | Transferir expiração para atualização |
| README descreve interface e inventário como futuros | Instruções divergentes do código | Atualizar descrição e comandos |
| Ausência de testes inventariados | Dificuldade de separar regressão de defeito anterior | Registrar baseline e testar fluxos críticos |

Esses achados são de inspeção estática. O jogo não foi executado nesta análise, e não há medições de desempenho que sustentem comparações numéricas.

## 13. Sequência de implementação e entregáveis

| Etapa | Trabalho | Dependência | Critério de saída |
|---|---|---|---|
| 0 — Baseline | Registrar build SDL, capturas e fluxos atuais; listar defeitos conhecidos | Ambiente atual | Evidência comparável disponível |
| 1 — Build e renderer | Integrar versão fixa, abrir janela, desenhar formas/textos e empacotar fonte | Etapa 0 | Tela mínima executando sem SDL |
| 2 — Interface e input | Adaptar Button/HealthBar/BattleScreen, tipos e estados | Etapa 1 | Todos os menus e cliques funcionam |
| 3 — Tempo e combate | Relógio, integração da arena, saída e reinício | Etapa 2 | Batalha completa e todos os padrões validados |
| 4 — Distribuição desktop | Docker, caminhos, licenças, README e build Release | Etapa 3 | Pacote executa fora da árvore de fontes |
| 5 — Web | Callback, Emscripten, preload, foco, canvas e saída web | Etapa 4 | Partida completa pelo navegador |
| 6 — Evoluções | Áudio, sprites e efeitos conforme prioridade do jogo | Equivalência concluída | Critérios específicos por recurso |

Estimativa qualitativa: renderer/build têm complexidade média; entrada, transições e temporização concentram maior risco funcional; web adiciona integração de plataforma e distribuição. Não há estimativa confiável de dias sem validar o ambiente de compilação, os sistemas-alvo e o escopo final da entrega web.

Manter mudanças revisáveis por etapa e preservar a revisão SDL de referência. Reverter por commits de migração se necessário, sem apagar trabalho anterior. Não é obrigatório manter dois backends simultâneos no produto.

## 14. Plano de validação

### 14.1 Compilação e dependências

- Compilar Debug e Release em diretórios novos.
- Verificar ausência de SDL/TTF em `src`, `include` e dependências do executável.
- Conferir que o backend da raylib selecionado não é SDL.
- Compilar uma pequena unidade que inclua cada header de UI para identificar dependências transitivas esquecidas.
- Abrir o pacote por outro diretório e conferir fonte/recursos.
- Fazer o teste web com toolchain e biblioteca recompiladas para Wasm.

Uma busca como `rg -n 'SDL_|SDL3|TTF_|Uint8|Uint64' src include CMakeLists.txt Dockerfile` ajuda a localizar resíduos. Menções históricas no relatório não são dependências do executável.

### 14.2 Matriz funcional

| Caso | Resultado esperado |
|---|---|
| Início | HP/MP, nomes, quatro entradas de log no máximo e estado inicial corretos |
| Skill com mana | Uma ação e um consumo de mana |
| Skill sem mana | Não executa nem passa turno indevidamente |
| Cura | HP respeita máximo e efeito visual termina |
| Defesa | Regra existente preservada e turno avança corretamente |
| Item válido | Quantidade reduz uma unidade e atributos corretos |
| Item inútil/indisponível | Bloqueado sem consumir turno |
| Back e submenus | Nenhum clique atravessa para outra tela |
| Mouse mantido | Nenhuma repetição de habilidade, item ou Continue |
| Hover após transição | Estado visual corresponde à nova tela |
| WASD e setas | Mesmas direções e limites da arena |
| Todos os padrões | Spawn, movimento, dimensões e término corretos |
| Colisão/invencibilidade | Dano aplicado segundo intervalo e projétil removido |
| Vitória e derrota | Mensagens, log e botões finais coerentes |
| Restart | Personagens, inventário conforme reset atual, arena e UI reiniciados |
| Exit/X/Escape | Comportamento definido para cada plataforma |
| Aba oculta/foco | Sem salto de simulação ou teclas presas ao retornar |
| Canvas escalado | Botões clicáveis exatamente onde aparecem |
| Fonte ausente | Erro/fallback definido sem crash silencioso |

### 14.3 Testes automatizados úteis

Priorizar testes sem janela para dano, mana, consumo de itens, limites da alma, colisão, invencibilidade e encerramento de padrões. Adicionar testes de transição usando snapshots sintéticos se a UI ficar separada da leitura raylib. Comparar cenários com sementes controladas; não exigir trajetórias idênticas entre execuções aleatórias.

Validar manualmente fontes, bordas, clipping e posições. A compilação não comprova esses aspectos. Capturas SDL e raylib devem ser feitas no mesmo estado e resolução.

### 14.4 Desempenho

Medir tempo de quadro e uso de memória no mesmo hardware, resolução e cenário, especialmente com muitos projéteis. Separar build Debug de Release e VSync de tempo de CPU. Para uma meta de 60 FPS, usar 16,7 ms como orçamento de quadro, não como desempenho já observado. Verificar que fontes e futuros sons/texturas não sejam recarregados a cada frame.

## 15. Riscos e mitigação

| Risco | Impacto | Mitigação |
|---|---|---|
| Início/fim de desenho em dois lugares | Renderização inconsistente | Um único proprietário do ciclo de quadro |
| Confusão ms/segundos | Efeitos e movimento incorretos | Unidades explícitas e casos temporais controlados |
| Clique reaplicado após transição | Múltiplas ações por entrada | Consumo único por quadro |
| Botões reconstruídos após hover | Interface sem feedback | Atualizar estrutura antes da entrada |
| Grande delta após suspensão | Dano ou término instantâneo | Política de pausa e limite de recuperação |
| Recursos liberados após contexto | Falha no encerramento | Ordem de vida e liberação explícita |
| Fonte/glifos ausentes | Texto ilegível | Assets próprios e teste de cobertura |
| Cache nativo usado no web | Erro de configuração/link | Diretórios e toolchains separados |
| Dependências X11/Wayland incompletas | Build ou janela falham | Perfil Linux explícito e teste no destino |
| Pressupor suporte WebGPU | Escopo e arquitetura incorretos | Planejar WebGL como entrega raylib web |

## 16. Critérios de conclusão

A migração desktop está concluída quando o jogo compila e executa sem SDL3/SDL3_ttf, completa todos os fluxos existentes, distribui sua fonte, preserva a simulação e possui instruções reproduzíveis. A versão web só está concluída após uma partida completa no navegador com recursos carregados, input correto e pausa/saída definidos.

Os exemplos deste documento precisam ser integrados e testados durante a implementação. Esta entrega consiste no relatório: não altera o backend atual, não instala dependências, não publica o jogo e não afirma que os testes propostos já passaram.
