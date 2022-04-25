# Trabalho Prático - Processamento Gráfico

![readme](https://user-images.githubusercontent.com/52571672/165000927-b379c539-cc62-4262-a7e4-055a814d0374.jpg)

## 👾 Grupo:
Mayara Damiani e Vanessa Silveira

## 👾 Requisitos:
- GIT
- IDE para C++

## 👾 Como clonar o projeto? 

- Se você está lendo este readme pelo github, pode clicar em ![image](https://user-images.githubusercontent.com/52571672/164999984-79c92e39-5bde-4e56-b792-cafa3c033c1e.png) e escolher a melhor forma de clonar o projeto :)

- Basta abrir a pasta onde deseja adicionar o projeto e acessar o cmd na pasta, executando o seguinte comando:

```
git clone https://github.com/vnssilveira/PG2022-Trabalho-GA
```

## 👾 O que fazer após clonar o projeto?

- Acessando a pasta do Trabalho GA, abra o arquivo com a extensão '.sln'. O projeto deve ser executado através desse arquivo.


# Notas técnicas sobre o projeto:

👾 **Estrutura geral do código (classes e/ou funções):**
As principais classes se dividem em:
  - Game
  - Camadas
  - Sprite

👾 **Estrutura dos buffers e shaders:**
As especificações de vértices encontram-se na classe Sprite e os envios de shaders encontram-se na classe Camadas. O shader é setado ao adicionarmos o objeto (nesse caso utilizamos diversos sprites - jpg, png).

👾 **Gerenciamento dos sprites:**
O gerenciamento dos sprites é realizado na classe Game. São retirados e adicionados os objetos conforme a necessidade. Quando o personagem pula, caminha ou até mesmo sua posição inicial.

👾 **Como é feito o controle do personagem (teclado e/ou mouse):**

![3169603-200](https://user-images.githubusercontent.com/52571672/165001381-fd10739f-a855-4a54-a746-f44e9b24b4a9.png)

Tecla W: O personagem pula; <br>
Tecla A: O personagem anda para trás; <br>
Tecla D: O personagem anda para frente.

👾 **Como são verificadas as colisões:**
As colisões são verificadas através da comparação da posição do personagem e da bomba (o inimigo).

👾 **Como é gerenciada a animação dos sprites:**
Criamos uma lista com os sprites do persogem e da bomba e setamos uma flag para cada ação do personagem. Quando determinada ação é atribuida ao estado do personagem, o sprite coerente a está ação é selecionado.
