/*
 * main.c
 *
 *  Created on: 28 feb. 2019
 *      Author: utnso
 */

#include "tp0.h"


int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/
	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	logger = iniciar_logger();

	//Loggear "soy un log"
	log_info(logger, "soy un log");

	config = leer_config();

	//asignar valor de config a la variable valor
	valor = config_get_string_value(config, "CLAVE");

	//Loggear valor de config
	log_info(logger, valor);

	leer_consola(logger);


	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	//antes de continuar, tenemos que asegurarnos que el servidor esté corriendo porque lo necesitaremos para lo que sigue.

	//crear conexion
	conexion = crear_conexion(config_get_string_value(config,"IP"), config_get_string_value(config,"PUERTO"));

	//enviar CLAVE al servirdor
	enviar_mensaje(valor,conexion);

	paquete(conexion);

	terminar_programa(conexion, logger, config);
}

t_log* iniciar_logger(void)
{
	return log_create("tp0.log", "TP0", 1, LOG_LEVEL_INFO);//con 0 lo guarda solo en el archivo y con 1 lo guarda en la consola tmb
}

t_config* leer_config(void)
{
	return config_create("tp0.config");
}

void leer_consola(t_log* logger)
{
	char* leido;

	//El primero te lo dejo de yapa
	leido = readline(">");

	while(strcmp(leido, "") != 0) //pq strcmp retorna 0 cuando los dos valores son iguales
	{
		log_info(logger, leido);
		free(leido);
		leido = readline(">");
	}

	free(leido);

}


void paquete(int conexion)
{
	//Ahora toca lo divertido!

	char* leido;
	t_paquete* paquete = crear_super_paquete();

	leido = readline(">");

	while(strcmp(leido, "") != 0)//itera mientras no encuentre una linea vacia
	{
		agregar_a_paquete(paquete, leido, strlen(leido)+1);//+1 pq no te esta contando el /0 y necesitamos que lo cuente
		free(leido);
		leido = readline(">");
	}

	enviar_paquete(paquete,conexion);
	free(leido);

	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	//Y por ultimo, para cerrar, hay que liberar lo que utilizamos (conexion, log y config) con las funciones de las commons y del TP mencionadas en el enunciado
	liberar_conexion(conexion);
	log_destroy(logger);
	config_destroy(config);
}
