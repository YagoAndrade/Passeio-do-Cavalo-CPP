#include <iostream>
#define MAX 100

using namespace std;

void receberEntrada();
void printTabuleiro(int [][MAX]);
void finalizarCod(int, int, int, int [][MAX]);                       /* Funções a serem utilizadas */
void validezTour(int, int, int, int, int [][MAX]);                      /* no programa */
void iniciarTour(int, int, int, int, int [][MAX]);
void semEspacosDisponiveis(int, int, int [][MAX]);
int contarMovsUm(int [][MAX], int, int, int, int, bool);
void contarMovsDois(int [][MAX], int, int, int, int, int *);

typedef struct posicao {    int x;  int y;  } pos;  // Posição linha e coluna inicial do cavalo.

int main(void) {
    receberEntrada();
    return 0;
}

void receberEntrada() {
    /* Definindo o tabuleiro com máximo de [100][100], para que o usuário possa definir o tamanho. */
    int tabuleiro[MAX][MAX] = {};   

    /* RECEBER ENTRADAS (linhas e colunas do tabuleiro e a posição inicial do cavalo) */
    int linhas, colunas;
    pos c_pos;

    cout << "Quantidade de linhas do tabuleiro: ";
    cin >> linhas;
    cout << "Quantidade de colunas do tabuleiro: ";
    cin >> colunas;

    cout << "Linha em que o cavalo iniciara: ";
    cin >> c_pos.x;
    cout << "Coluna em que o cavalo iniciara: ";
    cin >> c_pos.y;

    /* Verificar se o tour poderá ser feito, e se sim, iniciar o tour. */
    validezTour(linhas, colunas, c_pos.x, c_pos.y, tabuleiro);
    iniciarTour(linhas, colunas, c_pos.x, c_pos.y, tabuleiro);
}

void iniciarTour(int l, int c, int l_cavalo, int c_cavalo, int tabuleiro[][MAX]) {
    /* Percorrer a quantidade de casas do tabuleiro (l * c) */
    for(int i = 1; i <= (l * c); i++) {
        tabuleiro[l_cavalo][c_cavalo] = 'C';    // Marcar onde o cavalo está como C
        printTabuleiro(tabuleiro);              // Mostrar o tabuleiro
        tabuleiro[l_cavalo][c_cavalo] = i;      // Marcar o lugar atual do cavalo como o número da posição (i)
        switch(contarMovsUm(tabuleiro, l, c, l_cavalo, c_cavalo, false)) {
            case 0:
                l_cavalo -= 2;  c_cavalo -= 1;  /* Verificar o retorno de contarMovsUm para saber para qual  */
                break;                          /* posição nós iremos (o programa sempre optará pela posição */
            case 1:                                     /* com menos posições adjacentes disponíveis */
                l_cavalo -= 1;  c_cavalo -= 2;
                break;
            case 2:
                l_cavalo += 1;  c_cavalo -= 2;
                break; 
            case 3:
                l_cavalo += 2;  c_cavalo -= 1;
                break;
            case 4:
                l_cavalo -= 2;  c_cavalo += 1;
                break;
            case 5:
                l_cavalo -= 1;  c_cavalo += 2;
                break;
            case 6:
                l_cavalo += 1;  c_cavalo += 2;
                break;
            case 7:
                l_cavalo += 2;  c_cavalo += 1;
                break;
            default:
                finalizarCod(6, l, c, tabuleiro);    /* Se a posição atual não tiver nenhuma posição adjacente disponível, um erro */
                break;                                          /* será mostrado (não há posições para ir) */
        }
    }
}

