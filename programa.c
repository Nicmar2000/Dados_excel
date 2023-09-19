#include <stdio.h>
#include <string.h>
#include "sqlite3.h"

int main() {
    FILE *file;
    char linha[1024];
    char *token;
    int line=0;
    int column;
    char textos[20][3][100];

    file= fopen("texto2.txt","r");

    
    while(fgets(linha, sizeof(linha), file) != NULL){
        column=0;
        token = strtok(linha,"\t");
        while (token != NULL) {
            strcpy(textos[line][column], token);
            token = strtok(NULL,"\t");
            column++;
        }
        line++;
    }

    for(int i=0; i<line; i++){
        for(int j=0; j<3; j++){
            printf("Linha %i coluna %i: %s\n", i, j, textos[i][j]);
        }
    }

    sqlite3 *db;
    char* errMsg = 0;
    int rc = sqlite3_open("seubanco.db", &db);

    if (rc) {
        fprintf(stderr, "Erro ao abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }

    const char *sql = "CREATE TABLE tabela (ID INTEGER PRIMARY KEY AUTOINCREMENT, Nome TEXT, Numero INT, endereco TEXT);";
    rc = sqlite3_exec(db, sql,0,0,0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro ao criar a tabela: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }

    for(int i=1; i<line; i++){
        char insert[200];
        sprintf(insert,"INSERT INTO tabela (Nome, Numero, endereco) VALUES ('%s','%s','%s');",textos[i][0],textos[i][1],textos[i][2]);

        rc = sqlite3_exec(db, insert, 0, 0, &errMsg);
    
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Erro na inserção: %s\n", errMsg);
            sqlite3_free(errMsg);
        } else {
            printf("Inserção bem-sucedida!\n");
        }
    }

    

    sqlite3_close(db);

    return 0;
}