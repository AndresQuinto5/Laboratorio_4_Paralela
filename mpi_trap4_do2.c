#include <stdio.h>
#include <mpi.h>

void Build_mpi_type(double* a_p, double* b_p, int* n_p,
      MPI_Datatype* input_mpi_t_p);

void Get_input(int my_rank, int comm_sz, double* a_p, double* b_p,
      int* n_p);

double Trap(double left_endpt, double right_endpt, int trap_count, 
   double base_len);    

double f(double x); 

int main(void) {
   int my_rank, comm_sz, n, local_n;   
   double a, b, h, local_a, local_b;
   double local_int, total_int;
   double start_time, end_time, elapsed_time;

   MPI_Init(NULL, NULL);

   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

   Get_input(my_rank, comm_sz, &a, &b, &n);

   if (my_rank == 0) {
        start_time = MPI_Wtime(); // Mover esta línea aquí
    }

   h = (b-a)/n;          
   local_n = n/comm_sz; 

   local_a = a + my_rank*local_n*h;
   local_b = local_a + local_n*h;
   local_int = Trap(local_a, local_b, local_n, h);

   MPI_Reduce(&local_int, &total_int, 1, MPI_DOUBLE, MPI_SUM, 0,
         MPI_COMM_WORLD);

   if (my_rank == 0) {
      end_time = MPI_Wtime(); // Mover esta línea aquí
      printf("With n = %d trapezoids, our estimate\n", n);
      printf("of the integral from %f to %f = %.15e\n",
          a, b, total_int);
      elapsed_time = end_time - start_time; // Calcular el tiempo transcurrido
      printf("Elapsed time: %f seconds\n", elapsed_time); // Imprimir el tiempo transcurrido
   }

   MPI_Finalize();


   return 0;
} 
void Build_mpi_type(
      double*        a_p            /* in  */, 
      double*        b_p            /* in  */, 
      int*           n_p            /* in  */,
      MPI_Datatype*  input_mpi_t_p  /* out */) {

   int array_of_blocklengths[3] = {1, 1, 1};
   // Cambia el orden de los elementos en array_of_types a {b, n, a}
   MPI_Datatype array_of_types[3] = {MPI_DOUBLE, MPI_INT, MPI_DOUBLE};
   MPI_Aint array_of_displacements[3] = {0};
   MPI_Aint a_addr, b_addr, n_addr;

   MPI_Get_address(a_p, &a_addr);
   MPI_Get_address(b_p, &b_addr);
   MPI_Get_address(n_p, &n_addr);

   // Modifica el orden de los desplazamientos para coincidir con {b, n, a}
   array_of_displacements[0] = b_addr - a_addr;
   array_of_displacements[1] = n_addr - a_addr;
   array_of_displacements[2] = 0;

   MPI_Type_create_struct(3, array_of_blocklengths, 
         array_of_displacements, array_of_types,
         input_mpi_t_p);

   MPI_Type_commit(input_mpi_t_p);
}

void Get_input(
      int      my_rank  /* in  */, 
      int      comm_sz  /* in  */, 
      double*  a_p      /* out */, 
      double*  b_p      /* out */,
      int*     n_p      /* out */) {

   MPI_Datatype input_mpi_t;
   Build_mpi_type(a_p, b_p, n_p, &input_mpi_t);

   if (my_rank == 0) {
      printf("Enter b, n, and a\n");
      scanf("%lf %d %lf", b_p, n_p, a_p);
   }

   // Empaqueta y envía los valores de a, b y n utilizando la estructura creada
   MPI_Bcast(a_p, 1, input_mpi_t, 0, MPI_COMM_WORLD);

   MPI_Type_free(&input_mpi_t);
}


double Trap(
      double left_endpt  /* in */, 
      double right_endpt /* in */, 
      int    trap_count  /* in */, 
      double base_len    /* in */) {
   double estimate, x; 
   int i;

   estimate = (f(left_endpt) + f(right_endpt))/2.0;
   for (i = 1; i <= trap_count-1; i++) {
      x = left_endpt + i*base_len;
      estimate += f(x);
   }
   estimate = estimate*base_len;

   return estimate;
} 

double f(double x /* in */) {
   return x*x;
}