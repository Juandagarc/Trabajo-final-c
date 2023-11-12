//Presentado por: Juan David García Arce y Maximiliano Giraldo Ocampo

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>

char getch() {
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return (buf);
}
#endif

struct Usuario
{
    char nombre[50];
    char contrasena[50];
    char correo[50];
    char ocupacion[50];
    int voto;
};

struct Candidato
{
    int tarjeton;
    char nombre[50];
    int votos;
};

bool validarCredenciales(char correo[], char contrasena[], char ocupacion[], char nombreusuario[]) {
    FILE *usuarios;
    usuarios = fopen("usuarios.txt", "r");
    
    if (usuarios == NULL) {
        printf("Error al abrir el archivo de usuarios.\n");
        return false;
    }
    
    struct Usuario usuario;

    
    while (fscanf(usuarios, "%[^,],%[^,],%[^,],%[^,],%d.%*c\n",
                  usuario.nombre, usuario.contrasena, usuario.correo, usuario.ocupacion, &usuario.voto) == 5) {
        if (strcmp(usuario.correo, correo) == 0 && strcmp(usuario.contrasena, contrasena) == 0 && strcmp(usuario.ocupacion, ocupacion) == 0) {
            if (usuario.voto == 0) {
                printf("\n%s", usuario.nombre);
                strcpy(nombreusuario, usuario.nombre);
                fclose(usuarios);
                return true; 
            }
            else
            {
                printf("\nSolo puede votar una vez\n");
                fclose(usuarios);
                return false; 
            }
            
        }
    }
    
    fclose(usuarios);
    return false; 
}

void ingresodevoto(char nombreusuario[]){
    FILE *candidatos;
    candidatos = fopen("candidatos.txt", "r+");

    int voto;
    int tarjeton;

    struct Candidato candidato;

    printf("\tCANDIDATOS\n");
    printf("\n1. Luis Fernando Gaviria Trujillo");
    printf("\n2. Carlos Alfonso Victoria Mena");
    printf("\n3. Giovanni Arias");
    printf("\n4. Xiomara Rocío González Ordoñez");
    printf("\n5. Alexander Molina Cabrera");
    printf("\n6. Juan Carlos Gutiérrez Arias");

    printf("\nIngrese el numero de su candidato:\t");
    scanf("%d", &voto);
   while (fscanf(candidatos, "%d,%[^,],%d.\n", &tarjeton, candidato.nombre, &candidato.votos) == 3) {
        if (voto == tarjeton) {
            candidato.votos++;
            fseek(candidatos, -4, SEEK_CUR);
            fprintf(candidatos, "%d.\n", candidato.votos);
            break;
        }
    }

    fclose(candidatos);

    FILE *usuarios;
    usuarios = fopen("usuarios.txt", "r+");

    printf("%s", nombreusuario);

    struct Usuario usuario;

    // Encuentra al usuario y marca que ya votó
    while (fscanf(usuarios, "%[^,],%*[^,],%[^,],%*[^,],%d.\n", usuario.nombre, usuario.correo, &usuario.voto) == 3) {
        if (strcmp(usuario.nombre, nombreusuario) == 0 && usuario.voto == 0) {
            fseek(usuarios, -4, SEEK_CUR); 
            fprintf(usuarios, "%d.\n", 1); 
            break;
        }
    }

    fclose(usuarios);

    printf("\nVoto realizado\n");
}



void mostrarMenu(int opcion) {
    system("clear || cls");  // Limpiar pantalla en sistemas macOS o Windows
    printf("Bienvenido al sistema de votaciones:\n");
    if (opcion == 0) {
        printf(">> Ingresar\n");
        printf("   Admin\n");
        printf("   Salir\n");
    } else if (opcion == 1) {
        printf("   Ingresar\n");
        printf(">> Admin\n");
        printf("   Salir\n");
    } else {
        printf("   Ingresar\n");
        printf("   Admin\n");
        printf(">> Salir\n");
    }
}

