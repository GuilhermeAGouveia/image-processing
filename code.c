#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
//int rotate(int **m);

int menu(void);
int binarizar(int matriz[1000][1000], int nL, int nC, int n);
int brilho(int matriz[1000][1000], int nL, int nC, float n);
int iconizar(int matriz[1000][1000], int nL, int nC);
int filtro(int matriz[1000][1000], int nL, int nC);
int escurecerBordas(int matriz[1000][1000], int nL, int nC);

int main(void){
    int nC, nL, i, j, opcao;
    bool invalid = true;
    char c, temp[30], nameFile[20], caminho[50];
    FILE *arq;
    float n;
    while(true){
        arq = fopen("./input.pgm", "r");
        while (!arq){
            puts("Erro ao abrir o arquivo!");
            printf("Digite o caminho de uma outra imagem ou dê CTRL+C para abortar: ");
            scanf("%s", caminho);
            arq = fopen(caminho, "r");
        }
        fscanf(arq, "%s", temp);
        fscanf(arq, "%s", temp);
        fscanf(arq, "%d %d", &nC, &nL);
        fseek(arq, 4, SEEK_CUR);
        int matrizImage[1000][1000];
        for (i = 0; i < nL; i++){
            for(j = 0; j < nC; j++){
                fscanf(arq, "%d", &matrizImage[i][j]);
            }
        }
        fclose(arq);
        printf("(%dx%d)", nL, nC);
        do {
            opcao = menu();
            puts("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
            switch (opcao)
            {
                case 1:
                    puts("\n\n------------------Brilho----------------------\n");
                    puts("FATOR > 1 => clarear");
                    puts("0 < FATOR < 1 => escurecer\n");
                    invalid = false;
                    do {
                        printf("Insira um fator de brilho > ");
                        scanf("%f", &n);
                    } while (n < 0);
                    brilho(matrizImage, nL, nC, n);
                    break;
                case 2:
                    puts("\n\n------------------Binarizar-------------------\n\n");
                    invalid = false;
                    do {
                        printf("Insira um valor para N > ");
                        scanf("%f", &n);
                    } while (n < 0 || n > 255);
                    binarizar(matrizImage, nL, nC, n);
                
                    break;
                case 3:
                    puts("\n\n------------------Iconizar--------------------\n\n");
                    invalid = false;
                    iconizar(matrizImage, nL, nC);
                    nL = nC = 64;
                    break;
                case 4:
                    puts("\n\n------------------Escurecer Bordas------------\n\n");
                    invalid = false;
                    escurecerBordas(matrizImage, nL, nC);
                    break;
                case 5:
                    puts("\n\n--------------------Filtro--------------------\n\n");
                    invalid = false;
                    filtro(matrizImage, nL, nC);
                    break;
                case 6:
                    exit(0);
                    break;
                default:
                        puts("Opção invalida!");
                        c = getchar();
                    break;
            }
        } while (invalid);
        puts("\nDeseja salvar a imagem?(y/n)");
        getchar(); //por algum motivo o primeiro getchar passava em branco, é como se ele capturasse alguma tecla em buffer teclada anteriormente, a solução foi colocar dois seguidos.
        c = getchar();
        if( c == 'y' ){
            printf("Digite um nome para a imagem: ");
            scanf("%s", nameFile);
            sprintf(temp, "./%s.pgm",nameFile);
            arq = fopen(temp, "w+");
            fputs("P2\n", arq); 
            fputs("#Guilherme_Augusto_Gouveia\n",arq);
            fprintf(arq, "%d %d\n", nC, nL);
            fprintf(arq, "%d\n", 255);
            for (i = 0; i < nL; i++){
                for (j = 0; j < nC; j++){
                    fprintf(arq, "%d ", matrizImage[i][j]);
                }
                fputs("\n", arq);
            }
            fclose(arq);
            puts("Imagem Salva!");
            puts("Pressione ENTER para continuar...");
            getchar();
            getchar();
        }
        puts("\n\n\n\n\n\n\n");
    }
    return 0;
}

int menu(void){
    int op;
    puts("-----------------------Menu de Opções------------------------------");
    puts("\n1)Escurecer ou Clarear");
    puts("2)Binarizar");
    puts("3)Gerar Icone");
    puts("4)Escurecer Bordas");
    puts("5)Eliminar Ruídos(Filtro Passa Baixa)");
    puts("\n6)Sair");
    printf("\n\nDigite uma opção > ");
    scanf("%d", &op);
    return op;
}

int binarizar(int matriz[1000][1000], int nL, int nC, int n){
    
    for (int i = 0; i < nL; i++){
        for(int j = 0; j < nC; j++){
            matriz[i][j] = matriz[i][j] >= n ? 255 : 0;
        }
    }
    return 0;
}

int brilho(int matriz[1000][1000], int nL, int nC, float n){
    int value;
    for (int i = 0; i < nL; i++){
        for(int j = 0; j < nC; j++){
            value = matriz[i][j] * n;
            if (value > 255) {value = 255;}
            matriz[i][j] = value;
        }
    }
    return 0;
}

int escurecerBordas(int matriz[1000][1000], int nL, int nC){
    int n = 16;
    int layer = n;
    int fator = 3;
    int k = 0;
    int auxValue;
    //border-top
    for (int i = layer; i >= 0; i--){
        layer--;
        if (layer < 0) layer = 0;
        for(int j = layer; j < nC - layer; j++){
            auxValue =  matriz[i][j] - k;
            matriz[i][j] = auxValue < 0 ? 0 : auxValue;
        }
        k = k + fator;
    }
    //border-bottom
    layer = n;
    k = 0;
    for (int i = nL - layer; i < nL; i++){
        layer--;
        if (layer < 0) layer = 0;
        for(int j = layer; j < nC - layer; j++){
            auxValue =  matriz[i][j] - k;
            matriz[i][j] = auxValue < 0 ? 0 : auxValue;
        }
        k = k + fator;
    }
    
    //border-right
    k = 0;
    layer = n + 1;
    for(int j = nC - layer; j < nC; j++){
        layer--;
        if (layer < 0) layer = 0;
        for (int i = layer + 1; i < nL - layer; i++){
            auxValue =  matriz[i][j] - k;
            matriz[i][j] = auxValue < 0 ? 0 : auxValue;
        }
        k = k + fator;
    }
  
    //border-left
    layer = n;
    k = 0;
    for(int j = layer; j >= 0; j--){
        layer--;
        if (layer < 0) layer = 0;
        for (int i = layer; i < nL - layer; i++){
            auxValue =  matriz[i][j] - k;
            matriz[i][j] = auxValue < 0 ? 0 : auxValue;
        }
        k = k + fator;
    }
    return 0;
}

int iconizar(int matriz[1000][1000], int nL, int nC){
    int iL = nL / 64;
    int iC = nC / 64;
    long int somatorio, elementos;
    int m, n;
    m = n = -1;
    somatorio = 0;
    for (int i = 0; i <= nL;i += iL){
        m++;
        n = -1;
        for (int j = 0; j <= nC;j += iC){
            n++;
            elementos = 0;
            somatorio = 0;
            for (int k = i; k < (i + iL); k++){
                for (int y = j; y < (j + iC); y++){
                    somatorio += matriz[k][y];
                    elementos++;
                }
            }
            long int media = (int) somatorio/elementos;
            matriz[m][n] = media;
        }
    }
    return 0;
}

int filtro(int matriz[1000][1000], int nL, int nC){
    int filtro[3][3] = {
        1,2,1,
        2,4,2,
        1,2,1
    };
    
    int somatorio;
    int m,n;
    for ( int i = 1; i < nL - 1; i++){
        for (int j = 1; j < nC - 1; j++){
            somatorio = 0;
            m = -1;
            for (int x = i - 1; x <= (i + 1); x++){
                ++m;
                n = -1;
                for (int y = j - 1; y <= (j + 1); y++){
                    ++n;
                    somatorio += matriz[x][y] * filtro[m][n];
                }
            }
            matriz[i][j] = somatorio / 16;
        }
    }
    return 0;
}
