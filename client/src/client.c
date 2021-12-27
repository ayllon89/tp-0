#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger,"Hola, soy un log");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	ip=config_get_string_value(config,"IP");
	puerto=config_get_string_value(config,"PUERTO"); 


	// Loggeamos el valor de config

	log_info(logger,"lei la ip:%s y el puerto:%s\n",ip,puerto);

	/* ---------------- LEER DE CONSOLA ---------------- */

	//leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él
	printf("se procede a enviar cosas al servidor\n");
	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip,puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje

	valor=config_get_string_value(config,"CLAVE");


	enviar_mensaje(valor,conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	int cod_op;
	if(recv(conexion, &cod_op, sizeof(int), MSG_WAITALL) > 0)
	{} //el flag MSG_WAITALL se encarga de esperar a que llegue por socket la cantidad de bytes 
																	//que le decimos en el tercer parámetro
		//el codigo de operacion seria el mensaje serializado
	else
	{
		close(conexion);  //si el rcv da un valor negativo significa que el cliente se desconecto y cerras el socket de conexion
		return -1;
	}
		
	
	terminar_programa(conexion, logger, config); //aca se libera el socket del cliente
	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;
	if ((nuevo_logger=log_create("tp0.log","tp0",1,LOG_LEVEL_INFO))==NULL)  //te crea un archivo tp0.log donde se setean los logs
	{
		printf("no se pudo iniciar el logger");
		exit(1);
	}
	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;
	if((nuevo_config=config_create("cliente.config"))==NULL) //previamente hay que crear el archivo tp02.config
	{
		printf("no se puede crear el config");
		exit(2);
	}

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline(">ingresar linea:");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío

	// ¡No te olvides de liberar las lineas antes de regresar!

	while(strcmp(leido,"") != 0)  //me fijo que lo que ingreso el usuario no sea vacio, si es vacio salgo del while
	{
		log_info(logger,leido);
		free(leido);  //analizar si no pones free se podria producir memory leak??
		leido = readline(">ingresar linea:");
	}

	/*while(leido[0]!='\0')  //otra forma de hacerlo
	{
		log_info(logger,leido);
		free(leido);
		leido = readline(">ingresar linea:");
	}*/ 

	free(leido);

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete;

	// Leemos y esta vez agregamos las lineas al paquete

	paquete=crear_paquete();

	leido = readline(">ingresar linea a enviar:");
	while(strcmp(leido,"") != 0)  //me fijo que lo que ingreso el usuario no sea vacio, si es vacio salgo del while
	{
		agregar_a_paquete(paquete,leido, strlen(leido)+1);
		free(leido);
		leido = readline(">ingresar linea a enviar:");
	}

	enviar_paquete(paquete,conexion);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);
	eliminar_paquete(paquete);
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
		if(logger!=NULL)
	  {
		log_destroy(logger);
	  }
	  	if(conexion!=0)
	  {
		liberar_conexion(conexion);
	  }
	  	if(config!=NULL)
	  {
		config_destroy(config);
	  }

}