void menugrupo( int opcion){
    system("clear || cls");  // Limpiar pantalla en sistemas macOS o Windows
    printf("Ingrese el grupo al que pertenece: \n");
    if (opcion == 0) {
        printf(">> Administrativo\n");
        printf("   Docente\n");
        printf("   Egresado\n");
        printf("   Estudiante\n");
    } else if (opcion == 1) {
        printf("   Administrativo\n");
        printf(">> Docente\n");
        printf("   Egresado\n");
        printf("   Estudiante\n");
    } else if (opcion == 2) {
        printf("   Administrativo\n");
        printf("   Docente\n");
        printf(">> Egresado\n");
        printf("   Estudiante\n");
    } else {
        printf("   Administrativo\n");
        printf("   Docente\n");
        printf("   Egresado\n");
        printf(">> Estudiante\n");
    }
}

void MenuVotar (int opcion) {
    system("clear || cls");  // Limpiar pantalla en sistemas macOS o Windows
    printf("\nQue desea hacer?\n");
    if (opcion == 0) {
        printf(">> Votar\n");
        printf("   Salir\n");
    } else {
        printf("   Votar\n");
        printf(">> Salir\n");
    }
}


void menuUsuario(char nombreusuario[]){
    int op = 0;
    char ch;
    do {
        MenuVotar(op);

        ch = getch();

        if (ch == 27) {  // Comprobamos si es una secuencia de escape (código para flechas en algunas terminales)
            getch(); // Descartamos el siguiente carácter
            switch (getch()) {
                case 65:  // Flecha arriba
                    op = (op - 1 + 2) % 2;
                    break;
                case 66:  // Flecha abajo
                    op = (op + 1) % 2;
                    break;
            }
        }

        system("clear || cls");  // Limpiar pantalla en sistemas macOS o Windows

    } while (ch != 10);  //  es el código ASCII para la tecla Enter
    switch (op){
        case 0:
            ingresodevoto(nombreusuario);
            break;
        default:
            printf("Cerrando...\n");
            break;
    }
}

void MenuAdmin (int opcion) {
    system("clear || cls");  // Limpiar pantalla en sistemas macOS o Windows
    if (opcion == 0) {
        printf(">> Ver votos\n");
        printf("   Ver histograma\n");
        printf("   Salir\n");
    } else if (opcion == 1) {
        printf("   Ver votos\n");
        printf(">> Ver histograma\n");
        printf("   Salir\n");
    } else {
        printf("   Ver votos\n");
        printf("   Ver histograma\n");
        printf(">> Salir\n");
    }
}

void ingresar() {
    char contrasena[50];
    char correo[50];
    char ocupacion[50];
    int g = 0;
    char ch;
    do {
        menugrupo(g);

        ch = getch();

        if (ch == 27) {  // Comprobamos si es una secuencia de escape (código para flechas en algunas terminales)
            getch(); // Descartamos el siguiente carácter
            switch (getch()) {
                case 65:  // Flecha arriba
                    g = (g - 1 + 4) % 4;
                    break;
                case 66:  // Flecha abajo
                    g = (g + 1) % 4;
                    break;
            }
        }

        system("clear || cls");  // Limpiar pantalla en sistemas macOS o Windows

    } while (ch != 10);  //  es el código ASCII para la tecla Enter
    switch (g){
        case 0:
            strcpy(ocupacion, "Administrativo");
            break;
        case 1:
            strcpy(ocupacion, "Docente");
            break;  
        case 2:
            strcpy(ocupacion, "Egresado");
            break;
        default:
            strcpy(ocupacion, "Estudiante");
    }

    //muestra la ocupacion
    printf("%s", ocupacion);

    printf("Ingrese su correo: ");
    scanf("%s", correo);

    printf("Ingrese su contraseña: ");

    int i = 0;
    while (i < 49) {
        char ch = getch();

        if (ch == '\n' || ch == '\r') {
            contrasena[i] = '\0';
            break;
        } else if (ch == 8) {
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            contrasena[i] = ch;
            i++;
            printf("*");
        }
    }

    char nombreusuario[50];

    if (validarCredenciales(correo, contrasena, ocupacion, nombreusuario)) {
        printf("\nBienvenido.\n");
        menuUsuario(nombreusuario);
    } else {
        printf("\nHubo un error al verificar datos.\n");
        printf("\nVuelva a intentarlo\n");
        sleep(2);
        ingresar(); // Reintenta el ingreso
    }
}

