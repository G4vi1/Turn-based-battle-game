# ⚔️ Turn Game

> Jogo de batalha por turnos desenvolvido em C++ com foco na aplicação de Programação Orientada a Objetos.

---

# 📖 Visão Geral

O projeto implementa um sistema de combate em turno utilizando conceitos de Programação Orientada a Objetos, separando responsabilidades entre classes para facilitar manutenção, expansão e reutilização do código.

---

# ✨ Funcionalidades

- Sistema de batalha por turnos.
- Sistema de ataque utilizando habilidades.
- Sistema de defesa.
- Sistema de mana.
- Validação de mana antes da execução de habilidades.
- Barras visuais de HP.
- Barras visuais de MP.
- Exibição do estado da batalha a cada turno.
- Determinação automática do vencedor.

---

# 🔄 Fluxo Geral da Batalha

```mermaid
flowchart TD

A[Início da batalha]

A --> B[Turno do jogador]

B --> C[Exibe status]

C --> D[Escolhe ação]

D --> E{Atacar?}

E -- Sim --> F[Escolher habilidade]

F --> G{Mana suficiente?}

G -- Não --> F

G -- Sim --> H[Executa ataque]

H --> I[Atualiza HP]

E -- Defender --> J[Ativa defesa]

I --> K{Oponente vivo?}

J --> K

K -- Sim --> L[Turno do adversário]

L --> C

K -- Não --> M[Fim da batalha]
```

---

# 🏛️ Arquitetura

O projeto é dividido em três classes principais, cada uma responsável por uma parte específica do sistema.

```mermaid
classDiagram

class Batalha{
+iniciar()
+turno()
+menu()
+mostrarStatus()
}

class Personagem{
+atacar()
+defender()
+receberDano()
+usoMana()
+receberCura()
}

class Habilidade{
+nome
+valorAtaque
+custoMana
}

Batalha --> Personagem : controla
Personagem --> Habilidade : utiliza
```

---

# 🔁 Sequência de um Ataque

```mermaid
sequenceDiagram

participant Jogador
participant Batalha
participant Personagem
participant Habilidade

Jogador->>Batalha: Escolhe atacar
Batalha->>Personagem: atacar()
Personagem->>Habilidade: Consulta dano e custo
Habilidade-->>Personagem: Retorna valores
Personagem->>Personagem: Consome mana
Personagem->>Personagem: Calcula dano
Personagem->>Personagem: Aplica dano no alvo
Personagem-->>Batalha: Atualiza atributos
Batalha-->>Jogador: Exibe novo status
```

---

# 📂 Estrutura do Projeto

```text
Turn-game/
│
├── README.md
│
└── game/
    ├── main.cpp
    ├── Batalha.cpp
    ├── Batalha.hpp
    ├── Personagem.cpp
    ├── Personagem.hpp
    ├── Habilidade.cpp
    ├── Habilidade.hpp
    └── Makefile
```

---

# 📚 Classes

## Batalha

Responsável por controlar toda a lógica do combate.

### Principais responsabilidades

- Alternância de turnos.
- Exibição do estado da batalha.
- Leitura das ações do jogador.
- Encerramento da partida.

---

## Personagem

Representa um combatente.

### Principais responsabilidades

- Armazenar atributos.
- Atacar.
- Defender.
- Receber dano.
- Consumir mana.
- Gerenciar vida e mana.

---

## Habilidade

Representa uma habilidade utilizável durante o combate.

### Principais responsabilidades

- Nome.
- Valor de ataque.
- Custo de mana.

---

# 💡 Conceitos de POO Aplicados

| Conceito | Aplicação |
|----------|-----------|
| Encapsulamento | Todos os atributos privados acessados por getters e setters. |
| Associação | A classe `Batalha` trabalha com objetos `Personagem`. |
| Composição | Cada `Personagem` possui um conjunto de `Habilidade`. |
| Abstração | Cada classe representa uma entidade específica do domínio. |
| Const | Métodos que não alteram estado e parâmetros constantes. |
| Referências | Evitam cópias de objetos durante o combate. |

---

# 🚀 Compilação

```bash
g++ *.cpp -o jogo
```

---

# ▶️ Execução

```bash
./jogo
```

---

# 💻 Exemplo

```text
=====================================
         A LUTA COMEÇA!
=====================================

========== STATUS DA BATALHA ==========

Arcanjo
[====================] 100 HP
[********************] 100 MP

Leviathan
[====================] 100 HP
[********************] 100 MP

=======================================
```

---

# 🔮 Melhorias Futuras

- [ ] Interface gráfica.
- [ ] Sistema de inventário.
- [ ] Efeitos de status.
- [ ] Inteligência Artificial.
- [ ] Sons.
- [ ] Animações.
- [ ] Sistema de níveis.
- [ ] Novas habilidades.
- [ ] Salvamento de progresso.

---

# 📄 Licença

Projeto desenvolvido para fins de estudo e prática de Programação Orientada a Objetos em C++.