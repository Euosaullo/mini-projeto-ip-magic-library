# Documentação das Funções - Magic Library

Este documento explica, de forma didática, as principais funções do projeto **Magic Library**. A ideia é servir tanto como material de estudo quanto como apoio para a apresentação do vídeo.

O projeto foi dividido em módulos para manter o código organizado. Cada arquivo possui uma responsabilidade específica: o `main.c` controla o fluxo do programa, o `library.c` gerencia os livros, o `files.c` cuida do salvamento e carregamento, o `encryption.c` implementa a criptografia e o `utils.c` reúne funções auxiliares reutilizáveis.

---

## Visão Geral da Organização

| Arquivo | Responsabilidade |
|:---:|---|
| `main.c` | Controla a execução do programa, o menu principal e a chamada das operações |
| `library.c` | Implementa o cadastro, edição, listagem, remoção, busca e perfil de poder dos livros |
| `files.c` | Salva e carrega os livros no arquivo de dados |
| `encryption.c` | Criptografa e descriptografa os títulos dos livros |
| `utils.c` | Concentra funções auxiliares de leitura, cópia de texto e validação |
| `structs.h` | Define as estruturas de dados usadas pelo sistema |

> Observação: algumas funções são declaradas como `static`. Isso significa que elas são internas ao próprio arquivo `.c` onde foram criadas. Elas ajudam na organização do módulo, mas não são chamadas diretamente por outros arquivos.

---

# `main.c`

O arquivo `main.c` é responsável por iniciar o programa, carregar os dados, mostrar o menu e direcionar o usuário para a funcionalidade escolhida.

## `main`

A função `main` é o ponto de entrada do programa. Ela recebe os argumentos da linha de comando e verifica se o usuário informou o nome do arquivo de dados.

Se o arquivo não for informado, o programa mostra uma mensagem de uso e encerra. Caso contrário, a função inicializa a biblioteca, carrega os livros já salvos e entra em um laço que mantém o menu ativo até o usuário escolher a opção de salvar e sair.

Durante a execução, `main` usa um `switch` para decidir qual função será chamada de acordo com a opção escolhida: cadastrar, deletar, mostrar, editar, listar ou sair.

**Ponto importante para explicar:**

A biblioteca é um vetor de ponteiros para `MagicBook`. Isso permite que cada livro seja alocado dinamicamente e que posições vazias sejam representadas por `NULL`.

## `readMenuOption`

Essa função imprime o menu principal no terminal e lê a opção digitada pelo usuário.

Antes de mostrar as opções, ela calcula quantos livros estão cadastrados usando `countRegisteredBooks`. Depois, exibe o menu com bordas e linhas formatadas, deixando a interface mais organizada.

A leitura é feita com `scanf`. Se o usuário digitar algo que não seja número, a função retorna `-1`, permitindo que o programa trate a opção como inválida. Se a entrada acabar, ela retorna `6`, que corresponde à opção de salvar e sair.

## `readBookId`

Essa função lê um ID informado pelo usuário. Ela recebe uma mensagem como parâmetro, exibe essa mensagem no terminal e tenta ler um número inteiro.

Ela é usada nas operações que precisam localizar um livro específico, como deletar, mostrar ou editar. Se a leitura for válida, retorna verdadeiro. Se o usuário digitar algo inválido, retorna falso.

## `countRegisteredBooks`

Essa função percorre todas as posições do vetor da biblioteca e conta quantas estão ocupadas.

Como posições vazias ficam com `NULL`, basta verificar quais posições são diferentes de `NULL`. O resultado é usado no menu para mostrar algo como `Registered books: 003/100`.

## `printMenuLine`

Essa função imprime uma linha formatada dentro do menu. Ela recebe um texto e o coloca dentro de uma estrutura visual com bordas.

Ela também limita o tamanho do texto exibido, evitando que uma linha muito grande quebre o formato do menu.

## `printMenuInfoLine`

Essa função monta uma linha informativa do menu, juntando um rótulo e um valor. Por exemplo: `Registered books: 003/100`.

Depois de montar essa string, ela chama `printMenuLine`, reutilizando a mesma lógica de impressão visual.

## `printMenuBorder`

Essa função imprime as bordas do menu. Ela recebe um caractere, como `=` ou `-`, e o repete várias vezes para formar uma linha horizontal.

Esse tipo de função não altera a lógica do sistema, mas melhora bastante a aparência da interface no terminal.

