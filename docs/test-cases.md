# Casos de Teste - Magic Library

Este documento descreve casos de teste manuais para validar o funcionamento do sistema **Magic Library**.

Os testes cobrem:

- compilacao;
- execucao;
- menu principal;
- cadastro de livros;
- listagem;
- exibicao completa;
- atributos opcionais de RPG;
- perfil de poder;
- edicao seletiva;
- delecao;
- validacoes;
- salvamento e carregamento em arquivo;
- criptografia do titulo salvo.

> Observacao: os textos esperados do terminal estao em ingles porque a interface do programa foi mantida em ingles.

---

## Ambiente de Teste

### Bash

```bash
bash build.sh
./library data/magicLibrary.txt
```

### PowerShell

```powershell
.\build.ps1
.\library.exe data\magicLibrary.txt
```

### Arquivo de Dados

O arquivo usado nos testes deve ser:

```txt
data/magicLibrary.txt
```

Para testar do zero, limpe o conteudo desse arquivo antes de executar o programa.

---

## Resumo dos Testes

| ID | Area | Objetivo |
|:---:|:---:|---|
| TC-01 | Build | Compilar no Bash |
| TC-02 | Build | Compilar no PowerShell |
| TC-03 | Menu | Abrir o programa e exibir o menu |
| TC-04 | Cadastro | Cadastrar livro com varios atributos |
| TC-05 | Listagem | Listar titulos cadastrados |
| TC-06 | Exibicao | Mostrar dados, atributos e perfil de poder |
| TC-07 | Exibicao | Tentar mostrar ID inexistente |
| TC-08 | Cadastro | Bloquear ID duplicado |
| TC-09 | Cadastro | Bloquear ID invalido |
| TC-10 | Cadastro | Bloquear data invalida |
| TC-11 | Atributos | Cadastrar livro com apenas um atributo |
| TC-12 | Atributos | Cadastrar livro sem atributos |
| TC-13 | Perfil | Validar nivel Legendary |
| TC-14 | Perfil | Validar livro sem nivel de poder |
| TC-15 | Perfil | Validar atributo dominante e arquetipo |
| TC-16 | Edicao | Editar apenas o titulo |
| TC-17 | Edicao | Editar apenas um atributo e atualizar perfil |
| TC-18 | Edicao | Remover um atributo existente |
| TC-19 | Edicao | Nao alterar nenhum campo |
| TC-20 | Delecao | Deletar livro existente |
| TC-21 | Delecao | Tentar deletar ID inexistente |
| TC-22 | Persistencia | Salvar e carregar novamente |
| TC-23 | Criptografia | Verificar se o titulo nao fica em texto puro |

---

## TC-01 - Compilar no Bash

**Objetivo:** verificar se o projeto compila corretamente em Bash.

**Passos:**

```bash
bash build.sh
```

**Resultado esperado:**

```txt
Build finished: ./library
```

O arquivo executavel `library` deve ser gerado na raiz do projeto.

---

## TC-02 - Compilar no PowerShell

**Objetivo:** verificar se o projeto compila corretamente em PowerShell.

**Passos:**

```powershell
.\build.ps1
```

**Resultado esperado:**

```txt
Build finished: .\library.exe
```

O arquivo executavel `library.exe` deve ser gerado na raiz do projeto.

---

## TC-03 - Abrir o Programa e Exibir o Menu

**Objetivo:** verificar se o menu principal aparece corretamente.

**Passos no Bash:**

```bash
./library data/magicLibrary.txt
```

**Passos no PowerShell:**

```powershell
.\library.exe data\magicLibrary.txt
```

**Resultado esperado:**

```txt
+====================================================+
| MAGIC LIBRARY                                      |
| Enchanted Book Inventory                           |
+====================================================+
| Registered books: 000/100                          |
+----------------------------------------------------+
| [1] Register book                                  |
| [2] Delete book                                    |
| [3] Display book                                   |
| [4] Update book                                    |
| [5] List book titles                               |
| [6] Save and exit                                  |
+====================================================+
Choose an option:
```

---

## TC-04 - Cadastrar Livro com Varios Atributos

