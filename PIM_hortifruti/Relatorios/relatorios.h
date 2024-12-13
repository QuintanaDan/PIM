#ifndef RELATORIOS_H
#define RELATORIOS_H

typedef struct {
    char tipo[20];
    char filtro[100];
    char dataInicio[11];
    char dataFim[11];
} FiltroRelatorio;

typedef struct {
    float totalGeral;
    int numeroRegistros;
} ResumoRelatorio;

// Funções auxiliares existentes
int compararDatas(const char* data1, const char* data2);
int validarFormatoData(const char* data);
void quebrarData(const char* data, int* dia, int* mes, int* ano);
long dataParaNumero(const char* data);
int dataNoIntervalo(const char* data, const char* inicio, const char* fim);
void obterFiltro(FiltroRelatorio *filtro, int tipoRelatorio);
void obterDataValida(const char *prompt, char *data);
void gerarNomeArquivo(const char *prefixo, const char *tipoRelatorio, char *nomeArquivo);
void salvarRelatorio(const char *nomeArquivo, const char *cabecalho, const char *conteudo);
void gerarRelatorioVendas();
void gerarRelatorioCompras();
void menuRelatorios();

#endif
