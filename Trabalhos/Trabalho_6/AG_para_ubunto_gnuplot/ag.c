#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define genes 10
#define tampopulacao 16
#define geracoes 75
#define txmut 15
#define mutrate 0.20

typedef struct individuo_
{
    float fitness;
    float cromossomo[genes];

}individuo;

individuo populacao[tampopulacao];
individuo result;

float ub[genes] = {100.0e-6, 5e-12 , 50e-6, 50e-6, 50e-6, 50e-6, 50e-6, 50e-6, 50e-6, 50e-6};
float lb[genes] = {100.0e-9, 0.1e-12, 1.5e-6, 1.5e-6, 1.5e-6, 1.5e-6, 1.5e-6, 1.5e-6, 1.5e-6, 1.5e-6};
float plot[geracoes];

void criapop ();
void avaliacao ();
void ordena ();
void cruzamento();
void mutacao();
void printpopulation();
void plotagrafico();
void printresult();

int main ()
{
    srand(time(NULL));
    result.fitness = 0;

    int g=0;

    criapop();

    while(g<geracoes)
    {
        avaliacao();
        ordena();
        plot[g]=populacao[0].fitness;
        printf("\nGeracao %d  || Melhor ganho da geracao: %f", g+1, plot[g]);
        printpopulation();
        cruzamento();
        mutacao();
        g++;
    }
    //plotagrafico();
    printresult();

    return 0;
}

void criapop()
{
    int i, j;
    for ( i=0; i<tampopulacao ;i++)
    {
        populacao[i].fitness = 0.0;

        for(j=0; j<genes; j++)
        {
            populacao[i].cromossomo[j]= lb[j] + (float) (rand()) /( (float) (RAND_MAX/(ub[j]-lb[j])));;
        }
    }
}

void avaliacao()
{
    FILE *netlist, *log;

    int i;
    float ganho;

    for (i=0; i<tampopulacao; i++)
    {
        netlist= fopen("circuito.cir", "w");
        fprintf(netlist, "*Circuit Name\n");
        fprintf(netlist, "V1 VDD 0 2.5\n");
        fprintf(netlist, "M1 VDD N001 N001 VDD P_1u l=%f w=%f\n", populacao[i].cromossomo[3], populacao[i].cromossomo[2]);
        fprintf(netlist, "M2 VDD N001 N002 VDD P_1u l=%f w=%f\n", populacao[i].cromossomo[3], populacao[i].cromossomo[2]);
        fprintf(netlist, "M3 VDD N001 Vout VDD P_1u l=%f w=%f\n", populacao[i].cromossomo[3], populacao[i].cromossomo[2]);
        fprintf(netlist, "M4 N002 0 N004 N002 P_1u l=%f w=%f\n", populacao[i].cromossomo[5],populacao[i].cromossomo[4]);
        fprintf(netlist, "M5 N002 vi+ N003 N002 P_1u l=%f w=%f\n", populacao[i].cromossomo[5],populacao[i].cromossomo[4]);
        fprintf(netlist, "M6 N004 N004 VSS VSS N_1u l=%f w=%f\n", populacao[i].cromossomo[7], populacao[i].cromossomo[6]);
        fprintf(netlist, "M7 N003 N004 VSS VSS N_1u l=%f w=%f\n", populacao[i].cromossomo[7], populacao[i].cromossomo[6]);
        fprintf(netlist, "M8 Vout N003 VSS VSS N_1u l=%f w=%f\n", populacao[i].cromossomo[9], populacao[i].cromossomo[8]);
        fprintf(netlist, "C1 Vout N003 %f\n", populacao[i].cromossomo[1]);
        fprintf(netlist, "I1 N001 VSS %f\n", populacao[i].cromossomo[0]);
        fprintf(netlist, "V2 VSS 0 -2.5\n");
        fprintf(netlist, "C2 0 Vout 1p\n");
        fprintf(netlist, "V3 Vi+ 0 AC 1\n");
        fprintf(netlist, ".model NMOS NMOS\n");
        fprintf(netlist, ".model PMOS PMOS\n");
        fprintf(netlist, ".ac oct 100 1 1meg\n");
        fprintf(netlist, ".include cmosedu_models.txt\n");
        fprintf(netlist, ".meas AC ganho FIND v(vout) AT 1\n");
        fprintf(netlist, ".end");

        fclose(netlist);

        //system("ltspice -b circuito.cir");
        system("C:\\Users\\PCC\\Desktop\\Topicos_Especiais_1\\Programas\\LTC\\LTspiceXVII\\XVIIx64.exe -b C:\\Users\\PCC\\Desktop\\Nova_pasta\\circuito.cir");

        log = fopen("circuito.log", "r");

        char line[250], meas[8];

        while (!feof(log))
        {
            fgets(line, 249, log);

            if (strstr(line, "ganho") != NULL)
                {
                    strncpy(meas, &line[16], 7);
                    meas[7]='\0';
                    ganho = atof(meas);
                    populacao[i].fitness = ganho;
                }
        }
        fclose(log);
    }
}

