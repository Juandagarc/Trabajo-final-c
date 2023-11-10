#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>

void ingresar() {
    printf("Ingresar\n");
}

void salir() {
    printf("Salir\n");
}

void mostrarMenu(int opcion) {
    system("cls");  // Limpiar pantalla en sistemas Windows

    if (opcion == 0) {
        printf(">> Ingresar\n");
        printf("   Salir\n");
    } else {
        printf("   Ingresar\n");
        printf(">> Salir\n");
    }
}

int main() {
    int opcion = 0;
    char ch;
    printf("  _____ _     _                             _       __      __   _             _                       \n");
    printf(" / ____(_)   | |                           | |      \\ \\    / /  | |           (_)                      \n");
    printf("| (___  _ ___| |_ ___ _ __ ___   __ _    __| | ___   \\ \\  / /__ | |_ __ _  ___ _  ___  _ __   ___  ___ \n");
    printf(" \\___ \\| / __| __/ _ \\ '_ ` _ \\ / _` |  / _` |/ _ \\   \\ \\/ / _ \\| __/ _` |/ __| |/ _ \\| '_ \\ / _ \\/ __|\n");
    printf(" ____) | \\__ \\ ||  __/ | | | | | (_| | | (_| |  __/    \\  / (_) | || (_| | (__| | (_) | | | |  __/\\__ \\\n");
    printf("|_____/|_|___/\\__\\___|_| |_| |_|\\__,_|  \\__,_|\\___|     \\/ \\___/ \\__\\__,_|\\___|_|\\___/|_| |_|\\___||___/\n");
    printf("\n");
    sleep(3);

    do {
        mostrarMenu(opcion);

        ch = getch();
        switch (ch) {
            case 72:  // Flecha arriba
                opcion = (opcion - 1 + 2) % 2;
                break;
            case 80:  // Flecha abajo
                opcion = (opcion + 1) % 2;
                break;
        }
    } while (ch != 13);  // 13 es el código ASCII para la tecla Enter

    system("cls");  // Limpiar pantalla antes de salir en sistemas Windows

    switch (opcion) {
        case 0:
            ingresar();
            break;
        case 1:
            salir();
            break;
    }

    return 0;
}