---

# `library.c`

O arquivo `library.c` concentra a lógica principal do sistema. Nele ficam as operações sobre os livros e também as funções relacionadas aos atributos de RPG e ao perfil de poder.

## `initializeLibrary`

Essa função prepara a biblioteca para uso. Ela percorre o vetor de livros e coloca todas as posições como `NULL`.

Isso é essencial porque o programa usa `NULL` para identificar quais posições estão vazias. Sem essa inicialização, o vetor poderia conter lixo de memória, causando comportamento incorreto.

## `freeLibrary`

Essa função libera a memória ocupada pelos livros cadastrados.

Como cada livro é criado com alocação dinâmica usando `malloc`, é necessário liberar essa memória com `free` antes de encerrar o programa. A função percorre todas as posições, verifica quais possuem livros e libera cada uma delas.

Depois de liberar, a posição recebe `NULL`, evitando ponteiros pendurados.

## `registerBook`

Essa função realiza o cadastro completo de um novo livro mágico.

Primeiro, ela procura uma posição livre no vetor usando `findFreeLibrarySlot`. Depois, aloca memória para um novo `MagicBook`. Em seguida, lê e valida o ID, garantindo que ele seja positivo e que ainda não exista outro livro com o mesmo ID.

Depois do ID, a função lê o título, o nome do autor, a data de nascimento do autor, a data de escrita e os atributos opcionais de RPG.

Se alguma etapa falhar, a memória alocada é liberada e o cadastro é cancelado. Se tudo der certo, o ponteiro do novo livro é colocado na posição livre do vetor.

## `deleteBookById`

Essa função remove um livro a partir do ID informado.

Ela primeiro procura o livro com `findBookIndexById`. Se o livro não existir, mostra uma mensagem de erro. Se existir, libera a memória daquele livro com `free` e marca a posição como `NULL`.

Essa lógica mantém a biblioteca consistente, porque a posição fica disponível para um novo cadastro futuro.

## `displayBookById`

Essa função mostra os detalhes completos de um livro.

Ela procura o livro pelo ID e, se encontrar, exibe o ID, título, autor, data de nascimento do autor e data de escrita. Depois disso, chama `displayBookAttributes` para mostrar os atributos de RPG e `displayBookPowerProfile` para mostrar o perfil de poder.

Essa função é importante porque reúne a visualização completa do livro, incluindo as funcionalidades extras adicionadas ao projeto.

## `updateBookById`

Essa função edita um livro existente de forma seletiva.

Primeiro, ela localiza o livro pelo ID. Depois, copia os dados atuais para variáveis temporárias. Isso é importante porque, se alguma etapa da edição falhar, os dados originais continuam preservados.

A função pergunta separadamente se o usuário deseja editar o título, o autor, a data de nascimento, a data de escrita e os atributos de RPG. Apenas os campos escolhidos são alterados.

No final, se alguma mudança foi feita, os novos dados substituem os antigos. Se nenhuma alteração for escolhida, o programa informa que nada foi modificado.

## `listBookTitles`

Essa função lista todos os livros cadastrados.

Ela percorre o vetor da biblioteca e imprime o ID e o título de cada posição ocupada. Se nenhuma posição estiver ocupada, informa que a biblioteca está vazia.

Essa é uma função mais simples, mas muito útil para o usuário consultar rapidamente quais livros existem antes de escolher um ID para exibir, editar ou deletar.

## `readRequiredText`

Essa função lê um texto obrigatório, como título ou nome do autor.

Ela usa uma função auxiliar para ler a linha e verifica se o texto não ficou vazio. Se o usuário apertar Enter sem digitar nada, a função mostra uma mensagem e pede novamente.

Isso evita que livros sejam cadastrados com campos essenciais em branco.

## `readValidDate`

Essa função lê uma data formada por dia, mês e ano.

Ela usa `scanf` para obter três números e depois chama `isValidDate` para conferir se a data existe de verdade. Por exemplo, ela rejeita datas como `31/02/2024`.

A função também considera anos bissextos, então `29/02` só é aceito quando o ano permite essa data.

## `readBookAttributes`

Essa função lê todos os atributos opcionais de RPG de um livro.

Antes de começar, ela chama `initializeBookAttributes`, garantindo que todos os atributos comecem como ausentes. Depois pergunta, um por um, se o livro possui FOR, DES, CON, INT, SAB, CAR e MAG.

