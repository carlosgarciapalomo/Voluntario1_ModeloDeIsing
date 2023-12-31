
// EJERCICIO VOLUNTARIO DE ISING (Voluntario 1) hecho por Carlos García Palomo
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

 static const int N= 16;
using namespace std;
void ConfiguracionOrdenadaPositiva(int s[][N]);
double Energia (int i, int j, int M[][N]);
double FuncionE_S (int s[][N]);
double FuncionMagnetizacion(int M[][N]);
double FuncionCorrelacion ( int s[][N], int indice);
double Incertidumbre (double vector[10]);

double Incertidumbre (double vector [10]){
    double media=0.0;
    double suma=0.0;
    for (int i=0;i<10; i++){
        media=media+vector[i];
    }
    media=media/10.0 ;
    for (int j=0;j<10; j++ ){
        suma=suma+pow(vector[j]-media,2);
    }

    return 1.96*sqrt(suma)/10.0;
}


double FuncionE_S (int s[][N]){

double E;
int lim1,lim2,lim3,lim4;
     for (int i = 0; i < N; i++) {
     

      for (int j=0; j<N;j++){
       lim1=j+1; 
       lim2=j-1; 
       lim3=i+1;
       lim4=i-1;
       if(j==N-1)
       {
         lim1=0;
                }
       if(i==N-1)
       {
         lim3=0;
                }
       if(i==0)
       {
         lim4=N-1;
                }

       if(j==0)
       {
         lim2=N-1;
                }

       E = E + s[i][j]*(s[i][lim1]+s[i][lim2]+s[lim3][j]+s[lim4][j]);




      }
        

    }
    return -0.5*E;
}

//Función que calcule  el valor absoluto de la suma de los espines
 
 double FuncionMagnetizacion(int M[][N]) {
   double sumatorio = 0.0;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            sumatorio= sumatorio + M[i][j];
        }
    }

    return abs(sumatorio)/(N*N);
 }
double FuncionCorrelacion ( int s[][N], int indice){

double correlacion=0.0;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
             if(i<N-indice)
             {
                correlacion=correlacion+s[i][j]*s[i+indice][j];
             }
            else
            {
                correlacion=correlacion+s[i][j]*s[i+indice-N][j];
            }
            
        }
        
    }
    return correlacion/(1.0*N*N);   

}


int main()
{
  int indice=3; // INDICE PARA LA FUNCION CORRELACION
  int n,m; // (n,m) son las posiciones aleatorias de la red
  double num1,num2; // VARIABLE AUXILIAR PARA ALMACENAR UN NUMERO
  int s[N][N];
  double Probabilidad; //Variable en la que almacenaremos la probabilidad
  double Temperatura; //Vaiable en la que almaceno la temperatura
  long long int ciclos=10e3;
  int medidas=0; //CADA VEZ QUE SE CALCULAN LAS MAGNITUDES VA SUMANDOSE 1
  double EnergiaMedia, Magnetizacion, Correlacion, CalorEspecifico, EnergiaMediaAuxiliar,EnergiaAlCuadrado;
  double M_T[10],E_T[10], Calor_T[10],Correla_T[10], Temperature[10];

        Magnetizacion=0.0;
 
        EnergiaMedia=0.0;
        
        EnergiaAlCuadrado=0.0;

        Correlacion=0.0;


 
  int h; //Este indice sirve para que solo escribamos pasos de 100 en 100
  srand(time(NULL));
  int config;

  // Abrimos los archivos donde escribiremos las magnitudes
    std::ofstream archivo_Magnetizacion("Magnetizacion.txt");
    archivo_Magnetizacion.precision(16); // definir la precisión del archivo de salida

    std::ofstream archivo_EnergiaMedia("EnergiaMedia.txt");
    archivo_EnergiaMedia.precision(16); // definir la precisión del archivo de salida

    std::ofstream archivo_CalorEspecifico("CalorEspecifico.txt");
    archivo_CalorEspecifico.precision(16); // definir la precisión del archivo de salida

     std::ofstream archivo_Correlacion("Correlacion.txt");
    archivo_Correlacion.precision(16); // definir la precisión del archivo de salida

  Temperatura=1.7; //ESTA ES LA TEMPERATURA POR LA QUE SE EMPIEZA
  for (int l=0;l<10;l++){


  //ELEGIMOS UNA CONFIGURACIÓN INICIAL ORDENADA
  ConfiguracionOrdenadaPositiva (s);


    h=0; //INICIAMOS EL PARÁMETRO h a cero

    //ALGORITMO DE MONTECARLO 
    for (long long int j=0; j < ciclos; j++)
    {
          for(int k=0;k<N*N;k++) { 
            //NUMEROS ALEATORIOS (STEP 1)
            n=rand() % N;
            m=rand() % N;

            //EVALUAR PROBABILIDAD (STEP 2)
           num1=exp(-1.0*Energia(n,m,s) /Temperatura);
           if (num1<1) Probabilidad=num1;
           else Probabilidad=1.0;
            
            //GENERACION DEL NUMERO AL AZAR Y COMPROBAR SI QUEDA DENTRO DE LA PROBABILIDAD 
           num2 =(double) rand() / RAND_MAX;
           if (num2<Probabilidad)
           s[n][m]=-1*s[n][m];
       }
         
        
     if(h==1)
     {
        Magnetizacion=Magnetizacion+FuncionMagnetizacion(s);
 
        EnergiaMedia=EnergiaMedia+FuncionE_S(s);
        
        EnergiaAlCuadrado=EnergiaAlCuadrado+pow(FuncionE_S(s),2);

        Correlacion=Correlacion+FuncionCorrelacion(s,indice);

        medidas=medidas+1;
        
        
     
     }
     else {
        h=h+1;
     }

    }
    EnergiaMediaAuxiliar=EnergiaMedia/(2.0*medidas);
    EnergiaAlCuadrado=EnergiaAlCuadrado/(1.0*medidas);
    CalorEspecifico=(EnergiaAlCuadrado-pow(EnergiaMediaAuxiliar,2))/(1.0*N*N*Temperatura);
    Magnetizacion=Magnetizacion/(1.0*medidas);
    EnergiaMedia=EnergiaMedia/(2.0*N*medidas);
    Correlacion=Correlacion/(1.0*medidas);

    M_T[l]=Magnetizacion;
    Calor_T[l]=CalorEspecifico;
    E_T[l]=EnergiaMedia;
    Correla_T[l]=Correlacion;
    Temperature[l]=Temperatura;

   Temperatura=Temperatura+0.15;
    }

    for(int p=0; p<10;p++)
    {
    archivo_Magnetizacion<< Temperature[p] << " " <<  M_T[p] << " " << Incertidumbre(M_T) << std::endl;
    archivo_EnergiaMedia<< Temperature[p]<< " " << Calor_T[p] << " " << Incertidumbre(Calor_T) << std::endl;
    archivo_CalorEspecifico<< Temperature[p] << " " << E_T[p] << " " << Incertidumbre(E_T) << std::endl;
    archivo_Correlacion<< Temperature[p]<< " " <<  Correla_T[p]<< " " << Incertidumbre(Correla_T) << std::endl;

    }

    // Cerramos los archivos donde hemos escrito las magnitudes
    archivo_Magnetizacion.close();
    archivo_EnergiaMedia.close();
    archivo_CalorEspecifico.close();
     archivo_Correlacion.close();
   cout << medidas ;



 
  

  return 0;
}