void ordena()
{
    int i, j;
    individuo a;

    for (i=0;i<tampopulacao-1;i++)
    {
        for(j=0;j<tampopulacao-i-1;j++)
        {
            if(populacao[j+1].fitness > populacao[j].fitness)
            {
                    a = populacao[j+1];
                    populacao[j+1] = populacao[j];
                    populacao[j] = a;
            }
        }
    }
    if (populacao[0].fitness > result.fitness)
	result = populacao[0];
}

void cruzamento()
{
    individuo auxiliar[tampopulacao];

    int i, j;

    for (i=0;i<tampopulacao;i++)
    {
        auxiliar[i] = populacao[i];
    }

    for (i=0;i<tampopulacao;i++)
    {
        for (j=0;j<genes/2;j++)
        {
            populacao[i].cromossomo[j]=auxiliar[i].cromossomo[j];
            populacao[i+1].cromossomo[j]=auxiliar[i+1].cromossomo[j];
        }
        for (j=genes/2;j<genes;j++)
        {
            populacao[i].cromossomo[j]=auxiliar[i+1].cromossomo[j];
            populacao[i+1].cromossomo[j]=auxiliar[i].cromossomo[j];
        }
        populacao[i].fitness = 0;
        populacao[i+1].fitness = 0;
    	i++;
    }

}

void mutacao()
{
    int i, j;

    for (i=0;i<tampopulacao;i++)
    {
        for (j=0;j<genes;j++)
        {
            if (rand()%100 < txmut){

                if(rand()%2)
                {
                    populacao[i].cromossomo[j]*= (1-mutrate);
                    if(populacao[i].cromossomo[j] < lb[j])
                        populacao[i].cromossomo[j] = lb[j];
                }
                else{
                    populacao[i].cromossomo[j]*= (1+mutrate);
                    if(populacao[i].cromossomo[j] > ub[j])
                        populacao[i].cromossomo[j] = ub[j];
                }
            }
        }
    }
}

void plotagrafico()
{
    int NUM_COMMANDS = 2;
    char * commandsForGnuplot[] = {"set title \"ganho por geracao\"", "plot 'data.temp' with linespoints ls 1"};
    FILE * temp = fopen("data.temp", "w");
    /*Abre uma interface que recebe linhas de comando para o gnuplot
     */
    //FILE * gnuplotPipe = popen ("gnuplot -persistent", "w"); //linux
    FILE * gnuplotPipe = popen ("c:\\gnuplot\\bin\\gnuplot.exe -persistent", "w"); //windows
    int i;

    for (i=0; i < geracoes; i++)
    {
    fprintf(temp, "%d %lf \n", i, plot[i]); //escreve o arquivo a ser lido pelo gnuplot
    }

    for (i=0; i < NUM_COMMANDS; i++)
    {
    fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[i]); //manda os comandos para o gnuplot, um a um.
    }

}

void printpopulation()
{
    int i, j;

    for (i=0;i<tampopulacao;i++)
    {
        printf("\nIndividuo %d: Ganho do individuo: %f\n", i+1, populacao[i].fitness);

        for (j=0;j<genes;j++)
        {
            if (j == 1)
            printf(" %.15lf", populacao[i].cromossomo[j]);
            else
            printf(" %.10lf", populacao[i].cromossomo[j]);
        }
    }
}

void printresult()
{
    system ("clear");
    printf ("\n\n Melhor Ganho: %.10lf", result.fitness);
    printf ("\n Parametros:");
    printf ("\n            I: %.10lf", result.cromossomo[0]);
    printf ("\n            C: %.15lf", result.cromossomo[1]);
    printf ("\n            W1: %.10lf", result.cromossomo[2]);
    printf ("\n            W2: %.10lf", result.cromossomo[3]);
    printf ("\n            W3: %.10lf", result.cromossomo[4]);
    printf ("\n            L1,2,3: %.10lf", result.cromossomo[5]);
    printf ("\n            W4,5: %.10lf", result.cromossomo[6]);
    printf ("\n            L4,5: %.10lf", result.cromossomo[7]);
    printf ("\n            W6,7: %.10lf", result.cromossomo[8]);
    printf ("\n            L6,7: %.10lf", result.cromossomo[9]);
    printf ("\n            W8: %.10lf", result.cromossomo[10]);
    printf ("\n            L8: %.10lf", result.cromossomo[11]);
}