Quando o usuário responde que o livro possui determinado atributo, a função pede o valor entre 1 e 20. Quando responde que não possui, o valor daquele atributo fica como zero.

## `initializeBookAttributes`

Essa função coloca todos os atributos em um estado inicial seguro.

Cada atributo possui duas informações: uma flag indicando se ele existe e um valor numérico. Essa função define todas as flags como `0` e todos os valores como `0`.

Isso permite diferenciar um atributo ausente de um atributo presente com valor válido.

## `readOptionalAttribute`

Essa função lê um atributo específico.

Ela recebe as mensagens que serão exibidas, além dos ponteiros para a flag de existência e para o valor do atributo. Primeiro pergunta se o atributo existe. Se não existir, define o valor como zero. Se existir, chama `readAttributeValue` para ler o valor entre 1 e 20.

Essa função evita repetição, porque a mesma lógica serve para força, destreza, constituição, inteligência, sabedoria, carisma e magia.

## `editBookAttributes`

Essa função controla a edição dos atributos de RPG.

Ela pergunta separadamente se o usuário deseja editar cada atributo. Isso combina com a proposta da edição seletiva: o usuário não precisa redigitar todos os atributos, apenas aqueles que quer alterar.

Para cada atributo, a função chama `editOptionalAttribute`, que realiza a alteração específica.

## `editOptionalAttribute`

Essa função edita um único atributo opcional.

Primeiro, pergunta se o usuário deseja atualizar aquele atributo. Se a resposta for não, ela mantém o valor atual. Se a resposta for sim, marca que houve alteração e chama `readOptionalAttribute` para redefinir se o atributo existe e qual é seu valor.

Com isso, o usuário pode tanto mudar o valor de um atributo quanto remover completamente aquele atributo do livro.

## `askYesOrNo`

Essa função padroniza perguntas de sim ou não.

Ela aceita apenas `1` para sim e `0` para não. Se o usuário digitar outro número ou uma entrada inválida, a função mostra uma mensagem e pergunta novamente.

Isso reduz erros de entrada e evita repetir validações iguais em várias partes do código.

## `readAttributeValue`

Essa função lê o valor de um atributo de RPG.

Ela garante que o valor digitado seja um número e que esteja dentro do intervalo permitido, de 1 a 20. Se o valor estiver fora desse intervalo, o programa informa o erro e pede novamente.

Essa validação é importante porque o perfil de poder depende desses valores para calcular média, nível e rank.

## `displayBookAttributes`

Essa função exibe os atributos de RPG de um livro.

Ela verifica cada atributo e imprime somente aqueles que estão ativos. Por exemplo, se o livro possui apenas força e magia, apenas esses dois aparecem no terminal.

Se nenhum atributo estiver ativo, a função mostra uma mensagem informando que o livro não possui atributos de RPG cadastrados.

## `displayBookPowerProfile`

Essa função calcula e mostra o perfil de poder do livro.

Ela percorre os sete atributos possíveis e considera apenas os que estão ativos. Com isso, calcula quantos atributos existem, soma seus valores e obtém a média.

Depois, chama `calculatePowerLevel` para transformar a média em um nível de 0 a 5. Também chama `getPowerRank` para converter o nível em um rank textual.

Além disso, a função identifica o atributo dominante, ou seja, o atributo ativo com maior valor. A partir desse atributo, define também o arquétipo do livro, como `Warrior Tome`, `Scholar Grimoire` ou `Arcane Spellbook`.

## `includeAttributeInPowerProfile`

Essa função auxilia o cálculo do perfil de poder.

Ela recebe um atributo, verifica se ele está ativo e, se estiver, incrementa a quantidade de atributos ativos e soma seu valor ao total.

Ela também compara o valor do atributo atual com o maior valor encontrado até o momento. Se for maior, atualiza o atributo dominante e o arquétipo correspondente.

## `calculatePowerLevel`

Essa função converte a média dos atributos em um nível de poder.

A escala vai de `0/5` até `5/5`. Se a média for zero, o livro não possui poder calculado. Conforme a média aumenta, o nível também aumenta.

A divisão usada é:

| Média | Nível |
|:---:|:---:|
| 0 | 0/5 |
| 1 a 4 | 1/5 |
| acima de 4 até 8 | 2/5 |
| acima de 8 até 12 | 3/5 |
| acima de 12 até 16 | 4/5 |
| acima de 16 até 20 | 5/5 |

