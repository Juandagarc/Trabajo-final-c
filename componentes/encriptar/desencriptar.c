#include <stdio.h>
#include <string.h>
#include <openssl/aes.h>
#include <stdlib.h>

#define AES_BLOCK_SIZE 16

void decrypt(const unsigned char *ciphertext, int ciphertext_len, const unsigned char *key, unsigned char *plaintext) {
    AES_KEY dec_key;
    AES_set_decrypt_key(key, 128, &dec_key);
    AES_decrypt(ciphertext, plaintext, &dec_key);
}

int main() {
    unsigned char key[AES_BLOCK_SIZE];  // Llave de 128 bits (16 bytes)
    unsigned char ciphertext[AES_BLOCK_SIZE];
    unsigned char decrypted_text[AES_BLOCK_SIZE];

    // Llave de desencriptación (debe ser la misma que se usó para encriptar)
    memset(key, 0, AES_BLOCK_SIZE); // Debes usar la misma clave que se utilizó para encriptar

    FILE *file = fopen("archivo_encriptado.bin", "rb");
    if (file == NULL) {
        printf("Error al abrir el archivo encriptado.\n");
        return 1;
    }

    fread(ciphertext, sizeof(unsigned char), AES_BLOCK_SIZE, file);
    fclose(file);

    decrypt(ciphertext, AES_BLOCK_SIZE, key, decrypted_text);

    FILE *output_file = fopen("archivo_desencriptado.txt", "w");
    if (output_file == NULL) {
        printf("Error al crear el archivo desencriptado.\n");
        return 1;
    }

    fprintf(output_file, "%s", decrypted_text);
    
    // Agregar la secuencia de números al final del archivo
    fprintf(output_file, "%s", "1866");

    fclose(output_file);

    printf("Texto desencriptado y guardado en 'archivo_desencriptado.txt'.\n");

    return 0;
}
