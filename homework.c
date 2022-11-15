#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <limits.h>

typedef struct Date{
    int dimMatrice;
    int X, Y, Z;
    double **matrice2D;
    double **matrice2DNew;
    char **vecini2D;
    char ***vecini3D;
    double ***matrice3D;
    double ***matrice3DNew;
    int timpRulare;
} Date;
Date *dateProblema;
int P;

pthread_barrier_t copyBarrier,sumBarrier;
void getArgs(int argc, char **argv)
{
    //./homework INPUT_FILE OUTPUT_FILE NUM_THREADS
    if(argc<4){
        printf("Not enough parameters: ./homework INPUT_FILE OUTPUT_FILE NUM_THREADS");
        exit(1);
    }
    FILE *f = fopen(argv[1], "r");
    if(!f){
        printf("Could not open file\n");
        exit(1);
    }
    int i, j,k;
    char tip;
    double temperatura;
    dateProblema = malloc(sizeof(Date));
    if(!dateProblema){
        printf("Malloc failed\n");
        exit(1);
    }

    fscanf(f, "%d ", &dateProblema->dimMatrice);   
    if (dateProblema->dimMatrice == 2)
    {
        fscanf(f, "%d %d\n", &dateProblema->X,&dateProblema->Y);

        dateProblema->matrice2D = (double **)malloc((dateProblema->X + 2) * sizeof(double *));
        dateProblema->vecini2D = (char **)malloc((dateProblema->X + 2) * sizeof(char *));
        dateProblema->matrice2DNew = (double **)malloc(dateProblema->X * sizeof(double *));
        for ( i = 0; i < dateProblema->X;i++){
            dateProblema->matrice2D[i] = (double *)calloc( (dateProblema->Y+2), sizeof(double) );
            dateProblema->vecini2D[i] = (char *)malloc( (dateProblema->Y+2)* sizeof(char) );
            dateProblema->matrice2DNew[i] = (double *)malloc(dateProblema->Y * sizeof(double));
        }
        dateProblema->matrice2D[i] = (double *)calloc( (dateProblema->Y+2), sizeof(double) );
        dateProblema->vecini2D[i] = (char *)malloc( (dateProblema->Y+2)* sizeof(char) );
        dateProblema->matrice2D[i+1] = (double *)calloc( (dateProblema->Y+2), sizeof(double) );
        dateProblema->vecini2D[i+1] = (char *)malloc( (dateProblema->Y+2)* sizeof(char) );
     
        for ( j = 1; j <= dateProblema->Y;j++){
            for ( i = 1; i <= dateProblema->X;i++){
                fscanf(f,"%c %lf\n", &tip, &temperatura);
                dateProblema->matrice2D[i][j] = temperatura;
                dateProblema->vecini2D[i][j] = tip;
               // printf("(%d %d )%f %c\n", i,j,dateProblema->matrice2D[i][j], dateProblema->vecini2D[i][j]);
            }
        }
        // bordam matricea de vecini cu 'n' si bordarea matricei de valori va fi cu 0.0
        for (i = 0; i <= dateProblema->X+1;i++)//prima si ultima coloana
            dateProblema->vecini2D[i][0] = dateProblema->vecini2D[i][dateProblema->Y + 1] = 'n';
        
        for (j = 0; j <= dateProblema->Y+1;j++)
            dateProblema->vecini2D[0][j] = dateProblema->vecini2D[dateProblema->X + 1][j] = 'n';
        
    }
    else if (dateProblema->dimMatrice == 3)
    {
        fscanf(f, "%d %d %d\n", &dateProblema->X,&dateProblema->Y,&dateProblema->Z);
        
        dateProblema->matrice3D = (double ***)malloc((dateProblema->X + 2) * sizeof(double **));
        dateProblema->matrice3DNew = (double ***)malloc(dateProblema->X * sizeof(double **));
        dateProblema->vecini3D = (char ***)malloc((dateProblema->X+2) * sizeof(char **));
        for (i = 0; i < dateProblema->X; i++)
        {
            dateProblema->matrice3D[i] = (double **)malloc((dateProblema->Y + 2 ) * sizeof(double *));
            dateProblema->matrice3DNew[i] = (double **)malloc(dateProblema->Y * sizeof(double *));
            dateProblema->vecini3D[i] = (char **)malloc((dateProblema->Y + 2) * sizeof(char*));
            for (j = 0; j < dateProblema->Y; j++)
            {
                dateProblema->matrice3D[i][j] = (double *)calloc(dateProblema->Z+2,sizeof(double)  );
                dateProblema->vecini3D[i][j] = (char *)malloc((dateProblema->Z+2)*sizeof(double)  );
                dateProblema->matrice3DNew[i][j] = (double *)malloc(sizeof(double) * dateProblema->Z);
                
            }
            dateProblema->matrice3D[i][j] = (double *)calloc(dateProblema->Z+2,sizeof(double)  );
            dateProblema->vecini3D[i][j] = (char*)malloc((dateProblema->Z+2)*sizeof(char)  );
            dateProblema->matrice3D[i][j+1] = (double *)calloc(dateProblema->Z+2,sizeof(double)  );
            dateProblema->vecini3D[i][j+1] = (char* )malloc((dateProblema->Z+2)*sizeof(char)  );
        }
        dateProblema->matrice3D[i] = (double **)malloc((dateProblema->Y + 2 ) * sizeof(double *));
        dateProblema->matrice3D[i+1] = (double **)malloc((dateProblema->Y + 2 ) * sizeof(double *));
        dateProblema->vecini3D[i] = (char **)malloc((dateProblema->Y + 2 ) * sizeof(char *));
        dateProblema->vecini3D[i+1] = (char **)malloc((dateProblema->Y + 2 ) * sizeof(char *));
        for ( j = 0; j <= dateProblema->Y+1;j++){
            dateProblema->matrice3D[i][j] = (double *)calloc(dateProblema->Z+2,sizeof(double)  );
            dateProblema->matrice3D[i+1][j] = (double *)calloc(dateProblema->Z+2,sizeof(double)  );
            dateProblema->vecini3D[i][j] = (char *)malloc((dateProblema->Z+2)*sizeof(char)  );
            dateProblema->vecini3D[i+1][j] = (char *)malloc((dateProblema->Z+2)*sizeof(char)  );
        }
       
        for (k = 1; k <= dateProblema->Z;k++)
            for (j = 1; j <= dateProblema->Y;j++)
                for (i = 1; i <= dateProblema->X;i++){
                    fscanf(f,"%c %lf\n", &tip, &temperatura);
                    dateProblema->matrice3D[i][j][k] = temperatura;
                    dateProblema->vecini3D[i][j][k] = tip;
                    //printf("(%d %d %d ) %c %f \n", i, j, k, dateProblema->vecini3D[i][j][k], dateProblema->matrice3D[i][j][k]);
                }
        for (i = 0; i <= dateProblema->X + 1;i++)
            for (j = 0; j <= dateProblema->Y + 1;j++)
                dateProblema->vecini3D[i][j][0] = dateProblema->vecini3D[i][j][dateProblema->Z + 1] = 'n';
        for (j = 0; j <= dateProblema->Y + 1;j++)
            for (k= 0; k <= dateProblema->Z + 1;k++)
                dateProblema->vecini3D[0][j][k] = dateProblema->vecini3D[dateProblema->X+1][j][k] = 'n';
        for (i = 0; i <= dateProblema->X + 1;i++)
            for (k = 0; k <= dateProblema->Z + 1;k++)
                dateProblema->vecini3D[i][0][k] = dateProblema->vecini3D[i][dateProblema->Y+1][k] = 'n';
   
    }
    else
    {
        printf("Matrix should be 2D or 3D\n");
        exit(1);
    }
    fscanf(f, "%d", &dateProblema->timpRulare);
    //printf("%d \n\n", dateProblema->timpRulare);
    fclose(f);
    P = atoi(argv[3]);
}