void titulo () {
    printf("  _____ _     _                             _       __      __   _             _                       \n");
    printf(" / ____(_)   | |                           | |      \\ \\    / /  | |           (_)                      \n");
    printf("| (___  _ ___| |_ ___ _ __ ___   __ _    __| | ___   \\ \\  / /__ | |_ __ _  ___ _  ___  _ __   ___  ___ \n");
    printf(" \\___ \\| / __| __/ _ \\ '_ ` _ \\ / _` |  / _` |/ _ \\   \\ \\/ / _ \\| __/ _` |/ __| |/ _ \\| '_ \\ / _ \\/ __|\n");
    printf(" ____) | \\__ \\ ||  __/ | | | | | (_| | | (_| |  __/    \\  / (_) | || (_| | (__| | (_) | | | |  __/\\__ \\\n");
    printf("|_____/|_|___/\\__\\___|_| |_| |_|\\__,_|  \\__,_|\\___|     \\/ \\___/ \\__\\__,_|\\___|_|\\___/|_| |_|\\___||___/\n");
    printf("\n");
    sleep(1);
}

void VerVotos () {
    FILE *candidatos;
    candidatos = fopen("candidatos.txt", "r");

    struct Candidato candidato;

    int votos[6] = {0, 0, 0, 0, 0, 0};
    //si la estructura es 1,Luis Fernando Gaviria Trujillo,0. el while seria
    while (fscanf(candidatos, "%d,%[^,],%d.\n", &candidato.tarjeton, candidato.nombre, &candidato.votos) == 3) {
        votos[candidato.tarjeton - 1] = candidato.votos;
    }

    fclose(candidatos);

    //Se halla los votos totales
    float votostotales = 0;
    for (int i = 0; i < 6; i++) {
        votostotales += votos[i];
    }

    printf("\n");
    printf("        Candidatos: \t\t Votos: \t Porcentaje(%%)\n");
    printf("1. Luis Fernando Gaviria Trujillo: \t %d \t %f\n", votos[0], votos[0] * 100 / votostotales);
    printf("2. Carlos Alfonso Victoria Mena: \t %d \t %f\n", votos[1], votos[1] * 100 / votostotales);
    printf("3. Giovanni Arias: \t\t\t %d \t %f\n", votos[2], votos[2] * 100 / votostotales);
    printf("4. Xiomara Rocío González Ordoñez: \t %d \t %f\n", votos[3], votos[3] * 100 / votostotales);
    printf("5. Alexander Molina Cabrera: \t\t %d \t %f\n", votos[4], votos[4] * 100 / votostotales);
    printf("6. Juan Carlos Gutiérrez Arias: \t %d \t %f\n", votos[5], votos[5] * 100 / votostotales);

    printf("\n");

}

//Ahora se genera un histograma con los votos
void histograma () {
    FILE *candidatos;
    candidatos = fopen("candidatos.txt", "r");

    struct Candidato candidato;

    int votos[6] = {0, 0, 0, 0, 0, 0};
    //si la estructura es 1,Luis Fernando Gaviria Trujillo,0. el while seria
    while (fscanf(candidatos, "%d,%[^,],%d.\n", &candidato.tarjeton, candidato.nombre, &candidato.votos) == 3) {
        votos[candidato.tarjeton - 1] = candidato.votos;
    }

    fclose(candidatos);

    //Se halla los votos totales
    float votostotales = 0;
    for (int i = 0; i < 6; i++) {
        votostotales += votos[i];
    }
    printf("\n");
    printf("Histograma de votos:\n");
    printf("1. Luis Fernando Gaviria Trujillo: \t");
    for (int i = 0; i < votos[0]; i++) {
        printf("*");
    }
    printf("\n");
    printf("2. Carlos Alfonso Victoria Mena: \t");
    for (int i = 0; i < votos[1]; i++) {
        printf("*");
    }
    printf("\n");
    printf("3. Giovanni Arias: \t\t\t");
    for (int i = 0; i < votos[2]; i++) {
        printf("*");
    }
    printf("\n");
    printf("4. Xiomara Rocío González Ordoñez: \t");
    for (int i = 0; i < votos[3]; i++) {
        printf("*");
    }
    printf("\n");
    printf("5. Alexander Molina Cabrera: \t\t");
    for (int i = 0; i < votos[4]; i++) {
        printf("*");
    }
    printf("\n");
    printf("6. Juan Carlos Gutiérrez Arias: \t");
    for (int i = 0; i < votos[5]; i++) {
        printf("*");
    }
    printf("\n");
}

