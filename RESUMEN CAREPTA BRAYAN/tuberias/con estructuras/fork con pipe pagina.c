#include <sys/types.h>
#include <wait.h>
#include <unistd.h>
#include <stdio.h>

/*
 * Programa principal.
 * Se crea una tuber�a (pipe) y un proceso hijo. Luego el padre transmite la 
 * cadena de caracteres "Hola" al proceso hijo a trav�s de la tuber�a.
 */
main()
{
	/* Identificador del proceso hijo creado */
	pid_t idProceso;

	/* Valor devuelto por el proceso hijo cuando termine */
	int estadoHijo;

	/* Descriptores para la tuberia. El 0 ser� de entrada y el 1 de salida */
	int descriptorTuberia[2];

	/* Buffer para escribir o leer caracteres de la tuberia */
	char buffer[100];

	/* Antes de llamar al fork(), Se crea una tuberia, con dos descriptores, 
	 * uno de entrada y otro de salida */
	if (pipe (descriptorTuberia) == -1)
	{
		perror ("No se puede crear Tuberia");
		exit (-1);
	}
	
	/* Se crea el proceso hijo. En alg�n sitio dentro del fork(), nuestro
	 * programa se duplica en dos procesos. Cada proceso obtendr� una salida
	 * distinta. */
	idProceso = fork();

	/* Si fork() devuelve -1, es que hay un error y no se ha podido crear el
	 * proceso hijo. */
	if (idProceso == -1)
	{
		perror ("No se puede crear proceso");
		exit (-1);
	}

	/* Proceso hijo:
	 * fork() devuelve 0 al proceso hijo.*/
	if (idProceso == 0)
	{
		/* Cierra el descriptor de escritura puesto que no lo va a usar. */
		close (descriptorTuberia[1]);

		/* Lee 5 caracteres del descriptor de lectura, escribe el resultado en
		 * pantalla y sale */
		read (descriptorTuberia[0], buffer, 5);
		printf ("Hijo  : Recibido \"%s\"\n", buffer);
		exit (0);
	}

	/* Proceso padre: 
	 * fork() devuelve un n�mero positivo al padre. Este n�mero es el id del
	 * hijo. */
	if (idProceso > 0)
	{
		/* Cierra el descriptor de lectura, puesto que no lo va a usar */
		close (descriptorTuberia[0]);

		/* Escribe "Hola" por el descriptor de escritura */
		printf ("Padre : Envio \"Hola\"\n");
		strcpy (buffer, "Hola");
		write (descriptorTuberia[1], buffer, strlen(buffer)+1);

		/* Espera que el hijo muera y sale*/
		wait (&estadoHijo);
		exit (0);
	}
}