**Objetivo:** verificar o cadastro de um livro com atributos opcionais.

**Entrada:**

```txt
1
1
Grimorio das Chamas
Elion Arcanus
1 1 1320
2 2 1350
1
12
0
0
1
17
0
0
1
19
```

**Explicacao da entrada:**

| Campo | Valor |
|:---:|---|
| Opcao do menu | `1` |
| ID | `1` |
| Title | `Grimorio das Chamas` |
| Author | `Elion Arcanus` |
| Author birth date | `1 1 1320` |
| Writing date | `2 2 1350` |
| FOR | sim, valor `12` |
| DES | nao |
| CON | nao |
| INT | sim, valor `17` |
| SAB | nao |
| CAR | nao |
| MAG | sim, valor `19` |

**Resultado esperado:**

```txt
Book registered successfully.
```

---

## TC-05 - Listar Titulos

**Objetivo:** verificar se o livro cadastrado aparece na listagem.

**Entrada:**

```txt
5
```

**Resultado esperado:**

```txt
[ID 001] Grimorio das Chamas
```

---

## TC-06 - Mostrar Dados, Atributos e Perfil de Poder

**Objetivo:** verificar a exibicao completa do livro, incluindo o novo perfil de poder.

**Pre-condicao:** o livro do TC-04 deve estar cadastrado.

**Entrada:**

```txt
3
1
```

**Resultado esperado:**

```txt
ID: 1
Title: Grimorio das Chamas
Author: Elion Arcanus
Author birth date: 01/01/1320
Writing date: 02/02/1350
FOR / Strength: 12
INT / Intelligence: 17
MAG / Magic: 19
Active attributes: 3/7
Average attribute value: 16.00
Power level: 4/5
Rank: Arcane
Dominant attribute: MAG / Magic
Book archetype: Arcane Spellbook
```

**Observacao:** a media e calculada somente com os atributos ativos:

```txt
(12 + 17 + 19) / 3 = 16.00
```

Os atributos `DES`, `CON`, `SAB` e `CAR` nao devem aparecer, pois nao foram cadastrados para esse livro.

---

## TC-07 - Mostrar ID Inexistente

**Objetivo:** verificar se o sistema trata corretamente um ID nao encontrado.

**Entrada:**

```txt
3
999
```

**Resultado esperado:**

```txt
Book with ID 999 was not found.
```

---

## TC-08 - Bloquear ID Duplicado

**Objetivo:** verificar se o sistema impede cadastro de dois livros com o mesmo ID.

**Pre-condicao:** o livro com ID `1` ja deve estar cadastrado.

**Entrada:**

```txt
1
1
```

**Resultado esperado:**

```txt
This ID already exists. Try another one.
```

Depois da mensagem, o programa deve pedir outro ID.

---

## TC-09 - Bloquear ID Invalido

**Objetivo:** verificar se o sistema rejeita IDs invalidos.

**Entrada:**

```txt
1
abc
```

**Resultado esperado:**

```txt
Invalid ID. Try again.
```

**Entrada alternativa:**

```txt
1
-5
```

**Resultado esperado:**

```txt
The ID must be a positive number.
```

---

## TC-10 - Bloquear Data Invalida

**Objetivo:** verificar se o sistema rejeita datas impossiveis.

**Entrada durante o cadastro:**

```txt
31 2 1400
```

**Resultado esperado:**

```txt
Invalid date. Try again.
```

**Outras datas invalidas para testar:**

```txt
0 1 1400
15 13 1400
29 2 2023
```

---

## TC-11 - Cadastrar Livro com Apenas um Atributo

**Objetivo:** verificar se um livro pode ter somente um atributo de RPG e ainda assim gerar perfil de poder.

**Entrada:**

```txt
1
2
Codex Lunar
Mira Noctis
10 5 1450
20 8 1490
0
0
0
0
0
0
1
18
```

**Resultado esperado:**

```txt
Book registered successfully.
```

Ao mostrar o livro `2`, deve aparecer:

```txt
MAG / Magic: 18
Active attributes: 1/7
Average attribute value: 18.00
Power level: 5/5
Rank: Legendary
Dominant attribute: MAG / Magic
Book archetype: Arcane Spellbook
```

