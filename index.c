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
    int votos_estudiantes;
    int votos_docentes;
    int votos_egresados;
    int votos_administrativos;
    int votos_consejo;
};

typedef struct {
    int id;
    char* role;
} User;

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

void ingresodevoto(char nombreusuario[], char ocupacion[]){
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
    //Pero ahora se hace teniendo en cuenta que la estructura es 1,Luis Fernando Gaviria Trujillo,11,votos-estudiantes,votos-docentes,votos-egresados,votos-administrativos,votos-consejo.
    while (fscanf(candidatos, "%d,%[^,],%d,%d,%d,%d,%d,%d.\n", &tarjeton, candidato.nombre, &candidato.votos, &candidato.votos_estudiantes, &candidato.votos_docentes, &candidato.votos_egresados, &candidato.votos_administrativos, &candidato.votos_consejo) == 8) {
        if (voto == tarjeton) {
            candidato.votos++;
            if (strcmp(ocupacion, "Estudiante") == 0) {
                candidato.votos_estudiantes++;
            } else if (strcmp(ocupacion, "Docente") == 0) {
                candidato.votos_docentes++;
            } else if (strcmp(ocupacion, "Egresado") == 0) {
                candidato.votos_egresados++;
            } else if (strcmp(ocupacion, "Administrativo") == 0) {
                candidato.votos_administrativos++;
            } else {
                candidato.votos_consejo++;
            }
        //    Se actualiza este archivo borrando todo y volviendo a escribirlo
            fseek(candidatos, 0, SEEK_SET);
            fprintf(candidatos, "1,Luis Fernando Gaviria Trujillo,%d,%d,%d,%d,%d,%d.\n", candidato.votos, candidato.votos_estudiantes, candidato.votos_docentes, candidato.votos_egresados, candidato.votos_administrativos, candidato.votos_consejo);
            fprintf(candidatos, "2,Carlos Alfonso Victoria Mena,%d,%d,%d,%d,%d,%d.\n", candidato.votos, candidato.votos_estudiantes, candidato.votos_docentes, candidato.votos_egresados, candidato.votos_administrativos, candidato.votos_consejo);
            fprintf(candidatos, "3,Giovanni Arias,%d,%d,%d,%d,%d,%d.\n", candidato.votos, candidato.votos_estudiantes, candidato.votos_docentes, candidato.votos_egresados, candidato.votos_administrativos, candidato.votos_consejo);
            fprintf(candidatos, "4,Xiomara Rocío González Ordoñez,%d,%d,%d,%d,%d,%d.\n", candidato.votos, candidato.votos_estudiantes, candidato.votos_docentes, candidato.votos_egresados, candidato.votos_administrativos, candidato.votos_consejo);
            fprintf(candidatos, "5,Alexander Molina Cabrera,%d,%d,%d,%d,%d,%d.\n", candidato.votos, candidato.votos_estudiantes, candidato.votos_docentes, candidato.votos_egresados, candidato.votos_administrativos, candidato.votos_consejo);
            fprintf(candidatos, "6,Juan Carlos Gutiérrez Arias,%d,%d,%d,%d,%d,%d.\n", candidato.votos, candidato.votos_estudiantes, candidato.votos_docentes, candidato.votos_egresados, candidato.votos_administrativos, candidato.votos_consejo);
            //Se borra el resto del archivo
            ftruncate(fileno(candidatos), ftell(candidatos));
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
            fprintf(usuarios, "%d.", 1); 
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
        printf("   Consejo superior\n");
        printf("   Admin\n");
        printf("   Salir\n");
    } else if (opcion == 1) {
        printf("   Ingresar\n");
        printf(">> Consejo superior\n");
        printf("   Admin\n");
        printf("   Salir\n");
    } else if (opcion == 2) {
        printf("   Ingresar\n");
        printf("   Consejo superior\n");
        printf(">> Admin\n");
        printf("   Salir\n");
    } else {
        printf("   Ingresar\n");
        printf("   Consejo superior\n");
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

void MenuConsejo(int opcion, User users[]) {
    system("clear || cls");

    printf("Seleccione una opción:\n");

    for (int i = 0; i < 9; ++i) {
        if (opcion == i) {
            printf(">> %d. %s\n", users[i].id, users[i].role);
        } else {
            printf("   %d. %s\n", users[i].id, users[i].role);
        }
    }
}


void menuUsuario(char nombreusuario[], char ocupacion[]){
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
            ingresodevoto(nombreusuario, ocupacion);
            break;
        default:
            printf("Cerrando...\n");
            break;
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
    // printf("%s", ocupacion);

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
        menuUsuario(nombreusuario, ocupacion);

    } else {
        printf("\nHubo un error al verificar datos.\n");
        printf("\nVuelva a intentarlo\n");
        sleep(2);
        ingresar(); // Reintenta el ingreso
    }
}


void VotarConsejoSuperior(char rol[]) {
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
    //Pero ahora se hace teniendo en cuenta que la estructura es 1,Luis Fernando Gaviria Trujillo,11,votos-estudiantes,votos-docentes,votos-egresados,votos-administrativos,votos-consejo. Igual el rol siempre sera Consejo_Superior
    while (fscanf(candidatos, "%d,%[^,],%d,%d,%d,%d,%d,%d.\n", &tarjeton, candidato.nombre, &candidato.votos, &candidato.votos_estudiantes, &candidato.votos_docentes, &candidato.votos_egresados, &candidato.votos_administrativos, &candidato.votos_consejo) == 8) {
        if (voto == tarjeton) {
            candidato.votos++;
            candidato.votos_consejo++;
        //    Se actualiza este archivo borrando todo y volviendo a escribirlo
            fseek(candidatos, 0, SEEK_SET);
            fprintf(candidatos, "1,Luis Fernando Gaviria Trujillo,%d,%d,%d,%d,%d,%d.\n", candidato.votos, candidato.votos_estudiantes, candidato.votos_docentes, candidato.votos_egresados, candidato.votos_administrativos, candidato.votos_consejo);
            fprintf(candidatos, "2,Carlos Alfonso Victoria Mena,%d,%d,%d,%d,%d,%d.\n", candidato.votos, candidato.votos_estudiantes, candidato.votos_docentes, candidato.votos_egresados, candidato.votos_administrativos, candidato.votos_consejo);
            fprintf(candidatos, "3,Giovanni Arias,%d,%d,%d,%d,%d,%d.\n", candidato.votos, candidato.votos_estudiantes, candidato.votos_docentes, candidato.votos_egresados, candidato.votos_administrativos, candidato.votos_consejo);
            fprintf(candidatos, "4,Xiomara Rocío González Ordoñez,%d,%d,%d,%d,%d,%d.\n", candidato.votos, candidato.votos_estudiantes, candidato.votos_docentes, candidato.votos_egresados, candidato.votos_administrativos, candidato.votos_consejo);
            fprintf(candidatos, "5,Alexander Molina Cabrera,%d,%d,%d,%d,%d,%d.\n", candidato.votos, candidato.votos_estudiantes, candidato.votos_docentes, candidato.votos_egresados, candidato.votos_administrativos, candidato.votos_consejo);
            fprintf(candidatos, "6,Juan Carlos Gutiérrez Arias,%d,%d,%d,%d,%d,%d.\n", candidato.votos, candidato.votos_estudiantes, candidato.votos_docentes, candidato.votos_egresados, candidato.votos_administrativos, candidato.votos_consejo);
            //Se borra el resto del archivo
            ftruncate(fileno(candidatos), ftell(candidatos));
            break;
        }
    }
    
    fclose(candidatos);

    FILE *usuarios;
    usuarios = fopen("consejo_superior.txt", "r+");

    printf("%s", rol);

    struct Usuario rolUsuario;

    while (fscanf(usuarios, "%*d,%[^,],%*[^,],%d.\n", rolUsuario.ocupacion, &rolUsuario.voto) == 2) {
        if (strcmp(rolUsuario.ocupacion, rol) == 0 && rolUsuario.voto == 0) {
            fseek(usuarios, -3, SEEK_CUR); 
            fprintf(usuarios, "%d.\n", 1); 
            break;
        }
    }
    fclose(usuarios);

    printf("\nVoto realizado\n");
    sleep(2);
}

void IngresoConsejoSuperior() {

    User users[9];

    // Inicializar los usuarios con los roles proporcionados
    users[0] = (User){1, "Ministro_Educacion"};
    users[1] = (User){2, "Gobernador_Risaralda"};
    users[2] = (User){3, "Miembro_Presidente"};
    users[3] = (User){4, "Representante_Directivas"};
    users[4] = (User){5, "Representante_Docentes"};
    users[5] = (User){6, "Representante_Estudiantes"};
    users[6] = (User){7, "Representante_Egresados"};
    users[7] = (User){8, "Representante_Sector_Productivo"};
    users[8] = (User){9, "Exrector_UTP"};

    int opcion = 0;
    char ch;
    do {
        MenuConsejo(opcion, users);

        ch = getch();

        if (ch == 27) {  // Comprobamos si es una secuencia de escape (código para flechas en algunas terminales)
            getch(); // Descartamos el siguiente carácter
            switch (getch()) {
                case 65:  // Flecha arriba
                    opcion = (opcion - 1 + 9) % 9;
                    break;
                case 66:  // Flecha abajo
                    opcion = (opcion + 1) % 9;
                    break;
            }
        }

        system("clear || cls");  // Limpiar pantalla en sistemas macOS o Windows

    } while (ch != 10);  //  es el código ASCII para la tecla Enter

    //dependiendo de la opcion se le asigna el rol
    char rol[50];
    strcpy(rol, users[opcion].role);

    system("clear || cls");  // Limpiar pantalla en sistemas macOS o Windows

    printf("Ingrese su contraseña: ");

    int i = 0;
    char contrasena[50];
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

    //Se busca en consejo_superior.txt el rol y la contraseña suponiendo que la estructura es id,Rol,contraseña,0.
    FILE *candidatos;
    
    candidatos = fopen("consejo_superior.txt", "r");

    // Se verifica que el rol y la contraseña sean correctos y que el ultimo numero sea 0
    //el id es la opcion + 1
    char rolconsejo[50];
    char contrasenaconsejo[50];
    // lo mas importante es que el ultimo numero sea 0
    int voto;
    while (fscanf(candidatos, "%*d,%[^,],%[^,],%d.\n", rolconsejo, contrasenaconsejo, &voto) == 3) {
        if (strcmp(rolconsejo, rol) == 0 && strcmp(contrasenaconsejo, contrasena) == 0 && voto == 0) {
            printf("\nBienvenido.\n");
            VotarConsejoSuperior(rol);
            break;
        } else if (strcmp(rolconsejo, rol) == 0 && strcmp(contrasenaconsejo, contrasena) == 0 && voto == 1) {
            printf("\nya ha votado.\n");
            break;
        }
    }
    fclose(candidatos);
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
    printf("        Candidatos: \t\t\t Votos:  Porcentaje(%%)\n");
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

    //para mac
    do {
        mostrarMenu(opcion);

        ch = getch();

        if (ch == 27) {  // Comprobamos si es una secuencia de escape (código para flechas en algunas terminales)
            getch(); // Descartamos el siguiente carácter
            switch (getch()) {
                case 65:  // Flecha arriba
                    opcion = (opcion - 1 + 4) % 4;
                    break;
                case 66:  // Flecha abajo
                    opcion = (opcion + 1) % 4;
                    break;
            }
        }

    } while (ch != 10);  //  es el código ASCII para la tecla Enter

    system("clear || cls");  // Limpiar pantalla en sistemas macOS o Windows

    switch (opcion) {
        case 0:
            ingresar();
            main();
            break;
        case 1:
            IngresoConsejoSuperior();
            main();
            break;
        case 2:
            ValidarAdmin();
            main();
            break;
        case 3:
            printf("Cerrando...\n");
            return 0;
            break;
    }

    return 0;
}