## `getPowerRank`

Essa função transforma o nível numérico de poder em um nome de rank.

Por exemplo, nível 1 vira `Weak`, nível 3 vira `Adept` e nível 5 vira `Legendary`. Isso deixa a saída do programa mais descritiva e mais fácil de entender.

## `hasAnyBookAttribute`

Essa função verifica se o livro possui pelo menos um atributo de RPG ativo.

Ela é usada principalmente por `displayBookAttributes`. Se nenhum atributo estiver ativo, o programa evita imprimir uma lista vazia e mostra uma mensagem explicativa.

## `printSectionHeader`

Essa função imprime um cabeçalho visual para separar seções no terminal.

Ela é usada em partes como cadastro, edição, detalhes do livro e perfil de poder. Isso melhora a experiência do usuário e torna a saída do programa mais organizada.

## `findFreeLibrarySlot`

Essa função procura a primeira posição livre no vetor da biblioteca.

Ela percorre o vetor e retorna o índice da primeira posição que contém `NULL`. Se todas as posições estiverem ocupadas, retorna `-1`, indicando que a biblioteca está cheia.

## `findBookIndexById`

Essa função procura um livro pelo ID.

Ela percorre todas as posições do vetor e compara o ID de cada livro cadastrado com o ID procurado. Se encontrar, retorna o índice da posição no vetor. Se não encontrar, retorna `-1`.

## `bookIdExists`

Essa função verifica se um ID já está sendo usado.

Internamente, ela usa `findBookIndexById`. Se a função encontrar o ID, significa que ele já existe. Essa verificação é usada no cadastro para impedir IDs duplicados.

---

# `files.c`

O arquivo `files.c` é responsável pela persistência dos dados. Ele permite que os livros continuem salvos mesmo depois que o programa é fechado.

## `saveLibraryToFile`

Essa função salva a biblioteca no arquivo informado pelo usuário.

Ela abre o arquivo em modo de escrita e percorre o vetor da biblioteca. Para cada livro existente, grava o ID, o título criptografado, o nome do autor, as datas e os atributos de RPG.

Ao final, fecha o arquivo e mostra uma mensagem confirmando que o salvamento foi realizado.

## `loadLibraryFromFile`

Essa função carrega os livros salvos no arquivo.

Ela abre o arquivo em modo de leitura e tenta ler os dados livro por livro. Para cada registro encontrado, aloca memória para um novo `MagicBook`, lê todos os campos e valida o resultado.

Se o livro carregado for inválido ou tiver ID duplicado, ele é ignorado. Se for válido, é inserido na primeira posição livre da biblioteca.

## `writeEncryptedTitle`

Essa função grava o título do livro de forma protegida.

Primeiro, copia o título para uma variável temporária. Depois, chama `encryptString` para criptografar o texto. Em seguida, grava cada caractere criptografado em formato hexadecimal.

O uso de hexadecimal facilita salvar caracteres criptografados no arquivo sem problemas de formatação.

## `readEncryptedTitle`

Essa função lê uma linha do arquivo contendo o título criptografado.

Depois de ler a linha, remove a quebra de linha final. Em seguida, chama `decodeEncryptedTitle`, que converte o texto hexadecimal de volta para caracteres e descriptografa o título.

## `decodeEncryptedTitle`

Essa função faz a conversão do título salvo.

O título é salvo em pares de caracteres hexadecimais. A função lê dois caracteres por vez, transforma esses dois caracteres em um byte e monta novamente a string criptografada.

Depois que a string é reconstruída, chama `decryptString` para recuperar o título original.

## `hexDigitValue`

Essa função converte um caractere hexadecimal para seu valor numérico.

Ela aceita dígitos de `0` a `9`, letras de `A` a `F` e letras de `a` a `f`. Se o caractere não for hexadecimal, retorna `-1`, indicando erro.

## `writeBookAttributes`

Essa função grava todos os atributos de RPG de um livro no arquivo.

Para cada atributo, são gravadas duas informações: se o atributo existe e qual é o valor dele. Por isso, ela salva quatorze números no total: sete flags de presença e sete valores.

## `readBookAttributesFromFile`

Essa função lê os atributos de RPG salvos no arquivo.

Ela espera encontrar exatamente quatorze valores: presença e valor para cada atributo. Se não conseguir ler todos os valores esperados, retorna erro.