int contarMovsUm(int tabuleiro[][MAX], int l, int c, int l_cavalo, int c_cavalo, bool f_switch) {
    if(f_switch == false) {         /* Se o f_switch estiver desablitado (false), estamos procurando uma posição para ir... */
        int caminho[8];     /* ... se o f_switch estiver habilitado (true), estamos contando a acessibilidade da posição que queremos ir */

        for(int n = 0; n < 8; n++) { 
            caminho[n] = MAX;   // Inicializando o array caminho[8] com todos os valores em 100, pois buscaremos o menor valor.
        }                       // dos que serão inseridos nele.

        // À esquerda
        caminho[0] = contarMovsUm(tabuleiro, l, c, l_cavalo - 2, c_cavalo - 1, true); // 0
        caminho[1] = contarMovsUm(tabuleiro, l, c, l_cavalo - 1, c_cavalo - 2, true); // 1
        caminho[2] = contarMovsUm(tabuleiro, l, c, l_cavalo + 1, c_cavalo - 2, true); // 2
        caminho[3] = contarMovsUm(tabuleiro, l, c, l_cavalo + 2, c_cavalo - 1, true); // 3

        // À direita
        caminho[4] = contarMovsUm(tabuleiro, l, c, l_cavalo - 2, c_cavalo + 1, true); // 4
        caminho[5] = contarMovsUm(tabuleiro, l, c, l_cavalo - 1, c_cavalo + 2, true); // 5
        caminho[6] = contarMovsUm(tabuleiro, l, c, l_cavalo + 1, c_cavalo + 2, true); // 6
        caminho[7] = contarMovsUm(tabuleiro, l, c, l_cavalo + 2, c_cavalo + 1, true); // 7

        int menor_caminho = MAX;    // Buscar o menor valor contido no array caminho[8],
        int menor_caminho_pos = -1; // e retornar sua posição.

        for(int i = 0; i < 8; i++) {
            if(caminho[i] < menor_caminho) {
                menor_caminho = caminho[i];
                menor_caminho_pos = i;
            }
        }
        
        return menor_caminho_pos;

    } else { /* Como o f_switch está habilitado (true), estamos contando a acessibilidade da posição que queremos ir */
        int qtd_caminhos = 0;

        if(l_cavalo < 0 || c_cavalo < 0) {  /* Filtrando escolhas não disponíveis */
            return MAX;
        } else if (l_cavalo >= l || (c_cavalo >= c)) {
            return MAX;
        } else if (tabuleiro[l_cavalo][c_cavalo] != 0) {
            return MAX;
        } else {
            contarMovsDois(tabuleiro, l, c, l_cavalo, c_cavalo, &qtd_caminhos); // Caso não tenha problemas, calcular a acessibilidade.
        }

        return qtd_caminhos; // Após calcular, retornar a acessibilidade da posição.
    }
}

void contarMovsDois(int tabuleiro[][MAX], int l, int c, int l_cavalo, int c_cavalo, int * qtd_caminhos) {
    /* Calcular a acesssibilidade das posições acessíveis para a posição */
    if(tabuleiro[l_cavalo - 2][c_cavalo - 1] == 0 && l_cavalo - 2 >= 0 && c_cavalo - 1 >= 0 && l_cavalo - 2 < l && c_cavalo - 1 < c) { // 0
        *qtd_caminhos = *qtd_caminhos + 1;
    }
    if(tabuleiro[l_cavalo - 1][c_cavalo - 2] == 0 && l_cavalo - 1 >= 0 && c_cavalo - 2 >= 0 && l_cavalo - 1 < l && c_cavalo - 2 < c) { // 1
        *qtd_caminhos = *qtd_caminhos + 1;
    }
    if(tabuleiro[l_cavalo + 1][c_cavalo - 2] == 0 && l_cavalo + 1 >= 0 && c_cavalo - 2 >= 0 && l_cavalo + 1 < l && c_cavalo - 2 < c) { // 2
        *qtd_caminhos = *qtd_caminhos + 1;
    }
    if(tabuleiro[l_cavalo + 2][c_cavalo - 1] == 0 && l_cavalo + 2 >= 0 && c_cavalo - 1 >= 0 && l_cavalo + 2 < l && c_cavalo - 1 < c) { // 3
        *qtd_caminhos = *qtd_caminhos + 1;
    }
    if(tabuleiro[l_cavalo - 2][c_cavalo + 1] == 0 && l_cavalo - 2 >= 0 && c_cavalo + 1 >= 0 && l_cavalo - 2 < l && c_cavalo + 1 < c) { // 4
        *qtd_caminhos = *qtd_caminhos + 1;
    }
    if(tabuleiro[l_cavalo - 1][c_cavalo + 2] == 0 && l_cavalo - 1 >= 0 && c_cavalo + 2 >= 0 && l_cavalo - 1 < l && c_cavalo + 2 < c) { // 5
        *qtd_caminhos = *qtd_caminhos + 1;
    }
    if(tabuleiro[l_cavalo + 1][c_cavalo + 2] == 0 && l_cavalo + 1 >= 0 && c_cavalo + 2 >= 0 && l_cavalo + 1 < l && c_cavalo + 2 < c) { // 6
        *qtd_caminhos = *qtd_caminhos + 1;
    }
    if(tabuleiro[l_cavalo + 2][c_cavalo + 1] == 0 && l_cavalo + 2 >= 0 && c_cavalo + 1 >= 0 && l_cavalo + 2 < l && c_cavalo + 1 < c) { // 7
        *qtd_caminhos = *qtd_caminhos + 1;
    }
    return;
}

