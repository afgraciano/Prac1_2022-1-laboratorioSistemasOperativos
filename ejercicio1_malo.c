void primerPunto(int fd) {
    char ruta[100];
    printf("\n1. Ingrese la ruta del archivo que será impreso de manera inversa.\n");
    scanf("%s", ruta);

    FILE * fp;
    char * linea = NULL;
    size_t len = 0;
    ssize_t lectura;

    fp = fopen(ruta, "r");
    if (fp == NULL) {
        perror("No se pudo abrir el archivo";
    }

    printf("\n Contenido del archivo :\n");
    while ((lectura = getline(&linea, &len, fp)) != -1) {
        printf("%s", linea);
        write_message(fd, BRIDGE_W_S, linea);
    }
    printf("\n Archivo Inverso:\n");
    read_all_messages(fd);
    printf("\n --Fin del Archivo Inverso--\n");

    fclose(fp);
    if (linea)
        free(linea);
}