---

## TC-12 - Cadastrar Livro sem Atributos

**Objetivo:** verificar se um livro pode ser cadastrado sem atributos opcionais.

**Entrada:**

```txt
1
3
Manual do Aprendiz
Tarin Vale
4 4 1200
7 7 1230
0
0
0
0
0
0
0
```

**Resultado esperado:**

```txt
Book registered successfully.
```

Ao mostrar o livro `3`, deve aparecer:

```txt
No RPG attributes registered for this book.
Active attributes: 0/7
Average attribute value: 0.00
Power level: 0/5
Rank: No power level
Dominant attribute: None
Book archetype: None
```

---

## TC-13 - Validar Nivel Legendary

**Objetivo:** verificar se medias entre `17` e `20` geram nivel `5/5` e rank `Legendary`.

**Pre-condicao:** o livro `2` do TC-11 deve existir.

**Entrada:**

```txt
3
2
```

**Resultado esperado:**

```txt
Average attribute value: 18.00
Power level: 5/5
Rank: Legendary
```

---

## TC-14 - Validar Livro sem Nivel de Poder

**Objetivo:** verificar se livros sem atributos recebem nivel `0/5`.

**Pre-condicao:** o livro `3` do TC-12 deve existir.

**Entrada:**

```txt
3
3
```

**Resultado esperado:**

```txt
Active attributes: 0/7
Power level: 0/5
Rank: No power level
Dominant attribute: None
Book archetype: None
```

---

## TC-15 - Validar Atributo Dominante e Arquetipo

**Objetivo:** verificar se o maior atributo define o atributo dominante e o arquetipo.

**Pre-condicao:** o livro `1` do TC-04 deve existir.

**Entrada:**

```txt
3
1
```

**Resultado esperado:**

```txt
Dominant attribute: MAG / Magic
Book archetype: Arcane Spellbook
```

Como `MAG = 19` e maior que `INT = 17` e `FOR = 12`, o arquetipo deve ser `Arcane Spellbook`.

---

## TC-16 - Editar Apenas o Titulo

**Objetivo:** verificar se a edicao seletiva altera somente o campo escolhido.

**Pre-condicao:** o livro com ID `1` deve existir.

**Entrada:**

```txt
4
1
1
Grimorio das Chamas Eternas
0
0
0
0
0
0
0
0
0
0
0
```

**Explicacao:**

- escolhe a opcao `4`;
- informa o ID `1`;
- responde `1` para editar o titulo;
- informa o novo titulo;
- responde `0` para os outros campos e atributos.

**Resultado esperado:**

```txt
Book updated successfully.
```

Ao mostrar o livro, o titulo deve ter mudado, mas autor, datas, atributos e perfil de poder devem permanecer equivalentes.

---

## TC-17 - Editar Apenas um Atributo e Atualizar Perfil

**Objetivo:** verificar se alterar um atributo tambem atualiza media, nivel, rank e arquetipo.

**Pre-condicao:** o livro com ID `1` deve existir.

**Entrada para alterar apenas MAG para `20`:**

```txt
4
1
0
0
0
0
0
0
0
0
0
0
1
1
20
```

**Resultado esperado:**

```txt
Book updated successfully.
```

Ao mostrar o livro `1`, deve aparecer:

```txt
MAG / Magic: 20
Average attribute value: 16.33
Power level: 5/5
Rank: Legendary
Dominant attribute: MAG / Magic
Book archetype: Arcane Spellbook
```

**Calculo esperado:**

```txt
(12 + 17 + 20) / 3 = 16.33
```

---

## TC-18 - Remover um Atributo Existente

**Objetivo:** verificar se um atributo pode ser removido de um livro e se o perfil e recalculado.

**Pre-condicao:** o livro com ID `1` deve possuir `MAG`.

**Entrada:**

```txt
4
1
0
0
0
0
0
0
0
0
0
0
1
0
```

**Explicacao:**

- o usuario escolhe editar `MAG`;
- depois informa que o livro nao fornece mais `MAG`.

**Resultado esperado:**

```txt
Book updated successfully.
```

Ao mostrar o livro:

```txt
MAG / Magic
```

nao deve aparecer.

O perfil deve ser recalculado usando apenas os atributos restantes.

---

## TC-19 - Nao Alterar Nenhum Campo

**Objetivo:** verificar se o sistema reconhece quando nenhuma alteracao foi feita.

**Entrada:**

```txt
4
1
0
0
0
0
0
0
0
0
0
0
0
```

**Resultado esperado:**

```txt
No changes made.
```

---

## TC-20 - Deletar Livro Existente

**Objetivo:** verificar se o livro e removido corretamente.

**Pre-condicao:** o livro com ID `3` deve existir.

**Entrada:**

```txt
2
3
```

**Resultado esperado:**

```txt
Book deleted successfully.
```

Ao listar os titulos, o livro com ID `3` nao deve aparecer.

---

## TC-21 - Tentar Deletar ID Inexistente

**Objetivo:** verificar se o sistema trata corretamente uma tentativa de remover livro inexistente.

**Entrada:**

```txt
2
999
```

**Resultado esperado:**

```txt
Book with ID 999 was not found.
```

---

## TC-22 - Salvar e Carregar Novamente

**Objetivo:** verificar se os livros persistem no arquivo, incluindo atributos e perfil derivado.

**Passos:**

1. Cadastre pelo menos um livro.
2. Escolha a opcao `6`.
3. Execute o programa novamente usando o mesmo arquivo.
4. Escolha a opcao `5`.
5. Escolha a opcao `3` e mostre o livro carregado.

**Entrada para salvar:**

```txt
6
```

**Resultado esperado ao sair:**

```txt
Library saved successfully.
Exiting Magic Library...
```

**Resultado esperado ao abrir novamente e listar:**

```txt
[ID 001] Grimorio das Chamas Eternas
```

**Resultado esperado ao mostrar o livro carregado:**

O livro deve manter:

- dados basicos;
- atributos de RPG;
- perfil de poder recalculado corretamente.

---

## TC-23 - Verificar Criptografia no Arquivo

**Objetivo:** confirmar que o titulo nao esta salvo em texto puro.

**Passos:**

1. Cadastre um livro com titulo conhecido.
2. Saia usando a opcao `6`.
3. Abra o arquivo `data/magicLibrary.txt`.

**Exemplo de titulo cadastrado:**

```txt
Grimorio das Chamas
```

**Resultado esperado:**

O titulo **nao** deve aparecer diretamente como:

```txt
Grimorio das Chamas
```

Ele deve aparecer em formato hexadecimal, por exemplo:

```txt
B88D9692908D9690DF9B9E8CDFBC979E929E8C
```

Isso indica que o titulo foi criptografado e convertido para hexadecimal antes do salvamento.

---

## Teste Completo Sugerido

Este fluxo valida as principais funcionalidades em sequencia:

1. Compilar o projeto.
2. Executar o programa.
3. Cadastrar o livro `Grimorio das Chamas`.
4. Listar titulos.
5. Mostrar o livro pelo ID.
6. Conferir atributos e perfil de poder.
7. Editar apenas o titulo.
8. Editar apenas um atributo.
9. Conferir se o perfil de poder mudou.
10. Cadastrar um segundo livro com apenas `MAG`.
11. Cadastrar um terceiro livro sem atributos.
12. Deletar o terceiro livro.
13. Salvar e sair.
14. Executar novamente.
15. Listar os livros carregados.
16. Mostrar um livro carregado.
17. Conferir o arquivo `data/magicLibrary.txt`.

Se todos esses passos funcionarem, o sistema atende aos principais requisitos do projeto e valida as funcionalidades extras implementadas.

---

## Observacoes Finais

- Sempre saia pela opcao `6` para garantir que os dados sejam salvos.
- O ID de cada livro deve ser unico.
- Datas invalidas devem ser rejeitadas.
- Atributos de RPG aceitam valores de `1` a `20`.
- Livros podem ter todos, alguns ou nenhum atributo de RPG.
- O perfil de poder e calculado automaticamente a partir dos atributos ativos.
- O arquivo executavel (`library` ou `library.exe`) nao deve ser enviado ao GitHub.