void validezTour(int l, int c, int x, int y, int tabuleiro[][MAX]) {
    /*  Se...
            I.      a quantidade de linhas e colunas forem ímpares
            II.     l = 1, 2 ou 4
            III.    l = 3 e c = 4, 6, 8
    ... não será possível fazer o tour do cavalo    */

    if((l % 2 != 0) && (c % 2 != 0)) {  // Caso I.
        finalizarCod(1, l, c, tabuleiro);
    } else if(l == 1 || l == 2 || l == 4) { // Caso II.
        finalizarCod(2, l, c, tabuleiro);
    } else if(l == 3 && (c == 4 || c == 6 || c == 8)) { // Caso III.
        finalizarCod(2, l, c, tabuleiro);
    }
    
    if(l > MAX || c > MAX) {    // L ou C superando o máximo.
        finalizarCod(3, l, c, tabuleiro);
    }
    if(x < 0 || y < 0) {        // O cavalo iniciando em uma posição fora do tabuleiro.
        finalizarCod(4, l, c, tabuleiro);
    }
    if(x > l || y > c) {        // O cavalo iniciando em uma posição fora do tabuleiro.
        finalizarCod(4, l, c, tabuleiro);
    }
    if(l <= 0 || c <= 0) {        // L ou C tendo um valor nulo ou negativo.
        finalizarCod(5, l, c, tabuleiro);
    }

    cout << "\n" << "Cavalo iniciando em (" << x << ", " << y << ")" << "\n\n";

    return;
}

void printTabuleiro(int tabuleiro[][MAX]) {     /* Exibir o tabuleiro inteiro */
    for(int p = 0; p < 8; p++) {
        cout << "| ";
        for(int m = 0; m < 8; m++) {
            if(tabuleiro[p][m] < 10) {
                cout << " " << tabuleiro[p][m] << " | ";
            } else if (tabuleiro[p][m] == 67) {
                cout << " " << 'C' << " | ";
            } else {
                cout << tabuleiro[p][m] << " | ";
            }
        }
        cout << "\n";
    }
    cout << "\n";
}

void semEspacosDisponiveis(int l, int c, int tabuleiro[][MAX]) {    /* Verificar se não há espaços disponíveis para o cavalo ir */
    for(int i = 0; i < l; i++) {
        for(int k = 0; k < c; k++) {
            if(tabuleiro[i][k] == 0) {
                cout << "\n" << "Erro 3: Nao ha espacos validos para o cavalo ir." << "\n";
                exit(3);
            }
        }
    }
}

void finalizarCod(int codFim, int l, int c, int tabuleiro[][MAX]) { /* Encerrar o programa */
    switch(codFim) {
        case 1:
            cout << "\n" << "Erro 1: A quantidade de linhas e colunas são impares, nao eh possível fazer o tour do cavalo." << "\n";
            break;
        case 2:
            cout << "\n" << "Erro 2: O tour do cavalo nao eh possivel sob essas condicoes. Tente outros valores." << "\n";
            break;
        case 3:
            cout << "Erro 3: Tabuleiro superou o limite ([100][100]). Tente com numeros menores!\n";
            break;
        case 4:
            cout << "Erro 4: Posicao inicial esta fora do tabuleiro.\n";
            break;
        case 5:
            cout << "Erro 5: O tabuleiro tem algum valor nulo ou negativo.\n";
            break;
        case 6:
            semEspacosDisponiveis(l, c, tabuleiro);    // Se não houverem espaços disponíveis, dar o erro 3 e encerrar.
            cout << "Caminho do cavalo:\n\n";   // Caso contrário, mostrar o caminho final do cavalo e encerrar o programa.
            printTabuleiro(tabuleiro);
            exit(0);
            break;
        default:
            cout << "\n" << "Erro inesperado!" << "\n";
            break;
    }
    exit(codFim);
}