void putArgs(char * outfile)
{
    int i, j, k;
    FILE *f = fopen(outfile, "w");
    if(!f){
        printf("Cannout open file for writing \n");
        exit(1);
    }
    if(dateProblema->dimMatrice==2){
        fprintf(f, "%d %d %d\n", dateProblema->dimMatrice, dateProblema->X, dateProblema->Y);
        for (j = 1; j <= dateProblema->Y;j++)
            for (i = 1; i <= dateProblema->X;i++)
                    fprintf(f, "%c %f\n",dateProblema->vecini2D[i][j], dateProblema->matrice2D[i][j]);
    }
    else{
         fprintf(f, "%d %d %d %d\n", dateProblema->dimMatrice, dateProblema->X, dateProblema->Y,dateProblema->Z);
         for (k = 1; k <= dateProblema->Z;k++)
             for (j = 1; j <= dateProblema->Y; j++)
                 for (i = 1; i <= dateProblema->X; i++)
                     fprintf(f,"%c %f\n", dateProblema->vecini3D[i][j][k], dateProblema->matrice3D[i][j][k]);
    }

    fclose(f);
}

int countNeighbours2D(int i,int j){
    int count = 0;
    int x, y;
    for (x = i - 1; x <= i + 1;x++)
        for (y = j - 1; y <= j + 1;y++)
        //if(!(x == i && y == j))
            if(dateProblema->vecini2D[x][y] != 'n')
                count++;
    return count;
}