//FUNCION QUE INICIALIZA LA RED 


//FUNCION QUE INICIALIZA LOS ESPINES A 1
void ConfiguracionOrdenadaPositiva(int s[][N])
{
  
int i,j;
    
    //Inicialización de las condiciones de contorno
    for(j=0;j<N;j++)
    {
        
            s[0][j]=1;
            s[N-1][j]=1;
     
    }

    for(i=1;i<N;i++)
    {
       
            s[i][0]=1;
            s[i][N-1]=1;
    }

    //inicialización del resto de la red
    for (i=1;i<(N-1);i++)
        for (j=1;j<(N-1);j++)
        {
                s[i][j]=1;
        }

    return;

}



double Energia (int i, int j, int M[][N])
{
    double E;
    if ((i != N - 1) && (j != N - 1) && (i != 0) && (j != 0))
        {
            E = (2.0) * (M[i][j]) * ((M[i + 1][j]) + (M[i - 1][j]) + (M[i][j + 1]) + (M[i][j - 1]));
        }
        else
        {
            if ((i == N - 1) && (j != N - 1) && (j != 0))
            {
                E = (2.0) * (M[i][j]) * ((M[0][j]) + (M[i - 1][j]) + (M[i][j + 1]) + (M[i][j - 1]));
            }
            else
            {
                if ((i != N - 1) && (j == N - 1) && (i != 0))
                {
                    E = (2.0) * (M[i][j]) * ((M[i + 1][j]) + (M[i - 1][j]) + (M[i][0]) + (M[i][j - 1]));
                }
                else
                {
                    if ((j != N - 1) && (i == 0) && (j != 0))
                    {
                        E = (2.0) * (M[i][j]) * ((M[i + 1][j]) + (M[N - 1][j]) + (M[i][j + 1]) + (M[i][j - 1]));
                    }
                    else
                    {
                        if ((i != N - 1) && (i != 0) && (j == 0))
                        {
                            E = (2.0) * (M[i][j]) * ((M[i + 1][j]) + (M[i - 1][j]) + (M[i][j + 1]) + (M[i][N - 1]));
                        }
                        else
                        {
                            if ((i == 0) && (j == 0))
                            {
                                E = (2.0) * (M[i][j]) * ((M[i + 1][j]) + (M[N - 1][j]) + (M[i][j + 1]) + (M[i][N - 1]));
                            }
                            else
                            {
                                if ((i == N - 1) && (j == N - 1))
                                {
                                    E = (2.0) * (M[i][j]) * ((M[0][j]) + (M[i - 1][j]) + (M[i][0]) + (M[i][j - 1]));
                                }
                                else
                                {
                                    if ((i == 0) && (j == N - 1))
                                    {
                                        E = (2.0) * (M[i][j]) * ((M[i + 1][j]) + (M[N - 1][j]) + (M[i][0]) + (M[i][j - 1]));
                                    }
                                    else
                                    {
                                        E = (2.0) * (M[i][j]) * ((M[0][j]) + (M[i - 1][j]) + (M[i][j + 1]) + (M[i][N - 1]));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        return E;
}