Após a leitura, descarta o restante da linha para manter o arquivo alinhado para a próxima leitura.

## `isLoadedBookValid`

Essa função valida um livro carregado do arquivo antes de inserir na biblioteca.

Ela verifica se o ID é positivo, se não existe outro livro com o mesmo ID, se título e autor não estão vazios, se as datas são válidas e se os atributos de RPG respeitam as regras.

Essa validação protege o programa contra arquivos corrompidos ou editados manualmente de forma incorreta.

## `isBookAttributesValid`

Essa função valida todos os atributos de RPG de um livro.

Ela chama `isOptionalAttributeValid` para cada um dos sete atributos. Se qualquer atributo estiver fora do formato esperado, a função retorna erro.

## `isOptionalAttributeValid`

Essa função valida um único atributo opcional.

Se a flag de presença for diferente de `0` ou `1`, o atributo é inválido. Se a flag for `0`, o valor precisa ser `0`. Se a flag for `1`, o valor precisa estar entre 1 e 20.

Essa regra garante que não exista, por exemplo, um atributo marcado como ausente mas com valor positivo.

---

# `encryption.c`

O arquivo `encryption.c` concentra as funções de criptografia e descriptografia das strings.

## `encryptString`

Essa função criptografa uma string usando o complemento de 255.

Ela percorre a string caractere por caractere até encontrar o `\0`. Para cada caractere, calcula `255 - valor_do_caractere` e substitui o caractere original pelo resultado.

Essa criptografia é simples, mas atende à proposta do projeto porque impede que o título seja salvo em texto puro no arquivo.

## `decryptString`

Essa função descriptografa uma string criptografada.

Como a operação de complemento de 255 é reversível, aplicar a mesma operação duas vezes recupera o texto original. Por isso, `decryptString` simplesmente chama `encryptString` novamente.

---

# `utils.c`

O arquivo `utils.c` reúne funções auxiliares usadas por vários módulos. Isso evita repetição de código e deixa as funções principais mais limpas.

## `clearInputBuffer`

Essa função limpa o restante da entrada do teclado.

Ela lê caracteres até encontrar uma quebra de linha ou o fim da entrada. Isso é importante depois de usar `scanf`, porque o `scanf` pode deixar caracteres no buffer que atrapalhariam a próxima leitura.

## `removeLineBreak`

Essa função remove a quebra de linha de uma string.

Quando `fgets` lê uma linha, normalmente guarda também o caractere `\n`. Essa função procura esse caractere e o substitui por `\0`, marcando o fim da string.

Ela retorna verdadeiro se encontrou e removeu a quebra de linha. Se não encontrou, significa que a linha pode ter sido maior do que o espaço disponível.

## `discardRestOfLine`

Essa função descarta o restante de uma linha de um arquivo.

Ela é usada quando a leitura não consumiu a linha inteira. Isso evita que sobras de uma linha interfiram na leitura dos próximos dados.

## `readLine`

Essa função lê uma linha de texto com segurança.

Ela exibe uma mensagem, usa `fgets` para ler o texto e depois remove a quebra de linha final. Se a entrada for maior que o espaço disponível, limpa o restante do buffer.

Essa função é mais segura do que usar `scanf` com strings, porque respeita o tamanho máximo do array.

## `copyText`

Essa função copia uma string para outra respeitando o limite de tamanho do destino.

Ela copia caractere por caractere até chegar ao fim da origem ou até restar apenas espaço para o `\0`. Isso evita estouro de buffer.

Ela é usada em situações como edição de livro, quando os dados antigos são copiados para variáveis temporárias antes de confirmar a alteração.

## `isValidDate`

Essa função verifica se uma data é válida.

Primeiro, ela confere se o ano é positivo, se o mês está entre 1 e 12 e se o dia é positivo. Depois, usa uma tabela com a quantidade de dias de cada mês.

Se o ano for bissexto, fevereiro passa a ter 29 dias. No final, a função verifica se o dia informado não ultrapassa o limite do mês.

---

# Conclusão

A organização das funções mostra uma divisão clara de responsabilidades. O `main.c` coordena a execução, o `library.c` concentra as regras do sistema, o `files.c` resolve a persistência, o `encryption.c` trata a proteção dos títulos e o `utils.c` fornece ferramentas auxiliares.

Essa estrutura deixa o projeto mais legível, facilita testes, evita repetição de código e torna a apresentação mais didática, porque cada módulo pode ser explicado separadamente.