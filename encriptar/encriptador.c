#include <stdio.h>
#include <string.h>
#include <openssl/aes.h>

#define AES_BLOCK_SIZE 16

void encrypt(const unsigned char *plaintext, int plaintext_len, const unsigned char *key, unsigned char *ciphertext) {
    AES_KEY enc_key;
    AES_set_encrypt_key(key, 128, &enc_key);
    AES_encrypt(plaintext, ciphertext, &enc_key);
}

int main() {
    unsigned char key[AES_BLOCK_SIZE];  // Llave de 128 bits (16 bytes)
    const unsigned char *plaintext = (unsigned char *)"Juandagarce, contraseña";  // Texto a encriptar
    unsigned char ciphertext[AES_BLOCK_SIZE];

    // Llave de encriptación (debe ser de 128, 192 o 256 bits)
    memset(key, 0, AES_BLOCK_SIZE); // En este ejemplo la llave se inicializa con ceros, debes utilizar una llave segura y adecuada

    encrypt(plaintext, AES_BLOCK_SIZE, key, ciphertext);

    FILE *file = fopen("archivo_encriptado.bin", "wb");
    if (file == NULL) {
        printf("Error al abrir el archivo.\n");
        return 1;
    }

    fwrite(ciphertext, sizeof(unsigned char), AES_BLOCK_SIZE, file);
    fclose(file);

    printf("Texto encriptado y guardado en 'archivo_encriptado.bin'.\n");

    return 0;
}
