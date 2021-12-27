#include "server.h"

void atender_cliente(int* cliente_fd);

int main(void) {
	logger = log_create("log.log", "Servidor", 1, LOG_LEVEL_DEBUG);

	int server_fd = iniciar_servidor();
	log_info(logger, "Servidor listo para recibir al cliente");
	
	//int cliente_fd = esperar_cliente(server_fd);  //el programa se queda trabado aca hasta que alguien se conecte,aca se hace un accept,
												//una vez que se hace accept no se puede conectar otro cliente hasta que termine el proceso con el while de abajo porque el servidor no esta en el estado accept
	while (1) {

	int* cliente_fd = malloc(sizeof(int));
	*cliente_fd=esperar_cliente(server_fd);
	printf("la direccion de esta conexion es &cliente_fd=%p\n",cliente_fd);	
	pthread_t hilo;
	pthread_create(&hilo,NULL,(void*) atender_cliente,cliente_fd);
	pthread_detach(hilo);
		
	}
	return EXIT_SUCCESS;

}

void iterator(char* value) {
	log_info(logger,"%s", value);
}


void atender_cliente(int* cliente_fd)
{
	t_list* lista;
	while(1){
		int cod_op = recibir_operacion(*cliente_fd);  //aca recibe el socket del cliente que se conecto y se hace un recv
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(*cliente_fd);
			break;
		case PAQUETE:
			lista = recibir_paquete(*cliente_fd);
			log_info(logger, "Me llegaron los siguientes valores:\n");
			list_iterate(lista, (void*) iterator);
			break;
		case -1:
			log_error(logger, "el cliente se desconecto. Terminando servidor");
			return;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}
}