double getAverage2D(int i,int j)
{
    int x, y, neighs;
    neighs= countNeighbours2D(i , j );
    // printf("(%d %d), %d\n", i, j, neighs);
    double sum = 0.0;
    if(neighs==0)
        return dateProblema->matrice2D[i][j];
    // calculam vecinii doar pentru ce e in interior, nu si border
    for (x = i - 1; x <= i + 1; x++)
        for (y = j - 1; y <= j + 1;y++)
            if (dateProblema->vecini2D[x][y] != 'n') //inseamna ca acolo e non fluid, valoarea ramane pe loc
                sum += dateProblema->matrice2D[x][y];
    // printf("%f\n", sum);
    return sum / neighs;
}
void* medie2D(void *var)
{
    int thread_id = *(int*)var;
    int start, end;
    int i, j;
    start = thread_id * ceil(dateProblema->X / P) ;
    if (thread_id == P-1)
        end = dateProblema->X;
    else
        end = fmin(dateProblema->X, (thread_id + 1) * ceil(dateProblema->X / P));
    for (int timp = 0; timp < dateProblema->timpRulare;timp++){
            for (i = start + 1; i <= end; i++)
                for (j = 1; j <= dateProblema->Y; j++)
                    if (dateProblema->vecini2D[i][j] != 'n')              ////calculam average-ul doar pentru celulele care contin fluid
                        dateProblema->matrice2DNew[i - 1][j - 1] = getAverage2D(i, j); // 0-urile sunt doar pe border
                    else
                        dateProblema->matrice2DNew[i - 1][j - 1] = dateProblema->matrice2D[i][j]; // else ramane pe loc

        pthread_barrier_wait(&sumBarrier);
        for (i = start + 1; i <= end; i++)
            for (j = 1; j <= dateProblema->Y;j++)
                dateProblema->matrice2D[i][j] = dateProblema->matrice2DNew[i - 1][j - 1];

        pthread_barrier_wait(&copyBarrier);
        
    }
    return NULL;
}