void Admin() {

    int op = 0;
    char ch;
    do {
        MenuAdmin(op);

        ch = getch();

        if (ch == 27) {  // Comprobamos si es una secuencia de escape (código para flechas en algunas terminales)
            getch(); // Descartamos el siguiente carácter
            switch (getch()) {
                case 65:  // Flecha arriba
                    op = (op - 1 + 3) % 3;
                    break;
                case 66:  // Flecha abajo
                    op = (op + 1) % 3;
                    break;
            }
        }

        system("clear || cls");  // Limpiar pantalla en sistemas macOS o Windows

    } while (ch != 10);  //  es el código ASCII para la tecla Enter
    switch (op){
        case 0:
            VerVotos();
            printf("Presione cualquier tecla para continuar...\n");
            getch();
            Admin();
            break;
        case 1:
            histograma();
            printf("Presione cualquier tecla para continuar...\n");
            getch();
            Admin();
            break;
        default:
            printf("Cerrando...\n");
            break;
    }
}

void ValidarAdmin() {
    //Valida el usuario y la contraseña del admin
    char contrasena[50];
    char usuario[50];
    printf("Ingrese su usuario: ");
    scanf("%s", usuario);

    printf("Ingrese su contraseña: ");
    
    int i = 0;
    while (i < 49) {
        char ch = getch();

        if (ch == '\n' || ch == '\r') {
            contrasena[i] = '\0';
            break;
        } else if (ch == 8) {
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            contrasena[i] = ch;
            i++;
            printf("*");
        }
    }

    //busca en admin.txt el usuario y la contraseña suponiendo que la estructura es administrador,1234
    FILE *admin;
    admin = fopen("admin.txt", "r");

    char usuarioadmin[50];
    char contrasenaadmin[50];

    fscanf(admin, "%[^,],%[^,].\n", usuarioadmin, contrasenaadmin);

    fclose(admin);

    if (strcmp(usuario, usuarioadmin) == 0 && strcmp(contrasena, contrasenaadmin) == 0) {
        printf("\nBienvenido.\n");
        Admin();
    } else {
        printf("\nHubo un error al verificar datos.\n");
        sleep(2);
    }
}


int main() {
    titulo();
    int opcion = 0;
    printf("Ingrese una opción: ");

    char ch;

    //cuando este en windows cambiar el 27 por 224
    // do {
    //     mostrarMenu(opcion);

    //     ch = getch();
    //     switch (ch) {
    //         case 72:  // Flecha arriba
    //             opcion = (opcion - 1 + 2) % 2;
    //             break;
    //         case 80:  // Flecha abajo
    //             opcion = (opcion + 1) % 2;
    //             break;
    //     }
    // } while (ch != 13);  // 13 es el código ASCII para la tecla Enter

    //para mac
    do {
        mostrarMenu(opcion);

        ch = getch();

        if (ch == 27) {  // Comprobamos si es una secuencia de escape (código para flechas en algunas terminales)
            getch(); // Descartamos el siguiente carácter
            switch (getch()) {
                case 65:  // Flecha arriba
                    opcion = (opcion - 1 + 3) % 3;
                    break;
                case 66:  // Flecha abajo
                    opcion = (opcion + 1) % 3;
                    break;
            }
        }

    } while (ch != 10);  //  es el código ASCII para la tecla Enter

    system("clear || cls");  // Limpiar pantalla en sistemas macOS o Windows

    switch (opcion) {
        case 0:
            ingresar();
            break;
        case 1:
            ValidarAdmin();
            main();
            break;
        case 2:
            printf("Cerrando...\n");
            return 0;
            break;
    }

    return 0;
}