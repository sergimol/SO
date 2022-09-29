#include <stdio.h>
/**** TAREAS ********
 * Compila el codigo, ejecutalo y responde a las cuestiones
 * 1. ¿Por qué el primer printf() imprime valores distintos para 'a'?
 * 2. ¿Cuánto ocupa un tipo de datos char?
 * 3. ¿Por qué el valor de 'a' cambia tanto al incrementarlo en 6?
 * (la  respuesta está relacionada con la cuestión anterior)
 * 4. Si un "long" y un "double" ocupan lo mismo, ¿por qué hay 2 tipos de datos diferentes?
 */

char a = 122;
int b = 41;

int main()
{
	printf("a = %d a = %c \n", a, a);
	a += 6;
	printf("a = %d a = %c b=%d  b=%c\n", a, a, b, b);
	printf("Size of int: %lu\n", sizeof(int) );
	printf("Size of char: %lu\n", sizeof(char) );
	printf("Size of float: %lu\n", sizeof(float) );
	printf("Size of double: %lu\n", sizeof(double) );
	printf("Size of long: %lu\n", sizeof(long) );
	printf("Size of short: %lu\n", sizeof(short) );
	printf("Size of void*: %lu\n", sizeof(void*) );

}