int countNeighbours3D(int i,int j,int k)
{
    int count = 0;
    int x, y,z;
    for (x = i - 1; x <= i + 1;x++)
        for (y = j - 1; y <= j + 1;y++)
            for (z = k - 1; z <= k + 1;z++)
           // if(!(x == i && y == j && z == k))
                if(dateProblema->vecini3D[x][y][z] !='n') //daca nu e border sau non-fluid
                    count++;
 
    return count;
}
double getAverage3D(int i,int j,int k)
{
    int x, y,z, neighs;
    double sum = 0.0;
    neighs= countNeighbours3D(i , j ,k);
    if (neighs ==0)
        return dateProblema->matrice3D[i][j][k];
    for (x = i - 1; x <= i + 1; x++)
        for (y = j - 1; y <= j + 1;y++)
            for (z = k - 1; z <= k + 1;z++)
                if (dateProblema->vecini3D[x][y][z] != 'n') // inseamna ca acolo e non fluid
                    sum += dateProblema->matrice3D[x][y][z];
    return sum / neighs;
}
void* medie3D(void*var)
{
    int thread_id = *(int*)var;
    
    int start, end;
    int i, j, k;
    start = thread_id * ceil(dateProblema->X / P) ;
    if (thread_id == P-1)
        end = dateProblema->X;
    else
        end = fmin(dateProblema->X, (thread_id + 1) * ceil(dateProblema->X / P));
    for (int timp = 0; timp < dateProblema->timpRulare;timp++){
        for (i = start + 1; i <= end;i++)
            for (j = 1; j <= dateProblema->Y;j++)
                for (k = 1; k <= dateProblema->Z;k++)
                    if(dateProblema->vecini3D[i][j][k] == 'n')
                        dateProblema->matrice3DNew[i - 1][j - 1][k - 1] = dateProblema->matrice3D[i][j][k];
                    else
                        dateProblema->matrice3DNew[i - 1][j - 1][k - 1] = getAverage3D(i, j, k);

        pthread_barrier_wait(&sumBarrier);
        for (i = start + 1; i <= end; i++)
            for (j = 1; j <= dateProblema->Y;j++)
                for (k = 1; k <= dateProblema->Z;k++)
                    dateProblema->matrice3D[i][j][k] = dateProblema->matrice3DNew[i - 1][j - 1][k - 1];

        pthread_barrier_wait(&copyBarrier);
    }
    return NULL;
}
void prinMatrix()
{
    if(dateProblema->dimMatrice==2){
        for (int i = 0; i <= dateProblema->X+1;i++,printf("\n"))
            for (int j = 0; j <= dateProblema->Y + 1;j++)
                printf("%f ", dateProblema->matrice2D[i][j]);
    }
    else
    {
        for (int i = 0; i <= dateProblema->X + 1;i++)
            for (int j = 0; j <= dateProblema->Y + 1;j++)
                for (int k = 0; k <= dateProblema->Z + 1;k++)
                    printf("(%d %d %d) %c %f\n", i, j, k, dateProblema->vecini3D[i][j][k], dateProblema->matrice3D[i][j][k]);
       
    }
}
int main(int argc, char **argv){
    int i,timp;
    getArgs(argc, argv);
   
    pthread_barrier_init(&copyBarrier, NULL, P);
    pthread_barrier_init(&sumBarrier, NULL, P);
    
    pthread_t tid[P];
	int thread_id[P];
	for(i = 0;i < P; i++)
		thread_id[i] = i;
    if (dateProblema->dimMatrice == 2)
        for (i = 0; i < P; i++)
            pthread_create(&(tid[i]), NULL, medie2D, &(thread_id[i]));
    else {
        for (i = 0; i < P; i++)
            pthread_create(&(tid[i]), NULL, medie3D, &(thread_id[i]));
    }
    for (i = 0; i < P; i++)
        pthread_join(tid[i], NULL);
   
    putArgs(argv[2]);
    //prinMatrix();
    pthread_barrier_destroy(&copyBarrier);
    pthread_barrier_destroy(&sumBarrier);
    return 0;
}